/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Args.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Args
 *
 * @brief   Concrete borrowed process arguments and cross-platform iterator
 */
#pragma once
#ifndef proc_Args__included
#define proc_Args__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Args_Vec)(P_const$P_const$u8));
T_alias$((proc_Args_Posix)(struct proc_Args_Posix {
    var_(count, usize);
    var_(items, proc_Args_Vec);
}));
T_alias$((proc_Args_Win32)(struct proc_Args_Win32 {
    var_(cmd_line, S_const$u16);
}));
T_alias$((proc_Args)(variant_((proc_Args $fits($packed))(
    (proc_Args_posix, proc_Args_Posix),
    (proc_Args_win32, proc_Args_Win32)
))));
T_use_prl$(proc_Args);
$attr($inline_always)
$static fn_((proc_Args_fromPosix(usize count, proc_Args_Vec items))(proc_Args));
$attr($inline_always)
$static fn_((proc_Args_fromWin32(S_const$u16 cmd_line))(proc_Args));

T_alias$((proc_Args_Iter)(struct proc_Args_Iter {
    var_(src, proc_Args);
    var_(idx, usize);
    var_(offset, usize);
}));
T_use_prl$(proc_Args_Iter);
$extern fn_((proc_Args_iter(proc_Args self))(proc_Args_Iter));
$attr($must_check)
$extern fn_((proc_Args_Iter_next(
    proc_Args_Iter* self,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8));
$attr($must_check)
$extern fn_((proc_Args_Iter_skip(proc_Args_Iter* self))(proc_Args_E$bool));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Args_fromPosix(usize count, proc_Args_Vec items))(proc_Args)) {
    claim_assert_nonnull(items);
    return union_of$((proc_Args)(proc_Args_posix){
        .count = count,
        .items = items,
    });
};
fn_((proc_Args_fromWin32(S_const$u16 cmd_line))(proc_Args)) {
    claim_assert_nonnullS(cmd_line);
    return union_of$((proc_Args)(proc_Args_win32){
        .cmd_line = cmd_line,
    });
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Args__included */
