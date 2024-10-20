// build `clang -x c game_of_life.c -o game_of_life -O3 -static`
// run with `.\launcher game_of_life 160 50`


#include <assert.h>
#include <locale.h>
#include <math.h>
#include <processenv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#ifdef _WIN32
#  include <conio.h>
#  include <corecrt.h>
#  define NOMINMAX
#  include <windows.h>
#else
#  include <fcntl.h>
#  include <termios.h>
#  include <unistd.h>

// Check if a key has been pressed
int kbhit() {
    struct termios oldattr = { 0 };
    struct termios newattr = { 0 };

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    int oldFlag = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFlag | O_NONBLOCK);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    fcntl(STDIN_FILENO, F_SETFL, oldFlag);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Get a single character from the keyboard without waiting for a newline
int getch() {
    struct termios oldattr = { 0 };
    struct termios newattr = { 0 };

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}
#endif

#define UnusedValue(_param) ((void)(_param))
#define IgnoreReturn(_func) (void)_func
#define PtrT(_Type)         _Type*
#define RefT(_Type)         _Type* const
#define Swap(_Type, _a, _b)      \
    do {                         \
        PtrT(_Type) __a = &(_a); \
        PtrT(_Type) __b = &(_b); \
        _Type __t       = *__a;  \
        *__a            = *__b;  \
        *__b            = __t;   \
    } while (false)


typedef wchar_t wchar;

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


#define Terminal_Width  (80 * 2)
#define Terminal_Height (25 * 2)
#define Terminal_Size   (Terminal_Width * Terminal_Height)

#define Terminal_FontWidth  (6)
#define Terminal_FontHeight (Terminal_FontWidth * 2)

#define Display_Width  (Terminal_Width)
#define Display_Height (Terminal_Height * 2)
#define Display_Size   (Display_Width * Display_Height)

void TerminalCursor_ResetColor() {
    printf("\033[0m");
}

void TerminalCursor_SetColor(RGBA foreground, RGBA background) {
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
    dwMode |= ENABLE_MOUSE_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
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
    // Clear the scrollback buffer
    // system("cls"); // NOLINT

    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD                      topLeft  = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen   = { 0 };
    DWORD                      written  = { 0 };

    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
#else
    // Clear screen and scrollback buffer
    printf("\033[2J\033[3J\033[H");
#endif
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
    Swap(char*, Display_bufferCurrent, Display_bufferNext);
    Swap(usize, Display_bufferCurrentSize, Display_bufferNextSize);
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
    Display_bufferNext[--index] = '\0';
    Display_bufferNextSize      = index;

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



typedef struct GameOfLife GameOfLife;
struct GameOfLife {
    int* states_;
    int* nextStates_;
};
void GameOfLife_Init();
void GameOfLife_Update();
void GameOfLife_Fini();


static GameOfLife self[1] = {
    {
        .states_     = NULL,
        .nextStates_ = NULL,
    }
};
static int  getCell(int x, int y);
static void setCell(int x, int y, const char* str);


int main(int argc, const char* argv[]) {
    UnusedValue(argc);
    UnusedValue(argv);
    Terminal_Bootup();
    Display_Init();

    GameOfLife_Init();
    bool isRunning = true;
    while (isRunning) {
        if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState('Q')) {
            isRunning = false;
        }

        GameOfLife_Update();
        Display_Render();
        Sleep(100);
    }
    GameOfLife_Fini();

    Terminal_Shutdown();
    return 0;
}



void GameOfLife_Init() {
    const int size = Display_Size;

    self->states_     = (int*)malloc(size * sizeof(int));
    self->nextStates_ = (int*)malloc(size * sizeof(int));
    assert(self->states_);
    assert(self->nextStates_);

    memset(self->states_, 0, size * sizeof(int));
    memset(self->nextStates_, 0, size * sizeof(int));

    srand((unsigned int)time(NULL)); // NOLINT
    // for (int i = 0; i < size; ++i) {
    //     self->nextStates_[i] = rand() % 2;
    // }

    // // R-Pentomino
    // setCell(80, 50, "  ## ");
    // setCell(80, 51, " ##  ");
    // setCell(80, 52, "  #  ");

    // // Gosper Glider Gun
    setCell(60, 45, "........................#............");
    setCell(60, 46, "......................#.#............");
    setCell(60, 47, "............##......##............##.");
    setCell(60, 48, "...........#...#....##............##.");
    setCell(60, 49, "##........#.....#...##...............");
    setCell(60, 50, "##........#...#.##....#.#............");
    setCell(60, 51, "..........#.....#.......#............");
    setCell(60, 52, "...........#...#.....................");
    setCell(60, 53, "............##.......................");

    // // Infinite Growth
    // setCell(20, 50, "########.#####...###......#######.#####");
}

void GameOfLife_Update() {
    RGBA colors[Display_Size] = { 0 };

    // Store output state
    const int size = Display_Size;
    for (int i = 0; i < size; ++i) {
        self->states_[i] = self->nextStates_[i];
    }

    const int width  = Display_Width;
    const int height = Display_Height;
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // The secret of artificial life =================================================
            int neighborsNum = (getCell(x - 1, y - 1) + getCell(x - 0, y - 1) + getCell(x + 1, y - 1)) +
                               (getCell(x - 1, y + 0) + 0 + getCell(x + 1, y + 0)) +
                               (getCell(x - 1, y + 1) + getCell(x + 0, y + 1) + getCell(x + 1, y + 1));

            if (getCell(x, y) == 1) {
                self->nextStates_[x + y * width] = neighborsNum == 2 || neighborsNum == 3;
            } else {
                self->nextStates_[x + y * width] = neighborsNum == 3;
            }
            // ===============================================================================

            if (getCell(x, y) == 1) {
                colors[x + y * width] = RGBA_White;
            } else {
                colors[x + y * width] = RGBA_Black;
            }
        }
    }

    Display_SetBufferFromColors(colors);
}

void GameOfLife_Fini() {
    free(self->states_);
    free(self->nextStates_);
}

int getCell(int x, int y) {
    assert(0 <= x);
    assert(x < Display_Width);
    assert(0 <= y);
    assert(y < Display_Height);

    return self->states_[x + y * Display_Width];
}

void setCell(int x, int y, const char* str) {
    assert(0 <= x);
    assert(x < Display_Width);
    assert(0 <= y);
    assert(y < Display_Height);
    assert(str != NULL);

    const int width = Display_Width;
    const int len   = (int)strlen(str);
    for (int i = 0; i < len; ++i) {
        self->nextStates_[(x + i) + y * width] = str[i] == '#' ? 1 : 0;
    }
}
