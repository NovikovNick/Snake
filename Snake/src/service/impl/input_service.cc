#include "../input_service.h"

namespace snake {

void InputService::AddInput(const PlayerInput& input) {
  input_queue_.push_front(input);
}

PlayerInput InputService::PopInputs() {
  if (!input_queue_.empty()) {
    PlayerInput res = input_queue_.front();
    input_queue_.clear();
    return res;
  }
  return PlayerInput{};
}

}  // namespace snake
