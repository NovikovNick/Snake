#ifndef SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOOP_SERVICE_H_


#include <thread>
#include <iostream>

#include "GameModels.h"
#include "RenderService.h"
#include "InputService.h"
#include "GameLogicService.h"
#include "GameStateHolder.h"
#include "AIService.h"


namespace snake {


class GameLoopService {
public:
	void start();
	void stop();
	GameLoopService(InputService* inputService,
		RenderService* renderService,
		GameLogicService* gameLogicService,
		AIService* aiService) {

		_inputService = inputService;
		_renderService = renderService;
		_gameLogicService = gameLogicService;
		_aiService = aiService;
	}

private:

	InputService* _inputService;
	RenderService* _renderService;
	GameLogicService* _gameLogicService;
	AIService* _aiService;

	std::thread _renderThread;
	bool _running = false;
	std::chrono::duration<double, std::milli> _nanos_60fps = std::chrono::duration<double, std::milli>(16);


	void _startGameLoop();
};

} // namespace snake

#endif // SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
