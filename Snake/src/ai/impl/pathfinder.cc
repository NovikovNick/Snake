#include "../pathfinder.h"

#include <stdlib.h>

#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace snake {

namespace {
const int MAX_ATTEMPTS = 100;

struct CoordCompare {
  Coord point_;

  CoordCompare(Coord point) : point_(point) {}

  bool operator()(const Coord& a, const Coord& b) {
    return std::abs(point_.x - a.x) + std::abs(point_.y - a.y) >
           std::abs(point_.x - b.x) + std::abs(point_.y - b.y);
  }
};

std::vector<std::pair<Coord, Direction>> getNextPossibleCoord(
    const Coord& coord) {
  return {std::make_pair(coord + Direction::kRight, Direction::kRight),
          std::make_pair(coord + Direction::kLeft, Direction::kLeft),
          std::make_pair(coord + Direction::kUp, Direction::kUp),
          std::make_pair(coord + Direction::kDown, Direction::kDown)};
}
}  // namespace

Path Pathfinder::FindPath(const IPathFindRequest& req) {
  const Coord& start = req.getFrom();
  const Coord& to = req.getTo();
  const std::vector<Coord>& barriers = req.getBarriers();

  std::unordered_map<Coord, std::deque<Direction>, hash_coord> reachable = {
      {start, {}}};
  std::unordered_set<Coord, hash_coord> explored(barriers.begin(),
                                                 barriers.end());

  CoordCompare comp(to);
  std::priority_queue<Coord, std::vector<Coord>, CoordCompare> reachableQueue(
      comp);
  reachableQueue.push(start);

  DebugContext ctx;

  for (size_t depth = 0; !reachable.empty() && depth < MAX_ATTEMPTS; ++depth) {
    // ctx.pathfinding.push_back({});

    Coord cursor = reachableQueue.top();
    reachableQueue.pop();
    std::deque<Direction> currentPath = reachable[cursor];

    for (auto candidate : getNextPossibleCoord(cursor)) {
      Coord candidateCoord = candidate.first;
      Direction candidateDirection = candidate.second;

      std::deque<Direction> possiblePath(currentPath);
      possiblePath.push_front(candidateDirection);

      if (candidateCoord == to) {
        for (auto pathDirection : possiblePath) {
          // ctx.pathfinding[depth].push_back({ candidateCoord - pathDirection,
          // DebugMark::PATH });
        }

        return Path{{possiblePath.back()}, ctx};
      }

      if (reachable.find(candidateCoord) == reachable.end() &&
          explored.find(candidateCoord) == explored.end()) {
        reachable[candidateCoord] = possiblePath;
        reachableQueue.push(candidateCoord);
      }

      /*for (auto it : reachable) {
              ctx.pathfinding[depth].push_back({ it.first, DebugMark::REACHABLE
      });
      }
      for (auto it : explored) {
              ctx.pathfinding[depth].push_back({ it, DebugMark::EXPLORED });
      }*/
    }
    explored.insert(cursor);
    reachable.extract(cursor);
  }

  return Path{{Direction::kRight}};
}

}  // namespace snake