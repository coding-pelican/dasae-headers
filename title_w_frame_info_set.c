#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Buffer size for the title string
#define TITLE_BUFFER_SIZE (256)

// #define NO_DISPLAY_FRAME_RATE_IN_TITLE (1)

void FrameStats_setTerminalTitle(const char* base_title, int current_frame, float current_fps, float current_frame_time, float avg_fps, float avg_frame_time) {
    char title_buffer[TITLE_BUFFER_SIZE] = { 0 };

    // Format the title string using snprintf to prevent buffer overflow
    (void)snprintf(title_buffer, TITLE_BUFFER_SIZE, "%s | frame: %d / %.1f(%.2fms) avg: %.1f(%.2fms)", base_title, current_frame, current_fps, current_frame_time, avg_fps, avg_frame_time);

#ifndef NO_DISPLAY_FRAME_RATE_IN_TITLE
#ifdef _WIN32
    // Windows: Use SetConsoleTitleA for ANSI strings
    SetConsoleTitleA(title_buffer);
#else
    // Linux/Unix: Use ANSI escape sequence
    // \033]0; sets the title, \007 terminates the sequence
    printf("\033]0;%s\007", title_buffer);
    fflush(stdout); // Ensure the output is sent immediately
#endif
#else
    printf("%s\n", title_buffer);
#endif
}

// Example usage
#define EXAMPLE_MAIN
#ifdef EXAMPLE_MAIN
int main() {
    const char* game_title         = "My Game";
    int         frame_count        = 1000;
    float       current_fps        = 60.0f;
    float       current_frame_time = 16.67f;
    float       avg_fps            = 59.8f;
    float       avg_frame_time     = 16.72f;

    FrameStats_setTerminalTitle(game_title, frame_count, current_fps, current_frame_time, avg_fps, avg_frame_time);

// Keep the terminal open to see the result
#ifdef _WIN32
    Sleep(5000);
#else
    sleep(5);
#endif

    return 0;
}
#endif
