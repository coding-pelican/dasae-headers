#include <stdio.h>
#include <windows.h>

// Function to enable ANSI escape sequence processing
void Terminal_EnableANSI() {
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Function to print text in Rgb color
void printRgb(int r, int g, int b, const char* text) {
    printf("\033[38;2;%d;%d;%dm%s\033[0m", r, g, b, text);
}

int main() {
    (void)getchar();
    // Enable ANSI escape sequence processing
    Terminal_EnableANSI();

    // Print some colored text
    printRgb(255, 100, 0, "This is orange text\n");
    printRgb(0, 255, 0, "This is green text\n");
    printRgb(0, 100, 255, "This is light blue text\n");

    (void)getchar();
    return 0;
}
