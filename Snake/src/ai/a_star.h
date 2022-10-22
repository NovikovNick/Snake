#ifndef SNAKE_A_STAR_H_
#define SNAKE_A_STAR_H_

namespace snake {

//template <typename N>
//concept node_concept = std::is_class_v<N>;


/*
&& requires(N n) {
  n.GetX();
};*/

struct TestNode {
 private:
  int x_, y_;

 public:
  TestNode(int x, int y) : x_(x), y_(y){};
  int GetX() const { return x_; };
  int GetY() const { return y_; };
};

class AStarPathfinder final {
 public:
  void FindPath(TestNode start);
};

}  // namespace snake

#endif  // SNAKE_A_STAR_H_
