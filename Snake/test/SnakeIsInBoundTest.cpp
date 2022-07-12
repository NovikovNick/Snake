#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/GameModels.h"
#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE(testSnakeisInBound) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 1 }, Direction::RIGHT)
        });
    Snake snake1 = Snake({
        std::make_pair(Coord{ 1, 3 }, Direction::DOWN),
        std::make_pair(Coord{ 1, 2 }, Direction::DOWN),
        std::make_pair(Coord{ 1, 1 }, Direction::DOWN)
        });
    std::unique_ptr<Snake> snake2(snake.move());
    std::unique_ptr<Snake> snake3(snake1.move(Direction::RIGHT));

    // act
    // assert
    BOOST_CHECK(snake.isInBound(Coord{ 0, 0 }, Coord{ 4, 2 }));
    BOOST_CHECK(snake1.isInBound(Coord{ 0, 0 }, Coord{ 2, 4 }));
    BOOST_CHECK(snake2.get()->isInBound(Coord{ 1, 0 }, Coord{ 5, 2 }));
    BOOST_CHECK(snake3.get()->isInBound(Coord{ 0, 1 }, Coord{ 3, 4 }));

    BOOST_CHECK(!snake.isInBound(Coord{ 1, 1 }, Coord{ 4, 2}));
    BOOST_CHECK(!snake.isInBound(Coord{ 0, 0 }, Coord{ 3, 1 }));
}