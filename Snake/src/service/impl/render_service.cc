#include "../render_service.h"

namespace snake {

void RenderService::BeginDraw() {
  _pRT->BeginDraw();
  _pRT->Clear();
}

void RenderService::EndDraw() { _pRT->EndDraw(); }

void RenderService::renderWinState() {
  auto size = setting_.cell_size;
  _pRT->FillRectangle(D2D1::RectF(size, _rc.bottom, 0, 0), _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, size, 0, 0), _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, _rc.right - size, 0),
                      _pGreenBrush);
  _pRT->FillRectangle(D2D1::RectF(_rc.right, _rc.bottom, 0, _rc.bottom - size),
                      _pGreenBrush);
}

void RenderService::DrawInput(float x, float y, int dir, float arrowBlockSize,
                              bool focused, ID2D1SolidColorBrush* _pBrush) {
  ID2D1PathGeometry* pPathGeometry = NULL;
  ID2D1GeometrySink* pSink = NULL;

  HRESULT hr = _pD2DFactory->CreatePathGeometry(&pPathGeometry);
  if (SUCCEEDED(hr)) {
    hr = pPathGeometry->Open(&pSink);
  }
  if (SUCCEEDED(hr)) {
    pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

    switch (dir) {
      case 3:
        pSink->BeginFigure(D2D1::Point2F(x, y + arrowBlockSize),
                           D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize / 2, y));
        break;
      case 2:
        pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y));
        pSink->AddLine(
            D2D1::Point2F(x + arrowBlockSize / 2, y + arrowBlockSize));
        break;
      case 1:
        pSink->BeginFigure(D2D1::Point2F(x + arrowBlockSize, y),
                           D2D1_FIGURE_BEGIN_FILLED);
        pSink->AddLine(D2D1::Point2F(x + arrowBlockSize, y + arrowBlockSize));
        pSink->AddLine(D2D1::Point2F(x, y + arrowBlockSize / 2));
        break;
      case 0:
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

void RenderService::Render(const int offset) {
  int size = setting_.cell_size;
  int width = setting_.height;
  int height = setting_.width;
  int top_padding = (setting_.screen_height - setting_.cell_size * height) / 2;
  int lft_padding = (setting_.screen_width - setting_.cell_size * width) / 2;

  BeginDraw();
  for (int i = 0; i < width * height; ++i) {
    auto [x, y, type] = game_objects_[i];

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

    int lft = lft_padding + x * size;
    int top = top_padding + y * size;
    int rht = lft_padding + (x + 1) * size;
    int btm = top_padding + (y + 1) * size;

    if (brush != nullptr)
      _pRT->FillRectangle(D2D1::RectF(lft, top, rht, btm), brush);

    _pRT->DrawRectangle(D2D1::RectF(lft, top, rht, btm), _pGrayBrush);
  }

  for (int x = 0; x < setting_.width; ++x) {
    auto [snake_id, dir, type] = game_objects_[width * height + x];
    int inp_x = 5 + lft_padding + size * (setting_.width - 1) - size * x;
    int inp_y = top_padding - size;
    DrawInput(inp_x, inp_y, type == 5 ? dir : -1, size * 0.7, x == offset,
              _pGreenBrush);
  }
  EndDraw();
}

}  // namespace snake
