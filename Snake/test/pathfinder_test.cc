#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/pathfinder.h"
#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE(testPathfinder) {
    using namespace snake;
    
    // arrange
    Pathfinder pathfinder;

    // act
    Path path1 = pathfinder.findPath(Coord{ 1, 1 }, Coord{ 2, 1 });
    Path path2 = pathfinder.findPath(Coord{ 1, 1 }, Coord{ 0, 1 });
    Path path3 = pathfinder.findPath(Coord{ 1, 1 }, Coord{ 1, 0 });
    Path path4 = pathfinder.findPath(Coord{ 1, 1 }, Coord{ 1, 2 });
    
    // assert
    BOOST_CHECK(Direction::RIGHT == path1.path[0]);
    BOOST_CHECK(Direction::LEFT  == path2.path[0]);
    BOOST_CHECK(Direction::UP    == path3.path[0]);
    BOOST_CHECK(Direction::DOWN  == path4.path[0]);
}
