/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Thrd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  Thrd
 *
 * @brief   Thread management utilities
 * @details Provides functionality for:
 *          - Thread creation and management
 *          - Thread synchronization primitives
 *          - Thread-local storage management
 *          - Thread-specific data handling
 */

#ifndef THRD_INCLUDED
#define THRD_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Thrd/cfg.h"
#include "Thrd/common.h"
#include "Thrd/Mtx.h"
#include "Thrd/Cond.h"
// #include "Thrd/RwLock.h"
// #include "Thrd/Sem.h"
// #include "Thrd/ResetEvent.h"
// #include "Thrd/WaitGroup.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_INCLUDED */
