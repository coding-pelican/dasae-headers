/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc
 *
 * @brief   Explicit borrowed startup context of the current process
 */
#pragma once
#ifndef proc_Self__included
#define proc_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Args.h"
#include "Env.h"
#include "Preopens.h"

/*========== Macros and Declarations ========================================*/

/// Startup context passed explicitly to `dh_main`.
///
/// No hidden current-process object is installed. The contained collection
/// views remain valid for the active `dh_main` invocation.
T_alias$((proc_Self)(struct proc_Self {
    var_(args, proc_Args);
    var_(env, proc_Env);
    var_(preopens, proc_Preopens);
}));
T_use_prl$(proc_Self);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Self__included */
