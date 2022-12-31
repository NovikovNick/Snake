#ifndef SNAKE_SNAKE_GAME_GRID_2D_H_
#define SNAKE_SNAKE_GAME_GRID_2D_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../serialization.h"
#include "direction.h"
#include "game_object.h"
#include "grid_cell.h"

namespace snake {
using SNAKE_PART = std::tuple<int, int, Direction>;
using GAME_OBJECT = std::tuple<int, int, SNAKE_INDEX, int>;
using SNAKE_DATA = std::deque<SNAKE_PART>;
using SNAKE_DATA_CONST_ITERATOR = SNAKE_DATA::const_iterator;
using SNAKE_DATA_ITERATOR = SNAKE_DATA::iterator;
using GAME_OBJECT_ITERATOR = std::vector<GAME_OBJECT>::iterator;
using byte = unsigned char;

// template <typename COORD>
class Grid2d final {
  using COORD = GridCell;
  using GRID_DATA = std::vector<COORD>;
  using COORD_ITERATOR = GRID_DATA::iterator;

  struct GameObjectHash {
    int operator()(const GAME_OBJECT& a) const {
      auto [x, y, index, snake_id] = a;
      auto h1 = std::hash<int>{}(x);
      auto h2 = std::hash<int>{}(y);
      return h1 == h2 ? h1 : h1 ^ h2;
    }
  };
  struct GameObjectEquals {
    bool operator()(const GAME_OBJECT& a, const GAME_OBJECT& b) const {
      auto& [x1, y1, index1, snake_id1] = a;
      auto& [x2, y2, index2, snake_id2] = b;
      return (snake_id1 == snake_id2) && (x1 == x2) && (y1 == y2);
    }
  };
  using GAME_OBJECT_SET =
      std::unordered_set<GAME_OBJECT, GameObjectHash, GameObjectEquals>;

 public:
  std::vector<GAME_OBJECT_SET> game_objects_;
  std::vector<SNAKE_DATA> snakes_;
  std::vector<int> snake_length_;
  int width_, height_;
  COORD food;
  int snake_count;

  Grid2d(int width, int height, int snake_count)
      : width_(width),
        height_(height),
        snake_count(snake_count),
        snakes_(std::vector<SNAKE_DATA>(snake_count)),
        snake_length_(std::vector<int>(snake_count)),
        game_objects_(std::vector<GAME_OBJECT_SET>(snake_count)){};

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

  COORD GetSnakeHead(const int& index) const {
    auto [length, iter] = GetSnake(index);
    auto [x, y, dir] = *iter;
    return COORD(x, y);
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
      *(out++) = COORD(x + 0, y + 1);
    }
    if (x + 1 < width_ && !IsSnake(x + 1, y)) {
      index = width_ * (y + 0) + (x + 1);
      *(out++) = COORD(x + 1, y + 0);
    }
    if (y - 1 >= 0 && !IsSnake(x, y - 1)) {
      index = width_ * (y - 1) + (x + 0);
      *(out++) = COORD(x + 0, y - 1);
    }
    if (x - 1 >= 0 && !IsSnake(x - 1, y)) {
      index = width_ * (y + 0) + (x - 1);
      *(out) = COORD(x - 1, y + 0);
    }
  }

  bool IsSnake(const int& x, const int& y) const {
    for (int snake_id = 0; snake_id < snake_count; ++snake_id) {
      if (IsSnake(x, y, snake_id)) return true;
    }
    return false;
  }

  bool IsSnake(const int& x, const int& y, const int snake_id) const {
    if (game_objects_[snake_id].empty()) {
      return false;
    }
    auto snake_part_set = game_objects_[snake_id];
    return snake_part_set.find({x, y, SNAKE_INDEX::EMPTY, snake_id}) !=
           snake_part_set.end();
  }

  bool IsOutOfBound(const int& x, const int& y) const {
    return x < 0 || y < 0 || x >= width_ || y >= height_;
  }

  void RebuildFilled(const int snake_id) {
    auto set = GAME_OBJECT_SET(32, GameObjectHash{}, GameObjectEquals{});

    const auto& snake = snakes_[snake_id];
    int size = snake_length_[snake_id];
    for (int i = 1; i < (size - 1); ++i) {
      const auto [x, y, cdir] = snake[i];
      const auto [_, __, ndir] = snake[i + 1];
      SNAKE_INDEX index;
      if (cdir == ndir) {
        index = (cdir == Direction::kRight || cdir == Direction::kLeft)
                    ? SNAKE_INDEX::BODY_HOR
                    : SNAKE_INDEX::BODY_VER;
      } else if ((cdir == Direction::kRight && ndir == Direction::kUp) ||
                 (cdir == Direction::kDown && ndir == Direction::kLeft)) {
        index = SNAKE_INDEX::TURN_TL;
      } else if ((cdir == Direction::kLeft && ndir == Direction::kUp) ||
                 (cdir == Direction::kDown && ndir == Direction::kRight)) {
        index = SNAKE_INDEX::TURN_TR;
      } else if ((cdir == Direction::kRight && ndir == Direction::kDown) ||
                 (cdir == Direction::kUp && ndir == Direction::kLeft)) {
        index = SNAKE_INDEX::TURN_BL;
      } else if ((cdir == Direction::kLeft && ndir == Direction::kDown) ||
                 (cdir == Direction::kUp && ndir == Direction::kRight)) {
        index = SNAKE_INDEX::TURN_BR;
      }
      set.insert({x, y, index, snake_id});
    }

    const auto [hx, hy, hdir] = snake[0];
    SNAKE_INDEX hindex;
    switch (hdir) {
      case Direction::kUp:
        hindex = SNAKE_INDEX::HEAD_TOP;
        break;
      case Direction::kDown:
        hindex = SNAKE_INDEX::HEAD_BTM;
        break;
      case Direction::kLeft:
        hindex = SNAKE_INDEX::HEAD_LFT;
        break;
      case Direction::kRight:
        hindex = SNAKE_INDEX::HEAD_RHT;
        break;
    }
    set.insert({hx, hy, hindex, snake_id});

    const auto [tx, ty, tdir] = snake[size - 1];
    SNAKE_INDEX tindex;
    switch (tdir) {
      case Direction::kUp:
        tindex = SNAKE_INDEX::TAIL_BTM;
        break;
      case Direction::kDown:
        tindex = SNAKE_INDEX::TAIL_TOP;
        break;
      case Direction::kLeft:
        tindex = SNAKE_INDEX::TAIL_RHT;
        break;
      case Direction::kRight:
        tindex = SNAKE_INDEX::TAIL_LFT;
        break;
    }
    set.insert({tx, ty, tindex, snake_id});

    game_objects_[snake_id] = set;
  }

  SNAKE_INDEX GetType(const int& x, const int& y) const {
    if (food.GetX() == x && food.GetY() == y) return SNAKE_INDEX::APPLE;

    for (int snake_id = 0; snake_id < snake_count; ++snake_id) {
      if (game_objects_[snake_id].empty()) {
        continue;
      }
      auto snake_part_set = game_objects_[snake_id];
      auto it_snake = snake_part_set.find({x, y, SNAKE_INDEX::EMPTY, snake_id});
      if (it_snake == snake_part_set.end()) {
        continue;
      }
      const auto& [_, __, index, ___] = *it_snake;
      return index;
    }

    return SNAKE_INDEX::EMPTY;
  }

  static byte* serialize(const Grid2d& src, byte** dst) {
    byte* begin = *dst;
    begin = serialisation::write(&begin, src.width_);
    begin = serialisation::write(&begin, src.height_);
    begin = serialisation::write(&begin, src.snake_count);
    begin = serialisation::write(&begin, src.food);
    begin = serialisation::write(&begin, src.snake_length_);

    begin = serialisation::write(
        &begin,
        std::vector<SNAKE_PART>(src.snakes_[0].begin(), src.snakes_[0].end()));
    begin = serialisation::write(
        &begin,
        std::vector<SNAKE_PART>(src.snakes_[1].begin(), src.snakes_[1].end()));

    return begin;
  };
  
  static byte* deserialize(byte* src, Grid2d& dst) {
    src = serialisation::read(src, &dst.width_);
    src = serialisation::read(src, &dst.height_);
    src = serialisation::read(src, &dst.snake_count);
    src = serialisation::read(src, &dst.food);
    src = serialisation::readVector(src, &dst.snake_length_);

    dst.snakes_.resize(2);
    dst.game_objects_.resize(2);
    std::vector<SNAKE_PART> snake;

    src = serialisation::readVector(src, &snake);
    dst.AddSnake(0, snake.begin(), snake.end());
    dst.RebuildFilled(0);

    snake.clear();

    src = serialisation::readVector(src, &snake);
    dst.AddSnake(1, snake.begin(), snake.end());
    dst.RebuildFilled(1);

    return src;
  };
};

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_GRID_2D_H_
