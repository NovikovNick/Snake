#define BOOST_TEST_MODULE SolutionTest
#define CASE_2 1
#define CASE_1 0
#define SNAKE_DEBUG 1

#include <boost/test/included/unit_test.hpp>

#include "../src/v2/model/grid.h"
#include "../src/v2/model/ring_buffer.h"
#include "../src/v2/util.h"
#include "../src/v2/v2_game_state_service.h"

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
}  // namespace snake
