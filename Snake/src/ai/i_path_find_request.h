#ifndef SNAKE_SNAKE_I_PATH_FIND_REQUEST_H_
#define SNAKE_SNAKE_I_PATH_FIND_REQUEST_H_

#include <vector>

#include "../model/common_models.h"
#include "../service/game_models.h"

namespace snake {

struct IPathFindRequest {
 public:
  virtual const Coord& getFrom() const = 0;
  virtual const Coord& getTo() const = 0;
  virtual const std::vector<Coord>& getBarriers() const = 0;
  virtual ~IPathFindRequest() = default;
};

}  // namespace snake
#endif  // #define SNAKE_SNAKE_I_PATH_FIND_REQUEST_H_
