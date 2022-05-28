#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>

#include <iostream>
#include <deque>

#include "windows/MainWindow.h"
#include "GameLoopService.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    snake::MainWindow win;

    if (!win.Create(L"Snake The Game", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

   
    // init services
    snake::InputService *inputService = new snake::InputService();
    win.SetInputService(inputService);

    snake::RenderService* renderService = new snake::RenderService(win.Window());
    snake::GameLogicService* gameLogicService = new snake::GameLogicService();
    snake::GameLoopService *gameLoopService = new snake::GameLoopService(inputService, renderService, gameLogicService);

    // start the game
    gameLoopService->start();

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    gameLoopService->stop();

    delete inputService;
    delete renderService;
    delete gameLoopService;
    delete gameLogicService;

    return 0;
}
