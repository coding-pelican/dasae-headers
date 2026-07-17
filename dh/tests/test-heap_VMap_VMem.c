#include "dh-main.h"
#include "dh/heap/VMap.h"
#include "dh/heap/VMem.h"

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
} $unguarded(TEST_fn)
