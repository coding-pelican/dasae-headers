/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-09 (date of creation)
 * @updated 2025-12-09 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  cmp
 */

#ifndef cmp__included
#define cmp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations =========================================*/

typedef enum_(cmp_MathType $bits(8)) {
    cmp_MathType_u8 = 0,
    cmp_MathType_u16,
    cmp_MathType_u32,
    cmp_MathType_u64,
    cmp_MathType_usize,
    cmp_MathType_i8,
    cmp_MathType_i16,
    cmp_MathType_i32,
    cmp_MathType_i64,
    cmp_MathType_isize,
    cmp_MathType_f32,
    cmp_MathType_f64,
} cmp_MathType;

typedef u_EqlFn cmp_EqlFn;
$extern fn_((cmp_EqlFn_default(cmp_MathType type))(cmp_EqlFn));
typedef u_EqlCtxFn cmp_EqlCtxFn;
$extern fn_((cmp_EqlCtxFn_default(cmp_MathType type))(cmp_EqlCtxFn));

typedef u_OrdFn cmp_OrdFn;
$extern fn_((cmp_OrdFn_defaultAsc(cmp_MathType type))(cmp_OrdFn));
$extern fn_((cmp_OrdFn_defaultDesc(cmp_MathType type))(cmp_OrdFn));
typedef u_OrdCtxFn cmp_OrdCtxFn;
$extern fn_((cmp_OrdCtxFn_defaultAsc(cmp_MathType type))(cmp_OrdCtxFn));
$extern fn_((cmp_OrdCtxFn_defaultDesc(cmp_MathType type))(cmp_OrdCtxFn));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* cmp__included */
