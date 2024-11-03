/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    compat_primitive.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/compat
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef COMPAT_PRIMITIVE_INCLUDED
#define COMPAT_PRIMITIVE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "compat_cfg_arch.h"

/*========== Macros and Definitions =========================================*/

/* Experimental features */
#define prim_Array(_T)                      RETURN_prim_Array(_T)
#define prim_ArrayN(_T, _n)                 RETURN_prim_ArrayN(_T, _n)
#define prim_array(_T)                      RETURN_prim_array(_T)
#define prim_arrayFrom(_T, _braced...)      RETURN_prim_arrayFrom(_T, _braced)
#define prim_arrayN(_T, _n)                 RETURN_prim_arrayN(_T, _n)
#define prim_arrayNWith(_T, _n, _braced...) RETURN_prim_arrayNWith(_T, _n, _braced)

// #define make(_T)                    RETURN_make(_T)
// #define makeWith(_T, _braced...)    RETURN_makeWith(_T, _braced)
// #define create(_T)                  RETURN_create(_T)
// #define createWith(_T, _braced...)  RETURN_createWith(_T, _braced)
// #define new(_T, _n)                 RETURN_new(_T, _n)
// #define newWith(_T, _n, _braced...) RETURN_newWith(_T, _n, _braced)

/*========== Macros Implementation ==========================================*/

#define RETURN_prim_Array(_T)                      _T[]
#define RETURN_prim_ArrayN(_T, _n)                 _T[_n]
#define RETURN_prim_array(_T)                      ((prim_Array(_T)){ 0 })
#define RETURN_prim_arrayFrom(_T, _braced...)      ((prim_Array(_T))_braced)
#define RETURN_prim_arrayN(_T, _n)                 ((prim_ArrayN(_T, _n)){ 0 })
#define RETURN_prim_arrayNWith(_T, _n, _braced...) ((prim_ArrayN(_T, _n))_braced)

// #define RETURN_make(_T)                    ((_T){ 0 })
// #define RETURN_makeWith(_T, _braced...)    ((_T)_braced)
// #define RETURN_create(_T)                  ((prim_Array(_T)){ 0 })
// #define RETURN_createWith(_T, _braced...)  ((prim_Array(_T))_braced)
// #define RETURN_new(_T, _n)                 ((prim_ArrayN(_T, _n)){ 0 })
// #define RETURN_newWith(_T, _n, _braced...) ((prim_ArrayN(_T, _n))_braced)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* COMPAT_PRIMITIVE_INCLUDED */
