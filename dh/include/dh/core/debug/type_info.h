/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-08 (date of creation)
 * @updated 2026-05-08 (date of last update)
 * @ingroup dal-project/da-dsl/core/debug
 * @prefix  (none)
 *
 * @brief   Type information utilities for debugging
 * @details Provides structures and functions for tracking type information for debugging.
 */
#pragma once
#ifndef da_dsl_core_debug_type_info__included
#define da_dsl_core_debug_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../type_info.h"

/*========== Macros and Declarations ========================================*/

typedef struct debug_TypeInfo debug_TypeInfo;
#define debug_typeInfo$(_$T... /*(debug_TypeInfo)*/) __val__debug_typeInfo$(_$T)

#define $typing(_$type /*: TypeInfo*/... /*(debug_TypeInfo)*/) __val__$typing(_$type)
#define $typed(_$ti /*: debug_TypeInfo*/... /*(TypeInfo|Void)*/) __expr__$typed(_$ti)

/*========== Macros and Definitions =========================================*/

struct debug_TypeInfo { /* clang-format off */
    var_(impl, pp_if_(debug_enabled)(
        pp_then_(struct {
            var_(is_bound, bool);
            var_(inner, TypeInfo);
        }),
        pp_then_(struct {
            var_(inner, Void);
        })
    ));
}; /* clang-format on */
#define __val__debug_typeInfo$(_$T...) l$((debug_TypeInfo){ \
    .impl = pp_if_(debug_enabled)( \
        pp_then_({ .is_bound = true, .inner = typeInfo$(_$T) }), \
        pp_else_({ .inner = Void_({}) }) \
    ), \
})

#define __val__$typing(_$type...) l$((debug_TypeInfo){ \
    .impl = pp_if_(debug_enabled)( \
        pp_then_({ .is_bound = true, .inner = _$type }), \
        pp_else_({ .inner = Void_(_$type) }) \
    ), \
})
#define __expr__$typed(_$ti...) __pp__$typed__emit(pp_uniqTok(ti), _$ti)
#define __pp__$typed__emit(__ti, _$ti...) local_({ \
    let_(__ti, debug_TypeInfo) = _$ti; \
    debug_assert(__ti.impl.is_bound); \
    local_return_(__ti.impl.inner); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_core_debug_type_info__included */
