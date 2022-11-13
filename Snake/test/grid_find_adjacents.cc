#define BOOST_TEST_MODULE SolutionTest
#define CASE_4 1
#define CASE_3 0
#define CASE_2 0
#define CASE_1 0
#define SNAKE_DEBUG 1

#include <boost/test/included/unit_test.hpp>

#include "../src/model/grid.h"
#include "../src/model/ring_buffer.h"
#include "../src/service/v2_game_state_service.h"
#include "../src/util.h"
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

#if CASE_2
BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  Grid2d grid(3, 3, 1);
  SNAKE_DATA snake0{{2, 0, 2}, {1, 0, 0}, {0, 0, 0}};
  int snake_id = 0;

  GameStateService servise(10, 10, std::shared_ptr<AIService>(nullptr));

  for (int i = 0; i < 10; ++i) {
    debug("--------- {} ---------\n", i);
    servise.MoveSnake(snake0.begin(), snake0.end(), snake0.begin());
    servise.Print(snake0.begin(), snake0.end());
  }

  // act
  // assert
}
#endif

#if CASE_3
BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  GridCell o1{2, 5}, o2{5, 2};

  // act
  auto [sum_x, sum_y] = o1 + o2;
  auto [diff_x, diff_y] = o1 - o2;

  // assert
  debug("sum  = [{:2d};{:2d}]\n", sum_x, sum_y);
  BOOST_CHECK_EQUAL(7, sum_x);
  BOOST_CHECK_EQUAL(7, sum_y);

  debug("diff = [{:2d};{:2d}]\n", diff_x, diff_y);
  BOOST_CHECK_EQUAL(-3, diff_x);
  BOOST_CHECK_EQUAL(3, diff_y);
}
#endif

#if CASE_4
BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  RingBuffer<int> buf(16);

  // act
  for (int i = 0; i < 50; ++i) {
    buf.Add(i + 0);
    // todo fix iterating and change deque to vector
    debug("{:2d}: ", i);
    for (int j = 0; j < buf.Size(); ++j) {
      debug("{:2d} ", buf[j]);
    }
    debug("and head is {}\n", buf[0]);
  }

  // assert
}
#endif
}  // namespace snake
