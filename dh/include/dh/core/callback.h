/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    callback.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-10 (date of creation)
 * @updated 2025-03-10 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Compatibility layer for function pointers and block objects
 */

#ifndef core_callback__included
#define core_callback__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/lambda.h"
#include "fn.h"

/*========== Callback Wrapper ===============================================*/

// Define the callback wrapper structure - contains both function pointer and block
#define Callback(_Params, T_Return...) comp_type_unnamed__Callback(_Params, T_Return)
#define use_Callback(T_Callback, _Params, T_Return...) comp_gen__use_Callback(T_Callback, _Params, T_Return)

/// Create a wrapper from a function pointer or block obj
#define wrapLam(val_callbackLamObj...) comp_op__wrapLam(val_callbackLamObj)
#define wrapFn(val_callbackFnPtr...) comp_op__wrapFn(val_callbackFnPtr)

#define wrapLam$(T_Callback, val_callbackLamObj...) comp_op__wrapLam$(T_Callback, val_callbackLamObj)
#define wrapFn$(T_Callback, val_callbackFnPtr...) comp_op__wrapFn$(T_Callback, val_callbackFnPtr)

/// Invoke the callback wrapper with given arguments
#define invoke(val_wrapper, _Args...) comp_op__invoke(pp_uniqTok(wrapper), val_wrapper, _Args)

/*========== Macros and Implementations =====================================*/

#if lang_mode == lang_mode_c && BUILTIN_COMP_CLANG
#define comp_type_unnamed__Callback(_Params, T_Return...) \
    struct { \
        union { \
            fn_(((^lamObj)_Params)(T_Return)); \
            fn_(((*fnPtr)_Params)(T_Return)); \
        } callback; \
        bool is_lam; \
    }
#define comp_gen__use_Callback(T_Callback, _Params, T_Return...) \
    typedef struct T_Callback { \
        union { \
            fn_(((^lamObj)_Params)(T_Return)); \
            fn_(((*fnPtr)_Params)(T_Return)); \
        } callback; \
        bool is_lam; \
    } T_Callback
#else /* others */
#define comp_type_unnamed__Callback(_Params, T_Return...) \
    struct { \
        union { \
            fn_(((^lamObj)_Params)(T_Return)); \
            fn_(((*fnPtr)_Params)(T_Return)); \
        } callback; \
        bool is_lam; \
    }
#define comp_gen__use_Callback(T_Callback, _Params, T_Return...) typedef struct T_Callback {
union {
    fn_(((^lamObj)_Params)(T_Return));
    fn_(((*fnPtr)_Params)(T_Return));
} callback;
bool is_lam;
}
T_Callback
#endif /* others */

#define comp_op__wrapLam(val_callbackLamObj...) {.callback = {.lamObj = val_callbackLamObj}, .is_lam = true}
#define comp_op__wrapFn(val_callbackFnPtr...) {.callback = {.fnPtr = val_callbackFnPtr}, .is_lam = false}

#define comp_op__wrapLam$(T_Callback, val_callbackLamObj...) ((T_Callback)wrapLam(val_callbackLamObj))
#define comp_op__wrapFn$(T_Callback, val_callbackFnPtr...) ((T_Callback)wrapFn(val_callbackFnPtr))

#define comp_op__invoke(__wrapper, val_wrapper, _Args...) \
    blk({ \
        let __wrapper = val_wrapper; \
        blk_return((__wrapper).is_lam ? (__wrapper).callback.lamObj(_Args) : (__wrapper).callback.fnPtr(_Args)); \
    })

/*========== Example Usage ==================================================*/

#if EXAMPLE_USAGE /* Demonstrates usage of the callback compatibility layer */
// Example: Comparison function type
use_Callback(sort_CmpFn_compat, (P_const$raw lhs, P_const$raw rhs), cmp_Ord);

// Example: Invoke a comparison function
$inline_always fn_((sort_CmpFn_compat_invoke(sort_CmpFn_compat cb, P_const$raw lhs, P_const$raw rhs))(cmp_Ord)) { return invoke(cb, lhs, rhs); }
// Example: Sort function that can accept both function pointers and blocks
extern fn_((sort_insertionSort_compat(meta_S base_sli, sort_CmpFn_compat cmpFn))(void));
// Actual implementation that uses the compatibility layer
fn_((sort_insertionSort_compat(meta_S base_sli, sort_CmpFn_compat cmpFn))(void)) {
    // Implementation that calls sort_CmpFn_compat_invoke(cmpFn, lhs, rhs) instead of cmpFn(lhs, rhs)
    // ...
}

#define main_no_args 1
#include "dh/main.h"
#include "dh/io/stream.h"

// Original function pointer
typedef fn_((i32, i32)(i32) $T)* IntBinaryOp;
// Compatible callback type that works with both function pointers and blocks
use_Callback(IntBinOp_compat, (i32 lhs, i32 rhs), i32);

// Function that accepts only function pointers
fn_((operateFnptr(i32 a, i32 b, IntBinaryOp op))(void)) {
    io_stream_print(u8_l("Result: {:d}\n"), op(a, b));
}
// Function that accepts the compatible callback type
fn_((operateCompat(i32 a, i32 b, IntBinOp_compat op))(void)) {
    io_stream_print(u8_l("Result: {:d}\n"), invoke(op, a, b));
}

// Function that adds two integers
fn_((funcAdd(i32 lhs, i32 rhs))(i32)) {
    return lhs + rhs;
}
// Example main function showing how to use the compatibility layer
fn_((dh_main(void))(E$void) $guard) {
    // Create a block/lambda
    let lambdaAdd = lam_((i32 lhs, i32 rhs)(i32)) { return lhs + rhs; };

    // Using compatibility wrapper with function pointer
    IntBinOp_compat compatFn = wrapFn(funcAdd);
    io_stream_print(u8_l("Function via compat: {:d}\n"), invoke(compatFn, 10, 5));

    // Using compatibility wrapper with lambda/block
    IntBinOp_compat compatLambda = wrapLam(lambdaAdd);
    io_stream_print(u8_l("Lambda via compat: {:d}\n"), invoke(compatLambda, 10, 5));

    // Both function pointers and blocks work with the operate_compat function
    operateCompat(10, 5, (IntBinOp_compat)wrapFn(funcAdd));
    operateCompat(10, 5, wrapLam$(IntBinOp_compat, lambdaAdd));

    // Only function pointers work with operate_fnptr
    operateFnptr(10, 5, funcAdd);
    // operate_fnptr(10, 5, lambda_add); // This would fail!

    return_(ok({}));
} $unguarded_(fn);
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_callback__included */
