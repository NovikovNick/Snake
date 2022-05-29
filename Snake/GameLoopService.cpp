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

	GameState gameState = { snakeHead, {settings.startFoodXCoord, settings.startFoodYCoord } };

	GameStateHolder gameStateHolder = GameStateHolder(&gameState);

    do {

		std::vector<Input> inputs = _inputService->popInputs();

		GameState* nextGameState = gameStateHolder.ApplyForces(inputs, settings);
		_gameLogicService->applyForcesAndCheck(nextGameState, inputs, settings);
		_renderService->render(&gameStateHolder, settings);

		// int64_t delay = (settings.initialSpeedMs - settings.maxSpeedMs) / settings.scoreToWin * (settings.scoreToWin - gameState.score + 1);
		int64_t delay = settings.initialSpeedMs;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));

    } while (_running);
}

} // namespace snake
