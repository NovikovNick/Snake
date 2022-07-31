#ifndef SNAKE_PATHFINDER_H_
#define SNAKE_PATHFINDER_H_

#include "game_models.h"

#include <vector>

namespace snake {

struct Pathfinder {

    Path findPath(
        const Coord& start, 
        const Coord& to, 
        const std::vector<Coord>& barriers = {});
};

} // namespace snake

#endif  // SNAKE_PATHFINDER_H_
