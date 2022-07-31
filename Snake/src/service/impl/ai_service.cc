#include "../ai_service.h"

#include <stdlib.h>

#include <queue>
#include <unordered_map>

#include "../Pathfinder.h"

namespace snake {

namespace {

Pathfinder pathfinder;

}  // namespace

InputDTO AIService::GetInputs(const GameState& gameState,
                              const GameSettigs& settings) {
  const Snake& player = gameState.getPlayer(0);
  const Snake& bot = gameState.getPlayer(1);
  const Coord& bot_head_coord = bot.getHeadCoord();
  const Coord& food = gameState.getFood();

  std::vector<Coord> barriers;
  for (auto snakePart : player.getParts()) {
    barriers.push_back(snakePart.first);
  }
  for (auto snakePart : bot.getParts()) {
    barriers.push_back(snakePart.first);
  }
  for (int x = settings.leftBoundaries; x <= settings.rightBoundaries; x++) {
    barriers.push_back({x, settings.topBoundaries - 1});
    barriers.push_back({x, settings.bottomBoundaries});
  }
  for (int y = settings.topBoundaries; y <= settings.bottomBoundaries; y++) {
    barriers.push_back({settings.leftBoundaries - 1, y});
    barriers.push_back({settings.rightBoundaries, y});
  }

  Path path = pathfinder.FindPath(bot_head_coord, food, barriers);

  return InputDTO{{{path.path[0]}}, path.ctx};
}
}  // namespace snake
