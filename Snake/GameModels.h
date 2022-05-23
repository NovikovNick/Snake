#pragma once

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
    int scoreToWin = 10;
    int initialSpeedMs = 300;
    int maxSpeedMs = 100;
    
    int leftBoundaries = 5;
    int rightBoundaries = 30;
    int topBoundaries = 2;
    int bottomBoundaries = 16;


    int startPlayedXCoord = 9;
    int startPlayedYCoord = 9;
    int startLenght = 3;
    Direction startPlayedDirection = RIGHT;

    int startFoodXCoord = 16;
    int startFoodYCoord = 6;


    int foodSize = 5;
    int snakeSize = 1;
};
