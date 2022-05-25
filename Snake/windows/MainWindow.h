#include "BaseWindow.h"

#include <deque>

#include "../GameModels.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    PCWSTR  ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    std::deque<Input> inputQueue = std::deque<Input, std::allocator<Input>>();
};
