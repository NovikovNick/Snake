#ifndef SNAKE_SNAKE_INPUT_SERVICE_H_
#define SNAKE_SNAKE_INPUT_SERVICE_H_
#include <vector>

#include "deque"
#include "game_models.h"

namespace snake {

class InputService final {
 public:
  void AddInput(const Input& input);
  Input PopInputs();

 private:
  std::deque<Input> input_queue_;
};

}  // namespace snake

#endif  // SNAKE_SNAKE_INPUT_SERVICE_H_
