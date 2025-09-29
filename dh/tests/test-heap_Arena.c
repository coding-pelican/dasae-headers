#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/heap/Arena.h"

TEST_fn_("test heap_Arena reset while retaining a buffer" $guard) {
    var arena = heap_Arena_init(heap_Page_allocator(&(heap_Page){}));
    defer_(heap_Arena_fini(arena));
    let a = heap_Arena_allocator(&arena);

    // Create two internal buffers
    $ignore = try_(mem_Allocator_alloc(a, typeInfo$(u8), 1));
    $ignore = try_(mem_Allocator_alloc(a, typeInfo$(u8), 1000));

    // Check that we have at least two buffers
    try_(TEST_expect(isSome(unwrap(arena.state.buffer_list.first)->next)));

    // This retains the first allocated buffer
    try_(TEST_expect(heap_Arena_reset(&arena, variant_of$(heap_Arena_ResetMode, heap_Arena_ResetMode_retain_with_limit, 1))));
} $unguarded_TEST_fn;

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $guard) {
    $ignore   = args;
    var arena = heap_Arena_init(heap_Page_allocator(&(heap_Page){}));
    defer_(heap_Arena_fini(arena));
    let a = heap_Arena_allocator(&arena);

    // Create two internal buffers
    $ignore = try_(mem_Allocator_alloc(a, typeInfo$(u8), 1));
    $ignore = try_(mem_Allocator_alloc(a, typeInfo$(u8), 1000));

    // Check that we have at least two buffers
    try_(TEST_expect(isSome(unwrap(arena.state.buffer_list.first)->next)));

    // This retains the first allocated buffer
    try_(TEST_expect(heap_Arena_reset(&arena, variant_of$(heap_Arena_ResetMode, heap_Arena_ResetMode_retain_with_limit, 1))));
    return_ok({});
} $unguarded
