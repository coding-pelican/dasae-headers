#include "core.h"
#include "opt.h"
#include "err_res.h"
#include "mem/Allocator.h"

typedef struct Vec {
    meta_Sli      items;
    usize         cap;
    mem_Allocator allocator;
} Vec;
using_Err$(Vec);

#define using_Vec$(T) \
    decl_Vec$(T);     \
    impl_Vec$(T);
#define decl_Vec$(T) \
    typedef union pp_join($, Vec, T) pp_join($, Vec, T)
#define impl_Vec$(T)                  \
    union pp_join($, Vec, T) {        \
        Vec base[1];                  \
        struct {                      \
            TypeInfo type;            \
            pp_join($, Sli, T) items; \
            usize         cap;        \
            mem_Allocator allocator;  \
        };                            \
    }

// Initialize empty list
extern Vec     Vec_init(TypeInfo type, mem_Allocator allocator);
// Initialize with capacity
extern Err$Vec Vec_initCap(TypeInfo type, mem_Allocator allocator, usize cap) must_check;
// Free resources
extern void    Vec_fini(Vec* self);

// Return owned slice
extern Err$meta_Sli Vec_toOwnedSlice(Vec* self) must_check;
// Create from owned slice
extern Vec          Vec_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice);
// Clone list
extern Err$Vec      Vec_clone(const Vec* self) must_check;

// Ensure total capacity
extern Err$void Vec_ensureTotalCap(Vec* self, usize new_cap) must_check;
extern Err$void Vec_ensureTotalCapPrecise(Vec* self, usize new_cap) must_check;
// Ensure unused capacity
extern Err$void Vec_ensureUnusedCap(Vec* self, usize additional) must_check;

// Modify size
extern Err$void Vec_resize(Vec* self, usize new_len) must_check;
extern void     Vec_shrinkAndFree(Vec* self, usize new_len);
extern void     Vec_shrinkRetainingCap(Vec* self, usize new_len);
extern void     Vec_expandToCap(Vec* self);

// Add elements
extern Err$void     Vec_append(Vec* self, meta_Ptr item) must_check;
extern Err$meta_Ptr Vec_addBackOne(Vec* self) must_check;
extern meta_Ptr     Vec_addBackOneAssumeCap(Vec* self);

extern Err$void     Vec_prepend(Vec* self, meta_Ptr item) must_check;
extern Err$meta_Ptr Vec_addFrontOne(Vec* self) must_check;
extern meta_Ptr     Vec_addFrontOneAssumeCap(Vec* self);

// Insert elements
extern Err$void Vec_insert(Vec* self, usize index, meta_Ptr item) must_check;

// Remove elements
extern void         Vec_pop(Vec* self);       // Remove last element
extern Opt$meta_Ptr Vec_popOrNull(Vec* self); // Remove last element

extern void         Vec_shift(Vec* self);       // Remove first element
extern Opt$meta_Ptr Vec_shiftOrNull(Vec* self); // Remove first element

extern meta_Ptr Vec_removeOrdered(Vec* self, usize index);
extern meta_Ptr Vec_removeSwap(Vec* self, usize index);

// Clear
extern void Vec_clearRetainingCap(Vec* self);
extern void Vec_clearAndFree(Vec* self);
