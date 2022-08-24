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
  // init window
  snake::MainWindow win;
  if (!win.Create(L"Snake The Game", WS_OVERLAPPEDWINDOW)) {
    return 0;
  }
  ShowWindow(win.Window(), nCmdShow);

  // init services
  auto input_srv = std::make_shared<snake::InputService>();
  win.SetInputService(input_srv);

  auto render_srv = std::make_shared<snake::RenderService>(win.Window());
  auto game_logic_srv = std::make_shared<snake::GameLogicService>();
  auto snake_srv = std::make_shared<snake::SnakeService>();
  auto game_state_srv = std::make_shared<snake::GameStateService>(snake_srv);
  auto ai_srv = std::make_shared<snake::AIService>();

  auto game_loop_srv = std::make_shared<snake::GameLoopService>(
      input_srv, render_srv, game_logic_srv, ai_srv, game_state_srv);

  // start the game
  game_loop_srv->Start();

  // run the message loop and block main thread
  win.RunMessageLoop();

  game_loop_srv->Stop();

  return 0;
}
