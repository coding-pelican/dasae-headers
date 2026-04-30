#include "dh/heap/Sbrk.h"
#include "dh/heap/vmem.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_sbrk(u_P$raw ctx, usize n))(usize));
$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_ctx(P$raw self))(heap_Sbrk_Ctx));

/*========== External Definitions ===========================================*/

#if plat_is_windows
fn_((heap_Sbrk_Sys_Windows_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Windows*)(ctx));
    let aligned_n = heap_alignPage(n);
    if (self->committed_size + aligned_n > self->reserved_size) {
        return 0; /* Out of reserved space */
    }

    let commit_addr = ptrToInt(self->base_addr) + self->committed_size;
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_addr)), aligned_n)) { return 0; /* Commit failed */ }

    let old_committed = self->committed_size;
    self->committed_size += aligned_n;
    return ptrToInt(self->base_addr) + old_committed;
};

fn_((heap_Sbrk_Sys_Windows_ctx(heap_Sbrk_Sys_Windows* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Windows_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Windows_init(usize reserve_size))(heap_Sbrk_Sys_Windows)) {
    claim_assert(reserve_size > 0);
    let aligned_size = heap_alignPage(reserve_size);
    let base = orelse_((heap_vmem_reserve(null, aligned_size))(null));
    if (base == null) { return l0$((heap_Sbrk_Sys_Windows)); }
    return (heap_Sbrk_Sys_Windows){
        .base_addr = ptrToInt(base),
        .reserved_size = aligned_size,
        .committed_size = 0,
    };
};

fn_((heap_Sbrk_Sys_Windows_fini(heap_Sbrk_Sys_Windows* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        let_ignore = heap_vmem_release(intToPtr$((P$raw)(self->base_addr)), self->reserved_size);
        self->base_addr = 0;
        self->committed_size = 0;
    }
    asg_l((self)(cleared()));
};
#endif /* plat_is_windows */

#if plat_is_posix
fn_((heap_Sbrk_Sys_Posix_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Posix*)(ctx));
    let aligned_n = heap_alignPage(n);
    if (self->committed_size + aligned_n > self->reserved_size) {
        return 0; /* Out of reserved space */
    }

    let commit_addr = self->base_addr + self->committed_size;
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_addr)), aligned_n)) { return 0; /* Commit failed */ }

    let old_committed = self->committed_size;
    self->committed_size += aligned_n;
    return self->base_addr + old_committed;
};

fn_((heap_Sbrk_Sys_Posix_ctx(heap_Sbrk_Sys_Posix* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Posix_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Posix_init(usize reserve_size))(heap_Sbrk_Sys_Posix)) {
    claim_assert(reserve_size > 0);
    let aligned_size = heap_alignPage(reserve_size);
    let base = orelse_((heap_vmem_reserve(null, aligned_size))(null));
    if (base == null) { return l0$((heap_Sbrk_Sys_Posix)); }
    return (heap_Sbrk_Sys_Posix){
        .base_addr = ptrToInt(base),
        .reserved_size = aligned_size,
        .committed_size = 0,
    };
}

fn_((heap_Sbrk_Sys_Posix_fini(heap_Sbrk_Sys_Posix* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        let_ignore = heap_vmem_release(intToPtr$((P$raw)(self->base_addr)), self->reserved_size);
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
