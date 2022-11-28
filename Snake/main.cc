#include <iostream>

//#include "src/model/game_settings.h"
//#include "src/service/game_loop_service.h"
#include "src/ui/resource_manager.h"
#include "src/ui/layout_service.h"
#include "src/ui/ui_service.h"

int main() {
  // snake::GameSettigs stg;

  // init services
  auto resource_srv = std::make_shared<snake::ResourceManager>();
  auto layout_srv = std::make_shared<snake::LayoutService>(resource_srv);
  auto ui_srv = std::make_shared<snake::UIService>(resource_srv, layout_srv);

  /*auto input_srv = std::make_shared<snake::InputService>();
  auto food_srv = std::make_shared<snake::FoodService>(stg.width, stg.height);
  auto ai_srv = std::make_shared<snake::AIService>(stg.width, stg.height);
  auto state_srv = std::make_shared<snake::GameStateService<snake::FoodService>>( stg.width, stg.height, ai_srv, food_srv);
  auto game_loop_srv = std::make_shared<snake::GameLoopService>( stg, input_srv, ui_srv, food_srv, ai_srv, state_srv);*/

  // start the game
  // game_loop_srv->Start();

  // run the SFML event loop and block main thread
  ui_srv->startEventLoop();
  // game_loop_srv->Stop();
  return 0;
}