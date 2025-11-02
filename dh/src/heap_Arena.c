#include "dh/heap/Arena.h"
#include "dh/mem/common.h"
#include "dh/meta/common.h"
#include "dh/opt.h"
#include "dh/sli.h"
#include "dh/debug.h"

T_use$(usize, (
    ListSgl_Node_init,
    ListSgl_init
));

// Forward declarations for allocator vtable functions
static fn_((heap_Arena_alloc(P$raw ctx, usize len, u32 align))(O$P$u8));
static fn_((heap_Arena_resize(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(bool));
static fn_((heap_Arena_remap(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(O$P$u8));
static fn_((heap_Arena_free(P$raw ctx, S$u8 buf, u32 buf_align))(void));

// Internal helper functions
static fn_((heap_Arena_createNode(heap_Arena* self, usize prev_len, usize minimum_size))(O$P$ListSgl_Node$usize));

extern fn_((heap_Arena_State_promote(heap_Arena_State self, mem_Allocator child_allocator))(heap_Arena)) {
    return (heap_Arena){
        .child_allocator = child_allocator,
        .state           = self
    };
}

extern fn_((heap_Arena_allocator(heap_Arena* self))(mem_Allocator)) {
    debug_assert_nonnull(self);
    // VTable for Arena allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc  = heap_Arena_alloc,
        .resize = heap_Arena_resize,
        .remap  = heap_Arena_remap,
        .free   = heap_Arena_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt  = vt
    };
}

extern fn_((heap_Arena_init(mem_Allocator child_allocator))(heap_Arena)) {
    return heap_Arena_State_promote(
        lit$((heap_Arena_State){ .buffer_list = ListSgl_init$usize(), .end_index = 0 }),
        child_allocator
    );
}

extern fn_((heap_Arena_fini(heap_Arena self))(void)) {
    // Free all buffers in the list
    var it = self.state.buffer_list.first;
    while_some(it, node) {
        // Save next pointer before freeing current node
        let next_it   = node->next;
        let alloc_buf = init$S$((u8)(as$((u8*)(node)), node->data));
        mem_Allocator_rawFree(self.child_allocator, alloc_buf, alignOf$(ListSgl_Node$usize));
        it = next_it;
    }
}

extern fn_((heap_Arena_queryCap(const heap_Arena* self))(usize)) {
    debug_assert_nonnull(self);
    usize size = 0;
    var   it   = self->state.buffer_list.first;
    while_some(it, node) {
        // Add buffer size excluding the node header
        size += node->data - sizeOf$(ListSgl_Node$usize);
        it = node->next;
    }
    return size;
}

extern fn_((heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode))(bool)) {
    debug_assert_nonnull(self);

    // Calculate requested capacity based on mode
    let requested_capacity = expr_(usize $scope) match_(mode, {
        pattern_(heap_Arena_ResetMode_free_all, $break_(0));
        pattern_(heap_Arena_ResetMode_retain_capacity, $break_(heap_Arena_queryCap(self)));
        pattern_(heap_Arena_ResetMode_retain_with_limit, (limit), $break_(prim_min(*limit, heap_Arena_queryCap(self))));
    }) $unscoped_(expr);
    if (requested_capacity == 0) {
        // Free all memory and reset state
        heap_Arena_fini(*self);
        self->state = make$((heap_Arena_State){
            .buffer_list = type$((ListSgl$usize)(ListSgl_init(typeInfo$(ListSgl_Node$usize)))),
            .end_index = 0
        });
        return true;
    }
    // Calculate total size needed including node header
    let total_size       = requested_capacity + sizeOf$(ListSgl_Node$usize);
    // Free all nodes except the last one
    var it               = self->state.buffer_list.first;
    var maybe_first_node = expr_(O$P$ListSgl_Node$usize $scope) while_some(it, node) {
        let next_it = node->next;
        if_none(next_it) {
            $break_(some(node));
        }
        let alloc_buf = init$S$((u8)(as$((u8*)(node)), node->data));
        mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf$(ListSgl_Node$usize));
        it = next_it;
    } $unscoped_(expr);

    // Reset end index before resizing buffers
    self->state.end_index = 0;
    if_some((maybe_first_node)(first_node)) {
        self->state.buffer_list.first = some$((P$ListSgl_Node$usize)(first_node));

        // Perfect size match, no need to resize
        if (first_node->data == total_size) {
            return true;
        }

        // Try to resize the buffer
        let alloc_buf = init$S$((u8)(as$((u8*)(first_node)), first_node->data));
        if (mem_Allocator_rawResize(self->child_allocator, alloc_buf, alignOf$(ListSgl_Node$usize), total_size)) {
            first_node->data = total_size;
            return true;
        }

        // Manual reallocation needed
        let new_ptr = orelse_((mem_Allocator_rawAlloc(self->child_allocator, total_size, alignOf$(ListSgl_Node$usize)))(return false));
        // Free old buffer and update state
        mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf$(ListSgl_Node$usize));
        let new_node = as$((ListSgl_Node$usize*)(new_ptr));
        *new_node    = *ListSgl_Node_init$usize(&lit$((ListSgl_Node$usize){ .data = total_size }));
        asg$O((&self->state.buffer_list.first)(some(new_node)));
    }

    return true;
}

/*========== Allocator Interface Implementation =============================*/

static fn_((heap_Arena_alloc(P$raw ctx, usize len, u32 align))(O$P$u8) $scope) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");

    let self      = as$((heap_Arena*)(ctx));
    let ptr_align = align;

    // 문제로 예상되는 지점 - Fixed
    var cur_node = expr_($P$(ListSgl_Node$usize) $scope) if_some((self->state.buffer_list.first)(first_node)) {
        $break_(first_node);
    } else_none {
        $break_(orelse_((heap_Arena_createNode(self, 0, len + ptr_align))(
            return_none()
        )));
    } $unscoped_(expr);
    while (true) {
        let cur_buf     = as$((u8*)(cur_node)) + sizeOf$(ListSgl_Node$usize);
        let cur_buf_len = cur_node->data - sizeOf$(ListSgl_Node$usize);

        let addr           = ptrToInt(cur_buf) + self->state.end_index;
        let aligned_addr   = mem_alignForward(addr, ptr_align);
        let adjusted_index = self->state.end_index + (aligned_addr - addr);
        let new_end_index  = adjusted_index + len;

        if (new_end_index <= cur_buf_len) {
            self->state.end_index = new_end_index;
            return_some(intToPtr$(u8*, aligned_addr));
        }

        // Try to resize current buffer
        let bigger_buf_size = sizeOf$(ListSgl_Node$usize) + new_end_index;
        let cur_alloc_buf   = init$S$((u8)(as$((u8*)(cur_node)), cur_node->data));

        if (mem_Allocator_rawResize(
                self->child_allocator,
                cur_alloc_buf,
                alignOf$(ListSgl_Node$usize),
                bigger_buf_size
            )) {
            cur_node->data = bigger_buf_size;
        } else {
            // Allocate new node if resize fails
            cur_node = orelse_((heap_Arena_createNode(self, cur_buf_len, len + ptr_align))(
                return_none()
            ));
        }
    }
} $unscoped_(fn);

static fn_((heap_Arena_resize(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(bool)) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self   = as$((heap_Arena*)(ctx));
    let_ignore = buf_align;

    // Check if this is the most recent allocation
    if_none(self->state.buffer_list.first) {
        return false;
    }
    let cur_node    = unwrap_(self->state.buffer_list.first);
    let cur_buf     = as$((u8*)(cur_node)) + sizeOf$(ListSgl_Node$usize);
    let cur_buf_len = cur_node->data - sizeOf$(ListSgl_Node$usize);

    if (ptrToInt(cur_buf) + self->state.end_index != ptrToInt(buf.ptr) + buf.len) {
        // Not the most recent allocation, can only shrink
        return new_len <= buf.len;
    }

    if (new_len <= buf.len) {
        // Shrinking
        self->state.end_index -= buf.len - new_len;
        return true;
    }
    if (new_len - buf.len <= cur_buf_len - self->state.end_index) {
        // Growing within current buffer
        self->state.end_index += new_len - buf.len;
        return true;
    }

    return false;
}

static fn_((heap_Arena_remap(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(O$P$u8) $scope) {
    if (heap_Arena_resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }
    return_none();
} $unscoped_(fn);

static fn_((heap_Arena_free(P$raw ctx, S$u8 buf, u32 buf_align))(void)) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self   = as$((heap_Arena*)(ctx));
    let_ignore = buf_align;

    // Only free if it's the most recent allocation
    if_none(self->state.buffer_list.first) {
        return;
    }
    let cur_node = unwrap_(self->state.buffer_list.first);
    let cur_buf  = as$((u8*)(cur_node)) + sizeOf$(ListSgl_Node$usize);

    if (ptrToInt(cur_buf) + self->state.end_index == ptrToInt(buf.ptr) + buf.len) {
        self->state.end_index -= buf.len;
    }
}

/*========== Internal Helper Functions =====================================*/

static fn_((heap_Arena_createNode(heap_Arena* self, usize prev_len, usize minimum_size))(O$P$ListSgl_Node$usize) $scope) {
    debug_assert_nonnull(self);

    // Calculate new buffer size with exponential growth
    let actual_min_size = minimum_size + (sizeOf$(ListSgl_Node$usize) + 16);
    let big_enough_len  = prev_len + actual_min_size;
    let len             = big_enough_len + (big_enough_len / 2);

    // Allocate new buffer
    let ptr  = orelse_((mem_Allocator_rawAlloc(self->child_allocator, len, alignOf$(ListSgl_Node$usize)))(return_none()));
    let node = as$((ListSgl_Node$usize*)(ptr));
    *node = *ListSgl_Node_init$usize(&lit$((ListSgl_Node$usize){ .data = 0 }));
    ListSgl_prepend(self->state.buffer_list.as_raw, node->as_raw);
    self->state.end_index = 0;

    return_some(node);
} $unscoped_(fn);
