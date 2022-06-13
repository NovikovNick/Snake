#include "GameLogicService.h"

#include <stdlib.h>
#include <time.h>

namespace snake {

namespace {

bool isCollide(Coord& a, Coord& b) {
	return a.x == b.x && a.y == b.y;
}

bool isBorderCollision(SnakePart* snakeHead, GameSettigs& settings) {
	return snakeHead->coord.x <= settings.leftBoundaries - 1
		|| snakeHead->coord.x >= settings.rightBoundaries
		|| snakeHead->coord.y <= settings.topBoundaries - 1
		|| snakeHead->coord.y >= settings.bottomBoundaries;
}


bool isSelfCollision(SnakePart* snakeHead, GameSettigs& settings) {

	SnakePart* tail = snakeHead->next;
	for (; tail != NULL; tail = tail->next) {
		if (isCollide(tail->coord, snakeHead->coord)) {
			return true;
		}
	}
	return false;
}


bool isPlayerCollision(SnakePart* player, SnakePart* enemy, GameSettigs& settings) {

	SnakePart* tail = enemy;
	for (; tail != NULL; tail = tail->next) {
		if (isCollide(tail->coord, player->coord)) {
			return true;
		}
	}
	return false;
}


} // namespace

void GameLogicService::check(GameState* gameState, GameSettigs settings) {

	if (gameState->gamePhase != IN_PROCESS) {
		return;
	};

	if (isBorderCollision(gameState->snake_head[0], settings)
		|| isSelfCollision(gameState->snake_head[0], settings)
		|| isPlayerCollision(gameState->snake_head[0], gameState->snake_head[1], settings)
		|| gameState->score[1] >= settings.scoreToWin) {
		gameState->gamePhase = LOSE;
		return;
	}

	if (isBorderCollision(gameState->snake_head[1], settings)
		|| isSelfCollision(gameState->snake_head[1], settings)
		|| isPlayerCollision(gameState->snake_head[1], gameState->snake_head[0], settings)
		|| gameState->score[0] >= settings.scoreToWin) {
		gameState->gamePhase = WIN;
		return;
	}
}

} // namespace snake
