#define BOOST_TEST_MODULE SolutionTest
#define CASE_1 1
#define SNAKE_DEBUG 0

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>
#include <random>
#include <stack>

#include "../src/ai/a_star.h"
#include "../src/ai/impl/a_star_dp.cc"
#include "../src/model/ring_buffer.h"
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

  // todo split to 2 methods!
  int FindPath(const MyCoord& start, const MyCoord& goal, const Grid2d& grid) {
    if (grid.food != start &&
        pathfinder_.FindPath(start, goal, grid, out_.begin(), out_.end())) {
      return GetDirection(out_[1], start);
    } else {
      return FindPath(start, grid);
    }
  };

  int FindPath(const MyCoord& start, const Grid2d& grid) {
    for (int dir = 0; auto vector : dirs_) {
      int nextX = start.GetX() + vector.GetX();
      int nextY = start.GetY() + vector.GetY();
      if (!grid.IsOutOfBound(nextX, nextY) && !grid.IsSnake(nextX, nextY)) {
        return dir;
      }
      ++dir;
    }
    return 0;
  }

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

struct GameStateV2 {
  Grid2d grid;
  std::vector<int> inputs;
  std::vector<int> score;
  bool is_score_reached;
  bool is_collide;
  bool is_food_consumed;

  GameStateV2(const int snake_count, Grid2d&& grid)
      : grid(std::move(grid)),
        inputs(std::vector<int>(snake_count)),
        score(std::vector<int>(snake_count)),
        is_collide(false),
        is_food_consumed(false) {}
};

struct SnakeContextService {
 private:
  SNAKE_DATA snake_;
  std::shared_ptr<AIService> ai_service;

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
      *out = SNAKE_PART(x + dirs[dir].GetX(), y + dirs[dir].GetY(), prevDir);
      prevDir = dir;
      ++out;
    }
  }

 public:
  SnakeContextService(const int& width, const int& height,
                      std::shared_ptr<AIService> ai_service)
      : snake_(SNAKE_DATA(width * height)), ai_service(ai_service){};

  void SetInputs(const GameStateV2& prev, GameStateV2& out) {
    for (int botId = 0; botId < out.inputs.size(); ++botId) {
      prev.grid.CopySnake(botId, snake_.begin());
      auto [x, y, _] = snake_[0];

      MyCoord head(x, y);
      out.inputs[botId] = ai_service->FindPath(head, prev.grid.food, prev.grid);
    }
  }

  void ApplyInputs(const GameStateV2& prev, GameStateV2& out) {
    for (int snake_id = 0; snake_id < prev.inputs.size(); ++snake_id) {
      // load snake to buffer and arrange data
      prev.grid.CopySnake(snake_id, snake_.begin());
      int length = prev.grid.GetSnakeLength(snake_id);
      auto& [head_x_ref, head_y_ref, head_dir_ref] = snake_[0];
      MyCoord head(head_x_ref, head_y_ref);
      auto prev_tail = snake_[length - 1];
      // movement
      head_dir_ref = out.inputs[snake_id];
      moveSnake(snake_.begin(), snake_.end(), snake_.begin());
      // check collision
      out.is_collide = out.grid.IsOutOfBound(head_x_ref, head_y_ref) ||
                       out.grid.IsSnake(head_x_ref, head_y_ref);
      // check food consumption
      if (out.grid.food == head) {
        out.is_food_consumed = true;
        snake_[length] = prev_tail;
        ++out.score[snake_id];
        ++length;
      }
      // save snake into next state
      out.grid.AddSnake(snake_id, snake_.begin(), snake_.begin() + length);
      out.grid.RebuildFilled();
    }
  }
};

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  // services
  int width = 20, height = 20, snake_count = 2, winScore = 10;
  RenderService renderService(width, height);
  auto ai_service = std::make_shared<AIService>(width, height);
  SnakeContextService ctx(width, height, ai_service);

  std::stack<MyCoord> foods_sequence = GenerateFoods(width, height);
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  SNAKE_DATA snake1{{5, 2, 2}, {5, 1, 2}, {5, 0, 2}};

  RingBuffer<GameStateV2> buffer(3);

  // init fst state
  buffer.add(GameStateV2(snake_count, Grid2d(width, height)));
  auto& state = buffer.head();
  state.grid.AddSnake(0, snake0.begin(), snake0.end());
  state.grid.AddSnake(1, snake1.begin(), snake1.end());
  state.grid.food = foods_sequence.top();
  foods_sequence.pop();

  // act
  bool running = !foods_sequence.empty();
  while (running) {
    auto& prev = buffer.head();
    buffer.add(GameStateV2(snake_count, Grid2d(width, height)));
    auto& next = buffer.head();
    next.grid.food = prev.grid.food;
    next.score = prev.score;


    ctx.SetInputs(prev, next);
    ctx.ApplyInputs(prev, next);

    if (next.is_collide) {
      debug("Snake was collided\n");
      running = false;
    }

    if (next.score[0] > winScore) {
      debug("Score is reached!\n");
      running = false;
    }

    while (next.is_food_consumed && !foods_sequence.empty()) {
      next.grid.food = foods_sequence.top();
      foods_sequence.pop();

      next.is_food_consumed =
          next.grid.IsSnake(next.grid.food.GetX(), next.grid.food.GetY());
      debug(next.is_food_consumed
                ? "There is a snake on [{:2d},{:2d}] only {} foods has left\n"
                : "Added food to [{:2d},{:2d}] only {} foods has left\n",
            next.grid.food.GetX(), next.grid.food.GetY(),
            foods_sequence.size());
    }
    running = running && !foods_sequence.empty();

    // assert (render to console)
    next.grid.copy(renderService.GetOutput());
    renderService.render();
  }
}
#endif
}  // namespace snake
