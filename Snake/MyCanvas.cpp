
#include <d2d1.h>
#include <thread>
#include <iostream>
#include <deque>
#pragma comment(lib, "d2d1") // This line of code is equivalent to adding d2d1.lib in the additional dependency linker options.

struct Input {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

class MyCanvas {
public:

    std::deque<Input> inputQueue = std::deque<Input, std::allocator<Input>>();
    int x = 0;
    int y = 0;

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
	}

    ~MyCanvas() {

        SafeRelease(&_pRT);
        SafeRelease(&_pBlackBrush);
        SafeRelease(&_pLightSlateGrayBrush);
        SafeRelease(&_pD2DFactory);
    }

    void startRender() {
        _running = true;
        _renderThread =  std::thread(&MyCanvas::_render, this);
    }


    void stopRender() {
        _running = false;
        _renderThread.join();
    }

    
private:
	
    HWND _hwnd;
    ID2D1Factory* _pD2DFactory = NULL;
    ID2D1HwndRenderTarget* _pRT = NULL;
    ID2D1SolidColorBrush* _pBlackBrush = NULL;
    ID2D1SolidColorBrush* _pLightSlateGrayBrush = NULL;

    RECT _rc;

    std::thread _renderThread;
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

            for (; !inputQueue.empty(); inputQueue.pop_back()) {

                Input input = inputQueue.back();

                if (input.left) x -= 50;
                if (input.right) x += 50;
                if (input.up) y -= 50;
                if (input.down) y += 50;
            }

            _pRT->DrawRectangle(
                D2D1::RectF(
                    x + 50.0f,      //left
                    y + 50.0f,      //top
                    x + 0.0f,       //right
                    y + 0.0f),      //bottom
                _pBlackBrush);

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