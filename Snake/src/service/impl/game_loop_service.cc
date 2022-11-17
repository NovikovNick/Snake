#include "../game_loop_service.h"

#include <unordered_set>

#include "../../model/game_settings.h"
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

void GameLoopService::Render(const int offset, GAME_STATE_HOLDER& holder) {
  auto& state = holder[offset];

  auto out = render_service_->GetOutput();
  state.grid.copy(out);
  out = out + settings_.width * settings_.height;
  for (int i = 0; i < holder.Size(); ++i) {
    *(out + i) = GAME_OBJECT(0, holder[i].inputs[0], 5);
  }

  render_service_->Render(offset);
}

void GameLoopService::Stop() {
  running_ = false;
  render_thread_.join();
}

void GameLoopService::StartGameLoop() {
  GameSettigs stg;
  // arrange
  int frame = 0;
  int buf_capacity = settings_.buffer_capacity;

  std::vector<SNAKE_DATA> snakes;
  for (int i = 0; i < stg.snake_count; ++i)
    snakes.push_back(SNAKE_DATA{{i, 2, 2}, {i, 1, 2}, {i, 0, 2}});

  GAME_STATE_HOLDER buf(buf_capacity);
  buf.Add(GameState(frame, stg.snake_count,
                    Grid2d(stg.width, stg.height, stg.snake_count)));

  auto& init_game_state = buf[0];
  init_game_state.inputs[0] = -1;

  for (int i = 0; i < stg.snake_count; ++i)
    init_game_state.grid.AddSnake(i, snakes[i].begin(), snakes[i].end());

  food_srv_->SetFood(init_game_state);

  auto delay = std::chrono::duration<double, std::milli>(stg.speed);

  bool paused = false;
  int frame_offset = 0;
  do {
    auto t0 = std::chrono::steady_clock::now();
    // 1.
    auto player_input = input_service_->PopInputs();

    if (player_input.command == SystemCommand::kPause) {
      paused = !paused;
      frame_offset = 0;
    }

    // 2.
    if (!paused) {
      buf.Add(GameState(++frame, stg.snake_count,
                        Grid2d(stg.width, stg.height, stg.snake_count)));

      buf[frame_offset].inputs[0] = AdaptToV2(player_input.direction);
      game_state_service_->Move(buf[1], buf[0]);

      paused = buf[0].is_collide;

    } else {
      if (player_input.command == SystemCommand::kStepBackward)
        frame_offset = std::clamp<int>(frame_offset + 1, 0,
                                       std::min<int>(frame, buf_capacity) - 2);

      if (player_input.command == SystemCommand::kStepForward)
        frame_offset = std::clamp<int>(frame_offset - 1, 0,
                                       std::min<int>(frame, buf_capacity) - 2);

      if (player_input.direction != Direction::kNone) {
        buf[frame_offset].inputs[0] = AdaptToV2(player_input.direction);
        game_state_service_->RollbackAndMove(frame_offset, buf);
      }
    }

    Render(frame_offset, buf);

    // End loop
    auto t1 = std::chrono::steady_clock::now();
    auto elapsed = duration_cast<std::chrono::milliseconds>(t0 - t1);
    std::this_thread::sleep_for(delay - elapsed);
  } while (running_);
}
}  // namespace snake
