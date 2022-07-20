#include "../Pathfinder.h"

#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <unordered_set>

namespace snake {

namespace {
	const int MAX_ATTEMPTS = 100;

	struct CoordCompare
	{
		Coord point;

		bool operator()(const Coord& a, const Coord& b)
		{
			return std::abs(point.x - a.x) + std::abs(point.y - a.y) > std::abs(point.x - b.x) + std::abs(point.y - b.y);
		}
	};

	std::vector<std::pair<Coord, Direction>> getNextPossibleCoord(const Coord& coord) {
		return {
			std::make_pair(coord + Direction::RIGHT, Direction::RIGHT),
			std::make_pair(coord + Direction::LEFT, Direction::LEFT),
			std::make_pair(coord + Direction::UP, Direction::UP),
			std::make_pair(coord + Direction::DOWN, Direction::DOWN)
		};
	}
}

Path Pathfinder::findPath(
	const Coord& start,
	const Coord& to,
	const std::vector<Coord>& barriers) {

	std::unordered_map<Coord, std::deque<Direction>, hash_coord> reachable = { { start, {} } };
	std::unordered_set<Coord, hash_coord> explored(barriers.begin(), barriers.end());

	CoordCompare comp({ to });
	std::priority_queue<Coord, std::vector<Coord>, CoordCompare> reachableQueue(comp);
	reachableQueue.push(start);

	DebugContext ctx;

	for (size_t depth = 0; !reachable.empty() && depth < MAX_ATTEMPTS; depth++) {
		
		ctx.pathfinding.push_back({});
		
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
					ctx.pathfinding[depth].push_back({ candidateCoord - pathDirection, DebugMark::PATH });
				}

				return Path{ { possiblePath.back() }, ctx};
			}

			if (reachable.find(candidateCoord) == reachable.end() && explored.find(candidateCoord) == explored.end()) {
				
				reachable[candidateCoord] = possiblePath;
				reachableQueue.push(candidateCoord);
			}

			for (auto it : reachable) {
				ctx.pathfinding[depth].push_back({ it.first, DebugMark::REACHABLE });
			}
			for (auto it : explored) {
				ctx.pathfinding[depth].push_back({ it, DebugMark::EXPLORED });
			}
		}
		explored.insert(cursor);
		reachable.extract(cursor);
	}
	
	return Path{ { Direction::RIGHT } };
}

} // namespace snake