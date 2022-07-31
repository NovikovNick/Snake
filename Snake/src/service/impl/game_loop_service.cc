#include "../game_loop_service.h"

#include <unordered_set>

namespace snake {

namespace {

Snake* InitPlayer(const int& x, const int& y, const int& lenght,
                  const Direction& dir) {
  std::vector<std::pair<Coord, Direction>> list{
      std::make_pair(Coord{x, y}, dir)};

  for (int i = 0; i < lenght; ++i) {
    list.push_back(std::make_pair(list.back().first - dir, dir));
  }

  return new Snake(list);
}

struct GameLoopContext final {
  GameLoopContext(const int& capacity) : capacity_(capacity) {}

  const int& getFrame() const noexcept { return frame_; }
  const int& getOffsetFrame() const noexcept { return frameOffset_; }
  const int& getPauseFrame() const noexcept { return pauseFrame_; }
  const bool& isPaused() const noexcept { return paused_; }

  void incrementFrame() noexcept { frame_++; }
  void switchPause() noexcept {
    paused_ = !paused_;
    pauseFrame_ = 0;
    frameOffset_ = 0;
  }
  void frameForward() noexcept {
    if (paused_) {
      frameOffset_++;

      if (frameOffset_ > frame_) {
        frameOffset_ = frame_;
      } else if (frameOffset_ >= capacity_) {
        frameOffset_ = capacity_ - 1;
      }
    }
  }
  void frameBackward() noexcept {
    if (paused_) {
      frameOffset_--;
      if (frameOffset_ < 0) {
        frameOffset_ = 0;
      }
    }
  }
  void pauseFrameForward() noexcept {
    if (paused_) {
      pauseFrame_++;
    }
  }
  void pauseFrameBackward() noexcept {
    if (paused_) {
      pauseFrame_--;
      if (pauseFrame_ < 0) {
        pauseFrame_ = 0;
      }
    }
  }
  void pause() noexcept {
    paused_ = true;
    pauseFrame_ = 0;
    frameOffset_ = 0;
  }

 private:
  int frame_ = 0;
  int capacity_;
  int frameOffset_ = 0;
  int pauseFrame_ = 0;

  bool paused_ = false;
};

void UpdateGameLoopContext(GameLoopContext& gameLoopCtx, const Input& input) {
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

Coord GenerateNewFood(const GameState& gameState, GameSettigs& settings) {
  std::unordered_set<Coord, hash_coord> set;

  for (size_t i = 0; i < 2; i++) {
    for (auto part : gameState.getPlayer(i).getParts()) {
      set.insert(part.first);
    }
  }

  Coord res;
  do {
    res.x = settings.leftBoundaries +
            rand() % (settings.rightBoundaries - settings.leftBoundaries);
    res.y = settings.topBoundaries +
            rand() % (settings.bottomBoundaries - settings.topBoundaries);

  } while (set.find(res) != set.end());

  return res;
}

}  // namespace

void GameLoopService::Start() {
  running_ = true;
  render_thread_ = std::thread(&GameLoopService::StartGameLoop, this);
}

void GameLoopService::Stop() {
  running_ = false;
  render_thread_.join();
}

void GameLoopService::StartGameLoop() {
  GameSettigs settings = {};

  GameState* init_game_state = new GameState(
      0,
      InitPlayer(settings.startPlayedXCoord, settings.startPlayedYCoord,
                 settings.startLenght, settings.startPlayedDirection),
      InitPlayer(10, 13, 3, Direction::RIGHT));
  init_game_state->setFood(
      {settings.startFoodXCoord, settings.startFoodYCoord});

  GameStateBuffer<GameState> holder(32);
  holder.add(init_game_state);

  GameLoopContext game_loop_ctx(32);

  int64_t delay = settings.initialSpeedMs;
  float progress = 0;
  std::vector<Input> inputs;
  inputs.resize(2);

  std::vector<DebugContext> debug_ctx;
  debug_ctx.resize(32);

  srand(settings.foodGenerationSeed);

  do {
    // get inputs
    inputs[0] = input_service_->PopInputs();
    UpdateGameLoopContext(game_loop_ctx, inputs[0]);

    // Calculating...

    if (!game_loop_ctx.isPaused()) {
      const GameState& prev_game_state = holder.head();

      InputDTO botInput = ai_service_->GetInputs(prev_game_state, settings);
      inputs[1] = botInput.inputs.empty() ? Input{} : botInput.inputs.front();

      const Snake& prevPlayer = prev_game_state.getPlayer(0);
      const Snake& prevBot = prev_game_state.getPlayer(1);

      Snake* next_player = prevPlayer.move(inputs[0].direction);
      Snake* next_bot = prevBot.move(inputs[1].direction);

      bool is_player_gained =
          next_player->getHeadCoord() == prev_game_state.getFood();
      bool is_bot_gained =
          next_bot->getHeadCoord() == prev_game_state.getFood();

      GameState* next_game_state =
          new GameState(game_loop_ctx.getFrame() + 1, next_player, next_bot);
      next_game_state->setDebugContext(botInput.ctx);
      next_game_state->setFood(prev_game_state.getFood());
      next_game_state->setInputs(inputs);
      next_game_state->setScore(0, prev_game_state.getScore(0));
      next_game_state->setScore(1, prev_game_state.getScore(1));

      if (is_player_gained) {
        next_player->gain();
        next_game_state->setFood(GenerateNewFood(*next_game_state, settings));

        next_game_state->setScore(0, prev_game_state.getScore(0) + 1);
      } else if (is_bot_gained) {
        next_bot->gain();
        next_game_state->setFood(GenerateNewFood(*next_game_state, settings));
        next_game_state->setScore(1, prev_game_state.getScore(1) + 1);
      }

      next_game_state->setPhase(
          game_logic_service_->check(*next_game_state, settings));

      holder.add(next_game_state);

      if (next_game_state->getPhase() == WIN ||
          next_game_state->getPhase() == LOSE) {
        game_loop_ctx.pause();
      }
    }

    const GameState& gameState = holder[game_loop_ctx.getOffsetFrame()];

    render_service_->BeginDraw();
    render_service_->renderSelf(gameState, 0, settings);
    render_service_->renderEnemy(gameState, 1, settings);
    render_service_->renderFood(gameState.getFood(), settings);
    render_service_->renderBoard(settings);

    if (game_loop_ctx.isPaused()) {
      auto path = gameState.getDebugContext().pathfinding;
      if (path.size() > 0) {
        int pauseFrame = game_loop_ctx.getPauseFrame();
        if (pauseFrame >= path.size()) {
          pauseFrame = path.size() - 1;
        }
        auto pathfindingIteration =
            gameState.getDebugContext().pathfinding[pauseFrame];
        render_service_->renderDebugAI(pathfindingIteration);
      }

      render_service_->renderInputs(game_loop_ctx.getOffsetFrame(), holder,
                                    settings);
    }

    switch (gameState.getPhase()) {
      case WIN:
        render_service_->renderWinState();
        break;
      case LOSE:
        render_service_->renderLoseState();
        break;
    }

    render_service_->EndDraw();

#if 0 
    progress =
        (gameState.getScore(0) / (settings.scoreToWin / 100.0f)) / 100.0f;
    delay = gameLoopCtx.isPaused()
                ? 15
                : settings.maxSpeedMs +
                      (settings.initialSpeedMs - settings.maxSpeedMs) *
                          (1 - progress);
#endif

    if (!game_loop_ctx.isPaused()) {
      game_loop_ctx.incrementFrame();
    }

    delay = game_loop_ctx.isPaused() ? 15 : settings.maxSpeedMs;

    // End loop
    std::this_thread::sleep_for(
        std::chrono::duration<double, std::milli>(delay));

  } while (running_);
}
}  // namespace snake
