#ifndef SNAKE_SNAKE_GAME_MODEL_H_
#define SNAKE_SNAKE_GAME_MODEL_H_

#include <vector>

#include "../util/log.h"
#include <iostream>
#include <list>
#include <unordered_map>

namespace snake {

enum GamePhase {
    IN_PROCESS, WIN, LOSE, PAUSED
};

enum class Direction {
    UP, DOWN, LEFT, RIGHT, NONE,
};

enum class SystemCommand {
    PAUSE,
    STEP_FORWARD, STEP_BACKWARD,
    AI_STEP_FORWARD, AI_STEP_BACKWARD,
    NONE
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

   Coord operator - (const Direction& dir) const {
       switch (dir) {
       case Direction::LEFT:
           return { x + 1, y };
       case Direction::RIGHT:
           return { x - 1, y };
       case Direction::UP:
           return { x, y + 1 };
       case Direction::DOWN:
           return { x, y - 1 };
       default:
           return { x, y };
       }
   }

   bool operator == (const Coord& other) const {
       return x == other.x && y == other.y;
   }
};

struct hash_coord {
    size_t operator()(const snake::Coord& p) const
    {
        auto hash1 = std::hash<int>{}(p.x);
        auto hash2 = std::hash<int>{}(p.y);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

enum class DebugMark {
    REACHABLE, EXPLORED, PATH, NONE
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

struct Path {
    std::vector<Direction> path;
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

//struct GameState {
//    int frame = 0;
//    std::vector<SnakePart*> snake_head = { nullptr, nullptr };
//
//    Coord food;
//    int score[2] = { 0, 0 };
//    GamePhase gamePhase = IN_PROCESS;
//    Input input[2];// = { Direction::NONE }; // todo: is it should be like list or something?
//
//    GameState(int frame) {
//        this->frame = frame;
//        log("    GameState created " + ToString());
//    }
//
//    GameState(GameState& state) {
//        frame = state.frame;
//
//        for (int i = 0; i < 2; i++) {
//            snake_head[i] = state.snake_head[i];
//            input[i] = state.input[i];
//            score[i] = state.score[i];
//        }
//
//        food = state.food;
//        gamePhase = state.gamePhase;
//
//        log("    GameState copyied");
//    }
//
//    ~GameState() {
//
//        log("    GameState start destoying " + ToString());
//
//        if (snake_head[0] != nullptr) {
//            delete snake_head[0];
//            snake_head[0] = nullptr;
//        }
//
//        if (snake_head[1] != nullptr) {
//            delete snake_head[1];
//            snake_head[1] = nullptr;
//        }
//        // log("    GameState destoyed " + ToString());
//    }
//
//    std::string ToString() {
//        std::string res = "State[frame:";
//        res += std::to_string(frame);
//        res += "];";
//        if (snake_head[0] != nullptr) {
//            res += " p1=";
//            res += snake_head[0]->ToString();
//            res += ";";
//        }
//        if (snake_head[1] != nullptr) {
//            res += " p2=";
//            res += snake_head[1]->ToString();
//            res += ";";
//        }
//        return res;
//    }
//};



struct GameSettigs {
    int scoreToWin = 50;
    int initialSpeedMs = 100;
    int maxSpeedMs = 100;
    
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


struct Snake {

    Snake(std::vector<std::pair<Coord, Direction>> list) 
        : list_(list), 
        leftTop_(list_[0].first), 
        rightBottom_(list_[0].first)  {

        for (auto const& item : list_) {

            leftTop_.x = leftTop_.x < item.first.x ? leftTop_.x : item.first.x;
            leftTop_.y = leftTop_.y < item.first.y ? leftTop_.y : item.first.y;

            rightBottom_.x = rightBottom_.x > item.first.x ? rightBottom_.x : item.first.x;
            rightBottom_.y = rightBottom_.y > item.first.y ? rightBottom_.y : item.first.y;

            selfCollision_ = selfCollision_ || map_.find(item.first) == map_.end();

            map_[item.first] = item.second;
        }
        std::cout << "Snake direct constructor " << this << std::endl; 
    }
    Snake(Snake const& src) : Snake(src.list_) { std::cout << "copy constructor " << this << std::endl; }
    Snake(Snake&&) noexcept { std::cout << "move constructor " << this << std::endl;}
    ~Snake() noexcept { std::cout << "destructor " << " : " << this << std::endl; }

    Snake* move(const Direction& dir = Direction::NONE, 
                const bool& gain = false) const noexcept;

    void gain() noexcept;

    bool isInBound(const Coord& leftTop, const Coord& rightBottom) const noexcept;

    const Coord& getHeadCoord() const noexcept;

    bool isCollide(const Coord& coord) const noexcept;

    bool isSelfCollide() const noexcept { return selfCollision_; };

    const std::vector<std::pair<Coord, Direction>>& getParts() const { return list_; };

private:
    std::vector<std::pair<Coord, Direction>> list_;
    Coord leftTop_;
    Coord rightBottom_;
    std::unordered_map<Coord, Direction, hash_coord> map_;
    bool selfCollision_ = false;
};

struct GameState {
    
    GameState(
        const int& frame, 
        Snake* player, 
        Snake* enemy) : frame_(frame) {
    
        players_.resize(2);
        players_[0] = std::unique_ptr<Snake>(player);
        players_[1] = std::unique_ptr<Snake>(enemy);
    }

    const Snake& getPlayer(const int& index) const noexcept { return *(players_[index].get()); }

    const Coord& getFood() const noexcept { return food_; }
    const GamePhase& getPhase() const noexcept { return gamePhase_; }
    const int& getScore(const int& index) const noexcept { return scores_[index]; }
    const DebugContext& getDebugContext() const noexcept { return ctx_; }

    void setFood(const Coord& food) noexcept { food_ = food; }
    void setPhase(const GamePhase& gamePhase) noexcept { gamePhase_ = gamePhase; }
    void setDebugContext(const DebugContext& ctx) noexcept { ctx_ = ctx; }

private:
    int frame_ = 0;
    GamePhase gamePhase_ = IN_PROCESS;

    std::vector<std::unique_ptr<Snake>> players_;
    std::vector<Input> input_;
    std::vector<int> scores_ = {0, 0};
    Coord food_;

    DebugContext ctx_;
};

template <typename T>
struct GameStateBuffer {

    GameStateBuffer(int size) : size_(size) {
        data_.resize(size);
    }

    void add(T* item) noexcept {
        cursor_ = (cursor_ + 1) % size_;
        data_[cursor_] = std::unique_ptr<T>(item);
    }

    const T& head() const noexcept {
        return *(data_[cursor_].get());
    }

    const T& operator[](const int& index) const noexcept {
        return *(data_[index].get());
    }

    int getSize() const noexcept {
        return cursor_ < size_ ? cursor_ + 1: size_;
    }

private:
    int cursor_ = -1;
    int size_;
    std::vector<std::unique_ptr<T>> data_;
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
