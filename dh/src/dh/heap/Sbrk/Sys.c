#include "dh/heap/Sbrk.h"
#include "dh/heap/VMem.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_sbrk(u_P$raw ctx, usize n))(usize));
$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_ctx(P$raw self))(heap_Sbrk_Ctx));
$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__claimCommitted(usize* committed_size, usize reserved_size, usize aligned_n))(O$usize));

/*========== External Definitions ===========================================*/

#if plat_is_windows
fn_((heap_Sbrk_Sys_Windows_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Windows*)(ctx));
    let geometry = heap_VMem_geom(self->vmem);
    let aligned_n = heap_Geom_alignCommitWith(geometry, n);
    let old_committed = orelse_((heap_Sbrk_Sys__claimCommitted(
        &self->committed_size,
        self->reserved_size,
        aligned_n
    ))(return 0));

    let commit_addr = self->base_addr + old_committed;
    if (!heap_VMem_commit(self->vmem, intToPtr$((P$raw)(commit_addr)), aligned_n)) { return 0; }
    return commit_addr;
};

fn_((heap_Sbrk_Sys_Windows_ctx(heap_Sbrk_Sys_Windows* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Windows_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Windows_from(heap_VMem vmem, usize reserve_size))(heap_Sbrk_Sys_Windows)) {
    claim_assert(reserve_size > 0);
    vmem = heap_VMem_ensureValid(vmem);
    let aligned_size = heap_Geom_alignReserveWith(heap_VMem_geom(vmem), reserve_size);
    let base = orelse_((heap_VMem_reserve(vmem, none$((O$P$raw)), aligned_size))(null));
    if (base == null) { return l0$((heap_Sbrk_Sys_Windows)); }
    return (heap_Sbrk_Sys_Windows){
        .vmem = vmem,
        .base_addr = ptrToInt(base),
        .reserved_size = aligned_size,
        .committed_size = 0,
    };
};

fn_((heap_Sbrk_Sys_Windows_init(usize reserve_size))(heap_VMem_E$heap_Sbrk_Sys_Windows) $scope) {
    return_ok(heap_Sbrk_Sys_Windows_from(try_(heap_VMem_system()), reserve_size));
} $unscoped(fn);

fn_((heap_Sbrk_Sys_Windows_fini(heap_Sbrk_Sys_Windows* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        let_ignore = heap_VMem_release(self->vmem, intToPtr$((P$raw)(self->base_addr)), self->reserved_size);
        self->base_addr = 0;
        self->committed_size = 0;
    }
    asg_l((self)(cleared()));
};
#endif /* plat_is_windows */

#if plat_is_posix
fn_((heap_Sbrk_Sys_Posix_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Posix*)(ctx));
    let geometry = heap_VMem_geom(self->vmem);
    let aligned_n = heap_Geom_alignCommitWith(geometry, n);
    let old_committed = orelse_((heap_Sbrk_Sys__claimCommitted(
        &self->committed_size,
        self->reserved_size,
        aligned_n
    ))(return 0));

    let commit_addr = self->base_addr + old_committed;
    if (!heap_VMem_commit(self->vmem, intToPtr$((P$raw)(commit_addr)), aligned_n)) { return 0; }
    return commit_addr;
};

fn_((heap_Sbrk_Sys_Posix_ctx(heap_Sbrk_Sys_Posix* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Posix_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Posix_from(heap_VMem vmem, usize reserve_size))(heap_Sbrk_Sys_Posix)) {
    claim_assert(reserve_size > 0);
    vmem = heap_VMem_ensureValid(vmem);
    let aligned_size = heap_Geom_alignReserveWith(heap_VMem_geom(vmem), reserve_size);
    let base = orelse_((heap_VMem_reserve(vmem, none$((O$P$raw)), aligned_size))(null));
    if (base == null) { return l0$((heap_Sbrk_Sys_Posix)); }
    return (heap_Sbrk_Sys_Posix){
        .vmem = vmem,
        .base_addr = ptrToInt(base),
        .reserved_size = aligned_size,
        .committed_size = 0,
    };
};

fn_((heap_Sbrk_Sys_Posix_init(usize reserve_size))(heap_VMem_E$heap_Sbrk_Sys_Posix) $scope) {
    return_ok(heap_Sbrk_Sys_Posix_from(try_(heap_VMem_system()), reserve_size));
} $unscoped(fn);

fn_((heap_Sbrk_Sys_Posix_fini(heap_Sbrk_Sys_Posix* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        let_ignore = heap_VMem_release(self->vmem, intToPtr$((P$raw)(self->base_addr)), self->reserved_size);
        self->base_addr = 0;
        self->committed_size = 0;
    }
    asg_l((self)(cleared()));
};
#endif /* plat_is_posix */

#if plat_is_wasi
fn_((heap_Sbrk_Sys_Wasm_sbrk(u_P$raw ctx, usize n))(usize)) {
    /* TODO: Implement */
    return heap_Sbrk_Sys__unsupported_sbrk(ctx, n);
};

fn_((heap_Sbrk_Sys_Wasm_ctx(heap_Sbrk_Sys_Wasm* self))(heap_Sbrk_Ctx)) {
    /* TODO: Implement */
    return heap_Sbrk_Sys__unsupported_ctx(self);
};

fn_((heap_Sbrk_Sys_Wasm_init(void))(heap_Sbrk_Sys_Wasm)) {
    /* TODO: Implement */
    return (heap_Sbrk_Sys_Wasm){
        .page_count = 0,
    };
};

fn_((heap_Sbrk_Sys_Wasm_fini(heap_Sbrk_Sys_Wasm* self))(void)) {
    asg_l((self)(cleared()));
};
#endif /* plat_is_wasi */

/*========== Internal Definitions ===========================================*/

fn_((heap_Sbrk_Sys__unsupported_sbrk(u_P$raw ctx, usize n))(usize)) {
    let_ignore = ctx;
    let_ignore = n;
    return 0;
};

fn_((heap_Sbrk_Sys__unsupported_ctx(P$raw self))(heap_Sbrk_Ctx)) {
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys__unsupported_sbrk,
    };
};

fn_((heap_Sbrk_Sys__claimCommitted(usize* committed_size, usize reserved_size, usize aligned_n))(O$usize) $scope) {
    if (aligned_n == 0) { return_none(); }
    var old_committed = atom_load(committed_size, atom_MemOrd_seq_cst);
    while (true) {
        if (old_committed > reserved_size || reserved_size - old_committed < aligned_n) {
            return_none();
        }
        let new_committed = old_committed + aligned_n;
        old_committed = orelse_((atom_cmpXchgStrong(
            committed_size,
            old_committed,
            new_committed,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        ))(return_some(old_committed)));
    }
} $unscoped(fn);
