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
  Grid2d grid(3, 3, 1);
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  int snake_id = 0;

  // act
  grid.AddSnake(snake_id, snake0.begin(), snake0.end());
  grid.RebuildFilled(snake_id);

  // assert
  BOOST_CHECK(grid.IsSnake(2, 2));
  BOOST_CHECK(grid.IsSnake(2, 1));
  BOOST_CHECK(grid.IsSnake(2, 0));

  BOOST_CHECK(!grid.IsSnake(1, 2));
  BOOST_CHECK(!grid.IsSnake(1, 1));
  BOOST_CHECK(!grid.IsSnake(1, 0));

  BOOST_CHECK(!grid.IsSnake(0, 2));
  BOOST_CHECK(!grid.IsSnake(0, 1));
  BOOST_CHECK(!grid.IsSnake(0, 0));
}
#endif
}  // namespace snake
