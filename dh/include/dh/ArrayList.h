#ifndef ARRAY_LIST_INCLUDED
#define ARRAY_LIST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "core.h"
#include "opt.h"
#include "err_res.h"
#include "mem/Allocator.h"

typedef struct ArrayList {
    meta_Sli      items;
    usize         capacity;
    mem_Allocator allocator;
} ArrayList;
using_Err$(ArrayList);

// Initialize empty list
extern ArrayList ArrayList_init(TypeInfo type, mem_Allocator allocator);

// Initialize with capacity
extern Err$ArrayList ArrayList_initCapacity(TypeInfo type, mem_Allocator allocator, usize cap) must_check;

// Free resources
extern void ArrayList_fini(ArrayList* self);

// Return owned slice
extern Err$meta_Sli ArrayList_toOwnedSlice(ArrayList* self) must_check;

// Create from owned slice
extern ArrayList ArrayList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice);

// Clone list
extern Err$ArrayList ArrayList_clone(const ArrayList* self) must_check;

// Ensure total capacity
extern Err$void ArrayList_ensureTotalCapacity(ArrayList* self, usize new_cap) must_check;
extern Err$void ArrayList_ensureTotalCapacityPrecise(ArrayList* self, usize new_cap) must_check;

// Ensure unused capacity
extern Err$void ArrayList_ensureUnusedCapacity(ArrayList* self, usize additional) must_check;

// Modify size
extern Err$void ArrayList_resize(ArrayList* self, usize new_len) must_check;
extern void     ArrayList_shrinkAndFree(ArrayList* self, usize new_len);
extern void     ArrayList_shrinkRetainingCapacity(ArrayList* self, usize new_len);
extern void     ArrayList_expandToCapacity(ArrayList* self);

// Add elements
extern Err$void ArrayList_append(ArrayList* self, meta_Ptr item) must_check;
extern Err$void ArrayList_appendSlice(ArrayList* self, meta_Sli items) must_check;
extern Err$void ArrayList_appendNTimes(ArrayList* self, meta_Ptr value, usize n) must_check;

extern Err$meta_Ptr ArrayList_addOne(ArrayList* self) must_check;
extern meta_Ptr     ArrayList_addOneAssumeCapacity(ArrayList* self) must_check;

extern Err$meta_Sli ArrayList_addManyAsSlice(ArrayList* self, usize n) must_check;
extern meta_Sli     ArrayList_addManyAsSliceAssumeCapacity(ArrayList* self, usize n) must_check;

// Insert elements
extern Err$void ArrayList_insert(ArrayList* self, usize index, meta_Ptr item) must_check;
extern Err$void ArrayList_insertSlice(ArrayList* self, usize index, meta_Sli items) must_check;

// Remove elements
extern void         ArrayList_pop(ArrayList* self);
extern Opt$meta_Ptr ArrayList_popOrNull(ArrayList* self);
extern meta_Ptr     ArrayList_orderedRemove(ArrayList* self, usize index);
extern meta_Ptr     ArrayList_swapRemove(ArrayList* self, usize index);

// Clear
extern void ArrayList_clearRetainingCapacity(ArrayList* self);
extern void ArrayList_clearAndFree(ArrayList* self);

// Access
extern usize         ArrayList_len(const ArrayList* self);
extern usize         ArrayList_capacity(const ArrayList* self);
extern meta_SliConst ArrayList_items(const ArrayList* self);
extern meta_Sli      ArrayList_itemsMut(ArrayList* self);
extern meta_PtrConst ArrayList_at(const ArrayList* self, usize index);
extern meta_Ptr      ArrayList_atMut(ArrayList* self, usize index);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARRAY_LIST_INCLUDED */
