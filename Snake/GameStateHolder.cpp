#include "GameStateHolder.h"

#include <vector>


namespace snake {
	namespace {
		GameState* clone(const GameState* state) {

			int playerCount = 2; // todo: it should got from settings

			GameState* res = new GameState();
			res->frame = state->frame;
			
			for (int i = 0; i < playerCount; i++) {

				if (state->snake_head[i] == nullptr) {
					continue;
				}

				SnakePart* prevSnakePart = state->snake_head[i];
				SnakePart* nextSnakePart = new SnakePart();

				nextSnakePart->coord = { prevSnakePart->coord.x, prevSnakePart->coord.y };
				nextSnakePart->direction = prevSnakePart->direction;
				nextSnakePart->next = nullptr;

				res->snake_head[i] = nextSnakePart;
				res->input[i] = { Direction::NONE, SystemCommand::NONE };

				for (SnakePart* it = prevSnakePart->next; it != nullptr; it = it->next) {

					nextSnakePart->next = new SnakePart();
					nextSnakePart->next->coord = { it->coord.x, it->coord.y };
					nextSnakePart->next->direction = it->direction;
					nextSnakePart->next->next = nullptr;

					nextSnakePart = nextSnakePart->next;
				}
			}

			res->food = { state->food.x, state->food.y };
			res->score = state->score;
			res->gamePhase = state->gamePhase;

			return res;
		}
	
		inline Direction GetOpposite(const Direction* dir) {
			switch (*dir)
			{
			case Direction::LEFT:
				return Direction::RIGHT;
			case Direction::RIGHT:
				return Direction::LEFT;
			case Direction::UP:
				return Direction::DOWN;
			case Direction::DOWN:
				return Direction::UP;
			default:
				return Direction::NONE;
			}
		}

		inline Coord To(const Coord coord, const Direction dir) {

			switch (dir) {
			case Direction::LEFT:
				return { coord.x - 1, coord.y };
			case Direction::RIGHT:
				return { coord.x + 1, coord.y };
			case Direction::UP:
				return { coord.x, coord.y - 1 };
			case Direction::DOWN:
				return { coord.x, coord.y + 1 };
			default:
				return { coord.x, coord.y };
			}
		}
	}

	//* contains game state and return final presentation of it
    GameState* GameStateHolder::ApplyForces(std::vector<Input> inputs[2], GameSettigs settings) {

		int currentIndex = _frame++ % _capacity;
		int nextIndex = _frame % _capacity;

		GameState* gameState = _ringBuffer[currentIndex];

		GameState* nextGameState = clone(gameState);
		nextGameState->frame = _frame;

		if (_frame > _capacity) {
			delete _ringBuffer[nextIndex];
		}

		_ringBuffer[nextIndex] = nextGameState;

		for (size_t i = 0; i < _playerCount; i++) {


			_stateInputs[i][nextIndex] = {};
			SnakePart* snakeHead = nextGameState->snake_head[i];
			if (!inputs[i].empty()) {

				Direction inputDirection = inputs[i].front().direction;

				if (inputDirection != GetOpposite(&inputDirection)) {
					snakeHead->direction = inputDirection;
					nextGameState->input[i] = { inputDirection };
					_stateInputs[i][nextIndex] = { inputDirection };
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
		}

        return nextGameState;
    }

	GameState* GameStateHolder::GetState(int frame) {
		return _ringBuffer[frame % _capacity];
	}

	Input GameStateHolder::GetInput(int frame, int index) {
		return _stateInputs[index][frame % _capacity];
	}	

} // namespace snake
