#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 0

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star.cc"

namespace snake {

std::vector<MyCoord> dirs{
    {1, 0},   // right
    {-1, 0},  // left
    {0, 1},   // bottom
    {0, -1}   // top
};

void moveSnake(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end,
               SNAKE_DATA_ITERATOR out) {
  for (auto [_, __, prevDir] = *begin; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    *out =
        std::make_tuple(x + dirs[dir].GetX(), y + dirs[dir].GetY(), prevDir);
    prevDir = dir;
    ++out;
  }
}

void print(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end) {
  for (; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    std::cout << std::format("[{:2d},{:2d},{:2d}]\n", x, y, dir);
  }
}

MyCoord getDir(const MyCoord& o1, const MyCoord& o2) {
  return MyCoord(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
}

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 10, height = 10;
  Grid2d<MyCoord> grid(width, height);
  grid.food = MyCoord(9, 9);
  // todo: add barriers;

  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  grid.AddSnake(0, snake0.begin(), snake0.end());

  std::vector<GAME_OBJECT> gameObjects(width * height);

  std::vector<MyCoord> out(5);
  AStarPathfinder<MyCoord, Grid2d<MyCoord>, decltype(out.begin())> pathfinder;

  bool isNotReached = true;
  while (isNotReached) {
    // act
    grid.CopySnake(0, snake0.begin());

    auto [tailX, tailY, tailDir] = snake0[snake0.size() - 1];
    auto& [headX, headY, headDir] = snake0[0];
    
    isNotReached = !(grid.food.GetX() == headX && grid.food.GetY() == headY);

    if (isNotReached) {
      MyCoord start(headX, headY);
      pathfinder.FindPath(start, grid.food, grid, out.begin(), out.end());
      MyCoord dir = getDir(MyCoord(out[1].GetX(), out[1].GetY()), start);
      headDir = std::distance(dirs.cbegin(), std::find(dirs.cbegin(), dirs.cend(), dir));
    }
    
    moveSnake(snake0.begin(), snake0.end(), snake0.begin());

    if (grid.food.GetX() == headX && grid.food.GetY() == headY)
      snake0.emplace_back(tailX, tailY, tailDir);

    // assert
    print(snake0.begin(), snake0.end());
    grid.AddSnake(0, snake0.begin(), snake0.end());

    grid.copy(gameObjects.begin());
    for (int i = 0; auto [x, y, type] : gameObjects) {
      std::cout << (type == 0 ? "[]" : "::");
      std::cout << (++i % width == 0 ? "\n" : "");
    }
  }
}
}  // namespace snake
