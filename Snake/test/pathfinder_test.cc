#define BOOST_TEST_MODULE SolutionTest

#include "../src/ai/pathfinder.h"
#include "../src/ai/ai_service.h"

#include <boost/test/included/unit_test.hpp>

namespace snake {
namespace {
struct TestDTO : public IPathFindRequest {
 private:
  Coord from_;
  Coord to_;
  std::vector<Coord> barriers_;

 public:
  TestDTO(const Coord& from, const Coord& to,
          const std::vector<Coord>& barriers = {})
      : from_(from), to_(to), barriers_(barriers){};

  virtual const Coord& getFrom() const override { return from_; };
  virtual const Coord& getTo() const override { return to_; };
  virtual const std::vector<Coord>& getBarriers() const override {
    return barriers_;
  };
};
}  // namespace

BOOST_AUTO_TEST_CASE(testPathfinder) {
  // arrange
  Pathfinder pathfinder;

  // act
  Path path1 = pathfinder.FindPath(TestDTO(Coord{1, 1}, Coord{2, 1}));
  Path path2 = pathfinder.FindPath(TestDTO(Coord{1, 1}, Coord{0, 1}));
  Path path3 = pathfinder.FindPath(TestDTO(Coord{1, 1}, Coord{1, 0}));
  Path path4 = pathfinder.FindPath(TestDTO(Coord{1, 1}, Coord{1, 2}));

  // assert
  BOOST_CHECK(Direction::kRight == path1.path[0]);
  BOOST_CHECK(Direction::kLeft == path2.path[0]);
  BOOST_CHECK(Direction::kUp == path3.path[0]);
  BOOST_CHECK(Direction::kDown == path4.path[0]);
}

}  // namespace snake
