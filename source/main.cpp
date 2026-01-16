#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d11.h>
#include <dxgi.h>
// #include <string>
// #include "terminal_colors.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")




const int wWidth = 800;
const int wHeight = 800;
const TCHAR* WndClassName = TEXT("Window Blueprint");
const TCHAR* WndTitle = TEXT("Dxrend");


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(33, 32, 66));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WndClassName;
    wc.hIcon = nullptr;


    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Done know it mash up!"), NULL);
        return 1;
    }

    HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, WndClassName, WndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Guided Tour"), NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // initialize d3d



    MSG msg = {};
    bool running = true;
    while (running)
    {
        // Handle messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return  msg.wParam;
}


// handle messages within the procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Directx will paunt, so no need to paint here
            EndPaint(hWnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN: // WM_CHAR is for text input WM_KEYDOWN is for key presses like WASD
            if (wParam == 'C')
            {
                SetWindowText(hWnd, "New Window Name");
            }
            break;
        default:
            return DefWindowProc(hWnd,message, wParam, lParam);
            break;
    }

    return 0;
}
