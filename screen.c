#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#  include <conio.h>
#  include <windows.h>
#else // !_WIN32
#  include <sys/ioctl.h>
#  include <termios.h>
#  include <unistd.h>
#endif // _WIN32

enum ScreenDimensions {
    ScreenDimensions_Width  = 80,
    ScreenDimensions_Height = 25,
    ScreenDimensions_Size   = ScreenDimensions_Width * ScreenDimensions_Height
};

typedef struct ScreenCell ScreenCell;
struct ScreenCell {
    char character;
    int  foreground_color;
    int  background_color;
    int  attributes;
};

typedef struct Screen Screen;
struct Screen {
    ScreenCell buffer[ScreenDimensions_Size];
#ifdef _WIN32
    HANDLE     handle;
    CHAR_INFO  buffer_original[ScreenDimensions_Size];
    COORD      size;
    COORD      buffer_size;
    SMALL_RECT write_region;
#else // !_WIN32
    struct termios orig_termios;
#endif // _WIN32
};

Screen screen = {
    .buffer = { 0 },
#ifdef _WIN32
    .handle          = NULL,
    .buffer_original = { 0 },
    .size            = { ScreenDimensions_Width, ScreenDimensions_Height },
    .buffer_size     = { ScreenDimensions_Width, ScreenDimensions_Height },
    .write_region    = { 0, 0, ScreenDimensions_Width - 1, ScreenDimensions_Height - 1 }
#else // !_WIN32
    .orig_termios = { 0 }
#endif // _WIN32
};

void bootupScreen() {
#ifdef _WIN32
    screen.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(screen.handle, screen.size);
    SetConsoleWindowInfo(screen.handle, TRUE, &screen.write_region);

    CONSOLE_CURSOR_INFO cursor_info = { 0 };
    GetConsoleCursorInfo(screen.handle, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(screen.handle, &cursor_info);

    system("cls"); // NOLINT
#else // !_WIN32
    struct winsize w = { 0 };
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("\x1b[8;%d;%dt", ScreenDimensions_Height, ScreenDimensions_Width);

    tcgetattr(STDIN_FILENO, &screen.orig_termios);
    struct termios raw = screen.orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    printf("\x1b[2J\x1b[H");
    printf("\x1b[?25l");
#endif // _WIN32
}

void shutdownScreen() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO console_info = { 0 };
    GetConsoleScreenBufferInfo(screen.handle, &console_info);
    SetConsoleScreenBufferSize(screen.handle, console_info.dwSize);
    SetConsoleWindowInfo(screen.handle, TRUE, &console_info.srWindow);

    CONSOLE_CURSOR_INFO cursor_info = { 0 };
    GetConsoleCursorInfo(screen.handle, &cursor_info);
    cursor_info.bVisible = TRUE;
    SetConsoleCursorInfo(screen.handle, &cursor_info);

    system("cls"); // NOLINT
#else // !_WIN32
    printf("\x1b[?25h");
    printf("\x1b[2J\x1b[H");
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &screen.orig_termios);
#endif // _WIN32
}

void saveScreenToBuffer() {
#ifdef _WIN32
    ReadConsoleOutput(screen.handle, screen.buffer_original, screen.buffer_size, (COORD){ 0, 0 }, &screen.write_region);
    for (int i = 0; i < ScreenDimensions_Size; ++i) {
        screen.buffer[i].character        = screen.buffer_original[i].Char.AsciiChar;
        screen.buffer[i].foreground_color = screen.buffer_original[i].Attributes & 0x0F;
        screen.buffer[i].background_color = (screen.buffer_original[i].Attributes & 0xF0) >> 4;
        screen.buffer[i].attributes       = screen.buffer_original[i].Attributes & 0xFF00;
    }
#else // !_WIN32
    printf("\x1b[?1049h"); // Enable alternate screen buffer
    for (int y = 0; y < ScreenDimensions_Height; ++y) {
        for (int x = 0; x < ScreenDimensions_Width; ++x) {
            int index = y * ScreenDimensions_Width + x;
            printf("\x1b[%d;%dH\x1b[6n", y + 1, x + 1);
            char response[32];
            if (fgets(response, sizeof(response), stdin)) {
                int  row, col;
                char ch;
                if (sscanf(response, "\x1b[%d;%dR%c", &row, &col, &ch) == 3) {
                    screen.buffer[index].character = ch;
                }
            }
        }
    }
    printf("\x1b[?1049l"); // Disable alternate screen buffer
#endif // _WIN32
}

void loadScreenFromBuffer() {
#ifdef _WIN32
    CHAR_INFO new_screen[ScreenDimensions_Size] = { 0 };
    for (int i = 0; i < ScreenDimensions_Size; ++i) {
        new_screen[i].Char.AsciiChar = screen.buffer[i].character;
        new_screen[i].Attributes =
            (screen.buffer[i].background_color << 4) |
            screen.buffer[i].foreground_color |
            screen.buffer[i].attributes;
    }
    WriteConsoleOutput(screen.handle, new_screen, screen.buffer_size, (COORD){ 0, 0 }, &screen.write_region);
#else // !_WIN32
    for (int i = 0; i < ScreenDimensions_Size; ++i) {
        printf("\x1b[%d;%dH", (i / ScreenDimensions_Width) + 1, (i % ScreenDimensions_Width) + 1);
        printf("\x1b[%d;%dm%c", screen.buffer[i].foreground_color + 30, screen.buffer[i].background_color + 40, screen.buffer[i].character);
    }
#endif // _WIN32
}

void saveScreenToFile(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        (void)fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    (void)fwrite(screen.buffer, sizeof(ScreenCell), ScreenDimensions_Size, file);
    (void)fclose(file);
}

void loadScreenFromFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        (void)fprintf(stderr, "Error opening file for reading\n");
        return;
    }
    (void)fread(screen.buffer, sizeof(ScreenCell), ScreenDimensions_Size, file);
    (void)fclose(file);
    loadScreenFromBuffer();
}

int main() {
    bootupScreen();

    // Write some content to the screen
    printf("\x1b[31mThis is \x1b[1mred and bold\x1b[0m\x1b[31m text.\x1b[0m\n");
    printf("\x1b[32;44mThis is green text on blue background.\x1b[0m\n");
    printf("\x1b[34mThis is \x1b[4munderlined blue\x1b[0m\x1b[34m text.\x1b[0m\n");

    // Save the current screen to buffer
    saveScreenToBuffer();

    // Save the buffer to a file
    saveScreenToFile("screen_content.bin");

    // Clear the screen
#ifdef _WIN32
    system("cls"); // NOLINT
#else // !_WIN32
    printf("\x1b[2J\x1b[H");
#endif // _WIN32

    // Write something new
    printf("\x1b[35mThis is magenta text on the second screen.\n");
    printf("\x1b[7;36mThis is inverted cyan text on the second screen.\x1b[0m\n");

    // Wait for user input
    printf("Press Enter to load the saved screen...");
    (void)getch();

    // Load and display the saved screen
    loadScreenFromFile("screen_content.bin");

    // Wait for user input
    printf("\n\x1b[0mPress Enter to exit...");
    (void)getch();

    shutdownScreen();
    return 0;
}
