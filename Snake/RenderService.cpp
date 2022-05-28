#include "RenderService.h"

namespace snake {


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
        float margin = 15.0f;
        drawInput(200, 660, gameState.snake_head->direction, size - margin);
       

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


        RECT tRect;
        GetClientRect(_hwnd, &tRect);
        tRect.right = 300;
        tRect.top = 600;

        HDC dc = GetDC(_hwnd);
        DrawText(dc, "->", -1, &tRect, DT_CENTER);
        ReleaseDC(_hwnd, dc);


        break;
    }
    }

    _pRT->EndDraw();
}

void RenderService::drawInput(float x, float y, Direction dir, float arrowBlockSize) {

    ID2D1PathGeometry* pPathGeometry = NULL;
    ID2D1GeometrySink* pSink = NULL;

    HRESULT hr = _pD2DFactory->CreatePathGeometry(&pPathGeometry);
    if (SUCCEEDED(hr))
    {
        hr = pPathGeometry->Open(&pSink);
    }
    if (SUCCEEDED(hr))
    {
        pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

        switch (dir)
        {
        case snake::UP:
            pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y ));
            break;
        case snake::DOWN:
            pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y + arrowBlockSize));
            break;
        case snake::LEFT:
            pSink->BeginFigure(D2D1::Point2F(x + arrowBlockSize, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize / 2));
            break;
        case snake::RIGHT:
            pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize / 2));
            break;
        default:
            pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            break;
        }

        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        hr = pSink->Close();

        _pRT->DrawGeometry(pPathGeometry, _pGrayBrush);
    }
}

} // namespace snake
