#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 0

#include <boost/test/included/unit_test.hpp>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star_dp.cc"
#include "../src/model/grid.h"

namespace snake {

float getDistance(const MyCoord& n1, const MyCoord& n2) {
  return std::sqrt(std::pow(n1.GetX() - n2.GetX(), 2) +
                   std::pow(n1.GetY() - n2.GetY(), 2));
}

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  std::vector<MyCoord> out(5);
  MyCoord start(0, 0);
  MyCoord goal(0, 4);
  Grid2d grid(10, 10);
  AStarPathfinder<MyCoord, Grid2d, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  MyCoord empty, last;
  for (auto fst = out.begin(), snd = std::next(fst), end = out.end();
       snd != end && *snd != empty; last = *(fst = snd++)) {
    debug("Dist benween {} and {}\n", (*fst).ToString(), (*snd).ToString());
    BOOST_CHECK_EQUAL(1, getDistance(*fst, *snd));
  }
  BOOST_CHECK(goal == last);
}

BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  std::vector<MyCoord> out(5);
  MyCoord start(0, 0);
  MyCoord goal(4, 0);
  Grid2d grid(10, 10);
  AStarPathfinder<MyCoord, Grid2d, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  MyCoord empty, last;
  for (auto fst = out.begin(), snd = std::next(fst), end = out.end();
       snd != end && *snd != empty; last = *(fst = snd++)) {
    debug("Dist benween {} and {}\n", (*fst).ToString(), (*snd).ToString());
    BOOST_CHECK_EQUAL(1, getDistance(*fst, *snd));
  }
  BOOST_CHECK(goal == last);
}

BOOST_AUTO_TEST_CASE(case3) {
  // arrange
  std::vector<MyCoord> out(9);
  MyCoord start(0, 0);
  MyCoord goal(4, 4);
  Grid2d grid(10, 10);
  AStarPathfinder<MyCoord, Grid2d, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  MyCoord empty, last;
  for (auto fst = out.begin(), snd = std::next(fst), end = out.end();
       snd != end && *snd != empty; last = *(fst = snd++)) {
    debug("Dist benween {} and {}\n", (*fst).ToString(), (*snd).ToString());
    BOOST_CHECK_EQUAL(1, getDistance(*fst, *snd));
  }
  BOOST_CHECK(goal == last);
}

BOOST_AUTO_TEST_CASE(case4) {
  // arrange
  std::vector<MyCoord> out(400);
  MyCoord start(0, 0);
  MyCoord goal(0, 19);
  Grid2d grid(20, 20);
  SNAKE_DATA barrier;
  for (int i = 0; i < 19; ++i) {
    barrier.push_back({i, 6, 0});
  }
  grid.AddSnake(0, barrier.begin(), barrier.end());
  grid.RebuildFilled();

  AStarPathfinder<MyCoord, Grid2d, decltype(out.begin())> pathfinder;

  // act
  pathfinder.FindPath(start, goal, grid, out.begin(), out.end());

  // assert
  MyCoord empty, last;
  for (auto fst = out.begin(), snd = std::next(fst), end = out.end();
       snd != end && *snd != empty; last = *(fst = snd++)) {
    debug("Dist benween {} and {}\n", (*fst).ToString(), (*snd).ToString());
    BOOST_CHECK_EQUAL(1, getDistance(*fst, *snd));
  }
  BOOST_CHECK(goal == last);
}

// todo add heavy test case!
}  // namespace snake
