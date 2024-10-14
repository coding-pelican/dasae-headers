#include <stdio.h>

#ifdef _WIN32
#  include <windows.h>
#else // !_WIN32
#  include <sys/ioctl.h>
#  include <termios.h>
#  include <unistd.h>
#endif // _WIN32

void enableDoubleBuffering() {
#ifdef _WIN32
    // Windows implementation
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Enable alternative screen buffer
    printf("\x1b[?1049h");
#else // !_WIN32
    // Linux implementation
    struct termios term = { 0 };
    tcgetattr(STDOUT_FILENO, &term);
    term.c_oflag |= OPOST;
    tcsetattr(STDOUT_FILENO, TCSANOW, &term);

    // Enable alternative screen buffer
    printf("\x1b[?1049h");
#endif // _WIN32
}

void disableDoubleBuffering() {
#ifdef _WIN32
    // Disable alternative screen buffer
    printf("\x1b[?1049l");
#else // !_WIN32
    // Disable alternative screen buffer
    printf("\x1b[?1049l");
#endif // _WIN32
}

int main() {
    printf("A: begin screen.");
    // Wait for user input
    (void)getchar();

    printf("B: Enabling double-buffered screen.");
    // Wait for user input
    (void)getchar();
    enableDoubleBuffering();
    // Wait for user input
    (void)getchar();
    printf("C: Enabled double-buffered screen.");
    // Wait for user input
    (void)getchar();

    // Your code here
    printf("D: This is in double-buffered screen.");
    // Wait for user input
    (void)getchar();

    printf("E: Disabling double-buffered screen.");
    // Wait for user input
    (void)getchar();
    disableDoubleBuffering();
    // Wait for user input
    (void)getchar();
    printf("F: Disabled double-buffered screen.");
    // Wait for user input
    (void)getchar();

    printf("G: end screen.");
    // Wait for user input
    (void)getchar();
    return 0;
}
