#include <windows.h>
#include <stdio.h>

int main() {
    // Get the handle to the console window
    HWND consoleWindow = GetForegroundWindow();
    if (consoleWindow == NULL) {
        printf("Error: Could not get console window handle.\n");
        return 1;
    }

    // Get window rectangle (entire window including non-client area)
    RECT windowRect;
    if (!GetWindowRect(consoleWindow, &windowRect)) {
        printf("Error: Could not get window rectangle.\n");
        return 1;
    }

    // Get client rectangle (area inside the window where content is displayed)
    RECT clientRect;
    if (!GetClientRect(consoleWindow, &clientRect)) {
        printf("Error: Could not get client rectangle.\n");
        return 1;
    }

    // Convert client coordinates to screen coordinates
    POINT clientOrigin = { 0, 0 };
    if (!ClientToScreen(consoleWindow, &clientOrigin)) {
        printf("Error: Could not convert client to screen coordinates.\n");
        return 1;
    }

    // Get information about the console screen buffer
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        printf("Error: Could not get console screen buffer info.\n");
        return 1;
    }

    // Calculate the window metrics
    int windowWidth  = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    int clientWidth  = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;

    // Calculate border and titlebar sizes
    int horizontalBorderWidth = (windowWidth - clientWidth) / 2;
    int verticalBorderHeight  = (windowHeight - clientHeight - (clientOrigin.y - windowRect.top)) / 2;
    int titlebarHeight        = clientOrigin.y - windowRect.top;

    // Calculate the position of the first character cell (0,0)
    // This depends on the font size
    CONSOLE_FONT_INFO fontInfo;
    if (!GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo)) {
        printf("Error: Could not get console font info.\n");
        return 1;
    }

    // Get more detailed font information including dimensions
    CONSOLE_FONT_INFOEX fontInfoEx;
    fontInfoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (!GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfoEx)) {
        printf("Error: Could not get detailed console font info.\n");
        return 1;
    }

    // Calculate the position of the 0,0 character cell in screen coordinates
    int cellX = clientOrigin.x;
    int cellY = clientOrigin.y;

    // Print out all the metrics
    printf("Window Metrics for Console in Windows 11:\n");
    printf("----------------------------------------\n");
    printf("Full Window Size: %d x %d pixels\n", windowWidth, windowHeight);
    printf("Client Area Size: %d x %d pixels\n", clientWidth, clientHeight);
    printf("Horizontal Border Width: %d pixels\n", horizontalBorderWidth);
    printf("Vertical Border Height: %d pixels\n", verticalBorderHeight);
    printf("Titlebar Height: %d pixels\n", titlebarHeight);
    printf("\n");
    printf("Window Position (top-left): (%ld, %ld)\n", windowRect.left, windowRect.top);
    printf("Client Area Position (top-left): (%ld, %ld)\n", clientOrigin.x, clientOrigin.y);
    printf("Character Cell 0,0 Position: (%d, %d)\n", cellX, cellY);
    printf("\n");
    printf("Font Information:\n");
    printf("Font Size: %d x %d pixels\n", fontInfoEx.dwFontSize.X, fontInfoEx.dwFontSize.Y);

    // Calculate the position of the rendering area
    SMALL_RECT windowSize = csbi.srWindow;
    printf("\n");
    printf("Console Window Info:\n");
    printf("Window Buffer Size: %d x %d characters\n", csbi.dwSize.X, csbi.dwSize.Y);
    printf("Window Size: %d x %d characters\n", windowSize.Right - windowSize.Left + 1, windowSize.Bottom - windowSize.Top + 1);
    printf("Cursor Position: (%d, %d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);

    // Wait for user input before closing
    printf("\nPress Enter to exit...");
    (void)getchar();

    return 0;
}
