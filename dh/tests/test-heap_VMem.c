#include "dh-main.h"
#include "dh/heap/VMem.h"

T_alias$((test_heap_VMem_Ctx)(struct test_heap_VMem_Ctx {
    var_(byte, u8);
    var_(reserve_len, usize);
    var_(commit_len, usize);
    var_(decommit_len, usize);
    var_(protect_len, usize);
    var_(release_len, usize);
    var_(protect, heap_VMem_Protcn);
    var_(hint_was_none, bool);
}));

$static fn_((test_heap_VMem__reserve(
    const heap_VMem_Ctx* ctx, O$P$raw addr_hint, usize len
))(O$P$u8)) {
    let self = ptrCast$((test_heap_VMem_Ctx*)(ensureNonnull(ctx)->inner));
    self->reserve_len = len;
    self->hint_was_none = isNone(addr_hint);
    return some$((O$P$u8)(&self->byte));
};

$static fn_((test_heap_VMem__commit(
    const heap_VMem_Ctx* ctx, P$raw addr, usize len
))(bool)) {
    let self = ptrCast$((test_heap_VMem_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->commit_len = len;
    return true;
};

$static fn_((test_heap_VMem__decommit(
    const heap_VMem_Ctx* ctx, P$raw addr, usize len
))(bool)) {
    let self = ptrCast$((test_heap_VMem_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->decommit_len = len;
    return true;
};

$static fn_((test_heap_VMem__protect(
    const heap_VMem_Ctx* ctx, P$raw addr, usize len, heap_VMem_Protcn protect
))(bool)) {
    let self = ptrCast$((test_heap_VMem_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->protect_len = len;
    self->protect = protect;
    return true;
};

$static fn_((test_heap_VMem__release(
    const heap_VMem_Ctx* ctx, P$raw addr, usize len
))(bool)) {
    let self = ptrCast$((test_heap_VMem_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->release_len = len;
    return true;
};

TEST_fn_("heap/VMem: injected interface dispatches every address-space operation" $scope) {
    var_(state, test_heap_VMem_Ctx) = cleared();
    let_(ctx, heap_VMem_Ctx) = {
        .inner = &state,
        .geom = heap_Geom_default(),
    };
    let_(vtbl, heap_VMem_VTbl) = {
        .reserveFn = test_heap_VMem__reserve,
        .commitFn = test_heap_VMem__commit,
        .decommitFn = test_heap_VMem__decommit,
        .protectFn = test_heap_VMem__protect,
        .releaseFn = test_heap_VMem__release,
    };
    let vmem = heap_VMem_ensureValid((heap_VMem){
        .ctx = &ctx,
        .vtbl = &vtbl,
    });
    let reserve_len = heap_Geom_alignReserveWith(heap_VMem_geom(vmem), 1);
    let commit_len = heap_Geom_alignCommitWith(heap_VMem_geom(vmem), 1);
    let ptr = unwrap_(heap_VMem_reserve(vmem, none$((O$P$raw)), reserve_len));

    try_(TEST_expect(state.hint_was_none));
    try_(TEST_expect(state.reserve_len == reserve_len));
    try_(TEST_expect(heap_VMem_commit(vmem, ptr, commit_len)));
    try_(TEST_expect(heap_VMem_protect(
        vmem, ptr, commit_len, heap_VMem_Protcn_read_write_guard
    )));
    try_(TEST_expect(heap_VMem_decommit(vmem, ptr, commit_len)));
    try_(TEST_expect(heap_VMem_release(vmem, ptr, reserve_len)));
    try_(TEST_expect(state.commit_len == commit_len));
    try_(TEST_expect(state.protect_len == commit_len));
    try_(TEST_expect(state.protect == heap_VMem_Protcn_read_write_guard));
    try_(TEST_expect(state.decommit_len == commit_len));
    try_(TEST_expect(state.release_len == reserve_len));
} $unscoped(TEST_fn);

TEST_fn_("heap/VMem: system interface separates reserve commit protect decommit release" $guard) {
    let vmem = catch_((heap_VMem_system())($ignore, return_ok(try_(TEST_skip()))));
    let geom = heap_VMem_geom(vmem);
    try_(TEST_expect(heap_Geom_isValid(geom)));

    let reserve_len = heap_Geom_alignReserveWith(geom, 1);
    let commit_len = heap_Geom_alignCommitWith(geom, 1);
    let ptr = orelse_((heap_VMem_reserve(vmem, none$((O$P$raw)), reserve_len))(return_err(E_cause$OutOfMemory())));
    defer_(let_ignore = heap_VMem_release(vmem, ptr, reserve_len));

    try_(TEST_expect(heap_VMem_commit(vmem, ptr, commit_len)));
    *ptr = u8_(0xC3);
    try_(TEST_expect(*ptr == u8_(0xC3)));
    try_(TEST_expect(heap_VMem_protect(vmem, ptr, commit_len, heap_VMem_Protcn_read_write)));
    try_(TEST_expect(heap_VMem_decommit(vmem, ptr, commit_len)));
} $unguarded(TEST_fn);
