#include "../dh/compat/compat_wchar.h"
#include "../dh/debug/debug_assert.h"
#include "../dh/time.h"
#include "../dh/types.h"


typedef struct FrameStats   FrameStats;
typedef struct WindowConfig WindowConfig;
typedef enum EWindowDimensionsInitMode {
    WinDimInitMode_Pixel, // Dimensions specified in pixels
    WinDimInitMode_Cell   // Dimensions specified in terminal cells
} WindowMode;
typedef struct Window Window;


#define FrameStats_FPS_SAMPLE_COUNT            (120) // Number of frames to average
#define FrameState_UNCAPPED_STRING             "uncapped"
#define FrameState_FRAME_TIME_RING_BUFFER_SIZE (FrameStats_FPS_SAMPLE_COUNT)

struct FrameStats {
    const char* base_title;
    i32         frame_count;
    f32         frame_capacity; // Target FPS (0 for uncapped)
    bool        is_vsync;       // VSync enabled flag

    // Timing data
    Instant frame_instant_last;
    Instant frame_instant_current;
    f64     frame_times[FrameState_FRAME_TIME_RING_BUFFER_SIZE];
    i32     frame_time_index;

    // Running averages
    f32 current_frame_duration;
    f32 current_fps;
    f32 average_frame_duration;
    f32 average_fps;
};
// Initialize frame statistics
FrameStats* FrameStats_init(FrameStats* s, const char* title, f32 frame_cap, bool vsync);
// Update frame statistics
void        FrameStats_update(FrameStats* s);
const char* FrameStats_getTitle(const FrameStats* s);
void        FrameStats_setTerminalTitle(const FrameStats* s);


#define WindowConfig_title_buffer_size (256)
#define WindowConfig_font_width        (8)
#define WindowConfig_font_height       (16)

struct WindowConfig {
    struct {
        struct {
            u16 min_width;
            u16 min_height;
            u16 max_width;
            u16 max_height;
        } constraints;
        u16 width;
        u16 height;
    } buffer;
    struct {
        f32  target_fps;
        bool enabled;
        bool vsync;
    } frame_control;
    const char* title;
    bool        allows_title_in_buffer;
    bool        shows_frame_info;
};
static const WindowConfig WindowConfig_default = {
    .buffer                 = { .width = 80, .height = 25 },
    .frame_control          = { .target_fps = 62.5f, .enabled = true, .vsync = true },
    .title                  = "DH-Window",
    .allows_title_in_buffer = true,
    .shows_frame_info       = true,
};


struct Window {
    WindowConfig config;
    FrameStats   frame_stats;
    u32          actual_width;
    u32          actual_height;
    f32          cell_width;  // Fixed at 8
    f32          cell_height; // Fixed at 16
    // Additional fields for future extensions
    // Canvas* canvas;
    // Viewport viewport;
    // Camera camera;
};
Window*     Window_init(Window* w, WindowConfig config);
Window*     Window_withSize(Window* w, u32 width, u32 height);
Window*     Window_withMinSize(Window* w, u32 width, u32 height);
Window*     Window_withMaxSize(Window* w, u32 width, u32 height);
Window*     Window_withTitle(Window* w, const char* title, bool shows_in_buffer);
Window*     Window_withFrameControl(Window* w, bool enabled, f32 target_fps, bool vsync);
Window*     Window_fini(Window* w);
void        Window_update(Window* w);
const char* Window_title(const Window* w);
