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

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int width = 30, height = 15, snake_count = 2, winScore = 25, frame = 0;
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  SNAKE_DATA snake1{{5, 2, 2}, {5, 1, 2}, {5, 0, 2}};

  TestRenderService renderService(width, height);
  FoodService foodService(width, height);
  auto ai_service = std::make_shared<AIService>(width, height);
  GameStateService ctx(width, height, ai_service);

  RingBuffer<GameState> buffer(3);

  // init fst state
  buffer.add(GameState(frame, snake_count, Grid2d(width, height, snake_count)));
  auto& state = buffer.head();
  state.grid.AddSnake(0, snake0.begin(), snake0.end());
  state.grid.AddSnake(1, snake1.begin(), snake1.end());

  foodService.SetFood(state);

  // act
  bool running = true;
  while (running) {
    auto& prev = buffer.head();
    buffer.add(
        GameState(++frame, snake_count, Grid2d(width, height, snake_count)));
    auto& next = buffer.head();
    next.grid.food = prev.grid.food;
    next.score = prev.score;

    debug("Start processing {} frame\n", next.frame);
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

    if (next.is_food_consumed) {
      bool is_food_left = foodService.SetFood(next);
      running = running && is_food_left;
    }

    // assert (render to console)
    next.grid.copy(renderService.GetOutput());
    renderService.render();
  }
}
#endif
}  // namespace snake
