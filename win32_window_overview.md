## Fully understanding WIN32 Window Creation

```
/******************************************************************************
 * SECTION 1: PREPROCESSOR & INCLUDES
 * Purpose: Tell the compiler what external code we need
 ******************************************************************************/
#define WIN32_MEAN_AND_LEAN  // Excludes rarely-used Windows APIs (faster compile)
#include <windows.h>         // Core Windows API functions
#include <shellapi.h>        // For commandLineToArgvW
#include <wingdi.h>          // Graphics Device Interface functions

/******************************************************************************
 * SECTION 2: GLOBAL VARIABLES & CONSTANTS
 * Purpose: Values accessible throughout the program
 ******************************************************************************/
LPCTSTR WndClassName = "firstwindow";  // Unique identifier for our window class
HWND hwnd = NULL;                       // Handle (pointer) to our window
const int Width  = 800;                 // Window width in pixels
const int Height = 600;                 // Window height in pixels

/******************************************************************************
 * SECTION 3: FUNCTION DECLARATIONS (Forward Declarations)
 * Purpose: Tell compiler these functions exist (defined later)
 ******************************************************************************/
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int messageloop();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/******************************************************************************
 * SECTION 4: PROGRAM ENTRY POINT - WinMain
 * Purpose: Where Windows starts your program (like main() in console apps)
 * 
 * Parameters:
 *   hInstance     - Handle to this program instance
 *   hPrevInstance - (Legacy, always NULL in modern Windows)
 *   lpCmdLine     - Command line arguments as a string
 *   nShowCmd      - How to display the window (maximized, minimized, etc.)
 ******************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Try to create and initialize the window
    if(!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
    {
        // If it fails, show error message and exit
        MessageBox(0, "Window Initialization - Failed", "Error", MB_OK);
        return 0;
    }
    
    // Window created successfully, start the message loop
    messageloop();
    
    return 0;
}

/******************************************************************************
 * SECTION 5: WINDOW INITIALIZATION FUNCTION
 * Purpose: Creates the actual window in two steps:
 *   Step A: Register a Window Class (the blueprint/recipe)
 *   Step B: Create a Window from that class (the actual window)
 ******************************************************************************/
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
    /*-------------------------------------------------------------------------
     * STEP A: DEFINE THE WINDOW CLASS (The Blueprint)
     * Think of this as creating a "template" for windows
     *------------------------------------------------------------------------*/
    WNDCLASSEX wc = {0};  // Create and zero-initialize the structure
    
    // Fill in the blueprint specifications:
    wc.cbSize        = sizeof(WNDCLASSEX);              // Size of this structure
    wc.style         = CS_HREDRAW | CS_VREDRAW;         // Redraw if size changes
    wc.lpfnWndProc   = WndProc;                         // Function that handles messages
    wc.cbClsExtra    = NULL;                            // Extra class memory (none)
    wc.cbWndExtra    = NULL;                            // Extra window memory (none)
    wc.hInstance     = hInstance;                       // Handle to this program
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);     // Window icon (Windows logo)
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);     // Mouse cursor (arrow)
    wc.hbrBackground = CreateSolidBrush(RGB(245, 166, 39)); // Orange background
    wc.lpszMenuName  = NULL;                            // No menu bar
    wc.lpszClassName = WndClassName;                    // Name to identify this class
    wc.hIconSm       = LoadIcon(NULL, IDI_WINLOGO);     // Small icon (taskbar)
    
    // Register the blueprint with Windows
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    
    /*-------------------------------------------------------------------------
     * STEP B: CREATE THE ACTUAL WINDOW (Using the Blueprint)
     * This creates a real window based on the class we registered
     *------------------------------------------------------------------------*/
    hwnd = CreateWindowEx(
        0,                      // Extended window style (none)
        WndClassName,           // Class name (links to our blueprint above)
        "Active Window",        // Window title (text in title bar)
        WS_OVERLAPPEDWINDOW,    // Window style (standard window with borders, etc.)
        CW_USEDEFAULT,          // X position (let Windows decide)
        CW_USEDEFAULT,          // Y position (let Windows decide)
        width,                  // Width in pixels
        height,                 // Height in pixels
        NULL,                   // Parent window (none - this is top-level)
        NULL,                   // Menu handle (none)
        hInstance,              // Handle to this program
        NULL                    // Additional data (none)
    );
    
    // Check if window creation succeeded
    if (!hwnd)
    {
        MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    
    // Make the window visible and refresh it
    ShowWindow(hwnd, ShowWnd);    // Display the window
    UpdateWindow(hwnd);            // Send WM_PAINT message to draw window
    
    return true;
}

/******************************************************************************
 * SECTION 6: MESSAGE LOOP
 * Purpose: The "heartbeat" of the program - continuously checks for and 
 *          processes messages from Windows (mouse clicks, key presses, etc.)
 * 
 * How it works:
 *   1. Windows puts messages in a queue (like a todo list)
 *   2. We check the queue repeatedly
 *   3. When a message arrives, we process it
 *   4. Loop continues until WM_QUIT message received
 ******************************************************************************/
int messageloop()
{
    MSG msg;                           // Structure to hold message data
    ZeroMemory(&msg, sizeof(MSG));     // Clear the message structure
    
    while(true)  // Infinite loop (exits when WM_QUIT received)
    {
        // Check if there's a message waiting (non-blocking check)
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // If the message is WM_QUIT, exit the loop
            if (msg.message == WM_QUIT)
                break;
            
            TranslateMessage(&msg);    // Translate keyboard messages
            DispatchMessage(&msg);     // Send message to WndProc for handling
        }
        else
        {
            // No messages waiting - this is where you'd put:
            //   - Game logic updates
            //   - Rendering code
            //   - Physics calculations
            //   - etc.
        }
    }
    
    return (int)msg.wParam;  // Return exit code
}

/******************************************************************************
 * SECTION 7: WINDOW PROCEDURE (Message Handler)
 * Purpose: The "brain" of the window - decides what to do for each message
 * 
 * This function is called by Windows whenever something happens to your window.
 * Think of it as an event handler that responds to user actions and system events.
 * 
 * Parameters:
 *   hwnd   - Which window the message is for
 *   msg    - What type of message (WM_KEYDOWN, WM_PAINT, WM_DESTROY, etc.)
 *   wParam - Additional info (depends on message type)
 *   lParam - More additional info (depends on message type)
 ******************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)  // Check what type of message we received
    {
        /*---------------------------------------------------------------------
         * MESSAGE TYPE 1: Key Press
         * Triggered when user presses a key on the keyboard
         *--------------------------------------------------------------------*/
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE)  // Was it the ESC key?
            {
                // Ask user if they really want to quit
                if(MessageBox(0, "Are you sure you want to exit?",
                    "Exit Window", MB_YESNO | MB_ICONQUESTION) == IDYES)
                    DestroyWindow(hwnd);  // Close the window
            }
            return 0;
        
        /*---------------------------------------------------------------------
         * MESSAGE TYPE 2: Window Destruction
         * Triggered when the window is being destroyed (closed)
         *--------------------------------------------------------------------*/
        case WM_DESTROY:
            PostQuitMessage(0);  // Tell message loop to exit (sends WM_QUIT)
            return 0;
    }
    
    /*-------------------------------------------------------------------------
     * DEFAULT HANDLER
     * For any messages we don't handle above, let Windows handle them
     * (e.g., window resizing, minimizing, moving, painting, etc.)
     *------------------------------------------------------------------------*/
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/******************************************************************************
 * CONCEPTUAL FLOW OF THE PROGRAM:
 * 
 * 1. Windows calls WinMain() to start your program
 *    ↓
 * 2. WinMain() calls InitializeWindow()
 *    ↓
 * 3. InitializeWindow() does two things:
 *    a) Registers a window class (the blueprint)
 *    b) Creates a window from that class (the actual window)
 *    ↓
 * 4. WinMain() calls messageloop()
 *    ↓
 * 5. messageloop() runs forever, checking for messages:
 *    - When message arrives → sends to WndProc()
 *    - WndProc() handles the message
 *    - Loop continues...
 *    ↓
 * 6. When user closes window:
 *    - WndProc() receives WM_DESTROY
 *    - Calls PostQuitMessage(0)
 *    - messageloop() receives WM_QUIT
 *    - Loop exits
 *    ↓
 * 7. Program ends
 * 
 ******************************************************************************/
```
