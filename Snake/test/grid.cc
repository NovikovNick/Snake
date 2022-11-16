#define BOOST_TEST_MODULE SolutionTest
#define CASE_1 1
#define SNAKE_DEBUG 0

#include "../src/model/grid.h"

#include <boost/test/included/unit_test.hpp>

#include "../src/model/game_state.h"
#include "../src/model/ring_buffer.h"
#include "../src/service/ai/a_star.h"
#include "../src/service/v2_ai_service.h"
#include "../src/service/v2_food_service.h"
#include "../src/service/v2_game_state_service.h"
#include "../src/util.h"

namespace snake {

class TestRenderService {
 private:
  std::vector<GAME_OBJECT> gameObjects_;
  int width_;
  std::vector<std::string> signes_{"[]", "**", "::", "``"};

 public:
  TestRenderService(const int width, const int height)
      : width_(width), gameObjects_(std::vector<GAME_OBJECT>(width * height)) {}
  GAME_OBJECT_ITERATOR GetOutput() { return gameObjects_.begin(); }
  void render() {
    std::cout << std::string(width_ * 2, '-') << "\n";
    for (auto i = 0; auto [x, y, type] : gameObjects_) {
      std::cout << signes_[type] << (++i % width_ == 0 ? "\n" : "");
    }
  };
};

class TestFoodService {
  using STATE = GameState;
  using COORD = GridCell;
  std::stack<COORD, std::deque<COORD>> foods_sequence_;

 public:
  TestFoodService(std::deque<GridCell> coords)
      : foods_sequence_(std::stack<COORD, std::deque<COORD>>(coords)) {}

  bool SetFood(STATE& state) {
    bool is_food_not_added = true;
    while (is_food_not_added && !foods_sequence_.empty()) {
      state.grid.food = foods_sequence_.top();
      foods_sequence_.pop();

      is_food_not_added =
          state.grid.IsSnake(state.grid.food.GetX(), state.grid.food.GetY());
    }
    return !foods_sequence_.empty();

  }

  void AddFoodIfAbsent(COORD& coord) {
    if (foods_sequence_.empty() || foods_sequence_.top() != coord) {
      foods_sequence_.push(coord);
    }
  }

  inline bool HasFood() const { return !foods_sequence_.empty(); }
};

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 16, height = 16, count = 2, winScore = 25, frame = 0;
  std::deque<GridCell> coords{{0, 0}, {0, 15}, {15,0}, {15, 15}};
  TestRenderService renderService(width, height);
  auto ai_srv = std::make_shared<AIService>(width, height);
  auto food_srv = std::make_shared<TestFoodService>(coords);
  GameStateService<TestFoodService> state_srv(width, height, ai_srv, food_srv);

  RingBuffer<GameState> buffer(3);

  // init fst state
  buffer.Add(GameState(frame, count, Grid2d(width, height, count)));
  auto& state = buffer[0];
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  state.grid.AddSnake(0, snake0.begin(), snake0.end());
  SNAKE_DATA snake1{{10, 2, 2}, {10, 1, 2}, {10, 0, 2}};
  state.grid.AddSnake(1, snake1.begin(), snake1.end());

  food_srv->SetFood(state);

  // act
  bool running = true;
  while (running) {
    buffer.Add(GameState(++frame, count, Grid2d(width, height, count)));
    auto& next = buffer[0];
    auto& prev = buffer[1];

    next.inputs[0] =
        ai_srv->FindPath(prev.grid.GetSnakeHead(0), prev.grid.food, prev.grid);

    state_srv.Move(prev, next);
    running = food_srv->HasFood() && !next.is_collide;

    // assert (render to console)
    next.grid.copy(renderService.GetOutput());
    renderService.render();
  }
}
#endif
}  // namespace snake
