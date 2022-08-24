#define BOOST_TEST_MODULE SolutionTest
#define SNAKE_DEBUG 1

#include <boost/test/included/unit_test.hpp>

#include "../src/model/common_models.h"
#include "../src/service/ai_service.h"
#include "../src/service/game_models.h"
#include "../src/service/game_state_service.h"

namespace {
struct Controllable {
  // static int control;
  int* resource_;
  Controllable() : resource_(new int(42)) { std::cout << "ctor\n"; }

  Controllable(Controllable&& rhs) noexcept : resource_(rhs.resource_) {
    std::cout << "move ctor\n";
    rhs.resource_ = nullptr;
  }
  Controllable& operator=(Controllable&& rhs) noexcept {
    std::swap(resource_, rhs.resource_);
    std::cout << "move assign\n";
    return *this;
  }
  Controllable(const Controllable& rhs) : resource_(new int(*rhs.resource_)) {
    std::cout << "copy ctor\n";
#if 0
    if (control == 0) {
      control = 5;
      std::cout << "Control reached\n";
      throw std::bad_alloc{};
    }
    control -= 1;
#endif
  }
  Controllable& operator=(const Controllable& rhs) {
    Controllable tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

  ~Controllable() {
    std::cout << "dtor \n";
    delete resource_;
  }
};
struct Dummy {
  Dummy(int id) : id_(id) {
    std::cout << "ctor " << std::to_string(id_) << " : " << this << std::endl;
  }
  Dummy(const Dummy& src) : id_(src.id_) {
    std::cout << "copy" << std::to_string(id_) << " : " << this << std::endl;
  }
  Dummy(Dummy&&) noexcept { std::cout << "move " << this << std::endl; }
  ~Dummy() noexcept {
    if (std::uncaught_exceptions()) {
      std::cerr << "dtor called on unwinding" << std::endl;
    }
    std::cout << "dtor " << std::endl;
  }

  int getId() const { return id_; };

 private:
  int id_;
};
}  // namespace

namespace snake {

#if 0

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
#if SNAKE_SNAKE_GAME_MODEL_DEBUG
    std::cout << "  snake ctor " << this << std::endl;
#endif
  }

  ~Snake() {
#if SNAKE_SNAKE_GAME_MODEL_DEBUG
    std::cout << "  snake dtor " << this << std::endl;
#endif
  }

  void gain();

  bool isInBound(const Coord& leftTop, const Coord& rightBottom) const;

  const Coord& getHeadCoord() const;

  bool isCollide(const Coord& coord) const;

  bool isValid() const noexcept { return validState_; };

  int getSize() const noexcept { return list_.size(); };

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
  std::vector<int> scores_{2};
  Coord food_ = {0, 0};

  DebugContext ctx_;

 public:
  GameState(const int& frame, const Snake& player, const Snake& enemy)
      : frame_(frame) {
    players_[0] = player;
    players_[1] = enemy;

#if SNAKE_SNAKE_GAME_MODEL_DEBUG
    std::cout << "state ctor " << this << std::endl;
#endif
  }

  GameState(const GameState& rhs)
      : GameState(rhs.frame_, rhs.players_[0], rhs.players_[1]) {
#if SNAKE_SNAKE_GAME_MODEL_DEBUG
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

#if SNAKE_SNAKE_GAME_MODEL_DEBUG
    std::cout << "state move " << this << std::endl;
#endif
  }

  ~GameState() {
#if SNAKE_SNAKE_GAME_MODEL_DEBUG
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

  void setFood(const Coord& food) { food_ = food; }
  void setPhase(const GamePhase& gamePhase) { gamePhase_ = gamePhase; }
  void setDebugContext(const DebugContext& ctx) { ctx_ = ctx; }
  void setInputs(const std::vector<Input>& inputs) { input_ = inputs; }
  void setScore(const int& index, int score) { scores_[index] = score; }
  void setFrame(const int& frame) { frame_ = frame; }
};

#endif

void render(GameStateBuffer<GameState>& buf) {
#if SNAKE_DEBUG
  std::cout << "=== render. No copy! === \n";

  const GameState& state = buf.head();
  for (int i = 0, sz = buf.getSize(); i < sz; ++i) {
    std::cout << "frame[" << buf[i].getFrame() << "]:... \n";
  }
#endif
}

class PathFindRequestDTO : public IPathFindRequest {
 private:
  Coord from_;
  Coord to_;
  std::vector<Coord> barriers_;

 public:
  PathFindRequestDTO(const GameState& rhs, const int& playerIndex)
      : from_(rhs.getPlayer(playerIndex).getHeadCoord()), to_(rhs.getFood()) {
    for (size_t i = 0; i < 2; i++) {
      const auto& player = rhs.getPlayer(playerIndex);

    }
  }

  virtual const Coord& getFrom() const override { return from_; }
  virtual const Coord& getTo() const override { return to_; }
  virtual const std::vector<Coord>& getBarriers() const { return barriers_; };
};  // namespace snake

BOOST_AUTO_TEST_CASE(sampleTest) {
  // arrange
  auto snake_service = std::make_shared<SnakeService>();
  auto game_state_service = std::make_shared<GameStateService>(snake_service);
  auto ai_service = std::make_shared<AIService>();

  GameStateBuffer<GameState> buf(2);
  GameSettigs setting{};

  // SnakeService service{};
  auto begin = clock();
  GameState initialState = game_state_service->initState(setting);
#if SNAKE_DEBUG
  std::cout << "-- add\n";
#endif
  buf.add(initialState);

  for (int i = 0; i < 3; ++i) {
#if SNAKE_DEBUG
    std::cout << "-- frame -- \n";
#endif

    const auto& prevState = buf.head();
    const IPathFindRequest& req = PathFindRequestDTO(prevState, 1);

    ai_service->findPath(req);

#if SNAKE_DEBUG
    std::cout << "-- get head -- \n";
#endif

    GameState state = game_state_service->applyForces(
        prevState, {Direction::kRight, Direction::kRight});

#if SNAKE_DEBUG
    std::cout << "-- add \n";
#endif
    buf.add(state);
  }

  std::cout << "Elapsed: " << clock() - begin << " ms\n";

  render(buf);
}

}  // namespace snake
