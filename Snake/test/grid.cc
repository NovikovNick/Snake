#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 0

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>
#include <stack>

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
    *out = std::make_tuple(x + dirs[dir].GetX(), y + dirs[dir].GetY(), prevDir);
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

void print(const int width, GAME_OBJECT_ITERATOR begin,
           GAME_OBJECT_ITERATOR end) {
  for (int i = 0; begin != end; ++begin) {
    auto [x, y, type] = *begin;
    switch (type) {
      case 0:
        std::cout << "[]";
        break;
      case 1:
        std::cout << "::";
        break;
      case 2:
        std::cout << "**";
        break;
      default:
        std::cout << "XX";
        break;
    }
    std::cout << (++i % width == 0 ? "\n" : "");
  }
}

MyCoord getDir(const MyCoord& o1, const MyCoord& o2) {
  return MyCoord(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
}

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 15, height = 15;
  Grid2d grid(width, height);

  std::stack<MyCoord> foods;
  foods.push(MyCoord(14, 0));
  foods.push(MyCoord(0, 14));
  foods.push(MyCoord(0, 0));

  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  grid.AddSnake(0, snake0.begin(), snake0.end());
  SNAKE_DATA barrier{{0, 3, 0}, {1, 3, 0}, {2, 3, 0}, {3, 3, 0}, {4, 3, 0}};
  grid.AddSnake(1, barrier.begin(), barrier.end());

  std::vector<GAME_OBJECT> gameObjects(width * height);

  std::vector<MyCoord> out(5);
  AStarPathfinder<MyCoord, Grid2d, decltype(out.begin())> pathfinder;

  grid.food = foods.top();
  foods.pop();

  while (!foods.empty()) {
    // act
    grid.CopySnake(0, snake0.begin());

    auto& [headX, headY, headDir] = snake0[0];

    if (!(grid.food.GetX() == headX && grid.food.GetY() == headY)) {
      MyCoord start(headX, headY);
      pathfinder.FindPath(start, grid.food, grid, out.begin(), out.end());
      MyCoord dir = getDir(MyCoord(out[1].GetX(), out[1].GetY()), start);
      headDir = std::distance(dirs.cbegin(),
                              std::find(dirs.cbegin(), dirs.cend(), dir));
    }

    auto [tailX, tailY, tailDir] = snake0[snake0.size() - 1];

    moveSnake(snake0.begin(), snake0.end(), snake0.begin());

    if (grid.food.GetX() == headX && grid.food.GetY() == headY) {
      snake0.emplace_back(tailX, tailY, tailDir);
      grid.food = foods.top();
      foods.pop();
    }
    grid.AddSnake(0, snake0.begin(), snake0.end());
    grid.copy(gameObjects.begin());

    // assert
    print(snake0.begin(), snake0.end());
    print(width, gameObjects.begin(), gameObjects.end());
  }
}
}  // namespace snake
