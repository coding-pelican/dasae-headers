/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-13 (date of creation)
 * @updated 2025-01-13 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/scope
 * @prefix  scope
 *
 * @brief   Configuration settings for scope management
 * @details Defines compile-time options and configuration parameters
 *          for scope-related operations and utilities.
 */

#ifndef SCOPE_CFG_INCLUDED
#define SCOPE_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

#if !defined(SCOPE_RESERVE_RETURN_NO_CONTAINS_DEFER)
#define SCOPE_RESERVE_RETURN_CONTAINS_DEFER (1)
#endif /* !defined(SCOPE_RESERVE_RETURN_NO_CONTAIN_DEFER) */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* SCOPE_CFG_INCLUDED */
