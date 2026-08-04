#include "dh-main.h"
#include "dh/heap/VMap.h"

T_alias$((test_heap_VMap_Ctx)(struct test_heap_VMap_Ctx {
    var_(byte, u8);
    var_(map_len, usize);
    var_(release_len, usize);
    var_(remap_old_len, usize);
    var_(remap_new_len, usize);
    var_(hint_was_none, bool);
}));

$static fn_((test_heap_VMap__map(
    const heap_VMap_Ctx* ctx, O$P$raw addr_hint, usize len
))(O$P$u8)) {
    let self = ptrCast$((test_heap_VMap_Ctx*)(ensureNonnull(ctx)->inner));
    self->map_len = len;
    self->hint_was_none = isNone(addr_hint);
    return some$((O$P$u8)(&self->byte));
};

$static fn_((test_heap_VMap__release(
    const heap_VMap_Ctx* ctx, P$raw addr, usize len
))(bool)) {
    let self = ptrCast$((test_heap_VMap_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->release_len = len;
    return true;
};

$static fn_((test_heap_VMap__remap(
    const heap_VMap_Ctx* ctx, P$raw addr, usize old_len, usize new_len
))(O$P$u8)) {
    let self = ptrCast$((test_heap_VMap_Ctx*)(ensureNonnull(ctx)->inner));
    claim_assert(addr == &self->byte);
    self->remap_old_len = old_len;
    self->remap_new_len = new_len;
    return some$((O$P$u8)(&self->byte));
};

TEST_fn_("heap/VMap: injected interface dispatches map remap and release" $scope) {
    var_(state, test_heap_VMap_Ctx) = cleared();
    let_(ctx, heap_VMap_Ctx) = {
        .inner = &state,
        .geom = heap_Geom_default(),
    };
    let_(vtbl, heap_VMap_VTbl) = {
        .mapFn = test_heap_VMap__map,
        .releaseFn = test_heap_VMap__release,
        .remapFn = test_heap_VMap__remap,
    };
    let vmap = heap_VMap_ensureValid((heap_VMap){
        .ctx = &ctx,
        .vtbl = &vtbl,
    });
    let map_len = heap_Geom_alignPageWith(heap_VMap_geom(vmap), 1);
    let mapped = unwrap_(heap_VMap_map(vmap, none$((O$P$raw)), map_len));
    let remapped = unwrap_(heap_VMap_remap(vmap, mapped, map_len, map_len * 2));

    try_(TEST_expect(mapped == remapped));
    try_(TEST_expect(state.hint_was_none));
    try_(TEST_expect(state.map_len == map_len));
    try_(TEST_expect(state.remap_old_len == map_len));
    try_(TEST_expect(state.remap_new_len == map_len * 2));
    try_(TEST_expect(heap_VMap_release(vmap, remapped, map_len * 2)));
    try_(TEST_expect(state.release_len == map_len * 2));
} $unscoped(TEST_fn);

TEST_fn_("heap/VMap: system interface maps writable page-backed memory" $guard) {
    let vmap = catch_((heap_VMap_system())($ignore, return_ok(try_(TEST_skip()))));
    let geom = heap_VMap_geom(vmap);
    try_(TEST_expect(heap_Geom_isValid(geom)));

    let len = heap_Geom_alignPageWith(geom, 1);
    let ptr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), len))(return_err(E_cause$OutOfMemory())));
    defer_(let_ignore = heap_VMap_release(vmap, ptr, len));

    *ptr = u8_(0x5A);
    try_(TEST_expect(*ptr == u8_(0x5A)));
} $unguarded(TEST_fn)

TEST_fn_("heap/VMap: remap preserves prefix when supported" $guard) {
    let vmap = catch_((heap_VMap_system())($ignore, return_ok(try_(TEST_skip()))));
    let geom = heap_VMap_geom(vmap);
    let old_len = heap_Geom_alignPageWith(geom, 1);
    let new_len = old_len * 2;
    let ptr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), old_len))(return_err(E_cause$OutOfMemory())));

    *ptr = u8_(0x7B);
    if_some((heap_VMap_remap(vmap, ptr, old_len, new_len))(new_ptr)) {
        defer_(let_ignore = heap_VMap_release(vmap, new_ptr, new_len));
        try_(TEST_expect(*new_ptr == u8_(0x7B)));
    } else_none {
        let_ignore = heap_VMap_release(vmap, ptr, old_len);
        return_ok(try_(TEST_skip()));
    }
} $unguarded(TEST_fn)
