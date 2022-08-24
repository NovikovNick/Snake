#define BOOST_TEST_MODULE SolutionTest

#include <boost/test/included/unit_test.hpp>

#include "../src/model/common_models.h"
#include "../src/service/game_models.h"
#include "../src/service/snake_service.h"

BOOST_AUTO_TEST_CASE(testSnakeMoveWithoutArgs) {
  using namespace snake;

  // arrange
  SnakeService srv{};
  auto snake = Snake({std::make_pair(Coord{3, 1}, Direction::kRight),
                      std::make_pair(Coord{2, 1}, Direction::kRight),
                      std::make_pair(Coord{1, 1}, Direction::kRight)});

  // act
  auto res = srv.move(snake);

  // assert
  BOOST_CHECK_EQUAL(4, res.getParts()[0].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[0].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[0].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[1].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[1].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[1].second);

  BOOST_CHECK_EQUAL(2, res.getParts()[2].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[2].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeMoveWithOppositeDirection) {
  using namespace snake;
  // arrange
  SnakeService srv{};
  auto snake = Snake({std::make_pair(Coord{3, 1}, Direction::kRight),
                      std::make_pair(Coord{2, 1}, Direction::kRight),
                      std::make_pair(Coord{1, 1}, Direction::kRight)});

  // act
  auto res = srv.move(snake, Direction::kLeft);

  // assert
  BOOST_CHECK_EQUAL(4, res.getParts()[0].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[0].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[0].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[1].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[1].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[1].second);

  BOOST_CHECK_EQUAL(2, res.getParts()[2].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[2].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeMoveWithDirection) {
  using namespace snake;
  // arrange
  SnakeService srv{};
  Snake snake = Snake({std::make_pair(Coord{3, 1}, Direction::kRight),
                       std::make_pair(Coord{2, 1}, Direction::kRight),
                       std::make_pair(Coord{1, 1}, Direction::kRight)});

  // act
  auto res = srv.move(snake, Direction::kUp);

  // assert
  BOOST_CHECK_EQUAL(3, res.getParts()[0].first.x);
  BOOST_CHECK_EQUAL(0, res.getParts()[0].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[0].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[1].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[1].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[1].second);

  BOOST_CHECK_EQUAL(2, res.getParts()[2].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[2].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeSeveralMoves) {
  using namespace snake;
  // arrange
  SnakeService srv{};
  auto snake = Snake({std::make_pair(Coord{3, 2}, Direction::kRight),
                      std::make_pair(Coord{2, 2}, Direction::kRight),
                      std::make_pair(Coord{1, 2}, Direction::kRight)});

  // act
  auto snake2 = srv.move(snake, Direction::kUp);
  auto res = srv.move(snake2);

  // assert
  BOOST_CHECK_EQUAL(3, res.getParts()[0].first.x);
  BOOST_CHECK_EQUAL(0, res.getParts()[0].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[0].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[1].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[1].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[1].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[2].first.x);
  BOOST_CHECK_EQUAL(2, res.getParts()[2].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[2].second);
}

BOOST_AUTO_TEST_CASE(testSnakeSeveralMovesAndGain) {
  using namespace snake;
  // arrange
  SnakeService srv{};
  auto snake = Snake({std::make_pair(Coord{3, 2}, Direction::kRight),
                      std::make_pair(Coord{2, 2}, Direction::kRight),
                      std::make_pair(Coord{1, 2}, Direction::kRight)});

  // act
  auto snake2 = srv.move(snake, Direction::kUp, true);
  auto res = srv.move(snake2);

  // assert
  BOOST_CHECK_EQUAL(3, res.getParts()[0].first.x);
  BOOST_CHECK_EQUAL(0, res.getParts()[0].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[0].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[1].first.x);
  BOOST_CHECK_EQUAL(1, res.getParts()[1].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[1].second);

  BOOST_CHECK_EQUAL(3, res.getParts()[2].first.x);
  BOOST_CHECK_EQUAL(2, res.getParts()[2].first.y);
  BOOST_CHECK(Direction::kUp == res.getParts()[2].second);

  BOOST_CHECK_EQUAL(2, res.getParts()[3].first.x);
  BOOST_CHECK_EQUAL(2, res.getParts()[3].first.y);
  BOOST_CHECK(Direction::kRight == res.getParts()[3].second);
}
