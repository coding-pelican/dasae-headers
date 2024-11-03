/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    types.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "prim/prim.h"

/*========== Macros and Definitions =========================================*/

#define make(_T)                  RETURN_make(_T)
#define makeWith(_T, _initial...) RETURN_makeWith(_T, _initial)

#define create(_T)                  RETURN_create(_T)
#define createWith(_T, _initial...) RETURN_createWith(_T, _initial)

#define array(_T, _initial...)          RETURN_array(_T, _initial)
#define nArray(_N, _T)                  RETURN_nArray(_T, _N)
#define nArrayWith(_N, _T, _initial...) RETURN_nArrayWith(_T, _N, _initial)

/*========== Macros Implementation ==========================================*/

#define RETURN_make(_T) \
    (_T) { 0 }
#define RETURN_makeWith(_T, _initial...) \
    (_T) { _initial }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define RETURN_create(_T) \
    (_T[1]) {             \
        { 0 }             \
    }
#define RETURN_createWith(_T, _initial...) \
    (_T[1]) {                              \
        { _initial }                       \
    }

#define RETURN_array(_T, _initial...) \
    (_T[]) { _initial }
#define RETURN_nArray(_N, _T) \
    (_T[_N]) { 0 }
#define RETURN_nArrayWith(_N, _T, _initial...) \
    (_T[(_N)]) { _initial }
// NOLINTEND(bugprone-macro-parentheses)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TYPES_INCLUDED */
