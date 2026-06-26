/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    ANSI.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_ANSI
 */
#pragma once
#ifndef daterm_ANSI__included
#define daterm_ANSI__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "daterm-runtime/Term.h"
#include <dh/io/Buf.h>
#include <dh/io/TTY.h>
#include <dh/mem/Alctr.h>
#include <dh/time/Clock.h>

#if plat_is_windows
#include <dh/sys/api/windows/handle.h>
#endif /* plat_is_windows */
#if plat_is_posix
#include <dh/sys/posix.h>
#endif /* plat_is_posix */

/*========== Macros and Declarations ========================================*/

errset_((daterm_ANSI_E)(
    daterm_ANSI_Unsupported
));

typedef struct daterm_ANSI__RawMode {
    var_(tty_restore, io_TTY_Restore);
#if plat_is_posix
    var_(old_winch, sys_posix_sigaction);
#endif /* plat_is_posix */
} daterm_ANSI__RawMode;
T_use_prl$(daterm_ANSI__RawMode);

#if plat_is_windows
typedef struct daterm_ANSI__WindowsSurrogate {
    var_(high, u16);
    var_(mods, daterm_input_Mods);
    var_(action, daterm_key_Action);
} daterm_ANSI__WindowsSurrogate;
T_use_prl$(daterm_ANSI__WindowsSurrogate);
#endif /* plat_is_windows */

#define daterm_ANSI_input_buf_cap_default 256
#define daterm_ANSI_pending_event_cap 64
#define daterm_ANSI_esc_timeout_default time_Dur_fromMillis_static(8)

typedef enum_((daterm_ANSI_OutputMode $fits($packed))(
    daterm_ANSI_OutputMode_processed,
    daterm_ANSI_OutputMode_raw,
)) daterm_ANSI_OutputMode;
T_use_prl$(daterm_ANSI_OutputMode);

typedef enum_((daterm_ANSI_InputMode $fits($packed))(
    daterm_ANSI_InputMode_native,
    daterm_ANSI_InputMode_vt
)) daterm_ANSI_InputMode;
T_use_prl$(daterm_ANSI_InputMode);

typedef struct daterm_ANSI {
    var_(tty, io_TTY);
    var_(output_mode, daterm_ANSI_OutputMode);
    var_(input_mode, daterm_ANSI_InputMode);
    var_(raw_mode_, O$daterm_ANSI__RawMode);
    var_(is_tracking_mouse, bool);
    var_(mouse_report_mode_, u16);
    var_(mouse_pos_kind, daterm_mouse_PosKind);
    var_(is_tracking_focus, bool);
    var_(is_enhanced_keyboard, bool);
    var_(cached_screen_cells, O$daterm_CellSize);
#if plat_is_windows
    var_(windows_mouse_buttons, DWORD);
    var_(windows_surrogate, O$daterm_ANSI__WindowsSurrogate);
#endif /* plat_is_windows */
    var_(input_buf, struct {
        var_(reader, io_Buf_Reader);
        var_(is_owned, bool);
        var_(esc_started_at, O$time_Clock_Inst);
        var_(esc_timeout, time_Dur);
    });
    var_(pending, struct {
        var_(items, A$$(daterm_ANSI_pending_event_cap, daterm_Event));
        var_(head, usize);
        var_(len, usize);
    });
    var_(gpa, O$mem_Alctr);
    var_(clock, time_Clock);
} daterm_ANSI;
T_use_prl$(daterm_ANSI);
T_use_E$($set(mem_E)(daterm_ANSI));

typedef struct daterm_ANSI_Cfg { /* clang-format off */
    var_(gpa, O$mem_Alctr);
    var_(input_file, fs_File);
    var_(output_file, fs_File);
    var_(output_mode, daterm_ANSI_OutputMode);
    var_(input_mode, daterm_ANSI_InputMode);
    var_(esc_timeout, time_Dur);
    var_(input_buf, variant_(($fits($packed))(
        (daterm_ANSI_Cfg_input_buf_fixed, S$u8),
        (daterm_ANSI_Cfg_input_buf_owned, struct { var_(cap, usize); }),
    )));
    var_(clock, time_Clock);
} daterm_ANSI_Cfg; /* clang-format on */
T_use_prl$(daterm_ANSI_Cfg);
$attr($inline_always)
$static fn_((daterm_ANSI_Cfg_default(mem_Alctr gpa))(daterm_ANSI_Cfg));

$attr($must_check)
$extern fn_((daterm_ANSI_init(daterm_ANSI_Cfg cfg))(mem_E$daterm_ANSI));
$extern fn_((daterm_ANSI_fini(daterm_ANSI* self))(void));

$attr($must_check)
$extern fn_((daterm_ANSI_enableRawMode(daterm_ANSI* self))(E$void));
$extern fn_((daterm_ANSI_disableRawMode(daterm_ANSI* self))(void));
$extern fn_((daterm_ANSI_isInRawMode(const daterm_ANSI* self))(bool));

$extern fn_((daterm_ANSI_term(daterm_ANSI* self))(daterm_Term));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((daterm_ANSI_Cfg_default(mem_Alctr gpa))(daterm_ANSI_Cfg)) {
    return (daterm_ANSI_Cfg){
        .gpa = some(gpa),
        .input_file = io_handleStdIn(),
        .output_file = io_handleStdOut(),
        .output_mode = daterm_ANSI_OutputMode_processed,
#if plat_is_windows
        .input_mode = daterm_ANSI_InputMode_native,
#else
        .input_mode = daterm_ANSI_InputMode_vt,
#endif
        .esc_timeout = daterm_ANSI_esc_timeout_default,
        .input_buf = union_of((daterm_ANSI_Cfg_input_buf_owned){
            .cap = daterm_ANSI_input_buf_cap_default,
        }),
        .clock = union_of((time_Clock_awake)(catch_((time_Awake_direct())($ignore, time_Awake_noop)))),
    };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_ANSI__included */
