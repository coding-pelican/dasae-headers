#include "dh/core.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#define TITLE_BUFFER_SIZE           (256)
#define FPS_SAMPLE_COUNT            (120) // Number of frames to average
#define UNCAPPED_STRING             "uncapped"
#define FRAME_TIME_RING_BUFFER_SIZE (FPS_SAMPLE_COUNT)

typedef struct {
    const char* base_title;
    int         frame_count;
    float       frame_cap; // Target FPS (0 for uncapped)
    bool        is_vsync;  // VSync enabled flag

    // Timing data
    double last_time;
    double current_time;
    double frame_times[FRAME_TIME_RING_BUFFER_SIZE];
    int    frame_time_index;

    // Running averages
    float current_fps;
    float current_frame_time;
    float average_fps;
    float average_frame_time;
} FrameStats;

// Get current time in seconds with high precision
static double get_time() {
#ifdef _WIN32
    LARGE_INTEGER freq    = { 0 };
    LARGE_INTEGER counter = { 0 };
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
#else
    struct timespec ts = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

// Initialize frame statistics
void init_frame_stats(FrameStats* stats, const char* title, float frame_cap, bool vsync) {
    stats->base_title         = title;
    stats->frame_count        = 0;
    stats->frame_cap          = frame_cap;
    stats->is_vsync           = vsync;
    stats->last_time          = get_time();
    stats->current_time       = stats->last_time;
    stats->frame_time_index   = 0;
    stats->current_fps        = 0.0f;
    stats->current_frame_time = 0.0f;
    stats->average_fps        = 0.0f;
    stats->average_frame_time = 0.0f;

    // Initialize frame time buffer
    for (int i = 0; i < FRAME_TIME_RING_BUFFER_SIZE; i++) {
        stats->frame_times[i] = 0.0;
    }
}

// Update frame statistics
void update_frame_stats(FrameStats* stats) {
    stats->frame_count++;
    stats->current_time = get_time();

    // Calculate frame time
    double frame_time = (stats->current_time - stats->last_time) * 1000.0; // Convert to milliseconds
    stats->last_time  = stats->current_time;

    // Store frame time in ring buffer
    stats->frame_times[stats->frame_time_index] = frame_time;
    stats->frame_time_index                     = (stats->frame_time_index + 1) % FRAME_TIME_RING_BUFFER_SIZE;

    // Calculate averages
    double total_frame_time = 0.0;
    for (int i = 0; i < FRAME_TIME_RING_BUFFER_SIZE; i++) {
        total_frame_time += stats->frame_times[i];
    }

    // Update current values
    stats->current_frame_time = (float)frame_time;
    stats->current_fps        = 1000.0f / (float)frame_time;

    // Update running averages
    if (stats->frame_count >= FRAME_TIME_RING_BUFFER_SIZE) {
        float avg_frame_time      = (float)(total_frame_time / FRAME_TIME_RING_BUFFER_SIZE);
        stats->average_frame_time = avg_frame_time;
        stats->average_fps        = 1000.0f / avg_frame_time;
    }
}

void set_terminal_title(FrameStats* stats) {
    char title_buffer[TITLE_BUFFER_SIZE];
    char frame_cap_str[32];

    // Format frame cap string
    if (stats->is_vsync) {
        (void)snprintf(frame_cap_str, sizeof(frame_cap_str), "vsync");
    } else if (stats->frame_cap <= 0) {
        (void)snprintf(frame_cap_str, sizeof(frame_cap_str), UNCAPPED_STRING);
    } else {
        (void)snprintf(frame_cap_str, sizeof(frame_cap_str), "%.1f", stats->frame_cap);
    }

    // Format the title string
    (void)snprintf(title_buffer, TITLE_BUFFER_SIZE, "%s | frame: %d / %s(%.2fms) avg: %.1f(%.2fms)", stats->base_title, stats->frame_count, frame_cap_str, stats->current_frame_time, stats->average_fps, stats->average_frame_time);

#ifdef _WIN32
    SetConsoleTitleA(title_buffer);
#else
    printf("\033]0;%s\007", title_buffer);
    fflush(stdout);
#endif
}


typedef enum WindowMode {
    WindowMode_Pixel, // Dimensions specified in pixels
    WindowMode_Cell   // Dimensions specified in terminal cells
} WindowMode;

typedef struct WindowConfig {
    WindowMode mode;
    union {
        struct {
            u32 pixelWidth;
            u32 pixelHeight;
        };
        struct {
            u32 cellWidth;
            u32 cellHeight;
        };
    };
    const char* title;
    bool        allowTitleInBuffer;
    bool        showFrameInfo;
    struct {
        bool enabled;
        f32  targetFps;
        bool vsync;
    } frameControl;
    struct {
        u32 minWidth;
        u32 minHeight;
        u32 maxWidth;
        u32 maxHeight;
    } constraints;
} WindowConfig;

WindowConfig config = {
    .mode               = WindowMode_Pixel,
    .pixelWidth         = 800,
    .pixelHeight        = 600,
    .title              = "My Window",
    .allowTitleInBuffer = true,
    .showFrameInfo      = true,
    .frameControl       = {
              .enabled   = true,
              .targetFps = 60.0f,
              .vsync     = false }
};


typedef struct Window {
    WindowConfig config_;
    u32          actualWidth;
    u32          actualHeight;
    f32          cellWidth;  // Fixed at 8
    f32          cellHeight; // Fixed at 16
    FrameStats   frameStats;
    // Additional fields for future extensions
    // Canvas* canvas;
    // Viewport viewport;
    // Camera camera;
} Window;


Window* Window_create(const WindowConfig* config);
void    Window_withMinSize(Window* window, u32 width, u32 height);
void    Window_withMaxSize(Window* window, u32 width, u32 height);
void    Window_withTitle(Window* window, const char* title, bool showInBuffer);
void    Window_withFrameControl(Window* window, bool enabled, f32 targetFps, bool vsync);

Window* Window_create(const WindowConfig* config) {
    Window* window = mem_new(Window, 1);

    window->cellWidth  = 8;
    window->cellHeight = 16;

    if (config->mode == WindowMode_Pixel) {
        window->actualWidth  = config->pixelWidth / window->cellWidth;
        window->actualHeight = config->pixelHeight / window->cellHeight;
    } else {
        window->actualWidth  = config->cellWidth;
        window->actualHeight = config->cellHeight;
    }

    window->config = *config;

    if (config->showFrameInfo) {
        init_frame_stats(&window->frameStats, config->title, config->frameControl.targetFps, config->frameControl.vsync);
    }

    return window;
}

void Window_withMinSize(Window* window, u32 width, u32 height) {
    window->config.constraints.minWidth  = width;
    window->config.constraints.minHeight = height;
    // Validate current size against new constraints
}

void Window_withMaxSize(Window* window, u32 width, u32 height) {
    window->config.constraints.maxWidth  = width;
    window->config.constraints.maxHeight = height;
    // Validate current size against new constraints
}

void Window_withTitle(Window* window, const char* title, bool showInBuffer) {
    window->config.title              = title;
    window->config.allowTitleInBuffer = showInBuffer;
}

void Window_withFrameControl(Window* window, bool enabled, f32 targetFps, bool vsync) {
    window->config.frameControl.enabled   = enabled;
    window->config.frameControl.targetFps = targetFps;
    window->config.frameControl.vsync     = vsync;

    if (enabled && window->config.showFrameInfo) {
        init_frame_stats(&window->frameStats, window->config.title, targetFps, vsync);
    }
}
