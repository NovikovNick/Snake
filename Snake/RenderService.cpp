#include "RenderService.h"


RenderService::RenderService(HWND hwnd) {
	_hwnd = hwnd;

    // 2. --------------------------
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &_pD2DFactory
    );

    // 3. --------------------------
    // Obtain the size of the drawing area.

    GetClientRect(_hwnd, &_rc);

    // Create a Direct2D render target  
    hr = _pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hwnd,
            D2D1::SizeU(
                _rc.right - _rc.left,
                _rc.bottom - _rc.top)
        ),
        &_pRT
    );

    // 4. --------------------------

    if (SUCCEEDED(hr))
    {

        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &_pBlackBrush);
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &_pLightSlateGrayBrush);
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &_pRedBrush);
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &_pGreenBrush);
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &_pGrayBrush);
    }
}

RenderService::~RenderService() {

    SafeRelease(&_pRT);
    SafeRelease(&_pBlackBrush);
    SafeRelease(&_pLightSlateGrayBrush);
    SafeRelease(&_pRedBrush);
    SafeRelease(&_pGreenBrush);
    SafeRelease(&_pGrayBrush);
    SafeRelease(&_pD2DFactory);
}
    
void RenderService::render(GameState gameState, GameSettigs settings) {

    float size = 40.0f;

    _pRT->BeginDraw();
    _pRT->Clear();
    
    for (size_t x = settings.leftBoundaries; x < settings.rightBoundaries; x++)
    {
        for (size_t y = settings.topBoundaries; y < settings.bottomBoundaries; y++)
        {
            _pRT->DrawRectangle(
                D2D1::RectF(
                    size * (x + 1),
                    size * (y + 1),
                    size * x,
                    size * y
                ),
                _pGrayBrush);
        }
    }

    

    switch (gameState.gamePhase)
    {
    case IN_PROCESS:
    {
        Coord* food = &(gameState.food);
        if (food != NULL) {

            _pRT->FillRectangle(
                D2D1::RectF(
                    size * (food->x + 1) - settings.foodSize,
                    size * (food->y + 1) - settings.foodSize,
                    size * food->x  + settings.foodSize,
                    size * food->y + settings.foodSize
                ),
                _pLightSlateGrayBrush);
        }

        for (auto it = gameState.snake_head; it != NULL; it = it->next) {
            _pRT->FillRectangle(
                D2D1::RectF(
                    size * (it->coord.x + 1) - settings.snakeSize,
                    size * (it->coord.y + 1) - settings.snakeSize,
                    size * it->coord.x + settings.snakeSize,
                    size * it->coord.y + settings.snakeSize
                ),
                _pRedBrush);
        }
        break;
    }
    case WIN:
    {
        _pRT->FillRectangle(
            D2D1::RectF(
                _rc.right,
                _rc.bottom,
                0,
                0
            ),
            _pGreenBrush);
        break;
    }
    case LOSE:
    default:
    {
        _pRT->FillRectangle(
            D2D1::RectF(
                _rc.right,
                _rc.bottom,
                0,
                0
            ),
            _pRedBrush);
        break;
    }
    }

    _pRT->EndDraw();
}
