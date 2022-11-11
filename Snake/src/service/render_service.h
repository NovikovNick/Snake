#ifndef SNAKE_SNAKE_RENDER_SERVICE_H_
#define SNAKE_SNAKE_RENDER_SERVICE_H_

#include <d2d1.h>

#include <deque>
#include <iostream>
#include <thread>

#include "../model/grid.h"
#include "game_models.h"
#pragma comment(lib, \
                "d2d1")  // This line of code is equivalent to adding d2d1.lib
                         // in the additional dependency linker options.

namespace snake {

class RenderService final {
 public:
  RenderService(HWND hwnd, const int width, const int height)
      : width_(width),
        height_(height),
        game_objects_(std::vector<GAME_OBJECT>(width * height)),
        _hwnd(hwnd){
  
      // 2. --------------------------
    HRESULT hr =
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);

    // 3. --------------------------
    // Obtain the size of the drawing area.

    GetClientRect(_hwnd, &_rc);

    // Create a Direct2D render target
    hr = _pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(_rc.right - _rc.left, _rc.bottom - _rc.top)),
        &_pRT);

    // 4. --------------------------

    if (SUCCEEDED(hr)) {
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                  &_pBlackBrush);
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua),
                                  &_pLightSlateGrayBrush);
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson),
                                  &_pRedBrush);
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green),
                                  &_pGreenBrush);
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray),
                                  &_pGrayBrush);
      _pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Coral),
                                  &_pCoralBrush);
    }
  };

  ~RenderService();

  void renderWinState();

  void BeginDraw();

  void EndDraw();

  GAME_OBJECT_ITERATOR GetOutput();
  void Render();

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
  ID2D1SolidColorBrush* _pCoralBrush = NULL;
  RECT _rc;

  std::vector<GAME_OBJECT> game_objects_;
  int width_, height_;

  void DrawInput(float x, float y, Direction dir, float size, bool focused,
                 ID2D1SolidColorBrush* _pBrush);

  template <class T>
  void SafeRelease(T** ppT) {
    if (*ppT) {
      (*ppT)->Release();
      *ppT = NULL;
    }
  }
};
}  // namespace snake

#endif  // !SNAKE_SNAKE_RENDER_SERVICE_H_
