#include <dh/core.h>
#include <dh/time.h>

typedef struct FrameRateStats FrameRateStats;
typedef struct WindowConfig   WindowConfig;
typedef struct Window         Window;

typedef struct WindowTime {
    Instant  last_frame; // Time of last frame beginning
    Instant  curr_frame; // Time of current frame beginning
    Instant  prev_step;  // Time before current step
    Instant  curr_step;  // Time begin current step
    Duration time;       // Time since window creation
    f64      time_scale; // Time scale
    Duration target;     // Target frame time
    f64      target_fps; // Target frames per second
    Duration update;     // Update time
    Duration render;     // Render time
    Duration frame;      // Update + Render time
    Duration delay;      // Target frame - Frame time
} WindowTime;
f64  WindowTime_time();
f64  WindowTime_timeScale();
void WindowTime_setTimeScale(f64 scale);
f64  WindowTime_targetFPS();
void WindowTime_setTargetFPS(f64 target);
f64  WindowTime_deltaTime();
f64  WindowTime_unscaledDeltaTime();

typedef struct WindowFrameStats {
    u64  count;             // Total elapsed frames
    f64* sample_times;      // Dynamic array based on window config
    u16  sample_time_index; // Index of current sample time
    u16  sample_count;      // Criteria for ring-buffers (Derived from window config)
} WindowFrameStats;

struct FrameRateStats {
    // Timing data
    Instant instant_last;
    Instant instant_current;
    f32*    times; // Dynamic array based on window config
    u16     time_index;
    u16     sample_count; // Derived from window config

    // Frame count
    u32 count;

    // Running averages
    f32 current_duration;
    f32 current_fps;
    f32 average_duration;
    f32 average_fps;
};
// Initialize frame statistics
FrameRateStats* FrameRateStats_init(FrameRateStats* s, const WindowConfig* config);
// Finish frame statistics
FrameRateStats* FrameRateStats_fini(FrameRateStats* s);
// Update frame statistics
void            FrameRateStats_update(FrameRateStats* s);

#define WindowConfig_font_width                  (8)
#define WindowConfig_font_height                 (16)
#define WindowConfig_title_text_size             (256)
#define WindowConfig_frame_rate_target_text_size (32)
#define WindowConfig_frame_rate_uncapped_text    "uncapped"
#define WindowConfig_frame_rate_vsync_text       "vsync"

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
    } dimensions_;
    const char* title_text_;
    bool        title_shows_in_buffer_;
    struct {
        f32  target;
        u16  sample_count;
        bool enabled;
        bool vsync;
        bool displays_in_title;
    } frame_rate_;
};
/* clang-format off */
static const WindowConfig WindowConfig_default = {
    .dimensions_ = {
        .constraints = {
            .min_width  = 80,
            .min_height = 25,
            .max_width  = 160,
            .max_height = 50
        },
        .width  = 80,
        .height = 25
    },
    .title_text_            = "DH-Window",
    .title_shows_in_buffer_ = true,
    .frame_rate_            = {
        .target            = 0,
        .sample_count      = 128,
        .enabled           = false,
        .vsync             = false,
        .displays_in_title = true
    }
};
/* clang-format on */


struct Window {
    WindowConfig   config_;
    FrameRateStats frame_rate_stats_;
    // Additional fields for future extensions
    // Canvas* canvas;
    // Viewport viewport;
    // Camera camera;
};
// initialize window
Window*               Window_init(Window* w, WindowConfig config);
Window*               Window_withSize(Window* w, u32 width, u32 height);
Window*               Window_withMinSize(Window* w, u32 width, u32 height);
Window*               Window_withMaxSize(Window* w, u32 width, u32 height);
Window*               Window_withTitle(Window* w, const char* title, bool shows_in_buffer);
Window*               Window_withFrameRate(Window* w, bool enabled, f32 target_fps, bool vsync, bool displays_in_title);
// finish window
Window*               Window_fini(Window* w);
// configuration
const WindowConfig*   Window_config(const Window* w);
// dimensions
u16                   Window_width(const Window* w);
u16                   Window_height(const Window* w);
// title
const char*           Window_title(const Window* w, bool with_frame_rate_stats);
bool                  Window_showsTitleInBuffer(const Window* w);
// frame rate
const FrameRateStats* Window_frameRateStats(const Window* w);
bool                  Window_frameRateEnabled(const Window* w);
f32                   Window_frameRateTarget(const Window* w);
f32                   Window_frameRateCurrent(const Window* w);
f32                   Window_frameRateCurrentDuration(const Window* w);
f32                   Window_frameRateAverage(const Window* w);
f32                   Window_frameRateAverageDuration(const Window* w);
bool                  Window_displaysFrameRateInTitle(const Window* w);
// methods
void                  Window_update(Window* w);
void                  Window_delay(const Window* w);
