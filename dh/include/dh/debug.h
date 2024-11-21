/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    debug.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  debug
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef DEBUG_INCLUDED
#define DEBUG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "debug/cfg.h"
#include "debug/common.h"
#include "debug/assert.h"

/*========== Macros and Definitions =========================================*/

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_INCLUDED */
