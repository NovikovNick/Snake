#include "../GameModels.h"

namespace snake {

Snake Snake::move(const Direction& dir) {

	std::cout << "Snake [" << std::to_string(id_) << "]  move" << std::endl;

	return Snake(id_ + 1);
}

} // namespace snake
