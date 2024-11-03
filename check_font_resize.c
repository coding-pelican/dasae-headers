// build `clang -xc check_font_resize.c -o check_font_resize`
// run with `./check_font_resize`


#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif


/* typedef struct {
    int width;
    int height;
    int font_width;
    int font_height;
} TerminalMetrics;

TerminalMetrics get_terminal_metrics() {
    TerminalMetrics metrics = { 0 };

#ifdef _WIN32
    HANDLE                     hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    CONSOLE_FONT_INFO          font_info;

    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        metrics.width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        metrics.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    if (GetCurrentConsoleFont(hOut, FALSE, &font_info)) {
        metrics.font_width  = font_info.dwFontSize.X;
        metrics.font_height = font_info.dwFontSize.Y;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        metrics.width       = w.ws_col;
        metrics.height      = w.ws_row;
        // Note: Getting exact font dimensions in pixels is not directly
        // possible in all terminal emulators on Unix-like systems
        metrics.font_width  = 1; // Placeholder for character width
        metrics.font_height = 1; // Placeholder for character height
    }
#endif

    return metrics;
}

bool detect_font_resize(TerminalMetrics* previous, TerminalMetrics* current) {
    if (previous->font_width != current->font_width || previous->font_height != current->font_height) {
        return true;
    }
    return false;
}

void monitor_font_changes() {
    TerminalMetrics previous = get_terminal_metrics();

    printf("Initial terminal metrics:\n");
    printf("Size: %dx%d characters\n", previous.width, previous.height);
    printf("Font size: %dx%d pixels\n", previous.font_width, previous.font_height);

    while (1) {
        TerminalMetrics current = get_terminal_metrics();

        if (detect_font_resize(&previous, &current)) {
            printf("\nFont size changed!\n");
            printf("Previous: %dx%d pixels\n", previous.font_width, previous.font_height);
            printf("Current: %dx%d pixels\n", current.font_width, current.font_height);
            previous = current;
        }

#ifdef _WIN32
        Sleep(1000); // Check every second
#else
        usleep(1000000); // 1 second in microseconds
#endif
    }
} */


typedef struct {
    int width;
    int height;
    int pixel_width;
    int pixel_height;
} TerminalMetrics;

TerminalMetrics get_terminal_metrics() {
    TerminalMetrics metrics = { 0 };

#ifdef _WIN32
    HANDLE                     hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        metrics.width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        metrics.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        // Get actual window size in pixels
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow) {
            RECT rect;
            if (GetClientRect(consoleWindow, &rect)) {
                metrics.pixel_width  = rect.right - rect.left;
                metrics.pixel_height = rect.bottom - rect.top;
            }
        }
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        metrics.width        = w.ws_col;
        metrics.height       = w.ws_row;
        metrics.pixel_width  = w.ws_xpixel;
        metrics.pixel_height = w.ws_ypixel;
    }
#endif

    return metrics;
}

bool detect_font_resize(TerminalMetrics previous, TerminalMetrics current) {
    // Calculate and compare average character dimensions
    float prev_char_width  = (float)previous.pixel_width / (float)previous.width;
    float prev_char_height = (float)previous.pixel_height / (float)previous.height;

    float curr_char_width  = (float)current.pixel_width / (float)current.width;
    float curr_char_height = (float)current.pixel_height / (float)current.height;

    // Check if character dimensions changed significantly (using small threshold)
    const float threshold = 0.1f; // 10% change threshold
    return (fabsf(prev_char_width - curr_char_width) > threshold || fabsf(prev_char_height - curr_char_height) > threshold);
}

void monitor_font_changes() {
    TerminalMetrics previous = get_terminal_metrics();

    printf("Initial terminal metrics:\n");
    printf("Size: %dx%d characters\n", previous.width, previous.height);
    printf("Window: %dx%d pixels\n", previous.pixel_width, previous.pixel_height);
    printf("Char size: %.2fx%.2f pixels\n", (float)previous.pixel_width / (float)previous.width, (float)previous.pixel_height / (float)previous.height);

    while (1) {
        TerminalMetrics current = get_terminal_metrics();

        if (detect_font_resize(previous, current)) {
            printf("\nFont size changed!\n");
            printf("Previous char size: %.2fx%.2f pixels\n", (float)previous.pixel_width / (float)previous.width, (float)previous.pixel_height / (float)previous.height);
            printf("Current char size: %.2fx%.2f pixels\n", (float)current.pixel_width / (float)current.width, (float)current.pixel_height / (float)current.height);
            previous = current;
        }

#ifdef _WIN32
        Sleep(100); // 100ms delay
#else
        usleep(100000); // 100ms delay
#endif
    }
}


int main() {
    monitor_font_changes();
    return 0;
}
