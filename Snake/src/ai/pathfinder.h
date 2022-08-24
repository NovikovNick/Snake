#ifndef SNAKE_PATHFINDER_H_
#define SNAKE_PATHFINDER_H_

#include <vector>

#include "../model/common_models.h"
#include "../service/game_models.h"
#include "i_path_find_request.h"

namespace snake {

class Pathfinder final {
 public:
  Path FindPath(const IPathFindRequest& req);
};

}  // namespace snake

#endif  // SNAKE_PATHFINDER_H_
