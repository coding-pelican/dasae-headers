#define main_no_args (1)
#include "dh/main.h"
#include "dh/callback.h"

#include <stdio.h>

// Original function pointer
typedef fn_(((*IntBinaryOp)(i32, i32))(i32));
// Compatible callback type that works with both function pointers and blocks
use_Callback(IntBinOp_compat, (i32 lhs, i32 rhs), i32);

// Function that accepts only function pointers
static fn_((operateFnptr(i32 a, i32 b, IntBinaryOp op))(void)) {
    printf("Result: %d\n", op(a, b));
}
// Function that accepts the compatible callback type
static fn_((operateCompat(i32 a, i32 b, IntBinOp_compat op))(void)) {
    printf("Result: %d\n", invoke(op, a, b));
}

// Function that adds two integers
static fn_((funcAdd(i32 lhs, i32 rhs))(i32)) {
    return lhs + rhs;
}

#include "dh/ArrList.h"
#include "dh/heap/Page.h"

#define cinq_select(var_collection, _PayloadCapture, _Body...)           comp_syn__cinq_select(var_collection, _PayloadCapture, _Body)
#define comp_syn__cinq_select(var_collection, _PayloadCapture, _Body...) eval({ \
    let __collection = var_collection; \
    var __temp       = try_(ArrList_initCap(__collection.type, __collection.allocator, 8)); \
    for_slice (__collection.items, __iter_elem) { \
        var _PayloadCapture = *__iter_elem; \
        var __item          = eval({ _Body; }); \
        try_(ArrList_append(&__temp, meta_refPtr(&__item))); \
    }; \
    eval_return type$(TypeOf(__collection), __temp); \
})

// Example main function showing how to use the compatibility layer
fn_((dh_main(void))(Err$void) $guard) {
    var i32_list = type$(ArrList$$(i32), try_(ArrList_initCap(typeInfo$(i32), heap_Page_allocator(&(heap_Page){}), 8)));
    defer_(ArrList_fini(i32_list.base));
    i32_list.items.len = 8;
    printf("origin: ");
    for_slice_indexed (i32_list.items, item, index) {
        deref(item) = as$((i32)(index)) + 1;
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
} $unguarded_(fn);
