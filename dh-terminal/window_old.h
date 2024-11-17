#include "dh/core.h"
#include "dh/time.h"
#include "dh/time/instant.h"
#include "canvas.h"


typedef struct Window Window;
typedef struct WindowTime {
    const Window* owner;
    time_Duration time;       // Time since window creation
    f64           time_scale; // Time scale
    time_Instant  curr_step;  // Time begin current step
    time_Instant  prev_step;  // Time before current step
    time_Instant  update;     // Time measure for frame update
    time_Instant  render;     // Time measure for frame render
    f64           target_fps; // Target frames per second
    // time_Instant  last_frame; // Time of last frame beginning
    // time_Instant  curr_frame; // Time of current frame beginning
} WindowTime;

// static WindowTime* WindowTime_init(WindowTime* t, Window* owner, f64 target_fps);
// static WindowTime* WindowTime_fini(WindowTime* t);

f64  WindowTime_time(const WindowTime* t);
f64  WindowTime_timeScale(const WindowTime* t);
void WindowTime_setTimeScale(WindowTime* t, f64 scale);

f64  WindowTime_targetFPS(const WindowTime* t);
void WindowTime_setTargetFPS(WindowTime* t, f64 target);

f64 WindowTime_deltaTime(const WindowTime* t);
f64 WindowTime_unscaledDeltaTime(const WindowTime* t);

// Delay End -> Update Begin -> Update End
time_Duration WindowTime_durationUpdate(const WindowTime* t);
// Update End -> Render Begin -> Render End
time_Duration WindowTime_durationRender(const WindowTime* t);
// Update + Render
time_Duration WindowTime_durationFrame(const WindowTime* t);
// Target - Frame
time_Duration WindowTime_durationDelay(const WindowTime* t);
// Target fps duration
time_Duration WindowTime_durationTarget(const WindowTime* t);


typedef struct WindowFrameStats {
    const Window* owner;
    u64           count; // Total elapsed frames
    struct ds_Vec_FrameState {
        f64*  data;   // Dynamic array based on window config
        usize length; // Criteria for ring-buffers (Derived from window config)
        usize capacity;
    } samples;
    usize sample_time_index; // Index of current sample time
    f64   average;           // Average of all samples
} WindowFrameStats;

// static WindowFrameStats* WindowFrameStats_init(WindowFrameStats* s, Window* owner, u16 sample_count);
// static WindowFrameStats* WindowFrameStats_fini(WindowFrameStats* s);
// static void              WindowFrameStats_update(WindowFrameStats* s);
// static void              WindowFrameStats_reset(WindowFrameStats* s);
// static void              WindowFrameStats_resizeSamples(WindowFrameStats* s, u16 sample_count);


#define WindowConfig_font_width           (8)
#define WindowConfig_font_height          (16)
#define WindowConfig_title_text_size      (256)
#define WindowConfig_fps_target_text_size (32)
#define WindowConfig_fps_uncapped_text    "uncapped"
#define WindowConfig_vsync_text           "vsync"

typedef struct WindowConfig {
    const char* title_text_;
    bool        title_shows_in_buffer_;
    struct {
        // struct {
        //     u16 min_width;
        //     u16 min_height;
        //     u16 max_width;
        //     u16 max_height;
        // } constraints;
        u16 width;
        u16 height;
    } dimensions_;
    f64  fps_target_;
    u16  fps_sample_count_;
    bool fps_displays_in_title_;
    bool capped_fps_;
    bool enabled_motion_blur_;
    bool enabled_vsync_;

} WindowConfig;
/* clang-format off */
static const WindowConfig WindowConfig_default = {
    .title_text_            = "DH-Window",
    .title_shows_in_buffer_ = true,
    .dimensions_ = {
        // .constraints = {
        //     .min_width  = 80,
        //     .min_height = 25,
        //     .max_width  = 160,
        //     .max_height = 50
        // },
        .width  = 80,
        .height = 25
    },
    .fps_target_ = 50.0,
    .fps_sample_count_ = 32,
    .fps_displays_in_title_ = true,
    .capped_fps_ = false,
    .enabled_motion_blur_ = true,
    .enabled_vsync_ = true
};
/* clang-format on */


struct Window {
    WindowConfig     config_;
    WindowTime       time_[1];
    WindowFrameStats frame_stats_[1];
    // Additional fields for future extensions
    // Canvas* canvas;
    // Viewport viewport;
    // Camera camera;
};
// initialize window
Window*             Window_init(Window* w, WindowConfig config);
Window*             Window_withSize(Window* w, u32 width, u32 height);
Window*             Window_withMinSize(Window* w, u32 width, u32 height);
Window*             Window_withMaxSize(Window* w, u32 width, u32 height);
Window*             Window_withTitle(Window* w, const char* title, bool shows_in_buffer);
Window*             Window_withFrame(Window* w, bool cap_fps, f64 fps_target, u16 sample_count, bool vsync, bool displays_in_title);
// finish window
Window*             Window_fini(Window* w);
// configuration
const WindowConfig* Window_config(const Window* w);
// dimensions
u16                 Window_width(const Window* w);
u16                 Window_height(const Window* w);
// title
const char*         Window_title(const Window* w, bool with_fps_stats);
bool                Window_title_shows_in_buffer(const Window* w);
void                Window_showTitleInBuffer(Window* w);
void                Window_hideTitleInBuffer(Window* w);
// frame rate capped
f64                 Window_capped_fps(const Window* w);
void                Window_capFpsTarget(Window* w);
void                Window_uncapFpsTarget(Window* w);
// frame rate target
f64                 Window_fps_target(const Window* w);
void                Window_setFpsTarget(Window* w, f64 fps_target);
// frame rate sample count
u16                 Window_fps_sample_count(const Window* w);
void                Window_resizeFpsSampleCount(Window* w, u16 sample_count);
// frame rate display
bool                Window_fps_displays_in_title(const Window* w);
void                Window_displayFpsInTitle(Window* w);
void                Window_concealFpsInTitle(Window* w);
// motion blur
bool                Window_enabled_motion_blur(const Window* w);
void                Window_enableMotionBlur(Window* w);
void                Window_disableMotionBlur(Window* w);
// vsync
bool                Window_enabled_vsync(const Window* w);
void                Window_enableVsync(Window* w);
void                Window_disableVsync(Window* w);
// methods
void                Window_update(Window* w);
void                Window_render(Window* w, Canvas* canvas);
void                Window_delay(Window* w);


// struct FrameRateStats {
//     // Timing data
//     time_Instant instant_last;
//     time_Instant instant_current;
//     f32*         times; // Dynamic array based on window config
//     u16          time_index;
//     u16          sample_count; // Derived from window config

//     // Frame count
//     u32 count;

//     // Running averages
//     f32 current_duration;
//     f32 current_fps;
//     f32 average_duration;
//     f32 average_fps;
// };
// // Initialize frame statistics
// FrameRateStats* FrameRateStats_init(FrameRateStats* s, const WindowConfig* config);
// // Finish frame statistics
// FrameRateStats* FrameRateStats_fini(FrameRateStats* s);
// // Update frame statistics
// void            FrameRateStats_update(FrameRateStats* s);
