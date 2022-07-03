#include "BaseWindow.h"

#include <deque>

#include "../service/GameModels.h"
#include "../service/InputService.h"

namespace snake {

class MainWindow : public BaseWindow<MainWindow>
{
public:
    PCWSTR  ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline void SetInputService(InputService* inputService) { _inputService =  inputService; };

private:
    InputService* _inputService;
};

} // namespace snake
