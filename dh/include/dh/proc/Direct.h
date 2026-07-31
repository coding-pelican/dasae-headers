/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Direct.h
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Direct
 *
 * @brief   Caller-owned native provider for `proc_Self`
 */
#pragma once
#ifndef proc_Direct__included
#define proc_Direct__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../mem/Alctr.h"
#include "Self.h"
#include "std.h"

/*========== Macros and Declarations ========================================*/

/// Native process provider with explicit borrowed dependencies.
T_alias$((proc_Direct)(struct proc_Direct {
    var_(env, proc_Env);
    var_(gpa, mem_Alctr);
    var_(std, proc_std_Self);
}));
$attr($inline_always)
$static fn_((proc_Direct_init(proc_Env env, mem_Alctr gpa, proc_std_Self std))(proc_Direct));
$attr($must_check)
$extern fn_((proc_Direct_self(proc_Direct* self))(proc_Direct_E$proc_Self));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Direct_init(proc_Env env, mem_Alctr gpa, proc_std_Self std))(proc_Direct)) {
    return (proc_Direct){
        .env = env,
        .gpa = mem_Alctr_ensureValid(gpa),
        .std = proc_std_ensureValid(std),
    };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Direct__included */
