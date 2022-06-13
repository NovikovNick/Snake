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

void RenderService::render(GameState* gameState, GameStateHolder* holder, GameSettigs settings) {

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

    switch (gameState->gamePhase)
    {
    case IN_PROCESS:
    {
        

        for (auto it = gameState->snake_head[0]; it != NULL; it = it->next) {
            _pRT->FillRectangle(
                D2D1::RectF(
                    size * (it->coord.x + 1) - settings.snakeSize,
                    size * (it->coord.y + 1) - settings.snakeSize,
                    size * it->coord.x + settings.snakeSize,
                    size * it->coord.y + settings.snakeSize
                ),
                _pGreenBrush);
        }

        for (auto it = gameState->snake_head[1]; it != NULL; it = it->next) {
            _pRT->FillRectangle(
                D2D1::RectF(
                    size * (it->coord.x + 1) - settings.snakeSize,
                    size * (it->coord.y + 1) - settings.snakeSize,
                    size * it->coord.x + settings.snakeSize,
                    size * it->coord.y + settings.snakeSize
                ),
                _pRedBrush);
        }
        
        
        int frame = holder->GetFrame();
        int capacity = settings.rightBoundaries - settings.leftBoundaries;
        for (int i = 0; i < capacity && frame >= 0; i++) {
           
            DrawInput(
               size * -0.8 + size * settings.leftBoundaries  + size * capacity - size * i,
               size * 0.2 + settings.bottomBoundaries * size,
                holder->GetInput(frame, 0).direction,
                size * 0.7, 
                frame == gameState->frame,
                _pGreenBrush);

           frame--;
        }

        frame = holder->GetFrame();
        for (int i = 0; i < capacity && frame >= 0; i++) {

            DrawInput(
                size * -0.8 + size * settings.leftBoundaries + size * capacity - size * i,
                size * 0.2 + size,
                holder->GetInput(frame, 1).direction,
                size * 0.7,
                frame == gameState->frame,
                _pRedBrush);

            frame--;
        }

        Coord* food = &(gameState->food);
        if (food != NULL) {

            _pRT->FillRectangle(
                D2D1::RectF(
                    size * (food->x + 1) - settings.foodSize,
                    size * (food->y + 1) - settings.foodSize,
                    size * food->x + settings.foodSize,
                    size * food->y + settings.foodSize
                ),
                _pLightSlateGrayBrush);
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

void RenderService::DrawInput(
    float x,
    float y, 
    Direction dir, 
    float arrowBlockSize, 
    bool focused,
    ID2D1SolidColorBrush* _pBrush) {

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
        case Direction::UP:
            pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y ));
            break;
        case Direction::DOWN:
            pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y + arrowBlockSize));
            break;
        case Direction::LEFT:
            pSink->BeginFigure(D2D1::Point2F(x + arrowBlockSize, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize / 2));
            break;
        case Direction::RIGHT:
            pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize / 2));
            break;
        default:
            pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
            pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize * 0.9));
            pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize * 0.9));
            break;
        }

        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        hr = pSink->Close();

        if (focused) {
            _pRT->FillGeometry(pPathGeometry, _pBrush);
        } else {
            _pRT->DrawGeometry(pPathGeometry, _pGrayBrush);
        }

        pPathGeometry->Release();
        pSink->Release();
    }
}

} // namespace snake
