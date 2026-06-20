#include "daterm-context/ANSI.h"
#include "daterm-context/ANSI/private.h"
#include <dansi-core.h>
#include <dansi-dec.h>
#include <dansi-xterm.h>
#include <dh/fs/File.h>
#include <dh/mem/common.h>
#include <dh/time/Dur.h>
#include <dh/time/self/Awake.h>
#include <dh/utf16.h>
#include <dh/utf8.h>

/*========== Internal Declarations ==========================================*/

$static fn_((daterm_ANSI__readerOf(daterm_ANSI* self))(io_Reader));
$static fn_((daterm_ANSI__writerOf(const daterm_ANSI* self))(io_Writer));

$static fn_((daterm_ANSI__pollNativeEvent(daterm_ANSI* self))(O$daterm_Event));
$static fn_((daterm_ANSI__poll(P$raw ctx))(O$daterm_Event));
$static fn_((daterm_ANSI__wait(P$raw ctx))(Sched_Cancelable$daterm_Event));
$static fn_((daterm_ANSI__waitTimed(P$raw ctx, time_Dur timeout))(daterm_Term_WaitE$daterm_Event));
$static fn_((daterm_ANSI__waitProtn(P$raw ctx))(daterm_Event));
$static fn_((daterm_ANSI__reader(P$raw ctx))(io_Reader));
$static fn_((daterm_ANSI__writer(P$raw ctx))(io_Writer));
$static fn_((daterm_ANSI__flush(P$raw ctx))(E$void));
$static fn_((daterm_ANSI__caps(P$raw ctx))(daterm_TermCaps));
$static fn_((daterm_ANSI__queryLocal(
    P$raw ctx, daterm_LocalQuery query
))(E$daterm_LocalQueryResult));
$static fn_((daterm_ANSI__runTxn(P$raw ctx, daterm_Txn txn))(daterm_Txn_E$Void));
$static fn_((daterm_ANSI__queryNativeScreenCells(P$raw ctx))(E$daterm_Size));
$static fn_((daterm_ANSI__queryNativeCursorPos(P$raw ctx))(E$daterm_Pos));
$static fn_((daterm_ANSI__pendingPush(daterm_ANSI* self, daterm_Event event))(bool));
$static fn_((daterm_ANSI__pendingPop(daterm_ANSI* self))(O$daterm_Event));
$static fn_((daterm_ANSI__xtermKeyMods(dansi_xterm_key_Mods mods))(daterm_input_Mods));
$static fn_((daterm_ANSI__xtermMouseMods(dansi_xterm_mouse_Mods mods))(daterm_input_Mods));
$static fn_((daterm_ANSI__xtermKeyCode(dansi_xterm_key_Named named))(O$daterm_key_Code));
$static fn_((daterm_ANSI__decKeyCode(dansi_dec_key_Named named))(O$daterm_key_Code));
$static fn_((daterm_ANSI__decKeypadCode(dansi_dec_key_Keypad keypad))(O$daterm_key_Code));
$static fn_((daterm_ANSI__mouseBtn(dansi_xterm_mouse_Btn btn))(O$daterm_mouse_Btn));
$static fn_((daterm_ANSI__mouseWheel(dansi_xterm_mouse_Wheel wheel))(O$daterm_mouse_Wheel));
$static fn_((daterm_ANSI__mousePos(
    dansi_xterm_mouse_Pos pos, daterm_mouse_PosKind kind
))(daterm_mouse_Pos));
$static fn_((daterm_ANSI__fromXtermMouse(
    dansi_xterm_mouse_Event event, daterm_mouse_PosKind pos_kind
))(O$daterm_Event));
$static fn_((daterm_ANSI__fromDecKey(dansi_dec_key_Event event))(O$daterm_Event));
$static fn_((daterm_ANSI__fromXtermKey(dansi_xterm_key_Event event))(O$daterm_Event));
$static fn_((daterm_ANSI__parseXtermEnhancedKey(dansi_Seq seq))(O$daterm_Event));
$static fn_((daterm_ANSI__parseSeqEvent(
    dansi_Seq seq, daterm_mouse_PosKind mouse_pos_kind
))(O$daterm_Event));
$static fn_((daterm_ANSI__fromCodepoint(u32 codepoint, daterm_input_Mods mods))(daterm_Event));
$static fn_((daterm_ANSI__takeSeq(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    dansi_Seq_Kind kind, usize len
))(O$dansi_Seq));
$static fn_((daterm_ANSI__pollCSI(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    S_const$u8 ready, usize prefix_len
))(O$dansi_Seq));
$static fn_((daterm_ANSI__pollCtrlString(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    S_const$u8 ready, dansi_Seq_Kind kind, usize prefix_len
))(O$dansi_Seq));

#if plat_is_windows
#include <dh/sys/api/windows/console.h>
#include <dh/sys/api/windows/sync.h>
T_use_E$(DWORD);
$static fn_((daterm_ANSI__windows_ctrlHandler(DWORD type))(BOOL));
$static fn_((daterm_ANSI__windows_keyMods(DWORD control_key_state))(daterm_input_Mods));
$static fn_((daterm_ANSI__windows_keyCode(
    WORD virtual_key, WORD virtual_scan, DWORD control_key_state
))(O$daterm_key_Code));
$static fn_((daterm_ANSI__windows_mouseBtn(DWORD button_mask))(O$daterm_mouse_Btn));
$static fn_((daterm_ANSI__windows_mouseEvent(daterm_ANSI* self, MOUSE_EVENT_RECORD record))(O$daterm_Event));
$static fn_((daterm_ANSI__windows_pollNativeEvent(daterm_ANSI* self))(O$daterm_Event));
$static fn_((daterm_ANSI__windows_inputReady(daterm_ANSI* self))(bool));
$static fn_((daterm_ANSI__windows_enableRawMode(
    HANDLE input, daterm_ANSI_InputMode input_mode
))(E$DWORD));
$static fn_((daterm_ANSI__windows_disableRawMode(HANDLE input, DWORD old_in))(E$void));
$static fn_((daterm_ANSI__windows_enableVTerm(HANDLE output))(E$DWORD));
$static fn_((daterm_ANSI__windows_disableVTerm(HANDLE output, DWORD old_out))(E$void));
#endif /* plat_is_windows */
#if plat_is_posix
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
T_alias$((posix_termios)(struct termios));
T_alias$((posix_sigaction)(struct sigaction));
T_use_E$(posix_termios);
T_use_E$(posix_sigaction);
$static fn_((daterm_ANSI__posix_enableRawMode(fs_File_Handle handle, daterm_ANSI_OutputMode output_mode))(E$posix_termios));
$static fn_((daterm_ANSI__posix_disableRawMode(fs_File_Handle handle, posix_termios old))(E$void));
$static fn_((daterm_ANSI__posix_enableResizeEvents(void))(E$posix_sigaction));
$static fn_((daterm_ANSI__posix_disableResizeEvents(posix_sigaction old))(E$void));
$static fn_((daterm_ANSI__posix_onResize(int sig))(void));
$static var volatile sig_atomic_t daterm_ANSI__posix_resize_pending = 0;
#endif /* plat_is_posix */

/*========== External Definitions ===========================================*/

fn_((daterm_ANSI_init(daterm_ANSI_Cfg cfg))(mem_E$daterm_ANSI) $guard) {
    let should_alloc_input_buf = matches(cfg.input_buf, daterm_ANSI_Cfg_input_buf_owned);
    let input_mem = expr_(S$u8 $scope)(if (should_alloc_input_buf) {
        let cap = local_({
            let cfg_input_buf = union_to((cfg.input_buf)(daterm_ANSI_Cfg_input_buf_owned));
            cfg_input_buf.cap != 0
                ? local_return_(cfg_input_buf.cap)
                : local_return_(daterm_ANSI_input_buf_cap_default);
        });
        $break_(try_(mem_Alctr_allocBytes($trace unwrap_(cfg.gpa), cap)));
    } else {
        $break_(union_to((cfg.input_buf)(daterm_ANSI_Cfg_input_buf_fixed)));
    }) $unscoped(expr);
    errdefer_($ignore, if (should_alloc_input_buf) mem_Alctr_freeBytes($trace unwrap_(cfg.gpa), input_mem));

    return_ok((daterm_ANSI){
        .input_file = cfg.input_file,
        .output_file = cfg.output_file,
        .output_mode = cfg.output_mode,
        .input_mode = cfg.input_mode,
        .raw_mode_ = none(),
        .is_tracking_mouse = false,
        .mouse_report_mode_ = 0,
        .mouse_pos_kind = daterm_mouse_PosKind_cell,
        .is_tracking_focus = false,
        .is_enhanced_keyboard = false,
        .cached_screen_cells = none(),
#if plat_is_windows
        .windows_mouse_buttons = 0,
#endif /* plat_is_windows */
        .input_buf = {
            .reader = io_Buf_Reader_init(fs_File_reader(cfg.input_file), input_mem),
            .is_owned = should_alloc_input_buf,
            .esc_started_at = none(),
            .esc_timeout = cfg.esc_timeout,
        },
        .pending = { .items = {}, .head = 0, .len = 0 },
        .gpa = cfg.gpa,
        .clock = cfg.clock,
    });
} $unguarded(fn);

fn_((daterm_ANSI_fini(daterm_ANSI* self))(void)) {
    claim_assert_nonnull(self);
    let out = daterm_ANSI__writerOf(self);
    if (self->is_enhanced_keyboard) {
        catch_((dansi_xterm_key_disableEnhancedWrite(out))($ignore, $do_nothing));
    }
    if (self->is_tracking_focus) {
        catch_((dansi_xterm_focus_disableTrackingWrite(out))($ignore, $do_nothing));
    }
    if (self->is_tracking_mouse) {
        let_(encoding, dansi_xterm_mouse_Encoding) = self->mouse_pos_kind == daterm_mouse_PosKind_pixel
                                                       ? dansi_xterm_mouse_Encoding_sgr_pixels
                                                       : dansi_xterm_mouse_Encoding_sgr;
        catch_((dansi_xterm_mouse_disableEncodingWrite(encoding, out))($ignore, $do_nothing));
        catch_((dansi_xterm_mouse_disableReportModeWrite(
            as$(dansi_xterm_mouse_ReportMode)(self->mouse_report_mode_), out))($ignore, $do_nothing));
    }
    daterm_ANSI_disableRawMode(self);
    if (self->input_buf.is_owned) {
        mem_Alctr_freeBytes($trace unwrap_(self->gpa), self->input_buf.reader.buf);
    }
    asg_l((self)(cleared()));
};

fn_((daterm_ANSI_enableRawMode(daterm_ANSI* self))(E$void) $guard) {
    if (isSome(self->raw_mode_)) return_ok({});
#if plat_is_windows
    let old_in = try_(daterm_ANSI__windows_enableRawMode(
        self->input_file.handle, self->input_mode
    ));
    errdefer_($ignore, catch_((daterm_ANSI__windows_disableRawMode(self->input_file.handle, old_in))($ignore, $do_nothing)));
    let old_out = try_(daterm_ANSI__windows_enableVTerm(self->output_file.handle));
    errdefer_($ignore, catch_((daterm_ANSI__windows_disableVTerm(self->output_file.handle, old_out))($ignore, $do_nothing)));
#elif plat_is_posix
    let old_in = try_(daterm_ANSI__posix_enableRawMode(self->input_file.handle, self->output_mode));
    errdefer_($ignore, catch_((daterm_ANSI__posix_disableRawMode(self->input_file.handle, old_in))($ignore, $do_nothing)));
    let old_winch = try_(daterm_ANSI__posix_enableResizeEvents());
    errdefer_($ignore, catch_((daterm_ANSI__posix_disableResizeEvents(old_winch))($ignore, $do_nothing)));
#endif /* plat_is_posix || plat_is_windows */
#if plat_is_windows
    asg_l((&self->raw_mode_)(some({
        .old_in = old_in,
        .old_out = old_out,
    })));
#elif plat_is_posix
    asg_l((&self->raw_mode_)(some({
        .old_in = old_in,
        .old_winch = old_winch,
    })));
#endif /* plat_is_windows || plat_is_posix */
    return_ok({});
} $unguarded(fn);

fn_((daterm_ANSI_disableRawMode(daterm_ANSI* self))(void)) {
    let raw_mode = orelse_((self->raw_mode_)(return));
#if plat_is_windows
    catch_((daterm_ANSI__windows_disableRawMode(self->input_file.handle, raw_mode.old_in))($ignore, $do_nothing));
    catch_((daterm_ANSI__windows_disableVTerm(self->output_file.handle, raw_mode.old_out))($ignore, $do_nothing));
    self->windows_mouse_buttons = 0;
    self->windows_surrogate = none$((O$daterm_ANSI__WindowsSurrogate));
#elif plat_is_posix
    catch_((daterm_ANSI__posix_disableResizeEvents(raw_mode.old_winch))($ignore, $do_nothing));
    catch_((daterm_ANSI__posix_disableRawMode(self->input_file.handle, raw_mode.old_in))($ignore, $do_nothing));
#endif /* plat_is_posix || plat_is_windows */
    asg_l((&self->raw_mode_)(none()));
};

fn_((daterm_ANSI_isInRawMode(const daterm_ANSI* self))(bool)) {
    return isSome(self->raw_mode_);
};

fn_((daterm_ANSI_term(daterm_ANSI* self))(daterm_Term)) {
    $static let_(vtbl, daterm_Term_VTbl) = {
        .pollFn = daterm_ANSI__poll,
        .waitFn = daterm_ANSI__wait,
        .waitTimedFn = daterm_ANSI__waitTimed,
        .waitProtnFn = daterm_ANSI__waitProtn,
        .readerFn = daterm_ANSI__reader,
        .writerFn = daterm_ANSI__writer,
        .flushFn = daterm_ANSI__flush,
        .capsFn = daterm_ANSI__caps,
        .queryLocalFn = daterm_ANSI__queryLocal,
        .runTxnFn = daterm_ANSI__runTxn,
    };
    return daterm_Term_ensureValid((daterm_Term){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

fn_((daterm_ANSI__readerOf(daterm_ANSI* self))(io_Reader)) {
    return io_Buf_reader(&self->input_buf.reader);
};

fn_((daterm_ANSI__writerOf(const daterm_ANSI* self))(io_Writer)) {
    return fs_File_writer(self->output_file);
};

$static fn_((daterm_ANSI__xtermKeyMods(dansi_xterm_key_Mods mods))(daterm_input_Mods)) {
    return (daterm_input_Mods){
        .shift = mods.shift,
        .alt = mods.alt,
        .ctrl = mods.ctrl,
        .meta = mods.meta,
    };
};

$static fn_((daterm_ANSI__xtermMouseMods(dansi_xterm_mouse_Mods mods))(daterm_input_Mods)) {
    return (daterm_input_Mods){
        .shift = mods.shift,
        .alt = mods.alt,
        .ctrl = mods.ctrl,
        .meta = mods.meta,
    };
};

$static fn_((daterm_ANSI__xtermKeyCode(dansi_xterm_key_Named named))(O$daterm_key_Code)) {
    switch (named) {
    case_((dansi_xterm_key_Named_up)) return some$((O$daterm_key_Code)(daterm_key_Code_up)) $end(case);
    case_((dansi_xterm_key_Named_down)) return some$((O$daterm_key_Code)(daterm_key_Code_down)) $end(case);
    case_((dansi_xterm_key_Named_right)) return some$((O$daterm_key_Code)(daterm_key_Code_right)) $end(case);
    case_((dansi_xterm_key_Named_left)) return some$((O$daterm_key_Code)(daterm_key_Code_left)) $end(case);
    case_((dansi_xterm_key_Named_home)) return some$((O$daterm_key_Code)(daterm_key_Code_home)) $end(case);
    case_((dansi_xterm_key_Named_end)) return some$((O$daterm_key_Code)(daterm_key_Code_end)) $end(case);
    case_((dansi_xterm_key_Named_insert)) return some$((O$daterm_key_Code)(daterm_key_Code_insert)) $end(case);
    case_((dansi_xterm_key_Named_delete)) return some$((O$daterm_key_Code)(daterm_key_Code_delete)) $end(case);
    case_((dansi_xterm_key_Named_page_up)) return some$((O$daterm_key_Code)(daterm_key_Code_page_up)) $end(case);
    case_((dansi_xterm_key_Named_page_down)) return some$((O$daterm_key_Code)(daterm_key_Code_page_down)) $end(case);
    case_((dansi_xterm_key_Named_begin)) return some$((O$daterm_key_Code)(daterm_key_Code_begin)) $end(case);
    case_((dansi_xterm_key_Named_tab)) return some$((O$daterm_key_Code)(daterm_key_Code_tab)) $end(case);
    case_((dansi_xterm_key_Named_back_tab)) return some$((O$daterm_key_Code)(daterm_key_Code_back_tab)) $end(case);
    case_((dansi_xterm_key_Named_enter)) return some$((O$daterm_key_Code)(daterm_key_Code_enter)) $end(case);
    case_((dansi_xterm_key_Named_escape)) return some$((O$daterm_key_Code)(daterm_key_Code_escape)) $end(case);
    case_((dansi_xterm_key_Named_backspace)) return some$((O$daterm_key_Code)(daterm_key_Code_backspace)) $end(case);
    case_((dansi_xterm_key_Named_f1)) return some$((O$daterm_key_Code)(daterm_key_Code_f1)) $end(case);
    case_((dansi_xterm_key_Named_f2)) return some$((O$daterm_key_Code)(daterm_key_Code_f2)) $end(case);
    case_((dansi_xterm_key_Named_f3)) return some$((O$daterm_key_Code)(daterm_key_Code_f3)) $end(case);
    case_((dansi_xterm_key_Named_f4)) return some$((O$daterm_key_Code)(daterm_key_Code_f4)) $end(case);
    case_((dansi_xterm_key_Named_f5)) return some$((O$daterm_key_Code)(daterm_key_Code_f5)) $end(case);
    case_((dansi_xterm_key_Named_f6)) return some$((O$daterm_key_Code)(daterm_key_Code_f6)) $end(case);
    case_((dansi_xterm_key_Named_f7)) return some$((O$daterm_key_Code)(daterm_key_Code_f7)) $end(case);
    case_((dansi_xterm_key_Named_f8)) return some$((O$daterm_key_Code)(daterm_key_Code_f8)) $end(case);
    case_((dansi_xterm_key_Named_f9)) return some$((O$daterm_key_Code)(daterm_key_Code_f9)) $end(case);
    case_((dansi_xterm_key_Named_f10)) return some$((O$daterm_key_Code)(daterm_key_Code_f10)) $end(case);
    case_((dansi_xterm_key_Named_f11)) return some$((O$daterm_key_Code)(daterm_key_Code_f11)) $end(case);
    case_((dansi_xterm_key_Named_f12)) return some$((O$daterm_key_Code)(daterm_key_Code_f12)) $end(case);
    case_((dansi_xterm_key_Named_f13)) return some$((O$daterm_key_Code)(daterm_key_Code_f13)) $end(case);
    case_((dansi_xterm_key_Named_f14)) return some$((O$daterm_key_Code)(daterm_key_Code_f14)) $end(case);
    case_((dansi_xterm_key_Named_f15)) return some$((O$daterm_key_Code)(daterm_key_Code_f15)) $end(case);
    case_((dansi_xterm_key_Named_f16)) return some$((O$daterm_key_Code)(daterm_key_Code_f16)) $end(case);
    case_((dansi_xterm_key_Named_f17)) return some$((O$daterm_key_Code)(daterm_key_Code_f17)) $end(case);
    case_((dansi_xterm_key_Named_f18)) return some$((O$daterm_key_Code)(daterm_key_Code_f18)) $end(case);
    case_((dansi_xterm_key_Named_f19)) return some$((O$daterm_key_Code)(daterm_key_Code_f19)) $end(case);
    case_((dansi_xterm_key_Named_f20)) return some$((O$daterm_key_Code)(daterm_key_Code_f20)) $end(case);
    case_((dansi_xterm_key_Named_f21)) return some$((O$daterm_key_Code)(daterm_key_Code_f21)) $end(case);
    case_((dansi_xterm_key_Named_f22)) return some$((O$daterm_key_Code)(daterm_key_Code_f22)) $end(case);
    case_((dansi_xterm_key_Named_f23)) return some$((O$daterm_key_Code)(daterm_key_Code_f23)) $end(case);
    case_((dansi_xterm_key_Named_f24)) return some$((O$daterm_key_Code)(daterm_key_Code_f24)) $end(case);
    }
    return none$((O$daterm_key_Code));
};

$static fn_((daterm_ANSI__decKeyCode(dansi_dec_key_Named named))(O$daterm_key_Code)) {
    switch (named) {
    case_((dansi_dec_key_Named_up)) return some$((O$daterm_key_Code)(daterm_key_Code_up)) $end(case);
    case_((dansi_dec_key_Named_down)) return some$((O$daterm_key_Code)(daterm_key_Code_down)) $end(case);
    case_((dansi_dec_key_Named_right)) return some$((O$daterm_key_Code)(daterm_key_Code_right)) $end(case);
    case_((dansi_dec_key_Named_left)) return some$((O$daterm_key_Code)(daterm_key_Code_left)) $end(case);
    case_((dansi_dec_key_Named_home)) return some$((O$daterm_key_Code)(daterm_key_Code_home)) $end(case);
    case_((dansi_dec_key_Named_end)) return some$((O$daterm_key_Code)(daterm_key_Code_end)) $end(case);
    case_((dansi_dec_key_Named_find)) return some$((O$daterm_key_Code)(daterm_key_Code_find)) $end(case);
    case_((dansi_dec_key_Named_insert)) return some$((O$daterm_key_Code)(daterm_key_Code_insert)) $end(case);
    case_((dansi_dec_key_Named_remove)) return some$((O$daterm_key_Code)(daterm_key_Code_remove)) $end(case);
    case_((dansi_dec_key_Named_select)) return some$((O$daterm_key_Code)(daterm_key_Code_select)) $end(case);
    case_((dansi_dec_key_Named_prev)) return some$((O$daterm_key_Code)(daterm_key_Code_page_up)) $end(case);
    case_((dansi_dec_key_Named_next)) return some$((O$daterm_key_Code)(daterm_key_Code_page_down)) $end(case);
    case_((dansi_dec_key_Named_pf1)) return some$((O$daterm_key_Code)(daterm_key_Code_pf1)) $end(case);
    case_((dansi_dec_key_Named_pf2)) return some$((O$daterm_key_Code)(daterm_key_Code_pf2)) $end(case);
    case_((dansi_dec_key_Named_pf3)) return some$((O$daterm_key_Code)(daterm_key_Code_pf3)) $end(case);
    case_((dansi_dec_key_Named_pf4)) return some$((O$daterm_key_Code)(daterm_key_Code_pf4)) $end(case);
    case_((dansi_dec_key_Named_f6)) return some$((O$daterm_key_Code)(daterm_key_Code_f6)) $end(case);
    case_((dansi_dec_key_Named_f7)) return some$((O$daterm_key_Code)(daterm_key_Code_f7)) $end(case);
    case_((dansi_dec_key_Named_f8)) return some$((O$daterm_key_Code)(daterm_key_Code_f8)) $end(case);
    case_((dansi_dec_key_Named_f9)) return some$((O$daterm_key_Code)(daterm_key_Code_f9)) $end(case);
    case_((dansi_dec_key_Named_f10)) return some$((O$daterm_key_Code)(daterm_key_Code_f10)) $end(case);
    case_((dansi_dec_key_Named_f11)) return some$((O$daterm_key_Code)(daterm_key_Code_f11)) $end(case);
    case_((dansi_dec_key_Named_f12)) return some$((O$daterm_key_Code)(daterm_key_Code_f12)) $end(case);
    case_((dansi_dec_key_Named_f13)) return some$((O$daterm_key_Code)(daterm_key_Code_f13)) $end(case);
    case_((dansi_dec_key_Named_f14)) return some$((O$daterm_key_Code)(daterm_key_Code_f14)) $end(case);
    case_((dansi_dec_key_Named_f15)) return some$((O$daterm_key_Code)(daterm_key_Code_f15)) $end(case);
    case_((dansi_dec_key_Named_f16)) return some$((O$daterm_key_Code)(daterm_key_Code_f16)) $end(case);
    case_((dansi_dec_key_Named_f17)) return some$((O$daterm_key_Code)(daterm_key_Code_f17)) $end(case);
    case_((dansi_dec_key_Named_f18)) return some$((O$daterm_key_Code)(daterm_key_Code_f18)) $end(case);
    case_((dansi_dec_key_Named_f19)) return some$((O$daterm_key_Code)(daterm_key_Code_f19)) $end(case);
    case_((dansi_dec_key_Named_f20)) return some$((O$daterm_key_Code)(daterm_key_Code_f20)) $end(case);
    case_((dansi_dec_key_Named_f21)) return some$((O$daterm_key_Code)(daterm_key_Code_f21)) $end(case);
    case_((dansi_dec_key_Named_f22)) return some$((O$daterm_key_Code)(daterm_key_Code_f22)) $end(case);
    case_((dansi_dec_key_Named_f23)) return some$((O$daterm_key_Code)(daterm_key_Code_f23)) $end(case);
    case_((dansi_dec_key_Named_f24)) return some$((O$daterm_key_Code)(daterm_key_Code_f24)) $end(case);
    }
    return none$((O$daterm_key_Code));
};

$static fn_((daterm_ANSI__decKeypadCode(dansi_dec_key_Keypad keypad))(O$daterm_key_Code)) {
    switch (keypad) {
    case_((dansi_dec_key_Keypad_0)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_0)) $end(case);
    case_((dansi_dec_key_Keypad_1)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_1)) $end(case);
    case_((dansi_dec_key_Keypad_2)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_2)) $end(case);
    case_((dansi_dec_key_Keypad_3)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_3)) $end(case);
    case_((dansi_dec_key_Keypad_4)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_4)) $end(case);
    case_((dansi_dec_key_Keypad_5)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_5)) $end(case);
    case_((dansi_dec_key_Keypad_6)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_6)) $end(case);
    case_((dansi_dec_key_Keypad_7)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_7)) $end(case);
    case_((dansi_dec_key_Keypad_8)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_8)) $end(case);
    case_((dansi_dec_key_Keypad_9)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_9)) $end(case);
    case_((dansi_dec_key_Keypad_decimal)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_decimal)) $end(case);
    case_((dansi_dec_key_Keypad_minus)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_minus)) $end(case);
    case_((dansi_dec_key_Keypad_comma)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_comma)) $end(case);
    case_((dansi_dec_key_Keypad_enter)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_enter)) $end(case);
    }
    return none$((O$daterm_key_Code));
};

$static fn_((daterm_ANSI__mouseBtn(dansi_xterm_mouse_Btn btn))(O$daterm_mouse_Btn)) {
    switch (btn) {
    case_((dansi_xterm_mouse_Btn_left)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_left)) $end(case);
    case_((dansi_xterm_mouse_Btn_middle)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_middle)) $end(case);
    case_((dansi_xterm_mouse_Btn_right)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_right)) $end(case);
    case_((dansi_xterm_mouse_Btn_backward)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_backward)) $end(case);
    case_((dansi_xterm_mouse_Btn_forward)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_forward)) $end(case);
    case_((dansi_xterm_mouse_Btn_aux1)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_aux1)) $end(case);
    case_((dansi_xterm_mouse_Btn_aux2)) return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_aux2)) $end(case);
    }
    return none$((O$daterm_mouse_Btn));
};

$static fn_((daterm_ANSI__mouseWheel(dansi_xterm_mouse_Wheel wheel))(O$daterm_mouse_Wheel)) {
    switch (wheel) {
    case_((dansi_xterm_mouse_Wheel_up)) return some$((O$daterm_mouse_Wheel)(daterm_mouse_Wheel_up)) $end(case);
    case_((dansi_xterm_mouse_Wheel_down)) return some$((O$daterm_mouse_Wheel)(daterm_mouse_Wheel_down)) $end(case);
    case_((dansi_xterm_mouse_Wheel_left)) return some$((O$daterm_mouse_Wheel)(daterm_mouse_Wheel_left)) $end(case);
    case_((dansi_xterm_mouse_Wheel_right)) return some$((O$daterm_mouse_Wheel)(daterm_mouse_Wheel_right)) $end(case);
    }
    return none$((O$daterm_mouse_Wheel));
};

$static fn_((daterm_ANSI__mousePos(
    dansi_xterm_mouse_Pos pos, daterm_mouse_PosKind kind
))(daterm_mouse_Pos)) {
    return (daterm_mouse_Pos){
        .x = pos.x == 0 ? 0 : as$(u16)(pos.x - 1),
        .y = pos.y == 0 ? 0 : as$(u16)(pos.y - 1),
        .kind = kind,
    };
};

$static fn_((daterm_ANSI__fromXtermMouse(
    dansi_xterm_mouse_Event event, daterm_mouse_PosKind pos_kind
))(O$daterm_Event) $scope) {
    match_(event) {
    pattern_((dansi_xterm_mouse_Event_press)(press)) {
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_press){
            .btn = orelse_((daterm_ANSI__mouseBtn(press.btn))(return_none())),
            .pos = daterm_ANSI__mousePos(press.pos, pos_kind),
            .mods = daterm_ANSI__xtermMouseMods(press.mods),
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    } $end(pattern);
    pattern_((dansi_xterm_mouse_Event_release)(release)) {
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_release){
            .btn = none$((O$daterm_mouse_Btn)),
            .pos = daterm_ANSI__mousePos(release.pos, pos_kind),
            .mods = daterm_ANSI__xtermMouseMods(release.mods),
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    } $end(pattern);
    pattern_((dansi_xterm_mouse_Event_motion)(motion)) {
        var_(btn, O$daterm_mouse_Btn) = none();
        if_some((motion.btn)(protocol_btn)) {
            btn = some$((O$daterm_mouse_Btn)(orelse_((daterm_ANSI__mouseBtn(protocol_btn))(return_none()))));
        }
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_motion){
            .btn = btn,
            .pos = daterm_ANSI__mousePos(motion.pos, pos_kind),
            .mods = daterm_ANSI__xtermMouseMods(motion.mods),
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    } $end(pattern);
    pattern_((dansi_xterm_mouse_Event_wheel)(wheel)) {
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_wheel){
            .wheel = orelse_((daterm_ANSI__mouseWheel(wheel.wheel))(return_none())),
            .pos = daterm_ANSI__mousePos(wheel.pos, pos_kind),
            .mods = daterm_ANSI__xtermMouseMods(wheel.mods),
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    } $end(pattern);
    } $end(match);
    return_none();
} $unscoped(fn);

$static fn_((daterm_ANSI__fromDecKey(dansi_dec_key_Event event))(O$daterm_Event) $scope) {
    match_(event) {
    pattern_((dansi_dec_key_Event_named)(named)) {
        return_some(union_of((daterm_Event_key){
            .code = orelse_((daterm_ANSI__decKeyCode(named))(return_none())),
            .mods = daterm_input_modsNone(),
            .action = none(),
        }));
    } $end(pattern);
    pattern_((dansi_dec_key_Event_keypad)(keypad)) {
        return_some(union_of((daterm_Event_key){
            .code = orelse_((daterm_ANSI__decKeypadCode(keypad))(return_none())),
            .mods = daterm_input_modsNone(),
            .action = none(),
        }));
    } $end(pattern);
    } $end(match);
    return_none();
} $unscoped(fn);

$static fn_((daterm_ANSI__fromXtermKey(dansi_xterm_key_Event event))(O$daterm_Event) $scope) {
    match_(event) {
    pattern_((dansi_xterm_key_Event_special)(special)) {
        return_some(union_of((daterm_Event_key){
            .code = orelse_((daterm_ANSI__xtermKeyCode(special.key))(return_none())),
            .mods = daterm_ANSI__xtermKeyMods(special.mods),
            .action = none(),
        }));
    } $end(pattern);
    pattern_((dansi_xterm_key_Event_text)(text)) {
        return_some(daterm_ANSI__fromCodepoint(
            text.codepoint, daterm_ANSI__xtermKeyMods(text.mods)
        ));
    } $end(pattern);
    pattern_((dansi_xterm_key_Event_modify_other)(other)) {
        return_some(daterm_ANSI__fromCodepoint(
            other.codepoint, daterm_ANSI__xtermKeyMods(other.mods)
        ));
    } $end(pattern);
    } $end(match);
    return_none();
} $unscoped(fn);

$static fn_((daterm_ANSI__parseXtermEnhancedKey(dansi_Seq seq))(O$daterm_Event) $scope) {
    let report = orelse_((dansi_xterm_key_parseReport(seq))(return_none()));
    match_(report) {
    pattern_((dansi_xterm_key_Report_modified_csi)($ignore)) {
        let event = catch_((dansi_xterm_key_interpretReport(report))($ignore, return_none()));
        return daterm_ANSI__fromXtermKey(event);
    } $end(pattern);
    pattern_((dansi_xterm_key_Report_modify_other)($ignore)) {
        let event = catch_((dansi_xterm_key_interpretReport(report))($ignore, return_none()));
        return daterm_ANSI__fromXtermKey(event);
    } $end(pattern);
    pattern_((dansi_xterm_key_Report_csi_u)($ignore)) {
        let event = catch_((dansi_xterm_key_interpretReport(report))($ignore, return_none()));
        return daterm_ANSI__fromXtermKey(event);
    } $end(pattern);
    pattern_((dansi_xterm_key_Report_legacy_esc)($ignore)) return_none() $end(pattern);
    pattern_((dansi_xterm_key_Report_legacy_ss3)($ignore)) return_none() $end(pattern);
    pattern_((dansi_xterm_key_Report_legacy_csi)($ignore)) return_none() $end(pattern);
    } $end(match);
    return_none();
} $unscoped(fn);

$static fn_((daterm_ANSI__fromCodepoint(u32 codepoint, daterm_input_Mods mods))(daterm_Event)) {
    switch (codepoint) {
    case dansi_c0_Code_bs: $fallthrough;
    case_((dansi_ctrl_Code_del)) {
        return (daterm_Event)union_of((daterm_Event_key){
            .code = daterm_key_Code_backspace,
            .mods = mods,
            .action = none(),
        });
    } $end(case);
    case_((dansi_c0_Code_ht)) {
        return (daterm_Event)union_of((daterm_Event_key){
            .code = daterm_key_Code_tab,
            .mods = mods,
            .action = none(),
        });
    } $end(case);
    case dansi_c0_Code_lf: $fallthrough;
    case_((dansi_c0_Code_cr)) {
        return (daterm_Event)union_of((daterm_Event_key){
            .code = daterm_key_Code_enter,
            .mods = mods,
            .action = none(),
        });
    } $end(case);
    case_((dansi_c0_Code_esc)) {
        return (daterm_Event)union_of((daterm_Event_key){
            .code = daterm_key_Code_escape,
            .mods = mods,
            .action = none(),
        });
    } $end(case);
    default_() $do_nothing $end(default);
    }
    return (daterm_Event)union_of((daterm_Event_text){
        .codepoint = codepoint,
        .mods = mods,
        .action = none(),
    });
};

$static fn_((daterm_ANSI__parseSeqEvent(
    dansi_Seq seq, daterm_mouse_PosKind mouse_pos_kind
))(O$daterm_Event) $scope) {
    if_some((dansi_xterm_focus_parseReport(seq.bytes))(focus)) {
        return_some(union_of((daterm_Event_focus)(focus == dansi_xterm_focus_Event_in
                                                      ? daterm_focus_Event_in
                                                      : daterm_focus_Event_out)));
    }
    if_some((dansi_xterm_mouse_parseSGR(seq.bytes))(mouse)) {
        return daterm_ANSI__fromXtermMouse(mouse, mouse_pos_kind);
    }
    if_some((daterm_ANSI__parseXtermEnhancedKey(seq))(key)) return_some(key);
    if_some((dansi_dec_key_parseEvent(seq))(key)) {
        return daterm_ANSI__fromDecKey(key);
    }
    if_some((dansi_xterm_key_parseEvent(seq))(key)) {
        return daterm_ANSI__fromXtermKey(key);
    }

    if (seq.kind == dansi_Seq_Kind_esc) {
        if (seq.bytes.len == 1) {
            return_some(daterm_ANSI__fromCodepoint(
                dansi_c0_Code_esc, daterm_input_modsNone()
            ));
        }
        if (seq.bytes.len == 2) {
            let codepoint = catch_((utf8_decode(S_suffix((seq.bytes)(1))))($ignore, return_none()));
            var mods = daterm_input_modsNone();
            mods.alt = true;
            return_some(daterm_ANSI__fromCodepoint(codepoint, mods));
        }
        return_none();
    }

    if (seq.kind != dansi_Seq_Kind_text && seq.kind != dansi_Seq_Kind_c0) {
        return_none();
    }
    let codepoint = catch_((utf8_decode(seq.bytes))($ignore, return_none()));
    if (seq.kind == dansi_Seq_Kind_c0
        && ((dansi_c0_Code_soh <= codepoint && codepoint <= dansi_c0_Code_ff)
            || (dansi_c0_Code_so <= codepoint && codepoint <= dansi_c0_Code_sub))) {
        var mods = daterm_input_modsNone();
        mods.ctrl = true;
        return_some(daterm_ANSI__fromCodepoint(
            codepoint - dansi_c0_Code_soh + u8_c('a'), mods
        ));
    }
    return_some(daterm_ANSI__fromCodepoint(codepoint, daterm_input_modsNone()));
} $unscoped(fn);

fn_((daterm_ANSI_parseSeqEvent(dansi_Seq seq))(O$daterm_Event)) {
    return daterm_ANSI__parseSeqEvent(seq, daterm_mouse_PosKind_cell);
};

$static fn_((daterm_ANSI__pollNativeEvent(daterm_ANSI* self))(O$daterm_Event) $scope) {
#if plat_is_windows
    return daterm_ANSI__windows_pollNativeEvent(self);
#elif plat_is_posix
    if (daterm_ANSI__posix_resize_pending == 0) return_none();
    daterm_ANSI__posix_resize_pending = 0;
    let size = catch_((daterm_ANSI__queryNativeScreenCells(self))($ignore, return_none()));
    return_some(union_of((daterm_Event_resize)(size)));
#else /* others */
    let_ignore = self;
    return_none();
#endif /* plat_is_windows || plat_is_posix || others */
} $unscoped(fn);

$static fn_((daterm_ANSI__inputReady(daterm_ANSI* self))(bool)) {
#if plat_is_windows
    return daterm_ANSI__windows_inputReady(self);
#elif plat_is_posix
    struct pollfd pfd = { .fd = as$(i32)(self->input_file.handle), .events = POLLIN, .revents = 0 };
    return poll(&pfd, 1, 0) > 0 && (pfd.revents & POLLIN) != 0;
#else
    let_ignore = self;
    return false;
#endif
};

$static fn_((daterm_ANSI__takeSeq(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    dansi_Seq_Kind kind, usize len
))(O$dansi_Seq)) {
    let bytes = S_prefix((io_Buf_Reader_ready(*input))(len));
    io_Buf_Reader_drop(input, len);
    asg_l((esc_started_at)(none()));
    return some$((O$dansi_Seq)(dansi_Seq_from(kind, bytes)));
};

$static fn_((daterm_ANSI__pollCSI(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    S_const$u8 ready, usize prefix_len
))(O$dansi_Seq) $scope) {
    var idx = prefix_len;
    while (idx < ready.len) {
        let byte = *S_at((ready)[idx]);
        if (dansi_csi_final_min_byte <= byte && byte <= dansi_csi_final_max_byte) {
            return daterm_ANSI__takeSeq(
                input, esc_started_at, dansi_Seq_Kind_csi, idx + 1
            );
        }
        idx += 1;
    }
    return_none();
} $unscoped(fn);

$static fn_((daterm_ANSI__pollCtrlString(
    io_Buf_Reader* input, O$time_Clock_Inst* esc_started_at,
    S_const$u8 ready, dansi_Seq_Kind kind, usize prefix_len
))(O$dansi_Seq) $scope) {
    let terminator = orelse_((dansi_Seq_CtrlTer_find(ready, prefix_len))(return_none()));
    let terminator_len = terminator.eos == dansi_Seq_EOS_st_7bit
                           ? u8_l(dansi_Seq_st_7bit).len
                           : usize_(1);
    return daterm_ANSI__takeSeq(
        input, esc_started_at, kind, terminator.payload_end + terminator_len
    );
} $unscoped(fn);

fn_((daterm_ANSI_pollBufferedSeq(
    io_Buf_Reader* input,
    time_Clock time,
    O$time_Clock_Inst* esc_started_at,
    time_Dur esc_timeout
))(O$dansi_Seq) $scope) {
    let ready = io_Buf_Reader_ready(*input);
    if (ready.len == 0) return_none();

    let first = *S_at((ready)[0]);
    if (first <= dansi_Seq_c0_max_byte && first != dansi_Seq_esc_byte) {
        return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_c0, 1);
    }

    if (first != dansi_Seq_esc_byte) {
        switch (first) {
        case_((dansi_c1_Code_ss2)) {
            if (ready.len < 2) return_none();
            return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_ss2, 2);
        } $end(case);
        case_((dansi_c1_Code_ss3)) {
            if (ready.len < 2) return_none();
            return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_ss3, 2);
        } $end(case);
        case_((dansi_c1_Code_dcs)) {
            return daterm_ANSI__pollCtrlString(
                input, esc_started_at, ready, dansi_Seq_Kind_dcs, 1
            );
        } $end(case);
        case_((dansi_c1_Code_sos)) {
            return daterm_ANSI__pollCtrlString(
                input, esc_started_at, ready, dansi_Seq_Kind_sos, 1
            );
        } $end(case);
        case_((dansi_c1_Code_csi)) {
            return daterm_ANSI__pollCSI(input, esc_started_at, ready, 1);
        } $end(case);
        case_((dansi_c1_Code_osc)) {
            return daterm_ANSI__pollCtrlString(
                input, esc_started_at, ready, dansi_Seq_Kind_osc, 1
            );
        } $end(case);
        case_((dansi_c1_Code_pm)) {
            return daterm_ANSI__pollCtrlString(
                input, esc_started_at, ready, dansi_Seq_Kind_pm, 1
            );
        } $end(case);
        case_((dansi_c1_Code_apc)) {
            return daterm_ANSI__pollCtrlString(
                input, esc_started_at, ready, dansi_Seq_Kind_apc, 1
            );
        } $end(case);
        default_() $do_nothing $end(default);
        }
        if (dansi_Seq_c1_min_byte <= first && first <= dansi_Seq_c1_max_byte) {
            return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_c1, 1);
        }
        let len = catch_((utf8_byteSeqLen(first))($ignore, utf8_SeqLen_1));
        if (ready.len < as$(usize)(len)) return_none();
        return daterm_ANSI__takeSeq(
            input, esc_started_at, dansi_Seq_Kind_text, as$(usize)(len)
        );
    }

    if (ready.len == 1) {
        if (time_Dur_isZero(esc_timeout)) {
            return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_esc, 1);
        }
        if_none((*esc_started_at)) {
            asg_l((esc_started_at)(some(time_Clock_now(time))));
            return_none();
        }
        let elapsed = time_Clock_Inst_elapsed(unwrap_(*esc_started_at), time);
        if (!time_Dur_gt(elapsed, esc_timeout)) return_none();
        return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_esc, 1);
    }

    let second = *S_at((ready)[1]);
    switch (second) {
    case_((dansi_Seq_ss2_7bit_intro_byte)) {
        if (ready.len < 3) return_none();
        return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_ss2, 3);
    } $end(case);
    case_((dansi_Seq_ss3_7bit_intro_byte)) {
        if (ready.len < 3) return_none();
        return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_ss3, 3);
    } $end(case);
    case_((dansi_dcs_7bit_intro_byte)) {
        return daterm_ANSI__pollCtrlString(
            input, esc_started_at, ready, dansi_Seq_Kind_dcs, 2
        );
    } $end(case);
    case_((dansi_sos_7bit_intro_byte)) {
        return daterm_ANSI__pollCtrlString(
            input, esc_started_at, ready, dansi_Seq_Kind_sos, 2
        );
    } $end(case);
    case_((dansi_csi_7bit_intro_byte)) {
        return daterm_ANSI__pollCSI(input, esc_started_at, ready, 2);
    } $end(case);
    case_((dansi_osc_7bit_intro_byte)) {
        return daterm_ANSI__pollCtrlString(
            input, esc_started_at, ready, dansi_Seq_Kind_osc, 2
        );
    } $end(case);
    case_((dansi_pm_7bit_intro_byte)) {
        return daterm_ANSI__pollCtrlString(
            input, esc_started_at, ready, dansi_Seq_Kind_pm, 2
        );
    } $end(case);
    case_((dansi_apc_7bit_intro_byte)) {
        return daterm_ANSI__pollCtrlString(
            input, esc_started_at, ready, dansi_Seq_Kind_apc, 2
        );
    } $end(case);
    default_() {
        return daterm_ANSI__takeSeq(input, esc_started_at, dansi_Seq_Kind_esc, 2);
    } $end(default);
    }
} $unscoped(fn);

$static fn_((daterm_ANSI__pollSeq(daterm_ANSI* self))(O$dansi_Seq) $scope) {
    let os_ready = daterm_ANSI__inputReady(self);
    if (os_ready) catch_((io_Buf_Reader_fill(&self->input_buf.reader))(
        $ignore, return_none()
    ));
    return daterm_ANSI_pollBufferedSeq(
        &self->input_buf.reader,
        self->clock,
        &self->input_buf.esc_started_at,
        self->input_buf.esc_timeout
    );
} $unscoped(fn);

fn_((daterm_ANSI__poll(P$raw ctx))(O$daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    if_some((daterm_ANSI__pendingPop(self))(event)) return_some(event);
    if_some((daterm_ANSI__pollNativeEvent(self))(event)) return_some(event);
    let seq = orelse_((daterm_ANSI__pollSeq(self))(return_none()));
    return daterm_ANSI__parseSeqEvent(seq, self->mouse_pos_kind);
} $unscoped(fn);

fn_((daterm_ANSI__wait(P$raw ctx))(Sched_Cancelable$daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    while (true) {
        if_some((daterm_ANSI__poll(self))(event)) return_ok(event);
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
        let remaining = time_Dur_sub(timeout, elapsed);
        let one_milli = time_Dur_fromMillis(1);
        let sleep = time_Dur_gt(remaining, one_milli) ? one_milli : remaining;
        try_(time_Clock_sleep(self->clock, sleep));
    }
} $unscoped(fn);

fn_((daterm_ANSI__waitProtn(P$raw ctx))(daterm_Event) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    while (true) {
        if_some((daterm_ANSI__poll(self))(event)) return event;
        catch_((time_Awake_sleep(clock, time_Dur_fromMillis(1)))($ignore, $do_nothing));
    }
} $unscoped(fn);

fn_((daterm_ANSI__reader(P$raw ctx))(io_Reader)) {
    return daterm_ANSI__readerOf(ptrAlignCast$((daterm_ANSI*)(ctx)));
};

fn_((daterm_ANSI__writer(P$raw ctx))(io_Writer)) {
    return daterm_ANSI__writerOf(ptrAlignCast$((daterm_ANSI*)(ctx)));
};

fn_((daterm_ANSI__flush(P$raw ctx))(E$void) $scope) {
    let_ignore = ctx;
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI__caps(P$raw ctx))(daterm_TermCaps)) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    return (daterm_TermCaps){
        .native_screen_cells = true,
#if plat_is_windows
        .native_cursor_pos = true,
        .key_action = self->input_mode == daterm_ANSI_InputMode_native,
        .modifier_key_event = self->input_mode == daterm_ANSI_InputMode_native,
        .native_mouse_event = true,
        .native_resize_event = true,
        .native_focus_event = true,
#else
        .native_cursor_pos = false,
        .key_action = false,
        .modifier_key_event = false,
        .native_mouse_event = false,
        .native_resize_event = plat_is_posix,
        .native_focus_event = false,
#endif
        .protocol_txn = self->input_mode == daterm_ANSI_InputMode_vt,
        .pending_event_queue = true,
        .flush = true,
    };
};

fn_((daterm_ANSI__pendingPush(daterm_ANSI* self, daterm_Event event))(bool)) {
    if (self->pending.len == daterm_ANSI_pending_event_cap) return false;
    let idx = (self->pending.head + self->pending.len) % daterm_ANSI_pending_event_cap;
    *A_at((self->pending.items)[idx]) = event;
    self->pending.len += 1;
    return true;
};

fn_((daterm_ANSI__pendingPop(daterm_ANSI* self))(O$daterm_Event)) {
    if (self->pending.len == 0) return none$((O$daterm_Event));
    let event = *A_at((self->pending.items)[self->pending.head]);
    self->pending.head = (self->pending.head + 1) % daterm_ANSI_pending_event_cap;
    self->pending.len -= 1;
    return some$((O$daterm_Event)(event));
};

fn_((daterm_ANSI__queryNativeScreenCells(P$raw ctx))(E$daterm_Size) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
#if plat_is_windows
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) $undefined;
    if (!GetConsoleScreenBufferInfo(self->output_file.handle, &csbi)) return_err(E_cause$Unexpected());
    let_(size, daterm_Size) = {
        .cols = as$(u16)(csbi.srWindow.Right - csbi.srWindow.Left + 1),
        .rows = as$(u16)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1),
    };
#elif plat_is_posix
    var_(ws, struct winsize) $undefined;
    if (ioctl(as$(int)(self->output_file.handle), TIOCGWINSZ, &ws) < 0) return_err(E_cause$Unexpected());
    let_(size, daterm_Size) = { .cols = as$(u16)(ws.ws_col), .rows = as$(u16)(ws.ws_row) };
#else
    return_err(E_cause$daterm_ANSI_Unsupported());
#endif
    self->cached_screen_cells = some$((O$daterm_Size)(size));
    return_ok(size);
} $unscoped(fn);

fn_((daterm_ANSI__queryNativeCursorPos(P$raw ctx))(E$daterm_Pos) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
#if plat_is_windows
    var_(csbi, CONSOLE_SCREEN_BUFFER_INFO) $undefined;
    if (!GetConsoleScreenBufferInfo(self->output_file.handle, &csbi)) return_err(E_cause$Unexpected());
    return_ok({
        .col = as$(u16)(csbi.dwCursorPosition.X),
        .row = as$(u16)(csbi.dwCursorPosition.Y),
    });
#else
    let_ignore = self;
    return_err(E_cause$daterm_ANSI_Unsupported());
#endif
} $unscoped(fn);

fn_((daterm_ANSI__queryLocal(
    P$raw ctx, daterm_LocalQuery query
))(E$daterm_LocalQueryResult) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
    switch (query.kind) {
    case_((daterm_LocalQueryKind_native_screen_cells)) {
        return_ok(union_of((daterm_LocalQueryResult_size)(try_(daterm_ANSI__queryNativeScreenCells(self)))));
    } $end(case);
    case_((daterm_LocalQueryKind_cached_screen_cells)) {
        let size = orelse_((self->cached_screen_cells)(
            try_(daterm_ANSI__queryNativeScreenCells(self))
        ));
        return_ok(union_of((daterm_LocalQueryResult_size)(size)));
    } $end(case);
    case_((daterm_LocalQueryKind_native_cursor_pos)) {
        return_ok(union_of((daterm_LocalQueryResult_pos)(try_(daterm_ANSI__queryNativeCursorPos(self)))));
    } $end(case);
    }
    return_err(E_cause$daterm_ANSI_Unsupported());
} $unscoped(fn);

fn_((daterm_ANSI__runTxn(P$raw ctx, daterm_Txn txn))(daterm_Txn_E$Void) $scope) {
    let self = ptrAlignCast$((daterm_ANSI*)(ctx));
#if plat_is_windows
    if (self->input_mode != daterm_ANSI_InputMode_vt) {
        return_err(E_cause$daterm_Txn_Unsupported());
    }
#endif
    try_(txn.requestWriteFn(txn.ctx, daterm_ANSI__writerOf(self)));
    try_(daterm_ANSI__flush(self));
    let started = time_Clock_now(self->clock);
    while (true) {
        if_some((daterm_ANSI__pollNativeEvent(self))(event)) {
            if (!daterm_ANSI__pendingPush(self, event)) {
                return_err(E_cause$daterm_Txn_PendingFull());
            }
        }
        if_some((daterm_ANSI__pollSeq(self))(seq)) {
            let matched = try_(txn.matchFn(txn.ctx, seq, txn.out));
            if (matched == daterm_TxnMatch_done) return_ok({});
            if_some((daterm_ANSI__parseSeqEvent(seq, self->mouse_pos_kind))(event)) {
                if (!daterm_ANSI__pendingPush(self, event)) {
                    return_err(E_cause$daterm_Txn_PendingFull());
                }
            }
        }
        if (time_Dur_ge(time_Clock_Inst_elapsed(started, self->clock), txn.timeout)) {
            return_err(E_cause$Sched_Timeout());
        }
        try_(time_Clock_sleep(self->clock, time_Dur_fromMillis(1)));
    }
} $unscoped(fn);

#if plat_is_windows
fn_((daterm_ANSI__windows_ctrlHandler(DWORD type))(BOOL)) {
    switch (type) {
    case CTRL_C_EVENT: $fallthrough;
    case CTRL_BREAK_EVENT: return TRUE;
    default: return FALSE;
    }
};

fn_((daterm_ANSI__windows_keyMods(DWORD control_key_state))(daterm_input_Mods)) {
    return (daterm_input_Mods){
        .shift = (control_key_state & SHIFT_PRESSED) != 0,
        .alt = (control_key_state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0,
        .ctrl = (control_key_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0,
    };
};

fn_((daterm_ANSI__windows_keyCode(
    WORD virtual_key, WORD virtual_scan, DWORD control_key_state
))(O$daterm_key_Code)) {
    let is_enhanced = (control_key_state & ENHANCED_KEY) != 0;
    var_(resolved_virtual_key, WORD) = virtual_key;
    if (virtual_key == VK_SHIFT || virtual_key == VK_CONTROL || virtual_key == VK_MENU) {
        let_(mapped, WORD) = as$(WORD)(MapVirtualKeyW(virtual_scan, MAPVK_VSC_TO_VK_EX));
        switch (virtual_key) {
        case_((VK_SHIFT)) resolved_virtual_key = mapped == VK_RSHIFT ? VK_RSHIFT : VK_LSHIFT $end(case);
        case_((VK_CONTROL)) resolved_virtual_key = mapped == VK_RCONTROL || is_enhanced
                                                   ? VK_RCONTROL
                                                   : VK_LCONTROL $end(case);
        case_((VK_MENU)) resolved_virtual_key = mapped == VK_RMENU || is_enhanced
                                                ? VK_RMENU
                                                : VK_LMENU $end(case);
        default_() $unreachable $end(default);
        }
    }
    if (VK_NUMPAD0 <= virtual_key && virtual_key <= VK_NUMPAD9) {
        return some$((O$daterm_key_Code)(as$(daterm_key_Code)(
            as$(u16)(daterm_key_Code_keypad_0) + as$(u16)(virtual_key - VK_NUMPAD0))));
    }
    switch (resolved_virtual_key) {
    case_((VK_LSHIFT)) return some$((O$daterm_key_Code)(daterm_key_Code_left_shift)) $end(case);
    case_((VK_RSHIFT)) return some$((O$daterm_key_Code)(daterm_key_Code_right_shift)) $end(case);
    case_((VK_LCONTROL)) return some$((O$daterm_key_Code)(daterm_key_Code_left_ctrl)) $end(case);
    case_((VK_RCONTROL)) return some$((O$daterm_key_Code)(daterm_key_Code_right_ctrl)) $end(case);
    case_((VK_LMENU)) return some$((O$daterm_key_Code)(daterm_key_Code_left_alt)) $end(case);
    case_((VK_RMENU)) return some$((O$daterm_key_Code)(daterm_key_Code_right_alt)) $end(case);
    case_((VK_LWIN)) return some$((O$daterm_key_Code)(daterm_key_Code_left_meta)) $end(case);
    case_((VK_RWIN)) return some$((O$daterm_key_Code)(daterm_key_Code_right_meta)) $end(case);
    case_((VK_UP)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_up : daterm_key_Code_keypad_8)) $end(case);
    case_((VK_DOWN)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_down : daterm_key_Code_keypad_2)) $end(case);
    case_((VK_RIGHT)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_right : daterm_key_Code_keypad_6)) $end(case);
    case_((VK_LEFT)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_left : daterm_key_Code_keypad_4)) $end(case);
    case_((VK_HOME)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_home : daterm_key_Code_keypad_7)) $end(case);
    case_((VK_END)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_end : daterm_key_Code_keypad_1)) $end(case);
    case_((VK_INSERT)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_insert : daterm_key_Code_keypad_0)) $end(case);
    case_((VK_DELETE)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_delete : daterm_key_Code_keypad_decimal)) $end(case);
    case_((VK_PRIOR)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_page_up : daterm_key_Code_keypad_9)) $end(case);
    case_((VK_NEXT)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_page_down : daterm_key_Code_keypad_3)) $end(case);
    case_((VK_CLEAR)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_5)) $end(case);
    case_((VK_TAB)) return some$((O$daterm_key_Code)(daterm_key_Code_tab)) $end(case);
    case_((VK_RETURN)) return some$((O$daterm_key_Code)(is_enhanced ? daterm_key_Code_keypad_enter : daterm_key_Code_enter)) $end(case);
    case_((VK_ESCAPE)) return some$((O$daterm_key_Code)(daterm_key_Code_escape)) $end(case);
    case_((VK_BACK)) return some$((O$daterm_key_Code)(daterm_key_Code_backspace)) $end(case);
    case_((VK_DECIMAL)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_decimal)) $end(case);
    case_((VK_ADD)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_add)) $end(case);
    case_((VK_SUBTRACT)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_minus)) $end(case);
    case_((VK_MULTIPLY)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_multiply)) $end(case);
    case_((VK_DIVIDE)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_divide)) $end(case);
    case_((VK_SEPARATOR)) return some$((O$daterm_key_Code)(daterm_key_Code_keypad_comma)) $end(case);
    default_() $do_nothing $end(default);
    }
    if (VK_F1 <= virtual_key && virtual_key <= VK_F24) {
        return some$((O$daterm_key_Code)(as$(daterm_key_Code)(
            as$(u16)(daterm_key_Code_f1) + as$(u16)(virtual_key - VK_F1))));
    }
    return none$((O$daterm_key_Code));
};

fn_((daterm_ANSI_parseWindowsKeyEvent(
    daterm_ANSI* self, KEY_EVENT_RECORD record
))(O$daterm_Event) $scope) {
    claim_assert_nonnull(self);
    let_(action, daterm_key_Action) = !record.bKeyDown
                                        ? daterm_key_Action_release
                                    : record.wRepeatCount > 1
                                        ? daterm_key_Action_repeat
                                        : daterm_key_Action_press;
    var_(mods, daterm_input_Mods) = daterm_ANSI__windows_keyMods(record.dwControlKeyState);
    if_some((daterm_ANSI__windows_keyCode(
           record.wVirtualKeyCode, record.wVirtualScanCode, record.dwControlKeyState
       ))(code)) {
        self->windows_surrogate = none$((O$daterm_ANSI__WindowsSurrogate));
        let_(resolved, daterm_key_Code) = code == daterm_key_Code_tab && mods.shift
                                            ? daterm_key_Code_back_tab
                                            : code;
        if (action != daterm_key_Action_release) {
            switch (resolved) {
            case daterm_key_Code_left_shift: $fallthrough;
            case_((daterm_key_Code_right_shift)) mods.shift = true $end(case);
            case daterm_key_Code_left_ctrl: $fallthrough;
            case_((daterm_key_Code_right_ctrl)) mods.ctrl = true $end(case);
            case daterm_key_Code_left_alt: $fallthrough;
            case_((daterm_key_Code_right_alt)) mods.alt = true $end(case);
            case daterm_key_Code_left_meta: $fallthrough;
            case_((daterm_key_Code_right_meta)) mods.meta = true $end(case);
            default_() $do_nothing $end(default);
            }
        }
        return_some(union_of((daterm_Event_key){
            .code = resolved,
            .mods = mods,
            .action = some$((O$daterm_key_Action)(action)),
        }));
    }
    let codeunit = as$(u16)(record.uChar.UnicodeChar);
    if (codeunit == 0) return_none();
    if (utf16_isHighSurrogate(codeunit)) {
        let_(surrogate, daterm_ANSI__WindowsSurrogate) = {
            .high = codeunit,
            .mods = mods,
            .action = action,
        };
        self->windows_surrogate = some$((O$daterm_ANSI__WindowsSurrogate)(surrogate));
        return_none();
    }
    if (utf16_isLowSurrogate(codeunit)) {
        let surrogate = orelse_((self->windows_surrogate)(return_none()));
        self->windows_surrogate = none$((O$daterm_ANSI__WindowsSurrogate));
        let codepoint = catch_((utf16_decodeSurrogatePair(surrogate.high, codeunit))(
            $ignore, return_none()
        ));
        return_some(union_of((daterm_Event_text){
            .codepoint = codepoint,
            .mods = surrogate.mods,
            .action = some$((O$daterm_key_Action)(surrogate.action)),
        }));
    }
    self->windows_surrogate = none$((O$daterm_ANSI__WindowsSurrogate));
    return_some(union_of((daterm_Event_text){
        .codepoint = as$(u32)(codeunit),
        .mods = mods,
        .action = some$((O$daterm_key_Action)(action)),
    }));
} $unscoped(fn);

fn_((daterm_ANSI__windows_mouseBtn(DWORD button_mask))(O$daterm_mouse_Btn)) {
    if ((button_mask & FROM_LEFT_1ST_BUTTON_PRESSED) != 0) {
        return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_left));
    }
    if ((button_mask & RIGHTMOST_BUTTON_PRESSED) != 0) {
        return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_right));
    }
    if ((button_mask & FROM_LEFT_2ND_BUTTON_PRESSED) != 0) {
        return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_middle));
    }
    if ((button_mask & FROM_LEFT_3RD_BUTTON_PRESSED) != 0) {
        return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_backward));
    }
    if ((button_mask & FROM_LEFT_4TH_BUTTON_PRESSED) != 0) {
        return some$((O$daterm_mouse_Btn)(daterm_mouse_Btn_forward));
    }
    return none$((O$daterm_mouse_Btn));
};

fn_((daterm_ANSI__windows_mouseEvent(daterm_ANSI* self, MOUSE_EVENT_RECORD record))(O$daterm_Event) $scope) {
    let button_mask = record.dwButtonState
                    & (FROM_LEFT_1ST_BUTTON_PRESSED
                       | RIGHTMOST_BUTTON_PRESSED
                       | FROM_LEFT_2ND_BUTTON_PRESSED
                       | FROM_LEFT_3RD_BUTTON_PRESSED
                       | FROM_LEFT_4TH_BUTTON_PRESSED);
    let_(pos, daterm_mouse_Pos) = {
        .x = as$(u16)(record.dwMousePosition.X),
        .y = as$(u16)(record.dwMousePosition.Y),
        .kind = daterm_mouse_PosKind_cell,
    };
    let mods = daterm_ANSI__windows_keyMods(record.dwControlKeyState);

    if ((record.dwEventFlags & MOUSE_WHEELED) != 0) {
        let delta = as$(SHORT)((record.dwButtonState >> 16) & 0xffff);
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_wheel){
            .wheel = delta < 0 ? daterm_mouse_Wheel_down : daterm_mouse_Wheel_up,
            .pos = pos,
            .mods = mods,
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    }
#if defined(MOUSE_HWHEELED)
    if ((record.dwEventFlags & MOUSE_HWHEELED) != 0) {
        let delta = as$(SHORT)((record.dwButtonState >> 16) & 0xffff);
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_wheel){
            .wheel = delta < 0 ? daterm_mouse_Wheel_left : daterm_mouse_Wheel_right,
            .pos = pos,
            .mods = mods,
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    }
#endif /* defined(MOUSE_HWHEELED) */

    if ((record.dwEventFlags & MOUSE_MOVED) != 0) {
        let btn = daterm_ANSI__windows_mouseBtn(button_mask);
        self->windows_mouse_buttons = button_mask;
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_motion){
            .btn = btn,
            .pos = pos,
            .mods = mods,
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    }

    let changed = button_mask ^ self->windows_mouse_buttons;
    if (changed == 0 && button_mask == 0) { return_none(); }
    let active = changed != 0 ? changed : button_mask;
    let btn = daterm_ANSI__windows_mouseBtn(active);
    self->windows_mouse_buttons = button_mask;
    if ((button_mask & active) != 0) {
        let pressed = orelse_((btn)(return_none()));
        let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_press){
            .btn = pressed,
            .pos = pos,
            .mods = mods,
        });
        return_some(union_of((daterm_Event_mouse)(mouse)));
    }
    let_(mouse, daterm_mouse_Event) = union_of((daterm_mouse_Event_release){
        .btn = btn,
        .pos = pos,
        .mods = mods,
    });
    return_some(union_of((daterm_Event_mouse)(mouse)));
} $unscoped(fn);

fn_((daterm_ANSI__windows_pollNativeEvent(daterm_ANSI* self))(O$daterm_Event) $scope) {
    let input = self->input_file.handle;
    while (WaitForSingleObject(input, 0) == WAIT_OBJECT_0) {
        var_(record, INPUT_RECORD) $undefined;
        var_(read_count, DWORD) = 0;
        if (!PeekConsoleInputW(input, &record, 1, &read_count) || read_count == 0) {
            return_none();
        }
        if (record.EventType == KEY_EVENT
            && self->input_mode == daterm_ANSI_InputMode_vt) {
            return_none();
        }
        if (!ReadConsoleInputW(input, &record, 1, &read_count) || read_count == 0) {
            return_none();
        }
        switch (record.EventType) {
        case_((KEY_EVENT)) {
            if_some((daterm_ANSI_parseWindowsKeyEvent(self, record.Event.KeyEvent))(event)) {
                return_some(event);
            }
        } $end(case);
        case_((WINDOW_BUFFER_SIZE_EVENT)){
            let_(size, daterm_Size) = {
                .cols = as$(u16)(record.Event.WindowBufferSizeEvent.dwSize.X),
                .rows = as$(u16)(record.Event.WindowBufferSizeEvent.dwSize.Y),
            };
            self->cached_screen_cells = some$((O$daterm_Size)(size));
            return_some(union_of((daterm_Event_resize)(size)));
        } $end(case);
        case_((MOUSE_EVENT)) {
            if_some((daterm_ANSI__windows_mouseEvent(self, record.Event.MouseEvent))(event)) {
                return_some(event);
            }
        } $end(case);
        case_((FOCUS_EVENT)) {
            return_some(union_of((daterm_Event_focus)(record.Event.FocusEvent.bSetFocus
                                                          ? daterm_focus_Event_in
                                                          : daterm_focus_Event_out)));
        } $end(case);
        default_() $do_nothing $end(default);
        };
    }
    return_none();
} $unscoped(fn);

fn_((daterm_ANSI__windows_inputReady(daterm_ANSI* self))(bool)) {
    let input = self->input_file.handle;
    while (WaitForSingleObject(input, 0) == WAIT_OBJECT_0) {
        var_(record, INPUT_RECORD) $undefined;
        var_(read_count, DWORD) = 0;
        if (!PeekConsoleInputW(input, &record, 1, &read_count) || read_count == 0) {
            return false;
        }
        if (record.EventType == KEY_EVENT) {
            return self->input_mode == daterm_ANSI_InputMode_vt;
        }
        if (record.EventType == MOUSE_EVENT
            || record.EventType == WINDOW_BUFFER_SIZE_EVENT
            || record.EventType == FOCUS_EVENT) {
            return false;
        }
        if (!ReadConsoleInputW(input, &record, 1, &read_count) || read_count == 0) {
            return false;
        }
    }
    return false;
};

fn_((daterm_ANSI__windows_enableRawMode(
    HANDLE input, daterm_ANSI_InputMode input_mode
))(E$DWORD) $scope) {
    var_(old_mode, DWORD) = 0;
    if (!GetConsoleMode(input, &old_mode)) return_err(E_cause$Unexpected());
    let new_mode = as$(DWORD)(ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)
                | (input_mode == daterm_ANSI_InputMode_vt
                       ? as$(DWORD)(ENABLE_VIRTUAL_TERMINAL_INPUT)
                       : as$(DWORD)(0));
    if (!SetConsoleMode(input, new_mode)) return_err(E_cause$Unexpected());
    FlushConsoleInputBuffer(input);
    if (!SetConsoleCtrlHandler(daterm_ANSI__windows_ctrlHandler, TRUE)) {
        catch_((daterm_ANSI__windows_disableRawMode(input, old_mode))($ignore, $do_nothing));
        return_err(E_cause$Unexpected());
    }
    return_ok(old_mode);
} $unscoped(fn);

fn_((daterm_ANSI__windows_disableRawMode(HANDLE input, DWORD old_in))(E$void) $scope) {
    SetConsoleCtrlHandler(daterm_ANSI__windows_ctrlHandler, FALSE);
    if (!SetConsoleMode(input, old_in)) return_err(E_cause$Unexpected());
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI__windows_enableVTerm(HANDLE output))(E$DWORD) $scope) {
    var_(old_mode, DWORD) = 0;
    if (!GetConsoleMode(output, &old_mode)) return_err(E_cause$Unexpected());
    let new_mode = old_mode | as$(DWORD)(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!SetConsoleMode(output, new_mode)) return_err(E_cause$Unexpected());
    return_ok(old_mode);
} $unscoped(fn);

fn_((daterm_ANSI__windows_disableVTerm(HANDLE output, DWORD old_out))(E$void) $scope) {
    if (!SetConsoleMode(output, old_out)) return_err(E_cause$Unexpected());
    return_ok({});
} $unscoped(fn);
#endif /* plat_is_windows */
#if plat_is_posix
fn_((daterm_ANSI__posix_enableRawMode(fs_File_Handle handle, daterm_ANSI_OutputMode output_mode))(E$posix_termios) $scope) {
    let fd = as$(int)(handle);
    var_(old, posix_termios) $undefined;
    if (tcgetattr(fd, &old) < 0) return_err(E_cause$Unexpected());
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
    if (tcsetattr(fd, TCSAFLUSH, &new) < 0) return_err(E_cause$Unexpected());
    return_ok(old);
} $unscoped(fn);

fn_((daterm_ANSI__posix_disableRawMode(fs_File_Handle handle, posix_termios old))(E$void) $scope) {
    let fd = as$(int)(handle);
    if (tcsetattr(fd, TCSAFLUSH, &old) < 0) return_err(E_cause$Unexpected());
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI__posix_enableResizeEvents(void))(E$posix_sigaction) $scope) {
    var_(old, posix_sigaction) $undefined;
    var_(action, posix_sigaction) = cleared();
    action.sa_handler = daterm_ANSI__posix_onResize;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGWINCH, &action, &old) < 0) return_err(E_cause$Unexpected());
    daterm_ANSI__posix_resize_pending = 0;
    return_ok(old);
} $unscoped(fn);

fn_((daterm_ANSI__posix_disableResizeEvents(posix_sigaction old))(E$void) $scope) {
    if (sigaction(SIGWINCH, &old, null) < 0) return_err(E_cause$Unexpected());
    daterm_ANSI__posix_resize_pending = 0;
    return_ok({});
} $unscoped(fn);

fn_((daterm_ANSI__posix_onResize(int sig))(void)) {
    let_ignore = sig;
    daterm_ANSI__posix_resize_pending = 1;
};
#endif /* plat_is_posix */
