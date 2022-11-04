#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 1

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>
#include <random>
#include <stack>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star_dp.cc"
#include "../src/model/util.h"

namespace snake {

std::stack<MyCoord> GenerateFoods(const int width, const int height) {
  std::deque<MyCoord> f;
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      f.push_back(MyCoord(x, y));
    }
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(f.begin(), f.end(), g);
  return std::stack<MyCoord>(f);
}

void moveSnake(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end,
               SNAKE_DATA_ITERATOR out) {
  std::vector<MyCoord> dirs{
      {1, 0},   // right
      {-1, 0},  // left
      {0, 1},   // bottom
      {0, -1}   // top
  };
  for (auto [_, __, prevDir] = *begin; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    *out = std::make_tuple(x + dirs[dir].GetX(), y + dirs[dir].GetY(), prevDir);
    prevDir = dir;
    ++out;
  }
}

void print(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end) {
  std::cout << std::string(4, '-') << "snake" << std::string(4, '-') << "\n";
  for (; begin != end; ++begin) {
    auto [x, y, dir] = *begin;
    std::cout << std::format("[{:2d},{:2d},{:2d}]\n", x, y, dir);
  }
}

class RenderService {
 private:
  std::vector<GAME_OBJECT> gameObjects_;
  int width_;
  std::vector<std::string> signes_{"[]", "::", "**", "XX"};

 public:
  RenderService(const int width, const int height)
      : width_(width), gameObjects_(std::vector<GAME_OBJECT>(width * height)) {}
  GAME_OBJECT_ITERATOR GetOutput() { return gameObjects_.begin(); }
  void render() {
    std::cout << std::string(width_ * 2, '-') << "\n";
    for (auto i = 0; auto [x, y, type] : gameObjects_) {
      std::cout << signes_[type] << (++i % width_ == 0 ? "\n" : "");
    }
  };
};

class AIService {
 private:
  std::vector<MyCoord> out_;
  AStarPathfinder<MyCoord, Grid2d, std::vector<MyCoord>::iterator> pathfinder_;
  std::vector<MyCoord> dirs_{
      {1, 0},   // right
      {-1, 0},  // left
      {0, 1},   // bottom
      {0, -1}   // top
  };

 public:
  AIService(const int width, const int height)
      : out_(std::vector<MyCoord>(width * height)) {}

  int FindPath(const MyCoord& start, const MyCoord& goal, const Grid2d& grid) {
    if (grid.food != start &&
        pathfinder_.FindPath(start, goal, grid, out_.begin(), out_.end())) {
      return GetDirection(out_[1], start);
    } else {
      for (int dir = 0; auto vector : dirs_) {
        int nextX = start.GetX() + vector.GetX();
        int nextY = start.GetY() + vector.GetY();
        if (!grid.IsOutOfBound(nextX, nextY) && !grid.IsSnake(nextX, nextY)) {
          return dir;
        }
        ++dir;
      }
    }
    return 0;
  };

 private:
  int GetDirection(const MyCoord& o1, const MyCoord& o2) {
    MyCoord dir = MyCoord(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
    auto it = std::find(dirs_.begin(), dirs_.end(), dir);

    if (it == dirs_.end()) {
      return 0;
    } else {
      return std::distance(dirs_.begin(), it);
    }
  }
};

struct RingBuffer {
  std::vector<Grid2d> buffer_;
};

//struct SnakeContext {
//  SNAKE_DATA snake(width* height);
//  int length;
//  SNAKE_PART prevTail;
//};

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  // services
  int width = 10, height = 10, winScore = 5;
  RenderService renderService(width, height);
  AIService aiService(width, height);

  std::stack<MyCoord> foods_sequence = GenerateFoods(width, height);
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  SNAKE_DATA snake1{{1, 2, 2}, {1, 1, 2}, {1, 0, 2}};

  // 1. init fst state
  // todo add ring buffer

  Grid2d grid(width, height);
  grid.AddSnake(0, snake0.begin(), snake0.end());
  grid.AddSnake(1, snake1.begin(), snake1.end());

  grid.food = foods_sequence.top();
  foods_sequence.pop();

  // act
  bool isCollide = false;
  bool isFoodConsumed = false;
  bool isReachedScore = false;

  std::vector<int> score(2);

  SNAKE_DATA snake(width * height);  // todo move to separate service
  int length;
  SNAKE_PART prevTail;
  while (!isReachedScore && !isCollide && !foods_sequence.empty()) {
    isFoodConsumed = false;

    for (int playerId = 0; playerId < 2 && !isCollide; ++playerId) {
      // load snake to buffer
      grid.CopySnake(playerId, snake.begin());
      length = grid.GetSnakeLength(playerId);

      auto& [headXRef, headYRef, headDirRef] = snake[0];
      prevTail = snake[length - 1];

      MyCoord head(headXRef, headYRef);
      headDirRef = aiService.FindPath(head, grid.food, grid);

      moveSnake(snake.begin(), snake.end(), snake.begin());

      // 1. check collision
      // need to remove snake and stop the game!
      isCollide = grid.IsOutOfBound(headXRef, headYRef) ||
                  grid.IsSnake(headXRef, headYRef);
      if (isCollide) {
        debug("Snake was collided\n");
      }

      // 2. check food consumption
      if (!isFoodConsumed && grid.food == head) {
        snake[length] = prevTail;
        ++length;
        isFoodConsumed = true;
        ++score[playerId];
      }

      // 3. todo check player score
      if (score[playerId] >= winScore) {
        isReachedScore = true;
        debug("Player {} win!\n", playerId);
      }

      // print(snake.begin(), snake.begin() + length);
      grid.AddSnake(playerId, snake.begin(), snake.begin() + length);
      grid.RebuildFilled();
    }

    while (isFoodConsumed && !foods_sequence.empty()) {
      grid.food = foods_sequence.top();
      foods_sequence.pop();

      // todo IsSnake doesn't work properly
      isFoodConsumed = grid.IsSnake(grid.food.GetX(), grid.food.GetY());
      debug(isFoodConsumed
                ? "There is a snake on [{:2d},{:2d}] only {} foods has left\n"
                : "Added food to [{:2d},{:2d}] only {} foods has left\n",
            grid.food.GetX(), grid.food.GetY(), foods_sequence.size());
    }

    // assert
    grid.copy(renderService.GetOutput());
    renderService.render();
  }
}
}  // namespace snake
