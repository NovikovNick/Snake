#ifndef SNAKE_DIRECTION_H_
#define SNAKE_DIRECTION_H_

namespace snake {

enum class Direction : std::uint8_t {
  kRight,
  kLeft,
  kDown,
  kUp,
  kNone,
};

}  // namespace snake
#endif  // SNAKE_DIRECTION_H_
