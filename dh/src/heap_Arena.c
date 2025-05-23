#include "dh/heap/Arena.h"
#include "dh/mem/common.h"
#include "dh/meta/common.h"
#include "dh/opt.h"
#include "dh/sli.h"
#include "dh/debug.h"

// Forward declarations for allocator vtable functions
static fn_(heap_Arena_alloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8);
static fn_(heap_Arena_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len), bool);
static fn_(heap_Arena_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len), Opt$Ptr$u8);
static fn_(heap_Arena_free(anyptr ctx, Sli$u8 buf, u32 buf_align), void);

// Internal helper functions
static fn_(heap_Arena_createNode(heap_Arena* self, usize prev_len, usize minimum_size), Opt$Ptr$ListSgl_Node$usize);

extern fn_(heap_Arena_State_promote(heap_Arena_State* self, mem_Allocator child_allocator), heap_Arena) {
    debug_assert_nonnull(self);
    return (heap_Arena){
        .child_allocator = child_allocator,
        .state           = *self
    };
}

extern fn_(heap_Arena_allocator(heap_Arena* self), mem_Allocator) {
    debug_assert_nonnull(self);
    // VTable for Arena allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc  = heap_Arena_alloc,
        .resize = heap_Arena_resize,
        .remap  = heap_Arena_remap,
        .free   = heap_Arena_free,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt
    };
}

extern fn_(heap_Arena_init(mem_Allocator child_allocator), heap_Arena) {
    return (heap_Arena){
        .child_allocator = child_allocator,

        .state = {
            .buffer_list = type$(ListSgl$usize, ListSgl_init()),
            .end_index   = 0,
        }
    };
}

extern fn_(heap_Arena_fini(heap_Arena self), void) {
    // Free all buffers in the list
    var it = self.state.buffer_list.first;
    while_some(it, node) {
        // Save next pointer before freeing current node
        let next_it   = node->next;
        let alloc_buf = Sli_from$(Sli$u8, as$(u8*, node), *node->data);
        mem_Allocator_rawFree(self.child_allocator, alloc_buf, alignOf(ListSgl_Node$usize));
        it = next_it;
    }
}

extern fn_(heap_Arena_queryCap(const heap_Arena* self), usize) {
    debug_assert_nonnull(self);
    usize size = 0;
    var   it   = self->state.buffer_list.first;
    while_some(it, node) {
        // Add buffer size excluding the node header
        size += *node->data - sizeOf(ListSgl_Node$usize);
        it = node->next;
    }
    return size;
}

extern fn_scope(heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode), bool) {
    debug_assert_nonnull(self);

    // Calculate requested capacity based on mode
    let requested_capacity = eval({
        usize cap = 0;
        match_(mode) {
        pattern_(heap_Arena_ResetMode_free_all, _) {
            cap = 0;
        } break;
        pattern_(heap_Arena_ResetMode_retain_capacity, _) {
            cap = heap_Arena_queryCap(self);
        } break;
        pattern_(heap_Arena_ResetMode_retain_with_limit, limit) {
            cap = prim_min(*limit, heap_Arena_queryCap(self));
        } break;
        };
        eval_return cap;
    });
    if (requested_capacity == 0) {
        // Free all memory and reset state
        heap_Arena_fini(*self);
        self->state = (heap_Arena_State){
            .buffer_list = type$(ListSgl$usize, ListSgl_init()),
            .end_index   = 0
        };
        return_(true);
    }

    // Calculate total size needed including node header
    const usize total_size = requested_capacity + sizeOf(ListSgl_Node$usize);

    // Free all nodes except the last one
    var it               = self->state.buffer_list.first;
    var maybe_first_node = none$(Opt$Ptr$ListSgl_Node$usize);
    while_some(it, node) {
        let next_it = node->next;
        if_none(next_it) {
            maybe_first_node = some$(Opt$Ptr$ListSgl_Node$usize, node);
            break;
        }
        let alloc_buf = Sli_from$(Sli$u8, as$(u8*, node), *node->data);
        mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf(ListSgl_Node$usize));
        it = next_it;
    }

    // Reset end index before resizing buffers
    self->state.end_index = 0;

    if_some(maybe_first_node, first_node) {
        self->state.buffer_list.first = some$(Opt$Ptr$ListSgl_Node$usize, first_node);

        // Perfect size match, no need to resize
        if (*first_node->data == total_size) {
            return_(true);
        }

        // Try to resize the buffer
        let alloc_buf = Sli_from$(Sli$u8, as$(u8*, first_node), *first_node->data);
        if (mem_Allocator_rawResize(self->child_allocator, alloc_buf, alignOf(ListSgl_Node$usize), total_size)) {
            *first_node->data = total_size;
            return_(true);
        }

        // Manual reallocation needed
        let new_ptr = orelse(mem_Allocator_rawAlloc(self->child_allocator, total_size, alignOf(ListSgl_Node$usize)), eval({ return_(false); }));
        // Free old buffer and update state
        mem_Allocator_rawFree(self->child_allocator, alloc_buf, alignOf(ListSgl_Node$usize));
        let new_node                  = as$(ListSgl_Node$usize*, new_ptr);
        *new_node->data               = total_size;
        self->state.buffer_list.first = some$(Opt$Ptr$ListSgl_Node$usize, new_node);
    }

    return_(true);
} unscoped;

/*========== Allocator Interface Implementation =============================*/

static fn_scope(heap_Arena_alloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");

    let self      = (heap_Arena*)ctx;
    let ptr_align = align;

    var cur_node = eval({
        var node = make$(ListSgl_Node$usize*);
        if_some (self->state.buffer_list.first, first_node) {
            node = first_node;
        } else {
            node = orelse(heap_Arena_createNode(self, 0, len + ptr_align), eval({ return_none(); }));
        }
        eval_return node;
    });

    while (true) {
        let cur_buf     = (u8*)cur_node + sizeOf(ListSgl_Node$usize);
        let cur_buf_len = *cur_node->data - sizeOf(ListSgl_Node$usize);

        let addr           = (usize)cur_buf + self->state.end_index;
        let aligned_addr   = mem_alignForward(addr, ptr_align);
        let adjusted_index = self->state.end_index + (aligned_addr - addr);
        let new_end_index  = adjusted_index + len;

        if (new_end_index <= cur_buf_len) {
            self->state.end_index = new_end_index;
            return_some(intToRawptr$(u8*, aligned_addr));
        }

        // Try to resize current buffer
        let bigger_buf_size = sizeof(ListSgl_Node$usize) + new_end_index;
        let cur_alloc_buf   = Sli_from$(Sli$u8, as$(u8*, cur_node), *cur_node->data);

        if (mem_Allocator_rawResize(self->child_allocator, cur_alloc_buf, alignOf(ListSgl_Node$usize), bigger_buf_size)) {
            *cur_node->data = bigger_buf_size;
        } else {
            // Allocate new node if resize fails
            cur_node = orelse(heap_Arena_createNode(self, cur_buf_len, len + ptr_align), eval({ return_none(); }));
        }
    }
} unscoped;

static fn_(heap_Arena_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len), bool) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self = (heap_Arena*)ctx;
    $unused(buf_align);

    // Check if this is the most recent allocation
    if_none(self->state.buffer_list.first) {
        return false;
    }
    let cur_node    = unwrap(self->state.buffer_list.first);
    let cur_buf     = (u8*)cur_node + sizeOf(ListSgl_Node$usize);
    let cur_buf_len = *cur_node->data - sizeOf(ListSgl_Node$usize);

    if ((usize)cur_buf + self->state.end_index != (usize)buf.ptr + buf.len) {
        // Not the most recent allocation, can only shrink
        return new_len <= buf.len;
    }

    if (buf.len >= new_len) {
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

static fn_scope(heap_Arena_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len), Opt$Ptr$u8) {
    if (heap_Arena_resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }
    return_none();
} unscoped;

static fn_(heap_Arena_free(anyptr ctx, Sli$u8 buf, u32 buf_align), void) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self = (heap_Arena*)ctx;
    $unused(buf_align);

    // Only free if it's the most recent allocation
    if_none(self->state.buffer_list.first) {
        return;
    }
    let cur_node = unwrap(self->state.buffer_list.first);
    let cur_buf  = (u8*)cur_node + sizeof(ListSgl_Node$usize);

    if ((usize)cur_buf + self->state.end_index == (usize)buf.ptr + buf.len) {
        self->state.end_index -= buf.len;
    }
}

/*========== Internal Helper Functions =====================================*/

static fn_scope(heap_Arena_createNode(heap_Arena* self, usize prev_len, usize minimum_size), Opt$Ptr$ListSgl_Node$usize) {
    debug_assert_nonnull(self);

    // Calculate new buffer size with exponential growth
    let actual_min_size = minimum_size + (sizeOf(ListSgl_Node$usize) + 16);
    let big_enough_len  = prev_len + actual_min_size;
    let len             = big_enough_len + big_enough_len / 2;

    // Allocate new buffer
    let ptr     = orelse(mem_Allocator_rawAlloc(self->child_allocator, len, alignof(ListSgl_Node$usize)), eval({ return_none(); }));
    let node    = (ListSgl_Node$usize*)ptr;
    *node->data = len;
    ListSgl_prepend(self->state.buffer_list.base, node->base);
    self->state.end_index = 0;

    return_some(node);
} unscoped;
