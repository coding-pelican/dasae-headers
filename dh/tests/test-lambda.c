#define main_no_args (1)
#include "dh/main.h"
#include "dh/callback.h"
#include <stdio.h>

// Original function pointer
typedef fn_((*IntBinaryOp)(i32, i32), i32);
// Compatible callback type that works with both function pointers and blocks
use_Callback(IntBinOp_compat, (i32 lhs, i32 rhs), i32);

// Function that accepts only function pointers
fn_(operateFnptr(i32 a, i32 b, IntBinaryOp op), void) {
    printf("Result: %d\n", op(a, b));
}
// Function that accepts the compatible callback type
fn_(operateCompat(i32 a, i32 b, IntBinOp_compat op), void) {
    printf("Result: %d\n", invoke(op, a, b));
}

// Function that adds two integers
fn_(funcAdd(i32 lhs, i32 rhs), i32) { return lhs + rhs; }


#include "dh/ArrList.h"
#include "dh/heap/Page.h"

#define cinq_select(var_collection, _PayloadCapture, _Body...)           comp_syn__cinq_select(var_collection, _PayloadCapture, _Body)
#define comp_syn__cinq_select(var_collection, _PayloadCapture, _Body...) eval({           \
    let __collection = var_collection;                                                    \
    var __temp       = try_(ArrList_initCap(__collection.type, __collection.allocator, 8)); \
    for_slice (__collection.items, __iter_elem) {                                         \
        var _PayloadCapture = *__iter_elem;                                               \
        var __item          = eval({ _Body; });                                           \
        try_(ArrList_append(&__temp, meta_refPtr_mut(&__item)));                           \
    };                                                                                    \
    eval_return type$(TypeOf(__collection), __temp);                                      \
})

// Example main function showing how to use the compatibility layer
fn_ext_scope(dh_main(void), Err$void) {
    var i32_list = type$(ArrList$$(i32), try_(ArrList_initCap(typeInfo$(i32), heap_Page_allocator(&(heap_Page){}), 8)));
    defer_(ArrList_fini(i32_list.base));
    i32_list.items.len = 8;
    printf("origin: ");
    for_slice_indexed (i32_list.items, item, index) {
        deref(item) = as$(i32, index) + 1;
        printf("%d ", deref(item));
    }
    printf("\n");
    var squared_list = cinq_select(i32_list, x, x * x);
    defer_(ArrList_fini(squared_list.base));
    printf("squared: ");
    for_slice (squared_list.items, item) {
        printf("%d ", deref(item));
    }
    printf("\n");

    // Create a block/lambda
    let lambdaAdd = lam_((i32 lhs, i32 rhs), i32) { return lhs + rhs; };

    // Using compatibility wrapper with function pointer
    IntBinOp_compat compatFn = wrapFn(funcAdd);
    printf("Function via compat: %d\n", invoke(compatFn, 10, 5));

    // Using compatibility wrapper with lambda/block
    IntBinOp_compat compatLambda = wrapLam(lambdaAdd);
    printf("Lambda via compat: %d\n", invoke(compatLambda, 10, 5));

    // Both function pointers and blocks work with the operate_compat function
    operateCompat(10, 5, (IntBinOp_compat)wrapFn(funcAdd));
    operateCompat(10, 5, wrapLam$(IntBinOp_compat, lambdaAdd));

    // Only function pointers work with operate_fnptr
    operateFnptr(10, 5, funcAdd);
    // operate_fnptr(10, 5, lambda_add); // This would fail!

    return_(ok({}));
} ext_unscoped;

/* typedef fn_((^LambdaOp)(i32 lhs, i32 rhs), i32);
fn_(add(i32 lhs, i32 rhs), i32) { return lhs + rhs; }

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    unused(args);
    LambdaOp real_lambda    = lambda((i32 lhs, i32 rhs), i32) { return lhs + rhs; };
    LambdaOp test_asg_fnptr = (LambdaOp)add;
    printf("real_lambda: %d\n", real_lambda(10, 5));
    printf("test_asg_fnptr: %d\n", test_asg_fnptr(10, 5));
    return_(ok({}));
} ext_unscoped; */

/**
 * @file callback.h
 * @brief Compatibility layer for function pointers and block objects
 */

#ifndef CALLBACK_INCLUDED
#define CALLBACK_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/mem.h"

/*========== Callback Wrapper ===============================================*/

// Define the callback wrapper structure - contains both function pointer and block
#define config_Callback(T_Callback, _Params, T_Return...) GEN__config_Callback(T_Callback, _Params, T_Return)

/// Create a wrapper from a function pointer or block obj
#define wrapFn(val_callbackFnPtr...)         comp_op__wrapFn(val_callbackFnPtr)
#define wrapLambda(val_callbackLambdaObj...) comp_op__wrapLambda(val_callbackLambdaObj)

#define wrapFn$(T_Callback, val_callbackFnPtr...)         comp_op__wrapFn$(T_Callback, val_callbackFnPtr)
#define wrapLambda$(T_Callback, val_callbackLambdaObj...) comp_op__wrapLambda$(T_Callback, val_callbackLambdaObj)

/// Invoke the callback wrapper with given arguments
#define invoke(val_wrapper, _Args...) comp_op__invoke(pp_uniqTok(wrapper), val_wrapper, _Args)

/*========== Macros and Implementations =====================================*/

#if BUILTIN_LANG_MODE_C && BUILTIN_COMP_CLANG
#define GEN__config_Callback(T_Callback, _Params, T_Return...) \
    typedef struct T_Callback {                                \
        bool is_lambda;                                        \
        union {                                                \
            fn_((*fnPtr)_Params, T_Return);                     \
            fn_((^lambdaObj)_Params, T_Return);                 \
        } callback;                                            \
    } T_Callback
#else  /* others */
typedef struct T_Callback {
    bool is_lambda;
    union {
        fn_((*fnPtr)_Params, T_Return);
        fn_((*lambdaObj)_Params, T_Return);
    } callback;
} T_Callback
#endif /* others */

#define comp_op__wrapFn(val_callbackFnPtr...)      \
    {                                              \
        .is_lambda = false,                        \
        .callback  = {.fnPtr = val_callbackFnPtr } \
    }
#define comp_op__wrapLambda(val_callbackLambdaObj...)      \
    {                                                      \
        .is_lambda = true,                                 \
        .callback  = {.lambdaObj = val_callbackLambdaObj } \
    }

#define comp_op__wrapFn$(T_Callback, val_callbackFnPtr...)         ((T_Callback)wrapFn(val_callbackFnPtr))
#define comp_op__wrapLambda$(T_Callback, val_callbackLambdaObj...) ((T_Callback)wrapLambda(val_callbackLambdaObj))

#define comp_op__invoke(__wrapper, val_wrapper, _Args...) eval({      \
    let __wrapper = val_wrapper;                                      \
    eval_return(                                                      \
        (__wrapper).is_lambda ? (__wrapper).callback.lambdaObj(_Args) \
                              : (__wrapper).callback.fnPtr(_Args)     \
    );                                                                \
})

/*========== Example Usage ==================================================*/

/// @brief Demonstrates usage of the callback compatibility layer
#if EXAMPLE_USAGE
// Example: Comparison function type
config_Callback(sort_CmpFn_compat, (anyptr_const lhs, anyptr_const rhs), cmp_Ord);

// Example: Invoke a comparison function
force_inline fn_(sort_CmpFn_compat_invoke(sort_CmpFn_compat cb, anyptr_const lhs, anyptr_const rhs), cmp_Ord) {
    return invoke(cb, lhs, rhs);
}
// Example: Sort function that can accept both function pointers and blocks
extern fn_(sort_insertionSort_compat(
    meta_Sli          base_sli,
    sort_CmpFn_compat cmpFn
), void);
// Actual implementation that uses the compatibility layer
fn_(sort_insertionSort_compat(
    meta_Sli          base_sli,
    sort_CmpFn_compat cmpFn
), void) {
    // Implementation that calls sort_CmpFn_compat_invoke(cmpFn, lhs, rhs) instead of cmpFn(lhs, rhs)
    // ...
}

#define main_no_args (1)
#include "dh/main.h"
#include <stdio.h>

// Original function pointer
typedef fn_((*IntBinaryOp)(i32, i32), i32);
// Compatible callback type that works with both function pointers and blocks
config_Callback(IntBinOp_compat, (i32 lhs, i32 rhs), i32);

// Function that accepts only function pointers
fn_(operateFnptr(i32 a, i32 b, IntBinaryOp op), void) {
    printf("Result: %d\n", op(a, b));
}
// Function that accepts the compatible callback type
fn_(operateCompat(i32 a, i32 b, IntBinOp_compat op), void) {
    printf("Result: %d\n", invoke(op, a, b));
}

// Function that adds two integers
fn_(funcAdd(i32 lhs, i32 rhs), i32) { return lhs + rhs; }
// Example main function showing how to use the compatibility layer
fn_ext_scope(dh_main(void), Err$void) {
    // Create a block/lambda
    let lambdaAdd = lambda((i32 lhs, i32 rhs), i32) { return lhs + rhs; };

    // Using compatibility wrapper with function pointer
    IntBinOp_compat compatFn = wrapFn(funcAdd);
    printf("Function via compat: %d\n", invoke(compatFn, 10, 5));

    // Using compatibility wrapper with lambda/block
    IntBinOp_compat compatLambda = wrapLambda(lambdaAdd);
    printf("Lambda via compat: %d\n", invoke(compatLambda, 10, 5));

    // Both function pointers and blocks work with the operate_compat function
    operateCompat(10, 5, (IntBinOp_compat)wrapFn(funcAdd));
    operateCompat(10, 5, wrapLambda$(IntBinOp_compat, lambdaAdd));

    // Only function pointers work with operate_fnptr
    operateFnptr(10, 5, funcAdd);
    // operate_fnptr(10, 5, lambda_add); // This would fail!

    return_(ok({}));
} ext_unscoped;
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CALLBACK_INCLUDED */
