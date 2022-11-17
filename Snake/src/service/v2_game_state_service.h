#ifndef SNAKE_SNAKE_STATE_SERVICE_H_
#define SNAKE_SNAKE_STATE_SERVICE_H_
#include <vector>

#include "../model/game_state.h"
#include "../model/grid.h"
#include "../util.h"
#include "../snake_api.h"
#include "v2_ai_service.h"

namespace snake {

template <food_srv FOOD_SRV>
class GameStateService {
  using COORD = GridCell;
  using STATE = GameState;
  using STATE_HOLDER = RingBuffer<GameState>;

 private:
  SNAKE_DATA snake_;
  std::shared_ptr<AIService> ai_srv;
  std::shared_ptr<FOOD_SRV> food_srv;

 public:
  GameStateService(const int& width, const int& height,
                   std::shared_ptr<AIService> ai_srv,
                   std::shared_ptr<FOOD_SRV> food_srv)
      : snake_(SNAKE_DATA(width * height)),
        ai_srv(ai_srv),
        food_srv(food_srv){};

  void RollbackAndMove(const int offset, STATE_HOLDER& buf) {
    if (offset == 0) {
      Move(buf[1], buf[0]);
      return;
    }
    
    // 1. pull food
    for (int i = 0; i < offset + 1; ++i) {
      if (buf[i].is_food_consumed) food_srv->AddFoodIfAbsent(buf[i].grid.food);
    }

    // 3. update state of all subsequent -> if state is not invalid
    for (int i = offset; i >= 0; --i) {
      Move(buf[i + 1], buf[i]);
    }
  }

  void Move(const STATE& prev, STATE& next) {
    next.grid.food = prev.grid.food;
    next.score = prev.score;
    next.is_collide = prev.is_collide;
    next.is_score_reached = prev.is_score_reached;
    next.is_food_consumed = false;

    if (!prev.is_collide) {
      SetInputs(prev, next);
      ApplyInputs(prev, next);

      if (next.is_food_consumed) food_srv->SetFood(next);
    }
  }

  void SetInputs(const STATE& prev, STATE& out) {
    for (int botId = 1; botId < out.inputs.size(); ++botId) {
      out.inputs[botId] = ai_srv->FindPath(prev.grid.GetSnakeHead(botId),
                                           prev.grid.food, prev.grid);
    }
  }

  void ApplyInputs(const STATE& prev, STATE& out) {
    for (int snake_id = 0; snake_id < prev.grid.snake_count; ++snake_id) {
      // load snake to buffer and arrange data
      prev.grid.CopySnake(snake_id, snake_.begin());
      int length = prev.grid.GetSnakeLength(snake_id);
      auto& [head_x_ref, head_y_ref, head_dir_ref] = snake_[0];
      auto prev_tail = snake_[length - 1];

      // movement
      head_dir_ref =
          out.inputs[snake_id] >= 0 ? out.inputs[snake_id] : head_dir_ref;
      MoveSnake(snake_.begin(), snake_.end(), snake_.begin());

      // check collision
      out.is_collide = out.is_collide ||
                       out.grid.IsOutOfBound(head_x_ref, head_y_ref) ||
                       prev.grid.IsSnake(head_x_ref, head_y_ref);

      // check food consumption
      COORD head(head_x_ref, head_y_ref);
      if (out.grid.food == head) {
        out.is_food_consumed = true;
        snake_[length] = prev_tail;
        ++out.score[snake_id];
        ++length;
      }
      // save snake into next state
      out.grid.AddSnake(snake_id, snake_.begin(), snake_.begin() + length);
      out.grid.RebuildFilled(snake_id);
    }
  }

  void MoveSnake(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end,
                 SNAKE_DATA_ITERATOR out) {
    std::vector<COORD> dirs{
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

  void Print(SNAKE_DATA_ITERATOR begin, SNAKE_DATA_ITERATOR end) {
    for (; begin != end; ++begin) {
      auto [x, y, dir] = *begin;
      debug("[{:2d},{:2d},{:2d}]\n", x, y, dir);
    }
  }
};

}  // namespace snake

#endif  // SNAKE_SNAKE_STATE_SERVICE_H_
