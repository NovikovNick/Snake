#define BOOST_TEST_MODULE SolutionTest

#include <boost/test/included/unit_test.hpp>

#include "../src/ai/a_star.h"

namespace snake {

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  AStarPathfinder pathfinder;
  // act
  pathfinder.FindPath(TestNode(0, 0));
  // assert
}

}  // namespace snake
