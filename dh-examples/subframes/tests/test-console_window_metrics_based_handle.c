#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

// Helper function to find the terminal window
HWND FindTerminalWindow() {
    // Try the standard method first
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow != NULL) {
        printf("Found console window using GetConsoleWindow()\n");
        return consoleWindow;
    }

    printf("GetConsoleWindow() failed, trying alternative methods...\n");

    // Try to find Windows Terminal window
    HWND terminalWindow = FindWindowW(L"CASCADIA_HOSTING_WINDOW_CLASS", NULL);
    if (terminalWindow != NULL) {
        printf("Found Windows Terminal window\n");
        return terminalWindow;
    }

    // Try to find console window by class
    terminalWindow = FindWindowW(L"ConsoleWindowClass", NULL);
    if (terminalWindow != NULL) {
        printf("Found console window by class\n");
        return terminalWindow;
    }

    // Try to find active window as last resort
    printf("Trying to use the foreground window - please click on your terminal...\n");
    Sleep(2000); // Give user time to click on terminal
    terminalWindow = GetForegroundWindow();

    return terminalWindow;
}

// Helper function to find a character-based control within window
HWND FindConsoleControl(HWND parentWindow) {
    // First try to find RichEdit control (Windows 11 often uses this)
    HWND control = FindWindowExW(parentWindow, NULL, L"RICHEDIT50W", NULL);
    if (control != NULL) {
        printf("Found RICHEDIT50W control\n");
        return control;
    }

    // Try ConsoleWindowClass
    control = FindWindowExW(parentWindow, NULL, L"ConsoleWindowClass", NULL);
    if (control != NULL) {
        printf("Found ConsoleWindowClass control\n");
        return control;
    }

    // Try the console host window
    control = FindWindowExW(parentWindow, NULL, L"CASCADIA_HOSTING_WINDOW_CLASS", NULL);
    if (control != NULL) {
        printf("Found CASCADIA_HOSTING_WINDOW_CLASS control\n");
        return control;
    }

    // If no specific control found, just use the parent
    return parentWindow;
}

// Find the actual rendering position of character at 0,0
BOOL GetTerminalCellPosition(HWND terminalWindow, HWND controlWindow, POINT* cellPosition) {
    // Start with client origin
    cellPosition->x = 0;
    cellPosition->y = 0;

    // Convert to screen coordinates
    if (!ClientToScreen(controlWindow, cellPosition)) {
        printf("Failed to convert client to screen coordinates\n");
        return FALSE;
    }

    // Try to account for padding in modern terminals
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        // Get font information
        CONSOLE_FONT_INFOEX fontInfoEx;
        fontInfoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        if (GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfoEx)) {
            // Modern terminals often have padding
            // This is an estimate - actual values might vary
            cellPosition->x += fontInfoEx.dwFontSize.X / 4;  // Horizontal padding estimate
            cellPosition->y += fontInfoEx.dwFontSize.Y / 4;  // Vertical padding estimate
        }
    }

    return TRUE;
}

int main() {
    // Find the terminal window
    HWND terminalWindow = FindTerminalWindow();
    if (terminalWindow == NULL) {
        printf("Error: Could not find terminal window.\n");
        return 1;
    }

    // Find the console control
    HWND consoleControl = FindConsoleControl(terminalWindow);

    // Get window rectangle (entire window including non-client area)
    RECT windowRect;
    if (!GetWindowRect(terminalWindow, &windowRect)) {
        printf("Error: Could not get window rectangle.\n");
        return 1;
    }

    // Get client rectangle (area inside the window where content is displayed)
    RECT clientRect;
    if (!GetClientRect(terminalWindow, &clientRect)) {
        printf("Error: Could not get client rectangle.\n");
        return 1;
    }

    // Get control rectangle (if different from terminalWindow)
    RECT controlRect = {0};
    if (consoleControl != terminalWindow) {
        if (!GetClientRect(consoleControl, &controlRect)) {
            printf("Warning: Could not get control rectangle, using client rectangle.\n");
            controlRect = clientRect;
        }
    } else {
        controlRect = clientRect;
    }

    // Convert client origin to screen coordinates
    POINT clientOrigin = {0, 0};
    if (!ClientToScreen(terminalWindow, &clientOrigin)) {
        printf("Error: Could not convert client to screen coordinates.\n");
        return 1;
    }

    // Convert control origin to screen coordinates
    POINT controlOrigin = {0, 0};
    if (consoleControl != terminalWindow) {
        if (!ClientToScreen(consoleControl, &controlOrigin)) {
            printf("Warning: Could not convert control to screen coordinates.\n");
            controlOrigin = clientOrigin;
        }
    } else {
        controlOrigin = clientOrigin;
    }

    // Calculate the window metrics
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    int controlWidth = controlRect.right - controlRect.left;
    int controlHeight = controlRect.bottom - controlRect.top;

    // Calculate border and titlebar sizes
    int horizontalBorderWidth = (windowWidth - clientWidth) / 2;
    int verticalBorderHeight = (windowHeight - clientHeight - (clientOrigin.y - windowRect.top)) / 2;
    int titlebarHeight = clientOrigin.y - windowRect.top;

    // Try to get console buffer info
    bool hasConsoleInfo = false;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        hasConsoleInfo = true;
    }

    // Try to get font info
    bool hasFontInfo = false;
    CONSOLE_FONT_INFOEX fontInfoEx;
    fontInfoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfoEx)) {
        hasFontInfo = true;
    }

    // Calculate the position of the 0,0 character cell
    POINT cellPosition;
    bool hasCellPosition = GetTerminalCellPosition(terminalWindow, consoleControl, &cellPosition);

    // Print out all the metrics
    printf("Window Metrics for Terminal in Windows 11:\n");
    printf("----------------------------------------\n");
    printf("Terminal Window Handle: 0x%p\n", terminalWindow);
    printf("Console Control Handle: 0x%p\n", consoleControl);
    printf("\n");
    printf("Full Window Size: %d x %d pixels\n", windowWidth, windowHeight);
    printf("Client Area Size: %d x %d pixels\n", clientWidth, clientHeight);
    if (consoleControl != terminalWindow) {
        printf("Control Area Size: %d x %d pixels\n", controlWidth, controlHeight);
    }
    printf("Horizontal Border Width: %d pixels\n", horizontalBorderWidth);
    printf("Vertical Border Height: %d pixels\n", verticalBorderHeight);
    printf("Titlebar Height: %d pixels\n", titlebarHeight);
    printf("\n");
    printf("Window Position (top-left): (%d, %d)\n", windowRect.left, windowRect.top);
    printf("Client Area Position (top-left): (%d, %d)\n", clientOrigin.x, clientOrigin.y);
    if (consoleControl != terminalWindow) {
        printf("Control Area Position (top-left): (%d, %d)\n", controlOrigin.x, controlOrigin.y);
    }

    if (hasCellPosition) {
        printf("Character Cell 0,0 Position: (%d, %d)\n", cellPosition.x, cellPosition.y);
    }

    if (hasFontInfo) {
        printf("\n");
        printf("Font Information:\n");
        printf("Font Size: %d x %d pixels\n", fontInfoEx.dwFontSize.X, fontInfoEx.dwFontSize.Y);
        wprintf(L"Font Face: %s\n", fontInfoEx.FaceName);
    }

    if (hasConsoleInfo) {
        SMALL_RECT windowSize = csbi.srWindow;
        printf("\n");
        printf("Console Buffer Info:\n");
        printf("Buffer Size: %d x %d characters\n", csbi.dwSize.X, csbi.dwSize.Y);
        printf("Window Size: %d x %d characters\n",
               windowSize.Right - windowSize.Left + 1,
               windowSize.Bottom - windowSize.Top + 1);
        printf("Cursor Position: (%d, %d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
    }

    // Get additional information about the window
    printf("\n");
    printf("Additional Window Information:\n");

    // Get window style flags
    LONG style = GetWindowLong(terminalWindow, GWL_STYLE);
    printf("Window Style: 0x%lX\n", style);

    // Get extended style flags
    LONG exStyle = GetWindowLong(terminalWindow, GWL_EXSTYLE);
    printf("Extended Style: 0x%lX\n", exStyle);

    // Get class name
    wchar_t className[256] = {0};
    GetClassNameW(terminalWindow, className, 256);
    wprintf(L"Window Class: %s\n", className);

    // Get window text
    wchar_t windowText[256] = {0};
    GetWindowTextW(terminalWindow, windowText, 256);
    wprintf(L"Window Title: %s\n", windowText);

    // Enumerate child windows
    printf("\nChild Windows:\n");
    EnumChildWindows(terminalWindow,
        [](HWND child, LPARAM lParam) -> BOOL {
            wchar_t childClass[256] = {0};
            GetClassNameW(child, childClass, 256);

            RECT childRect;
            GetWindowRect(child, &childRect);

            wprintf(L"  Child 0x%p: Class=%s, Rect=(%d,%d,%d,%d)\n",
                child, childClass,
                childRect.left, childRect.top,
                childRect.right-childRect.left, childRect.bottom-childRect.top);

            return TRUE;  // Continue enumeration
        },
        0);

    // Wait for user input before closing
    printf("\nPress Enter to exit...");
    getchar();

    return 0;
}
