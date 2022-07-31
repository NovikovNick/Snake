#ifndef SNAKE_SNAKE_INPUT_SERVICE_H_
#define SNAKE_SNAKE_INPUT_SERVICE_H_
#include "game_models.h"

#include "deque"
#include <vector>


namespace snake {

class InputService final {
	std::deque<Input> input_queue_ = std::deque<Input, std::allocator<Input>>();
public:
	void AddInput(const Input& input);
	Input PopInputs();
};

} // namespace snake

#endif // SNAKE_SNAKE_INPUT_SERVICE_H_
