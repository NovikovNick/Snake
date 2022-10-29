#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 1

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>

namespace snake {

std::vector<std::pair<int, int>> dirs{
    {1, 0},   // right
    {-1, 0},  // left
    {0, 1},   // bottom
    {0, -1}   // top
};

void moveSnake(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end,
               SNAKE_DATA_ITERATOR out) {
  for (auto [_, __, prevDir] = *begin; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    *out = std::make_tuple(x + dirs[dir].first, y + dirs[dir].second, prevDir);
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

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 40, height = 10;
  Grid2d grid(width, height);
  grid.food = {1, 8};

  SNAKE_DATA snake0{{2, 0, 0}, {1, 0, 0}, {0, 0, 0}};
  grid.AddSnake(0, snake0.begin(), snake0.end());

  std::vector<GAME_OBJECT> gameObjects(width * height);

  int rht = 0, lft = 1, btm = 2, top = 3;
  for (auto dir :
       {btm, lft, btm, btm, btm, btm, btm, btm, btm, rht, rht, rht}) {
    // act
    grid.CopySnake(0, snake0.begin());

    auto [tailX, tailY, tailDir] = snake0[snake0.size() - 1];
    auto& [headX, headY, headDir] = snake0[0];
    headDir = dir;
    moveSnake(snake0.begin(), snake0.end(), snake0.begin());

    auto [foodX, foodY] = grid.food;
    if (foodX == headX && foodY == headY)
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
