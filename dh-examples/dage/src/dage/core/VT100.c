#if UNUSED_CODE
#include "engine/core/VT100.h"

#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#endif

/*========== Constants ==========*/

#define VT100_MAX_TARGETS (1) /* Terminal only supports one "window" */

/* Brightness to ASCII character mapping */
$static const char g_brightness_chars[] = " .'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
#define BRIGHTNESS_CHAR_COUNT (sizeof(g_brightness_chars) - 1)

/* ANSI escape sequences */
#define ESC "\x1b"
#define CSI ESC "["

/*========== Target State ==========*/

typedef struct VT100_Target {
    engine_TargetId id;
    bool active;
    engine_TargetMetrics metrics;

    /* Character buffer for output */
    char* output_buffer;
    usize output_buffer_size;
} VT100_Target;

/*========== VT100 Instance ==========*/

struct engine_core_VT100 {
    mem_Allocator gpa;

    VT100_Target target;
    engine_TargetId next_target_id;

    engine_core_VT100_ColorMode color_mode;
    bool enable_mouse;
    bool use_block_chars;

    /* Terminal state backup */
#if defined(_WIN32)
    DWORD original_console_mode;
    HANDLE stdin_handle;
    HANDLE stdout_handle;
#else
    struct termios original_termios;
    bool termios_saved;
#endif

    /* Event state */
    engine_RawEvent* event_buffer;
    usize event_count;
    usize event_capacity;

    /* Input state for key tracking */
    bool keys_held[256];
};

/*========== Forward Declarations ==========*/

$static fn_((VT100_createTarget(engine_Backend_Impl* ctx, engine_Target_Cfg cfg))(E$engine_TargetId));
$static fn_((VT100_destroyTarget(engine_Backend_Impl* ctx, engine_TargetId id))(void));
$static fn_((VT100_pumpEvents(engine_Backend_Impl* ctx, S$engine_RawEvent out_events))(usize));
$static fn_((VT100_present(engine_Backend_Impl* ctx, engine_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void));
$static fn_((VT100_getTargetMetrics(engine_Backend_Impl* ctx, engine_TargetId id))(engine_TargetMetrics));

/*========== V-Table ==========*/

$static const engine_Backend_VT g_vt100_vt = {
    .createTarget = VT100_createTarget,
    .destroyTarget = VT100_destroyTarget,
    .pumpEvents = VT100_pumpEvents,
    .present = VT100_present,
    .getTargetMetrics = VT100_getTargetMetrics,
};

/*========== Helper Functions ==========*/

$static fn_((getTerminalSize(u32* out_w, u32* out_h))(bool)) {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        *out_w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *out_h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return true;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        *out_w = ws.ws_col;
        *out_h = ws.ws_row;
        return true;
    }
#endif
    *out_w = 80;
    *out_h = 24;
    return false;
}

$static fn_((rgbaToLuminance(u32 rgba))(u8)) {
    let r = (rgba >> 0) & 0xFF;
    let g = (rgba >> 8) & 0xFF;
    let b = (rgba >> 16) & 0xFF;
    /* Standard luminance formula */
    return (u8)((r * 299 + g * 587 + b * 114) / 1000);
}

$static fn_((luminanceToChar(u8 lum))(char)) {
    let idx = (lum * (BRIGHTNESS_CHAR_COUNT - 1)) / 255;
    return g_brightness_chars[idx];
}

$static fn_((pushEvent(engine_core_VT100* self, engine_Event event))(void)) {
    if (self->event_count >= self->event_capacity) {
        return;
    }
    if (self->target.id == engine_TargetId_invalid) {
        return;
    }

    self->event_buffer[self->event_count++] = (engine_RawEvent){
        .target_id = self->target.id,
        .event = event,
    };
}

$static fn_((translateKeyCode(int ch))(engine_KeyCode)) {
    if (ch >= 'a' && ch <= 'z') {
        return engine_KeyCode_a + (ch - 'a');
    }
    if (ch >= 'A' && ch <= 'Z') {
        return engine_KeyCode_a + (ch - 'A');
    }
    if (ch >= '0' && ch <= '9') {
        return engine_KeyCode_0 + (ch - '0');
    }

    switch (ch) {
    case 27:   return engine_KeyCode_esc;
    case '\r':
    case '\n': return engine_KeyCode_enter;
    case ' ':  return engine_KeyCode_space;
    case '\t': return engine_KeyCode_tab;
    case 127:
    case '\b': return engine_KeyCode_backspace;
    default:   return engine_KeyCode_unknown;
    }
}

/*========== Platform-Specific ==========*/

#if defined(_WIN32)

$static fn_((VT100_initTerminal(engine_core_VT100* self))(E$void) $guard) {
    self->stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
    self->stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    /* Save original mode */
    GetConsoleMode(self->stdin_handle, &self->original_console_mode);

    /* Enable virtual terminal processing and raw input */
    DWORD mode = ENABLE_VIRTUAL_TERMINAL_INPUT;
    SetConsoleMode(self->stdin_handle, mode);

    /* Enable VT100 output */
    DWORD out_mode;
    GetConsoleMode(self->stdout_handle, &out_mode);
    out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(self->stdout_handle, out_mode);

    /* Hide cursor */
    printf(CSI "?25l");
    fflush(stdout);

    return_ok({});
} $unguarded_(fn);

$static fn_((VT100_finiTerminal(engine_core_VT100* self))(void)) {
    /* Show cursor */
    printf(CSI "?25h");
    /* Reset attributes */
    printf(CSI "0m");
    /* Clear screen */
    printf(CSI "2J" CSI "H");
    fflush(stdout);

    /* Restore original mode */
    SetConsoleMode(self->stdin_handle, self->original_console_mode);
}

$static fn_((VT100_pollInput(engine_core_VT100* self))(void)) {
    while (_kbhit()) {
        int ch = _getch();

        /* Handle escape sequences */
        if (ch == 0 || ch == 224) {
            ch = _getch();
            engine_KeyCode key = engine_KeyCode_unknown;
            switch (ch) {
            case 72: key = engine_KeyCode_arrow_up; break;
            case 80: key = engine_KeyCode_arrow_down; break;
            case 75: key = engine_KeyCode_arrow_left; break;
            case 77: key = engine_KeyCode_arrow_right; break;
            }
            if (key != engine_KeyCode_unknown) {
                pushEvent(self, variant_from$(engine_Event)(engine_Event_key_down)((engine_Event_Key){
                                    .key = key,
                                    .mods = { .packed = 0 },
                                }));
            }
        } else {
            let key = translateKeyCode(ch);
            if (key != engine_KeyCode_unknown) {
                pushEvent(self, variant_from$(engine_Event)(engine_Event_key_down)((engine_Event_Key){
                                    .key = key,
                                    .mods = { .packed = 0 },
                                }));
            }
        }
    }
}

#else /* POSIX */

$static fn_((VT100_initTerminal(engine_core_VT100* self))(E$void) $guard) {
    /* Save terminal attributes */
    if (tcgetattr(STDIN_FILENO, &self->original_termios) == 0) {
        self->termios_saved = true;

        struct termios raw = self->original_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG);
        raw.c_iflag &= ~(IXON);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    /* Set non-blocking input */
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    /* Hide cursor */
    printf(CSI "?25l");

    /* Enable mouse tracking if requested */
    if (self->enable_mouse) {
        printf(CSI "?1003h"); /* Any event tracking */
        printf(CSI "?1006h"); /* SGR extended mode */
    }

    fflush(stdout);
    return_ok({});
} $unguarded_(fn);

$static fn_((VT100_finiTerminal(engine_core_VT100* self))(void)) {
    /* Disable mouse tracking */
    if (self->enable_mouse) {
        printf(CSI "?1003l");
        printf(CSI "?1006l");
    }

    /* Show cursor */
    printf(CSI "?25h");
    /* Reset attributes */
    printf(CSI "0m");
    /* Clear screen */
    printf(CSI "2J" CSI "H");
    fflush(stdout);

    /* Restore terminal */
    if (self->termios_saved) {
        tcsetattr(STDIN_FILENO, TCSANOW, &self->original_termios);
    }

    /* Restore blocking mode */
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

$static fn_((VT100_pollInput(engine_core_VT100* self))(void)) {
    char buf[32];
    ssize_t n;

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < n; ++i) {
            int ch = (unsigned char)buf[i];

            /* Handle escape sequences */
            if (ch == 27 && i + 2 < n && buf[i + 1] == '[') {
                engine_KeyCode key = engine_KeyCode_unknown;
                switch (buf[i + 2]) {
                case 'A': key = engine_KeyCode_arrow_up; break;
                case 'B': key = engine_KeyCode_arrow_down; break;
                case 'C': key = engine_KeyCode_arrow_right; break;
                case 'D': key = engine_KeyCode_arrow_left; break;
                }
                if (key != engine_KeyCode_unknown) {
                    pushEvent(self, variant_from$(engine_Event)(engine_Event_key_down)((engine_Event_Key){
                                        .key = key,
                                        .mods = { .packed = 0 },
                                    }));
                    i += 2;
                    continue;
                }
            }

            let key = translateKeyCode(ch);
            if (key != engine_KeyCode_unknown) {
                pushEvent(self, variant_from$(engine_Event)(engine_Event_key_down)((engine_Event_Key){
                                    .key = key,
                                    .mods = { .packed = 0 },
                                }));
            }
        }
    }
}

#endif

/*========== V-Table Implementation ==========*/

$static fn_((VT100_createTarget(
    engine_Backend_Impl* ctx,
    engine_Target_Cfg cfg
))(E$engine_TargetId) $guard) {
    let self = (engine_core_VT100*)ctx;
    claim_assert_nonnull(self);

    /* VT100 only supports one target */
    if (self->target.active) {
        return_err(Err_Custom("VT100 only supports one target"));
    }

    u32 term_w, term_h;
    getTerminalSize(&term_w, &term_h);

    /* Use terminal size if not specified */
    let width = cfg.width > 0 ? cfg.width : term_w;
    let height = cfg.height > 0 ? cfg.height : term_h;

    self->target = (VT100_Target){
        .id = self->next_target_id++,
        .active = true,
        .metrics = {
            .physical_size = { width, height },
            .logical_size = { width, height },
            .dpi_scale = 1.0f,
            .is_focused = true,
        },
    };

    /* Allocate output buffer */
    /* Worst case: each pixel needs ~20 chars for truecolor escape + char */
    let buf_size = (usize)(width * height) * 24 + 1024;
    let raw = try_(mem_Allocator_alloc(self->gpa, typeInfo$(char), buf_size));
    self->target.output_buffer = (char*)raw.ptr;
    self->target.output_buffer_size = buf_size;

    /* Clear screen */
    printf(CSI "2J" CSI "H");
    fflush(stdout);

    return_ok(self->target.id);
} $unguarded_(fn);

$static fn_((VT100_destroyTarget(
    engine_Backend_Impl* ctx,
    engine_TargetId id
))(void)) {
    let self = (engine_core_VT100*)ctx;
    claim_assert_nonnull(self);

    if (!self->target.active || self->target.id != id) {
        claim_unreachable;
    }

    if (self->target.output_buffer != null) {
        mem_Allocator_free(self->gpa, (S$u8){
                                          .ptr = (u8*)self->target.output_buffer,
                                          .len = self->target.output_buffer_size,
                                      });
    }

    self->target = cleared();
}

$static fn_((VT100_pumpEvents(
    engine_Backend_Impl* ctx,
    S$engine_RawEvent out_events
))(usize)) {
    let self = (engine_core_VT100*)ctx;
    claim_assert_nonnull(self);
    claim_assert_nonnullS(out_events);

    self->event_buffer = out_events.ptr;
    self->event_count = 0;
    self->event_capacity = out_events.len;

    /* Check for terminal resize */
    u32 new_w, new_h;
    getTerminalSize(&new_w, &new_h);

    if (self->target.active && (new_w != self->target.metrics.physical_size.x || new_h != self->target.metrics.physical_size.y)) {
        let old_size = self->target.metrics.physical_size;
        self->target.metrics.physical_size = (m_V2u32){ new_w, new_h };
        self->target.metrics.logical_size = (m_V2u32){ new_w, new_h };

        pushEvent(self, variant_from$(engine_Event)(engine_Event_resize)((engine_Event_Resize){
                            .old_size = old_size,
                            .new_size = { new_w, new_h },
                        }));
    }

    /* Poll keyboard input */
    VT100_pollInput(self);

    let count = self->event_count;
    self->event_buffer = null;
    self->event_count = 0;
    self->event_capacity = 0;

    return count;
}

$static fn_((VT100_present(
    engine_Backend_Impl* ctx,
    engine_TargetId id,
    S_const$u32 pixels,
    u32 w,
    u32 h
))(void)) {
    let self = (engine_core_VT100*)ctx;
    claim_assert_nonnull(self);

    if (!self->target.active || self->target.id != id) {
        claim_unreachable;
    }

    char* buf = self->target.output_buffer;
    usize buf_pos = 0;

    /* Move cursor to home */
    buf_pos += snprintf(buf + buf_pos, self->target.output_buffer_size - buf_pos, CSI "H");

    u32 prev_r = 256, prev_g = 256, prev_b = 256; /* Invalid initial values */

    for (u32 y = 0; y < h && y < self->target.metrics.physical_size.y; ++y) {
        for (u32 x = 0; x < w && x < self->target.metrics.physical_size.x; ++x) {
            let pixel = pixels.ptr[y * w + x];
            let r = (pixel >> 0) & 0xFF;
            let g = (pixel >> 8) & 0xFF;
            let b = (pixel >> 16) & 0xFF;
            let lum = rgbaToLuminance(pixel);
            let ch = luminanceToChar(lum);

            switch (self->color_mode) {
            case engine_core_VT100_ColorMode_truecolor: {
                if (r != prev_r || g != prev_g || b != prev_b) {
                    buf_pos += snprintf(buf + buf_pos, self->target.output_buffer_size - buf_pos, CSI "38;2;%u;%u;%um", r, g, b);
                    prev_r = r;
                    prev_g = g;
                    prev_b = b;
                }
                break;
            }

            case engine_core_VT100_ColorMode_256: {
                /* Approximate to 6x6x6 color cube */
                let color = 16 + 36 * (r / 51) + 6 * (g / 51) + (b / 51);
                buf_pos += snprintf(buf + buf_pos, self->target.output_buffer_size - buf_pos, CSI "38;5;%um", color);
                break;
            }

            case engine_core_VT100_ColorMode_16: {
                /* Use basic ANSI colors based on dominant channel */
                u8 ansi = 37; /* White default */
                if (lum < 32) ansi = 30; /* Black */
                else if (r > g && r > b)
                    ansi = (r > 128) ? 91 : 31; /* Red */
                else if (g > r && g > b)
                    ansi = (g > 128) ? 92 : 32; /* Green */
                else if (b > r && b > g)
                    ansi = (b > 128) ? 94 : 34; /* Blue */
                buf_pos += snprintf(buf + buf_pos, self->target.output_buffer_size - buf_pos, CSI "%um", ansi);
                break;
            }

            case engine_core_VT100_ColorMode_mono:
            default:
                /* No color codes */
                break;
            }

            buf[buf_pos++] = ch;
        }

        /* Newline */
        buf[buf_pos++] = '\n';
    }

    /* Reset color */
    buf_pos += snprintf(buf + buf_pos, self->target.output_buffer_size - buf_pos, CSI "0m");
    buf[buf_pos] = '\0';

    /* Output everything at once */
    fwrite(buf, 1, buf_pos, stdout);
    fflush(stdout);
}

$static fn_((VT100_getTargetMetrics(
    engine_Backend_Impl* ctx,
    engine_TargetId id
))(engine_TargetMetrics)) {
    let self = (engine_core_VT100*)ctx;
    claim_assert_nonnull(self);

    if (!self->target.active || self->target.id != id) {
        claim_unreachable;
    }

    return self->target.metrics;
}

/*========== Public API ==========*/

fn_((engine_core_VT100_init(engine_core_VT100_Cfg cfg))(E$engine_core_VT100) $guard) {
    let gpa = mem_Allocator_ensureValid(cfg.gpa);

    let raw = try_(mem_Allocator_alloc(gpa, typeInfo$(engine_core_VT100), 1));
    let self = (engine_core_VT100*)raw.ptr;
    errdefer_(mem_Allocator_free(gpa, raw));

    *self = cleared();
    self->gpa = gpa;
    self->color_mode = cfg.color_mode;
    self->enable_mouse = cfg.enable_mouse;
    self->use_block_chars = cfg.use_block_chars;
    self->next_target_id = 1;

    try_(VT100_initTerminal(self));

    return_ok(*self);
} $unguarded_(fn);

fn_((engine_core_VT100_fini(engine_core_VT100* self))(void)) {
    claim_assert_nonnull(self);

    if (self->target.active) {
        VT100_destroyTarget((engine_Backend_Impl*)self, self->target.id);
    }

    VT100_finiTerminal(self);

    let gpa = self->gpa;
    mem_Allocator_free(gpa, (S$u8){
                                .ptr = (u8*)self,
                                .len = sizeof(engine_core_VT100),
                            });
}

fn_((engine_core_VT100_backend(engine_core_VT100* self))(engine_Backend)) {
    claim_assert_nonnull(self);

    return (engine_Backend){
        .ptr = (engine_Backend_Impl*)self,
        .vt = &g_vt100_vt,
    };
}
#endif /* UNUSED_CODE */
