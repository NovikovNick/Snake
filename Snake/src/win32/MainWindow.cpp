#include "MainWindow.h"

namespace snake {


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            inputService_->addInput({ Direction::LEFT, SystemCommand::NONE});
            break;
        case VK_RIGHT:
            inputService_->addInput({ Direction::RIGHT, SystemCommand::NONE });
            break;
        case VK_UP:
            inputService_->addInput({ Direction::UP, SystemCommand::NONE });
            break;
        case VK_DOWN:
            inputService_->addInput({ Direction::DOWN, SystemCommand::NONE });
            break;
        case VK_SPACE:
            inputService_->addInput({ Direction::NONE, SystemCommand::PAUSE });
            break;
        case VK_F1:
            inputService_->addInput({ Direction::NONE, SystemCommand::STEP_BACKWARD });
            break;
        case VK_F2:
            inputService_->addInput({ Direction::NONE, SystemCommand::STEP_FORWARD });
            break;
        case VK_F3:
            inputService_->addInput({ Direction::NONE, SystemCommand::AI_STEP_BACKWARD });
            break;
        case VK_F4:
            inputService_->addInput({ Direction::NONE, SystemCommand::AI_STEP_FORWARD });
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

} // namespace snake
