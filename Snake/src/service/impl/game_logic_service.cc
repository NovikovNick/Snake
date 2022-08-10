#include "../game_logic_service.h"

#include <stdlib.h>
#include <time.h>

namespace snake {

namespace {

bool isPlayerCollision(const Snake& p1, const Snake& p2) {
  for (auto p2Part : p2.getParts()) {
    if (p2Part.first == p1.getHeadCoord()) {
      return true;
    }
  }
  return false;
}

bool isPlayerReachedScore(const int& playerScore, const int& scoreToWin) {
  if (playerScore >= scoreToWin) {
    return true;  // more lines for debug :)
  }

  return false;
}

}  // namespace

const GamePhase& GameLogicService::check(const GameState& gameState,
                                         const GameSettigs& settings) {
  if (gameState.getPhase() != GamePhase::kInProcess) {
    return gameState.getPhase();
  };
  const Coord& leftTop = {settings.leftBoundaries - 1,
                          settings.topBoundaries - 1};
  const Coord& rightBottom = {settings.rightBoundaries,
                              settings.bottomBoundaries};

  if (!gameState.getPlayer(0).isInBound(leftTop, rightBottom)) {
    return GamePhase::kLose;
  }
  if (!gameState.getPlayer(0).isValid()) {
    return GamePhase::kLose;
  }
  if (isPlayerCollision(gameState.getPlayer(0), gameState.getPlayer(1))) {
    return GamePhase::kLose;
  }
  if (gameState.getScore(1) == settings.scoreToWin) {
    return GamePhase::kLose;
  }

  if (!gameState.getPlayer(1).isInBound(leftTop, rightBottom)) {
    return GamePhase::kWin;
  }
  if (!gameState.getPlayer(1).isValid()) {
    return GamePhase::kWin;
  }
  if (isPlayerCollision(gameState.getPlayer(1), gameState.getPlayer(0))) {
    return GamePhase::kWin;
  }
  if (gameState.getScore(0) == settings.scoreToWin) {
    return GamePhase::kWin;
  }

  return GamePhase::kInProcess;
}

}  // namespace snake
