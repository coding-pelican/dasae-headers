// wt --size 80,25 -d . cmd /k .\test_terminal_renderer.exe
#include <conio.h>
#include <stdio.h>
#include <wchar.h>


typedef struct Glyph Glyph;
struct Glyph {
    union {
        wchar_t unicode;
        char    ascii;
    };
};


typedef unsigned short Color;
enum Color {
    Color_Black = 0,
    Color_Red,
    Color_Green,
    Color_Yellow,
    Color_Blue,
    Color_Magenta,
    Color_Cyan,
    Color_White,
    Color__Count
};
const Color Color__Default = 9;
const int   Color__BeginFG = 30;
const int   Color__BeginBG = 40;


typedef struct Cell Cell;
struct Cell {
    Color color;
    Glyph glyph;
};
const Cell Cell__None = (Cell){
    .color         = Color_Black + Color__BeginBG,
    .glyph.unicode = L' '
};


enum EScreen {
    FontWidth  = 4,
    FontHeight = 8,

    RatioWidth  = 16,
    RatioHeight = 10,

    ResolutionScale  = 20,
    ResolutionWidth  = RatioWidth * ResolutionScale, // 16 * 20 = 320
    ResolutionHeight = RatioHeight * ResolutionScale, // 10 * 20 = 200

    CellsWidth  = ResolutionWidth / FontWidth, // 320 / 4 = 80
    CellsHeight = ResolutionHeight / FontHeight, // 200 / 8 = 25
    CellCount   = CellsWidth * CellsHeight,

    PixelsWidth  = CellsWidth / 2,
    PixelsHeight = CellsHeight,
    PixelCount   = PixelsWidth * PixelsHeight
};
typedef struct Screen Screen;
struct Screen {
    Cell cells[CellCount];
};
static Screen screen = { 0 };

Cell* Screen_CellAt(int x, int y);
void  Screen_Fill(Color color);
void  Screen_DrawCell(int x, int y, Glyph glyph, Color color);
void  Screen_DrawPixel(int x, int y, Color color);
void  Screen_Render();


typedef struct Text Text;
struct Text {
    const char* typos;
    int         size;
};
#define Text(...) (                                                       \
    (Text){ .typos = __VA_ARGS__,                                         \
            .size  = (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0])) - 1 } \
)
void PrintTypo(int x, int y, wchar_t typo, Color color);
void PrintText(int x, int y, Text text, Color color);


int main() {
    Screen_Fill(Color_Black);
    Screen_Render();
    getch();

    PrintText(0, 0, Text("Hello, world!"), Color_Magenta);
    Screen_Render();
    getch();

    for (int i = 1; i < Color__Count; ++i) {
        Screen_DrawPixel(i - 1, 1, i);
        Screen_DrawPixel(i - 1, 3, i);
        Screen_DrawPixel(i - 1, 5, i);
    }
    Screen_Render();
    getch();
    return 0;
}


Cell* Screen_CellAt(int x, int y) {
    return &screen.cells[x + CellsWidth * y];
}

void Screen_Fill(Color color) {
    color += Color__BeginBG;
    Cell cell  = Cell__None;
    cell.color = color;

    for (int i = 0; i < CellCount; ++i) {
        screen.cells[i] = cell;
    }
}

void Screen_DrawCell(int x, int y, Glyph glyph, Color color) {
    color += Color__BeginFG;
    Cell cell = {
        .glyph = glyph,
        .color = color
    };

    Screen_CellAt(x, y)[0] = cell;
}

void Screen_DrawPixel(int x, int y, Color color) {
    color += Color__BeginBG;
    Cell cell  = Cell__None;
    cell.color = color;

    Screen_CellAt(x * 2, y)[0]     = cell;
    Screen_CellAt(x * 2 + 1, y)[0] = cell;
}

void Screen_Render() {
    printf("\033[2J\033[H");
    for (int i = 0; i < CellCount; ++i) {
        printf("\033[%dm%lc", screen.cells[i].color, screen.cells[i].glyph.unicode);
    }
}

void PrintTypo(int x, int y, wchar_t typo, Color color) {
    color += Color__BeginFG;
    Cell buffer         = Cell__None;
    buffer.color        = color;
    Cell atlas          = buffer;
    atlas.glyph.unicode = typo;

    Screen_CellAt(x * 2, y)[0]     = atlas;
    Screen_CellAt(x * 2 + 1, y)[0] = buffer;
}

void PrintText(int x, int y, Text text, Color color) {
    for (int i = 0; i < text.size; ++i) {
        PrintTypo(x + i, y, text.typos[i], color);
    }
}
