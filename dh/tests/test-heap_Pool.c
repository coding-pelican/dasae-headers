#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/heap/Pool.h"

T_use$((u32)(
    ListSgl_Link,
    ListSgl,
    heap_Pool,
    heap_Pool_init,
    heap_Pool_fini,
    heap_Pool_create,
    heap_Pool_destroy,
    ListSgl_len
));

TEST_fn_("heap/Pool: reuses destroyed slots" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);

    var pool = try_(heap_Pool_init$u32(gpa, 2, heap_Pool_Opts_default()));
    defer_(heap_Pool_fini$u32(&pool, gpa));
    try_(TEST_expect(ListSgl_len$u32(&pool.free_list) == 2));

    let first = try_(heap_Pool_create$u32(&pool, gpa));
    try_(TEST_expect(ListSgl_len$u32(&pool.free_list) == 1));
    let second = try_(heap_Pool_create$u32(&pool, gpa));
    try_(TEST_expect(ListSgl_len$u32(&pool.free_list) == 0));
    try_(TEST_expect(first != second));

    heap_Pool_destroy$u32(&pool, second);
    try_(TEST_expect(ListSgl_len$u32(&pool.free_list) == 1));
    let reused = try_(heap_Pool_create$u32(&pool, gpa));
    try_(TEST_expect(ListSgl_len$u32(&pool.free_list) == 0));
    try_(TEST_expect(reused == second));
} $unguarded(TEST_fn);

TEST_fn_("heap/Pool: fixed pool reports out of memory" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);

    var pool = try_(heap_Pool_init$u32(gpa, 1, (heap_Pool_Opts){ .growable = false }));
    defer_(heap_Pool_fini$u32(&pool, gpa));

    let_ignore = try_(heap_Pool_create$u32(&pool, gpa));
    if_err((heap_Pool_create$u32(&pool, gpa))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$OutOfMemory().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unguarded(TEST_fn);
