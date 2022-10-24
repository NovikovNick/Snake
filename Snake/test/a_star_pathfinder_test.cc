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
    for (int row = 0; row < width; ++row) {
      for (int col = 0; col < height; ++col) {
        grid_[row * width + col] = N(row, col);
      }
    }
  };

  void FindAdjacents(int x, int y, std::vector<N>::iterator out) const {
    std::cout << "find anjacents" << std::endl;
    if (y + 1 < height_) *(out++) = grid_[width_ * (y + 1) + (x + 0)];
    if (x + 1 < width_) *(out++) = grid_[width_ * (y + 0) + (x + 1)];
    if (y - 1 >= 0) *(out++) = grid_[width_ * (y - 1) + (x + 0)];
    if (x - 1 >= 0) *(out) = grid_[width_ * (y + 0) + (x - 1)];
  }
};

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  std::vector<TestNode> out(100);
  TestNode start(0, 0);
  TestNode goal(9, 9);
  Grid2d<TestNode> grid(10, 10);
  AStarPathfinder<TestNode, Grid2d<TestNode>, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin());

  // assert
  TestNode empty;
  std::cout << "path: ";
  for (auto it : out) {
    if (it != empty) std::cout << it.ToString() << " => ";
  }
}

}  // namespace snake
