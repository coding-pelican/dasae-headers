#include <stdio.h>
#include <windows.h>

#define SCREEN_WIDTH  (80)
#define SCREEN_HEIGHT (25)

void clear_screen() {
    // Clear screen and move cursor to top-left corner
    printf("\033[2J\033[H");
}

void set_cursor_position(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

void draw_border() {
    int i = 0;

    // Top border
    set_cursor_position(1, 1);
    printf("+");
    for (i = 2; i < SCREEN_WIDTH; ++i) {
        printf("-");
    }
    printf("+");

    // Side borders
    for (i = 2; i < SCREEN_HEIGHT; ++i) {
        set_cursor_position(i, 1);
        printf("|");
        set_cursor_position(i, SCREEN_WIDTH);
        printf("|");
    }

    // Bottom border
    set_cursor_position(SCREEN_HEIGHT, 1);
    printf("+");
    for (i = 2; i < SCREEN_WIDTH; ++i) {
        printf("-");
    }
    printf("+");
}

void draw_text(int row, int col, const char* text) {
    set_cursor_position(row, col);
    printf("%s", text);
}

int main() {
    // Enable virtual terminal processing
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    clear_screen();
    draw_border();

    draw_text(3, 3, "Welcome to the 80x25 terminal program!");
    draw_text(5, 3, "This program uses ANSI escape codes to control output.");
    draw_text(7, 3, "It should work in most modern terminal emulators.");
    draw_text(SCREEN_HEIGHT - 2, 3, "Press any key to exit...");

    // Wait for a keypress
    (void)getchar();

    clear_screen();
    return 0;
}
