#ifndef SNAKE_A_STAR_H_
#define SNAKE_A_STAR_H_

#include "../model/common_models.h"
#include "../service/game_models.h"
#include "i_path_find_request.h"

namespace snake {

// is default constructor required?
// if set fields as const then need to define operator=?
//  operator==
template <typename N>
concept grid_2d_cell = requires(N n) {
  n.GetX();
  n.GetY();
  n == n;
};

template <typename G, typename N>
concept grid_2d = requires(G g) {
  g.FindAdjacents(std::int32_t(), std::int32_t(), std::vector<N>().begin());
};
template <grid_2d_cell N, grid_2d<N> G, std::output_iterator<N> I>
class AStarPathfinder final {
 public:
  void FindPath(const N& start, const N& goal, const G& grid, I out,
                I sentinel);
};
}  // namespace snake
#endif  // SNAKE_A_STAR_H_
