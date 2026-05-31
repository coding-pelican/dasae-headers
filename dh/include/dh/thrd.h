/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Thrd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  thrd
 *
 * @brief   Thread management utilities
 * @details Provides functionality for:
 *          - Thread creation and management
 *          - Thread synchronization primitives
 *          - Thread-local storage management
 *          - Thread-specific data handling
 */
#ifndef thrd__included
#define thrd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "thrd/cfg.h"
#include "thrd/base.h"
#include "thrd/common.h"
#include "thrd/Self.h"

#include "thrd/ftx.h"
#include "thrd/Mtx.h"
#include "thrd/Sem.h"
#include "thrd/Cond.h"

// #include "thrd/Once.h"
// #include "thrd/OnceLock.h"
// #include "thrd/LazyLock.h"
#include "thrd/RWLock.h"
#include "thrd/ResetEvent.h"
#include "thrd/WaitGroup.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd__included */
