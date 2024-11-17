// build `clang -xc check_unicode.c -o check_unicode`
// run with `./check_unicode`


#include <locale.h>
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

void printRgb(int r, int g, int b, const char* text) {
    printf("\033[38;2;%d;%d;%dm%s\033[0m", r, g, b, text);
}

int main() {
    // Set locale to UTF-8
    (void)setlocale(LC_ALL, ".UTF-8"); // chcp 65001

    // Set console output to UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // Enable ANSI escape sequence processing
    enableANSI();

    // Print diagnostic information
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
    printf("Active code page: %d\n", GetACP());
    printf("Output code page: %d\n", GetConsoleOutputCP());

    // Print colored Unicode block
    printRgb(255, 100, 0, "▀ This is an orange upper half block\n");
    printRgb(0, 255, 0, "▀ This is a green upper half block\n");
    printRgb(0, 100, 255, "▀ This is a light blue upper half block\n");

    // Print normal Unicode block
    printf("This is a normal upper half block: ▀\n");
    (void)getchar();

    return 0;
}
