#include "../game_loop_service.h"

#include <unordered_set>

#include "../../v2/model/game_state.h"
#include "../../v2/model/ring_buffer.h"

namespace snake {

namespace {
int AdaptToV2(const Direction dir) {
  switch (dir) {
    case Direction::kUp:
      return 3;
    case Direction::kDown:
      return 2;
    case Direction::kLeft:
      return 1;
    case Direction::kRight:
      return 0;
    default:
      return -1;
  }
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
  GameSettigs settings;
  // arrange
  int width = 10, height = 10, snake_count = 1, winScore = 5, frame = 0;
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  SNAKE_DATA snake1{{5, 2, 2}, {5, 1, 2}, {5, 0, 2}};

  RingBuffer<GameStateV2> buffer(32);
  buffer.add(
      GameStateV2(frame, snake_count, Grid2d(width, height, snake_count)));

  auto& init_game_state = buffer.head();
  init_game_state.grid.AddSnake(0, snake0.begin(), snake0.end());
  //init_game_state.grid.AddSnake(1, snake1.begin(), snake1.end());

  food_srv_->SetFood(init_game_state);

  do {
    // 1.
    auto& prev = buffer.head();
    buffer.add(
        GameStateV2(++frame, snake_count, Grid2d(width, height, snake_count)));
    auto& next = buffer.head();
    next.grid.food = prev.grid.food;
    next.score = prev.score;
    // 2. get inputs
    auto input = AdaptToV2(input_service_->PopInputs().direction);
    if (input >= 0) {
      next.inputs[0] = input;
    }
    game_state_service_->SetInputs(prev, next);
    // 3.
    game_state_service_->ApplyInputs(prev, next);

    running_ = running_ && !next.is_collide;
    running_ = running_ && next.score[0] < winScore;
    // running_ = running_ && next.score[1] < winScore;

    if (next.is_food_consumed) {
      bool is_food_left = food_srv_->SetFood(next);
      running_ = running_ && is_food_left;
    }

    next.grid.copy(render_service_->GetOutput());
    render_service_->Render();

    // End loop
    std::this_thread::sleep_for(
        std::chrono::duration<double, std::milli>(settings.maxSpeedMs));
  } while (running_);
}
}  // namespace snake
