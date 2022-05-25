#include "MainWindow.h"


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
