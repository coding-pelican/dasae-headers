#include <stdio.h>
#include <windows.h>

void set_console_size_and_font() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set console buffer and window size
    COORD bufferSize = { 80, 25 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, 79, 24 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    // Set font size
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize       = sizeof(cfi);
    cfi.nFont        = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 8;
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    // Set console mode to enable virtual terminal processing
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
}

int main() {
    set_console_size_and_font();

    // Clear the screen
    printf("\033[2J\033[H");

    // Your program code here
    printf("Console size set to 80x25 characters with 8x8 font.\n");
    printf("Press Enter to exit...");
    (void)getchar();

    return 0;
}
