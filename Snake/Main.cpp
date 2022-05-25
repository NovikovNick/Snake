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
    MainWindow win;

    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    // init services
    InputService *inputService = new InputService();
    RenderService* renderService = new RenderService(win.Window());
    GameLogicService* gameLogicService = new GameLogicService();
    GameLoopService *gameLoopService = new GameLoopService(inputService, renderService, gameLogicService);


    // start the game
    gameLoopService->start();


    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {

        for (;!win.inputQueue.empty(); win.inputQueue.pop_front()) {
            inputService->addInput(win.inputQueue.front());
        }

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
