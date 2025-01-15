#include "dh/claim/unreachable.h"
#include "dh/heap/Arena.h"
#include "dh/math/common.h"

// static Opt$Ptr$u8 heap_Arena_alloc(anyptr ctx, usize len, usize ptr_align);
// static bool       heap_Arena_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_size);
// static void       heap_Arena_free(anyptr ctx, Sli$u8 buf, usize buf_align);

// mem_Allocator heap_Arena_allocator(heap_Arena* self) {
//     static const mem_AllocatorVT vt[1] = { {
//         .alloc  = heap_Arena_alloc,
//         .resize = heap_Arena_resize,
//         .free   = heap_Arena_free,
//     } };
//     return (mem_Allocator){
//         .ctx = self,
//         .vt  = vt,
//     };
// }
// /*========== Lifecycle Implementation =======================================*/

// heap_Arena heap_Arena_init(mem_Allocator child) {
//     return (heap_Arena){
//         .child = child,
//         .state = (heap_Arena_State){
//             .first     = null,
//             .end_index = 0,
//         },
//     };
// }

// void heap_Arena_fini(heap_Arena* self) {
//     debug_assert_nonnull(self);

//     // Free all buffers
//     var it = self->state.first;
//     while (it != null) {
//         var next = it->next;
// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//         mem_Allocator_rawFree(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)it, .len = it->len },
//             sizeof(heap_Arena_BufNode)
//         );
// #else
//         mem_Allocator_rawFree_debug(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)it, .len = it->len },
//             sizeof(heap_Arena_BufNode),
//             __FILE__,
//             __LINE__,
//             __func__
//         );
// #endif
//         it = next;
//     }
// }

// /*========== Public Interface Implementation ================================*/

// usize heap_Arena_queryCap(const heap_Arena* self) {
//     debug_assert_nonnull(self);
//     usize total = 0;

//     var it = self->state.first;
//     while (it != null) {
//         total += it->len - sizeof(heap_Arena_BufNode);
//         it = it->next;
//     }

//     return total;
// }

// bool heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode) {
//     debug_assert_nonnull(self);

//     // Calculate requested capacity
//     let requested_capacity = eval(
//         usize _switch_mode = 0;
//         switch (mode.tag) {
//             case heap_Arena_ResetMode_free_all: {
//                 _switch_mode = 0;
//             } break;
//             case heap_Arena_ResetMode_retain_cap: {
//                 _switch_mode = heap_Arena_queryCap(self);
//             } break;
//             case heap_Arena_ResetMode_retain_limit: {
//                 const usize current = heap_Arena_queryCap(self);
//                 _switch_mode        = math_min(mode.retain_limit.limit, current);
//             } break;
//             default:
//                 claim_unreachable;
//         };
//         _switch_mode;
//     );

//     if (requested_capacity == 0) {
//         heap_Arena_fini(self);
//         self->state = (heap_Arena_State){
//             .first     = null,
//             .end_index = 0,
//         };
//         return true;
//     }

//     const usize total_size = requested_capacity + sizeof(heap_Arena_BufNode);
//     const usize align_bits = alignOf(heap_Arena_BufNode);

//     // Free all but the last node
//     heap_Arena_BufNode* it    = self->state.first;
//     heap_Arena_BufNode* first = null;

//     while (it != null) {
//         heap_Arena_BufNode* next = it->next;
//         if (next == null) {
//             first = it;
//             break;
//         }
// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//         mem_Allocator_rawFree(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)it, .len = it->len },
//             align_bits
//         );
// #else
//         mem_Allocator_rawFree_debug(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)it, .len = it->len },
//             align_bits,
//             __FILE__,
//             __LINE__,
//             __func__
//         );
// #endif
//         it = next;
//     }

//     // Reset state
//     self->state.end_index = 0;

//     if (first != null) {
//         self->state.first = first;
//         if (first->len == total_size) {
//             return true; // Perfect size
//         }

//         // Try to resize the buffer
// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//         if (mem_Allocator_rawResize(
//                 self->child,
//                 (Sli$u8){ .ptr = (u8*)first, .len = first->len },
//                 align_bits,
//                 total_size
//             )) {
// #else
//         if (mem_Allocator_rawResize(
//                 self->child,
//                 (Sli$u8){ .ptr = (u8*)first, .len = first->len },
//                 align_bits,
//                 total_size,
//                 __FILE__,
//                 __LINE__,
//                 __func__
//             )) {
// #endif
//             first->len = total_size;
//             return true;
//         }

// // Manual realloc
// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//         let new_node = typed(heap_Arena_BufNode*, orelse_default(mem_Allocator_rawAlloc(self->child, total_size, align_bits), null));
// #else
//         let new_node = typed(heap_Arena_BufNode*, orelse_default(mem_Allocator_rawAlloc(self->child, total_size, align_bits, __FILE__, __LINE__, __func__), null));
// #endif
//         if (new_node == null) {
//             return false; // Failed to preheat
//         }

// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//         mem_Allocator_rawFree(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)first, .len = first->len },
//             align_bits
//         );
// #else
//         mem_Allocator_rawFree_debug(
//             self->child,
//             (Sli$u8){ .ptr = (u8*)first, .len = first->len },
//             align_bits,
//             __FILE__,
//             __LINE__,
//             __func__
//         );
// #endif

//         new_node->next    = null;
//         new_node->len     = total_size;
//         self->state.first = new_node;
//     }

//     return true;
// }

/*========== State Management Functions =====================================*/

// heap_Arena heap_Arena_State_promote(heap_Arena_State* self, mem_Allocator child) {
//     debug_assert_nonnull(self);
//     return (heap_Arena){
//         .child = child,
//         .state = *self,
//     };
// }

// static Opt$Ptr$heap_Arena_BufNode heap_Arena_createNode(
//     heap_Arena* self,
//     usize       prev_len,
//     usize       minimum_size
// ) {
//     reserveReturn(Opt$Ptr$heap_Arena_BufNode);
//     debug_assert_nonnull(self);

//     // Calculate buffer sizes following Zig's pattern
//     const usize actual_min_size = minimum_size + (sizeof(heap_Arena_BufNode) + 16);
//     const usize big_enough_len  = prev_len + actual_min_size;
//     const usize len             = big_enough_len + big_enough_len / 2;

// // Allocate new buffer
// #if !COMP_TIME || (COMP_TIME && !DEBUG_ENABLED) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
//     let new_node = typed(heap_Arena_BufNode*, orelse_default(mem_Allocator_rawAlloc(self->child, len, alignOf(heap_Arena_BufNode)), null));
// #else
//     let new_node = typed(heap_Arena_BufNode*, orelse_default(mem_Allocator_rawAlloc(self->child, len, alignOf(heap_Arena_BufNode), __FILE__, __LINE__, __func__), null));
// #endif

//     // Initialize node
//     new_node->len         = len;
//     new_node->next        = self->state.first;
//     self->state.first     = new_node;
//     self->state.end_index = 0;

//     return_some(new_node);
// }

// /*========== Allocator Interface Implementation =============================*/

// static Opt$Ptr$u8 heap_Arena_alloc(anyptr ctx, usize len, usize ptr_align) {
//     reserveReturn(Opt$Ptr$u8);
//     debug_assert_nonnull(ctx);
//     let self = (heap_Arena*)ctx;

//     // Handle alignment to the buffer node's alignment
//     const ptr_alignment = (usize)1 << ptr_align;

//     // Get current buffer or create new one
//     heap_Arena_BufNode* cur_node = self->state.first;
//     if (cur_node == null) {
//         // Create initial buffer
//         const usize actual_min_size = len + (sizeof(heap_Arena_BufNode) + 16);
//         const usize big_enough_len  = actual_min_size;
//         const usize total_len       = big_enough_len + big_enough_len / 2;

//         let buf = try_none(meta_castPtr$(heap_Arena_BufNode*, self->child.alloc(self->child.ctx, total_len, log2a(alignof(heap_Arena_BufNode)))));

//         buf->next             = null;
//         buf->len              = total_len;
//         self->state.first     = buf;
//         self->state.end_index = 0;
//         cur_node              = buf;
//     }

//     while (true) {
//         // Get current buffer info
//         const u8*   cur_buf  = (u8*)cur_node + sizeof(heap_Arena_BufNode);
//         const usize buf_size = cur_node->len - sizeof(heap_Arena_BufNode);

//         // Try to allocate from current position
//         const usize addr           = (usize)cur_buf + self->state.end_index;
//         const usize adjusted_addr  = mem_alignForward(addr, ptr_alignment);
//         const usize adjusted_index = self->state.end_index + (adjusted_addr - addr);
//         const usize new_end_index  = adjusted_index + len;

//         if (new_end_index <= buf_size) {
//             // We have space! Return the allocation
//             self->state.end_index = new_end_index;
//             return_some(cur_buf + adjusted_index);
//         }

//         // Try to resize current buffer
//         const usize bigger_buf_size = sizeof(heap_Arena_BufNode) + new_end_index;
//         const usize log2_align      = log2a(alignof(heap_Arena_BufNode));

//         if (self->child.resize(self->child.ctx, (Sli$u8){ .ptr = (u8*)cur_node, .len = cur_node->len }, log2_align, bigger_buf_size)) {
//             cur_node->len = bigger_buf_size;
//             continue;
//         }

//         // Need new buffer
//         const usize new_min_size    = len + ptr_alignment;
//         const usize new_buffer_size = buf_size + new_min_size + (buf_size + new_min_size) / 2;

//         let new_node = try_none(meta_castPtr$(heap_Arena_BufNode*, self->child.alloc(self->child.ctx, new_buffer_size, log2a(alignof(heap_Arena_BufNode)))));

//         new_node->next        = self->state.first;
//         new_node->len         = new_buffer_size;
//         self->state.first     = new_node;
//         self->state.end_index = 0;
//         cur_node              = new_node;
//     }
// }

// static bool heap_Arena_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_size) {
//     unused(buf_align);
//     debug_assert_nonnull(ctx);
//     let self = (heap_Arena*)ctx;

//     if (self->state.first == null) {
//         return false;
//     }

//     // Get current buffer info
//     const u8*   cur_buf  = (u8*)self->state.first + sizeof(heap_Arena_BufNode);
//     const usize buf_size = self->state.first->len - sizeof(heap_Arena_BufNode);

//     // Only most recent allocation can be resized
//     if ((usize)cur_buf + self->state.end_index != (usize)buf.ptr + buf.len) {
//         return new_size <= buf.len; // Can only shrink
//     }

//     if (buf.len >= new_size) {
//         // Shrinking
//         self->state.end_index -= buf.len - new_size;
//         return true;
//     }
//     if (buf_size - self->state.end_index >= new_size - buf.len) {
//         // Growing within buffer
//         self->state.end_index += new_size - buf.len;
//         return true;
//     }

//     return false;
// }

// static void heap_Arena_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
//     unused(buf_align);
//     debug_assert_nonnull(ctx);
//     let self = (heap_Arena*)ctx;

//     if (self->state.first == null) {
//         return;
//     }

//     // Only track most recent allocation
//     const u8* cur_buf = (u8*)self->state.first + sizeof(heap_Arena_BufNode);
//     if ((usize)cur_buf + self->state.end_index == (usize)buf.ptr + buf.len) {
//         self->state.end_index -= buf.len;
//     }
// }
