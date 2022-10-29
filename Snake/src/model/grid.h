#ifndef SNAKE_SNAKE_GAME_GRID_2D_H_
#define SNAKE_SNAKE_GAME_GRID_2D_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

template <typename... Args>
void debug(const std::string_view& str, Args&&... args) {
#if SNAKE_DEBUG
  std::cout << std::vformat(str, std::make_format_args(args...));
#endif
}
}  // namespace

namespace snake {

using COORD = std::pair<int, int>;
using SNAKE_PART = std::tuple<int, int, int>;
using GAME_OBJECT = std::tuple<int, int, int>;
using SNAKE_DATA = std::deque<SNAKE_PART>;
using SNAKE_DATA_CONST_ITERATOR = SNAKE_DATA::const_iterator;
using SNAKE_DATA_ITERATOR = SNAKE_DATA::iterator;
using GRID_DATA = std::vector<COORD>;
using COORD_ITERATOR = GRID_DATA::iterator;
using GAME_OBJECT_ITERATOR = std::vector<GAME_OBJECT>::iterator;

class Grid2d {
  std::vector<SNAKE_DATA> snakes_ = std::vector<SNAKE_DATA>(2);
  std::vector<int> snake_length_ = std::vector<int>(2);
  std::vector<COORD> grid_;
  int width_, height_;

 public:
  std::pair<int, int> food;

  Grid2d(int width, int height)
      : width_(width), height_(height), grid_(GRID_DATA(width * height)) {
    for (int row = 0; row < height; ++row) {
      for (int col = 0; col < width; ++col) {
        grid_[row * width + col] = COORD(col, row);
      }
    }
  };

  void AddSnake(const int& index, auto begin, auto end) {
    snakes_[index].clear();
    snake_length_[index] = 0;

    for (; begin < end; ++begin) {
      auto [x, y, dir] = *begin;
      snakes_[index].push_back(std::make_tuple(x, y, dir));
      ++snake_length_[index];
    }
  };

  std::pair<int, SNAKE_DATA_CONST_ITERATOR> GetSnake(const int& index) {
    return std::make_pair(snake_length_[index], snakes_[index].cbegin());
  };

  void CopySnake(const int& index, SNAKE_DATA_ITERATOR out) {
    auto [length, iter] = GetSnake(index);
    for (int i = 0; i < length; ++i) {
      *out = *iter;
      ++iter;
      ++out;
    }
  };

  void FindAdjacents(int x, int y, COORD_ITERATOR out) const {
    if (y + 1 < height_) *(out++) = grid_[width_ * (y + 1) + (x + 0)];
    if (x + 1 < width_) *(out++) = grid_[width_ * (y + 0) + (x + 1)];
    if (y - 1 >= 0) *(out++) = grid_[width_ * (y - 1) + (x + 0)];
    if (x - 1 >= 0) *(out) = grid_[width_ * (y + 0) + (x - 1)];
  }

  void copy(GAME_OBJECT_ITERATOR out) {
    auto hash = [](const COORD& o) {
      auto h1 = std::hash<int>{}(o.first);
      auto h2 = std::hash<int>{}(o.second);
      if (h1 == h2) return h1;
      return h1 ^ h2;
    };
    auto equals = [](const COORD& o1, const COORD& o2) {
      return o1.first == o2.first && o1.second == o2.second;
    };
    std::unordered_set<COORD, decltype(hash), decltype(equals)> filled(32, hash,
                                                                       equals);
    filled.emplace(food.first, food.second);
    for (auto snake : snakes_) {
      std::transform(snake.cbegin(), snake.cend(),
                     std::inserter(filled, filled.begin()), [](const auto& it) {
                       auto [fst, snd, thd] = it;
                       return std::make_pair(fst, snd);
                     });
    }

    for (int i = 0, size = width_ * height_; i < size; ++i) {
      int row = i / width_;
      int col = i % width_;
      *out = std::make_tuple(col, row, filled.find({col, row}) != filled.end());
      ++out;
    }
  };
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_GRID_2D_H_
