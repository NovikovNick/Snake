#ifndef SNAKE_SNAKE_GAME_SNAKE_RING_BUFFER_H_
#define SNAKE_SNAKE_GAME_SNAKE_RING_BUFFER_H_

#include "GameModels.h"

#include <vector>

namespace snake {

    class GameStateRingBuffer {
    public:
        GameState* applyForces(GameState* gameState, std::vector<Input> inputs, GameSettigs settings);
    };

} // namespace snake

#endif  // SNAKE_SNAKE_GAME_SNAKE_RING_BUFFER_H_