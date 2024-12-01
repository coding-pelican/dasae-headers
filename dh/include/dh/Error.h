/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Error.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2024-12-01 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  Error
 *
 * @brief   Header for Error type
 * @details Common Error type for error presentation
 */

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

// Common error type
typedef enum Error {
    Error_none = 0,
    Error_out_of_memory,
    Error_invalid_argument,
    Error_not_found,
    Error_access_denied,
    // Add more standard error types as needed
} Error;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERROR_INCLUDED */
