/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-06-04 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Configuration settings for thread management
 * @details Defines compile-time options and configuration parameters
 *          for thread management.
 */
#ifndef THRD_CFG_INCLUDED
#define THRD_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/atom.h"

/*========== Macros and Definitions =========================================*/

#if !defined(Thrd_use_pthread)
#define Thrd_use_pthread 1
#endif /* !defined(Thrd_use_pthread) */

#if Thrd_use_pthread
#include <pthread.h>
// #include <sched.h>
// #include <semaphore.h>
#else /* !Thrd_use_pthread */
/* TODO: Implement other fallback support */
#endif /* !Thrd_use_pthread */
#if plat_is_windows
#include "dh/os/windows.h"
#endif /* plat_is_windows */
#if plat_is_darwin
#include <os/lock.h>
#endif /* plat_is_darwin */

#if Thrd_use_pthread
typedef usize Thrd_Id__Impl;
#define Thrd_invalid_id usize_limit_max
typedef pthread_t Thrd_Handle__Impl;
#define Thrd_max_name_len (15)
T_use_atom_V$(u32); /* for Thrd_Ftx, Thrd_Mtx */
typedef struct Thrd_Mtx__Impl Thrd_Mtx__Impl;
typedef struct Thrd_Cond__Impl Thrd_Cond__Impl;
typedef struct Thrd_RWLock__Impl Thrd_RWLock__Impl;
T_use_atom_V$(usize); /* for Thrd_WaitGroup */
#endif /* Thrd_use_pthread */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_CFG_INCLUDED */
