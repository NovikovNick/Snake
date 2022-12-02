#ifndef SNAKE_SNAKE_API_H_
#define SNAKE_SNAKE_API_H_
#include <type_traits>
#include "model/game_state.h"
#include "model/grid_cell.h"

namespace snake {

template <class T>
typename std::add_lvalue_reference<T>::type makeval();

template <typename N>
concept food_srv = requires(N n) {
                     n.SetFood(makeval<GameState>());
                     n.AddFoodIfAbsent(makeval<GridCell>());
                     { n.HasFood() } -> std::same_as<bool>;
                     { n.initFood() } -> std::same_as<void>;
                   };

}  // namespace snake

#endif  // SNAKE_SNAKE_API_H_
