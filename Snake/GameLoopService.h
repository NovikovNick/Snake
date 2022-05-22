#pragma once

#include "GameModels.h"
#include "RenderService.h"
#include "InputService.h"

#include <thread>
#include <iostream>

class GameLoopService {
public:
	void start();
	void stop();
	GameLoopService(InputService* inputService,
					RenderService* renderService) {

		_inputService = inputService;
		_renderService = renderService;
	}

private:
	
	InputService* _inputService;
	RenderService* _renderService;

	std::thread _renderThread;
	bool _running = false;
	std::chrono::duration<double, std::milli> _nanos_60fps = std::chrono::duration<double, std::milli>(16);


	void _startGameLoop();
};
