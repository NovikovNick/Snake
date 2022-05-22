#include "InputService.h"

void InputService::addInput(Input input)
{
	_inputQueue.push_front(input);
}

std::vector<Input> InputService::popInputs()
{

	std::vector<Input>res = std::vector<Input>();

	if (!_inputQueue.empty()) {
		for (; !_inputQueue.empty(); _inputQueue.pop_front()) {
			res.push_back(_inputQueue.front());
		}
	}
	return res;
}
