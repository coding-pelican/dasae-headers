#include "dh-main.h"
#include "dh/heap/Sbrk.h"
#include "dh/io/stream.h"

#define test_heap_Sbrk__enabled_outstream pp_false
$static fn_((ignorePrintln(S_const$u8 fmt, ...))(void)) { let_ignore = fmt; };
$static let io_println = pp_if_(test_heap_Sbrk__enabled_outstream)(
    pp_then_(io_stream_println),
    pp_else_(ignorePrintln));
$static fn_((ignoreNL(void))(void)) $do_nothing;
$static let io_nl = pp_if_(test_heap_Sbrk__enabled_outstream)(
    pp_then_(io_stream_nl),
    pp_else_(ignoreNL));

TEST_fn_("heap/Sbrk: size variants" $scope) {
    let small_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_small)cleared()));
    let medium_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_medium)cleared()));
    let large_size = heap_Sbrk_LocalRef_calcSelfSize(union_of$((heap_Sbrk_LocalRef)(heap_Sbrk_LocalRef_large)cleared()));

    io_nl();
    io_println(u8_l("SbrkAlctr sizes:"));
    io_println(u8_l("  .small:  {:uz} bytes"), small_size);
    io_println(u8_l("  .medium: {:uz} bytes"), medium_size);
    io_println(u8_l("  .large:  {:uz} bytes"), large_size);
    io_nl();

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

    let allocation = try_(mem_Alctr_allocBytes($trace gpa, 100));
    defer_(mem_Alctr_freeBytes($trace gpa, allocation));

    try_(TEST_expect(allocation.len == 100));
} $unguarded(TEST_fn);

TEST_fn_("heap/Sbrk: reuses a bigpage before requesting another one" $guard) {
    let ctx = test_heap_Sbrk_ctx();
    var local = l0$((heap_Sbrk_LocalMedium));
    var sbrk = heap_Sbrk_from(heap_Sbrk_LocalMedium_ref(&local), &ctx);
    let gpa = heap_Sbrk_alctr(&sbrk);
    var_(slots, A$$(30, S$u8)) = cleared();

    let first = try_(mem_Alctr_allocBytes($trace gpa, 100));
    defer_(mem_Alctr_freeBytes($trace gpa, first));

    for_(($s(A_ref((slots))))(slot)) {
        *slot = try_(mem_Alctr_allocBytes($trace gpa, 100));
    } $end(for);
    defer_(for_(($s(A_ref((slots))))(slot)) {
        mem_Alctr_freeBytes($trace gpa, *slot);
    } $end(for));

    try_(TEST_expect(
        u_castP$((test_heap_Sbrk__TestCtx*)(ctx.inner))->cursor
        == heap_Sbrk__bigpage_size_static(heap_Sbrk_LocalRef_medium)
    ));
} $unguarded(TEST_fn);
