#include <stdio.h>
#include <wchar.h>
#include <windows.h>

void enableANSI() {
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void printRgb(int r, int g, int b, const wchar_t* text) {
    wprintf(L"\033[38;2;%d;%d;%dm%ls\033[0m", r, g, b, text);
}

int main() {
    // Set console output to UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // Enable ANSI escape sequence processing
    enableANSI();
    (void)getchar();

    // Print colored Unicode block
    printRgb(255, 100, 0, L"▀ This is an orange upper half block\n");
    printRgb(0, 255, 0, L"▀ This is a green upper half block\n");
    printRgb(0, 100, 255, L"▀ This is a light blue upper half block\n");
    (void)getchar();

    // Print normal Unicode block
    wprintf(L"This is a normal upper half block: ▀\n");
    (void)getchar();

    return 0;
}
