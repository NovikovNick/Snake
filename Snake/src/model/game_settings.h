#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace snake {

struct GameSettigs final {
  int width = 16;
  int height = 16;
  int buffer_capacity = 16;
  float cell_size = 35.0f;
  int screen_width = 800;
  int screen_height = 800;
  int snake_count = 2;
  int winScore = 30;
  int speed = 150;
};

}  // namespace snake

#endif  // !SNAKE_SNAKE_GAME_MODEL_H_
