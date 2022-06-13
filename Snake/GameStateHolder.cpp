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
		res->score[i] = state->score[i];

		for (SnakePart* it = prevSnakePart->next; it != nullptr; it = it->next) {

			nextSnakePart->next = new SnakePart();
			nextSnakePart->next->coord = { it->coord.x, it->coord.y };
			nextSnakePart->next->direction = it->direction;
			nextSnakePart->next->next = nullptr;

			nextSnakePart = nextSnakePart->next;
		}
	}

	res->food = { state->food.x, state->food.y };
	res->gamePhase = state->gamePhase;

	return res;
}

inline Direction GetOpposite(Direction& dir) {
	switch (dir)
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


inline bool isCollide(Coord& a, Coord& b) {
	return a.x == b.x && a.y == b.y;
}

Coord generateNewFood(GameState* gameState, GameSettigs& settings) {

	srand((unsigned)time(0));

	Coord res = {
		 settings.leftBoundaries + rand() % (settings.rightBoundaries - settings.leftBoundaries),
		 settings.topBoundaries + rand() % (settings.bottomBoundaries - settings.topBoundaries),
	};

	return res;
}

} // namespace

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
		SnakePart* current = nextGameState->snake_head[i];

		if (!inputs[i].empty()) {

			Direction inputDirection = inputs[i].front().direction;

			if (inputDirection != Direction::NONE && inputDirection != GetOpposite(current->direction)) {
				current->direction = inputDirection;
				nextGameState->input[i] = { inputDirection };
				_stateInputs[i][nextIndex] = { inputDirection };
			}
		}

		Direction prevDir = current->direction;
		Coord prevCoord = current->coord;
		current->coord = To(current->coord, current->direction);

		bool collided = isCollide(gameState->food, current->coord);
		
		for (auto it = current->next; it != nullptr; it = it->next) {

			Direction dir = it->direction;
			it->direction = prevDir;
			prevDir = dir;
			prevCoord = it->coord;
			it->coord = To(it->coord, dir);
			current = it;
		}

		if (collided) {
			current->next = new SnakePart();
			current->next->coord = prevCoord;
			current->next->direction = prevDir;
			current->next->next = nullptr;

			nextGameState->score[i]++;
			nextGameState->food = generateNewFood(nextGameState, settings);
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

void GameStateHolder::StepForward() {
	if (++_frameOffset > 0) {
		_frameOffset = 0;
	}
}

void GameStateHolder::StepBackward() {
	int threshold = _frame <= _capacity
		? _frame
		: _capacity - 1;

	if (--_frameOffset < -threshold) {
		_frameOffset = -threshold;
	}
}

GameState* GameStateHolder::GetStateWithOffset() {
	return GetState(_frame + _frameOffset);
}

} // namespace snake
