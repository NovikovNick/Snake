#include "../a_star.h"

#include <iostream>
#include <vector>

namespace snake {



void AStarPathfinder::FindPath(TestNode n) {
  int x = n.GetX();
  int y = n.GetY();
  std::cout << x << " : " << y << std::endl;

  // std::priority_queue<NODE> reacheable;
  // std::unordered_set<NODE> explored;
  // std::vector<NODE> adjacents(4);
  // while (!reacheable.empty()) {
  //  auto [x, y] = reacheable.top();
  //  reacheable.pop();
  //  grid.FindAdjacents(x, y, adjacents.begin());
  //  for (auto adjacent : adjacents) {
  //    if (explored.find(adjacent) != explored.end()) {
  //      continue;
  //    }
  //    if (adjacent == goal) {
  //      // calculate path
  //      *out = adjacent;
  //    } else {
  //      reachable.add(adjacent);
  //    }
  //  }
  //  std::fill(adjacents.begin(), adjacents.end(), dummy);
  //  explored.add(cur);
  //}
};
}  // namespace snake