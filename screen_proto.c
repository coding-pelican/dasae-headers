#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#  include <windows.h>
#else
#  include <sys/ioctl.h>
#  include <termios.h>
#  include <unistd.h>
#endif

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

ScreenCell screenBuffer[ScreenDimensions_Size] = { 0 };

#ifdef _WIN32
HANDLE     hConsole = NULL;
CHAR_INFO  originalScreen[ScreenDimensions_Size];
COORD      screenSize  = { ScreenDimensions_Width, ScreenDimensions_Height };
COORD      bufferSize  = { ScreenDimensions_Width, ScreenDimensions_Height };
SMALL_RECT writeRegion = { 0, 0, ScreenDimensions_Width - 1, ScreenDimensions_Height - 1 };
#endif

void enableDoubleBuffering() {
#ifdef _WIN32
    hConsole                       = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo = { 0 };
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    printf("\x1b[?1049h");
#endif
}

void disableDoubleBuffering() {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO cursorInfo = { 0 };
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    printf("\x1b[?1049l");
#endif
}

void saveScreenToBuffer() {
#ifdef _WIN32
    ReadConsoleOutput(hConsole, originalScreen, bufferSize, (COORD){ 0, 0 }, &writeRegion);
    for (int i = 0; i < ScreenDimensions_Size; ++i) {
        screenBuffer[i].character        = originalScreen[i].Char.AsciiChar;
        screenBuffer[i].foreground_color = originalScreen[i].Attributes & 0x0F;
        screenBuffer[i].background_color = (originalScreen[i].Attributes & 0xF0) >> 4;
        screenBuffer[i].attributes       = originalScreen[i].Attributes & 0xFF00;
    }
#else
    // Use ANSI escape codes to query screen content
    printf("\x1b[?1049h"); // Enable alternate screen buffer
    for (int y = 0; y < ScreenDimensions_Height; ++y) {
        for (int x = 0; x < ScreenDimensions_Width; ++x) {
            int index = y * ScreenDimensions_Width + x;
            printf("\x1b[%d;%dH\x1b[6n", y + 1, x + 1);
            char response[32] = { 0 };
            if (fgets(response, sizeof(response), stdin)) {
                int  row = 0;
                int  col = 0;
                char ch  = 0;
                if (sscanf(response, "\x1b[%d;%dR%c", &row, &col, &ch) == 3) {
                    screenBuffer[index].character = ch;
                }
            }
        }
    }
    printf("\x1b[?1049l"); // Disable alternate screen buffer
#endif
}

void loadScreenFromBuffer() {
    printf("\x1b[H"); // Move cursor to home position
    for (int i = 0; i < ScreenDimensions_Size; ++i) {
        ScreenCell cell = screenBuffer[i];

        // Set attributes
        printf("\x1b[0m"); // Reset attributes
        if (cell.attributes & 1) {
            printf("\x1b[1m"); // Bold
        }
        if (cell.attributes & 2) {
            printf("\x1b[4m"); // Underline
        }
        if (cell.attributes & 4) {
            printf("\x1b[7m"); // Inverse
        }

        // Set colors
        printf("\x1b[38;5;%dm", cell.foreground_color);
        printf("\x1b[48;5;%dm", cell.background_color);

        // Print character
        putchar(cell.character);

        // Move to next line if at end of row
        if ((i + 1) % ScreenDimensions_Width == 0) {
            printf("\n");
        }
    }
}

void saveScreenToFile(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        (void)fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    (void)fwrite(screenBuffer, sizeof(ScreenCell), ScreenDimensions_Size, file);
    (void)fclose(file);
}

void loadScreenFromFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        (void)fprintf(stderr, "Error opening file for reading\n");
        return;
    }
    (void)fread(screenBuffer, sizeof(ScreenCell), ScreenDimensions_Size, file);
    (void)fclose(file);
    loadScreenFromBuffer();
}

int main() {
    enableDoubleBuffering();

    // Write some content to the screen
    printf("\x1b[31mThis is \x1b[1mred and bold\x1b[0m\x1b[31m text.\n");
    printf("\x1b[32;44mThis is green text on blue background.\x1b[0m\n");
    printf("\x1b[4;34mThis is underlined blue text.\x1b[0m\n");

    // Save the current screen to buffer
    saveScreenToBuffer();

    // Save the buffer to a file
    saveScreenToFile("screen_content.bin");

// Clear the screen
#ifdef _WIN32
    system("cls"); // NOLINT
#else
    printf("\x1b[2J\x1b[H");
#endif

    // Write something new
    printf("\x1b[35mThis is magenta text on the second screen.\n");
    printf("\x1b[7;36mThis is inverted cyan text on the second screen.\x1b[0m\n");

    // Wait for user input
    printf("Press Enter to load the saved screen...");
    (void)getchar();

    // Load and display the saved screen
    loadScreenFromFile("screen_content.bin");

    // Wait for user input
    printf("\n\x1b[0mPress Enter to exit...");
    (void)getchar();

    disableDoubleBuffering();
    return 0;
}
