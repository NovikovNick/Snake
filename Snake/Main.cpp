#ifndef UNICODE
#define UNICODE
#endif 


#include "GameLoopService.h"

#include <iostream>
#include <windows.h>
#include <deque>



std::deque<Input> inputQueue = std::deque<Input, std::allocator<Input>>();

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Snake The Game",              // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);


    // init services
    InputService *inputService = new InputService();
    RenderService* renderService = new RenderService(hwnd);
    GameLogicService* gameLogicService = new GameLogicService();
    GameLoopService *gameLoopService = new GameLoopService(inputService, renderService, gameLogicService);


    // start the game
    gameLoopService->start();


    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {

        for (;!inputQueue.empty();inputQueue.pop_front()) {
            inputService->addInput(inputQueue.front());
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)  
        {
        case VK_LEFT:
            inputQueue.push_back({ LEFT });
            break;
        case VK_RIGHT:
            inputQueue.push_back({ RIGHT });
            break;
        case VK_UP:
            inputQueue.push_back({ UP });
            break;
        case VK_DOWN:
            inputQueue.push_back({ DOWN });
            break;
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    //case WM_CLOSE:
    //    if (MessageBox(hwnd, L"Really quit?", L"Snake The Game", MB_OKCANCEL) == IDOK)
    //    {
    //        DestroyWindow(hwnd);
    //    }
    //    // Else: User canceled. Do nothing.
    //    return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
