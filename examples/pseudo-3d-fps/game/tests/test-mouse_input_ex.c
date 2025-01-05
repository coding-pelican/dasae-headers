#include <windows.h>
#include <stdio.h>

// Structure to hold pixel-precise coordinates
typedef struct {
    int x; // X coordinate in pixel space
    int y; // Y coordinate in double-height space
} PIXEL_COORD;

// Function to set up console for mouse input
void enableMouseInput() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  prevMode;
    GetConsoleMode(hInput, &prevMode);

    // Enable mouse input and disable QuickEdit
    DWORD newMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    newMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hInput, newMode);
}

// Function to convert console coordinates to pixel coordinates
PIXEL_COORD getPixelPosition(MOUSE_EVENT_RECORD mer) {
    PIXEL_COORD pixel;

    // Convert to our double-height coordinate system
    pixel.x = mer.dwMousePosition.X;
    pixel.y = mer.dwMousePosition.Y * 2;

    return pixel;
}

// Function to process mouse events
void handleMouseEvent(MOUSE_EVENT_RECORD mer) {
    PIXEL_COORD pos = getPixelPosition(mer);

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

int main() {
    // Enable mouse input
    enableMouseInput();

    printf("Mouse input enabled. Click anywhere in the console window.\n");
    printf("Press Ctrl+C to exit.\n\n");

    HANDLE       hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inBuffer[128];
    DWORD        numEvents;

    while (1) {
        // Wait for input events
        ReadConsoleInput(hInput, inBuffer, 128, &numEvents);

        // Process all received events
        for (DWORD i = 0; i < numEvents; i++) {
            if (inBuffer[i].EventType == MOUSE_EVENT) {
                handleMouseEvent(inBuffer[i].Event.MouseEvent);
            }
        }
    }

    return 0;
}
