#ifndef SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_

#include "game_models.h"

#include <vector>

namespace snake {

    class GameLogicService {
    public:
        const GamePhase& check(const GameState& gameState, const GameSettigs& settings);
    };

} // namespace snake

#endif  // SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
