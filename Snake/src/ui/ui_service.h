#ifndef SNAKE_SNAKE_UI_SERVICE_H_
#define SNAKE_SNAKE_UI_SERVICE_H_
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "cursor.cc"
#include "resource_manager.h"

namespace snake {

class UIService {
  std::shared_ptr<ResourceManager> resource_mng;
  std::shared_ptr<LayoutService> layout_srv;
  int event;

 public:
  UIService(std::shared_ptr<ResourceManager> resource_mng,
            std::shared_ptr<LayoutService> layout_srv)
      : resource_mng(resource_mng), layout_srv(layout_srv) {}

  void startEventLoop() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto mode = sf::VideoMode(1100, 730);
    auto style = sf::Style::Default;
    sf::RenderWindow window(mode, L"Змейка v0.2", style, settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    layout_srv->initLayouts(window.getSize().x, window.getSize().y);

    // 3. init other elements
    // cursor
    Cursor cursor(40, sf::Color::Red);
    sf::Sound click_sound(resource_mng->GetClickSoundBuffer());
    click_sound.setVolume(30.0f);

    // 5. event handling
    layout_srv->start_btn->onHover = [&click_sound](const Button& b) {
      click_sound.play();
    };
    layout_srv->start_btn->onClick = [&]() {
      click_sound.play();
      layout_srv->onStart();
    };

    layout_srv->exit_btn->onHover = [&click_sound](const Button& b) {
      click_sound.play();
    };
    layout_srv->exit_btn->onClick = [&window]() { window.close(); };

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        switch (event.type) {
          case sf::Event::Closed:
            window.close();
            break;
          case sf::Event::KeyPressed:
            switch (event.key.code) {
              case sf::Keyboard::Escape:
                window.close();
                break;
            }
            break;
          case sf::Event::MouseButtonPressed:

              if (layout_srv->game_rules->active ||
                layout_srv->game_win->active || layout_srv->game_lose->active) {
              layout_srv->onRulesRead();
            } else {
              if (event.mouseButton.button == sf::Mouse::Left) {
                auto [_, x, y] = event.mouseButton;
                for (const auto& btn : layout_srv->active_layout->buttons) {
                  if (btn->hover) btn->click();
                }
                debug(".setPosition({}, {});\n", x, y);
              }
            }

             if (event.mouseButton.button == sf::Mouse::Left) {
              click_sound.play();
            }
            break;
          case sf::Event::MouseMoved:
            auto [x, y] = event.mouseMove;
            cursor.setPosition(x, y);
            auto bounds = cursor.getBounds();
            for (const auto& btn : layout_srv->active_layout->buttons) {
              btn->setHover(btn->intersects(bounds));
            }
            break;
        }
      }

      window.clear(layout_srv->background_clr);
      window.draw(*layout_srv->active_layout);
      window.draw(cursor);
      window.display();
    }
  };

  void addEvent(int game_event) { int event = game_event; };

  int pollEvent() {
    int res = event;
    event = -1;
    return res;
  };
};
}  // namespace snake
#endif  // SNAKE_SNAKE_FOOD_SERVICE_H_
