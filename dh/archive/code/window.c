#include "window.h"
#include "dh/debug/assert.h"
#include "dh/mem.h"
#include "dh/time.h"

#include <stdio.h>


FrameRateStats* FrameRateStats_init(FrameRateStats* s, const WindowConfig* config) {
    debug_assert_nonnull(s);
    debug_assert_nonnull(config);

    s->times = mem_allocCleared(f32, config->frame_rate_.sample_count);
    debug_assert_nonnull(s->times);
    s->time_index      = 0;
    s->instant_last    = time_Instant_now();
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
    debug_assert_nonnull(s);

    mem_free(&s->times);
    debug_assert_null(s->times);

    return s;
}

void FrameRateStats_update(FrameRateStats* s) {
    debug_assert_nonnull(s);

    s->count++;
    s->instant_last    = s->instant_current;
    s->instant_current = time_Instant_now();

    // Calculate frame time duration
    f32 current_duration = (f32)time_Duration_toMillis(
        time_Instant_durationSince(
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
    s->current_fps      = (f32)time_MILLIS_PER_SEC / current_duration;

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
        s->average_fps       = (f32)time_MILLIS_PER_SEC / average_duration;
    }
}

Window* Window_init(Window* w, WindowConfig config) {
    debug_assert_nonnull(w);
    w->config_ = config;
    return w;
}

Window* Window_withSize(Window* w, u32 width, u32 height) {
    debug_assert_nonnull(w);
    w->config_.dimensions_.width  = width;
    w->config_.dimensions_.height = height;
    return w;
}

Window* Window_withMinSize(Window* w, u32 width, u32 height) {
    debug_assert_nonnull(w);
    w->config_.dimensions_.constraints.min_width  = width;
    w->config_.dimensions_.constraints.min_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withMaxSize(Window* w, u32 width, u32 height) {
    debug_assert_nonnull(w);
    w->config_.dimensions_.constraints.max_width  = width;
    w->config_.dimensions_.constraints.max_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withTitle(Window* w, const char* title, bool shows_in_buffer) {
    debug_assert_nonnull(w);
    w->config_.title_text_            = title;
    w->config_.title_shows_in_buffer_ = shows_in_buffer;
    return w;
}

Window* Window_withFrameRate(Window* w, bool enabled, f32 target_fps, bool vsync, bool displays_in_title) {
    debug_assert_nonnull(w);
    w->config_.frame_rate_.enabled           = enabled;
    w->config_.frame_rate_.target            = target_fps;
    w->config_.frame_rate_.vsync             = vsync;
    w->config_.frame_rate_.displays_in_title = displays_in_title;
    FrameRateStats_init(&w->frame_rate_stats_, &w->config_);
    return w;
}

Window* Window_fini(Window* w) {
    debug_assert_nonnull(w);
    FrameRateStats_fini(&w->frame_rate_stats_);
    return w;
}

const WindowConfig* Window_config(const Window* w) {
    return &w->config_;
}

u16 Window_width(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.dimensions_.width;
}

u16 Window_height(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.dimensions_.height;
}

const char* Window_title(const Window* w, bool with_frame_rate_stats) {
    debug_assert_nonnull(w);

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
    unused(frame_rate_target_text_size);

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

bool Window_title_shows_in_buffer(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.title_shows_in_buffer_;
}

const FrameRateStats* Window_frameRateStats(const Window* w) {
    debug_assert_nonnull(w);
    return &w->frame_rate_stats_;
}

bool Window_frameRateEnabled(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.frame_rate_.enabled;
}

f32 Window_frameRateTarget(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.frame_rate_.target;
}

f32 Window_frameRateCurrent(const Window* w) {
    debug_assert_nonnull(w);
    return w->frame_rate_stats_.current_fps;
}

f32 Window_frameRateCurrentDuration(const Window* w) {
    debug_assert_nonnull(w);
    return w->frame_rate_stats_.current_duration;
}

f32 Window_frameRateAverage(const Window* w) {
    debug_assert_nonnull(w);
    return w->frame_rate_stats_.average_fps;
}

f32 Window_frameRateAverageDuration(const Window* w) {
    debug_assert_nonnull(w);
    return w->frame_rate_stats_.average_duration;
}

bool Window_displaysFrameRateInTitle(const Window* w) {
    debug_assert_nonnull(w);
    return w->config_.frame_rate_.displays_in_title;
}

static void Window__setTerminalTitle(Window* w, const char* title) {
    debug_assert_nonnull(w);
    debug_assert_nonnull(title);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTitleA(title);
#else
    printf("\033]0;%s\007", title);
    fflush(stdout);
#endif
}

static void Window__updateTitle(Window* w) {
    debug_assert_nonnull(w);
    const bool  with_frame_rate_stats = Window_displaysFrameRateInTitle(w);
    const char* title                 = Window_title(w, with_frame_rate_stats);
    Window__setTerminalTitle(w, title);
}

void Window_update(Window* w) {
    debug_assert_nonnull(w);

    FrameRateStats_update(&w->frame_rate_stats_);
    Window__updateTitle(w);
}

void Window_delay(const Window* w) {
    debug_assert_nonnull(w);

    const bool enabled = Window_frameRateEnabled(w);
    if (!enabled) { return; }

    const f32 target = Window_frameRateTarget(w);
    if (target <= 0.0f) { return; }

    const time_Duration frame_time        = time_Instant_elapsed(w->frame_rate_stats_.instant_current);
    const time_Duration target_frame_time = time_Duration_fromSecs_f64(1.0 / w->config_.frame_rate_.target);
    const time_Duration sleep_time        = time_Duration_sub(target_frame_time, frame_time);

    time_SysTime_sleep(sleep_time);
}
