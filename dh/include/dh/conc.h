/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    conc.h
 * @ingroup dasae-headers(dh)
 * @prefix  conc
 *
 * @brief   Scheduler-aware concurrency composition layer.
 */
#pragma once
#ifndef conc__included
#define conc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "conc/base.h"

#include "conc/Waker.h"
#include "conc/wait.h"
#include "conc/Wakeable.h"

#include "conc/Mtx.h"
#include "conc/Cond.h"

#include "conc/Waiter.h"

#include "conc/Sem.h"
#include "conc/RWLock.h"

#include "conc/Once.h"
#include "conc/OnceLock.h"
#include "conc/LazeLock.h"

#include "conc/OnceEvt.h"
#include "conc/ResetEvt.h"

#include "conc/Latch.h"
#include "conc/Group.h"

#include "conc/chan.h"
#include "conc/Select.h"

#include "conc/Que.h"
#include "conc/SPSC.h"
#include "conc/MPSC.h"
#include "conc/SPMC.h"
#include "conc/MPMC.h"

#include "conc/Batch.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc__included */
