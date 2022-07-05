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

    void renderSelf(GameState* gameState, int index, GameSettigs settings);
    void renderEnemy(GameState* gameState, int index, GameSettigs settings);
    void renderPlayer(GameState* gameState, int index, GameSettigs settings, ID2D1SolidColorBrush* _pBrush);

    void renderBoard(GameSettigs settings);

    void renderFood(GameState* gameState, GameSettigs settings);

    void renderSelfInputs(GameState* gameState, GameStateHolder* holder, GameSettigs settings);
    void renderEnemyInputs(GameState* gameState, GameStateHolder* holder, GameSettigs settings);
    
    void renderWinState();
    void renderLoseState();
    void renderPauseState();

    void renderDebugAI(std::vector<DebugItem> debugCtx);

    void BeginDraw();
    void EndDraw();

    float size = 40.0f;

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

    void DrawInput(float x, float y, Direction dir, float size, bool focused, ID2D1SolidColorBrush* _pBrush);

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