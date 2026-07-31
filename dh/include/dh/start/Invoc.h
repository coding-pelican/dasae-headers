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

#include "../proc/Args.h"
#include "../proc/Env.h"

/*========== Macros and Declarations ========================================*/

T_alias$((start_Invoc_ArgCnt)(usize));
T_alias$((start_Invoc_ArgVec)(P_const$P_const$u8));

/// Caller-owned native storage whose borrowed views back process facts.
///
/// It is an adapter for the platform entry ABI, not a process-domain value.
/// Keep it alive for the complete application `main` call.
T_alias$((start_Invoc)(variant_((start_Invoc $fits($packed))(
    (start_Invoc_classic, struct {
        var_(argc, start_Invoc_ArgCnt);
        var_(argv, start_Invoc_ArgVec);
    }),
    (start_Invoc_posix, struct {
        var_(argc, start_Invoc_ArgCnt);
        var_(argv, start_Invoc_ArgVec);
        var_(envc, start_Invoc_ArgCnt);
        var_(envp, start_Invoc_ArgVec);
    }),
    (start_Invoc_win32, struct {
        var_(cmd_line, S_const$u16);
        var_(env_block, O$S_const$u16);
    })
))));
T_use_prl$(start_Invoc);
$extern fn_((start_Invoc_initClassic(start_Invoc* self, start_Invoc_ArgCnt argc, start_Invoc_ArgVec argv))(void));
$extern fn_((start_Invoc_initPosix(start_Invoc* self, start_Invoc_ArgCnt argc, start_Invoc_ArgVec argv, start_Invoc_ArgVec envp))(void));
#if plat_is_windows
$extern fn_((start_Invoc_initWin32(start_Invoc* self))(void));
#endif /* plat_is_windows */
#if plat_is_linux
$extern fn_((start_Invoc_initLinux(start_Invoc* self, P$raw initial_stack))(void));
#endif /* plat_is_linux */
$extern fn_((start_Invoc_fini(start_Invoc* self))(void));
$extern fn_((start_Invoc_args(start_Invoc* self))(proc_Args));
$extern fn_((start_Invoc_env(start_Invoc* self))(proc_Env));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start_Invoc__included */
