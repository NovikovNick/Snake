#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/game_models.h"
#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE(testSnakeIsValid) {
    using namespace snake;
    // arrange
    Snake* snake = new Snake({
        std::make_pair(Coord{10, 4 }, Direction::RIGHT),
        std::make_pair(Coord{9, 4 }, Direction::RIGHT),
        std::make_pair(Coord{9, 3 }, Direction::DOWN),
        std::make_pair(Coord{10, 3 }, Direction::LEFT),
        std::make_pair(Coord{11, 3 }, Direction::LEFT),
        std::make_pair(Coord{11, 4 }, Direction::UP),
        std::make_pair(Coord{11, 5 }, Direction::UP),
        std::make_pair(Coord{11, 6 }, Direction::UP),
        std::make_pair(Coord{11, 7 }, Direction::UP),
        std::make_pair(Coord{11, 8 }, Direction::UP),
        std::make_pair(Coord{11, 9 }, Direction::UP)
    });
    Snake* snake2 = snake->move();


    // act
    // assert
    BOOST_CHECK(snake->isValid());
    BOOST_CHECK(!snake2->isValid());

    delete snake;
    delete snake2;
}
