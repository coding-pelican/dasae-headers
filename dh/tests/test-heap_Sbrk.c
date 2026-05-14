#include "dh-main.h"
#include "dh/heap/Sbrk.h"
#include "dh/io/stream.h"

TEST_fn_("heap/Sbrk: size variants" $scope) {
    let small_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_small)cleared()));
    let medium_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_medium)cleared()));
    let large_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_large)cleared()));

    io_stream_nl();
    io_stream_println(u8_l("SbrkAlctr sizes:"));
    io_stream_println(u8_l("  .small:  {:uz} bytes"), small_size);
    io_stream_println(u8_l("  .medium: {:uz} bytes"), medium_size);
    io_stream_println(u8_l("  .large:  {:uz} bytes"), large_size);
    io_stream_nl();

    try_(TEST_expect(small_size < medium_size));
    try_(TEST_expect(medium_size < large_size));
} $unscoped(TEST_fn);

typedef struct test_heap_Sbrk__TestCtx {
    var_(buf, A$$(1024 * 1024, u8) $align(heap_page_size));
    var_(cursor, usize);
} test_heap_Sbrk__TestCtx;
$static fn_((test_heap_Sbrk__TestCtx_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((test_heap_Sbrk__TestCtx*)(ctx));
    if (orelse_((usize_addChkd(self->cursor, n))(usize_limit_max)) > A_len(self->buf)) { return 0; }
    let result = ptrToInt(A_at((self->buf)[self->cursor]));
    self->cursor += n;
    return result;
};
$static fn_((test_heap_Sbrk__testCtx(test_heap_Sbrk__TestCtx* self))(heap_Sbrk_Ctx)) {
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = test_heap_Sbrk__TestCtx_sbrk,
    };
};
$static fn_((test_heap_Sbrk_ctx(void))(heap_Sbrk_Ctx)) {
    $static var_(ctx, test_heap_Sbrk__TestCtx) $undefined_static;
    mem_set0Bytes(mem_asBytesMut(u_anyP(&ctx)));
    return test_heap_Sbrk__testCtx(&ctx);
};

TEST_fn_("heap/Sbrk: basic allocation" $guard) {
    let ctx = test_heap_Sbrk_ctx();
    var local = l0$((heap_Sbrk_LocalMedium));
    var sbrk = heap_Sbrk_from(heap_Sbrk_LocalMedium_ref(&local), &ctx);
    let gpa = heap_Sbrk_alctr(&sbrk);

    let allocation = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 100))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(allocation)));

    try_(TEST_expect(allocation.len == 100));
} $unguarded(TEST_fn);

TEST_fn_("heap/Sbrk: reuses a bigpage before requesting another one" $guard) {
    let ctx = test_heap_Sbrk_ctx();
    var local = l0$((heap_Sbrk_LocalMedium));
    var sbrk = heap_Sbrk_from(heap_Sbrk_LocalMedium_ref(&local), &ctx);
    let gpa = heap_Sbrk_alctr(&sbrk);
    var_(slots, A$$(30, S$u8)) = cleared();

    let first = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 100))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(first)));

    for_(($s(A_ref((slots))))(slot)) {
        *slot = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 100))));
    } $end(for);
    defer_(for_(($s(A_ref((slots))))(slot)) {
        mem_Alctr_free($trace gpa, u_anyS(*slot));
    } $end(for));

    try_(TEST_expect(
        u_castP$((test_heap_Sbrk__TestCtx*)(ctx.inner))->cursor
        == heap_Sbrk__bigpage_size_static(heap_Sbrk_LocalRef_medium)
    ));
} $unguarded(TEST_fn);
