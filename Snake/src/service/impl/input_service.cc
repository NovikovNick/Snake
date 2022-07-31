#include "../input_service.h"

namespace snake {

void InputService::AddInput(const Input& input) {
	input_queue_.push_front(input);
}

Input InputService::PopInputs() {

	if (!input_queue_.empty()) {

		Input res = input_queue_.front();
		input_queue_.clear();
		return res;

	}
	return Input{};
}

} // namespace snake
