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

	GameState* gameState = new GameState(0);
	gameState->snake_head[0] = new SnakePart();
	gameState->snake_head[1] = new SnakePart();

	InitPlayer(gameState->snake_head[0], settings);
	InitEnemy(gameState->snake_head[1], settings);

	gameState->food = { settings.startFoodXCoord, settings.startFoodYCoord };

	GameStateHolder holder = GameStateHolder(gameState);

	bool paused = false;
	int64_t delay = settings.initialSpeedMs;
	float progress = 0;
	std::vector<Input> inputs[2];
	GameState* nextGameState;

	std::vector<DebugContext> debugCtx;
	debugCtx.resize(32);

    do {
		// log("Start loop");
		inputs[0] = _inputService->popInputs();
		

		if (!inputs[0].empty()) {

			switch (inputs[0].front().command) {
			case SystemCommand::PAUSE:
				paused = !paused;
				holder.ClearOffset();
				break;

			case SystemCommand::STEP_FORWARD:
				holder.StepForward();
				break;

			case SystemCommand::STEP_BACKWARD:
				holder.StepBackward();
				break;
			default:
				break;
			}
		}

		// log("Fetching...");
		GamePhase gamePhase = holder.GetState(holder.GetFrame())->gamePhase;

		if (gamePhase == WIN || gamePhase == LOSE) {
			paused = true;
		}

		// log("Calculating...");

		if (paused) {
		
			nextGameState = holder.GetStateWithOffset();
			DebugContext ctx = debugCtx[(holder.GetFrame() + holder.GetOffset()) % holder.GetCapacity()];

			for (auto it : ctx.pathfinding) {
				_renderService->BeginDraw();
				_renderService->render(nextGameState, &holder, settings);
				_renderService->render(it);
				_renderService->EndDraw();
				//std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1));
			}

		} else {

			InputDTO botInput = _aiService->getInputs(holder.GetState(holder.GetFrame()), settings);
			inputs[1] = botInput.inputs;

			nextGameState = holder.ApplyForces(inputs, settings);
			debugCtx[holder.GetFrame() % holder.GetCapacity()] = botInput.ctx;
			

			_gameLogicService->check(nextGameState, settings);

			_renderService->BeginDraw();
			_renderService->render(nextGameState, &holder, settings);
			_renderService->EndDraw();
		}

		progress = (nextGameState->score[0] / (settings.scoreToWin / 100.0f)) / 100.0f;
		delay = paused ? 15 : settings.maxSpeedMs + (settings.initialSpeedMs - settings.maxSpeedMs) * (1 - progress);

		log("End loop");

		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));

    } while (_running);
}

} // namespace snake
