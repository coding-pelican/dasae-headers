/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Thrd.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  Thrd
 *
 * @brief   Cross-platform thread abstraction
 * @details Provides platform-independent thread management, synchronization primitives,
 *          and thread-local storage facilities.
 */

#ifndef THRD_INCLUDED
#define THRD_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Thrd/cfg.h"
#include "Thrd/common.h"
#include "Thrd/Ftx.h"
#include "Thrd/Mtx/Recur.h"
#include "Thrd/Mtx.h"
#include "Thrd/Cnd.h"
// #include "Thrd/Sem.h"
// #include "Thrd/ResetEvent.h"
// #include "Thrd/RwLock.h"
// #include "Thrd/Pool.h"
// #include "Thrd/WaitGroup.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_INCLUDED */
