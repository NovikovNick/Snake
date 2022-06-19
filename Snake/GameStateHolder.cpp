#include "GameStateHolder.h"

#include <vector>
#include <unordered_map>


namespace snake {
namespace {

GameState* clone(const GameState* state, int frame) {

	int playerCount = 2; // todo: it should got from settings

	GameState* res = new GameState(frame);
			
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

// A hash function used to hash a pair of any kind
struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);

		if (hash1 != hash2) {
			return hash1 ^ hash2;
		}

		// If hash1 == hash2, their XOR is zero.
		return hash1;
	}
};


Coord generateNewFood(GameState* gameState, GameSettigs& settings) {

	std::unordered_map<std::pair<int, int>, bool, hash_pair> um;

	for (size_t i = 0; i < 2; i++) {
		for (SnakePart* it = gameState->snake_head[i]; it != nullptr; it = it->next) {
			um[std::make_pair(it->coord.x, it->coord.y)] = true;
		}
	}

	Coord res;
	srand((unsigned)time(NULL));
	do {

		res = {
			settings.leftBoundaries + rand() % (settings.rightBoundaries - settings.leftBoundaries),
			settings.topBoundaries + rand() % (settings.bottomBoundaries - settings.topBoundaries),
		};

	} while (um[std::make_pair(res.x, res.y)]);

	return res;

}

} // namespace

	//* contains game state and return final presentation of it
GameState* GameStateHolder::ApplyForces(std::vector<Input> inputs[2], GameSettigs settings) {

	int currentIndex = _frame++ % _capacity;
	int nextIndex = _frame % _capacity;

	GameState* gameState = _ringBuffer[currentIndex];

	GameState* nextGameState = clone(gameState, _frame);

	if (_frame > _capacity && _ringBuffer[nextIndex] != nullptr) {
		// WHY _CRT_SECURITYCRITICAL_ATTRIBUTE?!
		// delete _ringBuffer[nextIndex];
	}

	_ringBuffer[nextIndex] = nextGameState;
	int collidedIndex = -1;

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
			collidedIndex = i;
		}
	}

	if (collidedIndex > -1) {
		nextGameState->score[nextIndex]++;
		nextGameState->food = generateNewFood(nextGameState, settings);
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
