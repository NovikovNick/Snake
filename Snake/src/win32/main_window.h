#include "base_window.h"

#include <deque>

#include "../service/game_models.h"
#include "../service/input_service.h"

namespace snake {

class MainWindow : public BaseWindow<MainWindow>{
    std::shared_ptr<InputService> inputService_;
public:
    PCWSTR  ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline void SetInputService(std::shared_ptr<InputService> inputService) { inputService_ =  inputService; };
};

} // namespace snake
