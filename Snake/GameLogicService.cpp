#include "GameLogicService.h"

#include <stdlib.h>
#include <time.h>

namespace snake {


void GameLogicService::applyForcesAndCheck(GameState* gameState, std::vector<Input> inputs, GameSettigs settings) {

	if (gameState->gamePhase == LOSE || gameState->gamePhase == WIN) {
		return;
	};

	SnakePart* snakeHead = gameState->snake_head;

	if (!inputs.empty()) {

		Direction inputDirection = inputs.front().direction;
		Direction oppositeDirection;

		switch (snakeHead->direction)
		{
		case LEFT:
			oppositeDirection = RIGHT;
			break;
		case RIGHT:
			oppositeDirection = LEFT;
			break;
		case UP:
			oppositeDirection = DOWN;
			break;
		case DOWN:
			oppositeDirection = UP;
			break;
		default:
			break;
		}

		if (inputDirection != oppositeDirection)
		{
			snakeHead->direction = inputDirection;
		}
	}

	Direction prevDir;
	bool isFirst = true;
	for (auto it = snakeHead; it != NULL; it = it->next) {

		Direction dir = it->direction;

		if (!isFirst)
		{
			it->direction = prevDir;
		}
		isFirst = false;

		prevDir = dir;

		switch (dir) {
		case LEFT:
			it->coord.x--;
			break;
		case RIGHT:
			it->coord.x++;
			break;
		case UP:
			it->coord.y--;
			break;
		case DOWN:
			it->coord.y++;
			break;
		default:
			break;
		}
	}

	if (snakeHead->coord.x <= settings.leftBoundaries - 1
		|| snakeHead->coord.x >= settings.rightBoundaries
		|| snakeHead->coord.y <= settings.topBoundaries - 1
		|| snakeHead->coord.y >= settings.bottomBoundaries)
	{
		gameState->gamePhase = LOSE;
		return;
	}

	SnakePart* tail = snakeHead->next;
	for (; tail != NULL; tail = tail->next) {
		if (tail->coord.x == snakeHead->coord.x && tail->coord.y == snakeHead->coord.y) {
			gameState->gamePhase = LOSE;
			return;
		}
	};

	if (gameState->food.x == snakeHead->coord.x && gameState->food.y == snakeHead->coord.y) {

		gameState->score++;

		srand((unsigned)time(0));
		gameState->food = {
			 settings.leftBoundaries + rand() % (settings.rightBoundaries - settings.leftBoundaries),
			 settings.topBoundaries + rand() % (settings.bottomBoundaries - settings.topBoundaries),
		};

		SnakePart* tail = snakeHead;
		for (; tail->next != NULL; tail = tail->next);

		tail->next = new SnakePart{ {tail->coord.x - 1, tail->coord.y}, RIGHT, NULL };
	}

	if (gameState->score >= settings.scoreToWin) {
		gameState->gamePhase = WIN;
	}
}

} // namespace snake
