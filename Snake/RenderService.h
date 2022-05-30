#ifndef SNAKE_SNAKE_RENDER_SERVICE_H_
#define SNAKE_SNAKE_RENDER_SERVICE_H_

#include "GameModels.h"


#include <d2d1.h>
#include <thread>
#include <iostream>
#include <deque>
#pragma comment(lib, "d2d1") // This line of code is equivalent to adding d2d1.lib in the additional dependency linker options.

#include "GameStateHolder.h"


namespace snake {

class RenderService {
public:
    RenderService(HWND hwnd);
    ~RenderService();
    void drawInput(float x, float y, Direction dir, float size, bool focused);
    void render(GameState gameState, GameStateHolder* gameStateHolder, GameSettigs settings);
private:

    HWND _hwnd;
    ID2D1Factory* _pD2DFactory = NULL;
    ID2D1HwndRenderTarget* _pRT = NULL;
    ID2D1SolidColorBrush* _pBlackBrush = NULL;
    ID2D1SolidColorBrush* _pLightSlateGrayBrush = NULL;
    ID2D1SolidColorBrush* _pRedBrush = NULL;
    ID2D1SolidColorBrush* _pGreenBrush = NULL;
    ID2D1SolidColorBrush* _pGrayBrush = NULL;
    RECT _rc;

    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }
};

} // namespace snake

#endif // !SNAKE_SNAKE_RENDER_SERVICE_H_
