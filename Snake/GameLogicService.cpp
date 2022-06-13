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
		|| isPlayerCollision(gameState->snake_head[0], gameState->snake_head[1], settings)) {
		gameState->gamePhase = LOSE;
		return;
	}

	if (isBorderCollision(gameState->snake_head[1], settings)
		|| isSelfCollision(gameState->snake_head[1], settings)
		|| isPlayerCollision(gameState->snake_head[1], gameState->snake_head[0], settings)) {
		gameState->gamePhase = WIN;
		return;
	}

	// todo: move to gameStateHolder

	SnakePart* snakeHead = gameState->snake_head[0];
	if (isCollide(gameState->food, snakeHead->coord)) {
		
		gameState->score++;

		srand((unsigned)time(0));
		gameState->food = {
			 settings.leftBoundaries + rand() % (settings.rightBoundaries - settings.leftBoundaries),
			 settings.topBoundaries + rand() % (settings.bottomBoundaries - settings.topBoundaries),
		};

		SnakePart* tail = snakeHead;
		for (; tail->next != NULL; tail = tail->next);

		tail->next = new SnakePart();
		tail->next->coord = { tail->coord.x - 1, tail->coord.y };
		tail->next->direction = Direction::RIGHT;
		tail->next->next = nullptr;
	}

	if (gameState->score >= settings.scoreToWin) {
		gameState->gamePhase = WIN;
	}
}

} // namespace snake
