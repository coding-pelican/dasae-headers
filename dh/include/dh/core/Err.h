/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Err.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2024-12-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
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

// Common error type
typedef enum Err {
    Err_none = 0,
    Err_out_of_memory,
    Err_insufficient_memory,
    Err_invalid_argument,
    Err_not_found,
    Err_access_denied,
    // Add more standard error types as needed
} Err;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_ERR_INCLUDED */
