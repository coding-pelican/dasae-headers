/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    claim.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  claim
 *
 * @brief   Assertion utilities
 * @details Provides assertion utilities for static and dynamic assertions.
 */

#ifndef CLAIM_INCLUDED
#define CLAIM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "claim/cfg.h"
#include "claim/assert_static.h"
#include "claim/assert.h"
#include "claim/unreachable.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_INCLUDED */
