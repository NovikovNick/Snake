#ifndef SNAKE_SNAKE_AI_SERVICE_H_
#define SNAKE_SNAKE_AI_SERVICE_H_

#include <vector>

#include "game_models.h"

namespace snake {

class AIService final {
 public:
  InputDTO GetInputs(const GameState& gameState, const GameSettigs& settings);
};
}  // namespace snake
#endif  // SNAKE_SNAKE_AI_SERVICE_H_
