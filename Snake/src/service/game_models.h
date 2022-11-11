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

struct GameSettigs final {
  // v2
  int width = 30;
  int height = 15;
  int snake_count = 3;
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
