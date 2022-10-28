#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 1

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>

namespace snake {

using NODE = std::tuple<int, int, int>;
using NODE_DATA = std::deque<NODE>;
using NODE_ITER = NODE_DATA::iterator;

std::vector<std::pair<int, int>> dirs{
    {1, 0},   // right
    {-1, 0},  // left
    {0, 1},   // bottom
    {0, -1}   // top
};

NODE moveSnake(NODE_ITER begin, NODE_ITER end, NODE_ITER out) {
  auto [prevX, prevY, prevDir] = *begin;

  while (begin != end) {
    auto [x, y, dir] = *begin;
    *out = std::make_tuple(x + dirs[dir].first, y + dirs[dir].second, prevDir);
    prevDir = dir;
    ++out;
    ++begin;
  }
  return std::make_tuple(prevX, prevY, prevDir);
}

NODE moveSnake(NODE_ITER begin, NODE_ITER end, NODE_ITER out,
               auto inserter) {
  auto lst = moveSnake(begin, end, out);
  auto [x, y, dir] = lst;
  inserter = std::make_tuple(x, y, dir);
  return lst;
}

void print(NODE_ITER begin, NODE_ITER end) {
  for (; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    std::cout << std::format("[{:2d},{:2d},{:2d}]\n", x, y, dir);
  }
}

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  Grid2d grid(10, 10);
  grid.food = {1, 8};
  NODE_DATA snake0{{2, 0, 0}, {1, 0, 0}, {0, 0, 0}};
  NODE_DATA snake1(snake0.size());
  NODE_DATA snake2(snake0.size());

  grid.AddSnake(0, snake0.begin(), snake0.end());

  // act

  grid.print();
  int rht = 0, lft = 1, btm = 2, top = 3;
  auto ins = std::back_inserter(snake0);
  for (auto dir : {btm, lft, btm, btm, btm, btm, btm, btm, btm, btm}) {
    auto [foodX, foodY] = grid.food;
    grid.CopySnake(0, snake0.begin());

    auto& [headX, headY, headDir] = snake0[0];
    headDir = dir;

    std::cout << " ------ \n";

    if (foodX == headX && foodY == headY) {
      moveSnake(snake0.begin(), snake0.end(), snake0.begin(),
                std::front_inserter(snake0));
    } else {
      moveSnake(snake0.begin(), snake0.end(), snake0.begin());
    }
    print(snake0.begin(), snake0.end());

    grid.AddSnake(0, snake0.begin(), snake0.end());
    grid.print();
  }

  // assert
  // print(snake1.begin(), snake1.end());
}

}  // namespace snake
