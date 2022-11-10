#include "../render_service.h"

namespace snake {

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

void RenderService::renderInputs(const int& frame,
                                 const GameStateBuffer<GameState>& holder,
                                 const GameSettigs& settings) {
  const int& capacity = settings.rightBoundaries - settings.leftBoundaries;

  for (int i = holder.getSize() - 1; i >= 0; --i) {
    const GameState& state = holder[i];
    const std::vector<Input>& inputs = state.GetInputs();

    if (inputs.empty()) {
      continue;
    }

    DrawInput(size * -0.8 + size * settings.leftBoundaries + size * capacity -
                  size * i,
              size * 0.2 + settings.bottomBoundaries * size,
              inputs[0].direction, size * 0.7, i == frame % capacity,
              _pGreenBrush);

    DrawInput(size * -0.8 + size * settings.leftBoundaries + size * capacity -
                  size * i,
              size * 0.2 + size, inputs[1].direction, size * 0.7,
              i == frame % capacity, _pRedBrush);
  }
}

void RenderService::BeginDraw() {
  _pRT->BeginDraw();
  _pRT->Clear();
}

void RenderService::EndDraw() { _pRT->EndDraw(); }

void RenderService::renderWinState() {
  _pRT->FillRectangle(D2D1::RectF(size, _rc.bottom, 0, 0), _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, size, 0, 0), _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, _rc.right - size, 0),
                      _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, 0, _rc.bottom - size),
                      _pGreenBrush);
}

void RenderService::DrawInput(float x, float y, Direction dir,
                              float arrowBlockSize, bool focused,
                              ID2D1SolidColorBrush* _pBrush) {
  ID2D1PathGeometry* pPathGeometry = NULL;
  ID2D1GeometrySink* pSink = NULL;

  HRESULT hr = _pD2DFactory->CreatePathGeometry(&pPathGeometry);
  if (SUCCEEDED(hr)) {
    hr = pPathGeometry->Open(&pSink);
  }
  if (SUCCEEDED(hr)) {
    pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

    switch (dir) {
      case Direction::kUp:
        pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize),
                           D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y));
        break;
      case Direction::kDown:
        pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y));
        pSink->AddLine(
            D2D1::Point2F(x + arrowBlockSize / 2, y + arrowBlockSize));
        break;
      case Direction::kLeft:
        pSink->BeginFigure(D2D1::Point2F(x + arrowBlockSize, y),
                           D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
        pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize / 2));
        break;
      case Direction::kRight:
        pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize));
        pSink->AddLine(
            D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize / 2));
        break;
      default:
        pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize),
                           D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
        pSink->AddLine(
            D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize * 0.9));
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

// v2
GAME_OBJECT_ITERATOR RenderService::GetOutput() {
  return game_objects_.begin();
}

void RenderService::Render() {
  BeginDraw();
  for (auto [x, y, type] : game_objects_) {
    switch (type) {
      case 1:
        _pRT->FillRectangle(
            D2D1::RectF(x * size, y * size, (x + 1) * size, (y + 1) * size),
            _pGreenBrush);
        break;
      case 2:
        _pRT->FillRectangle(
            D2D1::RectF(x * size, y * size, (x + 1) * size, (y + 1) * size),
            _pRedBrush);
        break;
      case 3:
        _pRT->FillRectangle(
            D2D1::RectF(x * size, y * size, (x + 1) * size, (y + 1) * size),
            _pCoralBrush);
        break;
      case 4:
        _pRT->FillRectangle(
            D2D1::RectF(x * size, y * size, (x + 1) * size, (y + 1) * size),
            _pGrayBrush);
        break;
      case 5:
        _pRT->FillRectangle(
            D2D1::RectF(x * size, y * size, (x + 1) * size, (y + 1) * size),
            _pLightSlateGrayBrush);
        break;
      default:
        _pRT->DrawRectangle(
            D2D1::RectF(size * (x + 1), size * (y + 1), size * x, size * y),
            _pGrayBrush);
        break;
    }
  }
  EndDraw();
}

}  // namespace snake
