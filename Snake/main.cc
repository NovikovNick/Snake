#define SNAKE_DEBUG 1

#include <ggponet.h>

#include <iostream>

#include "src/model/game_settings.h"
#include "src/service/game_loop_service.h"
#include "src/ui/layout_service.h"
#include "src/ui/resource_manager.h"
#include "src/ui/ui_service.h"
#include "src/util.h"

/*
1. game events to handle ggpo
2. callback to ggpo rollback
*/
int main() {
  using namespace snake;
  GameSettigs stg;
  auto type = GGPO_PLAYERTYPE_LOCAL;

  // init sockets? WSAStartup(MAKEWORD(2, 2), &wd);

  std::vector<GGPOPlayer> players(2);
  for (int i = 0; auto& player : players) {
    player.size = sizeof(player);
    player.player_num = ++i;
    player.type = GGPO_PLAYERTYPE_LOCAL;  // GGPO_PLAYERTYPE_REMOTE
  }
  /*GGPOSessionCallbacks cb = {0};
  cb.begin_game = vw_begin_game_callback;
  cb.advance_frame = vw_advance_frame_callback;
  cb.load_game_state = vw_load_game_state_callback;
  cb.save_game_state = vw_save_game_state_callback;
  cb.free_buffer = vw_free_buffer;
  cb.on_event = vw_on_event_callback;
  cb.log_game_state = vw_log_game_state;*/

  // init services
  auto resource_srv = std::make_shared<ResourceManager>();
  auto layout_srv = std::make_shared<LayoutService>(stg, resource_srv);
  auto input_srv = std::make_shared<GameEventService>();
  auto ui_srv =
      std::make_shared<UIService>(stg, resource_srv, layout_srv, input_srv);
  auto food_srv = std::make_shared<FoodService>(stg.width, stg.height);
  auto ai_srv = std::make_shared<AIService>(stg.width, stg.height);
  auto state_srv = std::make_shared<GameStateService<FoodService>>(
      stg.width, stg.height, ai_srv, food_srv);
  auto game_loop_srv = std::make_shared<GameLoopService>(
      stg, input_srv, food_srv, ai_srv, state_srv, ui_srv);

  // start game event loop in another thread
  game_loop_srv->Start();

  // run the SFML event loop and block main thread
  ui_srv->startEventLoop();
  game_loop_srv->Stop();
  return 0;
}