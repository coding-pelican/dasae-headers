/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Invoc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/start
 * @prefix  start_Invoc
 *
 * @brief   Native startup ABI adapter
 */
#pragma once
#ifndef start_Invoc__included
#define start_Invoc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Invoc/Args.h"
#include "Invoc/Env.h"
#include "../proc/Preopens.h"

/*========== Macros and Declarations ========================================*/

/// Caller-owned native startup adapters. Keep alive for the application main.
T_alias$((start_Invoc)(struct start_Invoc {
    var_(args, start_Invoc_Args);
    var_(env, start_Invoc_Env);
    var_(preopens, proc_Preopens);
    var_(owned_env_block, O$S_const$u16);
}));
T_use_prl$(start_Invoc);
$extern fn_((start_Invoc_initArgs(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv
))(void));
$extern fn_((start_Invoc_initArgsEnv(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv,
    usize envc, P_const$P_const$u8 envp
))(void));
$extern fn_((start_Invoc_initArgsEnvZ(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv,
    P_const$P_const$u8 envp
))(void));
#if plat_is_windows
$extern fn_((start_Invoc_initWin32(start_Invoc* self))(void));
#endif /* plat_is_windows */
#if in_analysis || plat_is_linux
$extern fn_((start_Invoc_initLinux(start_Invoc* self, P$raw initial_stack))(void));
#endif /* in_analysis || plat_is_linux */
$extern fn_((start_Invoc_fini(start_Invoc* self))(void));

$extern fn_((start_Invoc_args(start_Invoc* self))(proc_Args));
$extern fn_((start_Invoc_env(start_Invoc* self))(proc_Env));
$extern fn_((start_Invoc_preopens(start_Invoc* self))(proc_Preopens));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start_Invoc__included */
