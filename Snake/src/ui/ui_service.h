#ifndef SNAKE_SNAKE_UI_SERVICE_H_
#define SNAKE_SNAKE_UI_SERVICE_H_
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../service/input_service.h"
#include "cursor.cc"
#include "layout_service.h"
#include "resource_manager.h"

namespace snake {

class UIService {
  std::shared_ptr<ResourceManager> resource_mng;
  std::shared_ptr<GameEventService> event_srv;
  GameSettigs stg;
  std::vector<GAME_OBJECT> game_objects;

 public:
  bool game_finished;
  bool win;
  std::shared_ptr<LayoutService> layout_srv;

  UIService(const GameSettigs& settings,
            std::shared_ptr<ResourceManager> resource_mng,
            std::shared_ptr<LayoutService> layout_srv,
            std::shared_ptr<GameEventService> event_srv)
      : stg(settings),
        resource_mng(resource_mng),
        layout_srv(layout_srv),
        event_srv(event_srv),
        game_finished(false),
        win(false) {}

  void startEventLoop() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto mode = sf::VideoMode(stg.screen_width, stg.screen_height);
    auto style = sf::Style::Default;
    sf::RenderWindow window(mode, L"Змейка v0.2", style, settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    layout_srv->initLayouts(window.getSize().x, window.getSize().y);

    // cursor
    Cursor cursor(40, sf::Color::Red);
    sf::Sound click_sound(resource_mng->GetClickSoundBuffer());
    click_sound.setVolume(30.0f);

    // event handling
    layout_srv->exit_btn->onClick = [&]() { window.close(); };
    layout_srv->exit_btn->onHover = [&](auto _) { click_sound.play(); };
    layout_srv->start_btn->onHover = [&](auto _) { click_sound.play(); };
    layout_srv->start_btn->onClick = [&]() {
      click_sound.play();
      layout_srv->onStart();
    };

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        switch (event.type) {
          case sf::Event::Closed:
            window.close();
            break;
          case sf::Event::KeyPressed:
            if (isInfoLayout()) {
              layout_srv->onRulesRead();
              event_srv->sendEvent(
                  {Direction::kNone, SystemCommand::kStartGame});
            } else {
              switch (event.key.code) {
                case sf::Keyboard::Escape:
                  window.close();
                  break;
                case sf::Keyboard::Left:
                  event_srv->sendEvent(
                      {Direction::kLeft, SystemCommand::kNone});
                  break;
                case sf::Keyboard::Right:
                  event_srv->sendEvent(
                      {Direction::kRight, SystemCommand::kNone});
                  break;
                case sf::Keyboard::Up:
                  event_srv->sendEvent({Direction::kUp, SystemCommand::kNone});
                  break;
                case sf::Keyboard::Down:
                  event_srv->sendEvent(
                      {Direction::kDown, SystemCommand::kNone});
                  break;
                case sf::Keyboard::A:
                  event_srv->sendEvent(
                      {Direction::kLeft, SystemCommand::kNone});
                  break;
                case sf::Keyboard::D:
                  event_srv->sendEvent(
                      {Direction::kRight, SystemCommand::kNone});
                  break;
                case sf::Keyboard::W:
                  event_srv->sendEvent({Direction::kUp, SystemCommand::kNone});
                  break;
                case sf::Keyboard::S:
                  event_srv->sendEvent(
                      {Direction::kDown, SystemCommand::kNone});
                  break;
              }
            }
            break;
          case sf::Event::MouseButtonPressed:
            if (isInfoLayout()) {
              layout_srv->onRulesRead();
              event_srv->sendEvent(
                  {Direction::kNone, SystemCommand::kStartGame});
            } else {
              if (event.mouseButton.button == sf::Mouse::Left) {
                auto [_, x, y] = event.mouseButton;
                for (const auto& btn : layout_srv->active_layout->buttons) {
                  if (btn->hover) btn->click();
                }
              }
            }
            break;
          case sf::Event::MouseMoved:
            auto [x, y] = event.mouseMove;
            cursor.setPosition(x, y);

            // layout_srv->map->setPosition(x, y);

            auto bounds = cursor.getBounds();
            for (const auto& btn : layout_srv->active_layout->buttons) {
              btn->setHover(btn->intersects(bounds));
            }
            break;
        }
      }

      // todo: poll game events;
      if (game_finished) {
        game_finished = false;
        if (win) {
          layout_srv->onWin();
        } else {
          layout_srv->onLose();
        }
      }

      window.clear(layout_srv->background_clr);
      window.draw(*layout_srv->active_layout);
      window.draw(cursor);
      window.display();
    }
  };

  bool isInfoLayout() {
    return layout_srv->game_rules->active || layout_srv->game_win->active ||
           layout_srv->game_lose->active;
  };
};
}  // namespace snake
#endif  // SNAKE_SNAKE_FOOD_SERVICE_H_
