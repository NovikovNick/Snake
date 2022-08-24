#ifndef SNAKE_SNAKE_SNAKE_SERVICE_H_
#define SNAKE_SNAKE_SNAKE_SERVICE_H_

#include <vector>

#include "../model/common_models.h"
#include "game_models.h"

namespace snake {

class SnakeService final {
 public:
  Snake move(const Snake& snake, const Direction& dir = Direction::kNone,
             const bool& gain = false) const;
};
}  // namespace snake
#endif  // SNAKE_SNAKE_SNAKE_SERVICE_H_
