#include "window.h"
#include "../dh/debug/debug_assert.h"
#include "../dh/time.h"
#include <stdio.h>


FrameStats* FrameStats_init(FrameStats* s, const char* title, f32 frame_cap, bool vsync) {
    debug_assertNotNull(s);

    s->base_title             = title;
    s->frame_count            = 0;
    s->frame_capacity         = frame_cap;
    s->is_vsync               = vsync;
    s->frame_instant_last     = Instant_now();
    s->frame_instant_current  = s->frame_instant_last;
    s->frame_time_index       = 0;
    s->current_fps            = 0.0f;
    s->current_frame_duration = 0.0f;
    s->average_fps            = 0.0f;
    s->average_frame_duration = 0.0f;

    // Initialize frame time buffer
    for (usize i = 0; i < FrameState_FRAME_TIME_RING_BUFFER_SIZE; ++i) {
        s->frame_times[i] = 0.0f;
    }

    return s;
}

// Update frame statistics
void FrameStats_update(FrameStats* s) {
    debug_assertNotNull(s);

    s->frame_count++;
    s->frame_instant_last    = s->frame_instant_current;
    s->frame_instant_current = Instant_now();

    // Calculate frame time
    f64 frame_time = Duration_toMillis(
        Instant_durationSince(
            s->frame_instant_current,
            s->frame_instant_last
        )
    );

    // Store frame time in ring buffer
    s->frame_times[s->frame_time_index] = frame_time;
    s->frame_time_index                 = (s->frame_time_index + 1) % FrameState_FRAME_TIME_RING_BUFFER_SIZE;

    // Calculate current values first
    // Protect against division by zero and extreme values
    frame_time                = 0.0001 < frame_time ? frame_time : 0.0001;
    s->current_frame_duration = (f32)frame_time;
    s->current_fps            = 1000.0f / (f32)frame_time;

    // Calculate running averages only when we have enough samples
    if (FrameState_FRAME_TIME_RING_BUFFER_SIZE <= s->frame_count) {
        f64   total_frame_time = 0.0;
        usize valid_samples    = 0;

        for (usize i = 0; i < FrameState_FRAME_TIME_RING_BUFFER_SIZE; ++i) {
            if (0.0 < s->frame_times[i]) {
                total_frame_time += s->frame_times[i];
                valid_samples++;
            }
        }

        if (0 < valid_samples) {
            f32 avg_frame_time        = (f32)(total_frame_time / (f64)valid_samples);
            avg_frame_time            = 0.0001f < avg_frame_time ? avg_frame_time : 0.0001f;
            s->average_frame_duration = avg_frame_time;
            s->average_fps            = 1000.0f / avg_frame_time;
        }
    }
}

const char* FrameStats_getTitle(const FrameStats* s) {
    debug_assertNotNull(s);

    static char title_buffer[WindowConfig_title_buffer_size] = { 0 };
    char        frame_cap_str[32]                            = { 0 };

    // Format frame cap string
    if (s->is_vsync) {
        pp_ignore snprintf(frame_cap_str, sizeof(frame_cap_str), "vsync");
    } else if (s->frame_capacity <= 0) {
        pp_ignore snprintf(frame_cap_str, sizeof(frame_cap_str), FrameState_UNCAPPED_STRING);
    } else {
        pp_ignore snprintf(frame_cap_str, sizeof(frame_cap_str), "%.1f", s->frame_capacity);
    }

    // Format the title string
    pp_ignore snprintf(
        title_buffer,
        WindowConfig_title_buffer_size,
        "%s | frame: %s / %.1f(%.2fms) avg: %.1f(%.2fms)",
        s->base_title,
        frame_cap_str,
        s->current_fps,
        s->current_frame_duration,
        s->average_fps,
        s->average_frame_duration
    );

    return title_buffer;
}

void FrameStats_setTerminalTitle(const FrameStats* s) {
    debug_assertNotNull(s);

    const char* title = FrameStats_getTitle(s);
#ifdef _WIN32
    SetConsoleTitleA(title);
#else
    printf("\033]0;%s\007", title);
    fflush(stdout);
#endif
}

Window* Window_init(Window* w, WindowConfig config) {
    debug_assertNotNull(w);

    w->cell_width  = WindowConfig_font_width;
    w->cell_height = WindowConfig_font_height;

    w->actual_width  = config.buffer.width;
    w->actual_height = config.buffer.height;

    w->config = config;
    if (config.shows_frame_info) {
        FrameStats_init(
            &w->frame_stats,
            config.title,
            config.frame_control.target_fps,
            config.frame_control.vsync
        );
    }

    return w;
}

Window* Window_withSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);

    w->config.buffer.width  = width;
    w->config.buffer.height = height;
    w->actual_width         = width;
    w->actual_height        = height;

    return w;
}

Window* Window_withMinSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);

    w->config.buffer.constraints.min_width  = width;
    w->config.buffer.constraints.min_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withMaxSize(Window* w, u32 width, u32 height) {
    debug_assertNotNull(w);

    w->config.buffer.constraints.max_width  = width;
    w->config.buffer.constraints.max_height = height;
    // Validate current size against new constraints
    return w;
}

Window* Window_withTitle(Window* w, const char* title, bool shows_in_buffer) {
    debug_assertNotNull(w);

    w->config.title                  = title;
    w->config.allows_title_in_buffer = shows_in_buffer;
    return w;
}

Window* Window_withFrameControl(Window* w, bool enabled, f32 target_fps, bool vsync) {
    debug_assertNotNull(w);

    w->config.frame_control.enabled    = enabled;
    w->config.frame_control.target_fps = target_fps;
    w->config.frame_control.vsync      = vsync;

    FrameStats_init(
        &w->frame_stats,
        w->config.title,
        target_fps,
        vsync
    );

    return w;
}

Window* Window_fini(Window* w) {
    debug_assertNotNull(w);

    if (w->config.shows_frame_info) {
        // FrameStats_fini(&w->frame_stats);
    }
    return w;
}

void Window_update(Window* w) {
    debug_assertNotNull(w);

    FrameStats_update(&w->frame_stats);
    if (w->config.shows_frame_info) {
        FrameStats_setTerminalTitle(&w->frame_stats);
    }
}

const char* Window_title(const Window* w) {
    debug_assertNotNull(w);

    if (!w->config.shows_frame_info) {
        return w->config.title;
    }
    return FrameStats_getTitle(&w->frame_stats);
}
