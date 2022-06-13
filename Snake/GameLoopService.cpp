#include "GameLoopService.h"

namespace snake {

namespace {

	void InitPlayer(SnakePart* snakeHead, GameSettigs settings) {
		
		snakeHead->coord = { settings.startPlayedXCoord, settings.startPlayedYCoord };
		snakeHead->direction = settings.startPlayedDirection;
		snakeHead->next = nullptr;

		SnakePart* snakePart = snakeHead;
		for (int i = 1; i < settings.startLenght; i++)
		{

				snakePart->next = new SnakePart();

				snakePart->next->coord = {
					settings.startPlayedDirection == Direction::RIGHT 
					? settings.startPlayedXCoord - i 
					: settings.startPlayedXCoord + i,

					settings.startPlayedYCoord
				};
				snakePart->next->direction = settings.startPlayedDirection;
				snakePart->next->next = nullptr;

			snakePart = snakePart->next;
		}
	}

	void InitEnemy(SnakePart* snakeHead, GameSettigs settings) {


		int enemyStartPlayedXCoord = 10;
		int enemyStartPlayedYCoord = 13;
		int enemyLenght = 3;
		Direction startEnemyDirection = Direction::RIGHT;

		snakeHead->coord = { enemyStartPlayedXCoord, enemyStartPlayedYCoord };
		snakeHead->direction = startEnemyDirection;
		snakeHead->next = nullptr;

		SnakePart* snakePart = snakeHead;
		for (int i = 1; i < enemyLenght; i++)
		{

			snakePart->next = new SnakePart();

			snakePart->next->coord = {
				startEnemyDirection == Direction::RIGHT
				? enemyStartPlayedXCoord - i
				: enemyStartPlayedXCoord + i,

				enemyStartPlayedYCoord
			};
			snakePart->next->direction = startEnemyDirection;
			snakePart->next->next = nullptr;

			snakePart = snakePart->next;
		}
	}

}

void GameLoopService::start() {
	_running = true;
	_renderThread = std::thread(&GameLoopService::_startGameLoop, this);
}

void GameLoopService::stop() {
	_running = false;
	_renderThread.join();
}

void GameLoopService::_startGameLoop() {

	GameSettigs settings = {};

	SnakePart* snakeHead = new SnakePart();

	InitPlayer(snakeHead, settings);

	GameState* gameState = new GameState();
	gameState->frame = 0;
	gameState->snake_head[0] = new SnakePart();
	gameState->snake_head[1] = new SnakePart();

	InitPlayer(gameState->snake_head[0], settings);
	InitEnemy(gameState->snake_head[1], settings);

	gameState->food = { settings.startFoodXCoord, settings.startFoodYCoord };

	GameStateHolder gameStateHolder = GameStateHolder(gameState);

	bool paused = false;
	int64_t delay = settings.initialSpeedMs;
	float progress = 0;
	std::vector<Input> inputs[2];
	GameState* nextGameState;

    do {
		log("Start loop");
		inputs[0] = _inputService->popInputs();
		inputs[1] = _aiService->getInputs(gameStateHolder.GetState(gameStateHolder.GetFrame()), settings);

		if (!inputs[0].empty()) {

			switch (inputs[0].front().command) {
			case SystemCommand::PAUSE:
				paused = !paused;
				gameStateHolder.ClearOffset();
				break;

			case SystemCommand::STEP_FORWARD:
				gameStateHolder.StepForward();
				break;

			case SystemCommand::STEP_BACKWARD:
				gameStateHolder.StepBackward();
				break;
			default:
				break;
			}
		}

		log("Fetching...");
		GamePhase gamePhase = gameStateHolder.GetState(gameStateHolder.GetFrame())->gamePhase;

		if (gamePhase == WIN || gamePhase == LOSE) {
			paused = true;
		}
		
		log("Calculating...");
		nextGameState = paused
			? gameStateHolder.GetStateWithOffset()
			: gameStateHolder.ApplyForces(inputs, settings);

		std::cout << nextGameState->frame << std::endl;


		log("Checking...");
		_gameLogicService->check(nextGameState, settings);

		log("Rendering...");
		_renderService->render(nextGameState, &gameStateHolder, settings);

		progress = (nextGameState->score[0] / (settings.scoreToWin / 100.0f)) / 100.0f;
		delay = paused ? 15 : settings.maxSpeedMs + (settings.initialSpeedMs - settings.maxSpeedMs) * (1 - progress);

		log("End loop");

		if (!paused) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));
		}

    } while (_running);
}

} // namespace snake
