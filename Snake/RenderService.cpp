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

        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &_pBlackBrush);
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &_pLightSlateGrayBrush);
    }
}

RenderService::~RenderService() {

    SafeRelease(&_pRT);
    SafeRelease(&_pBlackBrush);
    SafeRelease(&_pLightSlateGrayBrush);
    SafeRelease(&_pD2DFactory);
}
    
void RenderService::render(GameState gameState) {

    float size = 40.0f;

    _pRT->BeginDraw();
    _pRT->Clear();

    Coord* food = &(gameState.food);

    if (food != NULL) {
        _pRT->DrawRectangle(
            D2D1::RectF(
                size * (food->x + 1),
                size * (food->y + 1),
                size * food->x,
                size * food->y
            ),
            _pLightSlateGrayBrush);
    }

    for (auto it = gameState.snake_head; it != NULL; it = it->next) {
        _pRT->DrawRectangle(
            D2D1::RectF(
                size * (it->coord.x + 1),
                size * (it->coord.y + 1),
                size * it->coord.x,
                size * it->coord.y
            ),
            _pBlackBrush);
    }

    _pRT->EndDraw();
}