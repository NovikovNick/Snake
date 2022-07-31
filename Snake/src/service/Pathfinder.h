#ifndef SNAKE_PATHFINDER_H_
#define SNAKE_PATHFINDER_H_

#include <vector>

#include "game_models.h"

namespace snake {

class Pathfinder final {
 public:
  Path FindPath(const Coord& start, const Coord& to,
                const std::vector<Coord>& barriers = {});
};

}  // namespace snake

#endif  // SNAKE_PATHFINDER_H_
