#include "GameModels.h"

#include <vector>

class GameLogicService {
public:
    void applyForcesAndCheck(GameState* gameState, std::vector<Input> inputs, GameSettigs settings);
};