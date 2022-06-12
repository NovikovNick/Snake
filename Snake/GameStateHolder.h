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
        _stateInputs[0][0] = initialGameState->input[0];
        _stateInputs[1][0] = initialGameState->input[1];
    }

    ~GameStateHolder() {
        for (auto it : _ringBuffer) {
            delete it;
        }
    }

    GameState* ApplyForces(std::vector<Input> inputs[2], GameSettigs settings);

    inline int GetFrame() const { return _frame; }
    inline int GetCapacity() const { return _capacity; }
    inline int GetPlayerCount() const { return _playerCount; }

    GameState* GetState(int frame);
    Input GetInput(int frame, int index);

private:
    int _frame;
    int const _capacity = 32;
    int const _playerCount = 2;

    std::vector<Input> _inputs[2];
    Input _stateInputs[2][32];
    GameState* _ringBuffer[32];
};

} // namespace snake

#endif  // SNAKE_SNAKE_GAME_SNAKE_HOLDER_H_