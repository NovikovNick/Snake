#include "GameStateHolder.h"

#include <vector>


namespace snake {
	namespace {
		GameState* clone(const GameState* state) {
			GameState* res = new GameState({
				state->frame,
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
	
		inline Direction GetOpposite(const Direction* dir) {
			switch (*dir)
			{
			case LEFT:
				return RIGHT;
			case RIGHT:
				return LEFT;
			case UP:
				return DOWN;
			case DOWN:
				return UP;
			default:
				return NONE;
			}
		}

		inline Coord To(const Coord coord, const Direction dir) {

			switch (dir) {
			case LEFT:
				return { coord.x - 1, coord.y };
			case RIGHT:
				return { coord.x + 1, coord.y };
			case UP:
				return { coord.x, coord.y - 1 };
			case DOWN:
				return { coord.x, coord.y + 1 };
			default:
				return { coord.x, coord.y };
			}
		}
	}

	//* contains game state and return final presentation of it
    GameState* GameStateHolder::ApplyForces(std::vector<Input> inputs, GameSettigs settings) {

		int currentIndex = _frame++ % _capacity;
		int nextIndex = _frame % _capacity;

		GameState* gameState = _ringBuffer[currentIndex];

		GameState* nextGameState = clone(gameState);
		nextGameState->frame = _frame;

		_ringBuffer[nextIndex] = nextGameState;
		_stateInputs[nextIndex] = {};

		SnakePart* snakeHead = nextGameState->snake_head;
		if (!inputs.empty()) {

			Direction inputDirection = inputs.front().direction;
			
			if (inputDirection != GetOpposite(&inputDirection)) {
				snakeHead->direction = inputDirection;
				nextGameState->input = { inputDirection };
				_stateInputs[nextIndex] = { inputDirection };
			}
		}

		Direction prevDir;
		bool isFirst = true;
		for (auto it = snakeHead; it != NULL; it = it->next) {

			Direction dir = it->direction;

			if (!isFirst) {
				it->direction = prevDir;
			}
			isFirst = false;
			
			prevDir = dir;

			it->coord = To(it->coord, dir);
		}
        
        return nextGameState;
    }

	GameState* GameStateHolder::GetState(int frame) {
		return _ringBuffer[frame % _capacity];
	}

	Input GameStateHolder::GetInput(int frame) {
		return _stateInputs[frame % _capacity];
	}	

} // namespace snake
