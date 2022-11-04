#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 0

#include <boost/test/included/unit_test.hpp>

#include "../src/model/grid.h"
#include "../src/model/util.h"

namespace snake {

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 10, height = 10;
  Grid2d grid(width, height);
  MyCoord empty;
  std::vector<MyCoord> out(4, empty);
  
  
  // act
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      grid.FindAdjacents(x, y, out.begin());

      std::cout << x << "," << y << ": ";
      for (auto it : out) {
        if (it != empty) {
          std::cout << it.ToString() << ", ";
        }
      }
      std::cout << std::endl;

      std::fill(out.begin(), out.end(), empty);
    }
  }
  // assert
  // grid.copy();
}
}  // namespace snake
