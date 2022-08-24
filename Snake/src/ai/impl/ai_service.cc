#include "../ai_service.h"

#include <stdlib.h>

#include <queue>
#include <unordered_map>

#include "../pathfinder.h"

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

Pathfinder pathfinder;

}  // namespace

InputDTO AIService::findPath(const IPathFindRequest& req) {
  Path path = pathfinder.FindPath(req);
  return InputDTO{{{path.path[0]}}, path.ctx};
}

}  // namespace snake
