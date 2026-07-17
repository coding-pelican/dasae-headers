#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Arena.h"

TEST_fn_("heap/Arena: reset while retaining a buffer" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    // Create two internal buffers
    let_ignore = try_(mem_Alctr_allocBytes($trace gpa, 1));
    let_ignore = try_(mem_Alctr_allocBytes($trace gpa, 1024 * 1024));

    // Check that we have at least two buffers
    try_(TEST_expect(isSome(unwrap_(arena.state.buf_list.first)->next)));

    // This retains the first allocated buffer
    try_(TEST_expect(heap_Arena_reset(
        &arena, union_of$((heap_Arena_ResetMode)(heap_Arena_ResetMode_retain_with_limit)(1))
    )));
} $unguarded(TEST_fn);
