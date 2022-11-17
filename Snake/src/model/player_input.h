#ifndef SNAKE_SNAKE_GAME_PLAYER_INPUT_H_
#define SNAKE_SNAKE_GAME_PLAYER_INPUT_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grid_cell.h"

namespace snake {

enum class Direction : std::uint8_t {
  kUp,
  kDown,
  kLeft,
  kRight,
  kNone,
};

enum class SystemCommand : std::int8_t {
  kPause,
  kStepForward,
  kStepBackward,
  kAIStepForward,
  kAIStepBackward,
  kNone
};

struct PlayerInput final {
  Direction direction = Direction::kNone;
  SystemCommand command = SystemCommand::kNone;
};

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_PLAYER_INPUT_H_
