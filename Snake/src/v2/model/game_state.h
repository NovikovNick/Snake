#ifndef SNAKE_SNAKE_GAME_STATE_H_
#define SNAKE_SNAKE_GAME_STATE_H_
#include <vector>

#include "grid.h"

namespace snake {

struct GameStateV2 {
  int frame;
  Grid2d grid;
  std::vector<int> inputs;
  std::vector<int> score;
  bool is_score_reached;
  bool is_collide;
  bool is_food_consumed;

  GameStateV2(const int frame, const int snake_count, Grid2d&& grid)
      : frame(frame),
        grid(std::move(grid)),
        inputs(std::vector<int>(snake_count)),
        score(std::vector<int>(snake_count)),
        is_collide(false),
        is_food_consumed(false) {}
};

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_STATE_H_
