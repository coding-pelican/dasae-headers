#include "dh-main.h"
#include "dh/heap/VMem.h"

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
