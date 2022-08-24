#define BOOST_TEST_MODULE SolutionTest

#include <boost/test/included/unit_test.hpp>

#include "../src/service/game_models.h"

namespace snake {
BOOST_AUTO_TEST_CASE(testMovingCoordToLeftDirection) {
  // arrange
  Coord a = {1, 1};
  Direction dir = Direction::kLeft;

  // act
  Coord b = a + dir;

  // assert
  BOOST_CHECK_EQUAL(b.x, 0);
  BOOST_CHECK_EQUAL(b.y, 1);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToRightDirection) {
  // arrange
  Coord a = {1, 1};
  Direction dir = Direction::kRight;

  // act
  Coord b = a + dir;

  // assert
  BOOST_CHECK_EQUAL(b.x, 2);
  BOOST_CHECK_EQUAL(b.y, 1);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToTopDirection) {
  // arrange
  Coord a = {1, 1};
  Direction dir = Direction::kUp;

  // act
  Coord b = a + dir;

  // assert
  BOOST_CHECK_EQUAL(b.x, 1);
  BOOST_CHECK_EQUAL(b.y, 0);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToDownDirection) {
  // arrange
  Coord a = {1, 1};
  Direction dir = Direction::kDown;

  // act
  Coord b = a + dir;

  // assert
  BOOST_CHECK_EQUAL(b.x, 1);
  BOOST_CHECK_EQUAL(b.y, 2);
}

BOOST_AUTO_TEST_CASE(testCoordEqualOperator) {
  // arrange
  Coord a = {1, 1};
  Coord b = {1, 1};
  Coord c = {2, 1};
  Coord d = {1, 2};

  // act
  // assert
  BOOST_CHECK_EQUAL(true, a == b);
  BOOST_CHECK_EQUAL(false, b == c);
  BOOST_CHECK_EQUAL(false, a == c);
  BOOST_CHECK_EQUAL(false, c == d);
}
}  // namespace snake
