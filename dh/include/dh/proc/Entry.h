/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Entry.h
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Entry
 *
 * @brief   Borrowed process facts and capability supplied to application main
 */
#pragma once
#ifndef proc_Entry__included
#define proc_Entry__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Args.h"
#include "Env.h"
#include "Preopens.h"
#include "Self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Entry)(struct proc_Entry {
    var_(proc, proc_Self);
    var_(args, proc_Args);
    var_(env, proc_Env);
    var_(preopens, proc_Preopens);
}));
T_use_prl$(proc_Entry);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Entry__included */
