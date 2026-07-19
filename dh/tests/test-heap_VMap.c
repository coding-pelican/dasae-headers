#include "dh-main.h"
#include "dh/heap/VMap.h"

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
