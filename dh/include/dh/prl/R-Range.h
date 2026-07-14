/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Range.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2026-06-14 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#pragma once
#ifndef prl_R_Range__included
#define prl_R_Range__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "L-Limit.h"

/*========== Macros and Declarations ========================================*/

#define R$(_$T...) tpl$(R, _$T)
#define R$$(_$T...) \
    struct { \
        var_(begin, L$$(_$T)); \
        var_(end, L$$(_$T)); \
    }
#define T_decl_R$(_$T...) \
    typedef struct R$(_$T) R$(_$T)
#define T_impl_R$(_$T...) \
    struct R$(_$T) { \
        var_(begin, L$(_$T)); \
        var_(end, L$(_$T)); \
    }
#define T_use_R$(_$T...) \
    T_decl_R$(_$T); \
    T_impl_R$(_$T)

#define R_bound(_$begin /*: L(_$T)*/, _$end /*: L(_$T)*/...) { .begin = _$begin, .end = _$end }
#define R_bound$(/*(_$R: R(_$T))(_$begin: L(_$T), _$end: L(_$T))*/... /*(_$R)*/) __val__R_bound$(__VA_ARGS__)

#define range_(_$begin, _$end) R_bound(__range___expandLimits _$begin, __range___expandLimits _$end)
#define range$(/*(_$R: R(_$T))(_$begin: L(_$T), _$end: L(_$T))*/... /*(_$R)*/) __val__range$(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __range___expandLimits(...) __VA_ARGS__
#define __val__range$(...) __step__range$__emit(__step__range$__parse __VA_ARGS__)
#define __step__range$__parse(_$R...) _$R, __step__range$__parseLimits
#define __step__range$__parseLimits(...) __VA_ARGS__
#define __step__range$__emit(...) __inline__range$(__VA_ARGS__)
#define __inline__range$(_$R, _$begin, _$end...) l$((_$R)range_(_$begin, _$end))

#define __val__R_bound$(...) __step__R_bound$__emit(__step__R_bound$__parse __VA_ARGS__)
#define __step__R_bound$__parse(_$R...) _$R, __step__R_bound$__parseLimits
#define __step__R_bound$__parseLimits(...) __VA_ARGS__
#define __step__R_bound$__emit(...) __inline__R_bound$(__VA_ARGS__)
#define __inline__R_bound$(_$R, _$begin, _$end...) l$((_$R)R_bound(_$begin, _$end))

T_use_R$(u8);
T_use_R$(u16);
T_use_R$(u32);
T_use_R$(ulong);
T_use_R$(u64);
T_use_R$(usize);
T_use_R$(i8);
T_use_R$(i16);
T_use_R$(i32);
T_use_R$(ilong);
T_use_R$(i64);
T_use_R$(isize);
T_use_R$(f32);
T_use_R$(f64);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_R_Range__included */
