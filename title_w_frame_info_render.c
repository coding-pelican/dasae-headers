#include <stdbool.h>
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
FrameStats* FrameStats_init(FrameStats* s, const char* title, float frame_cap, bool vsync) {
    s->base_title         = title;
    s->frame_count        = 0;
    s->frame_cap          = frame_cap;
    s->is_vsync           = vsync;
    s->last_time          = get_time();
    s->current_time       = s->last_time;
    s->frame_time_index   = 0;
    s->current_fps        = 0.0f;
    s->current_frame_time = 0.0f;
    s->average_fps        = 0.0f;
    s->average_frame_time = 0.0f;

    // Initialize frame time buffer
    for (int i = 0; i < FRAME_TIME_RING_BUFFER_SIZE; i++) {
        s->frame_times[i] = 0.0;
    }

    return s;
}

// Update frame statistics
void FrameStats_update(FrameStats* stats) {
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

void FrameStats_setTerminalTitle(FrameStats* stats) {
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

#define EXAMPLE_MAIN
#ifdef EXAMPLE_MAIN
// Simple sleep function for the example
void sleep_ms(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

int main() {
    // Example 1: Uncapped frame rate
    FrameStats uncapped_stats = { 0 };
    FrameStats_init(&uncapped_stats, "Uncapped Game", 0, false);

    printf("Running uncapped frame rate example for 10 seconds...\n");
    double end_time = get_time() + 10.0;
    while (get_time() < end_time) {
        FrameStats_update(&uncapped_stats);
        FrameStats_setTerminalTitle(&uncapped_stats);
        // Simulate some work
        // sleep_ms(8);
    }

    // Example 2: 60 FPS cap
    FrameStats capped_stats = { 0 };
    FrameStats_init(&capped_stats, "60 FPS Game", 60.0f, false);

    printf("\nRunning 60 FPS example for 10 seconds...\n");
    end_time = get_time() + 10.0;
    while (get_time() < end_time) {
        FrameStats_update(&capped_stats);
        FrameStats_setTerminalTitle(&capped_stats);
        // Simulate frame timing for 60 FPS
        sleep_ms(16);
    }

    // Example 3: VSync
    FrameStats vsync_stats = { 0 };
    FrameStats_init(&vsync_stats, "VSync Game", 0, true);

    printf("\nRunning VSync example for 10 seconds...\n");
    end_time = get_time() + 10.0;
    while (get_time() < end_time) {
        FrameStats_update(&vsync_stats);
        FrameStats_setTerminalTitle(&vsync_stats);
        // Simulate VSync timing
        sleep_ms(16);
    }

    return 0;
}
#endif
