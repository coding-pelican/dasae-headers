#include "dh/main.h"

/* #undef return_
#undef comp_syn__return_
#undef return_ok
#undef comp_syn__return_ok
#undef return_err
#undef comp_syn__return_err
#undef return_some
#undef comp_syn__return_some
#undef return_none
#undef comp_syn__return_none
#undef return_void
#undef comp_syn__return_void

#define fn_scope(_Name_With_Params, T_Return...) comp_syn__fn_scope(_Name_With_Params, T_Return)
#define $unscoped                                 comp_syn__unscoped

#define return_(_Expr...)     comp_syn__return_(_Expr)
#define return_ok(_Expr...)   comp_syn__return_ok(_Expr)
#define return_err(_Expr...)  comp_syn__return_err(_Expr)
#define return_some(_Expr...) comp_syn__return_some(_Expr)
#define return_none()         comp_syn__return_none()
#define return_void()         comp_syn__return_void()

#define fn_scope_ext(_Name_With_Params, T_Return...) comp_syn__fn_scope_ext(_Name_With_Params, T_Return)
#define $unguarded                                 comp_syn__unscoped_ext

// clang-format off
#define comp_syn__fn_scope(_Name_With_Params, T_Return...) \
T_Return _Name_With_Params {                               \
    let __reserved_return = as$(T_Return*,                 \
        (u8[bti_Generic_match$(T_Return,                   \
            bti_Generic_pattern$(void) 0,                  \
            bti_Generic_fallback_ sizeOf$(T_Return))]){}); \
    if (false) { __step_return: goto __step_unscope; }     \
    do
#define comp_syn__unscoped                    \
    while (false);                            \
    if (false) { __step_unscope:              \
        if (bti_Generic_match$(TypeOf(*__reserved_return),  \
            bti_Generic_pattern$(void) false, \
            bti_Generic_fallback_ true)       \
        ) { return __reserved_return[0]; }    \
    }                                         \
}
// clang-format on

#define comp_syn__return_(_Expr...) blk({                        \
    bti_memcpy(                                                   \
        as$(u8*, __reserved_return),                              \
        as$(u8*, (TypeOf(*__reserved_return)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_return)                               \
    );                                                            \
    goto __step_return;                                           \
})
#define comp_syn__return_ok(_Expr...)   return_(ok(_Expr))
#define comp_syn__return_err(_Expr...)  return_(err(_Expr))
#define comp_syn__return_some(_Expr...) return_(some(_Expr))
#define comp_syn__return_none()         return_(none())
#define comp_syn__return_void()         blk({                                 \
    claim_assert_static(isSameType$(TypeOf(*__reserved_return), TypeOf(void))); \
    goto __step_return;                                                        \
})

struct __ScopeCounter {
    u32 is_returning : 1;
    u32 current_line : 31;
};
// clang-format off
#define comp_syn__fn_scope_ext(_Name_With_Params, T_Return...) \
T_Return _Name_With_Params {                                   \
    let __reserved_return = as$(T_Return*,                     \
        (u8[bti_Generic_match$(T_Return,                       \
            bti_Generic_pattern$(void) 0,                      \
            bti_Generic_fallback_ sizeOf$(T_Return))]){});     \
    var __scope_counter   = (struct __ScopeCounter){           \
        .is_returning = false, .current_line = __LINE__        \
    };                                                         \
    if (false) { __step_return:                                \
        __scope_counter.is_returning = true;                   \
        goto __step_deferred;                                  \
    }                                                          \
__step_deferred: switch (__scope_counter.current_line) {       \
    default: { goto __step_unscope; } break;                   \
    case __LINE__: __scope_counter.current_line = __LINE__ - 1;
#define comp_syn__unscoped_ext                             \
        break;                                             \
    }                                                      \
    if (false) { __step_unscope:                           \
        if (bti_Generic_match$(TypeOf(*__reserved_return), \
            bti_Generic_pattern$(void) false,              \
            bti_Generic_fallback_ true)                    \
        ) { return __reserved_return[0]; }                 \
    }                                                      \
}
// clang-format on

fn_scope_ext(func(void), E$void) {
    printf("func\n");
} $unguarded;

fn_scope_ext(dh_main(S$S_const$u8 args), E$void) {
    let_ignore = args;
    printf("begin\n");
    {
        u32 _defer_scope_prev        = __scope_counter.current_line;
        __scope_counter.current_line = 98;
        if ((0)) {
        case 98:
            __scope_counter.current_line = _defer_scope_prev;
            printf("hello\n");
            goto __step_deferred;
        }
    };
    printf("end\n");
    return_ok({});
} $unguarded; */

// E$void dh_main(S$S_const$u8 args) {
//     const __auto_type __reserved_return = ((E$void*)((u8[_Generic(E$void, void: 0, default: sizeof(E$void))]){}));
//     __auto_type       __scope_counter   = (struct __ScopeCounter){ .is_returning = (0), .current_line = 98 };
//     if ((0)) {
//     __step_return:
//         printf("returning\n");
//         __scope_counter.is_returning = (1);
//         goto __step_deferred;
//     }
// __step_deferred:
//     switch (__scope_counter.current_line) {
//     default:
//         printf("default\n");
//         goto __step_unscope;
//         break;
//     case 98:
//         printf("case begin\n");
//         __scope_counter.current_line = 98 - 1;
//         {
//             let_ignore = args;
//             printf("hello\n");
//         }
//         printf("case end\n");
//         break;
//     }
//     if ((0)) {
//     __step_unscope:
//         printf("unscope\n");
//         if (_Generic(__typeof__(*__reserved_return), void: (0), default: (1))) { return __reserved_return[0]; }
//     }
//     printf("unreachable\n");
// };

// clang-format off
/* #define comp_syn__fn_scope_ext(_Name_With_Params, T_Return...) \
T_Return _Name_With_Params {                                   \
    let __reserved_return = as$(T_Return*,                     \
        (u8[bti_Generic_match$(T_Return,                       \
            bti_Generic_pattern$(void) 0,                      \
            bti_Generic_fallback_ sizeOf$(T_Return))]){});     \
    var __scope_counter   = (struct __ScopeCounter){           \
        .is_returning = 0, .current_line = 0                   \
    };                                                         \
    if (false) { __step_return: goto __step_defer; }           \
    else { claim_unreachable; }                                \
    __step_defer:                                              \
    switch (__scope_counter.current_line) {                    \
    case bti_line():                                           \
        __scope_counter.current_line = bti_line() - 1;
#define comp_syn__unscoped_ext                    \
    default:                                      \
        if (false) { __step_unscope:              \
            if (bti_Generic_match$(TypeOf(void),  \
                bti_Generic_pattern$(void) false, \
                bti_Generic_fallback_ true)       \
            ) { return __reserved_return[0]; }    \
        } else { claim_unreachable; }             \
    }                                             \
} */
// clang-format on

/* E$void dh_main(S$S_const$u8 args) {
    E$void* __reserved_return = ((void*)0);
    struct {
        i32  curr;
        bool returns;
    } __scope_defer = { .curr = 0, .returns = (0) };
    if (0) {
    __step_return:
        __scope_defer.returns = (1);
        goto __deferred;
    }
__deferred:
    switch (__scope_defer.curr) {
    default:
        return (({ if ( ! __builtin_types_compatible_p ( __typeof__ ( * __reserved_return ) , void ) ) { comp_inline__debug_assert_nonnull ( __reserved_return ) ; } }), __reserved_return[0]);
    case 0:
        __scope_defer.curr = -1;
        { // main begin
            let_ignore = args;
            return_void();
        } // main end
#if COMP_TIME
        goto __step_return;
#endif
    }
}; */

/* // clang-format off
#define fn_scope(_Name_With_Params, T_Return) \
    T_Return _Name_With_Params {              \
        T_Return* __reserved_return = null;   \
        if (false) { __step_return: goto __step_unscope; } \
        else { claim_unreachable; }
#define $unscoped \
        if (false) { __step_unscope: return __reserved_return[0]; } \
        else { claim_unreachable; } \
    }
// clang-format on */

/* // clang-format off
#define fn_scope(_Name_With_Params, T_Return)                   \
T_Return _Name_With_Params {                                    \
    u8          __reserved_buf[bti_Generic_match$(TypeOf(void), \
        bti_Generic_pattern$(void) 0,                           \
        bti_Generic_fallback_ sizeOf(void)                      \
    )];                                                         \
    T_Return* const __reserved_return = (P$raw)__reserved_buf; \
    if (false) { __step_return: goto __step_unscope; }          \
    else { claim_unreachable; }
#define $unscoped                              \
    if (false) { __step_unscope:              \
        if (bti_Generic_match$(TypeOf(void),  \
            bti_Generic_pattern$(void) false, \
            bti_Generic_fallback_ true)       \
        ) { return __reserved_return[0]; }    \
    } else { claim_unreachable; }             \
}
// clang-format on */

/* #define ret_(_Expr...)                                                      \
    __reserved_return = bti_Generic_match$(                                 \
        TypeOf(*__reserved_return),                                         \
        bti_Generic_pattern$(void) null,                                    \
        bti_Generic_fallback_(TypeOf(*__reserved_return)[1]){ [0] = _Expr } \
    );                                                                      \
    goto __step_return */

/* #define ret_void blk({                                                        \
    claim_assert_static(isSameType$(TypeOf(*__reserved_return), TypeOf(void))); \
    goto __step_return;                                                        \
})
#define ret_(_Expr...) blk({                                       \
    bti_memcpy(                                                     \
        (P$raw)__reserved_return,                                  \
        (P$raw)&*((TypeOf(*__reserved_return)[1]){ [0] = _Expr }), \
        sizeof(*__reserved_return)                                  \
    );                                                              \
    goto __step_return;                                             \
}) */

// void func(void) {
//     u8          __reserved_buf[bti_Generic_match$(TypeOf(void), bti_Generic_pattern$(void) 0, bti_Generic_fallback_ sizeOf(void))];
//     void* const __reserved_return = (P$raw)__reserved_buf;
//     struct { u32 curr : 31; u32 returns : 1; } __scope_counter = { .curr    = 0, .returns = 0 };
//     if (0) { __step_return: goto __step_defer; }
//     __step_defer:
//     switch (__scope_counter.curr) {
//     case __LINE__:
//         __scope_counter.curr = __LINE__ - 1;
//         {
//             {
//                 u32 _defer_scope_prev = __scope_counter.curr;
//                 __scope_counter.curr    = 84;
//                 if ((0)) {
//                 case 84:
//                     __scope_counter.curr = _defer_scope_prev;
//                     goto __step_defer;
//                 }
//             };
//             return_void();
//         }
//     default:
//         return (({ if ( ! __builtin_types_compatible_p ( __typeof__ ( * __reserved_return ) , void ) ) { comp_inline__debug_assert_nonnull ( __reserved_return ) ; } }), __reserved_return[0]);
//     }
// };

/* E$void dh_main(S$S_const$u8 args) {
    E$void* __reserved_return = ((void*)0);
    struct {
        u32 curr    : 31;
        u32 returns : 1;
    } __scope_defer = { .curr = 0, .returns = (0) };
    if (0) {
    __step_return:
        __scope_defer.returns = (1);
        goto __deferred;
    }
__deferred:
    switch (__scope_defer.curr) {
    case __LINE__:
        __scope_defer.curr = __LINE__ - 1;
        { // main begin
            let_ignore = args;
            return_void();
        } // main end
#if COMP_TIME
        goto __step_return;
#endif
        break;
    default:
        return (({ if ( ! __builtin_types_compatible_p ( __typeof__ ( * __reserved_return ) , void ) ) { comp_inline__debug_assert_nonnull ( __reserved_return ) ; } }), __reserved_return[0]);
    }
}; */
