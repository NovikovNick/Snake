#ifndef SNAKE_SNAKE_FOOD_SERVICE_H_
#define SNAKE_SNAKE_FOOD_SERVICE_H_
#include <random>
#include <stack>

#include "model/game_state.h"
#include "model/grid.h"
#include "util.h"

namespace snake {
class FoodService {
  using COORD = GridCell;
  using STATE = GameStateV2;
  std::stack<COORD> foods_sequence_;

 public:
  FoodService(const int width, const int height)
      : foods_sequence_(GenerateFoods(width, height)) {}

  bool SetFood(STATE& state) {
    bool is_food_not_added = true;
    while (is_food_not_added && !foods_sequence_.empty()) {
      state.grid.food = foods_sequence_.top();
      foods_sequence_.pop();

      is_food_not_added =
          state.grid.IsSnake(state.grid.food.GetX(), state.grid.food.GetY());

      debug(state.is_food_consumed
                ? "There is a snake on [{:2d},{:2d}] only {} foods has left\n"
                : "Added food to [{:2d},{:2d}] only {} foods has left\n",
            state.grid.food.GetX(), state.grid.food.GetY(),
            foods_sequence_.size());
    }
    return !foods_sequence_.empty();
  }

 private:
  std::stack<COORD> GenerateFoods(const int width, const int height) {
    std::deque<COORD> f;
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        f.push_back(COORD(x, y));
      }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(f.begin(), f.end(), g);
    return std::stack<COORD>(f);
  }
};

}  // namespace snake

#endif  // SNAKE_SNAKE_FOOD_SERVICE_H_
