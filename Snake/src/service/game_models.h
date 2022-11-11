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

#endif  // !SNAKE_SNAKE_GAME_MODEL_H_
