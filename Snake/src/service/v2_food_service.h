#ifndef SNAKE_SNAKE_FOOD_SERVICE_H_
#define SNAKE_SNAKE_FOOD_SERVICE_H_
#include <random>
#include <stack>

#include "../model/game_state.h"
#include "../model/grid.h"
#include "../util.h"

namespace snake {
class FoodService {
  using COORD = GridCell;
  using STATE = GameState;
  int width, height;
  std::stack<COORD> foods_sequence_;

 public:

  std::vector<GridCell> foods;

  FoodService(const int width, const int height)
      : width(width),
        height(height),
        foods_sequence_(GenerateFoods(width, height)) {}

  void initFood() {
    foods = {
        {6, 3}, {0, 4}, {3, 0}, {5, 7}, {5, 5}, {9, 6}, {9, 3}, {6, 6}, {2, 5},
        {0, 8}, {5, 2}, {8, 8}, {3, 8}, {8, 2}, {3, 9}, {3, 3}, {3, 6}, {5, 4},
        {1, 9}, {0, 6}, {7, 2}, {7, 8}, {2, 8}, {2, 3}, {4, 3}, {7, 7}, {2, 4},
        {7, 0}, {8, 1}, {3, 5}, {5, 0}, {2, 1}, {8, 0}, {6, 9}, {8, 5}, {8, 4},
        {9, 2}, {9, 5}, {9, 0}, {0, 9}, {2, 2}, {9, 7}, {4, 1}, {1, 7}, {3, 2},
        {7, 1}, {4, 5}, {4, 8}, {4, 9}, {4, 0}, {8, 9}, {6, 1}, {3, 4}, {0, 5},
        {2, 9}, {9, 8}, {6, 5}, {0, 1}, {7, 5}, {0, 0}, {1, 5}, {7, 9}, {0, 7},
        {1, 3}, {7, 4}, {4, 4}, {9, 1}, {3, 1}, {6, 2}, {1, 4}, {5, 8}, {9, 9},
        {5, 3}, {7, 6}, {6, 7}, {4, 6}, {6, 0}, {1, 8}, {6, 8}, {5, 1}, {2, 0},
        {8, 6}, {0, 2}, {5, 6}, {2, 7}, {1, 1}, {4, 7}, {1, 2}, {1, 6}, {7, 3},
        {2, 6}, {8, 7}, {6, 4}, {8, 3}, {5, 9}, {0, 3}, {9, 4}, {4, 2}, {3, 7},
        {1, 0}};
  }

  bool SetFood(STATE& state) {
    if (state.foods.empty()) return false;
    state.grid.food = state.foods.front();
    state.foods.erase(state.foods.begin());
    return !state.foods.empty();
  }

  void AddFoodIfAbsent(COORD& coord) {
    if (foods_sequence_.empty() || foods_sequence_.top() != coord) {
      foods_sequence_.push(coord);
    }
  }

  inline bool HasFood() const { return !foods_sequence_.empty(); }

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
