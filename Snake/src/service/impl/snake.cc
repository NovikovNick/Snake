#include "../../model/common_models.h"
#include "../game_models.h"
#include "../snake_service.h"

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

Snake SnakeService::move(const Snake& snake, const Direction& dir,
                         const bool& gain) const {
  std::cout << "Snake [" << this << "]  move" << std::endl;

  std::vector<std::pair<Coord, Direction>> srcParts = snake.getParts();
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

  return Snake(dstParts);
}

}  // namespace snake
