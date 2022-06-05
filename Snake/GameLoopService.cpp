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
					settings.startPlayedDirection == RIGHT ? settings.startPlayedXCoord - i : settings.startPlayedXCoord + i,
					settings.startPlayedYCoord
				};
				snakePart->next->direction = settings.startPlayedDirection;
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
	gameState->snake_head = snakeHead;
	gameState->food = { settings.startFoodXCoord, settings.startFoodYCoord };

	//GameState gameState = { 0, &snakeHead, };

	GameStateHolder gameStateHolder = GameStateHolder(gameState);

	bool paused = false;
	int frameOffset = 0;
	int64_t delay = settings.initialSpeedMs;
	float progress = 0;
	std::vector<Input> inputs;
	GameState* nextGameState;
	int threshold;

    do {
		log("Start loop");
		inputs = _inputService->popInputs();

		if (!inputs.empty()) {

			switch (inputs.front().command) {
			case PAUSE:
				paused = !paused;
				frameOffset = 0;
				break;
			case STEP_FORWARD:
				if (++frameOffset > 0) {
					frameOffset = 0;
				}
				break;
			case STEP_BACKWARD:
			{
				threshold = gameStateHolder.GetFrame() <= gameStateHolder.GetCapacity()
					? gameStateHolder.GetFrame()
					: gameStateHolder.GetCapacity() - 1;

				if (--frameOffset < -threshold) {
					frameOffset = -threshold;
				}
				break;
			}
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
			? gameStateHolder.GetState(gameStateHolder.GetFrame() + frameOffset)
			: gameStateHolder.ApplyForces(inputs, settings);

		std::cout << nextGameState->frame << std::endl;

		// todo backward and insert new inputs

		log("Checking...");
		_gameLogicService->applyForcesAndCheck(nextGameState, inputs, settings);
		

		log("Rendering...");
		_renderService->render(nextGameState, &gameStateHolder, settings);

		progress = (nextGameState->score / (settings.scoreToWin / 100.0f)) / 100.0f;
		delay = paused ? 15 : settings.maxSpeedMs + (settings.initialSpeedMs - settings.maxSpeedMs) * (1 - progress);

		log("End loop");

		if (!paused) {
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));
		}

    } while (_running);
}

} // namespace snake
