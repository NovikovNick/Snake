#include "../GameLoopService.h"
#include <unordered_set>

namespace snake {

namespace {

Snake* InitPlayer(
	const int& x,
	const int& y,
	const int& lenght,
	const Direction& dir) {

	std::vector<std::pair<Coord, Direction>> list{
		std::make_pair(Coord{x, y}, dir)
	};

	for (size_t i = 0; i < lenght; i++) {
		list.push_back(std::make_pair(list.back().first - dir, dir));
	}

	return new Snake(list);
}

struct GameLoopContext {

	GameLoopContext(const int& capacity) : capacity_(capacity) {}

	const int& getFrame() const noexcept { return frame_; }
	const int& getOffsetFrame() const noexcept { return frame_ - frameOffset_; }
	const int& getPauseFrame() const noexcept { return pauseFrame_; }
	const bool& isPaused() const noexcept { return paused_; }

	void incrementFrame() noexcept { frame_++; }
	void switchPause() noexcept { 
		paused_ = !paused_;
		pauseFrame_ = 0;
		frameOffset_ = 0;
	}
	void frameForward() noexcept {
		
		frameOffset_++;

		if (frameOffset_ > frame_) {
			frameOffset_ = frame_;
		} else if (frameOffset_ > capacity_) {
			frameOffset_ = capacity_;
		}
	}
	void frameBackward() noexcept {
		frameOffset_--;
		if (frameOffset_ < 0) {
			frameOffset_ = 0;
		}
	}
	void pauseFrameForward() noexcept {
		pauseFrame_++;
		if (pauseFrame_ > pauseMaxFrame_) {
			pauseFrame_ = pauseMaxFrame_;
		}
	}
	void pauseFrameBackward() noexcept {
		pauseFrame_--;
		if (pauseFrame_ < 0) {
			pauseFrame_ = 0;
		}
	}
	void pause() noexcept { paused_ = false; }

private:
	int frame_ = 0;
	int capacity_;
	int frameOffset_ = 0;
	int pauseFrame_ = 0;

	int pauseMaxFrame_ = 0;

	bool paused_ = false;
};

void updateGameLoopContext(
	GameLoopContext& gameLoopCtx,
	const Input& input) {

	switch (input.command) {
	case SystemCommand::PAUSE:
		gameLoopCtx.switchPause();
		break;
	case SystemCommand::STEP_FORWARD:
		gameLoopCtx.frameForward();
		break;
	case SystemCommand::STEP_BACKWARD:
		gameLoopCtx.frameBackward();
		break;
	case SystemCommand::AI_STEP_BACKWARD:
		gameLoopCtx.pauseFrameBackward();
		break;
	case SystemCommand::AI_STEP_FORWARD:
		gameLoopCtx.pauseFrameForward();
		break;
	default:
		break;
	}
}


Coord generateNewFood(const GameState& gameState, GameSettigs& settings) {


	std::unordered_set<Coord, hash_coord> set;

	for (size_t i = 0; i < 2; i++) {
		for (auto part : gameState.getPlayer(i).getParts()) {
			set.insert(part.first);
		}
	}


	Coord res;
    srand((unsigned)time(NULL));
    do {
		res.x = settings.leftBoundaries + rand() % (settings.rightBoundaries - settings.leftBoundaries);
		res.y = settings.topBoundaries + rand() % (settings.bottomBoundaries - settings.topBoundaries);

    } while (set.find(res) != set.end());

	return res;

}

}// namespace

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

	GameState* initGameState = new GameState(
		0, 
		InitPlayer(settings.startPlayedXCoord, settings.startPlayedYCoord, settings.startLenght, settings.startPlayedDirection),
		InitPlayer(10, 13, 3, Direction::RIGHT)
	);
	initGameState->setFood({ settings.startFoodXCoord, settings.startFoodYCoord });

	
	GameStateBuffer<GameState> holder(32);
	holder.add(initGameState);


	GameLoopContext gameLoopCtx(32);

	int64_t delay = settings.initialSpeedMs;
	float progress = 0;
	std::vector<Input> inputs;
	inputs.resize(2);

	std::vector<DebugContext> debugCtx;
	debugCtx.resize(32);



    do {

		// get inputs
		inputs[0] = _inputService->popInputs();
		updateGameLoopContext(gameLoopCtx, inputs[0]);

		// Calculating...

		_renderService->BeginDraw();
		if (gameLoopCtx.isPaused()) {

			_renderService->renderInputs(gameLoopCtx.getOffsetFrame(), holder, settings);

			/*
			
			nextGameState = holder.GetStateWithOffset();
			int index = (holder.GetFrame() + holder.GetOffset()) % holder.GetCapacity();
			DebugContext ctx = debugCtx[index];
			
			_renderService->BeginDraw();
			_renderService->renderBoard(settings);

			if (ctx.pathfinding.size() > 0) {

				if (pauseFrame >= ctx.pathfinding.size()) {
					pauseFrame = ctx.pathfinding.size() - 1;
				}

				auto it = ctx.pathfinding[pauseFrame];
				_renderService->renderDebugAI(it);
			}
			

			_renderService->renderSelfInputs(nextGameState, &holder, settings);
			_renderService->renderEnemyInputs(nextGameState, &holder, settings);

			_renderService->renderSelf(nextGameState, 0, settings);
			_renderService->renderEnemy(nextGameState, 1, settings);
			_renderService->renderFood(nextGameState, settings);
			if (nextGameState == WIN) {
				_renderService->renderWinState();
			} else if (nextGameState->gamePhase == LOSE) {
				_renderService->renderLoseState();
			}
			*/

		} else {

			const GameState& prevGameState = holder.head();
			const GamePhase& prevGamePhase = prevGameState.getPhase();

			if (prevGamePhase == WIN || prevGamePhase == LOSE) {
				gameLoopCtx.pause();
			}


			InputDTO botInput = _aiService->getInputs(prevGameState, settings);
			inputs[1] = botInput.inputs.empty() ? Input{} : botInput.inputs.front();

			const Snake& prevPlayer = prevGameState.getPlayer(0);
			const Snake& prevBot = prevGameState.getPlayer(1);

			Snake* nextPlayer = prevPlayer.move(inputs[0].direction);
			Snake* nextBot = prevBot.move(inputs[1].direction);


			bool playerGained = nextPlayer->getHeadCoord() == prevGameState.getFood();
			bool botGained = nextBot->getHeadCoord() == prevGameState.getFood();
			

			GameState* nextGameState = new GameState(gameLoopCtx.getFrame() + 1, nextPlayer, nextBot);
			nextGameState->setDebugContext(botInput.ctx);
			nextGameState->setFood(prevGameState.getFood());
			nextGameState->setInputs(inputs);

			if (playerGained) {
				nextPlayer->gain();
				nextGameState->setFood(generateNewFood(*nextGameState, settings));
			} else if (botGained) {
				nextBot->gain();
				nextGameState->setFood(generateNewFood(*nextGameState, settings));
			}

			nextGameState->setPhase(_gameLogicService->check(*nextGameState, settings));
			
			holder.add(nextGameState);

			gameLoopCtx.incrementFrame();
			
		}

		const GameState& gameState = holder.head();

		_renderService->renderSelf(gameState, 0, settings);
		_renderService->renderEnemy(gameState, 1, settings);
		_renderService->renderFood(gameState.getFood(), settings);

		if (gameState.getDebugContext().pathfinding.size() > 0) {

			/*if (pauseFrame >= ctx.pathfinding.size()) {
				pauseFrame = ctx.pathfinding.size() - 1;
			}*/

			auto it = gameState.getDebugContext().pathfinding[gameState.getDebugContext().pathfinding.size() - 1];
			_renderService->renderDebugAI(it);
		}
		_renderService->renderBoard(settings);

		/*if (nextGameState->getPhase() == WIN) {
			_renderService->renderWinState();
			gameLoopCtx.pause();
		}
		else if (nextGameState->getPhase() == LOSE) {
			_renderService->renderLoseState();
			gameLoopCtx.pause();
		}*/

		_renderService->EndDraw();

		// progress = (nextGameState->score[0] / (settings.scoreToWin / 100.0f)) / 100.0f;
		// delay = paused ? 15 : settings.maxSpeedMs + (settings.initialSpeedMs - settings.maxSpeedMs) * (1 - progress);

		delay = gameLoopCtx.isPaused() ? 15 : settings.maxSpeedMs;

		// End loop
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));

    } while (_running);
}

} // namespace snake
