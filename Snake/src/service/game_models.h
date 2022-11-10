#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

namespace snake {

enum class GamePhase : std::uint8_t { kInProcess, kWin, kLose, kPaused };

enum class Direction : std::uint8_t {
  kUp,
  kDown,
  kLeft,
  kRight,
  kNone,
};

enum class SystemCommand : std::int8_t {
  kPause,
  kStepForward,
  kStepBackward,
  kAIStepForward,
  kAIStepBackward,
  kNone
};

enum class DebugMark : std::int8_t { kReachable, kExplored, kPath, kNone };

struct Input final {
  Direction direction = Direction::kNone;
  SystemCommand command = SystemCommand::kNone;
};

struct Coord final {
  int x;
  int y;

  Coord operator+(const Direction& dir) const {
    switch (dir) {
      case Direction::kLeft:
        return {x - 1, y};
      case Direction::kRight:
        return {x + 1, y};
      case Direction::kUp:
        return {x, y - 1};
      case Direction::kDown:
        return {x, y + 1};
      default:
        return {x, y};
    }
  }

  Coord operator-(const Direction& dir) const {
    switch (dir) {
      case Direction::kLeft:
        return {x + 1, y};
      case Direction::kRight:
        return {x - 1, y};
      case Direction::kUp:
        return {x, y + 1};
      case Direction::kDown:
        return {x, y - 1};
      default:
        return {x, y};
    }
  }

  bool operator==(const Coord& other) const {
    return x == other.x && y == other.y;
  }
};

struct DebugItem final {
  Coord coord;
  DebugMark mark;
};

struct DebugContext final {
  std::vector<std::vector<DebugItem>> pathfinding;
};

struct InputDTO final {
  std::vector<Input> inputs;
  DebugContext ctx;
};

struct Path final {
  std::vector<Direction> path;
  DebugContext ctx;
};

struct Food final {
  Coord coord;
};

struct GameSettigs final {
  // v2
  int width = 17;
  int height = 10;
  int snake_count = 1;
  int winScore = 20;

  // v1
  int scoreToWin = 50;
  int initialSpeedMs = 100;
  int maxSpeedMs = 100;

  int leftBoundaries = 1;
  int rightBoundaries = 34;
  int topBoundaries = 2;
  int bottomBoundaries = 16;

  int startPlayedXCoord = 9;
  int startPlayedYCoord = 9;
  int startLenght = 5;
  Direction startPlayedDirection = Direction::kRight;

  int startFoodXCoord = 10;
  int startFoodYCoord = 14;

  int foodSize = 5;
  int snakeSize = 1;

  long foodGenerationSeed = 1658772641;
};

template <typename T>
class GameBufferAllocator {
 public:
  T* arr_;
  int size_ = 0, capacity_;

  explicit GameBufferAllocator(int capacity = 0)
      : capacity_(capacity),
        arr_(capacity == 0
                 ? nullptr
                 : static_cast<T*>(::operator new(sizeof(T) * capacity))) {}

  ~GameBufferAllocator() {
    int used = size_ > capacity_ ? capacity_ : size_;
    while (--used >= 0) {
      (arr_ + used)->~T();
    }
    ::operator delete(arr_);
  }

  GameBufferAllocator(const GameBufferAllocator& rhs) = delete;
  GameBufferAllocator& operator=(const GameBufferAllocator& rhs) = delete;

  GameBufferAllocator(GameBufferAllocator&& rhs) = delete;
  GameBufferAllocator operator=(GameBufferAllocator&& rhs) = delete;
};

template <typename T>
class GameStateBuffer final : private GameBufferAllocator<T> {
 private:
  int cursor_ = -1;
  using GameBufferAllocator<T>::arr_;
  using GameBufferAllocator<T>::capacity_;
  using GameBufferAllocator<T>::size_;

 public:
  explicit GameStateBuffer(const int& capacity = 0)
      : GameBufferAllocator<T>(capacity){};

  void add(const T& item) {
    cursor_ = (cursor_ + 1) % capacity_;
    T* p = arr_ + cursor_;
    if (size_ >= capacity_) {
      (p)->~T();
    }
    new (p) T(item);
    ++size_;
  };

  void add(T&& item) {
    cursor_ = (cursor_ + 1) % capacity_;
    T* p = arr_ + cursor_;
    if (size_ >= capacity_) {
      (p)->~T();
    }
    new (p) T(std::forward(item));
    ++size_;
  };

  const T& head() const noexcept { return arr_[cursor_]; }

  const T& operator[](const int& index) const {
    int offset = (cursor_ - index) % capacity_;
    return arr_[offset < 0 ? capacity_ + offset : offset];
  }

  int getSize() const noexcept {
    return size_ < capacity_ ? cursor_ + 1 : capacity_;
  }
};

}  // namespace snake

// we have right to add specialization to std
namespace std {
template <>
struct hash<snake::Coord> {
  size_t operator()(const snake::Coord& p) const {
    auto hash1 = std::hash<int>{}(p.x);
    auto hash2 = std::hash<int>{}(p.y);

    if (hash1 != hash2) {
      return hash1 ^ hash2;
    }

    // If hash1 == hash2, their XOR is zero.
    return hash1;
  }
};
}  // namespace std

#endif  // !SNAKE_SNAKE_GAME_MODEL_H_
