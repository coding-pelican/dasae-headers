#include <windows.h>
#include <stdio.h>

// Function to set up console for mouse input
void enableMouseInput() {
    HANDLE hInput   = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  prevMode = 0;
    GetConsoleMode(hInput, &prevMode);

    // Enable extended input flags
    DWORD newMode = prevMode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    newMode &= ~ENABLE_QUICK_EDIT_MODE; // Disable Quick Edit mode which can interfere with mouse input
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

// Function to get mouse position in console coordinates
COORD getMousePosition(MOUSE_EVENT_RECORD mer) {
    return mer.dwMousePosition;
}

// Function to process mouse events
void handleMouseEvent(MOUSE_EVENT_RECORD mer) {
    COORD pos = getMousePosition(mer);

    switch (mer.dwEventFlags) {
    case 0: // Mouse click
        if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
            printf("Left click at position: %d, %d\n", pos.X, pos.Y);
        }
        if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
            printf("Right click at position: %d, %d\n", pos.X, pos.Y);
        }
        break;

    case DOUBLE_CLICK:
        printf("Double click at position: %d, %d\n", pos.X, pos.Y);
        break;

    case MOUSE_MOVED:
        printf("Mouse moved to position: %d, %d\n", pos.X, pos.Y);
        break;

    case MOUSE_WHEELED:
        if ((int)mer.dwButtonState > 0) {
            printf("Mouse wheel scrolled up at position: %d, %d\n", pos.X, pos.Y);
        } else {
            printf("Mouse wheel scrolled down at position: %d, %d\n", pos.X, pos.Y);
        }
        break;
    }
}

int main() {
    // Enable mouse input
    enableMouseInput();

    printf("Mouse input enabled. Click anywhere in the console window.\n");
    printf("Press Ctrl+C to exit.\n\n");

    HANDLE       hInput        = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inBuffer[128] = { 0 };
    DWORD        numEvents     = 0;

    // Main event loop
    while (1) {
        // Wait for input events
        ReadConsoleInput(hInput, inBuffer, 128, &numEvents);

        // Process all received events
        for (DWORD i = 0; i < numEvents; ++i) {
            switch (inBuffer[i].EventType) {
            case MOUSE_EVENT:
                handleMouseEvent(inBuffer[i].Event.MouseEvent);
                break;

            case KEY_EVENT:
                // Optional: Handle keyboard events if needed
                break;
            }
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }
    }

    // Clean up (this won't be reached unless you modify the loop to have an exit condition)
    disableMouseInput();
    return 0;
}
