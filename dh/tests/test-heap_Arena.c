#include "dh-main.h"
#include "dh/heap/Page.h"
#include "dh/heap/Arena.h"

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(mem_Alctr_alloc));
TEST_fn_("test heap_Arena reset while retaining a buffer" $guard) {
    var arena = heap_Arena_init(heap_Page_alctr(&l0$((heap_Page))));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    // Create two internal buffers
    let_ignore = try_(mem_Alctr_alloc$u8($trace gpa, 1));
    let_ignore = try_(mem_Alctr_alloc$u8($trace gpa, 1000));

    // Check that we have at least two buffers
    try_(TEST_expect(isSome(unwrap_(arena.state.buf_list.first)->next)));

    // This retains the first allocated buffer
    try_(TEST_expect(heap_Arena_reset(
        &arena, union_of$((heap_Arena_ResetMode)(heap_Arena_ResetMode_retain_with_limit)(1))
    )));
} $unguarded(TEST_fn);
