#ifndef SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOOP_SERVICE_H_


#include <thread>
#include <iostream>

#include "GameModels.h"
#include "RenderService.h"
#include "InputService.h"
#include "GameLogicService.h"
#include "AIService.h"


namespace snake {


class GameLoopService {
public:
	void start();
	void stop();
	GameLoopService(std::shared_ptr<InputService> inputService,
		std::shared_ptr<RenderService> renderService,
		std::shared_ptr<GameLogicService> gameLogicService,
		std::shared_ptr<AIService> aiService) {

		inputService_ = inputService;
		renderService_ = renderService;
		gameLogicService_ = gameLogicService;
		aiService_ = aiService;
	}

private:

	std::shared_ptr<InputService> inputService_;
	std::shared_ptr<RenderService> renderService_;
	std::shared_ptr<GameLogicService> gameLogicService_;
	std::shared_ptr<AIService> aiService_;

	std::thread _renderThread;
	bool _running = false;
	std::chrono::duration<double, std::milli> _nanos_60fps = std::chrono::duration<double, std::milli>(16);


	void _startGameLoop();
};

} // namespace snake

#endif // SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
