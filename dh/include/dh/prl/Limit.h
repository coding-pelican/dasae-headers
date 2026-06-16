/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Limit.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2026-06-14 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#pragma once
#ifndef prl_Limit__included
#define prl_Limit__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

#define $limit_incl(_point...) (L_incl(_point))
#define $limit_excl(_point...) (L_excl(_point))

typedef enum_((L_Bound $fits($packed))(
    L_Bound_incl = true,
    L_Bound_excl = false
)) L_Bound;

#define L$(_T...) tpl$(Limit, _T)
#define L$$(_T...) \
    struct { \
        var_(bound, L_Bound); \
        var_(point, _T); \
    }
#define T_decl_L$(_T...) \
    typedef struct L$(_T) L$(_T)
#define T_impl_L$(_T...) \
    struct L$(_T) { \
        var_(bound, L_Bound); \
        var_(point, _T); \
    }
#define T_use_L$(_T...) \
    T_decl_L$(_T); \
    T_impl_L$(_T)

#define L_incl(_point...) { .bound = L_Bound_incl, .point = _point }
#define L_incl$(/*(_L: L(_T))(_point: _T)*/... /*(_L)*/) __val__L_incl$(__VA_ARGS__)
#define L_excl(_point...) { .bound = L_Bound_excl, .point = _point }
#define L_excl$(/*(_L: L(_T))(_point: _T)*/... /*(_L)*/) __val__L_excl$(__VA_ARGS__)

#define incl_(_point...) (L_incl(_point))
#define incl$ L_incl$
#define excl_(_point...) (L_excl(_point))
#define excl$ L_excl$

/*========== Macros and Definitions =========================================*/

#define __val__L_incl$(...) __step__L_incl$__emit(__step__L_incl$__parse __VA_ARGS__)
#define __step__L_incl$__parse(_L...) _L,
#define __step__L_incl$__emit(...) __inline__L_incl$(__VA_ARGS__)
#define __inline__L_incl$(_L, _point...) l$((_L)L_incl(_point))

#define __val__L_excl$(...) __step__L_excl$__emit(__step__L_excl$__parse __VA_ARGS__)
#define __step__L_excl$__parse(_L...) _L,
#define __step__L_excl$__emit(...) __inline__L_excl$(__VA_ARGS__)
#define __inline__L_excl$(_L, _point...) l$((_L)L_excl(_point))

T_use_L$(u8);
T_use_L$(u16);
T_use_L$(u32);
T_use_L$(ulong);
T_use_L$(u64);
T_use_L$(usize);
T_use_L$(i8);
T_use_L$(i16);
T_use_L$(i32);
T_use_L$(ilong);
T_use_L$(i64);
T_use_L$(isize);
T_use_L$(f32);
T_use_L$(f64);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Range__included */
