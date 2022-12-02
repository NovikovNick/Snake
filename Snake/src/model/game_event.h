#ifndef SNAKE_GAME_EVENT_H_
#define SNAKE_GAME_EVENT_H_

#include "direction.h"

namespace snake {

enum class SystemCommand : std::int8_t {
  kStartGame,
  kPause,
  kStepForward,
  kStepBackward,
  kAIStepForward,
  kAIStepBackward,
  kNone
};

struct GameEvent final {
  Direction direction = Direction::kNone;
  SystemCommand command = SystemCommand::kNone;
};

}  // namespace snake
#endif  // SNAKE_GAME_EVENT_H_
