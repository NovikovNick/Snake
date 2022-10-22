#include "../game_state_service.h"

#include <unordered_set>

#include "../../model/common_models.h"
#include "../game_models.h"

#define SNAKE_DEBUG 1

namespace snake {

namespace {
Snake newSnake(const int& x, const int& y, const int& lenght,
               const Direction& dir) {
  std::vector<std::pair<Coord, Direction>> list{
      std::make_pair(Coord{x, y}, dir)};

  for (int i = 0; i < lenght; ++i) {
    list.push_back(std::make_pair(list.back().first - dir, dir));
  }

  return Snake(list);
}

Coord GenerateNewFood(const GameState& gameState, GameSettigs& settings) {
  std::unordered_set<Coord> set;

  for (size_t i = 0; i < 2; i++) {
    for (auto part : gameState.getPlayer(i).getParts()) {
      set.insert(part.first);
    }
  }

  Coord res;
  do {
    res.x = settings.leftBoundaries +
            rand() % (settings.rightBoundaries - settings.leftBoundaries);
    res.y = settings.topBoundaries +
            rand() % (settings.bottomBoundaries - settings.topBoundaries);

  } while (set.find(res) != set.end());

  return res;
}
}  // namespace

GameState GameStateService::initState(const GameSettigs& settings) const {
#if SNAKE_DEBUG
  std::cout << "Init first game state. RVO optimisation excpected" << std::endl;
#endif

  auto player = newSnake(settings.startPlayedXCoord, settings.startPlayedYCoord,
                         settings.startLenght, settings.startPlayedDirection);
  auto enemy = newSnake(10, 13, 3, Direction::kRight);

  auto res = GameState(0, player, enemy);
  res.setFood({settings.startFoodXCoord, settings.startFoodYCoord});
  return res;
}

GameState GameStateService::applyForces(
    const GameState& prev_state, const std::vector<Direction>& inputs) const {
  const auto& prev_player = prev_state.getPlayer(0);
  const auto& prev_enemy = prev_state.getPlayer(1);

  auto next_player = snake_service_->move(prev_player, inputs[0]);
  auto next_enemy = snake_service_->move(prev_enemy, inputs[1]);

  bool is_player_gained = next_player.getHeadCoord() == prev_state.getFood();
  bool is_bot_gained = next_enemy.getHeadCoord() == prev_state.getFood();

  auto next_state = prev_state;

  // next_state.setDebugContext(botInput.ctx);
  // next_state.setInputs(inputs);
  GameSettigs emptySettings;

  if (is_player_gained) {
    next_player.gain();
    next_state.setFood(GenerateNewFood(prev_state, emptySettings));

    next_state.setScore(0, prev_state.getScore(0) + 1);
  } else if (is_bot_gained) {
    next_enemy.gain();
    next_state.setFood(GenerateNewFood(prev_state, emptySettings));
    next_state.setScore(1, prev_state.getScore(1) + 1);
  }

  next_state.setPlayer(next_player, 0);
  next_state.setPlayer(next_enemy, 1);

#if SNAKE_DEBUG
  std::cout << "Forces applyied [" << prev_state.getFrame() << "] "
            << std::endl;
#endif

  return next_state;
}

}  // namespace snake
