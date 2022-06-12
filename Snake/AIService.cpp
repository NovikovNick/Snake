#include "AIService.h"

#include <stdlib.h>

namespace snake {
    std::vector<Input> AIService::getInputs(GameState* gameState, GameSettigs settings) {

        Direction prevDirection = gameState->snake_head[1]->direction;
        int x = gameState->snake_head[1]->coord.x;
        int y = gameState->snake_head[1]->coord.y;

        Direction dir = Direction::NONE;

        if (x >= settings.rightBoundaries - 1 && prevDirection == Direction::RIGHT) {
            dir = Direction::UP;
        } else if (y <= settings.topBoundaries && prevDirection == Direction::UP) {
            dir = Direction::LEFT;
        } else if (x <= settings.leftBoundaries && prevDirection == Direction::LEFT) {
            dir = Direction::DOWN;
        } else if (y >= settings.bottomBoundaries - 1 && prevDirection == Direction::DOWN) {
            dir = Direction::RIGHT;
        }

        return { { dir, SystemCommand::NONE } };
    }

} // namespace snake
