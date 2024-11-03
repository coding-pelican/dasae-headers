// build `clang -xc check_window_resize.c -o check_window_resize`
// run with `./check_window_resize`


#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif


typedef struct {
    int width;
    int height;
} TerminalSize;

TerminalSize get_terminal_size() {
    TerminalSize size = { 0, 0 };

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size.width  = w.ws_col;
    size.height = w.ws_row;
#endif

    return size;
}

void check_resize() {
    TerminalSize last_size = get_terminal_size();

    while (1) {
        TerminalSize current_size = get_terminal_size();

        if (current_size.width != last_size.width || current_size.height != last_size.height) {
            printf("Terminal resized! New dimensions: %dx%d\n", current_size.width, current_size.height);

            if (current_size.width > last_size.width) {
                printf("Width increased by %d\n", current_size.width - last_size.width);
            } else if (current_size.width < last_size.width) {
                printf("Width decreased by %d\n", last_size.width - current_size.width);
            }

            if (current_size.height > last_size.height) {
                printf("Height increased by %d\n", current_size.height - last_size.height);
            } else if (current_size.height < last_size.height) {
                printf("Height decreased by %d\n", last_size.height - current_size.height);
            }

            last_size = current_size;
        }

#ifdef _WIN32
        Sleep(100); // 100ms delay
#else
        usleep(100000); // 100ms delay
#endif
    }
}

int main() {
    printf("Monitoring terminal size. Resize the window to see changes.\n");
    printf("Initial size: %dx%d\n", get_terminal_size().width, get_terminal_size().height);
    check_resize();
    return 0;
}
