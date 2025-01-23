/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Cross-platform thread abstraction
 * @details Provides platform-independent thread management, synchronization primitives,
 *          and thread-local storage facilities.
 */

#ifndef THRD_CFG_INCLUDED
#define THRD_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin.h"
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"

/*========== Macros and Definitions ==========================================*/

#define Thrd_max_name_len VAL__Thrd_max_name_len
#if bti_plat_windows
typedef void* ThrdHandleImpl;
typedef u32   ThrdIdImpl;
#define VAL__Thrd_max_name_len (31)
#elif bti_plat_linux
typedef i32 ThrdHandleImpl;
typedef u32 ThrdIdImpl;
#define VAL__Thrd_max_name_len (15)
#elif bti_plat_darwin
typedef u64 ThrdHandleImpl;
typedef u64 ThrdIdImpl;
#define VAL__Thrd_max_name_len (63)
#else
typedef usize ThrdHandleImpl;
typedef usize ThrdIdImpl;
#define VAL__Thrd_max_name_len (0)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_CFG_INCLUDED */
