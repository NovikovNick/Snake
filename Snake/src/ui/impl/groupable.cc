#ifndef SNAKE_SNAKE_UI_GROUPABLE_H_
#define SNAKE_SNAKE_UI_GROUPABLE_H_

#include <SFML/Graphics.hpp>
#include <iostream>

namespace snake {

class Groupable : public sf::Shape {
 public:
  std::shared_ptr<Groupable> parent;
  std::vector<std::shared_ptr<Groupable>> children;

  static void assign(std::shared_ptr<Groupable> parent,
                     std::shared_ptr<Groupable> child) {
    parent->children.push_back(child);
    child->parent = parent;
  }

  virtual ~Groupable() = default;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform = states.transform * getTransform();
    onDraw(target, states);
    for (const auto& child : children) child->draw(target, states);
  }

  virtual void onDraw(sf::RenderTarget& target,
                      sf::RenderStates states) const {};
};

}  // namespace snake
#endif  // SNAKE_SNAKE_UI_GROUPABLE_H_
