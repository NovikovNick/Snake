#ifndef SNAKE_SNAKE_AI_SERVICE_H_
#define SNAKE_SNAKE_AI_SERVICE_H_

#include "GameModels.h"

#include <vector>

namespace snake {

class AIService {
public:
    InputDTO getInputs(const GameState& gameState, const GameSettigs& settings);
};

} // namespace snake

#endif  // SNAKE_SNAKE_AI_SERVICE_H_
