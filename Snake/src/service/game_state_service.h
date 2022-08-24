#ifndef SNAKE_SNAKE_GAME_STATE_SERVICE_H_
#define SNAKE_SNAKE_GAME_STATE_SERVICE_H_

#include <vector>

#include "../model/common_models.h"
#include "game_models.h"
#include "snake_service.h"

namespace snake {

class IState {
 public:
  virtual const Coord& getFrom() const = 0;
  virtual ~IState() = default;
};

using State = std::shared_ptr<IState>;

class MyService final {
 public:
  State get() const;
};



class GameStateService final {
 private:
  std::shared_ptr<SnakeService> snake_service_;

 public:
  GameStateService(std::shared_ptr<SnakeService> snake_service)
      : snake_service_(snake_service) {}

  GameState initState(const GameSettigs& settings) const;
  GameState applyForces(const GameState& state,
                        const std::vector<Direction>& inputs) const;
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_STATE_SERVICE_H_
