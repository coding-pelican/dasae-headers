#include "daterm-context/ANSI.h"
#include "daterm-context/ANSI/private.h"
#include <dansi-core.h>
#include <dansi-xterm.h>
#include <dh/fs/File.h>
#include <dh/mem/common.h>
#include <dh/time/Dur.h>
#include <dh/time/self/Awake.h>
#include <dh/utf8.h>

/*========== Internal Declarations ==========================================*/

$static fn_((daterm_ANSI__readerOf(daterm_ANSI* self))(io_Reader));
$static fn_((daterm_ANSI__writerOf(const daterm_ANSI* self))(io_Writer));

$static fn_((daterm_ANSI__poll(P$raw ctx))(O$daterm_Event));
$static fn_((daterm_ANSI__wait(P$raw ctx))(Sched_Cancelable$daterm_Event));
$static fn_((daterm_ANSI__waitTimed(P$raw ctx, time_Dur timeout))(daterm_Term_WaitE$daterm_Event));
$static fn_((daterm_ANSI__waitProtn(P$raw ctx))(daterm_Event));
$static fn_((daterm_ANSI__reader(P$raw ctx))(io_Reader));
$static fn_((daterm_ANSI__writer(P$raw ctx))(io_Writer));
$static fn_((daterm_ANSI__queryScreenSize(P$raw ctx))(E$daterm_Size));
$static fn_((daterm_ANSI__queryCursorPos(P$raw ctx))(E$daterm_Pos));

#if plat_is_windows
#include <dh/sys/api/windows/console.h>
#include <dh/sys/api/windows/sync.h>
T_use_E$(DWORD);
$static fn_((daterm_ANSI__windows_ctrlHandler(DWORD type))(BOOL));
$static fn_((daterm_ANSI__windows_enableRawMode(HANDLE input))(E$DWORD));
$static fn_((daterm_ANSI__windows_disableRawMode(HANDLE input, DWORD old_in))(E$void));
$static fn_((daterm_ANSI__windows_enableVTerm(HANDLE output))(E$DWORD));
$static fn_((daterm_ANSI__windows_disableVTerm(HANDLE output, DWORD old_out))(E$void));
#endif /* plat_is_windows */
#if plat_is_posix
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
T_alias$((posix_termios)(struct termios));
T_use_E$(posix_termios);
$static fn_((daterm_ANSI__posix_enableRawMode(fs_File_Handle handle, daterm_ANSI_OutputMode output_mode))(E$posix_termios));
$static fn_((daterm_ANSI__posix_disableRawMode(fs_File_Handle handle, posix_termios old))(E$void));
#endif /* plat_is_posix */

/*========== External Definitions ===========================================*/

fn_((daterm_ANSI_init(daterm_ANSI_Cfg cfg))(mem_E$daterm_ANSI) $guard) {
    let should_alloc_input_buf = matches(cfg.input_buf, daterm_ANSI_Cfg_input_buf_owned);
    let input_mem = expr_(S$u8 $scope)(if (should_alloc_input_buf) {
        let cap = local_({
            let cfg_input_buf = union_to((cfg.input_buf)(daterm_ANSI_Cfg_input_buf_owned));
            local_return_(cfg_input_buf.cap != 0 ? cfg_input_buf.cap : daterm_ANSI_input_buf_cap_default);
        });
        $break_(try_(mem_Alctr_allocBytes($trace unwrap_(cfg.gpa), cap)));
    } else {
        $break_(union_to((cfg.input_buf)(daterm_ANSI_Cfg_input_buf_fixed)));
    }) $unscoped(expr);
    errdefer_($ignore, if (should_alloc_input_buf) mem_Alctr_freeBytes($trace unwrap_(cfg.gpa), input_mem));

    let should_alloc_report_buf = matches(cfg.report_buf, daterm_ANSI_Cfg_report_buf_owned);
    let report_mem = expr_(S$u8 $scope)(if (should_alloc_report_buf) {
        let cap = local_({
            let cfg_report_buf = union_to((cfg.report_buf)(daterm_ANSI_Cfg_report_buf_owned));
            local_return_(cfg_report_buf.cap != 0 ? cfg_report_buf.cap : daterm_ANSI_report_buf_cap_default);
        });
        $break_(try_(mem_Alctr_allocBytes($trace unwrap_(cfg.gpa), cap)));
    } else {
        $break_(union_to((cfg.report_buf)(daterm_ANSI_Cfg_report_buf_fixed)));
    }) $unscoped(expr);
    errdefer_($ignore, if (should_alloc_report_buf) mem_Alctr_freeBytes($trace unwrap_(cfg.gpa), report_mem));

    return_ok((daterm_ANSI){
        .input_file = cfg.input_file,
        .output_file = cfg.output_file,
        .output_mode = cfg.output_mode,
        .raw_mode_ = none(),
        .is_in_alt_screen = false,
        .is_tracking_mouse = false,
        .input_buf = {
            .reader = io_Buf_Reader_init(fs_File_reader(cfg.input_file), input_mem),
            .is_owned = should_alloc_input_buf,
            .esc_started_at = none(),
            .esc_timeout = cfg.esc_timeout,
        },
        .report_buf = {
            .mem = report_mem,
            .is_owned = should_alloc_report_buf,
        },
        .gpa = cfg.gpa,
        .clock = cfg.clock,
    });
} $unguarded(fn);

fn_((daterm_ANSI_fini(daterm_ANSI* self))(void)) {
    claim_assert_nonnull(self);
    daterm_ANSI_leave(self);
    if (self->input_buf.is_owned) {
        mem_Alctr_freeBytes($trace unwrap_(self->gpa), self->input_buf.reader.buf);
    }
    if (self->report_buf.is_owned) {
        mem_Alctr_freeBytes($trace unwrap_(self->gpa), self->report_buf.mem);
    }
    asg_l((self)(cleared()));
};

fn_((daterm_ANSI_enableRawMode(daterm_ANSI* self))(E$void) $scope) {
    if (isSome(self->raw_mode_)) return_ok({});
#if plat_is_windows
    let old_in = try_(daterm_ANSI__windows_enableRawMode(self->input_file.handle));
    let old_out = try_(daterm_ANSI__windows_enableVTerm(self->output_file.handle));
#elif plat_is_posix
    let old_in = try_(daterm_ANSI__posix_enableRawMode(self->input_file.handle, self->output_mode));
#endif
#if plat_is_windows
    asg_l((&self->raw_mode_)(some({
        .old_in = old_in,
        .old_out = old_out,
    })));
#elif plat_is_posix
    asg_l((&self->raw_mode_)(some({
        .old_in = old_in,
    })));
#endif
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI_disableRawMode(daterm_ANSI* self))(void)) {
    let raw_mode = orelse_((self->raw_mode_)(return));
#if plat_is_windows
    catch_((daterm_ANSI__windows_disableRawMode(self->input_file.handle, raw_mode.old_in))($ignore, $do_nothing));
    catch_((daterm_ANSI__windows_disableVTerm(self->output_file.handle, raw_mode.old_out))($ignore, $do_nothing));
#elif plat_is_posix
    catch_((daterm_ANSI__posix_disableRawMode(self->input_file.handle, raw_mode.old_in))($ignore, $do_nothing));
#endif
    asg_l((&self->raw_mode_)(none()));
};

fn_((daterm_ANSI_isInRawMode(const daterm_ANSI* self))(bool)) {
    return isSome(self->raw_mode_);
};

fn_((daterm_ANSI_enableMouseTracking(daterm_ANSI* self))(E$void) $guard) {
    let out = daterm_ANSI__writerOf(self);
    if (self->is_tracking_mouse) return_ok({});
    try_(dansi_mouse_enableAnyWrite(out));
    errdefer_($ignore, catch_((dansi_mouse_disableAnyWrite(out))($ignore, $do_nothing)));
    try_(dansi_mouse_enableSGRWrite(out));
    self->is_tracking_mouse = true;
    return_ok({});
} $unguarded(fn);

fn_((daterm_ANSI_disableMouseTracking(daterm_ANSI* self))(void)) {
    let out = daterm_ANSI__writerOf(self);
    if (!self->is_tracking_mouse) return;
    catch_((dansi_mouse_disableSGRWrite(out))($ignore, $do_nothing));
    catch_((dansi_mouse_disableAnyWrite(out))($ignore, $do_nothing));
    self->is_tracking_mouse = false;
};

fn_((daterm_ANSI_isTrackingMouse(const daterm_ANSI* self))(bool)) {
    return self->is_tracking_mouse;
};

fn_((daterm_ANSI_term(daterm_ANSI* self))(daterm_Term)) {
    $static let_(vtbl, daterm_Term_VTbl) = {
        .pollFn = daterm_ANSI__poll,
        .waitFn = daterm_ANSI__wait,
        .waitTimedFn = daterm_ANSI__waitTimed,
        .waitProtnFn = daterm_ANSI__waitProtn,
        .readerFn = daterm_ANSI__reader,
        .writerFn = daterm_ANSI__writer,
        .queryScreenSizeFn = daterm_ANSI__queryScreenSize,
        .queryCursorPosFn = daterm_ANSI__queryCursorPos,
    };
    return daterm_Term_ensureValid((daterm_Term){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

fn_((daterm_ANSI_enter(daterm_ANSI* self))(E$void) $guard) {
    try_(daterm_ANSI_enableRawMode(self));
    errdefer_($ignore, daterm_ANSI_disableRawMode(self));
    try_(daterm_ANSI_enableMouseTracking(self));
    return_ok({});
} $unguarded(fn);

fn_((daterm_ANSI_leave(daterm_ANSI* self))(void)) {
    daterm_ANSI_disableMouseTracking(self);
    daterm_ANSI_disableRawMode(self);
};

/*========== Internal Definitions ===========================================*/

fn_((daterm_ANSI__readerOf(daterm_ANSI* self))(io_Reader)) {
    return io_Buf_reader(&self->input_buf.reader);
};

fn_((daterm_ANSI__writerOf(const daterm_ANSI* self))(io_Writer)) {
    return fs_File_writer(self->output_file);
};

$static fn_((daterm_ANSI__asTermEvent(dansi_Event event))(O$daterm_Event) $scope) {
    match_(event) {
    pattern_((dansi_Event_key)(key)) return_some(union_of((daterm_Event_key)(key))) $end(pattern);
    default_() return_none() $end(default);
    } $end(match);
} $unscoped(fn);

$static fn_((daterm_ANSI__asMouseEvent(dansi_mouse_Event event))(daterm_Event)) {
    let mouse = (daterm_Event_Mouse){
        .col = event.x == 0 ? 0 : event.x - 1,
        .row = event.y == 0 ? 0 : event.y - 1,
        .button = as$(daterm_Event_MouseButton)(event.button),
        .action = as$(daterm_Event_MouseAction)(event.action),
        .wheel = as$(daterm_Event_MouseWheel)(event.wheel),
        .mods = event.mods,
    };
    return (daterm_Event)union_of((daterm_Event_mouse)(mouse));
};

$static fn_((daterm_ANSI__inputReady(daterm_ANSI* self))(bool)) {
#if plat_is_windows
    return WaitForSingleObject(self->input_file.handle, 0) == WAIT_OBJECT_0;
#elif plat_is_posix
    struct pollfd pfd = { .fd = as$(i32)(self->input_file.handle), .events = POLLIN, .revents = 0 };
    return poll(&pfd, 1, 0) > 0 && (pfd.revents & POLLIN) != 0;
#else
    let_ignore = self;
    return false;
#endif
};

fn_((daterm_ANSI_pollBufferedSeq(
    io_Buf_Reader* input,
    time_Clock time,
    O$time_Clock_Inst* esc_started_at,
    time_Dur esc_timeout
))(O$dansi_Seq) $scope) {
    let ready = io_Buf_Reader_ready(*input);
    if (ready.len == 0) { return_none(); }

    let first = *S_at((ready)[0]);
    if (first != 0x1B) {
        let len = catch_((utf8_byteSeqLen(first))($ignore, utf8_SeqLen_1));
        if (ready.len < as$(usize)(len)) { return_none(); }
        let bytes = S_prefix((ready)(as$(usize)(len)));
        io_Buf_Reader_drop(input, as$(usize)(len));
        asg_l((esc_started_at)(none()));
        return_some(dansi_Seq_raw(bytes));
    }

    if (ready.len == 1) {
        if (time_Dur_isZero(esc_timeout)) {
            let bytes = S_prefix((ready)(1));
            io_Buf_Reader_drop(input, 1);
            asg_l((esc_started_at)(none()));
            return_some(dansi_Seq_esc(bytes));
        }
        if_none((*esc_started_at)) {
            asg_l((esc_started_at)(some(time_Clock_now(time))));
            return_none();
        }
        let elapsed = time_Clock_Inst_elapsed(unwrap_(*esc_started_at), time);
        if (!time_Dur_gt(elapsed, esc_timeout)) {
            return_none();
        }
        let bytes = S_prefix((ready)(1));
        io_Buf_Reader_drop(input, 1);
        asg_l((esc_started_at)(none()));
        return_some(dansi_Seq_esc(bytes));
    }

    asg_l((esc_started_at)(none()));
    let second = *S_at((ready)[1]);
    switch (second) {
    case '[': {
        var_(idx, usize) = 2;
        while (idx < ready.len) {
            let ch = *S_at((ready)[idx]);
            if (0x40 <= ch && ch <= 0x7E) {
                let len = idx + 1;
                let bytes = S_prefix((ready)(len));
                io_Buf_Reader_drop(input, len);
                return_some(dansi_Seq_csi(bytes));
            }
            idx += 1;
        }
        return_none();
    }
    case 'O': {
        if (ready.len < 3) { return_none(); }
        let bytes = S_prefix((ready)(3));
        io_Buf_Reader_drop(input, 3);
        return_some(dansi_Seq_ss3(bytes));
    }
    case ']':
    case 'P': {
        let_(kind, dansi_Seq_Kind) = second == ']' ? dansi_Seq_Kind_osc
                                                   : dansi_Seq_Kind_dcs;
        var_(idx, usize) = 2;
        while (idx < ready.len) {
            let ch = *S_at((ready)[idx]);
            if (ch == 0x07) {
                let len = idx + 1;
                let bytes = S_prefix((ready)(len));
                io_Buf_Reader_drop(input, len);
                return_some(dansi_Seq_from(kind, bytes));
            }
            if (ch == 0x1B && idx + 1 < ready.len && *S_at((ready)[idx + 1]) == '\\') {
                let len = idx + 2;
                let bytes = S_prefix((ready)(len));
                io_Buf_Reader_drop(input, len);
                return_some(dansi_Seq_from(kind, bytes));
            }
            idx += 1;
        }
        return_none();
    }
    default: {
        let bytes = S_prefix((ready)(2));
        io_Buf_Reader_drop(input, 2);
        return_some(dansi_Seq_esc(bytes));
    }
    }
} $unscoped(fn);

$static fn_((daterm_ANSI__pollSeq(daterm_ANSI* self))(O$dansi_Seq) $scope) {
    let os_ready = daterm_ANSI__inputReady(self);
    if (os_ready) {
        catch_((io_Buf_Reader_fill(&self->input_buf.reader))($ignore, return_none()));
    }
    return daterm_ANSI_pollBufferedSeq(
        &self->input_buf.reader,
        self->clock,
        &self->input_buf.esc_started_at,
        self->input_buf.esc_timeout
    );
} $unscoped(fn);

fn_((daterm_ANSI__poll(P$raw ctx))(O$daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    let seq = orelse_((daterm_ANSI__pollSeq(self))(return_none()));
    let parsed = dansi_Event_tryParse(seq);
    if_some((parsed)(event)) {
        return daterm_ANSI__asTermEvent(event);
    }
    let mouse = catch_((dansi_mouse_parseSGR(seq))($ignore, return_none()));
    return_some(daterm_ANSI__asMouseEvent(mouse));
} $unscoped(fn);

fn_((daterm_ANSI__wait(P$raw ctx))(Sched_Cancelable$daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    while (true) {
        if_some((daterm_ANSI__poll(self))(event)) { return_ok(event); }
        try_(time_Clock_sleep(self->clock, time_Dur_fromMillis(1)));
    }
} $unscoped(fn);

fn_((daterm_ANSI__waitTimed(P$raw ctx, time_Dur timeout))(daterm_Term_WaitE$daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    let instant = time_Clock_now(self->clock);
    while (true) {
        if_some((daterm_ANSI__poll(ctx))(event)) { return_ok(event); }
        let elapsed = time_Clock_Inst_elapsed(instant, self->clock);
        if (time_Dur_gt(elapsed, timeout)) return_err(E_cause$Sched_Timeout());
        try_(time_Clock_sleep(self->clock, time_Dur_sub(timeout, elapsed)));
    }
} $unscoped(fn);

fn_((daterm_ANSI__waitProtn(P$raw ctx))(daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    while (true) {
        if_some((daterm_ANSI__poll(self))(event)) { return event; }
        catch_((time_Awake_sleep(clock, time_Dur_fromMillis(1)))($ignore, $do_nothing));
    }
} $unscoped(fn);

fn_((daterm_ANSI__reader(P$raw ctx))(io_Reader)) {
    return daterm_ANSI__readerOf(ptrAlignCast$((daterm_ANSI*)(ctx)));
};

fn_((daterm_ANSI__writer(P$raw ctx))(io_Writer)) {
    return daterm_ANSI__writerOf(ptrAlignCast$((daterm_ANSI*)(ctx)));
};

fn_((daterm_ANSI__queryScreenSize(P$raw ctx))(E$daterm_Size) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
#if plat_is_windows
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) $undefined;
    if (!GetConsoleScreenBufferInfo(self->output_file.handle, &csbi)) { return_err(E_cause$Unexpected()); }
    return_ok({
        .cols = as$(u16)(csbi.srWindow.Right - csbi.srWindow.Left + 1),
        .rows = as$(u16)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1),
    });
#elif plat_is_posix
    var_(ws, struct winsize) $undefined;
    if (ioctl(as$(int)(self->output_file.handle), TIOCGWINSZ, &ws) < 0) { return_err(E_cause$Unexpected()); }
    return_ok({ .cols = as$(u16)(ws.ws_col), .rows = as$(u16)(ws.ws_row) });
#else
    return_err(E_cause$daterm_ANSI_Unsupported());
#endif
} $unscoped(fn);

fn_((daterm_ANSI__queryCursorPos(P$raw ctx))(E$daterm_Pos) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
#if plat_is_windows
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) $undefined;
    if (!GetConsoleScreenBufferInfo(self->output_file.handle, &csbi)) { return_err(E_cause$Unexpected()); }
    return_ok({
        .col = as$(u16)(csbi.dwCursorPosition.X),
        .row = as$(u16)(csbi.dwCursorPosition.Y),
    });
#elif plat_is_posix
    let pos = try_(dansi_cursor_fetchPos(
        daterm_ANSI__writerOf(self),
        daterm_ANSI__readerOf(self),
        self->report_buf.mem
    ));
    return_ok({
        .col = pos.col == 0 ? 0 : as$(u16)(pos.col - 1),
        .row = pos.row == 0 ? 0 : as$(u16)(pos.row - 1),
    });
#else
    return_err(E_cause$daterm_ANSI_Unsupported());
#endif
} $unscoped(fn);

#if plat_is_windows

fn_((daterm_ANSI__windows_ctrlHandler(DWORD type))(BOOL)) {
    switch (type) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
        return TRUE;
    default:
        return FALSE;
    }
};

fn_((daterm_ANSI__windows_enableRawMode(HANDLE input))(E$DWORD) $scope) {
    var_(old_mode, DWORD) = 0;
    if (!GetConsoleMode(input, &old_mode)) { return_err(E_cause$Unexpected()); }
    let new_mode = as$(DWORD)(ENABLE_VIRTUAL_TERMINAL_INPUT
                          | ENABLE_WINDOW_INPUT
                          | ENABLE_MOUSE_INPUT);
    if (!SetConsoleMode(input, new_mode)) { return_err(E_cause$Unexpected()); }
    FlushConsoleInputBuffer(input);
    if (!SetConsoleCtrlHandler(daterm_ANSI__windows_ctrlHandler, TRUE)) {
        catch_((daterm_ANSI__windows_disableRawMode(input, old_mode))($ignore, $do_nothing));
        return_err(E_cause$Unexpected());
    }
    return_ok(old_mode);
} $unscoped(fn);

fn_((daterm_ANSI__windows_disableRawMode(HANDLE input, DWORD old_in))(E$void) $scope) {
    SetConsoleCtrlHandler(daterm_ANSI__windows_ctrlHandler, FALSE);
    if (!SetConsoleMode(input, old_in)) { return_err(E_cause$Unexpected()); }
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI__windows_enableVTerm(HANDLE output))(E$DWORD) $scope) {
    var_(old_mode, DWORD) = 0;
    if (!GetConsoleMode(output, &old_mode)) { return_err(E_cause$Unexpected()); }
    let new_mode = old_mode | as$(DWORD)(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!SetConsoleMode(output, new_mode)) { return_err(E_cause$Unexpected()); }
    return_ok(old_mode);
} $unscoped(fn);

fn_((daterm_ANSI__windows_disableVTerm(HANDLE output, DWORD old_out))(E$void) $scope) {
    if (!SetConsoleMode(output, old_out)) { return_err(E_cause$Unexpected()); }
    return_ok({});
} $unscoped(fn);

#endif /* plat_is_windows */

#if plat_is_posix

fn_((daterm_ANSI__posix_enableRawMode(fs_File_Handle handle, daterm_ANSI_OutputMode output_mode))(E$posix_termios) $scope) {
    let fd = as$(int)(handle);
    var_(old, posix_termios) $undefined;
    if (tcgetattr(fd, &old) < 0) { return_err(E_cause$Unexpected()); }
    var new = old;
    new.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    if (output_mode == daterm_ANSI_OutputMode_raw) {
        new.c_oflag &= ~(OPOST);
    }
    new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    new.c_cflag &= ~(CSIZE | PARENB);
    new.c_cflag |= CS8;
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSAFLUSH, &new) < 0) { return_err(E_cause$Unexpected()); }
    return_ok(old);
} $unscoped(fn);

fn_((daterm_ANSI__posix_disableRawMode(fs_File_Handle handle, posix_termios old))(E$void) $scope) {
    let fd = as$(int)(handle);
    if (tcsetattr(fd, TCSAFLUSH, &old) < 0) { return_err(E_cause$Unexpected()); }
    return_ok({});
} $unscoped(fn);

#endif /* plat_is_posix */
