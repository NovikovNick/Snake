#define BOOST_TEST_MODULE SolutionTest
#define CASE_1 1
#define SNAKE_DEBUG 1

#include <boost/test/included/unit_test.hpp>

#include "../src/model/grid.h"
#include "../src/model/ring_buffer.h"
#include "../src/model/util.h"

namespace snake {

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int size = 6;
  RingBuffer<int> buffer(size);

  // act
  for (int i = 0; i < 100; ++i) {
    buffer.add(i + 0);
    auto& head = buffer.head();
    debug("{:3d}: ", head);
    for (int j = 0; j < buffer.size(); ++j) {
      debug("{:2d} ", buffer[j]);
    }
    debug("\n");
  }

  // assert
  debug("end\n");
}
#endif
}  // namespace snake
