#ifndef SNAKE_SNAKE_INPUT_SERVICE_H_
#define SNAKE_SNAKE_INPUT_SERVICE_H_
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

#endif // !SNAKE_SNAKE_INPUT_SERVICE_H_
