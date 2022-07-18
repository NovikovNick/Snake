#include "../InputService.h"

namespace snake {

void InputService::addInput(Input input)
{
	_inputQueue.push_front(input);
}

std::vector<Input> InputService::popInputs()
{


	if (!_inputQueue.empty()) {
		
		std::vector<Input> res = std::vector<Input>();

		for (; !_inputQueue.empty(); _inputQueue.pop_back()) {
			res.push_back(_inputQueue.back());
		}

		return res;

	}
	return { Input{} };
}

} // namespace snake
