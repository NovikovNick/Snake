#include "main_window.h"

namespace snake {

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_KEYDOWN:
      switch (wParam) {
        case VK_LEFT:
          inputService_->AddInput({Direction::kLeft, SystemCommand::kNone});
          break;
        case VK_RIGHT:
          inputService_->AddInput({Direction::kRight, SystemCommand::kNone});
          break;
        case VK_UP:
          inputService_->AddInput({Direction::kUp, SystemCommand::kNone});
          break;
        case VK_DOWN:
          inputService_->AddInput({Direction::kDown, SystemCommand::kNone});
          break;
        case VK_SPACE:
          inputService_->AddInput({Direction::kNone, SystemCommand::kPause});
          break;
        case VK_F1:
          inputService_->AddInput(
              {Direction::kNone, SystemCommand::kStepBackward});
          break;
        case VK_F2:
          inputService_->AddInput(
              {Direction::kNone, SystemCommand::kStepForward});
          break;
        case VK_F3:
          inputService_->AddInput(
              {Direction::kNone, SystemCommand::kAIStepBackward});
          break;
        case VK_F4:
          inputService_->AddInput(
              {Direction::kNone, SystemCommand::kAIStepForward});
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

    case WM_PAINT: {
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

}  // namespace snake
