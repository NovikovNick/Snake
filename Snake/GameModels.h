#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_
#include "util/log.h"

namespace snake {

enum GamePhase {
    IN_PROCESS, WIN, LOSE, PAUSED
};

enum Direction {
    UP, DOWN, LEFT, RIGHT, NONE
};

enum SystemCommand {
    PAUSE, STEP_FORWARD, STEP_BACKWARD, NON
};

struct Input {
    Direction direction = Direction::NONE;
    SystemCommand command = SystemCommand::NON;
};

struct Coord {
   int x;
   int y;
};


struct SnakePart {
    Coord coord;
    Direction direction;
    SnakePart* next = nullptr;

    SnakePart() {
        log("        SnakePart created");
        next = nullptr;
    }

    SnakePart(SnakePart& src) {
        coord = src.coord;
        direction = src.direction;
        next = src.next;
        log("        SnakePart copied");
    }

    ~SnakePart() {
        log("        SnakePart start destoying");
        SnakePart* current = next;
        if (current != nullptr) {
            delete current;
        }
        log("        SnakePart destoyed");
    }
};

struct Food {
    Coord coord;
};

struct GameState {
    int frame;
    SnakePart* snake_head;
    Coord food;
    int score = 0;
    GamePhase gamePhase = IN_PROCESS;
    Input input = { NONE }; // ???

    GameState() {
        log("    GameState created");
    }

    GameState(GameState& state) {
        frame = state.frame;
        snake_head = state.snake_head;
        food = state.food;
        score = state.score;
        gamePhase = state.gamePhase;
        input = state.input;
        log("    GameState copyied");
    }

    ~GameState() {

        log("    GameState start destoying");
        if (snake_head != nullptr) {
            delete snake_head;
        }
        log("    GameState destoyed");

    }
};


struct GameSettigs {
    int scoreToWin = 50;
    int initialSpeedMs = 50;
    int maxSpeedMs = 50;
    
    int leftBoundaries = 1;
    int rightBoundaries = 33;
    int topBoundaries = 2;
    int bottomBoundaries = 15;


    int startPlayedXCoord = 9;
    int startPlayedYCoord = 9;
    int startLenght = 5;
    Direction startPlayedDirection = RIGHT;

    int startFoodXCoord = 16;
    int startFoodYCoord = 10;


    int foodSize = 5;
    int snakeSize = 1;
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
