#include <assert.h>
#include <conio.h>
#include <corecrt.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   usize;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ssize_t isize;

typedef wchar_t wchar;


typedef struct RGBA RGBA;
struct RGBA {
    union {
        u8 rgba[4];
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
    };
};
#define RGBA_from(...) (                  \
    (RGBA){ { .rgba = { __VA_ARGS__ } } } \
)
#define RGBA_fromRGB(...) \
    RGBA_from(__VA_ARGS__, 255)

const RGBA RGBA_Black = RGBA_fromRGB(0, 0, 0);
const RGBA RGBA_White = RGBA_fromRGB(255, 255, 255);
const RGBA RGBA_Red   = RGBA_fromRGB(255, 0, 0);
const RGBA RGBA_Green = RGBA_fromRGB(0, 255, 0);
const RGBA RGBA_Blue  = RGBA_fromRGB(0, 0, 255);


void TerminalCursor_ResetColor() {
    printf("\033[0m");
}

void TerminalCursor_SetColor(RGBA foreground, RGBA background) {
    printf(
        "\033[38;2;%d;%d;%dm" // foreground
        "\033[48;2;%d;%d;%dm", // background
        foreground.r,
        foreground.g,
        foreground.b,
        background.r,
        background.g,
        background.b
    );
}


//  Set locale to UTF-8 and set console output to UTF-8
void Terminal_EnsureLocaleUTF8() {
    static const char* const LocaleName = ".UTF-8";

    // Set locale to UTF-8
    const char* const settedLocale = setlocale(LC_ALL, LocaleName);
    if (!settedLocale) {
        perror("Failed to set locale to UTF-8\n");
        getch();
        return exit(1);
    }

    // Set console output to UTF-8
    const WINBOOL isSetted = SetConsoleOutputCP(CP_UTF8); // chcp 65001
    if (!isSetted) {
        perror("Failed to set console output to UTF-8\n");
        getch();
        return exit(1);
    }
}

// Enable ANSI escape sequence processing
void Terminal_EnableANSI() {
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Print diagnostic information for debugging
void Terminal_PrintDiagnosticInformation() {
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
    printf("Active code page: %d\n", GetACP());
    printf("Output code page: %d\n", GetConsoleOutputCP());
}

// Clear the terminal screen and move the cursor to the top left
void Terminal_Clear() {
    printf(
        "\033[2J" // clear screen
        "\033[H" // move cursor to top left
    );
}

void Terminal_Startup() {
    Terminal_EnsureLocaleUTF8();
    Terminal_EnableANSI();
    Terminal_PrintDiagnosticInformation();
    getch();
    Terminal_Clear();
}

void Terminal_Shutdown() {
    TerminalCursor_ResetColor();
    Terminal_Clear();
}

void Terminal_Print(const wchar* const string) {
    printf("%ls", string);
}

void DrawPixel1x2Single(RGBA upper, RGBA lower) {
    static const wchar* const Terminal_UpperHalfBlock = L"▀";

    TerminalCursor_SetColor(upper, lower);
    Terminal_Print(Terminal_UpperHalfBlock);
    TerminalCursor_ResetColor();
}


i32 main() {
    Terminal_Startup();

    DrawPixel1x2Single(RGBA_Red, RGBA_Blue);
    getch();

    Terminal_Shutdown();
    return 0;
}
