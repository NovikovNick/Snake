#include "../render_service.h"

namespace snake {

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

/*
DrawInput(size * -0.8 + size * settings.leftBoundaries + size * capacity -
                  size * i,
              size * 0.2 + settings.bottomBoundaries * size,
              inputs[0].direction, size * 0.7, i == frame % capacity,
              _pGreenBrush);

DrawInput(size * -0.8 + size * settings.leftBoundaries + size * capacity -
                size * i,
            size * 0.2 + size, inputs[1].direction, size * 0.7,
            i == frame % capacity, _pRedBrush);
*/
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
    ID2D1SolidColorBrush* brush = nullptr;
    switch (type) {
      case 1:
        brush = _pWhiteBrush;
        break;
      case 2:
        brush = _pGreenBrush;
        break;
      case 3:
        brush = _pRedBrush;
        break;
      case 4:
        brush = _pBlueBrush;
        break;
      case 5:
        brush = _pCoralBrush;
        break;
      default:
        brush = nullptr;
        break;
    }
    int top_padding = 60;
    int lft_padding = 20;

    int lft = lft_padding + x * size;
    int top = top_padding + y * size;
    int rht = lft_padding + (x + 1) * size;
    int btm = top_padding + (y + 1) * size;

    if (brush != nullptr)
      _pRT->FillRectangle(D2D1::RectF(lft, top, rht, btm), brush);

    _pRT->DrawRectangle(D2D1::RectF(lft, top, rht, btm), _pGrayBrush);
  }
  EndDraw();
}

}  // namespace snake
