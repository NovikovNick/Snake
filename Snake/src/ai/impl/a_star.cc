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
  N head_;
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
        head_(start),
        end_(end),
        distanceFromStart_(0.0),
        distanceToEnd_(getDistance(head_, end_)){};
  void AddNode(const N& node) {
    distanceFromStart_ = getDistance(start_, node);
    distanceToEnd_ = getDistance(end_, node);
    path_.push_back(node);

    std::cout << "new node added" << std::endl;
    head_ = node;
  };

  N GetHead() const { return head_; };

  float GetDistanceFromStart() const { return distanceFromStart_; };
  float GetDistanceToEnd() const { return distanceToEnd_; };
  std::deque<N>::const_iterator begin() const { return path_.cbegin(); };
  std::deque<N>::const_iterator end() const { return path_.cend(); };
  std::string ToString() const {
    auto res = "head: " + head_.ToString();

    res += std::format(" from start - {:2.1f}, to end - {:2.1f} ",
                       distanceFromStart_, distanceToEnd_);
    res += "{ ";
    for (auto it : path_) {
      res += " " + it.ToString();
    }
    res += "}";
    return res;
  }
};

template <typename N>
struct NodePathComparator {
  bool operator()(const NodePath<N>& p1, const NodePath<N>& p2) {
    float distToEnd1 = p1.GetDistanceToEnd();
    float distToEnd2 = p2.GetDistanceToEnd();
    if (distToEnd1 == distToEnd2) {
      return p1.GetDistanceFromStart() > p2.GetDistanceFromStart();
    } else {
      return distToEnd1 > distToEnd2;
    }
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
}  // namespace

namespace snake {

template <grid_2d_cell N, grid_2d<N> G, std::output_iterator<N> I>
void AStarPathfinder<N, G, I>::FindPath(const N& start, const N& goal,
                                        const G& grid, I out) {
  std::cout << std::format("Start [{:2d},{:2d}]...", start.GetX(), start.GetY())
            << std::endl;
  std::cout << std::format("Goal [{:2d},{:2d}]...", goal.GetX(), goal.GetY())
            << std::endl;
  N empty;
  NodePathComparator<N> comp;
  NodeHashcode<N> hashcode;
  NodeEquals<N> equals;
  std::priority_queue<NodePath<N>, std::vector<NodePath<N>>, decltype(comp)>
      reacheable(comp);
  std::unordered_set<N, decltype(hashcode), decltype(equals)> explored(
      32, hashcode, equals);
  std::vector<N> adjacents(4);

  reacheable.push(NodePath(start, goal));

  while (!reacheable.empty()) {
    auto path = reacheable.top();
    reacheable.pop();
    auto node = path.GetHead();

    std::cout << "Checking " << node.ToString() << std::endl;
    grid.FindAdjacents(node.GetX(), node.GetY(), adjacents.begin());

    for (auto adjacent : adjacents) {
      if (adjacent == empty) {
        continue;
      }
      std::cout << "Adjacent " << adjacent.ToString();
      if (explored.find(adjacent) != explored.end()) {
        std::cout << " is explored." << std::endl;
        continue;
      }
      std::cout << " is not explored. " << std::endl;
      
      if (adjacent == goal) {
        std::cout << "Is a goal! " << std::endl;
        for (auto it : path) {
          *(out++) = it;
        }
        return;
      } else {
        std::cout << "Is not a goal. Added to reacheable " << std::endl;
        path.AddNode(adjacent);
        reacheable.push(path);
      }
    }
    std::fill(adjacents.begin(), adjacents.end(), empty);
    explored.insert(node);
  }
};
}  // namespace snake