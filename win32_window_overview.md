## Understanding WIN32 Window Creation
```
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
