#ifndef ARR_LIST_INCLUDED
#define ARR_LIST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "opt.h"
#include "err_res.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

typedef struct ArrList {
    meta_Sli      items;
    usize         cap;
    mem_Allocator allocator;
} ArrList;
use_Opt$(ArrList);
use_Err$(ArrList);
use_ErrSet$(mem_AllocErr, ArrList);

#define ArrList$(T)                              comp_type_anon__ArrList$(T)
#define ArrList_asNamed$(T_ArrList, var_anon...) comp_op__ArrList_asNamed$(pp_uniqTok(anon), T_ArrList, var_anon)

#define use_ArrList$(T)  comp_gen__use_ArrList$(T)
#define decl_ArrList$(T) comp_gen__decl_ArrList$(T)
#define impl_ArrList$(T) comp_gen__impl_ArrList$(T)

/*========== Function Prototypes ============================================*/

/// Initialize empty list
extern fn_(ArrList_init(TypeInfo type, mem_Allocator allocator), ArrList);
/// Initialize with capacity
extern fn_(ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap), must_check mem_AllocErr$ArrList);
/// Free resources
extern fn_(ArrList_fini(ArrList* self), void);

/// Return owned slice
extern fn_(ArrList_toOwnedSlice(ArrList* self), must_check mem_AllocErr$meta_Sli);
/// Create from owned slice
extern fn_(ArrList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice), ArrList);
/// Clone list
extern fn_(ArrList_clone(const ArrList* self), must_check mem_AllocErr$ArrList);

/// Ensure total capacity
extern fn_(ArrList_ensureTotalCap(ArrList* self, usize new_cap), must_check mem_AllocErr$void);
extern fn_(ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap), must_check mem_AllocErr$void);
/// Ensure unused capacity
extern fn_(ArrList_ensureUnusedCap(ArrList* self, usize additional), must_check mem_AllocErr$void);

/// Modify size
extern fn_(ArrList_resize(ArrList* self, usize new_len), must_check mem_AllocErr$void);
extern fn_(ArrList_shrinkAndFree(ArrList* self, usize new_len), void);
extern fn_(ArrList_shrinkRetainingCap(ArrList* self, usize new_len), void);
extern fn_(ArrList_expandToCap(ArrList* self), void);

/// Add elements
extern fn_(ArrList_append(ArrList* self, meta_Ptr item), must_check mem_AllocErr$void);
extern fn_(ArrList_appendSlice(ArrList* self, meta_Sli items), must_check mem_AllocErr$void);
extern fn_(ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n), must_check mem_AllocErr$void);

extern fn_(ArrList_prepend(ArrList* self, meta_Ptr item), must_check mem_AllocErr$void);
extern fn_(ArrList_prependSlice(ArrList* self, meta_Sli items), must_check mem_AllocErr$void);
extern fn_(ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n), must_check mem_AllocErr$void);

extern fn_(ArrList_addBackOne(ArrList* self), must_check mem_AllocErr$meta_Ptr);
extern fn_(ArrList_addBackOneAssumeCap(ArrList* self), meta_Ptr);
extern fn_(ArrList_addBackManyAsSlice(ArrList* self, usize n), must_check mem_AllocErr$meta_Sli);
extern fn_(ArrList_addBackManyAsSliceAssumeCap(ArrList* self, usize n), meta_Sli);

extern fn_(ArrList_addFrontOne(ArrList* self), must_check mem_AllocErr$meta_Ptr);
extern fn_(ArrList_addFrontOneAssumeCap(ArrList* self), meta_Ptr);
extern fn_(ArrList_addFrontManyAsSlice(ArrList* self, usize n), must_check mem_AllocErr$meta_Sli);
extern fn_(ArrList_addFrontManyAsSliceAssumeCap(ArrList* self, usize n), meta_Sli);

/// Insert elements
extern fn_(ArrList_insert(ArrList* self, usize index, meta_Ptr item), must_check mem_AllocErr$void);
extern fn_(ArrList_insertSlice(ArrList* self, usize index, meta_Sli items), must_check mem_AllocErr$void);

/// Remove elements
extern fn_(ArrList_pop(ArrList* self), void);
extern fn_(ArrList_popOrNull(ArrList* self), Opt$meta_Ptr);
extern fn_(ArrList_shift(ArrList* self), void);
extern fn_(ArrList_shiftOrNull(ArrList* self), Opt$meta_Ptr);
extern fn_(ArrList_removeOrdered(ArrList* self, usize index), meta_Ptr);
extern fn_(ArrList_removeSwap(ArrList* self, usize index), meta_Ptr);

/// Clear
extern fn_(ArrList_clearRetainingCap(ArrList* self), void);
extern fn_(ArrList_clearAndFree(ArrList* self), void);

/*========== Macros and Definitions =========================================*/

#define comp_type_anon__ArrList$(T)  \
    union {                          \
        ArrList base[1];             \
        struct {                     \
            TypeInfo type;           \
            Sli$(T) items;           \
            usize         cap;       \
            mem_Allocator allocator; \
        };                           \
    }
#define comp_op__ArrList_asNamed$(__anon, T_ArrList, var_anon...) eval({                                 \
    let __anon = &var_anon;                                                                              \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(T_ArrList));                                   \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(T_ArrList));                                 \
    claim_assert_static(validateField(TypeOf(*__anon), base, FieldTypeOf(T_ArrList, base)));             \
    claim_assert_static(validateField(TypeOf(*__anon), type, FieldTypeOf(T_ArrList, type)));             \
    claim_assert_static(fieldAnonTypeCastable(T_ArrList, *__anon, Sli, items));                          \
    claim_assert_static(validateField(TypeOf(*__anon), cap, FieldTypeOf(T_ArrList, cap)));               \
    claim_assert_static(validateField(TypeOf(*__anon), allocator, FieldTypeOf(T_ArrList, allocator)));   \
    claim_assert_static(fieldPadding(TypeOf(*__anon), base) == fieldPadding(T_ArrList, base));           \
    claim_assert_static(fieldPadding(TypeOf(*__anon), type) == fieldPadding(T_ArrList, type));           \
    claim_assert_static(fieldPadding(TypeOf(*__anon), items) == fieldPadding(T_ArrList, items));         \
    claim_assert_static(fieldPadding(TypeOf(*__anon), cap) == fieldPadding(T_ArrList, cap));             \
    claim_assert_static(fieldPadding(TypeOf(*__anon), allocator) == fieldPadding(T_ArrList, allocator)); \
    eval_return rawderef(as$(rawptr$(T_ArrList), __anon));                                               \
})

#define comp_gen__use_ArrList$(T) \
    decl_ArrList$(T);             \
    impl_ArrList$(T)
#define comp_gen__decl_ArrList$(T) \
    typedef union pp_join($, ArrList, T) pp_join($, ArrList, T)
#define comp_gen__impl_ArrList$(T)    \
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
