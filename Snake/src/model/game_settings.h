#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace snake {

struct GameSettigs final {
  int width = 14;
  int height = 14;
  int buffer_capacity = 16;
  float cell_size = 35.0f;
  int screen_width = 1200;
  int screen_height = 800;
  int snake_count = 2;
  int winScore = 10;
  int speed = 125;
};

}  // namespace snake

#endif  // !SNAKE_SNAKE_GAME_MODEL_H_
