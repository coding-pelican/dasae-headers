/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 */
#ifndef Thrd_base__included
#define Thrd_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

typedef Thrd_Id__Impl Thrd_Id;
typedef Thrd_Handle__Impl Thrd_Handle;

typedef struct Thrd Thrd;
T_decl_E$(Thrd);

typedef struct Thrd_Mtx Thrd_Mtx;
typedef struct Thrd_Mtx_Recur Thrd_Mtx_Recur;
typedef struct Thrd_Cond Thrd_Cond;
typedef struct Thrd_RWLock Thrd_RWLock;

typedef A$$(Thrd_max_name_len, u8) Thrd_NameBuf;
T_use_O$(Thrd_NameBuf);
T_use_E$(O$S_const$u8);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_base__included */
