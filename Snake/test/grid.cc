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
  int width = 16, height = 16, count = 2, winScore = 25, frame = 0;

  TestRenderService renderService(width, height);
  auto ai_srv = std::make_shared<AIService>(width, height);
  auto food_srv = std::make_shared<FoodService>(width, height);
  GameStateService state_srv(width, height, ai_srv, food_srv);

  RingBuffer<GameState> buffer(3);

  // init fst state
  buffer.Add(GameState(frame, count, Grid2d(width, height, count)));
  auto& state = buffer[0];
  SNAKE_DATA snake0{{2, 2, 2}, {2, 1, 2}, {2, 0, 2}};
  state.grid.AddSnake(0, snake0.begin(), snake0.end());
  SNAKE_DATA snake1{{5, 2, 2}, {5, 1, 2}, {5, 0, 2}};
  state.grid.AddSnake(1, snake1.begin(), snake1.end());

  food_srv->SetFood(state);

  // act
  bool running = true;
  while (running) {
    buffer.Add(GameState(++frame, count, Grid2d(width, height, count)));
    auto& next = buffer[0];
    auto& prev = buffer[1];

    next.inputs[0] = ai_srv->FindPath(prev.grid.GetSnakeHead(0), prev.grid.food, prev.grid);
    
    state_srv.Move(prev, next);

    running = !next.is_collide;

    // assert (render to console)
    next.grid.copy(renderService.GetOutput());
    renderService.render();
  }
}
#endif
}  // namespace snake
