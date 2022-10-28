#ifndef SNAKE_SNAKE_GAME_GRID_2D_H_
#define SNAKE_SNAKE_GAME_GRID_2D_H_

#include <format>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <queue>

namespace {

template <typename... Args>
void debug(const std::string_view& str, Args&&... args) {
#if SNAKE_DEBUG
  std::cout << std::vformat(str, std::make_format_args(args...));
#endif
}
}  // namespace

namespace snake {

class Grid2d {
  using COORD = std::pair<int, int>;
  using SNAKE_DATA = std::deque<std::tuple<int, int, int>>;
  using SNAKE_DATA_CONST_ITERATOR = SNAKE_DATA::const_iterator;
  using SNAKE_DATA_ITERATOR = SNAKE_DATA::iterator;
  using GRID_DATA = std::vector<SNAKE_DATA>;

  GRID_DATA snakes_ = GRID_DATA(2);
  std::vector<int> snake_length_ = std::vector<int>(2);
  std::vector<std::string> grid_;
  int width_, height_;

 public:
  std::pair<int, int> food;

  Grid2d(int width, int height)
      : width_(width),
        height_(height),
        grid_(std::vector<std::string>(width * height)) {
    for (int row = 0; row < height; ++row) {
      for (int col = 0; col < width; ++col) {
        grid_[row * width + col] = std::format("[{:2d},{:2d}]", col, row);
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

  void print() {
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

    bool isFilled = false;
    for (auto i = 0; auto it : grid_) {
      int row = i / width_;
      int col = i % width_;

      isFilled = filled.find({col, row}) != filled.end();

      // debug((isFilled) ? ":::::::" : "{}", it);
      debug((isFilled) ? "::" : "[]");
      debug((++i % width_) == 0 ? "\n" : "");
    }
  };
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_GRID_2D_H_
