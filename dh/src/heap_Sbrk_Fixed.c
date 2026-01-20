#include "dh/heap/Sbrk.h"
#include "dh/heap/Fixed.h"

fn_((heap_Sbrk_Fixed_ctx(heap_Sbrk_Fixed* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Fixed_sbrk,
    };
};

fn_((heap_Sbrk_Fixed_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Fixed*)(ctx));
    // Align n to page size
    let aligned_n = mem_alignFwd(n, heap_page_size);
    // Check if we have enough space
    if (self->end_idx + aligned_n > self->buf.len) {
        return 0; // Out of memory
    }
    let addr = ptrToInt(self->buf.ptr) + self->end_idx;
    self->end_idx += aligned_n;
    return addr;
};
