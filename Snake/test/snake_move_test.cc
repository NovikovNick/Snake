#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/game_models.h"
#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE(testSnakeMoveWithoutArgs) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 1 }, Direction::RIGHT)
        });

    // act
    std::unique_ptr<Snake> res(snake.move());

    // assert
    BOOST_CHECK_EQUAL(4, res.get()->getParts()[0].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[0].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[0].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[1].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[1].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[1].second);

    BOOST_CHECK_EQUAL(2, res.get()->getParts()[2].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[2].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeMoveWithOppositeDirection) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 1 }, Direction::RIGHT)
        });

    // act
    std::unique_ptr<Snake> res(snake.move(Direction::LEFT));

    // assert
    BOOST_CHECK_EQUAL(4, res.get()->getParts()[0].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[0].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[0].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[1].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[1].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[1].second);

    BOOST_CHECK_EQUAL(2, res.get()->getParts()[2].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[2].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeMoveWithDirection) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 1 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 1 }, Direction::RIGHT)
        });

    // act
    std::unique_ptr<Snake> res(snake.move(Direction::UP));

    // assert
    BOOST_CHECK_EQUAL(3, res.get()->getParts()[0].first.x);
    BOOST_CHECK_EQUAL(0, res.get()->getParts()[0].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[0].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[1].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[1].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[1].second);

    BOOST_CHECK_EQUAL(2, res.get()->getParts()[2].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[2].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeSeveralMoves) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 2 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 2 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 2 }, Direction::RIGHT)
        });

    // act
    std::unique_ptr<Snake> snake2(snake.move(Direction::UP));
    std::unique_ptr<Snake> res(snake2.get()->move());

    // assert
    BOOST_CHECK_EQUAL(3, res.get()->getParts()[0].first.x);
    BOOST_CHECK_EQUAL(0, res.get()->getParts()[0].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[0].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[1].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[1].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[1].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[2].first.x);
    BOOST_CHECK_EQUAL(2, res.get()->getParts()[2].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeSeveralMovesAndGain) {
    using namespace snake;
    // arrange
    Snake snake = Snake({
        std::make_pair(Coord{ 3, 2 }, Direction::RIGHT),
        std::make_pair(Coord{ 2, 2 }, Direction::RIGHT),
        std::make_pair(Coord{ 1, 2 }, Direction::RIGHT)
        });

    // act
    std::unique_ptr<Snake> snake2(snake.move(Direction::UP, true));
    std::unique_ptr<Snake> res(snake2.get()->move());

    // assert
    BOOST_CHECK_EQUAL(3, res.get()->getParts()[0].first.x);
    BOOST_CHECK_EQUAL(0, res.get()->getParts()[0].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[0].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[1].first.x);
    BOOST_CHECK_EQUAL(1, res.get()->getParts()[1].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[1].second);

    BOOST_CHECK_EQUAL(3, res.get()->getParts()[2].first.x);
    BOOST_CHECK_EQUAL(2, res.get()->getParts()[2].first.y);
    BOOST_CHECK(Direction::UP == res.get()->getParts()[2].second);

    BOOST_CHECK_EQUAL(2, res.get()->getParts()[3].first.x);
    BOOST_CHECK_EQUAL(2, res.get()->getParts()[3].first.y);
    BOOST_CHECK(Direction::RIGHT == res.get()->getParts()[3].second);
}