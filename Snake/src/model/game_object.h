#ifndef SNAKE_SNAKE_GAME_OBJECT_H_
#define SNAKE_SNAKE_GAME_OBJECT_H_

namespace snake {
enum class SNAKE_INDEX {
  APPLE = 15,
  EMPTY = 10,

  BODY_HOR = 1,
  BODY_VER = 7,

  HEAD_TOP = 3,
  HEAD_BTM = 9,
  HEAD_LFT = 8,
  HEAD_RHT = 4,

  TAIL_TOP = 19,
  TAIL_BTM = 13,
  TAIL_LFT = 14,
  TAIL_RHT = 18,

  TURN_TL = 0,
  TURN_TR = 2,
  TURN_BL = 5,
  TURN_BR = 12
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_OBJECT_H_
