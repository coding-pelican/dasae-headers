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
    usize         cap;
    mem_Allocator allocator;
} ArrList;
use_Opt$(ArrList);
use_Err$(ArrList);
use_ErrSet$(mem_AllocErr, ArrList);

#define ArrList$(T)      TYPE_UNNAMED__ArrList$(T)
#define use_ArrList$(T)  GEN__use_ArrList$(T)
#define decl_ArrList$(T) GEN__decl_ArrList$(T)
#define impl_ArrList$(T) GEN__impl_ArrList$(T)

/// Initialize empty list
extern ArrList              ArrList_init(TypeInfo type, mem_Allocator allocator);
/// Initialize with capacity
extern mem_AllocErr$ArrList ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap) must_check;
/// Free resources
extern void                 ArrList_fini(ArrList* self);

/// Return owned slice
extern mem_AllocErr$meta_Sli ArrList_toOwnedSlice(ArrList* self) must_check;
/// Create from owned slice
extern ArrList               ArrList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice);
/// Clone list
extern mem_AllocErr$ArrList  ArrList_clone(const ArrList* self) must_check;

/// Ensure total capacity
extern mem_AllocErr$void ArrList_ensureTotalCap(ArrList* self, usize new_cap) must_check;
extern mem_AllocErr$void ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap) must_check;
/// Ensure unused capacity
extern mem_AllocErr$void ArrList_ensureUnusedCap(ArrList* self, usize additional) must_check;

/// Modify size
extern mem_AllocErr$void ArrList_resize(ArrList* self, usize new_len) must_check;
extern void              ArrList_shrinkAndFree(ArrList* self, usize new_len);
extern void              ArrList_shrinkRetainingCap(ArrList* self, usize new_len);
extern void              ArrList_expandToCap(ArrList* self);

/// Add elements
extern mem_AllocErr$void ArrList_append(ArrList* self, meta_Ptr item) must_check;
extern mem_AllocErr$void ArrList_appendSlice(ArrList* self, meta_Sli items) must_check;
extern mem_AllocErr$void ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n) must_check;

extern mem_AllocErr$void ArrList_prepend(ArrList* self, meta_Ptr item) must_check;
extern mem_AllocErr$void ArrList_prependSlice(ArrList* self, meta_Sli items) must_check;
extern mem_AllocErr$void ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n) must_check;

extern mem_AllocErr$meta_Ptr ArrList_addBackOne(ArrList* self) must_check;
extern meta_Ptr              ArrList_addBackOneAssumeCap(ArrList* self);
extern mem_AllocErr$meta_Sli ArrList_addBackManyAsSlice(ArrList* self, usize n) must_check;
extern meta_Sli              ArrList_addBackManyAsSliceAssumeCap(ArrList* self, usize n);

extern mem_AllocErr$meta_Ptr ArrList_addFrontOne(ArrList* self) must_check;
extern meta_Ptr              ArrList_addFrontOneAssumeCap(ArrList* self);
extern mem_AllocErr$meta_Sli ArrList_addFrontManyAsSlice(ArrList* self, usize n) must_check;
extern meta_Sli              ArrList_addFrontManyAsSliceAssumeCap(ArrList* self, usize n);

/// Insert elements
extern mem_AllocErr$void ArrList_insert(ArrList* self, usize index, meta_Ptr item) must_check;
extern mem_AllocErr$void ArrList_insertSlice(ArrList* self, usize index, meta_Sli items) must_check;

/// Remove elements
extern void         ArrList_pop(ArrList* self);
extern Opt$meta_Ptr ArrList_popOrNull(ArrList* self);
extern void         ArrList_shift(ArrList* self);
extern Opt$meta_Ptr ArrList_shiftOrNull(ArrList* self);
extern meta_Ptr     ArrList_removeOrdered(ArrList* self, usize index);
extern meta_Ptr     ArrList_removeSwap(ArrList* self, usize index);

/// Clear
extern void ArrList_clearRetainingCap(ArrList* self);
extern void ArrList_clearAndFree(ArrList* self);

/*========== Implementations ================================================*/

#define TYPE_UNNAMED__ArrList$(T)    \
    union {                          \
        ArrList base[1];             \
        struct {                     \
            TypeInfo type;           \
            Sli$(T) items;           \
            usize         cap;       \
            mem_Allocator allocator; \
        };                           \
    }

#define GEN__use_ArrList$(T) \
    decl_ArrList$(T);        \
    impl_ArrList$(T)

#define GEN__decl_ArrList$(T) \
    typedef union pp_join($, ArrList, T) pp_join($, ArrList, T)

#define GEN__impl_ArrList$(T)         \
    union pp_join($, ArrList, T) {    \
        ArrList base[1];              \
        struct {                      \
            TypeInfo type;            \
            pp_join($, Sli, T) items; \
            usize         cap;        \
            mem_Allocator allocator;  \
        };                            \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_LIST_INCLUDED */
