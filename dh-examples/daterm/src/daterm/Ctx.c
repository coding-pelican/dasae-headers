#include "daterm/Ctx.h"
#include "daterm/Event.h"
#include "daterm/utils.h"
#include <dh/ascii.h>
#include <dh/utf8.h>
#include <dh/fs/File.h>
#include <dh/time/Instant.h>

/*========== Internal Declarations ==========================================*/

$static fn_((daterm_Ctx__nextEvent(fs_File_Handle input))(O$daterm_Event));

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((daterm_Ctx__unsupported_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((daterm_Ctx__unsupported_fini(P$$(daterm_Ctx) self))(E$void));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((daterm_Ctx__unsupported__timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((daterm_Ctx__unsupported__queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos));
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((daterm_Ctx__unsupported__queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__windows_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__windows_fini(P$$(daterm_Ctx) self))(E$void));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__windows__timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__windows__queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__windows__queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size));
));
pp_if_(plat_is_posix)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__posix_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__posix_fini(P$$(daterm_Ctx) self))(E$void));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__posix__timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__posix__queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos));
    $attr($inline_always $must_check)
    $static fn_((daterm_Ctx__posix__queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size));
));

$static let daterm_Ctx__init = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows_init),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix_init),
        pp_else_(daterm_Ctx__unsupported_init)
    )));
$static let daterm_Ctx__fini = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows_fini),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix_fini),
        pp_else_(daterm_Ctx__unsupported_fini)
    )));
$static let daterm_Ctx___timedWait = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows__timedWait),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix__timedWait),
        pp_else_(daterm_Ctx__unsupported__timedWait)
    )));
$static let daterm_Ctx___queryCursorPos = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows__queryCursorPos),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix__queryCursorPos),
        pp_else_(daterm_Ctx__unsupported__queryCursorPos)
    )));
$static let daterm_Ctx___queryScreenSize = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows__queryScreenSize),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix__queryScreenSize),
        pp_else_(daterm_Ctx__unsupported__queryScreenSize)
    )));


/*========== External Definitions ===========================================*/

fn_((daterm_Ctx_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx)) {
    return daterm_Ctx__init(input, output);
};

fn_((daterm_Ctx_fini(daterm_Ctx* self))(E$void) $scope) {
    claim_assert_nonnull(self);
    if (self->is_raw) { try_(daterm_Ctx__fini(self)); }
    asg_l((self)(cleared()));
    return_ok({});
} $unscoped(fn);

fn_((daterm_Ctx_enableMouse(daterm_Ctx* self, io_Writer writer))(E$void) $scope) {
    if (!self->mouse_enabled) {
        try_(daterm_utils_formatCSIWrite(u8_l(daterm_utils_mouse_enable), writer));
        try_(daterm_utils_formatCSIWrite(u8_l(daterm_utils_mouse_sgr_enable), writer));
        self->mouse_enabled = true;
    }
    return_ok({});
} $unscoped(fn);

fn_((daterm_Ctx_disableMouse(daterm_Ctx* self, io_Writer writer))(E$void) $scope) {
    if (self->mouse_enabled) {
        try_(daterm_utils_formatCSIWrite(u8_l(daterm_utils_mouse_sgr_disable), writer));
        try_(daterm_utils_formatCSIWrite(u8_l(daterm_utils_mouse_disable), writer));
        self->mouse_enabled = false;
    }
    return_ok({});
} $unscoped(fn);

fn_((daterm_Ctx_poll(const daterm_Ctx* self))(O$daterm_Event)) {
    claim_assert(self->is_raw);
    return daterm_Ctx__nextEvent(self->input);
};

fn_((daterm_Ctx_wait(const daterm_Ctx* self))(daterm_Event)) {
    claim_assert(self->is_raw);
    while (true) {
        return orelse_((daterm_Ctx__nextEvent(self->input))({
            /* Small sleep to avoid busy-waiting */
            time_sleep(time_Duration_fromNanos(time_nanos_per_milli));
            continue;
        }));
    }
};

fn_((daterm_Ctx_timedWait(const daterm_Ctx* self, time_Duration timeout))(daterm_Ctx_E$daterm_Event)) {
    claim_assert(self->is_raw);
    return daterm_Ctx___timedWait(self->input, timeout);
};

fn_((daterm_Ctx_queryCursorPos(const daterm_Ctx* self))(E$daterm_cursor_Pos)) {
    claim_assert(self->is_raw);
    return daterm_Ctx___queryCursorPos(self->input, self->output);
};

fn_((daterm_Ctx_queryScreenSize(const daterm_Ctx* self))(E$daterm_screen_Size)) {
    claim_assert(self->is_raw);
    return daterm_Ctx___queryScreenSize(self->input, self->output);
};

/*========== Internal Definitions ===========================================*/

$attr($maybe_unused $inline_always)
$static fn_((daterm_Ctx__unsupported__pollImmediately(fs_File_Handle input))(O$daterm_Event));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((daterm_Ctx__windows__pollImmediately(fs_File_Handle input))(O$daterm_Event));
));
pp_if_(plat_is_posix)(pp_then_(
    $attr($inline_always)
    $static fn_((daterm_Ctx__posix__pollImmediately(fs_File_Handle input))(O$daterm_Event));
));
$static let daterm_Ctx___pollImmediately = pp_if_(plat_is_windows)(
    pp_then_(daterm_Ctx__windows__pollImmediately),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(daterm_Ctx__posix__pollImmediately),
        pp_else_(daterm_Ctx__unsupported__pollImmediately)
    )));
$static fn_((daterm_Ctx__pollImmediate(fs_File_Handle input))(O$daterm_Event)) {
    return daterm_Ctx___pollImmediately(input);
};

fn_((daterm_Ctx__nextEvent(fs_File_Handle input))(O$daterm_Event) $scope) {
    let_(input_file, fs_File) = { .handle = input };
    let reader = fs_File_reader(input_file);

    let c0 = catch_((io_Reader_readByte(reader))($ignore, return_none()));
    switch (c0) {
    case 0x1B: /* ESC */ {
        return_(daterm_Ctx__pollImmediate(input));
    } break;
    case 0x08:
    case 0x7F: {
        return_some(union_of((daterm_Event_key){
            .code = daterm_Event_KeyCode_backspace,
            .codepoint = 0,
            .mods = cleared(),
        }));
    } break;
    case 0x09: {
        return_some(union_of((daterm_Event_key){
            .code = daterm_Event_KeyCode_tab,
            .codepoint = 0,
            .mods = cleared(),
        }));
    } break;
    case 0x0A:
    case 0x0D: {
        return_some(union_of((daterm_Event_key){
            .code = daterm_Event_KeyCode_enter,
            .codepoint = 0,
            .mods = cleared(),
        }));
    } break;
    case 0x01: /* Ctrl+A through Ctrl+Z (excluding special keys) */
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
        $fallthrough;
    case 0x0B:
    case 0x0C:
        $fallthrough;
    case 0x0E:
    case 0x1A: {
        let ch = as$(u32)(c0 - 0x01 + 'a');
        return_some(union_of((daterm_Event_key){
            .code = daterm_Event_KeyCode_char,
            .codepoint = ch,
            .mods = { .ctrl = true },
        }));
    } break;
    default: /* UTF-8 character */ {
        let len = catch_((utf8_byteSeqLen(c0))($ignore, return_none()));
        var_(buf, A$$(4, u8)) = A_init({ [0] = c0 });
        for_(($s(A_slice((buf)$r(1, len))))(byte)) {
            *byte = catch_((io_Reader_readByte(reader))($ignore, return_none()));
        } $end(for);
        let ch = catch_((utf8_decode(A_prefix$((S$u8)(buf)(len)).as_const))($ignore, return_none()));
        let ch_is_upper = ascii_isUpper(*A_at((buf)[0]));
        return_some(union_of((daterm_Event_key){
            .code = daterm_Event_KeyCode_char,
            .codepoint = ch + (ch_is_upper ? 32 : 0),
            .mods = { .shift = ch_is_upper },
        }));
    } break;
    }
} $unscoped(fn);

/* --- Unsupported --- */

fn_((daterm_Ctx__unsupported_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx) $scope) {
    let_ignore = input;
    let_ignore = output;
    return_err(E_cause$daterm_Ctx_Unsupported());
} $unscoped(fn);

fn_((daterm_Ctx__unsupported_fini(P$$(daterm_Ctx) self))(E$void) $scope) {
    let_ignore = self;
    return_err(E_cause$daterm_Ctx_Unsupported());
} $unscoped(fn);

fn_((daterm_Ctx__unsupported__timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event) $scope) {
    let_ignore = input;
    let_ignore = timeout;
    return_err(E_cause$daterm_Ctx_Unsupported());
} $unscoped(fn);

fn_((daterm_Ctx__unsupported__queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos) $scope) {
    let_ignore = input;
    let_ignore = output;
    return_err(E_cause$daterm_Ctx_Unsupported());
} $unscoped(fn);

fn_((daterm_Ctx__unsupported__queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size) $scope) {
    let_ignore = input;
    let_ignore = output;
    return_err(E_cause$daterm_Ctx_Unsupported());
} $unscoped(fn);

fn_((daterm_Ctx__unsupported__pollImmediately(fs_File_Handle input))(O$daterm_Event) $scope) {
    let_ignore = input;
    return_none();
} $unscoped(fn);

/* --- Windows --- */

#if plat_is_windows
#include <dh/os/windows/console.h>
#include <io.h>

T_use_E$(HANDLE);
$attr($must_check)
$static fn_((daterm_Ctx__windows__getHandle(fs_File_Handle handle))(E$HANDLE) $scope) {
    let getted = ptrToInt(handle) <= 1
                  ? ptrToInt(handle) == 0
                      ? io_getStdIn().handle
                      : io_getStdOut().handle
                      : handle;
    if (getted == INVALID_HANDLE_VALUE) { return_err(E_cause$Unexpected()); }
    return_ok(getted);
} $unscoped(fn);

$attr($must_check)
$static fn_((daterm_Ctx__windows__enableRawMode(HANDLE handle, DWORD* old_mode))(E$void) $scope) {
    claim_assert_nonnull(old_mode);
    if (!GetConsoleMode(handle, old_mode)) { return_err(E_cause$Unexpected()); }
    // Set raw mode: enable mouse and window events, disable line input/echo
    let new_mode = as$(DWORD)(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
    if (!SetConsoleMode(handle, new_mode)) { return_err(E_cause$Unexpected()); }
    return_ok({});
} $unscoped(fn);

$attr($must_check)
$static fn_((daterm_Ctx__windows__enableVTerms(HANDLE handle, DWORD* old_mode))(E$void) $scope) {
    claim_assert_nonnull(old_mode);
    if (!GetConsoleMode(handle, old_mode)) { return_err(E_cause$Unexpected()); }
    // Enable Virtual Terminal Processing for ANSI escape sequences
    // Reference: https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    let new_mode = *old_mode | as$(DWORD)(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!SetConsoleMode(handle, new_mode)) { return_err(E_cause$Unexpected()); }
    return_ok({});
} $unscoped(fn);

fn_((daterm_Ctx__windows_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx) $scope) {
    let in_handle = try_(daterm_Ctx__windows__getHandle(input));
    let out_handle = try_(daterm_Ctx__windows__getHandle(output));
    var_(old_in_mode, DWORD) = 0;
    try_(daterm_Ctx__windows__enableRawMode(in_handle, &old_in_mode));
    var_(old_out_mode, DWORD) = 0;
    try_(daterm_Ctx__windows__enableVTerms(out_handle, &old_out_mode));
    return_ok({
        .input = input,
        .output = output,
        .impl = {
            .old_in_mode = old_in_mode,
            .old_out_mode = old_out_mode,
        },
        .is_raw = true,
        .mouse_enabled = false,
    });
} $unscoped(fn);

fn_((daterm_Ctx__windows_fini(P$$(daterm_Ctx) self))(E$void) $scope) {
    claim_assert_nonnull(self);
    let in_handle = try_(daterm_Ctx__windows__getHandle(self->input));
    if (!SetConsoleMode(in_handle, self->impl.old_in_mode)) { return_err(E_cause$Unexpected()); }
    self->impl.old_in_mode = 0;
    asg_l((&self->input)(cleared()));
    let out_handle = try_(daterm_Ctx__windows__getHandle(self->output));
    if (!SetConsoleMode(out_handle, self->impl.old_out_mode)) { return_err(E_cause$Unexpected()); }
    self->impl.old_out_mode = 0;
    asg_l((&self->output)(cleared()));
    self->is_raw = false;
    return_ok({});
} $unscoped(fn);

fn_((daterm_Ctx__windows__timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event) $scope) {
    let instant = time_Instant_now();
    var_(event, O$daterm_Event) = none();
    while_none((event = daterm_Ctx__nextEvent(input))) {
        let elapsed = time_Instant_elapsed(instant);
        if (time_Duration_gt(elapsed, timeout)) {
            return_err(E_cause$daterm_Ctx_Timeout());
        }
        let remaining = time_Duration_sub(timeout, elapsed);
        time_sleep(remaining);
    }
    return_ok(unwrap_(event));
} $unscoped(fn);

fn_((daterm_Ctx__windows__queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size) $scope) {
    let_ignore = input;
    let out_handle = try_(daterm_Ctx__windows__getHandle(output));
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) = cleared();
    if (!GetConsoleScreenBufferInfo(out_handle, &csbi)) { return_err(E_cause$Unexpected()); }
    return_ok({
        .cols = as$(u16)(csbi.srWindow.Right - csbi.srWindow.Left + 1),
        .rows = as$(u16)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1),
    });
} $unscoped(fn);

fn_((daterm_Ctx__windows__queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos) $scope) {
    let_ignore = input;
    let out_handle = try_(daterm_Ctx__windows__getHandle(output));
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) = cleared();
    if (!GetConsoleScreenBufferInfo(out_handle, &csbi)) { return_err(E_cause$Unexpected()); }
    // dwCursorPosition is 0-based, daterm uses 1-based
    return_ok({
        .row = as$(u16)(csbi.dwCursorPosition.Y + 1),
        .col = as$(u16)(csbi.dwCursorPosition.X + 1),
    });
} $unscoped(fn);

// Check console input events
fn_((daterm_Ctx__windows__pollImmediately(fs_File_Handle input))(O$daterm_Event) $scope) {
    let handle = catch_((daterm_Ctx__windows__getHandle(input))($ignore, return_none()));
    var_(events, DWORD) = 0;
    if (GetNumberOfConsoleInputEvents(handle, &events) && events > 1) {
        return_(daterm_Event_parseEscSeq(fs_File_reader(initial$((fs_File)(.handle = input)))));
    }
    return_none();
} $unscoped(fn);
#endif /* plat_is_windows */

/* --- POSIX --- */

#if plat_is_posix
#include <sys/ioctl.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

fn_((daterm_Ctx__posix_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx) $scope) {
    let in_fd = as$(int)(input);
    let out_fd = as$(int)(output);
    // Get current terminal settings for input
    struct termios old_in_termios;
    if (tcgetattr(in_fd, &old_in_termios) < 0) {
        return_err(E_cause$Unexpected());
    }
    // Get current terminal settings for output
    struct termios old_out_termios;
    if (tcgetattr(out_fd, &old_out_termios) < 0) {
        return_err(E_cause$Unexpected());
    }
    // Configure raw mode for input (based on cfmakeraw)
    var new_in_termios = old_in_termios;
    // Input flags
    new_in_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // Output flags
    new_in_termios.c_oflag &= ~(OPOST);
    // Local flags
    new_in_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // Control flags
    new_in_termios.c_cflag &= ~(CSIZE | PARENB);
    new_in_termios.c_cflag |= CS8;
    // Control characters: immediate read
    new_in_termios.c_cc[VMIN] = 1;
    new_in_termios.c_cc[VTIME] = 0;
    // Apply input settings
    if (tcsetattr(in_fd, TCSAFLUSH, &new_in_termios) < 0) {
        return_err(E_cause$Unexpected());
    }
    // Configure output (disable OPOST for raw output)
    var new_out_termios = old_out_termios;
    new_out_termios.c_oflag &= ~(OPOST);
    if (tcsetattr(out_fd, TCSAFLUSH, &new_out_termios) < 0) {
        // Restore input on failure
        tcsetattr(in_fd, TCSAFLUSH, &old_in_termios);
        return_err(E_cause$Unexpected());
    }
    return_ok({
        .input = input,
        .output = output,
        .impl = {
            .old_in_termios = old_in_termios,
            .old_out_termios = old_out_termios,
        },
        .is_raw = true,
        .mouse_enabled = false,
    });
} $unscoped(fn);

fn_((daterm_Ctx__posix_fini(P$$(daterm_Ctx) self))(E$void) $scope) {
    claim_assert_nonnull(self);
    let in_fd = as$(int)(self->input);
    let out_fd = as$(int)(self->output);
    if (tcsetattr(in_fd, TCSAFLUSH, &self->impl.old_in_termios) < 0) { return_err(E_cause$Unexpected()); }
    if (tcsetattr(out_fd, TCSAFLUSH, &self->impl.old_out_termios) < 0) { return_err(E_cause$Unexpected()); }
    self->is_raw = false;
    return_ok({});
} $unscoped(fn);

/* --- Timed Wait --- */

fn_((daterm_Ctx__posix_timedWait(fs_File_Handle input, time_Duration timeout))(daterm_Ctx_E$daterm_Event) $scope) {
    let fd = as$(int)(input);
    let timeout_ms = as$(int)(time_Duration_toMillis(timeout));

    struct pollfd pfd = {
        .fd = fd,
        .events = POLLIN,
        .revents = 0,
    };

    while (true) {
        // Poll for events
        let result = poll(&pfd, 1, timeout_ms);

        if (result < 0) {
            return_err(E_cause$Unexpected());
        }

        if (result == 0) {
            // Timeout
            return_err(E_cause$daterm_Ctx_Timeout());
        }

        if (pfd.revents & POLLIN) {
            // Data available, try to read event
            let event_opt = daterm_Ctx__nextEvent(input);
            if (isSome(event_opt)) {
                return_ok(unwrap_(event_opt));
            }
        }
    }
} $unscoped(fn);

/* --- Queries (ANSI + ioctl) --- */

fn_((daterm_Ctx__posix_queryScreenSize(fs_File_Handle input, fs_File_Handle output))(E$daterm_screen_Size) $scope) {
    let_ignore = input;
    let out_fd = as$(int)(output);

    // Use ioctl for fast system query
    struct winsize ws;
    if (ioctl(out_fd, TIOCGWINSZ, &ws) < 0) {
        return_err(E_cause$Unexpected());
    }

    return_ok({
        .cols = as$(u16)(ws.ws_col),
        .rows = as$(u16)(ws.ws_row),
    });
} $unscoped(fn);

fn_((daterm_Ctx__posix_queryCursorPos(fs_File_Handle input, fs_File_Handle output))(E$daterm_cursor_Pos) $scope) {
    // POSIX has no system API for cursor position - must use ANSI CPR
    let in_file = fs_File_fromHandle(input);
    let out_file = fs_File_fromHandle(output);

    return daterm_cursor_queryPos(fs_File_reader(in_file), fs_File_writer(out_file));
} $unscoped(fn);

fn_((daterm_Ctx__posix__pollImmediately(fs_File_Handle input))(O$daterm_Event) $scope) {
    struct pollfd pfd = { .fd = as$(i32)(input), .events = POLLIN };
    if (poll(&pfd, 1, 0) > 0) {
        return daterm_utils_parseEscSeq(reader);
    }
} $unscoped(fn);
#endif /* plat_is_posix */
