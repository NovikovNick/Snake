#ifndef SNAKE_SNAKE_GAME_STATE_H_
#define SNAKE_SNAKE_GAME_STATE_H_
#include <algorithm>
#include <deque>
#include <vector>

#include "../serialization.h"
#include "direction.h"
#include "grid.h"

namespace snake {

struct GameState {
  using GRID = Grid2d;
  using COORD = GridCell;
  using byte = unsigned char;

 public:
  int snake_count;
  int frame;
  GRID grid;
  std::vector<COORD> foods;
  std::vector<Direction> inputs;
  std::vector<int> score;
  std::vector<bool> collision;
  bool is_food_consumed;

  GameState(const int frame, const int snake_count, GRID &&grid,
            std::vector<COORD> &foods)
      : snake_count(snake_count),
        frame(frame),
        grid(std::move(grid)),
        foods(foods),
        inputs(std::vector<Direction>(snake_count)),
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

  static byte *serialize(const GameState &src, byte **dst) {
    byte *begin = *dst;
    begin = serialisation::write(&begin, src.frame);
    begin = serialisation::write(&begin, src.snake_count);
    begin = serialisation::write(&begin, src.foods);

    begin = Grid2d::serialize(src.grid, &begin);

    begin = serialisation::write(&begin, src.score);
    begin = serialisation::write(&begin, src.inputs);
    begin = serialisation::write(&begin, src.collision);

    return begin;
  };

  static GameState deserialize(byte *src) {
    int frame, snake_count = 0;
    std::vector<GridCell> foods;

    src = serialisation::read(src, &frame);
    src = serialisation::read(src, &snake_count);
    src = serialisation::readVector(src, &foods);

    Grid2d grid(0, 0, 2);
    src = Grid2d::deserialize(src, grid);
    GameState gs(frame, snake_count, std::move(grid), foods);

    src = serialisation::readVector(src, &gs.score);
    src = serialisation::readVector(src, &gs.inputs);
    src = serialisation::readVector(src, &gs.collision);
    return gs;
  };
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_STATE_H_
