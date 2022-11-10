#define BOOST_TEST_MODULE SolutionTest
#define CASE_1 1
#define CASE_2 1
#define CASE_3 1
#define CASE_4 1
#define SNAKE_DEBUG 0

#include <boost/test/included/unit_test.hpp>

#include "../src/v2/a_star.h"
#include "../src/v2/impl/a_star_dp.cc"
#include "../src/v2/model/grid.h"

namespace snake {

float getDistance(const MyCoord& n1, const MyCoord& n2) {
  return std::sqrt(std::pow(n1.GetX() - n2.GetX(), 2) +
                   std::pow(n1.GetY() - n2.GetY(), 2));
}

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  std::vector<MyCoord> out(5);
  MyCoord start(0, 0);
  MyCoord goal(0, 4);
  Grid2d grid(10, 10, 1);
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
#endif

#if CASE_2
BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  std::vector<MyCoord> out(5);
  MyCoord start(0, 0);
  MyCoord goal(4, 0);
  Grid2d grid(10, 10, 1);
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
#endif

#if CASE_3
BOOST_AUTO_TEST_CASE(case3) {
  // arrange
  std::vector<MyCoord> out(9);
  MyCoord start(0, 0);
  MyCoord goal(4, 4);
  Grid2d grid(10, 10, 1);
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
#endif

#if CASE_4
BOOST_AUTO_TEST_CASE(case4) {
  // arrange
  std::vector<MyCoord> out(400);
  MyCoord start(0, 0);
  MyCoord goal(0, 19);
  Grid2d grid(20, 20, 1);
  SNAKE_DATA barrier;
  for (int i = 0; i < 19; ++i) {
    barrier.push_back({i, 6, 0});
  }
  grid.AddSnake(0, barrier.begin(), barrier.end());
  grid.RebuildFilled(0);

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
#endif

// todo add heavy test case!
}  // namespace snake
