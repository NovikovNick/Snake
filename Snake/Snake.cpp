#ifndef UNICODE
#define UNICODE
#endif 

#include "Snake.h"
#include "MyCanvas.cpp"
#include <windows.h>
#include <deque>


bool flag = true;
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
        L"Learn to Program Windows",    // Window text
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

    MyCanvas myCanvas(hwnd);
    myCanvas.startRender();


    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {

        for (;!inputQueue.empty();inputQueue.pop_back()) {
            myCanvas.inputQueue.push_back(inputQueue.back());
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    myCanvas.stopRender();

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        flag = !flag;
        break;
    case WM_KEYDOWN:
        switch (wParam)  
        {
        case VK_LEFT:
            inputQueue.push_back({false, false, true, false});
            break;
        case VK_RIGHT:
            inputQueue.push_back({ false, false, false, true });
            break;
        case VK_UP:
            inputQueue.push_back({ true, false, false, false });
            break;
        case VK_DOWN:
            inputQueue.push_back({ false, true, false, false });
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
