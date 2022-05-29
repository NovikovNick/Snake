#include "GameStateHolder.h"

#include <vector>


namespace snake {
	namespace {
		GameState* clone(const GameState* state) {
			GameState* res = new GameState({
				new SnakePart({
					{state->snake_head->coord.x, state->snake_head->coord.y},
					state->snake_head->direction,
					NULL
				}),
				{state->food.x, state->food.y},
				state->score,
				state->gamePhase,
				{ NONE }
			});


			SnakePart* cursor = res->snake_head;
			for (SnakePart* it = state->snake_head->next; it != NULL; it = it->next) {
				cursor->next = new SnakePart({
					{it->coord.x, it->coord.y},
					it->direction,
					NULL
					});
				cursor = cursor->next;
			}
			return res;
		}
	}

	//* contains game state and return final presentation of it
    GameState* GameStateHolder::ApplyForces(std::vector<Input> inputs, GameSettigs settings) {
        
        // todo: implement

		int currentIndex = _frame++ % _capacity;
		int nextIndex = _frame % _capacity;

		GameState* gameState = _ringBuffer[currentIndex];

		GameState* nextGameState = clone(gameState);

		_ringBuffer[nextIndex] = nextGameState;

		SnakePart* snakeHead = nextGameState->snake_head;
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
				nextGameState->input = { inputDirection };
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
        
        return nextGameState;
    }

	GameState* GameStateHolder::GetState(int frame) {
		return _ringBuffer[frame % _capacity];
	}
	
	
	

} // namespace snake

