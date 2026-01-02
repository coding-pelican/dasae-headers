#include "dh/heap/Arena.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

T_use$((usize)(ListSgl_Adp));
T_use$((usize)(
    ListSgl_empty,
    ListSgl_prepend,
    ListSgl_Link_data,
    ListSgl_Link_dataMut,
    ListSgl_Adp_init
));
$static fn_((heap_Arena__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Arena__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Arena__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Arena__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
$static fn_((heap_Arena__createLink(heap_Arena* self, usize prev_len, usize minimum_size))(O$P$ListSgl_Link$usize));

/*========== External Definitions ===========================================*/

fn_((heap_Arena_State_default(void))(heap_Arena_State)) {
    return lit$((heap_Arena_State){
        .buf_list = ListSgl_empty$usize(),
        .end_idx = 0,
    });
};

fn_((heap_Arena_State_promote(heap_Arena_State self, mem_Allocator child_allocator))(heap_Arena)) {
    return lit$((heap_Arena){
        .child_allocator = child_allocator,
        .state = self,
    });
};

fn_((heap_Arena_allocator(heap_Arena* self))(mem_Allocator)) {
    // VTable for Arena allocator
    $static const mem_Allocator_VT vt $like_ref = { {
        .alloc = heap_Arena__alloc,
        .resize = heap_Arena__resize,
        .remap = heap_Arena__remap,
        .free = heap_Arena__free,
    } };
    return mem_Allocator_ensureValid((mem_Allocator){
        .ctx = self,
        .vt = vt,
    });
};

fn_((heap_Arena_init(mem_Allocator child_allocator))(heap_Arena)) {
    return heap_Arena_State_promote(heap_Arena_State_default(), child_allocator);
};

fn_((heap_Arena_fini(heap_Arena self))(void)) {
    // Free all buffers in the list
    var it = self.state.buf_list.first;
    while_some(it, link) {
        // Save next pointer before freeing current node
        let next_it = link->next;
        let alloc_buf_len = *ListSgl_Link_data$usize(link);
        let alloc_buf = init$S$((u8)(as$(u8*)(link), alloc_buf_len));
        mem_Allocator_rawFree(self.child_allocator, alloc_buf, alignOf$(ListSgl_Adp$usize));
        it = next_it;
    }
};

fn_((heap_Arena_queryCap(const heap_Arena* self))(usize)) {
    claim_assert_nonnull(self);
    var size = lit_n$(usize)(0);
    var it = self->state.buf_list.first;
    while_some(it, link) {
        // Add buffer size excluding the node header
        size += *ListSgl_Link_data$usize(link) - sizeOf$(ListSgl_Adp$usize);
        it = link->next;
    }
    return size;
};

fn_((heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode))(bool)) {
    claim_assert_nonnull(self);
    // Calculate requested capacity based on mode
    let requested_capacity = expr_(usize $scope)(match_(mode) {
        pattern_((heap_Arena_ResetMode_free_all)($ignore))        $break_(0) $end(pattern);
        pattern_((heap_Arena_ResetMode_retain_capacity)($ignore)) $break_(heap_Arena_queryCap(self)) $end(pattern);
        pattern_((heap_Arena_ResetMode_retain_with_limit)(limit)) $break_(prim_min(*limit, heap_Arena_queryCap(self))) $end(pattern);
    } $end(match)) $unscoped_(expr);
    if (requested_capacity == 0) {
        // Free all memory and reset state
        heap_Arena_fini(*self);
        self->state = heap_Arena_State_default();
        return true;
    }
    // Calculate total size needed including node header
    let total_size = requested_capacity + sizeOf$(ListSgl_Adp$usize);
    // Free all nodes except the last one
    var it = self->state.buf_list.first;
    let maybe_first_link = eval_(O$P$ListSgl_Link$usize $scope)(while_some(it, link) {
        let next_it = link->next;
        if_none(next_it) {
            $break_(some(link));
        }
        let alloc_buf_len = *ListSgl_Link_data$usize(link);
        let alloc_buf = init$S$((u8)(as$(u8*)(link), alloc_buf_len));
        mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf$(ListSgl_Adp$usize));
        it = next_it;
    }) eval_(else)($break_(none())) $unscoped_(eval);

    // Reset end index before resizing buffers
    self->state.end_idx = 0;
    if_some((maybe_first_link)(first_link)) {
        asg_lit((&self->state.buf_list.first)(some(first_link)));
        // Perfect size match, no need to resize
        let alloc_buf_len = *ListSgl_Link_data$usize(first_link);
        if (alloc_buf_len == total_size) {
            return true;
        }
        // Try to resize the buffer
        let alloc_buf = init$S$((u8)(as$(u8*)(first_link), alloc_buf_len));
        if (mem_Allocator_rawResize(self->child_allocator, alloc_buf, alignOf$(ListSgl_Adp$usize), total_size)) {
            *ListSgl_Link_dataMut$usize(first_link) = total_size;
        } else {
            // Manual reallocation needed
            let new_ptr = orelse_((mem_Allocator_rawAlloc(self->child_allocator, total_size, alignOf$(ListSgl_Adp$usize)))(return false));
            // Free old buffer and update state
            mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf$(ListSgl_Adp$usize));
            let new_adp = ptrAlignCast$((ListSgl_Adp$usize*)(new_ptr));
            *new_adp = ListSgl_Adp_init$usize(total_size);
            asg_lit((&self->state.buf_list.first)(some(&new_adp->link)));
        }
    }

    return true;
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Arena__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Arena*)(ctx);
    let ptr_align = mem_log2ToAlign(align);

    // 문제로 예상되는 지점 - Fixed
    var cur_link = expr_($P$(ListSgl_Link$usize) $scope)(if_some((self->state.buf_list.first)(first_link)) {
        $break_(first_link);
    }) expr_(else_none)({
        $break_(orelse_((heap_Arena__createLink(self, 0, len + ptr_align))(
            return_none()
        )));
    }) $unscoped_(expr);
    while (true) {
        let cur_buf_ptr = as$(u8*)(cur_link) + sizeOf$(ListSgl_Adp$usize);
        let cur_buf_len = *ListSgl_Link_data$usize(cur_link) - sizeOf$(ListSgl_Adp$usize);

        let addr = ptrToInt(cur_buf_ptr) + self->state.end_idx;
        let aligned_addr = mem_alignFwd(addr, ptr_align);
        let adjusted_idx = self->state.end_idx + (aligned_addr - addr);
        let new_end_idx = adjusted_idx + len;

        if (new_end_idx <= cur_buf_len) {
            let result = cur_buf_ptr + adjusted_idx;
            self->state.end_idx = new_end_idx;
            return_some(result);
        }

        // Try to resize current buffer
        let bigger_buf_size = sizeOf$(ListSgl_Adp$usize) + new_end_idx;
        let cur_alloc_buf_len = *ListSgl_Link_data$usize(cur_link);
        let cur_alloc_buf = init$S$((u8)(as$(u8*)(cur_link), cur_alloc_buf_len));

        if (mem_Allocator_rawResize(self->child_allocator, cur_alloc_buf, alignOf$(ListSgl_Adp$usize), bigger_buf_size)) {
            *ListSgl_Link_dataMut$usize(cur_link) = bigger_buf_size;
        } else {
            // Allocate new node if resize fails
            cur_link = orelse_((heap_Arena__createLink(self, cur_buf_len, len + ptr_align))(return_none()));
        }
    }
} $unscoped_(fn);

fn_((heap_Arena__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Arena*)(ctx);
    let_ignore = buf_align;

    // Check if this is the most recent allocation
    if_none(self->state.buf_list.first) {
        return false;
    }
    let cur_link = unwrap_(self->state.buf_list.first);
    let cur_buf_ptr = as$(u8*)(cur_link) + sizeOf$(ListSgl_Adp$usize);
    let cur_buf_len = *ListSgl_Link_data$usize(cur_link) - sizeOf$(ListSgl_Adp$usize);

    // Not the most recent allocation, can only shrink
    if (ptrToInt(cur_buf_ptr) + self->state.end_idx != ptrToInt(buf.ptr) + buf.len) {
        return new_len <= buf.len;
    }
    // Shrinking
    if (new_len <= buf.len) {
        self->state.end_idx -= buf.len - new_len;
        return true;
    }
    // Growing within current buffer
    if (new_len - buf.len <= cur_buf_len - self->state.end_idx) {
        self->state.end_idx += new_len - buf.len;
        return true;
    }
    return false;
};

fn_((heap_Arena__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    if (heap_Arena__resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }
    return_none();
} $unscoped_(fn);

fn_((heap_Arena__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Arena*)(ctx);
    let_ignore = buf_align;

    // Only free if it's the most recent allocation
    if_none(self->state.buf_list.first) {
        return;
    }
    let cur_link = unwrap_(self->state.buf_list.first);
    let cur_buf_ptr = as$(u8*)(cur_link) + sizeOf$(ListSgl_Adp$usize);

    if (ptrToInt(cur_buf_ptr) + self->state.end_idx == ptrToInt(buf.ptr) + buf.len) {
        self->state.end_idx -= buf.len;
    }
};

fn_((heap_Arena__createLink(heap_Arena* self, usize prev_len, usize minimum_size))(O$P$ListSgl_Link$usize) $scope) {
    claim_assert_nonnull(self);
    // Calculate new buffer size with exponential growth
    let actual_min_size = minimum_size + (sizeOf$(ListSgl_Adp$usize) + 16);
    let big_enough_len = prev_len + actual_min_size;
    let len = big_enough_len + big_enough_len / 2;

    // Allocate new buffer
    let ptr = orelse_((mem_Allocator_rawAlloc(self->child_allocator, len, alignOf$(ListSgl_Adp$usize)))(return_none()));
    let adp = ptrAlignCast$((ListSgl_Adp$usize*)(ptr));
    *adp = ListSgl_Adp_init$usize(len);
    ListSgl_prepend$usize(&self->state.buf_list, &adp->link);
    self->state.end_idx = 0;

    return_some(&adp->link);
} $unscoped_(fn);
