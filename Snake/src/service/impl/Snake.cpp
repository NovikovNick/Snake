#include "../GameModels.h"

namespace snake {

namespace {
Direction opporite(const Direction& dir) {
	switch (dir) {
	case Direction::LEFT:
		return Direction::RIGHT;
	case Direction::RIGHT:
		return Direction::LEFT;
	case Direction::UP:
		return Direction::DOWN;
	case Direction::DOWN:
		return Direction::UP;
	default:
		return Direction::NONE;
	}
}
}

Snake* Snake::move(const Direction& dir, const bool& gain) const noexcept {

	std::cout << "Snake [" << this << "]  move" << std::endl;

	std::vector<std::pair<Coord, Direction>> srcParts = getParts();
	std::vector<std::pair<Coord, Direction>> dstParts;

	Direction previous = (dir == Direction::NONE || dir == opporite(srcParts[0].second)) 
		? srcParts[0].second 
		: dir;

	dstParts.push_back(std::make_pair(srcParts[0].first + previous, previous));

	for (size_t i = 1; i < srcParts.size(); i++) {
		dstParts.push_back(std::make_pair(srcParts[i].first + srcParts[i].second, previous));
		previous = srcParts[i].second;
	}

	if (gain) {
		dstParts.push_back(std::make_pair(srcParts[srcParts.size() - 1].first, srcParts[srcParts.size() - 1].second));
	}

	return new Snake(dstParts);
}

bool Snake::isInBound(const Coord& leftTop, const Coord& rightBottom) const noexcept {
	return leftTop.x < leftTop_.x
		&& leftTop.y < leftTop_.y
		&& rightBottom.x > rightBottom_.x
		&& rightBottom.y > rightBottom_.y;
}

bool Snake::isCollide(const Coord& coord) const noexcept {
	return map_.find(coord) != map_.end();
}

} // namespace snake
