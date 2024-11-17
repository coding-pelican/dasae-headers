// clang test_terminal_win32.c -o test_terminal_win32.exe -lgdi32

#include <stdio.h>
#include <windows.h>

#define unused(x) (void)(x)

#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (400)
#define FONT_WIDTH    (8)
#define FONT_HEIGHT   (16)
#define BUFFER_WIDTH  (80)
#define BUFFER_HEIGHT (25)

HWND  hwnd;
HDC   hdc;
HFONT hFont;
char  buffer[BUFFER_HEIGHT][BUFFER_WIDTH];

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void             InitializeBuffer();
void             DrawBuffer(HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // NOLINT
    unused(hInstance);
    unused(hPrevInstance);
    unused(lpCmdLine);
    unused(nShowCmd);
    // Register the window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc      = { 0 };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window
    hwnd = CreateWindowEx(
        0,                      // Optional window styles
        CLASS_NAME,             // Window class
        "Terminal-like Window", // Window text
        WS_OVERLAPPEDWINDOW,    // Window style

        // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nShowCmd);

    // Create font
    hFont = CreateFont(FONT_HEIGHT, FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");

    // Initialize buffer
    InitializeBuffer();

    // Run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        SelectObject(hdc, hFont);
        SetTextColor(hdc, Rgb(255, 255, 255));
        SetBkColor(hdc, Rgb(0, 0, 0));

        DrawBuffer(hdc);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CHAR: {
        // Handle key presses here
        char c = (char)wParam;
        unused(c);
        // For example, you could add the character to your buffer
        // and call InvalidateRect(hwnd, NULL, TRUE) to trigger a redraw
        return 0;
    }

    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InitializeBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {
            buffer[y][x] = ' ';
        }
    }

    // Add some sample text
    const char* text = "Welcome to the 80x25 terminal-like window!";
    for (int i = 0; text[i] && i < BUFFER_WIDTH; ++i) {
        buffer[0][i] = text[i];
    }
}

void DrawBuffer(HDC hdc) {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        TextOut(hdc, 0, y * FONT_HEIGHT, buffer[y], BUFFER_WIDTH);
    }
}
