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

#define $limit_incl(_$point...) (L_incl(_$point))
#define $limit_excl(_$point...) (L_excl(_$point))

typedef enum_((L_Bound $fits($packed))(
    L_Bound_incl = true,
    L_Bound_excl = false
)) L_Bound;

#define L$(_$T...) tpl$(Limit, _$T)
#define L$$(_$T...) \
    struct { \
        var_(bound, L_Bound); \
        var_(point, _$T); \
    }
#define T_decl_L$(_$T...) \
    typedef struct L$(_$T) L$(_$T)
#define T_impl_L$(_$T...) \
    struct L$(_$T) { \
        var_(bound, L_Bound); \
        var_(point, _$T); \
    }
#define T_use_L$(_$T...) \
    T_decl_L$(_$T); \
    T_impl_L$(_$T)

#define L_incl(_$point...) { .bound = L_Bound_incl, .point = _$point }
#define L_incl$(/*(_$L: L(_$T))(_$point: _$T)*/... /*(_$L)*/) __val__L_incl$(__VA_ARGS__)
#define L_excl(_$point...) { .bound = L_Bound_excl, .point = _$point }
#define L_excl$(/*(_$L: L(_$T))(_$point: _$T)*/... /*(_$L)*/) __val__L_excl$(__VA_ARGS__)

#define incl_(_$point...) (L_incl(_$point))
#define incl$ L_incl$
#define excl_(_$point...) (L_excl(_$point))
#define excl$ L_excl$

/*========== Macros and Definitions =========================================*/

#define __val__L_incl$(...) __step__L_incl$__emit(__step__L_incl$__parse __VA_ARGS__)
#define __step__L_incl$__parse(_$L...) _$L,
#define __step__L_incl$__emit(...) __inline__L_incl$(__VA_ARGS__)
#define __inline__L_incl$(_$L, _$point...) l$((_$L)L_incl(_$point))

#define __val__L_excl$(...) __step__L_excl$__emit(__step__L_excl$__parse __VA_ARGS__)
#define __step__L_excl$__parse(_$L...) _$L,
#define __step__L_excl$__emit(...) __inline__L_excl$(__VA_ARGS__)
#define __inline__L_excl$(_$L, _$point...) l$((_$L)L_excl(_$point))

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
