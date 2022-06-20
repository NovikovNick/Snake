#include "AIService.h"

#include <stdlib.h>
#include <unordered_map>

namespace snake {

namespace {

std::vector<Input> DoSimpleStateCondition(GameState* gameState, GameSettigs settings) {
    Direction prevDirection = gameState->snake_head[1]->direction;
    int x = gameState->snake_head[1]->coord.x;
    int y = gameState->snake_head[1]->coord.y;

    int foodX = gameState->food.x;
    int foodY = gameState->food.y;

    Direction dir = Direction::NONE;

    if (x == foodX) {
        dir = foodY > y ? Direction::DOWN : Direction::UP;
    }
    else if (y == foodY) {
        dir = foodX > x ? Direction::RIGHT : Direction::LEFT;
    }

    if (x >= settings.rightBoundaries - 1 && prevDirection == Direction::RIGHT) {
        dir = Direction::UP;
    }
    else if (y <= settings.topBoundaries && prevDirection == Direction::UP) {
        dir = Direction::LEFT;
    }
    else if (x <= settings.leftBoundaries && prevDirection == Direction::LEFT) {
        dir = Direction::DOWN;
    }
    else if (y >= settings.bottomBoundaries - 1 && prevDirection == Direction::DOWN) {
    dir = Direction::RIGHT;
    }

    return { { dir, SystemCommand::NONE } };
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

inline bool equals(Coord& a, Coord& b) {
    return a.x == b.x && a.y == b.y;
}

struct Node {
    Coord coord;
    Direction toPrev = Direction::NONE;
    Node* prev = nullptr;

    Node(Coord from) {
        coord = from;
    }

    Node(Coord from, Direction dir, Node* previous) {
        coord = from;
        toPrev = dir;
        prev = previous;
    }
};

struct hash_coord {
    size_t operator()(const Coord& coord) const
    {
        auto hash1 = std::hash<int>{}(coord.x);
        auto hash2 = std::hash<int>{}(coord.y);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

bool isBorderCollision(Coord coord, GameSettigs& settings) {

    return coord.x <= settings.leftBoundaries - 1
        || coord.x >= settings.rightBoundaries
        || coord.y <= settings.topBoundaries - 1
        || coord.y >= settings.bottomBoundaries;
}

Direction directions[4] = {
        Direction::RIGHT,
        Direction::LEFT,
        Direction::DOWN,
        Direction::UP
};


std::vector<Input> FindPathToFood(GameState* gameState, GameSettigs settings) {

    SnakePart* player = gameState->snake_head[0];
    SnakePart* bot = gameState->snake_head[1];

    Coord from = bot->coord;
    Coord to = gameState->food;
    
    std::unordered_map<Coord, Node*, hash_coord> reachable = { { from, new Node(from) } };
    std::unordered_map<Coord, Node*, hash_coord> explored;

    for (auto it = player; it != nullptr; it = it->next) {
        explored[it->coord] = nullptr;
    }

    for (auto it = bot; it != nullptr; it = it->next) {
        explored[it->coord] = nullptr;
    }

    Direction res = Direction::RIGHT; // by default it 
    Node* pathToFood = nullptr;
    while (!reachable.empty()) {
        auto reachableIterator = reachable.begin();
        Coord coord = reachableIterator->second->coord;

        for (size_t i = 0; i < 4; i++) {
            Coord candidate = coord + directions[i];
            
            if (candidate == to) {
                Node* node = new Node(candidate, directions[i], reachableIterator->second);
                for (auto it = node; it->prev != nullptr; it = it->prev) {
                    res = it->toPrev;
                }
                return { { res, SystemCommand::NONE } };
            }

            bool isNotInReachable = reachable.find(candidate) == reachable.end();
            bool isNotInExplored = explored.find(candidate) == explored.end();
            bool isNotOutOfBorder = !isBorderCollision(candidate, settings);

            if (isNotInReachable && isNotInExplored && isNotOutOfBorder) {
                reachable[candidate] = new Node(candidate, directions[i], reachableIterator->second);
            }
        }
        explored[coord] = nullptr;
        reachable.extract(coord);
    }


    return { { res, SystemCommand::NONE } };
}

} // namespace

std::vector<Input> AIService::getInputs(GameState* gameState, GameSettigs settings) {

    // return DoSimpleStateCondition(gameState, settings);

    return FindPathToFood(gameState, settings);
}
} // namespace snake
