#ifndef SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_

#include "GameModels.h"

#include <vector>

class GameLogicService {
public:
    void applyForcesAndCheck(GameState* gameState, std::vector<Input> inputs, GameSettigs settings);
};

#endif  // SNAKE_SNAKE_GAME_LOGIC_SERVICE_H_
