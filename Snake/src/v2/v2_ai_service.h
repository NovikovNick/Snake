#ifndef SNAKE_SNAKE_AI_SERVICE_H_
#define SNAKE_SNAKE_AI_SERVICE_H_
#include <vector>

#include "model/grid.h"

namespace snake {

class AIService {
 private:
  std::vector<MyCoord> out_;
  AStarPathfinder<MyCoord, Grid2d, std::vector<MyCoord>::iterator> pathfinder_;
  std::vector<MyCoord> dirs_{
      {1, 0},   // right
      {-1, 0},  // left
      {0, 1},   // bottom
      {0, -1}   // top
  };

 public:
  AIService(const int width, const int height)
      : out_(std::vector<MyCoord>(width * height)) {}

  // todo split to 2 methods!
  int FindPath(const MyCoord& start, const MyCoord& goal, const Grid2d& grid) {
    if (grid.food != start &&
        pathfinder_.FindPath(start, goal, grid, out_.begin(), out_.end())) {
      return GetDirection(out_[1], start);
    } else {
      return FindPath(start, grid);
    }
  };

  int FindPath(const MyCoord& start, const Grid2d& grid) {
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
  int GetDirection(const MyCoord& o1, const MyCoord& o2) {
    MyCoord dir = MyCoord(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
    auto it = std::find(dirs_.begin(), dirs_.end(), dir);

    if (it == dirs_.end()) {
      return 0;
    } else {
      return std::distance(dirs_.begin(), it);
    }
  }
};

}  // namespace snake

#endif  // SNAKE_SNAKE_AI_SERVICE_H_
