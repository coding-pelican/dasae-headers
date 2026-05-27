/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    ANSI.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_ANSI
 */
#ifndef daterm_ANSI__included
#define daterm_ANSI__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "daterm-runtime/Term.h"
#include <dh/fs/File.h>
#include <dh/io/Buf.h>
#include <dh/io/common.h>
#include <dh/mem/Alctr.h>
#include <dh/time/Instant.h>

#if plat_is_windows
#include <dh/sys/api/windows/handle.h>
#endif /* plat_is_windows */
#if plat_is_posix
#include <termios.h>
#endif /* plat_is_posix */

errset_((daterm_ANSI_E)(
    daterm_ANSI_Unsupported
));

typedef struct daterm_ANSI__RawMode {
#if plat_is_windows
    var_(old_in, DWORD);
    var_(old_out, DWORD);
#endif /* plat_is_windows */
#if plat_is_posix
    var_(old_in, struct termios);
#endif /* plat_is_posix */
} daterm_ANSI__RawMode;
T_use_prl$(daterm_ANSI__RawMode);

#define daterm_ANSI_input_buf_cap_default 256
#define daterm_ANSI_report_buf_cap_default 64
#define daterm_ANSI_esc_timeout_default time_Duration_fromMillis_static(8)

typedef enum_((daterm_ANSI_OutputMode $fits($packed))(
    daterm_ANSI_OutputMode_processed,
    daterm_ANSI_OutputMode_raw,
)) daterm_ANSI_OutputMode;

typedef struct daterm_ANSI {
    var_(input_file, fs_File);
    var_(output_file, fs_File);
    var_(output_mode, daterm_ANSI_OutputMode);
    var_(raw_mode_, O$daterm_ANSI__RawMode);
    var_(is_in_alt_screen, bool);
    var_(is_tracking_mouse, bool);
    var_(input_buf, struct {
        var_(reader, io_Buf_Reader);
        var_(is_owned, bool);
        var_(esc_started_at, O$time_Instant);
        var_(esc_timeout, time_Duration);
    });
    var_(report_buf, struct {
        var_(mem, S$u8);
        var_(is_owned, bool);
    });
    var_(gpa, O$mem_Alctr);
} daterm_ANSI;
T_use_prl$(daterm_ANSI);
T_use_E$($set(mem_E)(daterm_ANSI));

typedef struct daterm_ANSI_Cfg { /* clang-format off */
    var_(gpa, O$mem_Alctr);
    var_(input_file, fs_File);
    var_(output_file, fs_File);
    var_(output_mode, daterm_ANSI_OutputMode);
    var_(esc_timeout, time_Duration);
    var_(input_buf, variant_(()(
        (daterm_ANSI_Cfg_input_buf_fixed, S$u8),
        (daterm_ANSI_Cfg_input_buf_owned, struct { var_(cap, usize); }),
    )));
    var_(report_buf, variant_(()(
        (daterm_ANSI_Cfg_report_buf_fixed, S$u8),
        (daterm_ANSI_Cfg_report_buf_owned, struct { var_(cap, usize); }),
    )));
} daterm_ANSI_Cfg; /* clang-format on */
T_use_prl$(daterm_ANSI_Cfg);
$attr($inline_always)
$static fn_((daterm_ANSI_Cfg_default(mem_Alctr gpa))(daterm_ANSI_Cfg)) {
    return (daterm_ANSI_Cfg){
        .gpa = some(gpa),
        .input_file = io_getStdIn(),
        .output_file = io_getStdOut(),
        .output_mode = daterm_ANSI_OutputMode_processed,
        .esc_timeout = daterm_ANSI_esc_timeout_default,
        .input_buf = union_of((daterm_ANSI_Cfg_input_buf_owned){
            .cap = daterm_ANSI_input_buf_cap_default,
        }),
        .report_buf = union_of((daterm_ANSI_Cfg_report_buf_owned){
            .cap = daterm_ANSI_report_buf_cap_default,
        }),
    };
};

$attr($must_check)
$extern fn_((daterm_ANSI_init(daterm_ANSI_Cfg cfg))(mem_E$daterm_ANSI));
$extern fn_((daterm_ANSI_fini(daterm_ANSI* self))(void));

$attr($must_check)
$extern fn_((daterm_ANSI_enableRawMode(daterm_ANSI* self))(E$void));
$extern fn_((daterm_ANSI_disableRawMode(daterm_ANSI* self))(void));
$extern fn_((daterm_ANSI_isInRawMode(const daterm_ANSI* self))(bool));

$attr($must_check)
$extern fn_((daterm_ANSI_enableMouseTracking(daterm_ANSI* self))(E$void));
$extern fn_((daterm_ANSI_disableMouseTracking(daterm_ANSI* self))(void));
$extern fn_((daterm_ANSI_isTrackingMouse(const daterm_ANSI* self))(bool));

$extern fn_((daterm_ANSI_term(daterm_ANSI* self))(daterm_Term));

$attr($must_check)
$extern fn_((daterm_ANSI_enter(daterm_ANSI* self))(E$void));
$extern fn_((daterm_ANSI_leave(daterm_ANSI* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_ANSI__included */
