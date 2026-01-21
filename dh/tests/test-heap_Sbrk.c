#include "dh/main.h"
#include "dh/heap/Sbrk.h"
#include "dh/io/stream.h"

TEST_fn_("SbrkAllocator size variants" $scope) {
    let small_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_small)cleared()));
    let medium_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_medium)cleared()));
    let large_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_large)cleared()));

    io_stream_nl();
    io_stream_println(u8_l("SbrkAllocator sizes:"));
    io_stream_println(u8_l("  .small:  {:uz} bytes"), small_size);
    io_stream_println(u8_l("  .medium: {:uz} bytes"), medium_size);
    io_stream_println(u8_l("  .large:  {:uz} bytes"), large_size);
    io_stream_nl();

    try_(TEST_expect(small_size < medium_size));
    try_(TEST_expect(medium_size < large_size));
} $unscoped_(TEST_fn);

typedef struct test_heap_Sbrk_TestCtx {
    var_(buf, A$$(1024 * 1024, u8)) $align(heap_page_size);
    var_(cursor, usize);
} test_heap_Sbrk_TestCtx;
$static fn_((test_heap_Sbrk_TestCtx_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((test_heap_Sbrk_TestCtx*)(ctx));
    if (self->cursor + n > A_len(self->buf)) { return 0; }
    let result = ptrToInt(A_at((self->buf)[self->cursor]));
    self->cursor += n;
    return result;
};
$static fn_((test_heap_Sbrk_testCtx(test_heap_Sbrk_TestCtx* self))(heap_Sbrk_Ctx)) {
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = test_heap_Sbrk_TestCtx_sbrk,
    };
};

TEST_fn_("SbrkAllocator basic allocation" $guard) {
    $static var_(test_ctx, test_heap_Sbrk_TestCtx) = cleared();
    let ctx = test_heap_Sbrk_testCtx(&test_ctx);
    var local = lit0$((heap_Sbrk_LocalMedium));
    var sbrk = heap_Sbrk_from(heap_Sbrk_LocalMedium_ref(&local), &ctx);
    let gpa = heap_Sbrk_allocator(&sbrk);

    let allocation = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), 100))));
    defer_(mem_Allocator_free(gpa, u_anyS(allocation)));

    try_(TEST_expect(allocation.len == 100));
} $unguarded_(TEST_fn);
