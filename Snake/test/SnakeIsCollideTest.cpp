#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/GameModels.h"
#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE(testSnakeIsCollide) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 1 }, Direction::RIGHT)
        });

    // act
    // assert
    BOOST_CHECK(snake.isCollide(Coord{ 3, 1 }));
    BOOST_CHECK(snake.isCollide(Coord{ 2, 1 }));
    BOOST_CHECK(snake.isCollide(Coord{ 1, 1 }));

    BOOST_CHECK(!snake.isCollide(Coord{ 0, 0 }));
    BOOST_CHECK(!snake.isCollide(Coord{ 5, 5 }));
}