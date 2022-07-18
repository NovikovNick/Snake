#include "../AIService.h"

#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include "../Pathfinder.h"

namespace snake {

namespace {

    Pathfinder pathfinder;

} // namespace

InputDTO AIService::getInputs(const GameState& gameState, const GameSettigs& settings) {

    const Snake& player = gameState.getPlayer(0);
    const Snake& bot = gameState.getPlayer(1);
    
    std::vector<Coord> barriers;
    for (auto snakePart : player.getParts()) {
        barriers.push_back(snakePart.first);
    }
    for (auto snakePart : bot.getParts()) {
        barriers.push_back(snakePart.first);
    }
    for (int x = settings.leftBoundaries; x <= settings.rightBoundaries; x++) {
        barriers.push_back({ x, settings.topBoundaries - 1});
        barriers.push_back({ x, settings.bottomBoundaries });
    }
    for (int y = settings.topBoundaries; y <= settings.bottomBoundaries; y++) {
        barriers.push_back({settings.leftBoundaries - 1, y});
        barriers.push_back({settings.rightBoundaries, y});
    }

    Path path = pathfinder.findPath(bot.getHeadCoord(), gameState.getFood(), barriers);
    
    return InputDTO{
            { {path.path[0]} },
            path.ctx 
    };
}
} // namespace snake
