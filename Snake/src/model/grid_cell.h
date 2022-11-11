#ifndef SNAKE_SNAKE_GAME_CELL_H_
#define SNAKE_SNAKE_GAME_CELL_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace snake {

struct GridCell final {
 private:
  int x_, y_;

 public:
  GridCell() : x_(-1), y_(-1){};
  GridCell(const int x, const int y) : x_(x), y_(y){};
  int GetX() const { return x_; };
  int GetY() const { return y_; };
  bool operator==(const GridCell& other) const {
    return (x_ == other.GetX()) && (y_ == other.GetY());
  };
  bool operator!=(const GridCell& other) const { return !(*this == other); };
  std::string ToString() const {
    return std::format("[{:2d},{:2d}]", GetX(), GetY());
  }
};
}  // namespace snake

// we have right to add specialization to std
namespace std {
template <>
struct hash<snake::GridCell> {
  size_t operator()(const snake::GridCell& p) const {
    auto h1 = std::hash<int>{}(p.GetX());
    auto h2 = std::hash<int>{}(p.GetY());
    return h1 == h2 ? h1 : h1 ^ h2;
  }
};
}  // namespace std

#endif  // SNAKE_SNAKE_GAME_CELL_H_
