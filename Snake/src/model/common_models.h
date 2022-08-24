#ifndef SNAKE_SNAKE_GAME_COMMON_MODEL_H_
#define SNAKE_SNAKE_GAME_COMMON_MODEL_H_

#ifndef SNAKE_DEBUG
#define SNAKE_DEBUG 0
#endif

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

#include "../service/game_models.h"

namespace snake {

class Snake final {
  std::vector<std::pair<Coord, Direction>> list_;
  Coord leftTop_;
  Coord rightBottom_;
  std::unordered_map<Coord, Direction, hash_coord> map_;
  bool validState_ = true;

 public:
  using SnakePart = std::pair<Coord, Direction>;

  Snake() = default;

  Snake(std::vector<SnakePart> list)
      : list_(list), leftTop_(list_[0].first), rightBottom_(list_[0].first) {
    for (const SnakePart& item : list_) {
      leftTop_.x = leftTop_.x < item.first.x ? leftTop_.x : item.first.x;
      leftTop_.y = leftTop_.y < item.first.y ? leftTop_.y : item.first.y;

      rightBottom_.x =
          rightBottom_.x > item.first.x ? rightBottom_.x : item.first.x;
      rightBottom_.y =
          rightBottom_.y > item.first.y ? rightBottom_.y : item.first.y;

      if (validState_) {
        validState_ = map_.find(item.first) == map_.end();
      }

      map_[item.first] = item.second;
    }
#if SNAKE_DEBUG
    std::cout << "  snake ctor " << this << std::endl;
#endif
  }

  ~Snake() {
#if SNAKE_DEBUG
    std::cout << "  snake dtor " << this << std::endl;
#endif
  }

  bool isInBound(const Coord& leftTop, const Coord& rightBottom) const {
    return leftTop.x < leftTop_.x && leftTop.y < leftTop_.y &&
           rightBottom.x > rightBottom_.x && rightBottom.y > rightBottom_.y;
  }

  bool isCollide(const Coord& coord) const {
    return map_.find(coord) != map_.end();
  }

  const Coord& getHeadCoord() const { return getParts()[0].first; }

  void gain() {
    std::pair<Coord, Direction> last = getParts().back();
    list_.push_back({last.first - last.second, last.second});
  }

  int getSize() const noexcept { return list_.size(); };

  int isValid() const noexcept { return validState_; };

  const std::vector<std::pair<Coord, Direction>>& getParts() const {
    return list_;
  };
};

struct GameState final {
 private:
  int frame_ = 0;
  GamePhase gamePhase_ = GamePhase::kInProcess;

  std::vector<Snake> players_{2};
  std::vector<Input> input_{2};
  std::vector<int> scores_ = {0, 0};
  Coord food_ = {0, 0};

  DebugContext ctx_;

 public:
  GameState(const int& frame, const Snake& player, const Snake& enemy)
      : frame_(frame) {
    players_[0] = player;
    players_[1] = enemy;

#if SNAKE_DEBUG
    std::cout << "state ctor " << this << std::endl;
#endif
  }

  GameState(const GameState& rhs)
      : GameState(rhs.frame_, rhs.players_[0], rhs.players_[1]) {
    food_ = rhs.food_;
    scores_ = rhs.scores_;
    input_ = rhs.input_;
#if SNAKE_DEBUG
    std::cout << "state copy " << this << std::endl;
#endif
  }

  GameState(GameState&& rhs) noexcept {
    std::swap(frame_, rhs.frame_);
    std::swap(players_, rhs.players_);
    std::swap(input_, rhs.input_);
    std::swap(scores_, rhs.scores_);
    std::swap(food_, rhs.food_);
    std::swap(ctx_, rhs.ctx_);

#if SNAKE_DEBUG
    std::cout << "state move " << this << std::endl;
#endif
  }

  ~GameState() {
#if SNAKE_DEBUG
    std::cout << "state dtor " << this << std::endl;
#endif
  }

  const Snake& getPlayer(const int& index) const { return players_[index]; }

  const Coord& getFood() const { return food_; }
  const GamePhase& getPhase() const { return gamePhase_; }
  const int& getScore(const int& index) const { return scores_[index]; }
  const DebugContext& getDebugContext() const { return ctx_; }
  const std::vector<Input>& GetInputs() const { return input_; }
  int getSize() const { return static_cast<int>(players_.size()); }
  int getFrame() const { return frame_; }

  void setPlayer(const Snake& player, const int& index) {
    players_[index] = player;
  }
  void setFood(const Coord& food) { food_ = food; }
  void setPhase(const GamePhase& gamePhase) { gamePhase_ = gamePhase; }
  void setDebugContext(const DebugContext& ctx) { ctx_ = ctx; }
  void setInputs(const std::vector<Input>& inputs) { input_ = inputs; }
  void setScore(const int& index, int score) { scores_[index] = score; }
  void setFrame(const int& frame) { frame_ = frame; }
};

}  // namespace snake

#endif  // !SNAKE_SNAKE_GAME_COMMON_MODEL_H_
