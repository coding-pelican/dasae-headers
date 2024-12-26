/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Err.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/Core
 * @prefix  Err
 *
 * @brief   Header for Err type
 * @details Common Err type for error presentation
 */

#ifndef CORE_ERR_INCLUDED
#define CORE_ERR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

typedef enum Err {
    Err_None = 0,
    Err_NotImplemented,
    Err_OutOfMemory,
    Err_InsufficientMemory,
    Err_InvalidArgument,
    Err_NotFound,
    Err_AccessDenied,
    // Add more standard error types as needed
} Err;

// TODO: Make extern
static_inline const char* Err_parseToCStr(Err err) {
    switch (err) {
    case Err_None:
        return "None";
    case Err_NotImplemented:
        return "NotImplemented";
    case Err_OutOfMemory:
        return "OutOfMemory";
    case Err_InsufficientMemory:
        return "InsufficientMemory";
    case Err_InvalidArgument:
        return "InvalidArgument";
    case Err_NotFound:
        return "NotFound";
    case Err_AccessDenied:
        return "AccessDenied";
    default:
        return claim_unreachable_fmt("Unknown error type (err: %d)", err), "UnknownError";
    }
}

#define Err_unreachable(_err) \
    claim_unreachable_fmt("Unexpected error: %s", Err_parseToCStr(_err))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_ERR_INCLUDED */
