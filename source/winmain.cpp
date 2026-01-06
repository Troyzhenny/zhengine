#include<windows.h>
#include <wingdi.h>
// #include <iostream>
// #include "terminal_colors.h"



// define window class
LPCTSTR WndClassName = "firstwindow";
// window handle
HWND hwnd = NULL;

const int Width  = 800;
const int Height = 600;


// Declarations
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    if(!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
    {
        MessageBox(0, "Window Initialization - Failed",
                    "Error", MB_OK);
        return 0;
    }

    messageloop();

    return 0;
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
    // windows extension class
    WNDCLASSEX wc = {0};

    // This is the blueprint / recipe for the window
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(245, 166, 39));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WndClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Error registering class",
            "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // this is the result of the window blueprint
    hwnd = CreateWindowEx(
        0,
        WndClassName,
        "Active Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL
        );

    if (!hwnd)
    {
        MessageBox(NULL, "Error creating window",
            "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    return true;
}

int messageloop(){

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while(true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);

            //Send the message to default windows procedure
            DispatchMessage(&msg);
        }

        else{

        }


    }

    return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd,
             UINT msg,
             WPARAM wParam,
             LPARAM lParam)
{
switch( msg )
{

    case WM_KEYDOWN:
        if( wParam == VK_ESCAPE ){
            if(MessageBox(0, "Are you sure you want to exit?",
                   "Exit Window", MB_YESNO | MB_ICONQUESTION) == IDYES)

                DestroyWindow(hwnd);
        }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
    return 0;
}
    //return the message for windows to handle it
    return DefWindowProc(hwnd,
             msg,
             wParam,
             lParam);
}
