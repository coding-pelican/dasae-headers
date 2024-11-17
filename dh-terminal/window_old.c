#include "window_old.h"
#include "dh/core.h"
#include "dh/core/cfg.h"
#include "dh/mem.h"
#include "dh/time/cfg.h"
#include "dh/time/duration.h"
#include "dh/time/instant.h"
#include "dh/time/system_time.h"

static WindowTime* WindowTime_init(WindowTime* t, const Window* owner, f64 target_fps) {
    t->owner      = owner;
    t->time       = time_Duration_ZERO;
    t->time_scale = 1.0;
    t->curr_step  = time_Instant_now();
    t->prev_step  = time_Instant_now();
    t->update     = time_Instant_now();
    t->render     = time_Instant_now();
    t->target_fps = target_fps;
    return t;
}

static WindowTime* WindowTime_fini(WindowTime* t) {
    return t;
}

f64 WindowTime_time(const WindowTime* t) {
    return time_Duration_asSecs_f64(t->time);
}

f64 WindowTime_timeScale(const WindowTime* t) {
    return t->time_scale;
}

void WindowTime_setTimeScale(WindowTime* t, f64 scale) {
    t->time_scale = scale;
}

f64 WindowTime_targetFPS(const WindowTime* t) {
    return t->target_fps;
}

void WindowTime_setTargetFPS(WindowTime* t, f64 target) {
    t->target_fps = target;
}

f64 WindowTime_deltaTime(const WindowTime* t) {
    return WindowTime_unscaledDeltaTime(t) * t->time_scale;
}

f64 WindowTime_unscaledDeltaTime(const WindowTime* t) {
    return time_Duration_asSecs_f64(WindowTime_durationFrame(t));
}

// Delay End -> Update Begin -> Update End
time_Duration WindowTime_durationUpdate(const WindowTime* t) {
    return time_Instant_durationSince(t->render, t->update);
}

// Update End -> Render Begin -> Render End
time_Duration WindowTime_durationRender(const WindowTime* t) {
    return time_Instant_durationSince(t->update, t->render);
}

// Update + Render
time_Duration WindowTime_durationFrame(const WindowTime* t) {
    return time_Duration_add(WindowTime_durationUpdate(t), WindowTime_durationRender(t));
}

// Target - Frame
time_Duration WindowTime_durationDelay(const WindowTime* t) {
    return time_Duration_sub(WindowTime_durationTarget(t), WindowTime_durationFrame(t));
}

// Target fps duration
time_Duration WindowTime_durationTarget(const WindowTime* t) {
    return time_Duration_fromSecs_f64(1.0 / t->target_fps);
}

static WindowFrameStats* WindowFrameStats_init(WindowFrameStats* s, const Window* owner, u16 sample_count) {
    s->owner             = owner;
    s->count             = 0;
    s->samples.data      = mem_alloc(f64, sample_count);
    s->samples.length    = 0;
    s->samples.capacity  = sample_count;
    s->sample_time_index = 0;
    s->average           = 0.0;
    return s;
}

static WindowFrameStats* WindowFrameStats_fini(WindowFrameStats* s) {
    mem_free(&s->samples.data);
    return s;
}

static void WindowFrameStats_update(WindowFrameStats* s) {
    s->count++;
    s->samples.data[s->sample_time_index] = WindowTime_unscaledDeltaTime(s->owner->time_);
    s->sample_time_index                  = (s->sample_time_index + 1) % s->samples.capacity;
    s->samples.length                     = (s->samples.length + 1) % s->samples.capacity;
    s->average                            = 0.0;
    for (usize i = 0; i < s->samples.length; i++) {
        s->average += s->samples.data[i];
    }
    s->average /= (f64)s->samples.length;
}

static void WindowFrameStats_reset(WindowFrameStats* s) {
    s->count             = 0;
    s->samples.length    = 0;
    s->sample_time_index = 0;
    s->average           = 0.0;
}

static void WindowFrameStats_resizeSamples(WindowFrameStats* s, u16 sample_count) {
    s->samples.data      = mem_reallocate(s->samples.data, sizeof(f64) * sample_count);
    s->samples.capacity  = sample_count;
    s->sample_time_index = 0;
}

Window* Window_init(Window* w, WindowConfig config) {
    w->config_ = config;
    WindowTime_init(w->time_, w, config.capped_fps_);
    WindowFrameStats_init(w->frame_stats_, w, config.fps_sample_count_);
    return w;
}
Window* Window_withSize(Window* w, u32 width, u32 height) {
    w->config_.dimensions_.width  = width;
    w->config_.dimensions_.height = height;
    return w;
}
Window* Window_withMinSize(Window* w, u32 width, u32 height) {
    unused(width);
    unused(height);
    return w;
}
Window* Window_withMaxSize(Window* w, u32 width, u32 height) {
    unused(width);
    unused(height);
    return w;
}
Window* Window_withTitle(Window* w, const char* title, bool shows_in_buffer) {
    w->config_.title_text_            = title;
    w->config_.title_shows_in_buffer_ = shows_in_buffer;
    return w;
}
Window* Window_withFrame(Window* w, bool cap_fps, f64 fps_target, u16 sample_count, bool vsync, bool displays_in_title) {
    w->config_.capped_fps_            = cap_fps;
    w->config_.fps_target_            = fps_target;
    w->config_.fps_sample_count_      = sample_count;
    w->config_.enabled_vsync_         = vsync;
    w->config_.fps_displays_in_title_ = displays_in_title;
    return w;
}
// finish window
Window* Window_fini(Window* w) {
    WindowTime_fini(w->time_);
    WindowFrameStats_fini(w->frame_stats_);
    return w;
}
// configuration
const WindowConfig* Window_config(const Window* w) {
    return &w->config_;
}
// dimensions
u16 Window_width(const Window* w) {
    return w->config_.dimensions_.width;
}
u16 Window_height(const Window* w) {
    return w->config_.dimensions_.height;
}
// title
const char* Window_title(const Window* w, bool with_fps_stats) {
    if (with_fps_stats) {
        return w->config_.title_text_;
    }
    return w->config_.title_text_;
}
bool Window_title_shows_in_buffer(const Window* w) {
    return w->config_.title_shows_in_buffer_;
}
void Window_showTitleInBuffer(Window* w) {
    w->config_.title_shows_in_buffer_ = true;
}
void Window_hideTitleInBuffer(Window* w) {
    w->config_.title_shows_in_buffer_ = false;
}
// frame rate capped
f64 Window_capped_fps(const Window* w) {
    return w->config_.capped_fps_;
}
void Window_capFpsTarget(Window* w) {
    w->config_.capped_fps_ = true;
}
void Window_uncapFpsTarget(Window* w) {
    w->config_.capped_fps_ = false;
}
// frame rate target
f64 Window_fps_target(const Window* w) {
    return w->config_.fps_target_;
}
void Window_setFpsTarget(Window* w, f64 fps_target) {
    Window_capFpsTarget(w);
    w->config_.fps_target_ = fps_target;
}
// frame rate sample count
u16 Window_fps_sample_count(const Window* w) {
    return w->config_.fps_sample_count_;
}
void Window_resizeFpsSampleCount(Window* w, u16 sample_count) {
    WindowFrameStats_resizeSamples(w->frame_stats_, sample_count);
    w->config_.fps_sample_count_ = sample_count;
}
// frame rate display
bool Window_fps_displays_in_title(const Window* w) {
    return w->config_.fps_displays_in_title_;
}
void Window_displayFpsInTitle(Window* w) {
    w->config_.fps_displays_in_title_ = true;
}
void Window_concealFpsInTitle(Window* w) {
    w->config_.fps_displays_in_title_ = false;
}
// motion blur
bool Window_enabled_motion_blur(const Window* w) {
    return w->config_.enabled_motion_blur_;
}
void Window_enableMotionBlur(Window* w) {
    w->config_.enabled_motion_blur_ = true;
}
void Window_disableMotionBlur(Window* w) {
    w->config_.enabled_motion_blur_ = false;
}
// vsync
bool Window_enabled_vsync(const Window* w) {
    return w->config_.enabled_vsync_;
}
void Window_enableVsync(Window* w) {
    w->config_.enabled_vsync_ = true;
}
void Window_disableVsync(Window* w) {
    w->config_.enabled_vsync_ = false;
}
// methods
void Window_update(Window* w) {
    w->time_[0].update = time_Instant_now();
    WindowFrameStats_update(w->frame_stats_);
}
void Window_render(Window* w, Canvas* canvas) {
    w->time_[0].render = time_Instant_now();
}
void Window_delay(Window* w) {
    time_Duration delay = WindowTime_durationDelay(w->time_);
    time_SysTime_sleep(delay);
}
