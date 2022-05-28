#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_


namespace snake {

enum GamePhase {
    IN_PROCESS, WIN, LOSE
};

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

struct Input {
    Direction direction;
};

struct Coord {
   int x;
   int y;
};


struct SnakePart {
    Coord coord;
    Direction direction;
    
    SnakePart *next;
};

struct Food {
    Coord coord;
};

struct GameState {
    SnakePart* snake_head;
    Coord food;
    int score = 0;
    GamePhase gamePhase = IN_PROCESS;
};


struct GameSettigs {
    int scoreToWin = 20;
    int initialSpeedMs = 400;
    int maxSpeedMs = 200;
    
    int leftBoundaries = 5;
    int rightBoundaries = 30;
    int topBoundaries = 2;
    int bottomBoundaries = 16;


    int startPlayedXCoord = 9;
    int startPlayedYCoord = 9;
    int startLenght = 3;
    Direction startPlayedDirection = RIGHT;

    int startFoodXCoord = 16;
    int startFoodYCoord = 10;


    int foodSize = 5;
    int snakeSize = 1;
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
