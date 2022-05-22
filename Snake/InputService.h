#pragma once

#include "GameModels.h"

#include "deque"
#include <vector>

class InputService {
public:
	void addInput(Input input);
	std::vector<Input> popInputs();
private:
	std::deque<Input> _inputQueue = std::deque<Input, std::allocator<Input>>();
};
