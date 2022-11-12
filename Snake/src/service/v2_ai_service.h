#ifndef SNAKE_SNAKE_AI_SERVICE_H_
#define SNAKE_SNAKE_AI_SERVICE_H_
#include <vector>

#include "ai/a_star.h"
#include "ai/impl/a_star_dp.cc"
#include "../model/grid.h"
#include "../model/grid_cell.h"
#include "../util.h"

namespace snake {

class AIService {
  using COORD = GridCell;
  using GRID = Grid2d;

 private:
  std::vector<COORD> out_;
  AStarPathfinder<COORD, GRID, std::vector<COORD>::iterator> pathfinder_;
  std::vector<COORD> dirs_{
      {1, 0},   // right
      {-1, 0},  // left
      {0, 1},   // bottom
      {0, -1}   // top
  };

 public:
  AIService(const int width, const int height)
      : out_(std::vector<COORD>(width * height)) {}

  // todo split to 2 methods!
  int FindPath(const COORD& start, const COORD& goal, const GRID& grid) {
    if (grid.food != start &&
        pathfinder_.FindPath(start, goal, grid, out_.begin(), out_.end())) {
      return GetDirection(out_[1], start);
    } else {
      return FindPath(start, grid);
    }
  };

  int FindPath(const COORD& start, const GRID& grid) {
    for (int dir = 0; auto vector : dirs_) {
      int nextX = start.GetX() + vector.GetX();
      int nextY = start.GetY() + vector.GetY();
      if (!grid.IsOutOfBound(nextX, nextY) && !grid.IsSnake(nextX, nextY)) {
        return dir;
      }
      ++dir;
    }
    return 0;
  }

 private:
  int GetDirection(const COORD& o1, const COORD& o2) {
    COORD vector = COORD(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
    for (int i = 0; const auto& dir : dirs_) {
      if (dir == vector) {
        return i;
      }
      ++i;
    }
    return 0;
  }
};

}  // namespace snake

#endif  // SNAKE_SNAKE_AI_SERVICE_H_
