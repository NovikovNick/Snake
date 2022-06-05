#include "GameStateHolder.h"

#include <vector>


namespace snake {
	namespace {
		GameState* clone(const GameState* state) {

			GameState* res = new GameState();
			res->frame = state->frame;
			res->snake_head = new SnakePart();
			res->snake_head->coord = { state->snake_head->coord.x, state->snake_head->coord.y };
			res->snake_head->direction = state->snake_head->direction;

			res->food = { state->food.x, state->food.y };
			res->score = state->score;
			res->gamePhase = state->gamePhase;
			res->input = { NONE };


			SnakePart* cursor = res->snake_head;
			for (SnakePart* it = state->snake_head->next; it != nullptr; it = it->next) {

				cursor->next = new SnakePart();
				cursor->next->coord = { it->coord.x, it->coord.y };
				cursor->next->direction = it->direction;
				cursor->next->next = nullptr;
				
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

		// todo release memory for last element in ring buffer

		int currentIndex = _frame++ % _capacity;
		int nextIndex = _frame % _capacity;

		GameState* gameState = _ringBuffer[currentIndex];

		GameState* nextGameState = clone(gameState);
		nextGameState->frame = _frame;

		if (_frame > _capacity) {
			delete _ringBuffer[nextIndex];
		}

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
		for (auto it = snakeHead; it != nullptr; it = it->next) {

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
