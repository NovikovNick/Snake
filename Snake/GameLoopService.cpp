#include "GameLoopService.h"

namespace snake {


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

	SnakePart* snakeHead = new SnakePart
	{ 
		{
			settings.startPlayedXCoord, 
			settings.startPlayedYCoord
		},
		settings.startPlayedDirection, 
		NULL 
	};

	SnakePart* snakePart = snakeHead;
	for (int i = 1; i < settings.startLenght; i++)
	{
		snakePart->next = new SnakePart
		{
			{
				settings.startPlayedDirection == RIGHT ? settings.startPlayedXCoord - i : settings.startPlayedXCoord + i,
				settings.startPlayedYCoord
			},
			settings.startPlayedDirection,
			NULL
		};

		snakePart = snakePart->next;
	}

	GameState gameState = { 0, snakeHead, {settings.startFoodXCoord, settings.startFoodYCoord } };

	GameStateHolder gameStateHolder = GameStateHolder(&gameState);

	bool paused = false;
	int frameOffset = 0;
	int64_t delay = settings.initialSpeedMs;


    do {

		std::vector<Input> inputs = _inputService->popInputs();

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
				int threshold = gameStateHolder.GetFrame() <= gameStateHolder.GetCapacity()
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


		GamePhase gamePhase = gameStateHolder.GetState(gameStateHolder.GetFrame())->gamePhase;

		if (gamePhase == WIN || gamePhase == LOSE) {
			paused = true;
		}

		GameState* nextGameState = paused 
			? gameStateHolder.GetState(gameStateHolder.GetFrame() + frameOffset)
			: gameStateHolder.ApplyForces(inputs, settings);

		// todo backward and insert new inputs

		_gameLogicService->applyForcesAndCheck(nextGameState, inputs, settings);
		_renderService->render(*nextGameState, &gameStateHolder, settings);

		float progress = (nextGameState->score / (settings.scoreToWin / 100.0f)) / 100.0f;
		delay = paused ? 15 : settings.maxSpeedMs + (settings.initialSpeedMs - settings.maxSpeedMs) * (1 - progress);

		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));

    } while (_running);
}

} // namespace snake
