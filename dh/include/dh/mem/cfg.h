/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2024-12-29 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef MEM_CFG_INCLUDED
#define MEM_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

#include <string.h>

/*========== Macros and Definitions =========================================*/

#if !defined(NMEM_TRACE)
#define MEM_TRACE_ENABLED (1)
#else
#define MEM_TRACE_ENABLED (0)
#endif

#define mem_trace_enabled (MEM_TRACE_ENABLED)

/*========== Memory Error ===================================================*/

/* Allocation Error type */
use_Err(
    mem_AllocErr,
    OutOfMemory
);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* MEM_CFG_INCLUDED */
