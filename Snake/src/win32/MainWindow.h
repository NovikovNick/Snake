#include "BaseWindow.h"

#include <deque>

#include "../service/GameModels.h"
#include "../service/InputService.h"

namespace snake {

class MainWindow : public BaseWindow<MainWindow>{
    InputService* inputService_;
public:
    PCWSTR  ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline void SetInputService(InputService* inputService) { inputService_ =  inputService; };
};

} // namespace snake
