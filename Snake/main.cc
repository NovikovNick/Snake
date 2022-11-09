#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include <deque>
#include <iostream>

#include "src/service/game_loop_service.h"
#include "src/win32/main_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine,
                    int nCmdShow) {
  snake::GameSettigs stg;

  // init window
  snake::MainWindow win;
  if (!win.Create(L"snake The Game", WS_OVERLAPPEDWINDOW, 0, 0, 0, 1400, 750)) {
    return 0;
  }
  ShowWindow(win.Window(), nCmdShow);

  // init services
  auto input_srv = std::make_shared<snake::InputService>();
  win.SetInputService(input_srv);

  auto render_srv = std::make_shared<snake::RenderService>(win.Window(), stg.width, stg.height);
  auto food_srv = std::make_shared<snake::FoodService>(stg.width, stg.height);
  auto ai_srv = std::make_shared<snake::AIService>(stg.width, stg.height);
  auto state_srv = std::make_shared<snake::GameStateService>(stg.width, stg.height, ai_srv);

  auto game_loop_srv = std::make_shared<snake::GameLoopService>(
      input_srv, render_srv, food_srv, ai_srv, state_srv);

  // start the game
  game_loop_srv->Start();

  // run the message loop and block main thread
  win.RunMessageLoop();

  game_loop_srv->Stop();

  return 0;
}
