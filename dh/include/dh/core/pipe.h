#pragma once
#ifndef core_pipe__included
#define core_pipe__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri.h"

/*========== Macros and Declarations ========================================*/

#define pipe_(/*(_$init)(<(_$capt)_$expr>...)*/...) __expr__pipe_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __expr__pipe_(...) __inline__pipe___emit(__inline__pipe___parseInit __VA_ARGS__)
#define __inline__pipe___parseInit(_$init...) pp_uniqTok(init), (_$init), /*_$steps*/
#define __inline__pipe___emit(...) __inline__pipe_(__VA_ARGS__)
#define __inline__pipe_(__init, _$init, _$steps...) local_({ \
    let __init = _$init; \
    __inline__pipe___steps__expand( \
        __inline__pipe___steps__emit( \
            __init, \
            pp_uniqTok(step), \
            __inline__pipe___expandSteps _$steps \
        ) \
    ); \
})
#define __inline__pipe___expandSteps(_$steps...) _$steps
#define __inline__pipe___steps__emit(...) __inline__pipe___steps(__VA_ARGS__)
#define __inline__pipe___steps(__curr, __step, _$step, /*_next*/...) \
    __VA_OPT__( \
        __inline__pipe___stepLoop__emit(__curr, __step, __inline__pipe___expandStep _$step); \
        pp_defer(__inline__pipe___stepsRecur)()(__step, pp_uniqTok(step), __VA_ARGS__) \
    ) \
    pp_if_(pp_not(pp_isParen(__VA_OPT__(()))))(pp_then_( \
        __inline__pipe___stepFinal___emit(__curr, __inline__pipe___expandStep _$step) \
    ))
#define __inline__pipe___stepsRecur() __inline__pipe___steps
#define __inline__pipe___expandStep(_$capt...) _$capt, /*_$expr*/
#define __inline__pipe___stepLoop__emit(...) __inline__pipe___stepLoop(__VA_ARGS__)
#define __inline__pipe___stepLoop(__prev, __curr, __capt, _$expr...) \
    let __curr = local_({ \
        let __capt = __prev; \
        local_return_(__inline__pipe___step__expr(_$expr)); \
    })
#define __inline__pipe___stepFinal___emit(...) __inline__pipe___stepFinal(__VA_ARGS__)
#define __inline__pipe___stepFinal(__prev, __capt, _$expr...) \
    let __capt = __prev; \
    local_return_(__inline__pipe___step__expr(_$expr))
#define __inline__pipe___step__expr(_$expr...) T_switch$((TypeOf(_$expr))( \
    T_case$((void)(($ignore_void _$expr), l0$((Void)))), \
    T_default_(_$expr) \
))
#define __inline__pipe___steps__expand(...) /* evaluate 2^5 */ __inline__pipe___steps__expand5(__VA_ARGS__)
#define __inline__pipe___steps__expand5(...) __inline__pipe___steps__expand4(__inline__pipe___steps__expand4(__VA_ARGS__))
#define __inline__pipe___steps__expand4(...) __inline__pipe___steps__expand3(__inline__pipe___steps__expand3(__VA_ARGS__))
#define __inline__pipe___steps__expand3(...) __inline__pipe___steps__expand2(__inline__pipe___steps__expand2(__VA_ARGS__))
#define __inline__pipe___steps__expand2(...) __inline__pipe___steps__expand1(__inline__pipe___steps__expand1(__VA_ARGS__))
#define __inline__pipe___steps__expand1(...) __VA_ARGS__

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pipe__included */
