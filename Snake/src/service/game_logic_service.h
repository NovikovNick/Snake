#ifndef SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_

#include <vector>

#include "game_models.h"
#include "../model/common_models.h"

namespace snake {

class GameLogicService final {
 public:
  const GamePhase& check(const GameState& gameState,
                         const GameSettigs& settings);
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
