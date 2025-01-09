#ifndef ARR_LIST_INCLUDED
#define ARR_LIST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "core.h"
#include "opt.h"
#include "err_res.h"
#include "mem/Allocator.h"

typedef struct ArrList {
    meta_Sli      items;
    usize         capacity;
    mem_Allocator allocator;
} ArrList;
using_Err$(ArrList);

// Initialize empty list
extern ArrList     ArrList_init(TypeInfo type, mem_Allocator allocator);
// Initialize with capacity
extern Err$ArrList ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap) must_check;
// Free resources
extern void        ArrList_fini(ArrList* self);

// Return owned slice
extern Err$meta_Sli ArrList_toOwnedSlice(ArrList* self) must_check;
// Create from owned slice
extern ArrList      ArrList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice);
// Clone list
extern Err$ArrList  ArrList_clone(const ArrList* self) must_check;

// Ensure total capacity
extern Err$void ArrList_ensureTotalCap(ArrList* self, usize new_cap) must_check;
extern Err$void ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap) must_check;
// Ensure unused capacity
extern Err$void ArrList_ensureUnusedCap(ArrList* self, usize additional) must_check;

// Modify size
extern Err$void ArrList_resize(ArrList* self, usize new_len) must_check;
extern void     ArrList_shrinkAndFree(ArrList* self, usize new_len);
extern void     ArrList_shrinkRetainingCap(ArrList* self, usize new_len);
extern void     ArrList_expandToCap(ArrList* self);

// Add elements
extern Err$void ArrList_append(ArrList* self, meta_Ptr item) must_check;
extern Err$void ArrList_appendSlice(ArrList* self, meta_Sli items) must_check;
extern Err$void ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n) must_check;

extern Err$void ArrList_prepend(ArrList* self, meta_Ptr item) must_check;
extern Err$void ArrList_prependSlice(ArrList* self, meta_Sli items) must_check;
extern Err$void ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n) must_check;

extern Err$meta_Ptr ArrList_addBackOne(ArrList* self) must_check;
extern meta_Ptr     ArrList_addBackOneAssumeCap(ArrList* self);
extern Err$meta_Sli ArrList_addBackManyAsSlice(ArrList* self, usize n) must_check;
extern meta_Sli     ArrList_addBackManyAsSliceAssumeCap(ArrList* self, usize n);

extern Err$meta_Ptr ArrList_addFrontOne(ArrList* self) must_check;
extern meta_Ptr     ArrList_addFrontOneAssumeCap(ArrList* self);
extern Err$meta_Sli ArrList_addFrontManyAsSlice(ArrList* self, usize n) must_check;
extern meta_Sli     ArrList_addFrontManyAsSliceAssumeCap(ArrList* self, usize n);

// Insert elements
extern Err$void ArrList_insert(ArrList* self, usize index, meta_Ptr item) must_check;
extern Err$void ArrList_insertSlice(ArrList* self, usize index, meta_Sli items) must_check;

// Remove elements
extern void         ArrList_pop(ArrList* self);
extern Opt$meta_Ptr ArrList_popOrNull(ArrList* self);
extern void         ArrList_shift(ArrList* self);
extern Opt$meta_Ptr ArrList_shiftOrNull(ArrList* self);
extern meta_Ptr     ArrList_removeOrdered(ArrList* self, usize index);
extern meta_Ptr     ArrList_removeSwap(ArrList* self, usize index);

// Clear
extern void ArrList_clearRetainingCap(ArrList* self);
extern void ArrList_clearAndFree(ArrList* self);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_LIST_INCLUDED */
