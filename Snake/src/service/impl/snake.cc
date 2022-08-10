#include "../game_models.h"

namespace snake {

namespace {
Direction opporite(const Direction& dir) {
  switch (dir) {
    case Direction::kLeft:
      return Direction::kRight;
    case Direction::kRight:
      return Direction::kLeft;
    case Direction::kUp:
      return Direction::kDown;
    case Direction::kDown:
      return Direction::kUp;
    default:
      return Direction::kNone;
  }
}
}  // namespace

Snake* Snake::move(const Direction& dir, const bool& gain) const noexcept {
  std::cout << "Snake [" << this << "]  move" << std::endl;

  std::vector<std::pair<Coord, Direction>> srcParts = getParts();
  std::vector<std::pair<Coord, Direction>> dstParts;

  Direction previous =
      (dir == Direction::kNone || dir == opporite(srcParts[0].second))
          ? srcParts[0].second
          : dir;

  dstParts.push_back(std::make_pair(srcParts[0].first + previous, previous));

  for (int i = 1, size = static_cast<int>(srcParts.size()); i < size; ++i) {
    dstParts.push_back(
        std::make_pair(srcParts[i].first + srcParts[i].second, previous));
    previous = srcParts[i].second;
  }

  // bool valid = dstParts.size() == srcParts.size();

  if (gain) {
    auto srcTail = srcParts[srcParts.size() - 1];
    dstParts.push_back(std::make_pair(srcTail.first, srcTail.second));
  }

  return new Snake(dstParts);
}

bool Snake::isInBound(const Coord& leftTop,
                      const Coord& rightBottom) const noexcept {
  return leftTop.x < leftTop_.x && leftTop.y < leftTop_.y &&
         rightBottom.x > rightBottom_.x && rightBottom.y > rightBottom_.y;
}

bool Snake::isCollide(const Coord& coord) const noexcept {
  return map_.find(coord) != map_.end();
}

const Coord& Snake::getHeadCoord() const noexcept {
  return getParts()[0].first;
}

void Snake::gain() noexcept {
  std::pair<Coord, Direction> last = getParts().back();
  list_.push_back({last.first - last.second, last.second});
}

}  // namespace snake
