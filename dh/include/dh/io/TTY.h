/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    TTY.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-24 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_TTY
 *
 * @brief   Attached terminal I/O device control
 */
#pragma once
#ifndef io_TTY__included
#define io_TTY__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"
#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

/*---------- Errors ---------------------------------------------------------*/

errset_((io_TTY_E)(
    io_TTY_Unsupported,
    io_TTY_NotTTY,
    io_TTY_BadHandle,
    io_TTY_ModeFailed,
    io_TTY_QueryFailed
));

/*---------- Terminal Mode --------------------------------------------------*/

T_alias$((io_TTY_ModeBit)(enum_((io_TTY_ModeBit $fits($packed))(
    io_TTY_ModeBit_none = u64_(0),
    io_TTY_ModeBit_echo = u64_(1) << 0,
    io_TTY_ModeBit_line_input = u64_(1) << 1,
    io_TTY_ModeBit_signal_input = u64_(1) << 2,
    io_TTY_ModeBit_extended_input = u64_(1) << 3,
    io_TTY_ModeBit_crlf_input = u64_(1) << 4,
    io_TTY_ModeBit_flow_input = u64_(1) << 5,
    io_TTY_ModeBit_break_input = u64_(1) << 6,
    io_TTY_ModeBit_parity_check = u64_(1) << 7,
    io_TTY_ModeBit_strip_input = u64_(1) << 8,
    io_TTY_ModeBit_vt_input = u64_(1) << 9,
    io_TTY_ModeBit_window_input = u64_(1) << 10,
    io_TTY_ModeBit_native_mouse = u64_(1) << 11,
    io_TTY_ModeBit_quick_edit = u64_(1) << 12,
    io_TTY_ModeBit_insert_input = u64_(1) << 13,
    io_TTY_ModeBit_output_process = u64_(1) << 32,
    io_TTY_ModeBit_vt_output = u64_(1) << 33,
    io_TTY_ModeBit_vt_auto_return = u64_(1) << 34,
    io_TTY_ModeBit_wrap_output = u64_(1) << 35
))));
claim_assert_static(eqlType$(io_TTY_ModeBit, u64));
T_alias$((io_TTY_ModeBits)(u64));

T_alias$((io_TTY_ModePatch)(struct io_TTY_ModePatch {
    var_(enable, io_TTY_ModeBits);
    var_(disable, io_TTY_ModeBits);
    var_(set_min_time, bool);
    var_(min_read, u8);
    var_(timeout_ds, u8);
}));
T_use_O$(io_TTY_ModePatch);

$attr($inline_always)
$static fn_((io_TTY_ModePatch_preserve(void))(io_TTY_ModePatch));
$attr($inline_always)
$static fn_((io_TTY_ModePatch_rawBytes(void))(io_TTY_ModePatch));
$attr($inline_always)
$static fn_((io_TTY_ModePatch_rawVT(void))(io_TTY_ModePatch));
$attr($inline_always)
$static fn_((io_TTY_ModePatch_cbreakVT(void))(io_TTY_ModePatch));

/*---------- Terminal State -------------------------------------------------*/

T_alias$((io_TTY_ModeState)(struct io_TTY_ModeState {
    var_(storage, A$$(8, u64));
}));
T_use_prl$(io_TTY_ModeState);

T_alias$((io_TTY_ModeSnapshot)(struct io_TTY_ModeSnapshot {
    var_(input, O$io_TTY_ModeState);
    var_(output, O$io_TTY_ModeState);
}));
T_use_E$($set(io_TTY_E)(io_TTY_ModeSnapshot));

T_alias$((io_TTY_CellSize)(struct io_TTY_CellSize {
    var_(cols, u16);
    var_(rows, u16);
}));
T_use_E$($set(io_TTY_E)(io_TTY_CellSize));

T_alias$((io_TTY_Pos)(struct io_TTY_Pos {
    var_(x, u16);
    var_(y, u16);
}));
T_use_E$($set(io_TTY_E)(io_TTY_Pos));

/*---------- Terminal Instance ----------------------------------------------*/

T_alias$((io_TTY_Cfg)(struct io_TTY_Cfg {
    var_(input_file, fs_File);
    var_(output_file, fs_File);
}));
$attr($inline_always)
$static fn_((io_TTY_Cfg_std(void))(io_TTY_Cfg));

T_alias$((io_TTY_Restore)(struct io_TTY_Restore {
    var_(snapshot, io_TTY_ModeSnapshot);
}));
T_use_O$(io_TTY_Restore);

T_alias$((io_TTY)(struct io_TTY {
    var_(input_file, fs_File);
    var_(output_file, fs_File);
    var_(restore, O$io_TTY_Restore);
}));

/*---------- Lifecycle ------------------------------------------------------*/

$extern fn_((io_TTY_init(io_TTY_Cfg cfg))(io_TTY));
$extern fn_((io_TTY_fini(io_TTY* self))(void));

/*---------- Reader and Writer Views ----------------------------------------*/

$extern fn_((io_TTY_reader(const io_TTY* self))(io_Reader));
$extern fn_((io_TTY_writer(const io_TTY* self))(io_Writer));

/*---------- Mode Control ---------------------------------------------------*/

$attr($must_check)
$extern fn_((io_TTY_snapshot(const io_TTY* self))(io_TTY_E$io_TTY_ModeSnapshot));
$attr($must_check)
$extern fn_((io_TTY_restore(const io_TTY* self, io_TTY_ModeSnapshot snapshot))(E$void));
$attr($must_check)
$extern fn_((io_TTY_applyModePatch(const io_TTY* self, io_TTY_ModePatch patch))(E$void));
$attr($must_check)
$extern fn_((io_TTY_enterMode(io_TTY* self, io_TTY_ModePatch patch))(E$void));
$extern fn_((io_TTY_leaveMode(io_TTY* self))(void));
$extern fn_((io_TTY_isInEnteredMode(const io_TTY* self))(bool));

/*---------- Queries --------------------------------------------------------*/

$attr($must_check)
$extern fn_((io_TTY_inputIsTTY(const io_TTY* self))(E$bool));
$attr($must_check)
$extern fn_((io_TTY_outputIsTTY(const io_TTY* self))(E$bool));
$attr($must_check)
$extern fn_((io_TTY_queryScreenCells(const io_TTY* self))(io_TTY_E$io_TTY_CellSize));
$attr($must_check)
$extern fn_((io_TTY_queryCursorPosNative(const io_TTY* self))(io_TTY_E$io_TTY_Pos));
$attr($must_check)
$extern fn_((io_TTY_inputReady(const io_TTY* self))(E$bool));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((io_TTY_ModePatch_preserve(void))(io_TTY_ModePatch)) {
    return (io_TTY_ModePatch){
        .enable = io_TTY_ModeBit_none,
        .disable = io_TTY_ModeBit_none,
        .set_min_time = false,
        .min_read = 0,
        .timeout_ds = 0,
    };
};
fn_((io_TTY_ModePatch_rawBytes(void))(io_TTY_ModePatch)) {
    return (io_TTY_ModePatch){
        .enable = io_TTY_ModeBit_none,
        .disable = io_TTY_ModeBit_echo
                 | io_TTY_ModeBit_line_input
                 | io_TTY_ModeBit_signal_input
                 | io_TTY_ModeBit_extended_input
                 | io_TTY_ModeBit_crlf_input
                 | io_TTY_ModeBit_flow_input
                 | io_TTY_ModeBit_break_input
                 | io_TTY_ModeBit_parity_check
                 | io_TTY_ModeBit_strip_input
                 | io_TTY_ModeBit_output_process,
        .set_min_time = true,
        .min_read = 1,
        .timeout_ds = 0,
    };
};
fn_((io_TTY_ModePatch_rawVT(void))(io_TTY_ModePatch)) {
    let raw = io_TTY_ModePatch_rawBytes();
    return (io_TTY_ModePatch){
        .enable = raw.enable
                | io_TTY_ModeBit_output_process
                | io_TTY_ModeBit_vt_input
                | io_TTY_ModeBit_vt_output
                | io_TTY_ModeBit_wrap_output,
        .disable = raw.disable & ~as$(io_TTY_ModeBits)(io_TTY_ModeBit_output_process),
        .set_min_time = raw.set_min_time,
        .min_read = raw.min_read,
        .timeout_ds = raw.timeout_ds,
    };
};
fn_((io_TTY_ModePatch_cbreakVT(void))(io_TTY_ModePatch)) {
    return (io_TTY_ModePatch){
        .enable = io_TTY_ModeBit_signal_input
                | io_TTY_ModeBit_vt_input
                | io_TTY_ModeBit_vt_output
                | io_TTY_ModeBit_wrap_output,
        .disable = io_TTY_ModeBit_echo
                 | io_TTY_ModeBit_line_input
                 | io_TTY_ModeBit_extended_input,
        .set_min_time = true,
        .min_read = 1,
        .timeout_ds = 0,
    };
};

fn_((io_TTY_Cfg_std(void))(io_TTY_Cfg)) {
    return (io_TTY_Cfg){
        .input_file = io_handleStdIn(),
        .output_file = io_handleStdOut(),
    };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_TTY__included */
