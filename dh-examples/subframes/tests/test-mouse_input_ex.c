#include <windows.h>
#include <stdio.h>

typedef struct {
    int x; // X coordinate in pixel space
    int y; // Y coordinate in double-height space
} PIXEL_COORD;

// Function to get console information
void printConsoleMetrics() {
    HANDLE                     hConsole   = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
    CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

    printf("\nConsole Metrics:\n");
    printf("Buffer Size: %d x %d\n", bufferInfo.dwSize.X, bufferInfo.dwSize.Y);
    printf("Window Size: %d x %d\n", bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
    printf("Font Size: %d x %d\n", fontInfo.dwFontSize.X, fontInfo.dwFontSize.Y);
    printf("Window Position: (%d,%d) to (%d,%d)\n", bufferInfo.srWindow.Left, bufferInfo.srWindow.Top, bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom);
}

// Function to set up console for mouse input
void enableMouseInput() {
    HANDLE hInput   = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  prevMode = 0;
    GetConsoleMode(hInput, &prevMode);

    // Enable mouse input and disable QuickEdit
    DWORD newMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    newMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hInput, newMode);
}

// Function to disable mouse input
void disableMouseInput() {
    HANDLE hInput   = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  prevMode = 0;
    GetConsoleMode(hInput, &prevMode);

    // Restore previous mode without mouse input
    DWORD newMode = prevMode & ~ENABLE_MOUSE_INPUT;
    newMode |= ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hInput, newMode);
}

// Function to convert mouse event to pixel coordinates
PIXEL_COORD getPixelPosition(MOUSE_EVENT_RECORD mer) {
    PIXEL_COORD                pos        = { 0 };
    HANDLE                     hConsole   = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
    CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

    // Get raw mouse position
    POINT mousePos = { 0 };
    GetCursorPos(&mousePos);

    // Get console window handle and position
    HWND consoleWnd  = GetConsoleWindow();
    RECT consoleRect = { 0 };
    GetWindowRect(consoleWnd, &consoleRect);

    // Calculate relative position
    int cellHeight = fontInfo.dwFontSize.Y;
    int relativeY  = mousePos.y - consoleRect.top;

    // Calculate cell and sub-cell position
    int consoleY      = mer.dwMousePosition.Y;
    int subCellOffset = (relativeY % cellHeight) >= (cellHeight / 2) ? 1 : 0;

    // Convert to pixel space
    pos.x = mer.dwMousePosition.X;
    pos.y = (consoleY * 2) + subCellOffset;

    // Debug output
    printf("Debug: MouseY=%d, RelY=%d, ConsoleY=%d, CellHeight=%d, SubOffset=%d, FinalY=%d\n", mousePos.y, relativeY, consoleY, cellHeight, subCellOffset, pos.y);

    return pos;
}

void handleMouseEvent(MOUSE_EVENT_RECORD mer) {
    PIXEL_COORD                pos      = getPixelPosition(mer);
    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);

    // Make sure coordinates are within bounds
    if (pos.x >= 0 && pos.x < bufferInfo.dwSize.X && pos.y >= 0 && pos.y < bufferInfo.dwSize.Y * 2) {

        // Debug information
        printf("Debug: ConsoleY=%d, PixelY=%d, Flags=0x%08lx\n", mer.dwMousePosition.Y, pos.y, mer.dwEventFlags);

        switch (mer.dwEventFlags) {
        case 0: // Mouse click
            if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                printf("Left click at pixel position: %d, %d\n", pos.x, pos.y);
            }
            if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                printf("Right click at pixel position: %d, %d\n", pos.x, pos.y);
            }
            break;

        case MOUSE_MOVED:
            printf("Mouse at pixel position: %d, %d\n", pos.x, pos.y);
            break;

        case MOUSE_WHEELED:
            if ((int)mer.dwButtonState > 0) {
                printf("Mouse wheel up at pixel position: %d, %d\n", pos.x, pos.y);
            } else {
                printf("Mouse wheel down at pixel position: %d, %d\n", pos.x, pos.y);
            }
            break;
        }
    }
}

int main() {
    enableMouseInput();
    printConsoleMetrics();

    printf("Mouse input enabled. Click anywhere in the console window.\n");
    printf("Press Ctrl+C to exit.\n\n");

    HANDLE       hInput        = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inBuffer[128] = { 0 };
    DWORD        numEvents     = 0;

    while (1) {
        ReadConsoleInput(hInput, inBuffer, 128, &numEvents);

        for (DWORD i = 0; i < numEvents; i++) {
            if (inBuffer[i].EventType == MOUSE_EVENT) {
                handleMouseEvent(inBuffer[i].Event.MouseEvent);
            }
        }
        if (GetAsyncKeyState('I')) {
            printConsoleMetrics();
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }
        // if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('C')) {
        //     break;
        // }
    }

    disableMouseInput();
    getchar();
    return 0;
}
