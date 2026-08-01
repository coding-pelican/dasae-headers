#include "dh-main.h"
#include "dh/ArrList.h"
#include "dh/clsr.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"

typedef fn_(((*sample_IntBinFn)(i32 lhs, i32 rhs))(i32));

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_appendWithin
));

$static fn_((sample_add(i32 lhs, i32 rhs))(i32)) { return lhs + rhs; };
fn_use_Clsr_((sample_add)(i32, i32)(i32));

$static fn_((sample_operate(i32 lhs, i32 rhs, sample_IntBinFn opFn))(i32)) {
    return opFn(lhs, rhs);
};
$static fn_((sample_completeAdd(P$$(Clsr_(sample_add)) op))(i32)) {
    return op->rtn(&op->ctx)->ret;
};

$static fn_((sample_printList(S_const$u8 label, ArrList$i32 values))(void)) {
    io_stream_print(u8_l("{:s}: "), label);
    for_(($s(values.items))(value)) {
        io_stream_print(u8_l("{:d} "), *value);
    } $end(for);
    io_stream_nl();
};

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);

    let init = ArrList_init$i32;
    let fini = ArrList_fini$i32;
    let append = ArrList_appendWithin$i32;

    var numbers = try_(init(gpa, 8));
    defer_(fini(&numbers, gpa));
    for_(($r(0, numbers.cap))(idx)) append(&numbers, as$(i32)(idx) + 1) $end(for);
    sample_printList(u8_l("origin"), numbers);

    var squared = chain$((ArrList$i32)(numbers.items)(
        map$((i32)(value)(*value * *value)),
        fold_((try_(init(gpa, numbers.cap)))(collected, value)(
            append(&collected, *value), collected
        ))
    ));
    defer_(fini(&squared, gpa));
    sample_printList(u8_l("squared"), squared);

    let fn_result = sample_operate(10, 5, sample_add);
    io_stream_println(u8_l("function pointer: {:d}"), fn_result);

    var add_clsr = clsr_((sample_add)(10, 5));
    let clsr_result = sample_completeAdd(&add_clsr);
    io_stream_println(u8_l("captured closure: {:d}"), clsr_result);

    var quadrupled = chain$((ArrList$i32)(squared.items)(
        map$((i32)(value)(*value * *value)),
        fold_((try_(init(gpa, squared.cap)))(collected, value)(
            append(&collected, *value), collected
        ))
    ));
    defer_(fini(&quadrupled, gpa));
    sample_printList(u8_l("quadrupled"), quadrupled);

    return_ok({});
} $unguarded(fn);
