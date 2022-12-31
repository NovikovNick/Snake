#define BOOST_TEST_MODULE SolutionTest
#define CASE_2 1
#define CASE_1 0
#define SNAKE_DEBUG 1

#define CHECK_EQUAL(L, R, M) \
  {                          \
    BOOST_TEST_INFO(M);      \
    BOOST_CHECK_EQUAL(L, R); \
  }

#include <boost/test/included/unit_test.hpp>
#include <random>

#include "../src/model/game_state.h"
#include "../src/model/grid.h"
#include "../src/util.h"

namespace snake {

#if CASE_1
BOOST_AUTO_TEST_CASE(case1) {
  // arrange
  int count = 2;
  std::vector<SNAKE_DATA> snakes;
  for (int i = 0; i < count; ++i)
    snakes.push_back(SNAKE_DATA{{i, 2, Direction::kDown},
                                {i, 1, Direction::kDown},
                                {i, 0, Direction::kDown}});

  std::vector<GridCell> src_foods{GridCell{4, 2}, GridCell{2, 4}};
  GameState src(7, 2, Grid2d(7, 7, 2), src_foods);

  src.grid.food = GridCell{12, 21};
  src.grid.snake_length_[0] = 3;
  src.grid.snake_length_[1] = 3;

  for (int i = 0; i < count; ++i)
    src.grid.AddSnake(i, snakes[i].begin(), snakes[i].end());

  src.grid.RebuildFilled(0);
  src.grid.RebuildFilled(1);

  src.score[0] = 11;
  src.score[1] = 22;
  src.inputs[0] = Direction::kLeft;
  src.inputs[1] = Direction::kNone;
  src.collision[1] = true;

  unsigned char *buf = new unsigned char[1024];

  // act
  GameState::serialize(src, &buf);
  auto dst = GameState::deserialize(buf);

  // assert
  CHECK_EQUAL(src.frame, dst.frame, "frame");
  CHECK_EQUAL(src.snake_count, dst.snake_count, "snake_count");

  CHECK_EQUAL(src.grid.food.GetX(), dst.grid.food.GetX(), "grid.food x");
  CHECK_EQUAL(src.grid.food.GetY(), dst.grid.food.GetY(), "grid.food y");

  for (int i = 0; i < src.foods.size(); ++i) {
    CHECK_EQUAL(src.foods[i].GetX(), dst.foods[i].GetX(), "food x " + i);
    CHECK_EQUAL(src.foods[i].GetY(), dst.foods[i].GetY(), "food y " + i);
  }
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < src.grid.snakes_[i].size(); ++j) {
      auto [src_x, src_y, src_dir] = src.grid.snakes_[i][j];
      auto [dst_x, dst_y, dst_dir] = dst.grid.snakes_[i][j];
      CHECK_EQUAL(static_cast<int>(src_x), static_cast<int>(dst_x),
                  "snake x " + j);
      CHECK_EQUAL(static_cast<int>(src_y), static_cast<int>(dst_y),
                  "snake y " + j);
      CHECK_EQUAL(static_cast<int>(src_dir), static_cast<int>(dst_dir),
                  "snake dir " + j);
    }

    CHECK_EQUAL(src.score[i], dst.score[i], "score " + i);
    CHECK_EQUAL(static_cast<int>(src.inputs[i]),
                static_cast<int>(dst.inputs[i]), "input " + i);
    CHECK_EQUAL(src.collision[i], dst.collision[i], "collision " + i);
    CHECK_EQUAL(src.grid.snake_length_[i], dst.grid.snake_length_[i],
                "grid snake length " + i);
  }

  CHECK_EQUAL(src.grid.width_, dst.grid.width_, "src.grid.width_");
  CHECK_EQUAL(src.grid.height_, dst.grid.height_, "src.grid.height_");

  // clean
  delete[] buf;
}
#endif

#if CASE_2
BOOST_AUTO_TEST_CASE(case2) {
  // arrange
  std::vector<std::pair<int, int>> matrix;
  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j) matrix.push_back(std::make_pair(i, j));
  debug("size = {}\n", matrix.size());
  matrix.erase(matrix.begin());
  debug("size = {}\n", matrix.size());
  for (auto [x, y] : matrix) debug("[{}, {}], ", x, y);
}
#endif
}  // namespace snake
