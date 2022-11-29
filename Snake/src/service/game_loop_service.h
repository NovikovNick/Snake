#ifndef SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOOP_SERVICE_H_

#include <iostream>
#include <thread>

#include "../model/game_settings.h"
#include "../model/game_state.h"
#include "../model/ring_buffer.h"
// #include "../ui/ui_service.h"
#include "input_service.h"
#include "v2_ai_service.h"
#include "v2_food_service.h"
#include "v2_game_state_service.h"

namespace snake {

class GameLoopService final {
  using GAME_STATE_HOLDER = RingBuffer<GameState>;

 public:
  void Start();
  void Stop();
  GameLoopService(
      const GameSettigs& settings,
      std::shared_ptr<GameEventService> input_service,
      std::shared_ptr<FoodService> food_srv,
      std::shared_ptr<AIService> ai_service,
      std::shared_ptr<GameStateService<FoodService>> game_state_service/*,
      std::shared_ptr<UIService> ui_srv*/)
      : settings_(settings),
        input_service_(input_service),
        food_srv_(food_srv),
        ai_service_(ai_service),
        game_state_service_(game_state_service)/*,
        ui_srv(ui_srv)*/ {}

 private:
  GameSettigs settings_;
  std::shared_ptr<GameEventService> input_service_;
  std::shared_ptr<FoodService> food_srv_;
  std::shared_ptr<AIService> ai_service_;
  std::shared_ptr<GameStateService<FoodService>> game_state_service_;
  // std::shared_ptr<UIService> ui_srv;

  // move to nameless
  std::thread render_thread_;
  bool running_ = false;
  void StartGameLoop();
  void Render(const int offset, GAME_STATE_HOLDER& holder);

  GAME_STATE_HOLDER initGameStates();
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
