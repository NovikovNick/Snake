#include "../RenderService.h"


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
        _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Coral), &_pCoralBrush);
    }
}

RenderService::~RenderService() {

    SafeRelease(&_pRT);
    SafeRelease(&_pBlackBrush);
    SafeRelease(&_pLightSlateGrayBrush);
    SafeRelease(&_pRedBrush);
    SafeRelease(&_pGreenBrush);
    SafeRelease(&_pGrayBrush);
    SafeRelease(&_pCoralBrush);
    SafeRelease(&_pD2DFactory);
}

void RenderService::renderEnemy(const GameState& gameState, const int& index, const GameSettigs& settings) {
    renderPlayer(gameState, index, settings, _pRedBrush);
}

void RenderService::renderSelf(const GameState& gameState, const int& index, const GameSettigs& settings) {

    renderPlayer(gameState, index, settings, _pGreenBrush);
}

void RenderService::renderPlayer(
    const GameState& gameState, 
    const int& index, 
    const GameSettigs& settings,
    ID2D1SolidColorBrush* _pBrush) {

    int margin = 5;

    for (auto part : gameState.getPlayer(index).getParts()) {
        _pRT->FillRectangle(
            D2D1::RectF(
                size * (part.first.x + 1) - settings.snakeSize - margin,
                size * (part.first.y + 1) - settings.snakeSize - margin,
                margin + size * part.first.x + settings.snakeSize,
                margin + size * part.first.y + settings.snakeSize
            ),
            _pBrush);
    }
}

void RenderService::renderBoard(const GameSettigs& settings) {
    
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
}

void RenderService::renderDebugAI(std::vector<DebugItem> debugCtx) {
    int margin = 16;

    for (DebugItem item : debugCtx) {

        auto rect = D2D1::RectF(
            size * (item.coord.x + 1) - margin,
            size * (item.coord.y + 1) - margin,
            size * item.coord.x + margin,
            size * item.coord.y + margin
        );

        switch (item.mark) {
        case DebugMark::EXPLORED:
            _pRT->FillRectangle(rect, _pCoralBrush);
            break;
        case DebugMark::REACHABLE:
            _pRT->FillRectangle(rect, _pLightSlateGrayBrush);
            break;
        case DebugMark::PATH:
            _pRT->FillRectangle(rect, _pLightSlateGrayBrush);
            break;
        default:
            break;
        }
    }
}

void RenderService::renderFood(const Coord& food, const GameSettigs& settings) {
    
    
    _pRT->FillRectangle(
        D2D1::RectF(
            size * (food.x + 1) - settings.foodSize,
            size * (food.y + 1) - settings.foodSize,
            size * food.x + settings.foodSize,
            size * food.y + settings.foodSize
        ),
        _pLightSlateGrayBrush);
}

void RenderService::renderInputs(
    const int& frame,
    const GameStateBuffer<GameState>& holder,
    const GameSettigs& settings) {

    const int& capacity = settings.rightBoundaries - settings.leftBoundaries;
    
    for (int i = holder.getSize() - 1; i >= 0; i--) {

        const GameState& state = holder[i];
        const std::vector<Input>& inputs = state.getInputs();

        if (inputs.empty()) {
            continue;
        }

        DrawInput(
            size * -0.8 + size * settings.leftBoundaries + size * capacity - size * i,
            size * 0.2 + settings.bottomBoundaries * size,
            inputs[0].direction,
            size * 0.7,
            i == frame % capacity,
            _pGreenBrush);

        DrawInput(
            size * -0.8 + size * settings.leftBoundaries + size * capacity - size * i,
            size * 0.2 + size,
            inputs[1].direction,
            size * 0.7,
            i == frame % capacity,
            _pRedBrush);
    }
}

void RenderService::BeginDraw() {
    _pRT->BeginDraw();
    _pRT->Clear();
}

void RenderService::EndDraw() {
    _pRT->EndDraw();
}

void RenderService::renderWinState() {
    _pRT->FillRectangle(D2D1::RectF(size, _rc.bottom, 0, 0), _pGreenBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, size, 0, 0), _pGreenBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, _rc.right - size, 0), _pGreenBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, 0, _rc.bottom - size), _pGreenBrush);
}

void RenderService::renderLoseState() {
    _pRT->FillRectangle(D2D1::RectF(size, _rc.bottom, 0, 0), _pRedBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, size, 0, 0), _pRedBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, _rc.right - size, 0), _pRedBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, 0, _rc.bottom - size), _pRedBrush);
}

void RenderService::renderPauseState() {
    _pRT->FillRectangle(D2D1::RectF(size, _rc.bottom, 0, 0), _pGrayBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, size, 0, 0), _pGrayBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, _rc.right - size, 0), _pGrayBrush);
    _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, 0, _rc.bottom - size), _pGrayBrush);
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
