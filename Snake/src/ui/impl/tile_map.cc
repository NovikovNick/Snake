﻿#include <SFML/Graphics.hpp>
#include <iostream>

#include "groupable.cc"
#include "../../model/game_object.h"

namespace snake {


class TileMap : public Groupable {
  sf::VertexArray vertices_;
  sf::Texture tileset_;
  sf::Vector2u ts;
  uint32_t width, height;
  std::vector<SNAKE_INDEX> tiles;

 public:
  TileMap(const sf::Texture& tileset, const sf::Vector2u ts,
          const uint32_t width, const uint32_t height)
      : ts(ts),
        width(width),
        height(height),
        tileset_(tileset),
        tiles(std::vector<SNAKE_INDEX>(width * height, SNAKE_INDEX::EMPTY)) {
    //tileset_.setSmooth(true);

    // resize the vertex array to fit the level size
    vertices_.setPrimitiveType(sf::Quads);
    vertices_.resize(width * height * 4);
  }

  std::vector<SNAKE_INDEX>::iterator getOutputIterator() {
    return tiles.begin();
  }

  /* populate the vertex array, with one quad per tile
   */
  void update() {
    for (uint32_t i = 0; i < width; ++i) {
      for (uint32_t j = 0; j < height; ++j) {
        // get the current tile number
        int tileNumber = static_cast<int>(tiles[i + j * width]);

        // find its position in the tileset texture
        int tu = tileNumber % (tileset_.getSize().x / ts.x);
        int tv = tileNumber / (tileset_.getSize().x / ts.x);

        // get a pointer to the current tile's quad
        sf::Vertex* quad = &vertices_[(i + j * width) * 4];

        // define its 4 corners
        quad[0].position = sf::Vector2f(i * ts.x, j * ts.y);
        quad[1].position = sf::Vector2f((i + 1) * ts.x, j * ts.y);
        quad[2].position = sf::Vector2f((i + 1) * ts.x, (j + 1) * ts.y);
        quad[3].position = sf::Vector2f(i * ts.x, (j + 1) * ts.y);

        // define its 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(tu * ts.x, tv * ts.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * ts.x, tv * ts.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * ts.x, (tv + 1) * ts.y);
        quad[3].texCoords = sf::Vector2f(tu * ts.x, (tv + 1) * ts.y);

        /*quad[0].color = sf::Color::Blue;
        quad[1].color = sf::Color::Blue;
        quad[2].color = sf::Color::Blue;
        quad[3].color = sf::Color::Blue;*/
      }
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Groupable::draw(target, states);
  }

  virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &tileset_;
    target.draw(vertices_, states);
  }

  virtual std::size_t getPointCount() const { return 0; }
  virtual sf::Vector2f getPoint(std::size_t index) const { return {}; }

  virtual void draw(sf::RenderTarget& target,
                    const sf::Transform& transform) const {
    Groupable::draw(target, sf::Transform::Identity);
  }
  virtual void onDraw(sf::RenderTarget& target,
                      const sf::Transform& transform) const {}
};

};  // namespace snake
