﻿#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>

#include <iostream>
#include <deque>

#include "windows/MainWindow.h"
#include "GameLoopService.h"

// todo: simplify ApplyForces algorithm
// todo: prevent instant fail with opposite inputs
// todo: generate food only in empty spot
// todo: improve AI to dodge player
// todo: add ability to increase speed for input with same direction
// todo: backward game state and insert new inputs


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    snake::log("Main loop started");

    snake::MainWindow win;

    if (!win.Create(L"Snake The Game", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);
   
    // init services
    snake::InputService *inputService = new snake::InputService();
    win.SetInputService(inputService);

    snake::RenderService* renderService = new snake::RenderService(win.Window());
    snake::GameLogicService* gameLogicService = new snake::GameLogicService();
    snake::AIService* aiService = new snake::AIService();
    snake::GameLoopService* gameLoopService = new snake::GameLoopService(inputService, renderService, gameLogicService, aiService);

    // start the game
    gameLoopService->start();

    // Run the message loop.
    win.RunMessageLoop();

    gameLoopService->stop();

    snake::log("Main loop ended. Deleting...");

    delete inputService;
    delete renderService;
    delete gameLoopService;
    delete gameLogicService;

    return 0;
}
