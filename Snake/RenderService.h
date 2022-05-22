#pragma once

#include "GameModels.h"


#include <d2d1.h>
#include <thread>
#include <iostream>
#include <deque>
#pragma comment(lib, "d2d1") // This line of code is equivalent to adding d2d1.lib in the additional dependency linker options.


class RenderService {
public:
    RenderService(HWND hwnd);
    ~RenderService();
    void render(GameState gameState);
private:
	
    HWND _hwnd;
    ID2D1Factory* _pD2DFactory = NULL;
    ID2D1HwndRenderTarget* _pRT = NULL;
    ID2D1SolidColorBrush* _pBlackBrush = NULL;
    ID2D1SolidColorBrush* _pLightSlateGrayBrush = NULL;
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