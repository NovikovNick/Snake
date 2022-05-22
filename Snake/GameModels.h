#pragma once

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
};