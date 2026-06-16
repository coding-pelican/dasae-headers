/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd
 */
#pragma once
#ifndef thrd_base__included
#define thrd_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

typedef thrd_Id__Impl thrd_Id;
typedef thrd_Handle__Impl thrd_Handle;

typedef struct thrd_Self thrd_Self;
T_decl_E$(thrd_Self);

typedef struct thrd_Mtx thrd_Mtx;
typedef struct thrd_Mtx_Recur thrd_Mtx_Recur;
typedef struct thrd_Cond thrd_Cond;
typedef struct thrd_RWLock thrd_RWLock;

typedef A$$(thrd_max_name_len, u8) thrd_NameBuf;
T_use_O$(thrd_NameBuf);
T_use_E$(O$S_const$u8);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_base__included */
