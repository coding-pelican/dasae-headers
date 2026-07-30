/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Args.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Args
 *
 * @brief   Borrowed argument view supplied to the current process
 */
#pragma once
#ifndef proc_Args__included
#define proc_Args__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/// Normalized command-line arguments in startup order.
///
/// The outer slice storage is borrowed from the startup bridge and remains
/// valid for the active `dh_main` invocation. Argument byte strings are opaque
/// on non-Windows targets and WTF-8 is the intended Windows representation.
T_alias$((proc_Args)(struct proc_Args {
    var_(items, S_const$S_const$u8);
}));
T_use_prl$(proc_Args);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Args__included */
