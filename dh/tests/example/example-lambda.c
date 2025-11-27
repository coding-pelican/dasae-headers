#define main_no_args 1
#include "dh/main.h"
#include "dh/io/stream.h"

// Original function pointer
typedef fn_(((*IntBinaryOp)(i32, i32))(i32));
// Compatible callback type that works with both function pointers and blocks
use_Callable(IntBinOp_compat, (i32 lhs, i32 rhs), i32);

// Function that accepts only function pointers
$static fn_((operateFnptr(i32 a, i32 b, IntBinaryOp op))(void)) {
    io_stream_print(u8_l("Result: {:d}\n"), op(a, b));
}
// Function that accepts the compatible callback type
$static fn_((operateCompat(i32 a, i32 b, IntBinOp_compat op))(void)) {
    io_stream_print(u8_l("Result: {:d}\n"), invoke(op, a, b));
}
// Function that adds two integers
$static fn_((funcAdd(i32 lhs, i32 rhs))(i32)) {
    return lhs + rhs;
}

#include "dh/heap/Page.h"
#include "dh/ArrList.h"
T_use$((i32)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_appendWithin
));

// Example main function showing how to use the compatibility layer
fn_((dh_main(void))(E$void) $guard) {
    var gpa = heap_Page_allocator(&(heap_Page){});
    let init = ArrList_init$i32;
    let fini = ArrList_fini$i32;
    let append = ArrList_appendWithin$i32;

    var num_list = try_(init(gpa, 8));
    defer_(fini(&num_list, gpa));
    io_stream_print(u8_l("origin: "));
    for_(($r(0, num_list.cap))(idx) {
        append(&num_list, as$(i32)(idx) + 1);
        io_stream_print(u8_l("{:d} "), *S_at((num_list.items)[idx]));
    });
    io_stream_nl();

    var sq_list = chain$((ArrList$i32)(num_list.items)(
        map$((i32)(x)(*x * *x)),
        fold_(try_(init(gpa, num_list.cap)), (collect, x)(append(&collect, *x), collect))
    ));
    defer_(fini(&sq_list, gpa));
    io_stream_print(u8_l("squared: "));
    for_(($s(sq_list.items))(item) {
        io_stream_print(u8_l("{:d} "), *item);
    });
    io_stream_nl();

    // Create a block/lambda
    var lambdaAdd = la_((i32 lhs, i32 rhs)(i32)) { return lhs + rhs; };
    // Using compatibility wrapper with function pointer
    IntBinOp_compat compatFn = wrapFn(funcAdd);
    io_stream_print(u8_l("Function via compat: {:d}\n"), invoke(compatFn, 10, 5));
    // Using compatibility wrapper with lambda/block
    IntBinOp_compat compatLambda = wrapLa(lambdaAdd);
    io_stream_print(u8_l("Lambda via compat: {:d}\n"), invoke(compatLambda, 10, 5));
    // Both function pointers and blocks work with the operate_compat function
    operateCompat(10, 5, (IntBinOp_compat)wrapFn(funcAdd));
    operateCompat(10, 5, wrapLa$(IntBinOp_compat, lambdaAdd));
    // Only function pointers work with operate_fnptr
    operateFnptr(10, 5, funcAdd);
    // operate_fnptr(10, 5, lambda_add); // This would fail!

    let lambdaSq = la_((i32 x)(i32)) { return x * x; };
    var qd_list = chain$((ArrList$i32)(sq_list.items)(
        map$((i32)(x)(lambdaSq(*x))),
        fold_(try_(init(gpa, sq_list.cap)), (collect, x)(append(&collect, *x), collect))
    ));
    defer_(fini(&qd_list, gpa));
    io_stream_print(u8_l("quadrupled: "));
    for_(($s(qd_list.items))(item) {
        io_stream_print(u8_l("{:d} "), *item);
    });
    io_stream_nl();

    return_(ok({}));
} $unguarded_(fn);
