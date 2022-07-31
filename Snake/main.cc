#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>

#include <iostream>
#include <deque>

#include "src/win32/main_window.h"
#include "src/service/game_loop_service.h"




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    snake::log("Main loop started");

    snake::MainWindow win;

    if (!win.Create(L"Snake The Game", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);
   
    // init services
    auto inputServicePtr = std::make_shared<snake::InputService>();
    win.SetInputService(inputServicePtr);

    auto renderServicePtr = std::make_shared<snake::RenderService>(win.Window());
    auto gameLogicServicePtr = std::make_shared<snake::GameLogicService>();
    auto aiServicePtr = std::make_shared<snake::AIService>();
    auto gameLoopService = std::make_shared<snake::GameLoopService>(inputServicePtr, renderServicePtr, gameLogicServicePtr, aiServicePtr);

    // start the game
    gameLoopService->start();

    // Run the message loop.
    win.RunMessageLoop();

    gameLoopService->stop();

    snake::log("Main loop ended. Deleting...");
    return 0;
}
