/* vt100_stub.c
 *
 * Minimal stub process that runs inside Windows Terminal.
 * Responsibilities:
 *   - Read framebuffer from shared memory
 *   - Perform ANSI formatting and WriteConsoleA
 *   - Capture console input and send to engine via shared memory
 *   - Convert mouse position to logical grid coordinates (half-block)
 */

#include "VT100Ex.h"
#include "dh/main.h"
#if plat_windows
#if BUILTIN_COMP_MSVC
#pragma comment(lib, "user32.lib")
#endif /* BUILTIN_COMP_MSVC */
#include "dh/os/windows.h"
#include "dh/heap/Page.h"
#include "dh/io/common.h"
#include "dh/log.h"
#include "dh/time.h"
#include "dh/math/vec.h"

/* ========== Global State ================================================== */

typedef struct Inst {
    var_(shared_mapping, HANDLE);
    var_(shared_state, engine_stub_VT100Ex_SharedState*);
    var_(console_out, fs_File);
    var_(console_in, fs_File);
    var_(console_wnd, HWND);
    var_(ansi_buf, S$u8);

    LONG last_rendered_frame;
    u32 last_metrics_version;
    /* Previous mouse button states for edge detection */
    u8 prev_mouse_buttons[8];
} Inst;
T_use$((Inst)(E));
$must_check
$static fn_((Inst_init(mem_Allocator gpa))(E$Inst));
$static fn_((Inst_fini(Inst* self, mem_Allocator gpa))(void));

typedef fn_(((*)(const Inst* self))(bool) $T) Inst_WaitPredicate;
$static fn_((Inst_waitFor(const Inst* self, Inst_WaitPredicate predicate))(void));

$must_check
$static fn_((Inst_run(Inst* self))(E$void));

/* ========== Main Loop ===================================================== */

$static fn_((engineToInitMetrics(const Inst* self))(bool)) {
    return self->shared_state->metrics.grid_width != 0 && !self->shared_state->control.should_exit;
};

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var page = lit$((heap_Page){});
    let gpa = heap_Page_allocator(&page);
    var inst = try_(Inst_init(gpa));
    defer_(Inst_fini(&inst, gpa));
    Inst_waitFor(&inst, engineToInitMetrics);
    return_ok(try_(Inst_run(&inst)));
} $unguarded_(fn);

/* ========== Initialization ================================================ */

fn_((Inst_init(mem_Allocator gpa))(E$Inst) $guard) {
    // Open shared memory mapping
    let shared_mapping = expr_(HANDLE $scope) if_(let mapping = OpenFileMappingW(
        FILE_MAP_ALL_ACCESS,
        false,
        engine_stub_VT100Ex_shared_mem_name
    ), !mapping) {
        log_error("Failed to open shared memory: %lu", GetLastError());
        return_err(Err_Unspecified());
    } else {
        $break_(mapping);
    } $unscoped_(expr);
    errdefer_($ignore, CloseHandle(shared_mapping));

    // Map shared state
    typedef engine_stub_VT100Ex_SharedState SharedState;
    let shared_state = expr_(SharedState* $scope) if_(let state = as$(SharedState*)(MapViewOfFile(
        shared_mapping,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        sizeOf$(engine_stub_VT100Ex_SharedState)
    )), !state) {
        log_error("Failed to map shared memory: %lu", GetLastError());
        return_err(Err_Unspecified());
    } else {
        $break_(state);
    } $unscoped_(expr);
    errdefer_($ignore, UnmapViewOfFile(shared_state));
    /* Register stub PID */
    shared_state->control.stub_pid = GetCurrentProcessId();

    /* Configure console output mode (enable VT100 sequences) */
    let console_out = io_getStdOut();
    if_(DWORD out_mode = 0, !GetConsoleMode(console_out.handle, &out_mode)) {
        log_error("Failed to get console output mode: %lu", GetLastError());
        return_err(Err_Unspecified());
    } else {
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        out_mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
        if (!SetConsoleMode(console_out.handle, out_mode)) {
            log_error("Failed to set console output mode: %lu", GetLastError());
            return_err(Err_Unspecified());
        }
    }

    // Configure console input mode
    let console_in = io_getStdIn();
    if_(DWORD in_mode = 0, !GetConsoleMode(console_in.handle, &in_mode)) {
        log_error("Failed to get console input mode: %lu", GetLastError());
        return_err(Err_Unspecified());
    } else {
        in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
        in_mode &= ~ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(console_in.handle, in_mode)) {
            log_error("Failed to set console input mode: %lu", GetLastError());
            return_err(Err_Unspecified());
        }
    }

    let console_wnd = GetConsoleWindow();

    // Hide cursor
    if (!SetConsoleCursorInfo(console_out.handle, &lit$((CONSOLE_CURSOR_INFO){ 1, false }))) {
        log_error("Failed to hide console cursor: %lu", GetLastError());
        return_err(Err_Unspecified());
    }

    // Set UTF-8 code page
    if_(DWORD out_cp = GetConsoleOutputCP(), out_cp != CP_UTF8 && !SetConsoleOutputCP(CP_UTF8)) {
        log_error("Failed to set UTF-8 code page: %lu", GetLastError());
        return_err(Err_Unspecified());
    }

    // Allocate ANSI buffer
    //   Max size: width * height/2 * (max_ansi_seq + utf8_char) + height/2 * newline + home_seq
    //   Conservative: 30 bytes per cell * max_cells
    let ansi_buf_len = engine_stub_VT100Ex_max_width * (engine_stub_VT100Ex_max_height / 2) * 30 + 1024;
    let ansi_buf = u_castS$((S$u8)(catch_((mem_Allocator_alloc(gpa, typeInfo$(u8), ansi_buf_len))(err, {
        log_error("Failed to allocate ANSI buffer: %lu", err);
        return_err(err);
    }))));
    errdefer_($ignore, mem_Allocator_free(gpa, u_anyS(ansi_buf)));

    return_ok({
        .shared_mapping = shared_mapping,
        .shared_state = shared_state,
        .ansi_buf = ansi_buf,
        .console_out = console_out,
        .console_in = console_in,
        .console_wnd = console_wnd,
        .last_rendered_frame = 0,
        .last_metrics_version = 0,
        .prev_mouse_buttons = { 0 },
    });
} $unguarded_(fn);

fn_((Inst_fini(Inst* self, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    mem_Allocator_free(gpa, u_anyS(self->ansi_buf));
    UnmapViewOfFile(self->shared_state);
    CloseHandle(self->shared_mapping);
};

fn_((Inst_waitFor(const Inst* self, Inst_WaitPredicate predicate))(void)) {
    while (!predicate(self)) { time_sleep(time_Duration_fromMillis(10)); }
};

$static fn_((renderFrame(const Inst* self, const engine_stub_VT100Ex_FrameBuffer* frame))(void));
$static fn_((captureInput(Inst* self))(void));
fn_((Inst_run(Inst* self))(E$void) $scope) {
    while (!self->shared_state->control.should_exit) {
        // Render any new frames
        for (usize buf_idx = 0; buf_idx < 2; ++buf_idx) {
            if (InterlockedCompareExchange(&self->shared_state->frame.ready[buf_idx], 0, 0)) {
                let frame = &self->shared_state->frame.bufs[buf_idx];
                // Only render if frame number increased (avoid re-rendering same frame)
                if ((LONG)frame->frame_number > self->last_rendered_frame) {
                    renderFrame(self, frame);
                    self->last_rendered_frame = (LONG)frame->frame_number;
                    InterlockedIncrement(&self->shared_state->stats.frames_rendered);
                }
                // Mark buffer as consumed
                InterlockedExchange(&self->shared_state->frame.ready[buf_idx], 0);
            }
        }
        // Capture input
        captureInput(self);
        // Update stub heartbeat
        InterlockedIncrement(&self->shared_state->control.stub_alive);
        // Yield CPU (1000+ Hz loop rate is plenty)
        time_sleep(time_Duration_fromMillis(1));
    }
    return_ok({});
} $unscoped_(fn);

/* ========== ANSI Rendering ================================================ */

typedef engine_stub_VT100Ex_Color Color;
/// Writes the cursor to the home position sequence "\033[H"
$inline_always
$static fn_((writeSeqCursorToHome(S$u8 writable_buf))(usize));
/// Direct ANSI sequence builder "\033[38;2;R;G;B;48;2;R;G;Bm"
$inline_always
$static fn_((writeSeqColor(S$u8 writable_buf, Color upper, Color lower))(usize));
/// Writes the half-block character "▀"
$inline_always
$static fn_((writeHalfBlock(S$u8 writable_buf))(usize));
/// Writes the newline character "\n"
$inline_always
$static fn_((writeNL(S$u8 writable_buf))(usize));
/// Flushes the buffer to the console
$inline_always
$static fn_((flushBuf(fs_File console_out, S$u8 written_buf))(void));
/// Presents the current buffer content to the console.
/// For a text-based approach, you could simply write the stored data to the console.
/// If you’re storing color/ANSI sequences, handle them accordingly.
fn_((renderFrame(const Inst* self, const engine_stub_VT100Ex_FrameBuffer* frame))(void)) {
    u32 width = frame->width;
    u32 height = frame->height;
    let pixels = frame->pixels;

    let console_out = self->console_out;
    let writable_buf = self->ansi_buf;
    var unwritten_buf = writable_buf;
    unwritten_buf = suffixS(unwritten_buf, writeSeqCursorToHome(unwritten_buf));
    // Render each pair of rows as one console row (half-block rendering)
    for (u32 y = 0; y + 1 < height; y += 2) {
        for (u32 x = 0; x < width;) {
            let upper = pixels[y * width + x];
            let lower = pixels[(y + 1) * width + x];
            // Run-length encoding: find consecutive identical color pairs
            u32 run_length = 1;
            while (x + run_length < width) {
                let next_upper = pixels[y * width + x + run_length];
                let next_lower = pixels[(y + 1) * width + x + run_length];
                if (memcmp(&upper, &next_upper, sizeOf$(Color)) != 0
                || memcmp(&lower, &next_lower, sizeOf$(Color)) != 0) {
                    break;
                }
                run_length++;
            }
            unwritten_buf = suffixS(unwritten_buf, writeSeqColor(unwritten_buf, upper, lower));
            for_(($r(0, run_length))($ignore) {
                unwritten_buf = suffixS(unwritten_buf, writeHalfBlock(unwritten_buf));
            });
            x += run_length - 1; // -1 because loop will increment
        }
        unwritten_buf = suffixS(unwritten_buf, writeNL(unwritten_buf));
    }
    let written_buf = prefixS(writable_buf, writable_buf.len - unwritten_buf.len);
    flushBuf(console_out, written_buf);
};

T_use_mem_copyFwds$(u8);
$inline_always
$static fn_((writeSeqCursorToHome(S$u8 writable_buf))(usize)) {
    $static let cursor_to_home = u8_l("\033[H");
    mem_copyFwds$u8(writable_buf, cursor_to_home);
    return cursor_to_home.len;
};

$inline_always
$static fn_((formatU8(S$u8 buf, u8 val))(usize));
$inline_always
$static fn_((writeSeqColor(S$u8 writable_buf, Color upper, Color lower))(usize)) {
    var unwritten = writable_buf;
    $static let cmd_sep = u8_c(';');
    $static let cmd_seq_begin = u8_l("\033[");
    unwritten = suffixS(mem_copyFwds$u8(unwritten, cmd_seq_begin), cmd_seq_begin.len);
    $static let cmd_upper = u8_l("38;2;");
    unwritten = suffixS(mem_copyFwds$u8(unwritten, cmd_upper), cmd_upper.len);
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.r));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.g));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, upper.b));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    $static let cmd_lower = u8_l("48;2;");
    unwritten = suffixS(mem_copyFwds$u8(unwritten, cmd_lower), cmd_lower.len);
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.r));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.g));
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_sep, 1));
    unwritten = suffixS(unwritten, formatU8(unwritten, lower.b));
    let cmd_seq_end = u8_c('m');
    unwritten = suffixS(unwritten, (*S_at((unwritten)[0]) = cmd_seq_end, 1));
    return writable_buf.len - unwritten.len;
};

/// Fast integer-to-string for 0-255 range
$static let digit_pairs = u8_a(
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
);
$inline_always
$static fn_((formatU8(S$u8 buf, u8 val))(usize)) {
    if (val >= 100) {
        u8 hi = val / 100;
        u8 lo = val % 100;
        *S_at((buf)[0]) = '0' + hi;
        *S_at((buf)[1]) = *A_at((digit_pairs)[lo * 2]);
        *S_at((buf)[2]) = *A_at((digit_pairs)[lo * 2 + 1]);
        return 3;
    }
    if (val >= 10) {
        *S_at((buf)[0]) = *A_at((digit_pairs)[val * 2]);
        *S_at((buf)[1]) = *A_at((digit_pairs)[val * 2 + 1]);
        return 2;
    }
    *S_at((buf)[0]) = '0' + val;
    return 1;
};

$inline_always
$static fn_((writeHalfBlock(S$u8 writable_buf))(usize)) {
    $static let half_block = u8_l("▀") ;
    mem_copyFwds$u8(writable_buf, half_block);
    return half_block.len;
};
$inline_always
$static fn_((writeNL(S$u8 writable_buf))(usize)) {
    $static let nl = u8_l("\n");
    mem_copyFwds$u8(writable_buf, nl);
    return nl.len;
};
$inline_always
$static fn_((flushBuf(fs_File console_out, S$u8 written_buf))(void)) {
    if_(DWORD written = 0, !WriteConsoleA(console_out.handle, written_buf.ptr, written_buf.len, &written, null) || written != written_buf.len) {
        log_error("Failed to write console buffer content: %d", GetLastError());
        claim_unreachable;
    }
}

/* ========== Input Capture ================================================= */

$static fn_((pushInputEvent(Inst* self, engine_stub_VT100Ex_InputEvent event))(void));
fn_((captureInput(Inst* self))(void)) {
    if_(DWORD available = 0, !GetNumberOfConsoleInputEvents(self->console_in.handle, &available)) {
        log_error("Failed to get number of console input events: %d", GetLastError());
        return;
    } else if (available == 0) {
        return;
    }

    A$$(64, INPUT_RECORD) records_buf = A_zero();
    let records = expr_(S$$(INPUT_RECORD) $scope) {
        if_(DWORD read_count = 0, !ReadConsoleInputW(self->console_in.handle, records_buf.val, A_len(records_buf), &read_count)) {
            log_error("Failed to read console input: %d", GetLastError());
            return;
        } else {
            $break_(prefixS(A_ref$((BreakType)records_buf), read_count));
        }
    } $unscoped_(expr);
    typedef engine_stub_VT100Ex_InputEvent InputEvent;
    for_(($s(records))(record) switch (record->EventType) {
        case KEY_EVENT: {
            let data = lit$((engine_stub_VT100Ex_InputEvent_KeyboardKey){
                .vk_code = record->Event.KeyEvent.wVirtualKeyCode,
                .scan_code = record->Event.KeyEvent.wVirtualScanCode,
                .shift = (record->Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) != 0,
                .ctrl = (record->Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0,
                .alt = (record->Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0,
            });
            let timestamp = time_Instant_now();
            if (record->Event.KeyEvent.bKeyDown) {
                pushInputEvent(self, lit$((InputEvent){
                    .timestamp = timestamp,
                    .data = union_of((engine_stub_VT100Ex_InputEvent_key_down)data),
                }));
            } else {
                pushInputEvent(self, lit$((InputEvent){
                    .timestamp = timestamp,
                    .data = union_of((engine_stub_VT100Ex_InputEvent_key_up)data),
                }));
            }
        } break;

        case MOUSE_EVENT:
            /* Mouse event data - handled by engine hook */
        case WINDOW_BUFFER_SIZE_EVENT:
            /* Console buffer resized - metrics will be updated by engine */
        default: {
        } break;
    });
};

fn_((pushInputEvent(Inst* self, engine_stub_VT100Ex_InputEvent event))(void)) {
    engine_stub_VT100Ex_InputQueue_push(
        &self->shared_state->input_queue.head,
        &self->shared_state->input_queue.tail,
        self->shared_state->input_queue.events,
        &event
    );
    InterlockedIncrement(&self->shared_state->stats.input_events_produced);
};

#endif /* plat_windows */
