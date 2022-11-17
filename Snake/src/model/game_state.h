#ifndef SNAKE_SNAKE_GAME_STATE_H_
#define SNAKE_SNAKE_GAME_STATE_H_
#include <vector>

#include "grid.h"

namespace snake {

struct GameState {
  using GRID = Grid2d;

  int frame;
  GRID grid;
  std::vector<int> inputs;
  std::vector<int> score;
  bool is_score_reached;
  bool is_collide;
  bool is_food_consumed;

  GameState(const int frame, const int snake_count, GRID&& grid)
      : frame(frame),
        grid(std::move(grid)),
        inputs(std::vector<int>(snake_count)),
        score(std::vector<int>(snake_count)),
        is_collide(false),
        is_food_consumed(false) {}
};

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_STATE_H_
