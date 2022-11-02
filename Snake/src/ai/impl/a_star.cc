#include "../a_star.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <numeric>
#include <queue>
#include <unordered_set>
#include <vector>

namespace {

template <typename N>
class NodePath {
 private:
  N start_;
  N end_;
  std::deque<N> path_;
  float distanceFromStart_;
  float distanceToEnd_;

  float getDistance(const N& n1, const N& n2) const {
    auto [x0, x1] = std::minmax(n1.GetX(), n2.GetX());
    auto [y0, y1] = std::minmax(n1.GetY(), n2.GetY());
    return std::sqrt(std::pow(x1 - x0, 2) + std::pow(y1 - y0, 2));
  }

 public:
  NodePath(const N& start, const N& end)
      : start_(start),
        end_(end),
        distanceFromStart_(0.0),
        distanceToEnd_(getDistance(start, end)) {
    path_.push_back(start);
  };

  void AddNode(const N& node) {
    distanceFromStart_ = getDistance(start_, node);
    distanceToEnd_ = getDistance(end_, node);
    path_.push_back(node);
  };

  N GetHead() const { return path_.back(); };

  float GetDistanceFromStart() const { return distanceFromStart_; };
  float GetDistanceToEnd() const { return distanceToEnd_; };
  float GetLength() const { return path_.size(); };
  std::deque<N>::const_iterator begin() const { return path_.cbegin(); };
  std::deque<N>::const_iterator end() const { return path_.cend(); };
  std::string ToString() const {
    std::string res =
        std::format("path [distance to goal {:2.1f}, length {:2d}]",
                    distanceToEnd_, path_.size());
    for (auto it : path_) {
      res += " " + it.ToString();
    }
    return res;
  }
};

template <typename N>
struct NodePathComparator {
  bool operator()(const NodePath<N>& p1, const NodePath<N>& p2) {
    float distToEnd1 = p1.GetDistanceToEnd();
    float distToEnd2 = p2.GetDistanceToEnd();

    if (distToEnd1 > distToEnd2) return true;
    if (distToEnd1 == distToEnd2) return p1.GetLength() > p2.GetLength();
    return false;
  };
};

template <typename N>
struct NodeHashcode {
  std::size_t operator()(const N& n) const {
    auto hash1 = std::hash<int>{}(n.GetX());
    auto hash2 = std::hash<int>{}(n.GetY());
    if (hash1 != hash2) {
      return hash1 ^ hash2;
    }
    return hash1;
  };
};

template <typename N>
struct NodeEquals {
  bool operator()(const N& n1, const N& n2) const { return n1 == n2; };
};

template <typename... Args>
void debug(const std::string_view& str, Args&&... args) {
#if SNAKE_DEBUG
  std::cout << std::vformat(str, std::make_format_args(args...));
#endif
}
}  // namespace

namespace snake {

template <grid_2d_cell N, grid_2d<N> G, std::output_iterator<N> I>
bool AStarPathfinder<N, G, I>::FindPath(const N& start, const N& goal,
                                        const G& grid, I out, I sentinel) {
  debug("A* pathfinding from [{:2d},{:2d}] to [{:2d},{:2d}]\n", start.GetX(),
        start.GetY(), goal.GetX(), goal.GetY());

  N empty;
  NodePathComparator<N> comp;
  NodeHashcode<N> hashcode;
  NodeEquals<N> equals;
  std::priority_queue<NodePath<N>, std::vector<NodePath<N>>, decltype(comp)>
      reacheable(comp);
  std::unordered_set<N, decltype(hashcode), decltype(equals)> explored(
      32, hashcode, equals);
  std::vector<N> adjacents(4);

  int maxDistance = std::distance(out, sentinel);

  reacheable.push(NodePath(start, goal));

  while (!reacheable.empty()) {
    auto path = reacheable.top();
    reacheable.pop();
    auto node = path.GetHead();

    debug("Checking {}\n", path.ToString());
    grid.FindAdjacents(node.GetX(), node.GetY(), adjacents.begin());

    for (auto adjacent : adjacents) {
      debug("Adjacent ");

      if (adjacent == empty) {
        debug("is empty\n");
        continue;
      }
      debug("{} ", adjacent.ToString());

      if (explored.find(adjacent) != explored.end()) {
        debug("is explored\n");
        continue;
      }

      if (adjacent == goal) {
        debug("is a goal!\n");
        path.AddNode(adjacent);

        auto it = path.begin();
        while (out != sentinel && it != path.end()) {
          *out = *it;
          ++it;
          ++out;
        }
        return true;
      } else {
       
        if (path.GetLength() < maxDistance) {
          auto nextPath = path;
          nextPath.AddNode(adjacent);
          reacheable.push(nextPath);
          debug("is not a goal. Add it to reacheable!\n");
        } else {
          debug("is not a goal. Out of range {}!\n", maxDistance);
        }
        
      }
    }
    std::fill(adjacents.begin(), adjacents.end(), empty);
    explored.insert(node);
  }
  return false;
};
}  // namespace snake