#include "dh/main.h"

/* Err$void dh_main(Sli$Str_const args) {
    Err$void* __reserved_return = ((void*)0);
    struct {
        i32  curr;
        bool returns;
    } __scope_defer = { .curr = 0, .returns = (0) };
    if (0) {
    __returned_scope:
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
            $ignore args;
            return_void();
        } // main end
#if COMP_TIME
        goto __returned_scope;
#endif
    }
}; */

/* // clang-format off
#define fn_scope(_Name_With_Params, T_Return) \
    T_Return _Name_With_Params {              \
        T_Return* __reserved_return = null;   \
        if (false) { __step_return: goto __step_unscope; } \
        else { claim_unreachable; }
#define unscoped \
        if (false) { __step_unscope: return __reserved_return[0]; } \
        else { claim_unreachable; } \
    }
// clang-format on */

// clang-format off
#define fn_scope(_Name_With_Params, T_Return)                   \
T_Return _Name_With_Params {                                    \
    u8          __reserved_buf[bti_Generic_match$(TypeOf(void), \
        bti_Generic_pattern$(void) 0,                           \
        bti_Generic_fallback_ sizeOf(void)                      \
    )];                                                         \
    T_Return* const __reserved_return = (anyptr)__reserved_buf; \
    if (false) { __step_return: goto __step_unscope; }          \
    else { claim_unreachable; }
#define unscoped                              \
    if (false) { __step_unscope:              \
        if (bti_Generic_match$(TypeOf(void),  \
            bti_Generic_pattern$(void) false, \
            bti_Generic_fallback_ true)       \
        ) { return __reserved_return[0]; }    \
    } else { claim_unreachable; }             \
}
// clang-format on

/* #define ret_(_Expr...)                                                      \
    __reserved_return = bti_Generic_match$(                                 \
        TypeOf(*__reserved_return),                                         \
        bti_Generic_pattern$(void) null,                                    \
        bti_Generic_fallback_(TypeOf(*__reserved_return)[1]){ [0] = _Expr } \
    );                                                                      \
    goto __step_return */

#define ret_void eval({                                                        \
    claim_assert_static(isSameType(TypeOf(*__reserved_return), TypeOf(void))); \
    goto __step_return;                                                        \
})
#define ret_(_Expr...) eval({                                       \
    bti_memcpy(                                                     \
        (anyptr)__reserved_return,                                  \
        (anyptr)&*((TypeOf(*__reserved_return)[1]){ [0] = _Expr }), \
        sizeof(*__reserved_return)                                  \
    );                                                              \
    goto __step_return;                                             \
})

// void func(void) {
//     u8          __reserved_buf[bti_Generic_match$(TypeOf(void), bti_Generic_pattern$(void) 0, bti_Generic_fallback_ sizeOf(void))];
//     void* const __reserved_return = (anyptr)__reserved_buf;
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

fn_scope(func(void), void) {
    ret_void;
}
unscoped;

fn_scope(funAc(void), i32) {
    ret_(12);
}
unscoped;

Err$void dh_main(Sli$Str_const args) {
    Err$void* __reserved_return = ((void*)0);
    struct {
        u32 curr    : 31;
        u32 returns : 1;
    } __scope_defer = { .curr = 0, .returns = (0) };
    if (0) {
    __returned_scope:
        __scope_defer.returns = (1);
        goto __deferred;
    }
__deferred:
    switch (__scope_defer.curr) {
    case __LINE__:
        __scope_defer.curr = __LINE__ - 1;
        { // main begin
            $ignore args;
            return_void();
        } // main end
#if COMP_TIME
        goto __returned_scope;
#endif
        break;
    default:
        return (({ if ( ! __builtin_types_compatible_p ( __typeof__ ( * __reserved_return ) , void ) ) { comp_inline__debug_assert_nonnull ( __reserved_return ) ; } }), __reserved_return[0]);
    }
};
