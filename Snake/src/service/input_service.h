#ifndef SNAKE_SNAKE_INPUT_SERVICE_H_
#define SNAKE_SNAKE_INPUT_SERVICE_H_
#include <vector>

#include "../model/player_input.h"
#include "deque"

namespace snake {

class InputService final {
 public:
  void AddInput(const PlayerInput& input);
  PlayerInput PopInputs();

 private:
  std::deque<PlayerInput> input_queue_;
};

}  // namespace snake

#endif  // SNAKE_SNAKE_INPUT_SERVICE_H_
