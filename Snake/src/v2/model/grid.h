#ifndef SNAKE_SNAKE_GAME_GRID_2D_H_
#define SNAKE_SNAKE_GAME_GRID_2D_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grid_cell.h"

namespace snake {

using SNAKE_PART = std::tuple<int, int, int>;
using GAME_OBJECT = std::tuple<int, int, int>;
using SNAKE_DATA = std::deque<SNAKE_PART>;
using SNAKE_DATA_CONST_ITERATOR = SNAKE_DATA::const_iterator;
using SNAKE_DATA_ITERATOR = SNAKE_DATA::iterator;
using GAME_OBJECT_ITERATOR = std::vector<GAME_OBJECT>::iterator;

// template <typename COORD>
class Grid2d final {
  using COORD = GridCell;
  using GRID_DATA = std::vector<COORD>;
  using COORD_ITERATOR = GRID_DATA::iterator;

  struct CoordHash {
    int operator()(const COORD& o) const {
      auto h1 = std::hash<int>{}(o.GetX());
      auto h2 = std::hash<int>{}(o.GetY());
      if (h1 == h2) return h1;
      return h1 ^ h2;
    }
  };
  struct CoordEquals {
    bool operator()(const COORD& o1, const COORD& o2) const {
      return o1.GetX() == o2.GetX() && o1.GetY() == o2.GetY();
    }
  };
  using COORD_SET = std::unordered_set<COORD, CoordHash, CoordEquals>;

  std::unordered_map<int, COORD_SET> filled_;
  std::vector<SNAKE_DATA> snakes_;
  std::vector<int> snake_length_;
  std::vector<COORD> grid_;
  int width_, height_;

 public:
  COORD food;
  int snake_count;

  Grid2d(int width, int height, int snake_count)
      : width_(width),
        height_(height),
        snake_count(snake_count),
        snakes_(std::vector<SNAKE_DATA>(snake_count)),
        snake_length_(std::vector<int>(snake_count)),
        grid_(GRID_DATA(width * height)),
        filled_(std::unordered_map<int, COORD_SET>()) {
    for (int row = 0; row < height; ++row) {
      for (int col = 0; col < width; ++col) {
        grid_[row * width + col] = COORD(col, row);
      }
    }
  };
  int GetSnakeLength(const int& index) const { return snake_length_[index]; }

  void AddSnake(const int& index, auto begin, auto end) {
    snakes_[index].clear();
    snake_length_[index] = 0;

    for (; begin < end; ++begin) {
      auto [x, y, dir] = *begin;
      snakes_[index].push_back(SNAKE_PART(x, y, dir));
      ++snake_length_[index];
    }
  };

  std::pair<int, SNAKE_DATA_CONST_ITERATOR> GetSnake(const int& index) const {
    return std::make_pair(snake_length_[index], snakes_[index].cbegin());
  };

  void CopySnake(const int& index, SNAKE_DATA_ITERATOR out) const {
    auto [length, iter] = GetSnake(index);
    for (int i = 0; i < length; ++i) {
      *out = *iter;
      ++iter;
      ++out;
    }
  };

  void FindAdjacents(int x, int y, COORD_ITERATOR out) const {
    int index;
    if (y + 1 < height_ && !IsSnake(x, y + 1)) {
      index = width_ * (y + 1) + (x + 0);
      *(out++) = grid_[index];
    }
    if (x + 1 < width_ && !IsSnake(x + 1, y)) {
      index = width_ * (y + 0) + (x + 1);
      *(out++) = grid_[index];
    }
    if (y - 1 >= 0 && !IsSnake(x, y - 1)) {
      index = width_ * (y - 1) + (x + 0);
      *(out++) = grid_[index];
    }
    if (x - 1 >= 0 && !IsSnake(x - 1, y)) {
      index = width_ * (y + 0) + (x - 1);
      *(out) = grid_[index];
    }
  }

  void copy(GAME_OBJECT_ITERATOR out) {
    for (int i = 0, size = width_ * height_; i < size; ++i) {
      int row = i / width_;
      int col = i % width_;
      *(out++) = SNAKE_PART(col, row, GetType(col, row));
    }
  };

  bool IsSnake(const int& x, const int& y) const {
    for (int snake_id = 0; snake_id < snake_count; ++snake_id) {
      if (IsSnake(x, y, snake_id)) return true;
    }
    return false;
  }

  bool IsSnake(const int& x, const int& y, const int snake_id) const {
    auto it = filled_.find(snake_id);
    if (it == filled_.end()) {
      return false;
    }
    auto snake_part_set = it->second;
    return snake_part_set.find({x, y}) != snake_part_set.end();
  }

  bool IsOutOfBound(const int& x, const int& y) const {
    return x < 0 || y < 0 || x >= width_ || y >= height_;
  }

  void RebuildFilled(const int snake_id) {
    if (filled_[snake_id].empty()) {
      filled_[snake_id] = COORD_SET(32, CoordHash{}, CoordEquals{});
    } else {
      filled_[snake_id].clear();
    }
    for (const auto& snake : snakes_) {
      for (const auto [x, y, _] : snake) {
        filled_[snake_id].emplace(x, y);
      }
    }
  }

 private:
  int GetType(const int& x, const int& y) const {
    if (food.GetX() == x && food.GetY() == y) return 1;
    for (int snake_id = 0; snake_id < snake_count; ++snake_id) {
      if (IsSnake(x, y, snake_id)) return snake_id + 2;
    }
    return 0;
  }
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_GRID_2D_H_
