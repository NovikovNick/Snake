#ifndef SNAKE_UI_LAYOUT_SERVER_H_
#define SNAKE_UI_LAYOUT_SERVER_H_

#include <SFML/Graphics.hpp>
#include <iostream>

#include "impl/button.cc"
#include "impl/grid.cc"
#include "impl/groupable.cc"
#include "impl/image.cc"
#include "impl/label.cc"
#include "impl/layout.cc"
#include "impl/tile_map.cc"
#include "resource_manager.h"

namespace {
struct I18n {
  const wchar_t* title{L"Змейка"};
  const wchar_t* version{L"v0.2.0"};
  const wchar_t* start_btn_lbl{L"Начать игру"};
  const wchar_t* exit_btn_lbl{L"Выход"};
  const wchar_t* score_lbl{L" / "};
  const wchar_t* control_lbl{L"Управление на стрелочки"};
  const wchar_t* press_any_btn_lbl{L"Нажмите на любую клавишу, чтобы начать"};
  const wchar_t* win_lbl{L"Победа!"};
  const wchar_t* lose_lbl{L"Проигрыш..."};
};
}  // namespace

namespace snake {

class LayoutService {
  I18n i18n;
  int width, height;
  GameSettigs settings;
  std::shared_ptr<ResourceManager> resource_mng;

 public:
  sf::Color key_clr, background_clr, txt_clr;
  std::shared_ptr<Layout> menu, game_layout, game_rules, game_lose, game_win;
  std::shared_ptr<Button> start_btn, exit_btn;
  std::shared_ptr<Layout> active_layout;
  std::vector<SNAKE_INDEX> level;
  std::shared_ptr<TileMap> map;
  std::shared_ptr<Label> game_score_lbl;

  LayoutService(const GameSettigs& settings,
                std::shared_ptr<ResourceManager> resource_mng)
      : key_clr(sf::Color(255, 100, 0)),
        background_clr(sf::Color(35, 35, 35)),
        txt_clr(sf::Color::White),
        settings(settings),
        width(settings.screen_width),
        height(settings.screen_height),
        i18n({}),
        resource_mng(resource_mng),
        active_layout(nullptr),
        level(std::vector<SNAKE_INDEX>(settings.width * settings.height,
                                       SNAKE_INDEX::EMPTY)){};

  void initLayouts(const int screen_width, const int screen_height) {
    initMenu(screen_width, screen_height, resource_mng);
    initGameLayout(screen_width, screen_height, resource_mng);
    initGameRulesLayout(screen_width, screen_height, resource_mng);
    initGameWinLayout(screen_width, screen_height, resource_mng);
    initGameLoseLayout(screen_width, screen_height, resource_mng);

    active_layout = menu;
    active_layout->active = true;
  }

  void onStart() { changeLayout(game_rules); }
  void onRulesRead() { changeLayout(game_layout); }
  void onWin() { changeLayout(game_win); }
  void onLose() { changeLayout(game_lose); }

  void update(const int score) {
    game_score_lbl->label.setString(std::to_string(score) + " / " +
                                    std::to_string(settings.winScore));
    map->update();
  }

 private:
  void initMenu(const int screen_width, const int screen_height,
                std::shared_ptr<ResourceManager> resource_mng) {
    auto version_lbl = std::make_shared<Label>(
        sf::Text(i18n.version, resource_mng->GetDefaultFont(), 25));
    version_lbl->setPosition(236, 180);
    version_lbl->label.setFillColor(txt_clr);

    auto game_title_lbl = std::make_shared<Label>(
        sf::Text(i18n.title, resource_mng->GetTitleFont(), 150));
    game_title_lbl->label.setFillColor(key_clr);
    game_title_lbl->setPosition(550, 230);
    Groupable::assign(game_title_lbl, version_lbl);

    start_btn = std::make_shared<Button>(
        400, 105,
        sf::Text(i18n.start_btn_lbl, resource_mng->GetDefaultFont(), 50));
    start_btn->form.setFillColor(key_clr);
    start_btn->on_hover_form.setFillColor(key_clr);
    start_btn->on_hover_form.setOutlineColor(txt_clr);
    start_btn->on_hover_form.setOutlineThickness(5.f);
    start_btn->label.setPosition(37, 14);
    start_btn->label.setFillColor(txt_clr);
    start_btn->setPosition(100, 267);

    exit_btn = std::make_shared<Button>(
        400, 105,
        sf::Text(i18n.exit_btn_lbl, resource_mng->GetDefaultFont(), 50));
    exit_btn->form.setFillColor(key_clr);
    exit_btn->on_hover_form.setFillColor(key_clr);
    exit_btn->on_hover_form.setOutlineColor(txt_clr);
    exit_btn->on_hover_form.setOutlineThickness(5.f);
    exit_btn->label.setPosition(119, 14);
    exit_btn->label.setFillColor(txt_clr);
    exit_btn->setPosition(100, 390);

    menu = std::make_shared<Layout>();
    menu->setPosition((screen_width - width) / 2, (screen_height - height) / 2);

    Groupable::assign(menu, start_btn);
    menu->buttons.push_back(start_btn);

    Groupable::assign(menu, exit_btn);
    menu->buttons.push_back(exit_btn);

    Groupable::assign(menu, game_title_lbl);
  }

  void initGameLayout(const int screen_width, const int screen_height,
                      std::shared_ptr<ResourceManager> resource_mng) {
    std::string score_str = "0 / " + std::to_string(settings.winScore);

    game_score_lbl = std::make_shared<Label>(
        sf::Text(score_str, resource_mng->GetTitleFont(), 100));
    game_score_lbl->label.setFillColor(key_clr);
    game_score_lbl->setPosition(760, 70);
    auto grid = std::make_shared<Grid>(settings.width, settings.height, 64);
    map = std::make_shared<TileMap>(resource_mng->GetSnakeSpriteSheet(),
                                    sf::Vector2u(64, 64), settings.width,
                                    settings.height);

    game_layout = std::make_shared<Layout>();
    game_layout->setPosition((screen_width - width) / 2,
                             (screen_height - height) / 2);
    Groupable::assign(game_layout, grid);
    Groupable::assign(game_layout, game_score_lbl);
    Groupable::assign(game_layout, map);
  }

  void initGameRulesLayout(const int screen_width, const int screen_height,
                           std::shared_ptr<ResourceManager> resource_mng) {
    auto lbl_1 = std::make_shared<Label>(
        sf::Text(i18n.control_lbl, resource_mng->GetTitleFont(), 60));
    lbl_1->label.setFillColor(key_clr);
    lbl_1->setPosition(229, 71);

    auto img = std::make_shared<Image>(resource_mng->GetKeyArrowsTexture());
    img->sprite.scale(0.2f, 0.2f);
    img->sprite.setColor(key_clr);
    img->setPosition(359, 214);

    auto lbl_2 = std::make_shared<Label>(
        sf::Text(i18n.press_any_btn_lbl, resource_mng->GetTitleFont(), 50));
    lbl_2->label.setFillColor(key_clr);
    lbl_2->setPosition(105, 512);
    game_rules = std::make_shared<Layout>();
    game_rules->setPosition((screen_width - width) / 2,
                            (screen_height - height) / 2);
    Groupable::assign(game_rules, lbl_2);
    Groupable::assign(game_rules, lbl_1);
    Groupable::assign(game_rules, img);
  }

  void initGameLoseLayout(const int screen_width, const int screen_height,
                          std::shared_ptr<ResourceManager> resource_mng) {
    auto lbl = std::make_shared<Label>(
        sf::Text(i18n.lose_lbl, resource_mng->GetTitleFont(), 150));
    lbl->label.setFillColor(key_clr);
    lbl->setPosition(233, 249);
    game_lose = std::make_shared<Layout>();
    game_lose->setPosition((screen_width - width) / 2,
                           (screen_height - height) / 2);
    Groupable::assign(game_lose, lbl);
  }

  void initGameWinLayout(const int screen_width, const int screen_height,
                         std::shared_ptr<ResourceManager> resource_mng) {
    auto lbl = std::make_shared<Label>(
        sf::Text(i18n.win_lbl, resource_mng->GetTitleFont(), 150));
    lbl->label.setFillColor(key_clr);
    lbl->setPosition(319, 246);
    game_win = std::make_shared<Layout>();
    game_win->setPosition((screen_width - width) / 2,
                          (screen_height - height) / 2);
    Groupable::assign(game_win, lbl);
  }

  void changeLayout(std::shared_ptr<Layout> layout) {
    active_layout->active = false;
    active_layout = layout;
    active_layout->active = true;
  }
};

};      // namespace snake
#endif  // SNAKE_UI_LAYOUT_SERVER_H_
