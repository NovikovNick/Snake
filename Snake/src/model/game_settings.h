#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace snake {

struct GameSettigs final {
  int width = 30;
  int height = 15;
  int snake_count = 2;
  int winScore = 30;
  int speed = 100;
};

}  // namespace snake

#endif  // !SNAKE_SNAKE_GAME_MODEL_H_
