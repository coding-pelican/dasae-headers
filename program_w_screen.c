// clang test_terminal_program_w_screen.c -o test_terminal_program_w_screen.exe '-Wl,-subsystem,windows'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#  include <conio.h>
#  include <windows.h>
#  define CLEAR_SCREEN() system("cls") // NOLINT
#  define SLEEP(seconds) Sleep((seconds) * 1000)
#  define GETCH()        _getch()
#else
#  include <termios.h>
#  include <unistd.h>
#  define CLEAR_SCREEN() printf("\033[H\033[J")
#  define SLEEP(seconds) sleep(seconds)
#  define GETCH()        getch()
#endif

#define unused(x) (void)(x)

#define SCREEN_WIDTH  (80)
#define SCREEN_HEIGHT (24)

char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

void init_screen() {
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            screen[i][j] = ' ';
        }
    }
}

void draw_border() {
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        screen[i][0]                = '|';
        screen[i][SCREEN_WIDTH - 1] = '|';
    }
    for (int j = 0; j < SCREEN_WIDTH; ++j) {
        screen[0][j]                 = '-';
        screen[SCREEN_HEIGHT - 1][j] = '-';
    }
    screen[0][0]                                = '+';
    screen[0][SCREEN_WIDTH - 1]                 = '+';
    screen[SCREEN_HEIGHT - 1][0]                = '+';
    screen[SCREEN_HEIGHT - 1][SCREEN_WIDTH - 1] = '+';
}

void draw_text(int row, int col, const char* text) {
    int len = (int)strlen(text);
    for (int i = 0; i < len && col + i < SCREEN_WIDTH - 1; ++i) {
        screen[row][col + i] = text[i];
    }
}

void render_screen() {
    CLEAR_SCREEN();
    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

#ifndef _WIN32
int getch() {
    struct termios oldattr, newattr;
    int            ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#endif

void run_program() {
    time_t start_time = time(NULL);
    char   buffer[64];
    int    quit = 0;

    while (!quit) {
        init_screen();
        draw_border();

        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, start_time);

        (void)snprintf(buffer, sizeof(buffer), "Program running for %.0f seconds", elapsed_time);
        draw_text(2, 2, buffer);
        draw_text(4, 2, "Menu:");
        draw_text(5, 4, "1. Option 1");
        draw_text(6, 4, "2. Option 2");
        draw_text(7, 4, "3. Option 3");
        draw_text(8, 4, "Q. Quit");
        draw_text(10, 2, "Enter your choice: ");

        render_screen();

        int ch = GETCH();
        switch (ch) {
        case '1':
            draw_text(12, 2, "You selected Option 1");
            break;
        case '2':
            draw_text(12, 2, "You selected Option 2");
            break;
        case '3':
            draw_text(12, 2, "You selected Option 3");
            break;
        case 'q':
        case 'Q':
            quit = 1;
            break;
        default:
            draw_text(12, 2, "Invalid option");
            break;
        }

        if (!quit) {
            render_screen();
            SLEEP(0.025);
        }
    }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // NOLINT
    unused(hInstance);
    unused(hPrevInstance);
    unused(lpCmdLine);
    unused(nShowCmd);
    AllocConsole();
    (void)freopen("CONIN$", "r", stdin);
    (void)freopen("CONOUT$", "w", stdout);
    (void)freopen("CONOUT$", "w", stderr);
    run_program();
    return 0;
}
#else
int main() {
    run_program();
    return 0;
}
#endif
