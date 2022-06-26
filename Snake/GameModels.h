#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_
#include "util/log.h"
#include <vector>

namespace snake {

enum GamePhase {
    IN_PROCESS, WIN, LOSE, PAUSED
};

enum class Direction {
    UP, DOWN, LEFT, RIGHT, NONE
};

enum class SystemCommand {
    PAUSE, STEP_FORWARD, STEP_BACKWARD, NONE
};

struct Input {
    Direction direction = Direction::NONE;
    SystemCommand command = SystemCommand::NONE;
};

struct Coord {
   int x;
   int y;

   Coord operator + (const Direction& dir) const {
       switch (dir) {
       case Direction::LEFT:
           return { x - 1, y };
       case Direction::RIGHT:
           return { x + 1, y };
       case Direction::UP:
           return { x, y - 1 };
       case Direction::DOWN:
           return { x, y + 1 };
       default:
           return { x, y };
       }
   }

   bool operator == (const Coord& other) const {
       return x == other.x && y == other.y;
   }
};

enum class DebugMark {
    REACHABLE, EXPLORED, NONE
};

struct DebugItem {
    Coord coord;
    DebugMark mark;
};

struct DebugContext {
    std::vector<std::vector<DebugItem>> pathfinding;
};

struct InputDTO {
    std::vector<Input> inputs;
    DebugContext ctx;
};

struct SnakePart {
    Coord coord;
    Direction direction;
    SnakePart* next = nullptr;

    SnakePart() {
        log("        SnakePart created");
    }

    SnakePart(SnakePart& src) {
        coord = src.coord;
        direction = src.direction;
        next = src.next;
        log("        SnakePart copied");
    }

    ~SnakePart() {
        log("        SnakePart start destoying " + ToString());
        SnakePart* current = next;
        if (current != nullptr) {
            delete current;
        }
        // log("        SnakePart destoyed " + ToString());
    }

    std::string ToString() {

        std::string res = "[";
        res += std::to_string(coord.x);// todo: why read access violation?!
        res += ";";
        res += std::to_string(coord.y);
        res += "]";
        return res;
    }
};

struct Food {
    Coord coord;
};

struct GameState {
    int frame = 0;
    std::vector<SnakePart*> snake_head = { nullptr, nullptr };

    Coord food;
    int score[2] = { 0, 0 };
    GamePhase gamePhase = IN_PROCESS;
    Input input[2];// = { Direction::NONE }; // todo: is it should be like list or something?

    GameState(int frame) {
        this->frame = frame;
        log("    GameState created " + ToString());
    }

    GameState(GameState& state) {
        frame = state.frame;

        for (int i = 0; i < 2; i++) {
            snake_head[i] = state.snake_head[i];
            input[i] = state.input[i];
            score[i] = state.score[i];
        }

        food = state.food;
        gamePhase = state.gamePhase;

        log("    GameState copyied");
    }

    ~GameState() {

        log("    GameState start destoying " + ToString());

        if (snake_head[0] != nullptr) {
            delete snake_head[0];
            snake_head[0] = nullptr;
        }

        if (snake_head[1] != nullptr) {
            delete snake_head[1];
            snake_head[1] = nullptr;
        }
        // log("    GameState destoyed " + ToString());
    }

    std::string ToString() {
        std::string res = "State[frame:";
        res += std::to_string(frame);
        res += "];";
        if (snake_head[0] != nullptr) {
            res += " p1=";
            res += snake_head[0]->ToString();
            res += ";";
        }
        if (snake_head[1] != nullptr) {
            res += " p2=";
            res += snake_head[1]->ToString();
            res += ";";
        }
        return res;
    }
};


struct GameSettigs {
    int scoreToWin = 20;
    int initialSpeedMs = 100;
    int maxSpeedMs = 50;
    
    int leftBoundaries = 1;
    int rightBoundaries = 34;
    int topBoundaries = 2;
    int bottomBoundaries = 16;


    int startPlayedXCoord = 9;
    int startPlayedYCoord = 9;
    int startLenght = 5;
    Direction startPlayedDirection = Direction::RIGHT;

    int startFoodXCoord = 10;
    int startFoodYCoord = 14;


    int foodSize = 5;
    int snakeSize = 1;
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
