/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Env.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Env
 *
 * @brief   Concrete borrowed process environment and iterator
 */
#pragma once
#ifndef proc_Env__included
#define proc_Env__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Args.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Env_Posix)(struct proc_Env_Posix {
    var_(count, usize);
    var_(items, proc_Args_Vec);
}));
T_alias$((proc_Env_Win32)(struct proc_Env_Win32 {
    var_(block, S_const$u16);
}));
T_alias$((proc_Env)(variant_((proc_Env $fits($packed))(
    (proc_Env_empty, Void),
    (proc_Env_posix, proc_Env_Posix),
    (proc_Env_win32, proc_Env_Win32)
))));
T_use_prl$(proc_Env);
$attr($inline_always)
$static fn_((proc_Env_isValid(proc_Env self))(bool));

$static let_(proc_Env_none, proc_Env) = union_of((proc_Env_empty){});
$attr($inline_always)
$static fn_((proc_Env_fromPosix(usize count, proc_Args_Vec items))(proc_Env));
$attr($inline_always)
$static fn_((proc_Env_fromWin32(S_const$u16 block))(proc_Env));

T_alias$((proc_Env_Iter)(struct proc_Env_Iter {
    var_(src, proc_Env);
    var_(idx, usize);
    var_(offset, usize);
}));
T_use_prl$(proc_Env_Iter);
$extern fn_((proc_Env_iter(proc_Env self))(proc_Env_Iter));
$attr($must_check)
$extern fn_((proc_Env_Iter_next(proc_Env_Iter* self, S$u8 scratch))(proc_Env_E$O$S_const$u8));
$attr($must_check)
$extern fn_((proc_Env_by(proc_Env self, S_const$u8 name, S$u8 scratch))(proc_Env_E$O$S_const$u8));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Env_isValid(proc_Env self))(bool)) {
    if (matches(self, proc_Env_empty)) return true;
    if (matches(self, proc_Env_posix)) {
        return isNonnull(union_to((self)(proc_Env_posix)).items);
    }
    if (matches(self, proc_Env_win32)) {
        return isNonnullS(union_to((self)(proc_Env_win32)).block);
    }
    return false;
};

fn_((proc_Env_fromPosix(usize count, proc_Args_Vec items))(proc_Env)) {
    claim_assert_nonnull(items);
    return union_of$((proc_Env)(proc_Env_posix){
        .count = count,
        .items = items,
    });
};
fn_((proc_Env_fromWin32(S_const$u16 block))(proc_Env)) {
    claim_assert_nonnullS(block);
    return union_of$((proc_Env)(proc_Env_win32){
        .block = block,
    });
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Env__included */
