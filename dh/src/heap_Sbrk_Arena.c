#include "dh/heap/Sbrk.h"
#include "dh/heap/Arena.h"

fn_((heap_Sbrk_Arena_ctx(heap_Sbrk_Arena* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Arena_sbrk,
    };
};

fn_((heap_Sbrk_Arena_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Arena*)(ctx));
    let aligned_n = mem_alignFwd(n, heap_page_size);
    return ptrToInt(orelse_((mem_Allocator_rawAlloc(
        heap_Arena_allocator(self),
        aligned_n,
        mem_alignToLog2(heap_page_size)
    ))(intToPtr$((P$u8)(0)))));
};
