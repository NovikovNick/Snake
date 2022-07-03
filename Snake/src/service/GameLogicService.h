#ifndef SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_

#include "GameModels.h"

#include <vector>

namespace snake {

    class GameLogicService {
    public:
        void check(GameState* gameState, GameSettigs settings);
    };

} // namespace snake

#endif  // SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
