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

struct Food {
    Coord coord;
};


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

    long foodGenerationSeed = 1658772641;
};


class Snake {

    std::vector<std::pair<Coord, Direction>> list_;
    Coord leftTop_;
    Coord rightBottom_;
    std::unordered_map<Coord, Direction, hash_coord> map_;
    bool validState_ = true;

public:
    Snake(std::vector<std::pair<Coord, Direction>> list) 
        : list_(list), 
        leftTop_(list_[0].first), 
        rightBottom_(list_[0].first)  {

        for (auto const& item : list_) {

            leftTop_.x = leftTop_.x < item.first.x ? leftTop_.x : item.first.x;
            leftTop_.y = leftTop_.y < item.first.y ? leftTop_.y : item.first.y;

            rightBottom_.x = rightBottom_.x > item.first.x ? rightBottom_.x : item.first.x;
            rightBottom_.y = rightBottom_.y > item.first.y ? rightBottom_.y : item.first.y;

            if (validState_) {
                validState_ = map_.find(item.first) == map_.end();
            }

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

    bool isValid() const noexcept { return validState_; };

    const std::vector<std::pair<Coord, Direction>>& getParts() const { return list_; };

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
    const std::vector<Input>& getInputs() const noexcept { return input_; }
    const int& getSize() const noexcept { return players_.size(); }

    void setFood(const Coord& food) noexcept { food_ = food; }
    void setPhase(const GamePhase& gamePhase) noexcept { gamePhase_ = gamePhase; }
    void setDebugContext(const DebugContext& ctx) noexcept { ctx_ = ctx; }
    void setInputs(const std::vector<Input>& inputs) noexcept { input_ = inputs; }
    void setScore(const int& index, int score) noexcept { scores_[index] = score; }

private:
    int frame_ = 0;
    GamePhase gamePhase_ = IN_PROCESS;

    std::vector<std::unique_ptr<Snake>> players_;
    std::vector<Input> input_ = { Input{}, Input{} };
    std::vector<int> scores_ = {0, 0};
    Coord food_ = { 0, 0 };

    DebugContext ctx_;
};

template <typename T>
class GameStateBuffer {

    int cursor_ = -1;
    int size_;
    bool overlaped = false;
    std::vector<std::unique_ptr<T>> data_;

public:
    GameStateBuffer(int size) : size_(size) {
        data_.resize(size);
    }

    void add(T* item) noexcept {
        cursor_ = (cursor_ + 1) % size_;
        data_[cursor_] = std::unique_ptr<T>(item);

        if (!overlaped && (cursor_ + 1) == size_) {
            overlaped = true;
        }
    }

    const T& head() const noexcept {
        return *(data_[cursor_].get());
    }

    const T& operator[](const int& index) const noexcept {

        int offset = (cursor_ - index) % size_;
        return *(data_[offset < 0 ? size_ + offset : offset].get());
    }

    int getSize() const noexcept {
        return overlaped ? data_.size() : cursor_ + 1;
    }
};

} // namespace snake

#endif // !SNAKE_SNAKE_GAME_MODEL_H_
