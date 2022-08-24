#include "../game_loop_service.h"

#include <unordered_set>

namespace snake {

namespace {

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
    case SystemCommand::kPause:
      gameLoopCtx.switchPause();
      break;
    case SystemCommand::kStepForward:
      gameLoopCtx.frameForward();
      break;
    case SystemCommand::kStepBackward:
      gameLoopCtx.frameBackward();
      break;
    case SystemCommand::kAIStepBackward:
      gameLoopCtx.pauseFrameBackward();
      break;
    case SystemCommand::kAIStepForward:
      gameLoopCtx.pauseFrameForward();
      break;
    default:
      break;
  }
}

/* Conversion GameState to API of AI service */

struct PathFindRequest : public IPathFindRequest {
 private:
  Coord from_;
  Coord to_;
  std::vector<Coord> barriers_;

 public:
  PathFindRequest(const Coord& from, const Coord& to,
                  const std::vector<Coord>& barriers = {})
      : from_(from), to_(to), barriers_(barriers){};

  virtual const Coord& getFrom() const override { return from_; };
  virtual const Coord& getTo() const override { return to_; };
  virtual const std::vector<Coord>& getBarriers() const override {
    return barriers_;
  };
};

PathFindRequest toPathFindRequest(const GameState& gameState,
                                  const GameSettigs& settings) {
  const Snake& player = gameState.getPlayer(0);
  const Snake& bot = gameState.getPlayer(1);
  const Coord& bot_head_coord = bot.getHeadCoord();
  const Coord& food = gameState.getFood();

  std::vector<Coord> barriers;
  for (auto snakePart : player.getParts()) {
    barriers.push_back(snakePart.first);
  }
  for (auto snakePart : bot.getParts()) {
    barriers.push_back(snakePart.first);
  }
  for (int x = settings.leftBoundaries; x <= settings.rightBoundaries; x++) {
    barriers.push_back({x, settings.topBoundaries - 1});
    barriers.push_back({x, settings.bottomBoundaries});
  }
  for (int y = settings.topBoundaries; y <= settings.bottomBoundaries; y++) {
    barriers.push_back({settings.leftBoundaries - 1, y});
    barriers.push_back({settings.rightBoundaries, y});
  }

  return PathFindRequest(bot_head_coord, food, barriers);
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
  GameState init_game_state = game_state_service_->initState(settings);

  GameStateBuffer<GameState> holder(32);
  holder.add(init_game_state);

  GameLoopContext game_loop_ctx(32);

  int64_t delay = settings.initialSpeedMs;
  float progress = 0;
  std::vector<Input> inputs(2);
  std::vector<DebugContext> debug_ctx(32);

  // todo: move it to state service
  srand(settings.foodGenerationSeed);

  do {
    // get inputs
    inputs[0] = input_service_->PopInputs();
    UpdateGameLoopContext(game_loop_ctx, inputs[0]);

    // Calculating...

    if (!game_loop_ctx.isPaused()) {
      const GameState& prev_game_state = holder.head();

      InputDTO botInput =
          ai_service_->findPath(toPathFindRequest(prev_game_state, settings));
      inputs[1] = botInput.inputs.empty() ? Input{} : botInput.inputs.front();

      auto next_game_state = game_state_service_->applyForces(
          prev_game_state, {inputs[0].direction, inputs[1].direction});

      next_game_state.setPhase(
          game_logic_service_->check(next_game_state, settings));

      holder.add(next_game_state);

      if (next_game_state.getPhase() == GamePhase::kWin ||
          next_game_state.getPhase() == GamePhase::kLose) {
        game_loop_ctx.pause();
      }
    }

    const GameState& gameState = holder[game_loop_ctx.getOffsetFrame()];

#if 1
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
      case GamePhase::kWin:
        render_service_->renderWinState();
        break;
      case GamePhase::kLose:
        render_service_->renderLoseState();
        break;
    }

    render_service_->EndDraw();
#endif

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
