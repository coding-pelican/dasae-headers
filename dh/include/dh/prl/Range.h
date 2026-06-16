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
#ifndef prl_Range__included
#define prl_Range__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Limit.h"

/*========== Macros and Declarations ========================================*/

#define R$(_T...) tpl$(R, _T)
#define R$$(_T...) \
    struct { \
        var_(begin, L$$(_T)); \
        var_(end, L$$(_T)); \
    }
#define T_decl_R$(_T...) \
    typedef struct R$(_T) R$(_T)
#define T_impl_R$(_T...) \
    struct R$(_T) { \
        var_(begin, L$(_T)); \
        var_(end, L$(_T)); \
    }
#define T_use_R$(_T...) \
    T_decl_R$(_T); \
    T_impl_R$(_T)

#define R_bound(_begin /*: L(_T)*/, _end /*: L(_T)*/...) { .begin = _begin, .end = _end }
#define R_bound$(/*(_R: R(_T))(_begin: L(_T), _end: L(_T))*/... /*(_R)*/) __val__R_bound$(__VA_ARGS__)

#define range_(_begin, _end) R_bound(__range___expandLimits _begin, __range___expandLimits _end)
#define range$(/*(_R: R(_T))(_begin: L(_T), _end: L(_T))*/... /*(_R)*/) __val__range$(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __range___expandLimits(...) __VA_ARGS__
#define __val__range$(...) __step__range$__emit(__step__range$__parse __VA_ARGS__)
#define __step__range$__parse(_R...) _R, __step__range$__parseLimits
#define __step__range$__parseLimits(...) __VA_ARGS__
#define __step__range$__emit(...) __inline__range$(__VA_ARGS__)
#define __inline__range$(_R, _begin, _end...) l$((_R)range_(_begin, _end))

#define __val__R_bound$(...) __step__R_bound$__emit(__step__R_bound$__parse __VA_ARGS__)
#define __step__R_bound$__parse(_R...) _R, __step__R_bound$__parseLimits
#define __step__R_bound$__parseLimits(...) __VA_ARGS__
#define __step__R_bound$__emit(...) __inline__R_bound$(__VA_ARGS__)
#define __inline__R_bound$(_R, _begin, _end...) l$((_R)R_bound(_begin, _end))

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
#endif /* prl_Range__included */
