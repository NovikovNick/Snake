
#include <d2d1.h>
#include <thread>
#include <iostream>
#pragma comment(lib, "d2d1") // This line of code is equivalent to adding d2d1.lib in the additional dependency linker options.


class MyCanvas {
public:
    bool flag = true;
    bool prevFlag = true;


    int x = 0;

    std::thread t1;

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

    void startRender() {
        _running = true;
        t1 =  std::thread(&MyCanvas::_render, this);
    }


    void stopRender() {
        _running = false;
        t1.join();
    }

    
private:
	
    HWND _hwnd;
    ID2D1Factory* _pD2DFactory = NULL;
    ID2D1HwndRenderTarget* _pRT = NULL;
    ID2D1SolidColorBrush* _pBlackBrush = NULL;
    ID2D1SolidColorBrush* _pLightSlateGrayBrush = NULL;

    RECT _rc;
    
    bool _running = false;
    std::chrono::duration<double, std::milli> _nanos_60fps = std::chrono::duration<double, std::milli>(16);

    void _render() {

        std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>(0);

        do {

            auto t0 = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(_nanos_60fps);
            auto t1 = std::chrono::high_resolution_clock::now();

            elapsed = t1 - t0;

            _pRT->BeginDraw();
            
            _pRT->Clear();

            /*D2D1::RectF(
                _rc.left + 100.0f,
                _rc.top + 100.0f,
                _rc.right - 100.0f,
                _rc.bottom - 100.0f);*/

            if (flag != prevFlag) {
                x += 50;
            }

            _pRT->DrawRectangle(
                D2D1::RectF(
                    x + 50.0f,
                    50.0f,
                    x + 0.0f,
                    0.0f),
                flag ? _pBlackBrush : _pLightSlateGrayBrush);

            prevFlag = flag;

            _pRT->EndDraw();

            std::cout << "FPS: " << elapsed.count() << " ms\n";

        } while (_running);
    }
    

    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }
};