/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Entry.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/start
 * @prefix  start_Entry
 *
 * @brief   Native startup adapter that creates a borrowed `proc_Entry`
 */
#pragma once
#ifndef start_Entry__included
#define start_Entry__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../proc/Entry.h"

/*========== Macros and Declarations ========================================*/

T_alias$((start_Entry_Argv)(P_const$P_const$u8));

/// Caller-owned native storage whose borrowed views back a `proc_Entry`.
///
/// It is an adapter for the platform entry ABI, not a process-domain value.
/// Keep it alive for the complete application `main` call.
T_alias$((start_Entry_Classic)(struct start_Entry_Classic {
    var_(argc, usize);
    var_(argv, start_Entry_Argv);
}));
T_alias$((start_Entry_Posix)(struct start_Entry_Posix {
    var_(argc, usize);
    var_(argv, start_Entry_Argv);
    var_(envc, usize);
    var_(envp, start_Entry_Argv);
}));
T_alias$((start_Entry_Win32)(struct start_Entry_Win32 {
    var_(cmd_line, S_const$u16);
    var_(env_block, O$S_const$u16);
}));
T_alias$((start_Entry)(variant_((start_Entry $fits($packed))(
    (start_Entry_classic, start_Entry_Classic),
    (start_Entry_posix, start_Entry_Posix),
    (start_Entry_win32, start_Entry_Win32)
))));
T_use_prl$(start_Entry);
$extern fn_((start_Entry_initClassic(start_Entry* self, usize argc, start_Entry_Argv argv))(void));
$extern fn_((start_Entry_initPosix(start_Entry* self, usize argc, start_Entry_Argv argv, start_Entry_Argv envp))(void));
#if plat_is_windows
$extern fn_((start_Entry_initWin32(start_Entry* self))(void));
#endif /* plat_is_windows */
#if plat_is_linux
$extern fn_((start_Entry_initLinux(start_Entry* self, P$raw initial_stack))(void));
#endif /* plat_is_linux */
$extern fn_((start_Entry_fini(start_Entry* self))(void));
$extern fn_((start_Entry_env(start_Entry* self))(proc_Env));
$extern fn_((start_Entry_entry(start_Entry* self, proc_Self proc))(proc_Entry));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start_Entry__included */
