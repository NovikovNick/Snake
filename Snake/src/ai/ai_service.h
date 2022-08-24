#ifndef SNAKE_SNAKE_AI_SERVICE_H_
#define SNAKE_SNAKE_AI_SERVICE_H_

#include <vector>

#include "../model/common_models.h"
#include "../service/game_models.h"
#include "i_path_find_request.h"

namespace snake {

class AIService final {
 public:
  InputDTO findPath(const IPathFindRequest& req);
};
}  // namespace snake
#endif  // SNAKE_SNAKE_AI_SERVICE_H_
