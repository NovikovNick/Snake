#include "GameLoopService.h"


void GameLoopService::start() {
	_running = true;
	_renderThread = std::thread(&GameLoopService::_startGameLoop, this);
}

void GameLoopService::stop() {
	_running = false;
	_renderThread.join();
}

void GameLoopService::_startGameLoop() {
    
    std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>(0);
	std::chrono::duration<double, std::milli> delay = std::chrono::duration<double, std::milli>(100);


	SnakePart* snakeHead = new SnakePart{ 
		{5, 6}, 
		RIGHT,
		new SnakePart{ 
			{4, 6},
			RIGHT,
			new SnakePart{ 
				{3, 6},
				RIGHT,
				NULL
			}
		}
	};


	GameState gameState = { snakeHead, {10, 5} };

    do {
        auto t0 = std::chrono::high_resolution_clock::now();

		std::vector<Input> inputs = _inputService->popInputs();


		if (!inputs.empty()) {
			snakeHead->direction = inputs.front().direction;
		}

		Direction prevDir;

		bool isFirst = true;

		for (auto it = snakeHead; it != NULL; it = it->next) {
		
			Direction dir = it->direction;

			if (!isFirst)
			{
				it->direction = prevDir;
			}
			isFirst = false;

			prevDir = dir;
			
			switch (dir) {
			case LEFT:
				it->coord.x--;
				break;
			case RIGHT:
				it->coord.x++;
				break;
			case UP:
				it->coord.y--;
				break;
			case DOWN:
				it->coord.y++;
				break;
			default:
				break;
			}
		}

		// todo: checkGameResult
		if (gameState.food.x == snakeHead->coord.x&& gameState.food.y == snakeHead->coord.y) {
			
			srand((unsigned)time(0));
			int i;
			i = (rand() % 20) + 1;

			gameState.food = {
				 rand() % 20,
				 rand() % 10,
			};

			SnakePart* tail = snakeHead;
			for (; tail->next != NULL; tail = tail->next);

			tail->next = new SnakePart{ {tail->coord.x - 1, tail->coord.y}, RIGHT, NULL };
		}


		_renderService->render(gameState);

		auto t1 = std::chrono::high_resolution_clock::now();
		elapsed = t1 - t0;
		// elapsed.count();
		std::this_thread::sleep_for(delay);

    } while (_running);
}
