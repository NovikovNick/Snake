#ifndef SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
#define SNAKE_SNAKE_GAME_LOOP_SERVICE_H_

#include <iostream>
#include <thread>

#include "ai_service.h"
#include "game_logic_service.h"
#include "game_models.h"
#include "input_service.h"
#include "render_service.h"

namespace snake {

class GameLoopService final {
 public:
  void Start();
  void Stop();
  GameLoopService(std::shared_ptr<InputService> input_service,
                  std::shared_ptr<RenderService> render_service,
                  std::shared_ptr<GameLogicService> game_logic_service,
                  std::shared_ptr<AIService> ai_service) {
    input_service_ = input_service;
    render_service_ = render_service;
    game_logic_service_ = game_logic_service;
    ai_service_ = ai_service;
  }

 private:
  std::shared_ptr<InputService> input_service_;
  std::shared_ptr<RenderService> render_service_;
  std::shared_ptr<GameLogicService> game_logic_service_;
  std::shared_ptr<AIService> ai_service_;

  std::thread render_thread_;
  bool running_ = false;

  void StartGameLoop();
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_LOOP_SERVICE_H_
