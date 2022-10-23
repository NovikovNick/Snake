#include "../a_star.h"

#include <format>
#include <iostream>
#include <vector>

namespace snake {

template <grid_2d_cell N, grid_2d G, std::output_iterator<N> I>
void AStarPathfinder<N, G, I>::FindPath(const N& start, const N& goal,
                                        const G& grid, I out) {
  std::cout << std::format(
                   "Need to find path from [{:2d},{:2d}] to [{:2d},{:2d}].",
                   start.GetX(), start.GetY(), goal.GetX(), goal.GetY())
            << std::endl;

  for (int i = 0; i < 4; ++i) {
    *out = goal;
    ++out;
  }

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