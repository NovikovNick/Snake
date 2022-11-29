#ifndef SNAKE_GAME_EVENT_SERVICE_H_
#define SNAKE_GAME_EVENT_SERVICE_H_
#include <vector>

#include "../model/game_event.h"
#include "deque"

namespace snake {

class GameEventService final {
 public:
  void sendEvent(const GameEvent& input) { queue_.push_front(input); }
  GameEvent pollEvent() {
    if (!queue_.empty()) {
      auto res = queue_.front();
      queue_.clear();
      return res;
    }
    return GameEvent{};
  }

 private:
  std::deque<GameEvent> queue_;
};

}  // namespace snake

#endif  // SNAKE_GAME_EVENT_SERVICE_H_
