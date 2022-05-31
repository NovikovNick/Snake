#ifndef SNAKE_SNAKE_GAME_SNAKE_HOLDER_H_
#define SNAKE_SNAKE_GAME_SNAKE_HOLDER_H_

#include "GameModels.h"

#include <vector>

namespace snake {

class GameStateHolder {
public:

    GameStateHolder(GameState* initialGameState) {
        _frame = 0;
        _ringBuffer[0] = initialGameState;
        _stateInputs[0] = initialGameState->input;
    }

    ~GameStateHolder() {
        for (auto it : _ringBuffer) {
            delete it;
        }
    }

    GameState* ApplyForces(std::vector<Input> inputs, GameSettigs settings);

    inline int GetFrame() const { return _frame; }
    inline int GetCapacity() const { return _capacity; }

    GameState* GetState(int frame);
    Input GetInput(int frame);

private:
    int _frame;
    int const _capacity = 32;
    std::vector<Input> _inputs;

    Input _stateInputs[32];
    GameState* _ringBuffer[32];
};

} // namespace snake

#endif  // SNAKE_SNAKE_GAME_SNAKE_HOLDER_H_