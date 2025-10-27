/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrList.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-03-23 (date of last update)
 * @version v0.1-alpha.5
 * @ingroup dasae-headers(dh)
 * @prefix  ArrList
 *
 * @brief   Dynamic array list implementation
 * @details This header provides an array list implementation with dynamic memory allocation.
 *          The array list automatically resizes as elements are added or removed.
 *          Supports operations for initialization, modification, and element access.
 */

/*========== Cheat Sheet ====================================================*/

#ifndef ArrList__included
#define ArrList__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrList Anonymous */
#define ArrList$$(T) \
    /** \
     * @brief Create a anonymous array list structure \
     * @param T Type of elements to store in the list \
     * @return Anonymous array list type token \
     * @details Creates an anonymous union with both base ArrList and typed fields \
     * @example \
     *     ArrList$$(i32) anon; // Create anonymous array list of i32 elements \
     */ \
    comp_type_anon__ArrList$$(T)

/* ArrList Alias */
#define ArrList$(T) \
    /** \
     * @brief Create an array list type \
     * @param T Type of elements to store in the list \
     * @return Array list type alias \
     * @example \
     *     ArrList$(i32) list; // Create an array list of i32 elements \
     */ \
    comp_type_alias__ArrList$(T)

/* ArrList Template */
#define T_decl_ArrList$(T) \
    /** \
     * @brief Declare typed array list structure \
     * @param T Type of elements to store in the list \
     * @example \
     *     T_decl_ArrList$(i32); // Declare i32 array list structure \
     */ \
    comp_type_gen__decl_ArrList$(T)
#define T_impl_ArrList$(T) \
    /** \
     * @brief Implement typed array list structure \
     * @param T Type of elements to store in the list \
     * @example \
     *     T_impl_ArrList$(i32); // Implement previously declared structure \
     */ \
    comp_type_gen__impl_ArrList$(T)
#define T_use_ArrList$(T) \
    /** \
     * @brief Declare and implement typed array list \
     * @param T Type of elements to store in the list \
     * @example \
     *     T_use_ArrList$(i32); // Declare and implement i32 array list \
     */ \
    comp_type_gen__use_ArrList$(T)

/* ArrList Raw Structure */
typedef struct ArrList {
    union {
        struct {
            var_(type, TypeInfo);
            var_(items, S$raw);
        };
        var_(items_meta, meta_S$raw);
    };
    var_(cap, usize);
} ArrList;
T_use$(ArrList, (O, E));
T_use_E$($set(mem_Err)(ArrList));

/*========== Function Prototypes ============================================*/

extern fn_((ArrList_empty(TypeInfo type))(ArrList));
extern fn_((ArrList_fromBuf(TypeInfo type, S$raw buf))(ArrList));
extern fn_((ArrList_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrList)) $must_check;
extern fn_((ArrList_fini(ArrList* self, mem_Allocator gpa))(void));
extern fn_((ArrList_clone(ArrList self, mem_Allocator gpa))(mem_Err$ArrList)) $must_check;

extern fn_((ArrList_ensureCap(ArrList* self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrList_ensureCapPrecise(ArrList* self, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrList_ensureUnusedCap(ArrList* self, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrList_expandToCap(ArrList* self))(void));
extern fn_((ArrList_resize(ArrList* self, mem_Allocator gpa, usize new_len))(mem_Err$void)) $must_check;
extern fn_((ArrList_shrinkRetainingCap(ArrList* self, usize new_len))(void));
extern fn_((ArrList_shrinkAndFree(ArrList* self, mem_Allocator gpa, usize new_len))(void));
extern fn_((ArrList_clearRetainingCap(ArrList* self))(void));
extern fn_((ArrList_clearAndFree(ArrList* self, mem_Allocator gpa))(void));

extern fn_((ArrList_addBack(ArrList* self, mem_Allocator gpa))(mem_Err$meta_P$raw)) $must_check;
extern fn_((ArrList_addBackFixed(ArrList* self))(mem_Err$meta_P$raw)) $must_check;
extern fn_((ArrList_addBackWithin(ArrList* self))(meta_P$raw));
extern fn_((ArrList_addBackN(ArrList* self, mem_Allocator gpa, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addBackFixedN(ArrList* self, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addBackWithinN(ArrList* self, usize n))(meta_S$raw));

extern fn_((ArrList_addAtN(ArrList* self, mem_Allocator gpa, usize idx, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addAtFixedN(ArrList* self, usize idx, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addAtWithinN(ArrList* self, usize idx, usize n))(meta_S$raw));

extern fn_((ArrList_addFront(ArrList* self, mem_Allocator gpa))(mem_Err$meta_P$raw)) $must_check;
extern fn_((ArrList_addFrontFixed(ArrList* self))(mem_Err$meta_P$raw)) $must_check;
extern fn_((ArrList_addFrontWithin(ArrList* self))(meta_P$raw));
extern fn_((ArrList_addFrontN(ArrList* self, mem_Allocator gpa, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addFrontFixedN(ArrList* self, usize n))(mem_Err$meta_S$raw)) $must_check;
extern fn_((ArrList_addFrontWithinN(ArrList* self, usize n))(meta_S$raw));

extern fn_((ArrList_append(ArrList* self, mem_Allocator gpa, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixed(ArrList* self, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithin(ArrList* self, meta_V$raw item))(void));
extern fn_((ArrList_appendS(ArrList* self, mem_Allocator gpa, meta_S$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixedS(ArrList* self, meta_S$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithinS(ArrList* self, meta_S$raw items))(void));
extern fn_((ArrList_appendN(ArrList* self, mem_Allocator gpa, meta_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendFixedN(ArrList* self, meta_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_appendWithinN(ArrList* self, meta_V$raw item, usize n))(void));

extern fn_((ArrList_insert(ArrList* self, mem_Allocator gpa, usize idx, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertFixed(ArrList* self, usize idx, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_insertWithin(ArrList* self, usize idx, meta_V$raw item))(void));

extern fn_((ArrList_prepend(ArrList* self, mem_Allocator gpa, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixed(ArrList* self, meta_V$raw item))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithin(ArrList* self, meta_V$raw item))(void));
extern fn_((ArrList_prependS(ArrList* self, mem_Allocator gpa, meta_S$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixedS(ArrList* self, meta_S$raw items))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithinS(ArrList* self, meta_S$raw items))(void));
extern fn_((ArrList_prependN(ArrList* self, mem_Allocator gpa, meta_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependFixedN(ArrList* self, meta_V$raw item, usize n))(mem_Err$void)) $must_check;
extern fn_((ArrList_prependWithinN(ArrList* self, meta_V$raw item, usize n))(void));

extern fn_((ArrList_pop(ArrList* self, meta_V$raw ret_mem))(O$meta_V$raw));
extern fn_((ArrList_removeOrd(ArrList* self, usize idx, meta_V$raw ret_mem))(meta_V$raw));
extern fn_((ArrList_removeSwap(ArrList* self, usize idx, meta_V$raw ret_mem))(meta_V$raw));
extern fn_((ArrList_shift(ArrList* self, meta_V$raw ret_mem))(O$meta_V$raw));

extern fn_((ArrList_cappedItems(ArrList self))(meta_S$raw));
extern fn_((ArrList_unusedItems(ArrList self))(meta_S$raw));

/*========== Macros and Definitions =========================================*/

#define comp_type_alias__ArrList$(T) \
    pp_join($, ArrList, T)


#define comp_type_gen__decl_ArrList$(T) \
    $maybe_unused typedef union ArrList$(T) ArrList$(T)
#define comp_type_gen__impl_ArrList$(T) \
    union ArrList$(T) { \
        struct { \
            union { \
                struct { \
                    var_(type, TypeInfo); \
                    var_(items, S$(T)); \
                }; \
                var_(items_meta, meta_S$raw); \
            }; \
            var_(cap, usize); \
        }; \
        var_(as_raw, ArrList); \
        var_(ref_raw, TypeOf(ArrList $like_ptr)); \
    }
#define comp_type_gen__use_ArrList$(T) \
    T_decl_ArrList$(T); \
    T_impl_ArrList$(T)


#define comp_type_anon__ArrList$$(T) \
    union { \
        struct { \
            union { \
                struct { \
                    var_(type, TypeInfo); \
                    var_(items, S$$(T)); \
                }; \
                var_(items_meta, meta_S$raw); \
            }; \
            var_(cap, usize); \
        }; \
        var_(as_raw, ArrList); \
        var_(ref_raw, TypeOf(ArrList $like_ptr)); \
    }
#define comp_op__ArrList_anonCast$(__anon, T_ArrList, var_anon...) blk({ \
    let __anon = &var_anon; \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(T_ArrList)); \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(T_ArrList)); \
    claim_assert_static(validateField(TypeOf(*__anon), base, FieldTypeOf(T_ArrList, base))); \
    claim_assert_static(validateField(TypeOf(*__anon), type, FieldTypeOf(T_ArrList, type))); \
    claim_assert_static(fieldAnonTypeCastable(T_ArrList, *__anon, Sli, items)); \
    claim_assert_static(validateField(TypeOf(*__anon), cap, FieldTypeOf(T_ArrList, cap))); \
    claim_assert_static(validateField(TypeOf(*__anon), gpa, FieldTypeOf(T_ArrList, gpa))); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), base) == fieldPadding(T_ArrList, base)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), type) == fieldPadding(T_ArrList, type)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), items) == fieldPadding(T_ArrList, items)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), cap) == fieldPadding(T_ArrList, cap)); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), gpa) == fieldPadding(T_ArrList, gpa)); \
    blk_return rawderef(as$(($P$(T_ArrList))(__anon))); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrList__included */
