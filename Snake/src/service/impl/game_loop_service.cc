#include "../game_loop_service.h"

#include <unordered_set>

#include "../../model/game_object.h"
#include "../../model/game_settings.h"
#include "../../model/game_state.h"
#include "../../model/ring_buffer.h"
#include "../../util.h"

namespace snake {

void GameLoopService::Start() {
  running_ = true;
  render_thread_ = std::thread(&GameLoopService::StartGameLoop, this);
}

GameLoopService::GAME_STATE_HOLDER GameLoopService::initGameStates() {
  // arrange
  int frame = 0;
  int buf_capacity = settings_.buffer_capacity;

  std::vector<SNAKE_DATA> snakes;
  for (int i = 0; i < settings_.snake_count; ++i)
    snakes.push_back(SNAKE_DATA{{i, 2, Direction::kDown},
                                {i, 1, Direction::kDown},
                                {i, 0, Direction::kDown}});

  GAME_STATE_HOLDER buf(buf_capacity);
  buf.Add(GameState(
      frame, settings_.snake_count,
      Grid2d(settings_.width, settings_.height, settings_.snake_count)));

  auto& init_game_state = buf[0];
  init_game_state.inputs[0] = Direction::kNone;

  for (int i = 0; i < settings_.snake_count; ++i)
    init_game_state.grid.AddSnake(i, snakes[i].begin(), snakes[i].end());

  food_srv_->SetFood(init_game_state);

  return buf;
}

void GameLoopService::Render(const int offset, GAME_STATE_HOLDER& holder) {
  auto& state = holder[offset];
  auto out = ui_srv->layout_srv->map->getOutputIterator();

  for (int y = 0; y < settings_.height; ++y) {
    for (int x = 0; x < settings_.width; ++x) {
      *out = state.grid.GetType(x, y);
      ++out;
    }
  }
  ui_srv->layout_srv->update(state.score[0]);
}

void GameLoopService::Stop() {
  running_ = false;
  render_thread_.join();
}

void GameLoopService::StartGameLoop() {
  GAME_STATE_HOLDER buf = initGameStates();
  auto delay = std::chrono::duration<double, std::milli>(settings_.speed);
  bool paused = false, started = false;
  int frame_offset, frame = 0;

  do {
    auto t0 = std::chrono::steady_clock::now();
    // 1.
    auto player_input = input_service_->pollEvent();

    if (player_input.command == SystemCommand::kStartGame) {
      started = true;
      paused = false;
      frame = 0;
      frame_offset = 0;
      buf = initGameStates();
    }

    if (player_input.command == SystemCommand::kPause) {
      paused = !paused;
      frame_offset = 0;
    }

    // 2.
    if (started) {
      if (!paused) {
        buf.Add(GameState(
            ++frame, settings_.snake_count,
            Grid2d(settings_.width, settings_.height, settings_.snake_count)));

        buf[frame_offset].inputs[0] = player_input.direction;
        game_state_service_->Move(buf[1], buf[0]);

        if (buf[0].isScoreReached(settings_.winScore)) {
          paused = true;
          ui_srv->game_finished = true;
          ui_srv->win = buf[0].score[0] == settings_.winScore;
        }

        if (buf[0].isCollide()) {
          paused = true;
          ui_srv->game_finished = true;
          ui_srv->win = !buf[0].collision[0];
        }

      } else {
        if (player_input.command == SystemCommand::kStepBackward)
          frame_offset = std::clamp<int>(
              frame_offset + 1, 0, std::min<int>(frame, buf.getCapacity()) - 2);

        if (player_input.command == SystemCommand::kStepForward)
          frame_offset = std::clamp<int>(
              frame_offset - 1, 0, std::min<int>(frame, buf.getCapacity()) - 2);

        if (player_input.direction != Direction::kNone) {
          buf[frame_offset].inputs[0] = player_input.direction;
          game_state_service_->RollbackAndMove(frame_offset, buf);
        }
      }
      Render(frame_offset, buf);
    }

    // End loop
    auto t1 = std::chrono::steady_clock::now();
    auto elapsed = duration_cast<std::chrono::milliseconds>(t0 - t1);
    std::this_thread::sleep_for(delay - elapsed);
  } while (running_);
}
}  // namespace snake
