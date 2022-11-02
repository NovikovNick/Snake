#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 0

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>
#include <random>
#include <stack>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star.cc"

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

void print(const int width, GAME_OBJECT_ITERATOR begin,
           GAME_OBJECT_ITERATOR end) {
  std::vector<std::string> signes{"[]", "::", "**", "XX"};
  std::cout << std::string(width * 2, '-') << "\n";
  for (int i = 0; begin != end; ++begin) {
    auto [x, y, type] = *begin;
    std::cout << signes[type] << (++i % width == 0 ? "\n" : "");
  }
}

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
          break;
        }
        ++dir;
      }
    }
    return 0;
  };

 private:
  int GetDirection(const MyCoord& o1, const MyCoord& o2) {
    MyCoord dir = MyCoord(o1.GetX() - o2.GetX(), o1.GetY() - o2.GetY());
    return std::distance(dirs_.begin(),
                         std::find(dirs_.begin(), dirs_.end(), dir));
  }
};

BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 4, height = 4;
  Grid2d grid(width, height);
  // todo add ring buffer

  std::stack<MyCoord> foods = GenerateFoods(width, height);

  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  // SNAKE_DATA snake1{{2, 5, 1}, {3, 5, 1}, {4, 5, 1}};

  grid.AddSnake(0, snake0.begin(), snake0.end());
  // grid.AddSnake(1, snake1.begin(), snake1.end());

  std::vector<GAME_OBJECT> gameObjects(width * height);

  AIService aiService(width, height);

  // act
  grid.food = foods.top();
  foods.pop();
  bool isCollide = false;

  SNAKE_DATA snake(width * height);

  while (!isCollide && !foods.empty()) {
    bool isFoodConsumed = false;

    for (int playerId = 0; playerId < 1; ++playerId) {
      grid.CopySnake(playerId, snake.begin());
      int length = grid.GetSnakeLength(playerId);

      auto& [headX, headY, headDir] = snake[0];
      MyCoord head(headX, headY);

      headDir = aiService.FindPath(head, grid.food, grid);

      auto prevTail = snake[length - 1];

      moveSnake(snake.begin(), snake.end(), snake.begin());

      // 1. check border collision
      isCollide = grid.IsOutOfBound(headX, headY) || grid.IsSnake(headX, headY);
      if (isCollide) {
        debug("Snake was collided\n");
      }

      // 2. check food consumed
      if (!isFoodConsumed && grid.food == head) {
        snake[length] = prevTail;
        ++length;
        isFoodConsumed = true;
      }

      // 4. check player score
      print(snake.begin(), snake.begin() + length);
      grid.AddSnake(playerId, snake.begin(), snake.begin() + length);
    }

    while (isFoodConsumed) {
      grid.food = foods.top();
      foods.pop();
      isFoodConsumed = grid.IsSnake(grid.food.GetX(), grid.food.GetX());
      debug("Placing food into [{:2d},{:2d}] only {} foods has left\n",
            grid.food.GetX(), grid.food.GetX(), foods.size());
    }

    // assert
    grid.copy(gameObjects.begin());
    print(width, gameObjects.begin(), gameObjects.end());
  }
}
}  // namespace snake
