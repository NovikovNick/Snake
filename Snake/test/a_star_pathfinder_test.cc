#define BOOST_TEST_MODULE SolutionTest

#include <boost/test/included/unit_test.hpp>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star.cc"

namespace snake {

struct TestNode {
 private:
  int x_, y_;

 public:
  TestNode() : x_(-1), y_(-1){};
  TestNode(const int x, const int y) : x_(x), y_(y){};
  int GetX() const { return x_; };
  int GetY() const { return y_; };
};

struct Grid2d {
 private:
  const int width_, height_;

 public:
  Grid2d(int width, int height) : width_(width), height_(height){};
  void FindAdjacents(const int x, const int y) const {};
};

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  std::vector<TestNode> out(16);

  TestNode start(0, 0);
  TestNode goal(9, 9);

  Grid2d grid(10, 10);
  AStarPathfinder<TestNode, Grid2d, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin());

  // assert
  for (auto it : out) {
    std::cout << it.GetX() << "." << it.GetY() << " ";
  }
}

}  // namespace snake
