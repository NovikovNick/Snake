
#include <d2d1.h>
#pragma comment(lib, "d2d1") // This line of code is equivalent to adding d2d1.lib in the additional dependency linker options.


class MyCanvas {
public:

    bool flag = true;

	MyCanvas (HWND hwnd) {
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

        // 5. --------------------------
        _pRT->BeginDraw();

        _pRT->DrawRectangle(
            D2D1::RectF(
                _rc.left + 100.0f,
                _rc.top + 100.0f,
                _rc.right - 100.0f,
                _rc.bottom - 100.0f),
            flag ? _pBlackBrush : _pLightSlateGrayBrush);

        hr = _pRT->EndDraw();

	}

    ~MyCanvas() {

        SafeRelease(&_pRT);
        SafeRelease(&_pBlackBrush);
        SafeRelease(&_pLightSlateGrayBrush);
        SafeRelease(&_pD2DFactory);
    }

    void render() {
        // 5. --------------------------
        _pRT->BeginDraw();

        _pRT->DrawRectangle(
            D2D1::RectF(
                _rc.left + 100.0f,
                _rc.top + 100.0f,
                _rc.right - 100.0f,
                _rc.bottom - 100.0f),
            flag ? _pBlackBrush : _pLightSlateGrayBrush);

         _pRT->EndDraw();
    }
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