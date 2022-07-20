#include "../InputService.h"

namespace snake {

void InputService::addInput(Input input)
{
	_inputQueue.push_front(input);
}

const Input& InputService::popInputs() {

	if (!_inputQueue.empty()) {
		Input res = _inputQueue.front();
		_inputQueue.clear();
		return res;

	}
	return Input{};
}

} // namespace snake
