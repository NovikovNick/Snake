#ifndef SNAKE_SNAKE_GAME_STATE_H_
#define SNAKE_SNAKE_GAME_STATE_H_
#include <algorithm>
#include <vector>

#include "grid.h"

namespace snake {

struct GameState {
  using GRID = Grid2d;

 private:
  int snake_count;

 public:
  int frame;
  GRID grid;
  std::vector<int> inputs;
  std::vector<int> score;
  std::vector<bool> collision;
  bool is_food_consumed;

  GameState(const int frame, const int snake_count, GRID&& grid)
      : snake_count(snake_count),
        frame(frame),
        grid(std::move(grid)),
        inputs(std::vector<int>(snake_count)),
        score(std::vector<int>(snake_count)),
        collision(std::vector<bool>(snake_count)),
        is_food_consumed(false) {}

  bool isCollide() const {
    return std::find(collision.begin(), collision.end(), true) !=
           collision.end();
  }

  bool isScoreReached(const int win_score) const {
    return std::find(score.begin(), score.end(), win_score) != score.end();
  }
};

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_STATE_H_
