#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 1

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
  bool operator==(const TestNode& other) const {
    return (x_ == other.GetX()) && (y_ == other.GetY());
  };
  bool operator!=(const TestNode& other) const { return !(*this == other); };
  std::string ToString() const {
    return std::format("[{:2d},{:2d}]", GetX(), GetY());
  }
};

template <grid_2d_cell N>
struct Grid2d {
 private:
  std::vector<N> grid_;
  int width_, height_;

 public:
  Grid2d(int width, int height)
      : width_(width), height_(height), grid_(std::vector<N>(width * height)) {
    for (int row = 0; row < height; ++row) {
      for (int col = 0; col < width; ++col) {
        grid_[row * width + col] = N(col, row);
      }
    }
  };

  void FindAdjacents(int x, int y, std::vector<N>::iterator out) const {
    if (y + 1 < height_) *(out++) = grid_[width_ * (y + 1) + (x + 0)];
    if (x + 1 < width_) *(out++) = grid_[width_ * (y + 0) + (x + 1)];
    if (y - 1 >= 0) *(out++) = grid_[width_ * (y - 1) + (x + 0)];
    if (x - 1 >= 0) *(out) = grid_[width_ * (y + 0) + (x - 1)];
  }
};

float getDistance(const TestNode& n1, const TestNode& n2) {
  auto [x0, x1] = std::minmax(n1.GetX(), n2.GetX());
  auto [y0, y1] = std::minmax(n1.GetY(), n2.GetY());
  return std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2));
}

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  std::vector<TestNode> out(5);
  TestNode start(0, 0);
  TestNode goal(0, 4);
  Grid2d<TestNode> grid(10, 10);
  AStarPathfinder<TestNode, Grid2d<TestNode>, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  auto itFst = out.begin();
  auto itSnd = itFst + 1;

  while (itSnd != out.end()) {
#if SNAKE_DEBUG
    std::cout << "checking distance benween " << (*itFst).ToString() << " and "
              << (*itSnd).ToString() << std::endl;
#endif
    BOOST_CHECK_EQUAL(1, getDistance(*itFst, *itSnd));
    ++itFst;
    ++itSnd;
  }
  BOOST_CHECK(goal == *out.rbegin());
}

BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  std::vector<TestNode> out(5);
  TestNode start(0, 0);
  TestNode goal(4, 0);
  Grid2d<TestNode> grid(10, 10);
  AStarPathfinder<TestNode, Grid2d<TestNode>, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  auto itFst = out.begin();
  auto itSnd = itFst + 1;
  while (itSnd != out.end()) {
#if SNAKE_DEBUG
    std::cout << "checking distance benween " << (*itFst).ToString() << " and "
              << (*itSnd).ToString() << std::endl;
#endif
    BOOST_CHECK_EQUAL(1, getDistance(*itFst, *itSnd));
    ++itFst;
    ++itSnd;
  }
  BOOST_CHECK(goal == *out.rbegin());
}

BOOST_AUTO_TEST_CASE(case3) {
  // arrange
  std::vector<TestNode> out(9);
  TestNode start(0, 0);
  TestNode goal(4, 4);
  Grid2d<TestNode> grid(10, 10);
  AStarPathfinder<TestNode, Grid2d<TestNode>, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  auto itFst = out.begin();
  auto itSnd = itFst + 1;
  while (itSnd != out.end()) {
#if SNAKE_DEBUG
    std::cout << "checking distance benween " << (*itFst).ToString() << " and "
              << (*itSnd).ToString() << std::endl;
#endif
    BOOST_CHECK_EQUAL(1, getDistance(*itFst, *itSnd));
    ++itFst;
    ++itSnd;
  }
  BOOST_CHECK(goal == *out.rbegin());
}
}  // namespace snake
