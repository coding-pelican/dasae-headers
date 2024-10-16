#include <corecrt.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

// Function to enable ANSI escape sequence processing
void EnableANSI() {
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}


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


const wchar* const UPPER_HALF_BLOCK = L"▀";

static inline void TerminalCursor_ResetColor() {
    wprintf(L"\033[0m");
}

static inline void TerminalCursor_SetColor(RGBA foreground, RGBA background) {
    wprintf(
        L"\033[38;2;%d;%d;%dm" // foreground
        L"\033[48;2;%d;%d;%dm", // background
        foreground.r,
        foreground.g,
        foreground.b,
        background.r,
        background.g,
        background.b
    );
}

static inline void Terminal_Print(const wchar* character) {
    wprintf(L"%ls", character);
}

static inline void Terminal_Clear() {
    wprintf(
        L"\033[2J" // clear screen
        L"\033[H" // move cursor to top left
    );
}

static inline void DrawPixel1x2Single(RGBA upper, RGBA lower) {
    TerminalCursor_SetColor(upper, lower);
    Terminal_Print(UPPER_HALF_BLOCK);
    TerminalCursor_ResetColor();
}


i32 main() {
    EnableANSI();
    Terminal_Clear();
    (void)getchar();
    DrawPixel1x2Single(RGBA_Red, RGBA_Blue);
    (void)getchar();
    Terminal_Clear();
    return 0;
}
