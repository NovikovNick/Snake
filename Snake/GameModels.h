#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_
#include "util/log.h"

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
    SnakePart* snake_head[2];
    Coord food;
    int score = 0;
    GamePhase gamePhase = IN_PROCESS;
    Input input = { Direction::NONE }; // todo: is it should be like list or something?

    GameState() {
        snake_head[0] = nullptr;
        snake_head[1] = nullptr;
        log("    GameState created");
    }

    GameState(GameState& state) {
        frame = state.frame;

        for (int i = 0; i < 2; i++) {
            snake_head[i] = state.snake_head[i];
        }

        food = state.food;
        score = state.score;
        gamePhase = state.gamePhase;
        input = state.input;
        log("    GameState copyied");
    }

    ~GameState() {

        log("    GameState start destoying");


        // todo: it can be simplefyed
        for (int i = 0; i < 2; i++) {
            if (snake_head[i] != nullptr) {
                delete snake_head[i];
            }
        }
        
        log("    GameState destoyed");

    }
};


struct GameSettigs {
    int scoreToWin = 50;
    int initialSpeedMs = 100;
    int maxSpeedMs = 100;
    
    int leftBoundaries = 1;
    int rightBoundaries = 33;
    int topBoundaries = 2;
    int bottomBoundaries = 15;


    int startPlayedXCoord = 9;
    int startPlayedYCoord = 9;
    int startLenght = 5;
    Direction startPlayedDirection = Direction::RIGHT;

    int startFoodXCoord = 16;
    int startFoodYCoord = 10;


    int foodSize = 5;
    int snakeSize = 1;
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
