// wt --size 160,50 -d . cmd /k .\single_rgb.exe

#include <assert.h>
#include <conio.h>
#include <corecrt.h>
#include <locale.h>
#include <math.h>
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

typedef float_t  f32;
typedef double_t f64;

typedef wchar_t wchar;


#define f32_Epsilon (1e-6f)
#define f64_Epsilon (1e-15)

bool f64_eq(f64 a, f64 b) { return fabs(a - b) < f64_Epsilon; }
bool f64_neq(f64 a, f64 b) { return fabs(a - b) >= f64_Epsilon; }
bool f64_lt(f64 a, f64 b) { return (b - a) > f64_Epsilon; }
bool f64_gt(f64 a, f64 b) { return (a - b) > f64_Epsilon; }
bool f64_leq(f64 a, f64 b) { return (a - b) < f64_Epsilon; }
bool f64_geq(f64 a, f64 b) { return (b - a) < f64_Epsilon; }

f64 f64_min(f64 x, f64 y) { return f64_lt(x, y) ? x : y; }
f64 f64_max(f64 x, f64 y) { return f64_gt(x, y) ? x : y; }
f64 f64_clamp(f64 x, f64 low, f64 high) { return f64_min(f64_max(low, x), high); }
f64 f64_clamp01(f64 x) { return f64_clamp(x, 0.0f, 1.0f); }
f64 f64_wrap(f64 x, f64 low, f64 high) { return (high - low <= 0.0f) ? x : fmod(x - low, high - low) + low; }
f64 f64_wrap01(f64 x) { return f64_wrap(x, 0.0f, 1.0f); }


typedef struct RGBA RGBA;
struct RGBA {
    union {
        u8 rgba[4];
        struct {
            u8 r; // Red in [0,255]
            u8 g; // Green in [0,255]
            u8 b; // Blue in [0,255]
            u8 a; // Alpha in [0,255]
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


typedef struct HSL HSL;
struct HSL {
    union {
        f64 hsl[3];
        struct {
            f64 h; // Hue in [0,360]
            f64 s; // Saturation in [0,100]
            f64 l; // Lightness in [0,100]
        };
    };
};
#define HSL_from(...) (                 \
    (HSL){ { .hsl = { __VA_ARGS__ } } } \
)


// RGB to HSL conversion
HSL RGBA_to_HSL(RGBA color) {
    f64 r = color.r / 255.0;
    f64 g = color.g / 255.0;
    f64 b = color.b / 255.0;

    f64 max  = f64_max(f64_max(r, g), b);
    f64 min  = f64_min(f64_min(r, g), b);
    f64 diff = max - min;

    f64 h = 0;
    f64 s = 0;
    f64 l = (max + min) / 2;

    if (diff != 0) {
        s = l > 0.5 ? diff / (2 - max - min) : diff / (max + min);

        if (max == r) {
            h = (g - b) / diff + (g < b ? 6 : 0);
        } else if (max == g) {
            h = (b - r) / diff + 2;
        } else {
            h = (r - g) / diff + 4;
        }
        h /= 6;
    }
    return HSL_from(h * 360, s * 100, l * 100);
}

// Helper function for HSL to RGB conversion
static f64 hue_to_rgb(f64 p, f64 q, f64 t) {
    if (t < 0) {
        t += 1;
    }
    if (t > 1) {
        t -= 1;
    }
    if (t < 1. / 6) {
        return p + (q - p) * 6 * t;
    }
    if (t < 1. / 2) {
        return q;
    }
    if (t < 2. / 3) {
        return p + (q - p) * (2. / 3 - t) * 6;
    }
    return p;
}

// HSL to RGB conversion
RGBA HSL_to_RGBA(HSL color) {
    f64 h = color.h / 360.0;
    f64 s = color.s / 100.0;
    f64 l = color.l / 100.0;

    f64 r = NAN;
    f64 g = NAN;
    f64 b = NAN;

    if (s == 0.0) {
        r = g = b = l;
    } else {
        f64 q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        f64 p = 2.0 * l - q;
        r     = hue_to_rgb(p, q, h + 1.0 / 3.0);
        g     = hue_to_rgb(p, q, h);
        b     = hue_to_rgb(p, q, h - 1.0 / 3.0);
    }

    return RGBA_fromRGB(
        (u8)f64_clamp(r * 255, 0, 255),
        (u8)f64_clamp(g * 255, 0, 255),
        (u8)f64_clamp(b * 255, 0, 255)
    );
}


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

#define Terminal_Width  (80)
#define Terminal_Height (25)
#define Screen_Width    (Terminal_Width)
#define Screen_Height   (Terminal_Height * 2)

void DrawPalette() {
    RGBA buffer[Screen_Width * Screen_Height] = { 0 };

    f64 xScale = 360.0 / Screen_Width;
    f64 yScale = 100.0 / Screen_Height;
    for (i32 y = 0; y < Screen_Height; ++y) {
        for (i32 x = 0; x < Screen_Width; ++x) {
            f64 h                        = (f64)x * xScale;
            f64 l                        = (f64)y * yScale;
            HSL hsl                      = HSL_from(h, 100.0, l);
            buffer[x + y * Screen_Width] = HSL_to_RGBA(hsl);
        }
    }

    for (i32 y = 0; y < Screen_Height; y += 2) {
        for (i32 x = 0; x < Screen_Width; ++x) {
            RGBA upper = buffer[x + y * Screen_Width];
            RGBA lower = buffer[x + (y + 1) * Screen_Width];
            DrawPixel1x2Single(upper, lower);
        }
        printf("\n");
    }
}


i32 main() {
    Terminal_Startup();

    DrawPixel1x2Single(RGBA_Red, RGBA_Blue);
    getch();

    Terminal_Clear();
    DrawPixel1x2Single(RGBA_White, RGBA_White);
    DrawPixel1x2Single(RGBA_White, RGBA_White);
    DrawPixel1x2Single(RGBA_White, RGBA_White);
    DrawPixel1x2Single(RGBA_White, RGBA_White);
    DrawPixel1x2Single(RGBA_White, RGBA_White);
    getch();

    Terminal_Clear();
    f64 xScale = 255.0 / Screen_Width;
    for (int i = 0; i < Screen_Width; ++i) {
        DrawPixel1x2Single(
            RGBA_fromRGB(i * xScale, 0 * xScale, 0 * xScale),
            RGBA_fromRGB(0 * xScale, i * xScale, 0 * xScale)
        );
    }
    printf("\n");
    for (int i = 0; i < Screen_Width; ++i) {
        DrawPixel1x2Single(
            RGBA_fromRGB(0 * xScale, 0 * xScale, i * xScale),
            RGBA_fromRGB(i * xScale, i * xScale, 0 * xScale)
        );
    }
    printf("\n");
    for (int i = 0; i < Screen_Width; ++i) {
        DrawPixel1x2Single(
            RGBA_fromRGB(i * xScale, 0 * xScale, i * xScale),
            RGBA_fromRGB(0 * xScale, i * xScale, i * xScale)
        );
    }
    printf("\n");
    for (int i = 0; i < Screen_Width; ++i) {
        DrawPixel1x2Single(
            RGBA_fromRGB(i * xScale, i * xScale, i * xScale),
            RGBA_fromRGB(0 * xScale, 0 * xScale, 0 * xScale)
        );
    }
    printf("\n");
    getch();

    Terminal_Clear();
    DrawPixel1x2Single(RGBA_Red, RGBA_Red);
    DrawPixel1x2Single(RGBA_Red, RGBA_Red);
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Red)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Red)));
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Red)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Red)));
    printf("\n");
    DrawPixel1x2Single(RGBA_Green, RGBA_Green);
    DrawPixel1x2Single(RGBA_Green, RGBA_Green);
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Green)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Green)));
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Green)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Green)));
    printf("\n");
    DrawPixel1x2Single(RGBA_Blue, RGBA_Blue);
    DrawPixel1x2Single(RGBA_Blue, RGBA_Blue);
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Blue)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Blue)));
    DrawPixel1x2Single(HSL_to_RGBA(RGBA_to_HSL(RGBA_Blue)), HSL_to_RGBA(RGBA_to_HSL(RGBA_Blue)));
    printf("\n");
    getch();

    Terminal_Clear();
    DrawPalette();
    getch();

    Terminal_Shutdown();
    return 0;
}
