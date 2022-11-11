#include "../game_loop_service.h"

#include <unordered_set>

#include "../../model/game_state.h"
#include "../../model/ring_buffer.h"

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
    case Direction::kNone:
      return -1;
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
  GameSettigs stg;
  // arrange
  int frame = 0;
  std::vector<SNAKE_DATA> snakes;
  for (int i = 0; i < stg.snake_count; ++i)
    snakes.push_back(SNAKE_DATA{{i, 2, 2}, {i, 1, 2}, {i, 0, 2}});

  RingBuffer<GameStateV2> buffer(32);
  buffer.add(GameStateV2(frame, stg.snake_count,
                         Grid2d(stg.width, stg.height, stg.snake_count)));

  auto& init_game_state = buffer.head();

  for (int i = 0; i < stg.snake_count; ++i)
    init_game_state.grid.AddSnake(i, snakes[i].begin(), snakes[i].end());

  food_srv_->SetFood(init_game_state);

  do {
    // 1.
    auto& prev = buffer.head();
    buffer.add(GameStateV2(++frame, stg.snake_count,
                           Grid2d(stg.width, stg.height, stg.snake_count)));
    auto& next = buffer.head();
    next.grid.food = prev.grid.food;
    next.score = prev.score;
    // 2. get inputs
    next.inputs[0] = AdaptToV2(input_service_->PopInputs().direction);
    game_state_service_->SetInputs(prev, next);
    // 3.
    game_state_service_->ApplyInputs(prev, next);

    running_ = running_ && !next.is_collide;
    for (int i = 0; i < stg.snake_count; ++i)
      running_ = running_ && next.score[i] < stg.scoreToWin;

    if (next.is_food_consumed) {
      bool is_food_left = food_srv_->SetFood(next);
      running_ = running_ && is_food_left;
    }

    next.grid.copy(render_service_->GetOutput());
    render_service_->Render();

    // End loop
    std::this_thread::sleep_for(
        std::chrono::duration<double, std::milli>(stg.maxSpeedMs));
  } while (running_);
}
}  // namespace snake
