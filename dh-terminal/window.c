#include "window.h"
#include "../dh/debug/debug_assert.h"
#include "../dh/mem.h"
#include "../dh/time.h"
#include <stdio.h>


FrameRateStats* FrameRateStats_init(FrameRateStats* s, const WindowConfig* config) {
    debug_assertNotNull(s);
    debug_assertNotNull(config);

    s->times = mem_newCleared(f32, config->frame_rate_.sample_count);
    debug_assertNotNull(s->times);
    s->time_index      = 0;
    s->instant_last    = Instant_now();
    s->instant_current = s->instant_last;
    s->sample_count    = config->frame_rate_.sample_count;

    s->count = 0;

    s->current_duration = 0;
    s->current_fps      = 0;
    s->average_duration = 0;
    s->average_fps      = 0;

    return s;
}

FrameRateStats* FrameRateStats_fini(FrameRateStats* s) {
    debug_assertNotNull(s);

    mem_delete(&s->times);
    debug_assertNull(s->times);

    return s;
}

void FrameRateStats_update(FrameRateStats* s) {
    debug_assertNotNull(s);

    s->count++;
    s->instant_last    = s->instant_current;
    s->instant_current = Instant_now();

    // Calculate frame time duration
    f32 current_duration = (f32)Duration_toMillis(
        Instant_durationSince(
            s->instant_current,
            s->instant_last
        )
    );

    // Store frame time in ring buffer
    s->times[s->time_index] = current_duration;
    s->time_index           = (s->time_index + 1) % s->sample_count;

    // Calculate current values first
    // Protect against division by zero and extreme values
    current_duration    = 0.0001f < current_duration ? current_duration : 0.0001f;
    s->current_duration = current_duration;
    s->current_fps      = (f32)Time_millis_per_sec / current_duration;

    if (s->count < s->sample_count) { return; }
    // Calculate running averages only when we have enough samples
    f64   total_duration     = 0.0;
    usize valid_sample_count = 0;
    for (usize index = 0; index < s->sample_count; ++index) {
        if (0.0 < s->times[index]) {
            total_duration += s->times[index];
            valid_sample_count++;
        }
    }

    if (0 < valid_sample_count) {
        f32 average_duration = (f32)(total_duration / (f64)valid_sample_count);
        average_duration     = 0.0001f < average_duration ? average_duration : 0.0001f;
        s->average_duration  = average_duration;
        s->average_fps       = (f32)Time_millis_per_sec / average_duration;
    }
}

Window* Window_init(Window* w, WindowConfig config) {
    debug_assertNotNull(w);
    w->config_ = config;
    return w;
}

Window* Window_withSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);
    w->config_.dimensions_.width  = width;
    w->config_.dimensions_.height = height;
    return w;
}

Window* Window_withMinSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);
    w->config_.dimensions_.constraints.min_width  = width;
    w->config_.dimensions_.constraints.min_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withMaxSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);
    w->config_.dimensions_.constraints.max_width  = width;
    w->config_.dimensions_.constraints.max_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withTitle(Window* w, const char* title, bool shows_in_buffer) {
    debug_assertNotNull(w);
    w->config_.title_text_            = title;
    w->config_.title_shows_in_buffer_ = shows_in_buffer;
    return w;
}

Window* Window_withFrameRate(Window* w, bool enabled, f32 target_fps, bool vsync, bool displays_in_title) {
    debug_assertNotNull(w);
    w->config_.frame_rate_.enabled           = enabled;
    w->config_.frame_rate_.target            = target_fps;
    w->config_.frame_rate_.vsync             = vsync;
    w->config_.frame_rate_.displays_in_title = displays_in_title;
    FrameRateStats_init(&w->frame_rate_stats_, &w->config_);
    return w;
}

Window* Window_fini(Window* w) {
    debug_assertNotNull(w);
    FrameRateStats_fini(&w->frame_rate_stats_);
    return w;
}

const WindowConfig* Window_config(const Window* w) {
    return &w->config_;
}

u16 Window_width(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.dimensions_.width;
}

u16 Window_height(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.dimensions_.height;
}

const char* Window_title(const Window* w, bool with_frame_rate_stats) {
    debug_assertNotNull(w);

    if (!with_frame_rate_stats) {
        if (!w->config_.title_text_) { return ""; }
        return w->config_.title_text_;
    }

    static char frame_rate_target_text[WindowConfig_frame_rate_target_text_size] = { 0 };

    usize frame_rate_target_text_size = 0;
    if (!w->config_.frame_rate_.enabled || w->config_.frame_rate_.target <= 0.0f) {
        frame_rate_target_text_size = snprintf(
            frame_rate_target_text,
            WindowConfig_frame_rate_target_text_size,
            "%s",
            WindowConfig_frame_rate_uncapped_text
        );
    } else if (w->config_.frame_rate_.vsync) {
        frame_rate_target_text_size = snprintf(
            frame_rate_target_text,
            WindowConfig_frame_rate_target_text_size,
            "%s",
            WindowConfig_frame_rate_vsync_text
        );
    } else {
        frame_rate_target_text_size = snprintf(
            frame_rate_target_text,
            WindowConfig_frame_rate_target_text_size,
            "%.1f",
            w->config_.frame_rate_.target
        );
    }
    pp_unused(frame_rate_target_text_size);

    static char title_text[WindowConfig_title_text_size] = { 0 };

    usize title_text_size = 0;
    if (w->config_.title_text_) {
        title_text_size = snprintf(
            title_text,
            WindowConfig_title_text_size,
            "%s",
            w->config_.title_text_
        );
    }
    if (0 < title_text_size) {
        title_text_size += snprintf(
            title_text + title_text_size,
            WindowConfig_title_text_size - title_text_size,
            " | "
        );
    }
    title_text_size += snprintf(
        title_text + title_text_size,
        WindowConfig_title_text_size - title_text_size,
        "frame: %s / %.1f(%.2fms) avg: %.1f(%.2fms)",
        frame_rate_target_text,
        w->frame_rate_stats_.current_fps,
        w->frame_rate_stats_.current_duration,
        w->frame_rate_stats_.average_fps,
        w->frame_rate_stats_.average_duration
    );

    return title_text;
}

bool Window_showsTitleInBuffer(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.title_shows_in_buffer_;
}

const FrameRateStats* Window_frameRateStats(const Window* w) {
    debug_assertNotNull(w);
    return &w->frame_rate_stats_;
}

bool Window_frameRateEnabled(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.frame_rate_.enabled;
}

f32 Window_frameRateTarget(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.frame_rate_.target;
}

f32 Window_frameRateCurrent(const Window* w) {
    debug_assertNotNull(w);
    return w->frame_rate_stats_.current_fps;
}

f32 Window_frameRateCurrentDuration(const Window* w) {
    debug_assertNotNull(w);
    return w->frame_rate_stats_.current_duration;
}

f32 Window_frameRateAverage(const Window* w) {
    debug_assertNotNull(w);
    return w->frame_rate_stats_.average_fps;
}

f32 Window_frameRateAverageDuration(const Window* w) {
    debug_assertNotNull(w);
    return w->frame_rate_stats_.average_duration;
}

bool Window_displaysFrameRateInTitle(const Window* w) {
    debug_assertNotNull(w);
    return w->config_.frame_rate_.displays_in_title;
}

static void Window__setTerminalTitle(Window* w, const char* title) {
    debug_assertNotNull(w);
    debug_assertNotNull(title);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTitleA(title);
#else
    printf("\033]0;%s\007", title);
    fflush(stdout);
#endif
}

static void Window__updateTitle(Window* w) {
    debug_assertNotNull(w);
    const bool  with_frame_rate_stats = Window_displaysFrameRateInTitle(w);
    const char* title                 = Window_title(w, with_frame_rate_stats);
    Window__setTerminalTitle(w, title);
}

void Window_update(Window* w) {
    debug_assertNotNull(w);

    FrameRateStats_update(&w->frame_rate_stats_);
    Window__updateTitle(w);
}

void Window_delay(const Window* w) {
    debug_assertNotNull(w);

    const bool enabled = Window_frameRateEnabled(w);
    if (!enabled) { return; }

    const f32 target = Window_frameRateTarget(w);
    if (target <= 0.0f) { return; }

    const Duration frame_time        = Instant_elapsed(w->frame_rate_stats_.instant_current);
    const Duration target_frame_time = Duration_fromSecs(1.0 / w->config_.frame_rate_.target);
    const Duration sleep_time        = Duration_sub(target_frame_time, frame_time);

    SystemTime_sleep(sleep_time);
}
