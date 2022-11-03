#ifndef SNAKE_SNAKE_GAME_UTIL_H_
#define SNAKE_SNAKE_GAME_UTIL_H_

#include <format>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

namespace snake {

template <typename... Args>
void debug(const std::string_view& str, Args&&... args) {
#if SNAKE_DEBUG
  std::cout << std::vformat(str, std::make_format_args(args...));
#endif
}

}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_UTIL_H_
