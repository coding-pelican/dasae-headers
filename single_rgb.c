/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    single_rgb.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-16 (date of creation)
 * @updated 2024-10-18 (date of last update)
 * @version v0.1.1
 * @ingroup test_terminal
 * @prefix  NONE
 *
 * @brief   Source for rgb print testing the terminal
 * @details Implements terminal color visualization tools:
 * - RGB and HSL color space conversions
 * - ASCII character-based color rendering
 * - ANSI escape sequence color output
 * - RGB channel palettes and HSL gradient displays
 * - Efficient double-buffering for smooth rendering
 * - Unit tests for color conversion accuracy
 * - Cross-platform terminal setup and color support
 */


// wt --size 80,25 -d . cmd /c .\single_rgb

#include <assert.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef _WIN32
#  include <conio.h>
#  include <corecrt.h>
#  include <windows.h>
#else
#  include <fcntl.h>
#  include <termios.h>
#  include <unistd.h>

// Check if a key has been pressed
int kbhit() {
    struct termios oldTermios = { 0 };
    struct termios newTermios = { 0 };

    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
    int oldFlag = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFlag | O_NONBLOCK);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
    fcntl(STDIN_FILENO, F_SETFL, oldFlag);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Get a single character from the keyboard without waiting for a newline
int getch() { return getchar(); }
#endif


#define PP_UnusedParam(_param) ((void)(_param))
#define PP_IgnoreReturn(_func) (void)_func
#define PtrT(_Type)            _Type*
#define RefT(_Type)            _Type* const
#define PP_Swap(_Type, _a, _b)   \
    do {                         \
        PtrT(_Type) __a = &(_a); \
        PtrT(_Type) __b = &(_b); \
        _Type __t       = *__a;  \
        *__a            = *__b;  \
        *__b            = __t;   \
    } while (false)


#define Terminal_Width  (80)
#define Terminal_Height (25)
#define Terminal_Size   (Terminal_Width * Terminal_Height)

#define Terminal_FontWidth  (6)
#define Terminal_FontHeight (Terminal_FontWidth * 2)

#define Display_Width  (Terminal_Width)
#define Display_Height (Terminal_Height * 2)
#define Display_Size   (Display_Width * Display_Height)


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
static f64 Hue_to_RGB(f64 p, f64 q, f64 t) {
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
        r     = Hue_to_RGB(p, q, h + 1.0 / 3.0);
        g     = Hue_to_RGB(p, q, h);
        b     = Hue_to_RGB(p, q, h - 1.0 / 3.0);
    }

    return RGBA_fromRGB(
        (u8)f64_clamp(r * 255, 0, 255),
        (u8)f64_clamp(g * 255, 0, 255),
        (u8)f64_clamp(b * 255, 0, 255)
    );
}


f64 sRGB_to_linear(f64 srgb) {
    if (srgb <= 0.04045) {
        return srgb / 12.92;
    }
    return pow((srgb + 0.055) / 1.055, 2.4);
}

f64 linear_to_sRGB(f64 linear) {
    if (linear <= 0.0031308) {
        return 12.92 * linear;
    }
    return 1.055 * pow(linear, 1 / 2.4) - 0.055;
}

RGBA HSL_to_sRGBA(HSL color) {
    RGBA linear = HSL_to_RGBA(color);
    return RGBA_fromRGB(
        (u8)(linear_to_sRGB(linear.r / 255.0) * 255),
        (u8)(linear_to_sRGB(linear.g / 255.0) * 255),
        (u8)(linear_to_sRGB(linear.b / 255.0) * 255)
    );
}

RGBA HSL_to_liner(HSL color) {
    RGBA linear = HSL_to_RGBA(color);
    return RGBA_fromRGB(
        (u8)(sRGB_to_linear(linear.r / 255.0) * 255),
        (u8)(sRGB_to_linear(linear.g / 255.0) * 255),
        (u8)(sRGB_to_linear(linear.b / 255.0) * 255)
    );
}


#define UNIT_TESTS
#ifdef UNIT_TESTS
// Helper function to compare floating point numbers
bool f64_approx_equal(f64 a, f64 b, f64 epsilon) {
    return fabs(a - b) < epsilon;
}

// Updated helper function to compare RGBA colors with tolerance
bool RGBA_approx_equal(RGBA a, RGBA b, i32 tolerance) {
    return abs(a.r - b.r) <= tolerance &&
           abs(a.g - b.g) <= tolerance &&
           abs(a.b - b.b) <= tolerance &&
           abs(a.a - b.a) <= tolerance;
}

// Helper function to compare HSL colors
bool HSL_approx_equal(HSL a, HSL b, f64 epsilon) {
    return f64_approx_equal(a.h, b.h, epsilon) &&
           f64_approx_equal(a.s, b.s, epsilon) &&
           f64_approx_equal(a.l, b.l, epsilon);
}


void TEST_RGB_to_HSL() {
    printf("Testing RGB to HSL conversion...\n");

    // Test black
    HSL black_hsl = RGBA_to_HSL(RGBA_Black);
    assert(HSL_approx_equal(black_hsl, HSL_from(0, 0, 0), 0.01));

    // Test white
    HSL white_hsl = RGBA_to_HSL(RGBA_White);
    assert(HSL_approx_equal(white_hsl, HSL_from(0, 0, 100), 0.01));

    // Test red
    HSL red_hsl = RGBA_to_HSL(RGBA_Red);
    assert(HSL_approx_equal(red_hsl, HSL_from(0, 100, 50), 0.01));

    // Test green
    HSL green_hsl = RGBA_to_HSL(RGBA_Green);
    assert(HSL_approx_equal(green_hsl, HSL_from(120, 100, 50), 0.01));

    // Test blue
    HSL blue_hsl = RGBA_to_HSL(RGBA_Blue);
    assert(HSL_approx_equal(blue_hsl, HSL_from(240, 100, 50), 0.01));

    printf("RGB to HSL conversion tests passed.\n");
}

void TEST_HSL_to_RGB() {
    printf("Testing HSL to RGB conversion...\n");

    // Test black
    RGBA black_rgb = HSL_to_RGBA(HSL_from(0, 0, 0));
    assert(RGBA_approx_equal(black_rgb, RGBA_Black, 1));

    // Test white
    RGBA white_rgb = HSL_to_RGBA(HSL_from(0, 0, 100));
    assert(RGBA_approx_equal(white_rgb, RGBA_White, 1));

    // Test red
    RGBA red_rgb = HSL_to_RGBA(HSL_from(0, 100, 50));
    assert(RGBA_approx_equal(red_rgb, RGBA_Red, 1));

    // Test green
    RGBA green_rgb = HSL_to_RGBA(HSL_from(120, 100, 50));
    assert(RGBA_approx_equal(green_rgb, RGBA_Green, 1));

    // Test blue
    RGBA blue_rgb = HSL_to_RGBA(HSL_from(240, 100, 50));
    assert(RGBA_approx_equal(blue_rgb, RGBA_Blue, 1));

    printf("HSL to RGB conversion tests passed.\n");
}

void TEST_RoundtripConversion() {
    printf("Testing roundtrip conversion...\n");

    // Test a variety of colors
    const RGBA colors[] = {
        RGBA_fromRGB(128, 64, 32), // Brown
        RGBA_fromRGB(255, 128, 0), // Orange
        RGBA_fromRGB(0, 255, 255), // Cyan
        RGBA_fromRGB(128, 0, 128), // Purple
        RGBA_fromRGB(192, 192, 192), // Silver
    };
    const int colorsNum = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < colorsNum; ++i) {
        RGBA original  = colors[i];
        HSL  hsl       = RGBA_to_HSL(original);
        RGBA converted = HSL_to_RGBA(hsl);

        if (!RGBA_approx_equal(original, converted, 1)) {
            printf("Conversion failed for color: R:%d G:%d B:%d\n", original.r, original.g, original.b);
            printf("Converted to: R:%d G:%d B:%d\n", converted.r, converted.g, converted.b);
            assert(0); // Force test failure
        }
    }

    printf("Roundtrip conversion tests passed.\n");
}
#endif


void TerminalCursor_ResetColor() {
    printf("\033[0m");
}

void TerminalCursor_SetColor(RGBA foreground, RGBA background) {
    // printf(
    //     "\033[38;2;%d;%d;%dm" // foreground
    //     "\033[48;2;%d;%d;%dm", // background
    //     foreground.r,
    //     foreground.g,
    //     foreground.b,
    //     background.r,
    //     background.g,
    //     background.b
    // );
    printf(
        "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm", // background
        foreground.r,
        foreground.g,
        foreground.b,
        background.r,
        background.g,
        background.b
    );
}


// Set locale to UTF-8
void Terminal_EnsureLocaleUTF8() {
    static const char* const locales[]  = { "en_US.UTF-8", "C.UTF-8", ".UTF-8", "" };
    static const int         localesNum = sizeof(locales) / sizeof(locales[0]);

    const char* settedLocale = NULL;

    for (int i = 0; i < localesNum; ++i) {
        settedLocale = setlocale(LC_ALL, locales[i]);
        if (settedLocale) {
            printf("Successfully set locale to: %s\n", settedLocale);
            break;
        }
    }

    if (!settedLocale) {
        printf("Warning: Failed to set UTF-8 locale. Using system default.\n");
        settedLocale = setlocale(LC_ALL, "");
        if (!settedLocale) {
            perror("Failed to set locale\n");
            // Not exiting here, as the program might still work
        }
    }

#ifdef _WIN32
    // Set console output to UTF-8 on Windows
    if (!SetConsoleOutputCP(CP_UTF8)) {
        perror("Failed to set console output to UTF-8\n");
        // Not exiting here, as the program might still work
    }
#endif
}


// Enable ANSI escape sequence processing
void Terminal_EnableANSI() {
#ifdef _WIN32
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#else
    // Not needed for Linux as it supports ANSI escape sequences by default
#endif
}

// Print diagnostic information for debugging
void Terminal_PrintDiagnosticInformation() {
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
#ifdef _WIN32
    printf("Active code page: %d\n", GetACP());
    printf("Output code page: %d\n", GetConsoleOutputCP());
#endif
}


// Clear the terminal screen and move the cursor to the top left
void Terminal_Clear() {
#ifdef _WIN32
    // system("cls"); // NOLINT
    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD                      topLeft  = (COORD){ 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen   = (CONSOLE_SCREEN_BUFFER_INFO){ 0 };
    DWORD                      written  = 0;

    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
#else
    system("clear"); // NOLINT
#endif
    // printf(
    //     "\033[2J" // clear screen
    //     "\033[H" // move cursor to top left
    // );
}

void Terminal_Bootup() {
    Terminal_EnsureLocaleUTF8();
    Terminal_EnableANSI();
    Terminal_PrintDiagnosticInformation();
    printf("Terminal setup complete.\n");
    printf("Press any key to continue...\n");
    getch();
    Terminal_Clear();
}

void Terminal_Shutdown() {
    TerminalCursor_ResetColor();
    Terminal_Clear();
}

void Terminal_Put(const wchar character) {
    printf("%lc", character);
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

void DrawPixel1x2SingleCharacter(RGBA upper, RGBA lower) {
    static const wchar Terminal_UpperHalfBlock = L'▀';

    TerminalCursor_SetColor(upper, lower);
    Terminal_Put(Terminal_UpperHalfBlock);
    TerminalCursor_ResetColor();
}

void DrawPixel1x2SingleCharacterDontResetColor(RGBA upper, RGBA lower) {
    static const wchar Terminal_UpperHalfBlock = L'▀';

    TerminalCursor_SetColor(upper, lower);
    Terminal_Put(Terminal_UpperHalfBlock);
}


void DrawRGBChannelPalette() {
    const f64 scalerByWidth = 255.0 / Display_Width;

    for (int i = 0; i < Display_Width; ++i) {
        const u8 scaled = (u8)scalerByWidth * i;
        DrawPixel1x2Single(
            RGBA_fromRGB(scaled, 0, 0),
            RGBA_fromRGB(0, scaled, 0)
        );
    }
    printf("\n");
    for (int i = 0; i < Display_Width; ++i) {
        const u8 scaled = (u8)scalerByWidth * i;
        DrawPixel1x2Single(
            RGBA_fromRGB(0, 0, scaled),
            RGBA_fromRGB(scaled, scaled, 0)
        );
    }
    printf("\n");
    for (int i = 0; i < Display_Width; ++i) {
        const u8 scaled = (u8)scalerByWidth * i;
        DrawPixel1x2Single(
            RGBA_fromRGB(scaled, 0, scaled),
            RGBA_fromRGB(0, scaled, scaled)
        );
    }
    printf("\n");
    for (int i = 0; i < Display_Width; ++i) {
        const u8 scaled = (u8)scalerByWidth * i;
        DrawPixel1x2Single(
            RGBA_fromRGB(scaled, scaled, scaled),
            RGBA_fromRGB(0, 0, 0)
        );
    }
    printf("\n");
}

void DrawRGBAToHSLConvertedTest() {
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
}

void DrawHSLGradientPalette() {
    RGBA buffer[Display_Size] = { 0 };

    const f64 saturation = 100.0;

    const f64 hueInterval       = 360.0 / Display_Width;
    const f64 lightnessInterval = 100.0 / Display_Height;

    for (i32 y = 0; y < Display_Height; ++y) {
        for (i32 x = 0; x < Display_Width; ++x) {
            f64 h                         = (f64)x * hueInterval;
            f64 l                         = (f64)y * lightnessInterval;
            HSL hsl                       = HSL_from(h, saturation, l);
            buffer[x + y * Display_Width] = HSL_to_RGBA(hsl);
        }
    }

    for (i32 y = 0; y < Display_Height; y += 2) {
        for (i32 x = 0; x < Display_Width; ++x) {
            RGBA upper = buffer[x + y * Display_Width];
            RGBA lower = buffer[x + (y + 1) * Display_Width];
            DrawPixel1x2Single(upper, lower);
        }
    }
}

void DrawHSLGradientPaletteCharacter() {
    RGBA buffer[Display_Size] = { 0 };

    const f64 saturation = 100.0;

    const f64 hueInterval       = 360.0 / Display_Width;
    const f64 lightnessInterval = 100.0 / Display_Height;

    for (i32 y = 0; y < Display_Height; ++y) {
        for (i32 x = 0; x < Display_Width; ++x) {
            f64 h                         = (f64)x * hueInterval;
            f64 l                         = (f64)y * lightnessInterval;
            HSL hsl                       = HSL_from(h, saturation, l);
            buffer[x + y * Display_Width] = HSL_to_RGBA(hsl);
        }
    }

    for (i32 y = 0; y < Display_Height; y += 2) {
        for (i32 x = 0; x < Display_Width; ++x) {
            RGBA upper = buffer[x + y * Display_Width];
            RGBA lower = buffer[x + (y + 1) * Display_Width];
            DrawPixel1x2SingleCharacter(upper, lower);
        }
    }
}

void DrawHSLGradientPaletteCharacterDontResetColor() {
    RGBA buffer[Display_Width * Display_Height] = { 0 };

    const f64 saturation = 100.0;

    const f64 hueInterval       = 360.0 / Display_Width;
    const f64 lightnessInterval = 100.0 / Display_Height;

    for (i32 y = 0; y < Display_Height; ++y) {
        for (i32 x = 0; x < Display_Width; ++x) {
            f64 h                         = (f64)x * hueInterval;
            f64 l                         = (f64)y * lightnessInterval;
            HSL hsl                       = HSL_from(h, saturation, l);
            buffer[x + y * Display_Width] = HSL_to_RGBA(hsl);
        }
    }

    for (i32 y = 0; y < Display_Height; y += 2) {
        for (i32 x = 0; x < Display_Width; ++x) {
            RGBA upper = buffer[x + y * Display_Width];
            RGBA lower = buffer[x + (y + 1) * Display_Width];
            DrawPixel1x2SingleCharacterDontResetColor(upper, lower);
        }
    }
}


#define Display_UnitPixel1x2Format            "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm▀"
#define Display_UnitPixel1x2FormatMaxCase     "\033[38;2;255;255;255;48;2;255;255;255m▀"
#define Display_UnitPixel1x2FormatMaxCaseSize (sizeof(Display_UnitPixel1x2FormatMaxCase) / sizeof(Display_UnitPixel1x2FormatMaxCase[0]))
#define Display_BufferSize                    ((usize)Terminal_Size * (usize)Display_UnitPixel1x2FormatMaxCaseSize)

static char  Display_frontBuffer[Display_BufferSize] = { 0 };
static char  Display_backBuffer[Display_BufferSize]  = { 0 };
static char* Display_bufferCurrent                   = Display_frontBuffer;
static usize Display_bufferCurrentSize               = 0;
static char* Display_bufferNext                      = Display_backBuffer;
static usize Display_bufferNextSize                  = 0;

void Display_Init() {
#ifdef _WIN32
    HANDLE     hConsoleOutput    = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSizeInitial = (SMALL_RECT){ 0, 0, 1, 1 };

    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSizeInitial);

    // void SetConsoleScreenBuffer()
    COORD dwSize = (COORD){ (SHORT)Terminal_Width, (SHORT)Terminal_Height };
    SetConsoleActiveScreenBuffer(hConsoleOutput);
    SetConsoleScreenBufferSize(hConsoleOutput, dwSize);

    // void SetConsoleFontSize()
    CONSOLE_FONT_INFOEX fontInfo = (CONSOLE_FONT_INFOEX){ 0 };
    fontInfo.cbSize              = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);
    fontInfo.dwFontSize.X = Terminal_FontWidth;
    fontInfo.dwFontSize.Y = Terminal_FontHeight;
    SetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);

    // void SetConsoleWindowSize()
    SMALL_RECT windowSize = (SMALL_RECT){ 0, 0, (SHORT)Terminal_Width - 1, (SHORT)Terminal_Height - 1 };
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSize);

    // void DisableConsoleCursor()
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
#endif
}

void Display_Clear() {
    memset(Display_bufferCurrent, 0, Display_bufferCurrentSize);
}

void Display_SwapBuffers() {
    PP_Swap(char*, Display_bufferCurrent, Display_bufferNext);
    PP_Swap(usize, Display_bufferCurrentSize, Display_bufferNextSize);
}

void Display_SetBufferFromColors(const RGBA colors[Display_Size]) {
    Display_Clear();

    i32 index = 0;
    for (i32 y = 0; y < Display_Height; y += 2) {
        for (i32 x = 0; x < Display_Width; ++x) {
            char displayFormat[Display_UnitPixel1x2FormatMaxCaseSize] = { 0 };

            const RGBA upper = colors[x + y * Display_Width];
            const RGBA lower = colors[x + (y + 1) * Display_Width];

            const i32 formattedSize = sprintf(
                displayFormat,
                Display_UnitPixel1x2Format,
                upper.r,
                upper.g,
                upper.b,
                lower.r,
                lower.g,
                lower.b
            );

            memcpy(Display_bufferNext + index, displayFormat, formattedSize);
            index += formattedSize;
        }
    }
    Display_bufferNext[index] = '\0';
    Display_bufferNextSize    = index;

    Display_SwapBuffers();
}

void Display_Render() {
    Terminal_Clear();
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  written  = 0;
    WriteConsoleA(hConsole, Display_bufferCurrent, (DWORD)Display_bufferCurrentSize, &written, NULL);
#else
    if (!fwrite(Display_buffer, sizeof(Display_bufferCurrent[0]), Display_bufferCurrentSize, stdout)) {
        assert(false);
    }
#endif
    TerminalCursor_ResetColor();
}

void DrawHSLGradientPaletteToDisplay() {
    RGBA colors[Display_Size] = { 0 };

    const f64 saturation = 100.0;

    const f64 hueInterval       = 360.0 / Display_Width;
    const f64 lightnessInterval = 100.0 / Display_Height;

    for (i32 y = 0; y < Display_Height; ++y) {
        for (i32 x = 0; x < Display_Width; ++x) {
            f64 h                         = (f64)x * hueInterval;
            f64 l                         = (f64)y * lightnessInterval;
            HSL hsl                       = HSL_from(h, saturation, l);
            colors[x + y * Display_Width] = HSL_to_RGBA(hsl);
        }
    }
    Display_SetBufferFromColors(colors);
}


i32 main() {
    Display_Init();
    Terminal_Bootup();

#ifdef UNIT_TESTS
    TEST_RGB_to_HSL();
    TEST_HSL_to_RGB();
    TEST_RoundtripConversion();
    getch();
    Terminal_Clear();
#endif

    DrawPixel1x2Single(RGBA_Red, RGBA_Green);
    DrawPixel1x2Single(RGBA_Blue, RGBA_White);
    DrawPixel1x2Single(RGBA_Red, RGBA_Red);
    DrawPixel1x2Single(RGBA_Green, RGBA_Green);
    DrawPixel1x2Single(RGBA_Blue, RGBA_Blue);
    getch();
    Terminal_Clear();

    DrawRGBChannelPalette();
    getch();
    Terminal_Clear();

    DrawRGBAToHSLConvertedTest();
    getch();
    Terminal_Clear();

    DrawHSLGradientPalette();
    getch();
    Terminal_Clear();

    DrawHSLGradientPaletteCharacter();
    getch();
    Terminal_Clear();

    DrawHSLGradientPaletteCharacterDontResetColor();
    getch();
    Terminal_Clear();

    DrawHSLGradientPaletteToDisplay();
    Display_Render();
    getch();
    DrawHSLGradientPaletteToDisplay();
    Display_Render();
    getch();
    DrawHSLGradientPaletteToDisplay();
    Display_Render();
    getch();

    Terminal_Shutdown();
    return 0;
}
