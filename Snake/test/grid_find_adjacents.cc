#define BOOST_TEST_MODULE SolutionTest
#define CASE_1 1
#define SNAKE_DEBUG 0

#include <boost/test/included/unit_test.hpp>

#include "../src/model/grid.h"
#include "../src/model/util.h"
#include "../src/model/ring_buffer.h"

namespace snake {

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 10, height = 10, size = 6;
  Grid2d grid(width, height);
  MyCoord empty;
  std::vector<MyCoord> out(4, empty);
  RingBuffer<Grid2d> buffer(size);

  // act
  for (int i = 1; i < 100; ++i) {
    buffer.add(Grid2d(i, i));
    auto& head = buffer.head();
    std::cout << std::format("{:2d}. it...\n", i);
  }

  // assert
  std::cout << "end\n";
}
#endif
}  // namespace snake
