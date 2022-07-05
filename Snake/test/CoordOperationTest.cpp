#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/GameModels.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testMovingCoordToLeftDirection) {

    using namespace snake;

    // arrange
    Coord a = { 1, 1 };
    Direction dir = snake::Direction::LEFT;

    // act 
    Coord b = a + dir;

    // assert 
    BOOST_CHECK_EQUAL(b.x, 0);
    BOOST_CHECK_EQUAL(b.y, 1);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToRightDirection) {

    using namespace snake;

    // arrange
    Coord a = { 1, 1 };
    Direction dir = snake::Direction::RIGHT;

    // act 
    Coord b = a + dir;

    // assert 
    BOOST_CHECK_EQUAL(b.x, 2);
    BOOST_CHECK_EQUAL(b.y, 1);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToTopDirection) {

    using namespace snake;

    // arrange
    Coord a = { 1, 1 };
    Direction dir = snake::Direction::UP;

    // act 
    Coord b = a + dir;

    // assert 
    BOOST_CHECK_EQUAL(b.x, 1);
    BOOST_CHECK_EQUAL(b.y, 0);
}

BOOST_AUTO_TEST_CASE(testMovingCoordToDownDirection) {

    using namespace snake;

    // arrange
    Coord a = { 1, 1 };
    Direction dir = snake::Direction::DOWN;

    // act 
    Coord b = a + dir;

    // assert 
    BOOST_CHECK_EQUAL(b.x, 1);
    BOOST_CHECK_EQUAL(b.y, 2);
}

BOOST_AUTO_TEST_CASE(testCoordEqualOperator) {

    using namespace snake;

    // arrange
    Coord a = { 1, 1 };
    Coord b = { 1, 1 };
    Coord c = { 2, 1 };
    Coord d = { 1, 2 };

    // act
    // assert 
    BOOST_CHECK_EQUAL(true, a == b);
    BOOST_CHECK_EQUAL(false, b == c);
    BOOST_CHECK_EQUAL(false, a == c);
    BOOST_CHECK_EQUAL(false, c == d);
}