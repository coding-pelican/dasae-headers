/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    ArrList.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
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

#if CHEAT_SHEET
/* Type Declarations */
ArrList     list        = ArrList_init(typeInfo$(i32), heap_allocator);                     // Initialize empty list
ArrList$i32 list        = type$(ArrList$i32, ArrList_init(typeInfo$(i32), heap_allocator)); // Typed list
Err$ArrList initWithCap = ArrList_initCap(typeInfo$(f32), heap_allocator, 10);              // With capacity

/* Operations */
ArrList_append(list.base, ptr_to_item);        // Add item to end
ArrList_prepend(list.base, ptr_to_item);       // Add item to front
meta_Ptr item = ArrList_addBackOne(list.base); // Add and get pointer to n`ew item
ArrList_resize(list.base, 5);                  // Resize to specific length
ArrList_clearRetainingCap(list.base);          // Clear but keep capacity
ArrList_fini(list.base);                       // Free resources

/* API Reference List =======================================================*/

/* Type Definitions */
/// Base dynamic array list structure
typedef struct ArrList {
    meta_Sli      items;     ///< Slice containing the elements with meta
    usize         cap;       ///< Current capacity of the list
    mem_Allocator allocator; ///< Memory allocator to use
} ArrList;
/// Typed dynamic array list structure
typedef union ArrList$T {
    ArrList base[1]; ///< Base array list structure
    struct {
        TypeInfo      type;      ///< Type information for the elements
        Sli$T         items;     ///< Slice containing the elements
        usize         cap;       ///< Current capacity of the list
        mem_Allocator allocator; ///< Memory allocator to use
    };
} ArrList$T;

/* Type Generations */
/// Declare and implement typed array list
#define use_ArrList$(T)  ...
/// Declare typed array list
#define decl_ArrList$(T) ...
/// Implement typed array list
#define impl_ArrList$(T) ...

/* Type Alias and Anonymous Types */
/// Create array list type
#define ArrList$(T)                               ...
/// Create anonymous array list
#define ArrList$$(T)                              ...
/// Convert anonymous array list to specific type
#define ArrList_anonCast$(T_ArrList, var_anon...) ...

/* Initialization/Finalization */
/// Initialize an array list
fn_(ArrList_init(TypeInfo type, mem_Allocator allocator), ArrList);
/// Initialize an array list with pre-allocated capacity
fn_(ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap), mem_Allocator_Err$ArrList);
/// Finalize an array list
fn_(ArrList_fini(ArrList* self), void);

/* Conversion */
/// Convert array list to owned slice
fn_(ArrList_toOwnedSli(ArrList* self), mem_Allocator_Err$meta_Sli);
/// Create array list from owned slice
fn_(ArrList_fromOwnedSli(mem_Allocator allocator, meta_Sli slice), ArrList);
/// Clone an array list
fn_(ArrList_clone(const ArrList* self), mem_Allocator_Err$ArrList);

/* Capacity Management */
/// Ensure total capacity of the array list
fn_(ArrList_ensureTotalCap(ArrList* self, usize new_cap), mem_Allocator_Err$void);
/// Ensure precise total capacity of the array list
fn_(ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap), mem_Allocator_Err$void);
/// Ensure unused capacity of the array list
fn_(ArrList_ensureUnusedCap(ArrList* self, usize additional), mem_Allocator_Err$void);

/* Size Operations */
/// Resize the array list to a new length
fn_(ArrList_resize(ArrList* self, usize new_len), mem_Allocator_Err$void);
/// Shrink the array list and free excess memory
fn_(ArrList_shrinkAndFree(ArrList* self, usize new_len), void);
/// Shrink the array list without freeing memory
fn_(ArrList_shrinkRetainingCap(ArrList* self, usize new_len), void);
/// Expand the array list to its full capacity
fn_(ArrList_expandToCap(ArrList* self), void);

/* Adding Elements */
/// Add an element to the end of the array list
fn_(ArrList_append(ArrList* self, meta_Ptr item), mem_Allocator_Err$void);
/// Add a slice of elements to the end of the array list
fn_(ArrList_appendSli(ArrList* self, meta_Sli items), mem_Allocator_Err$void);
/// Add an element a specified number of times to the end of the array list
fn_(ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n), mem_Allocator_Err$void);
/// Add an element to the front of the array list
fn_(ArrList_prepend(ArrList* self, meta_Ptr item), mem_Allocator_Err$void);
/// Add a slice of elements to the front of the array list
fn_(ArrList_prependSli(ArrList* self, meta_Sli items), mem_Allocator_Err$void);
/// Add an element a specified number of times to the front of the array list
fn_(ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n), mem_Allocator_Err$void);

/// Add one element to the end of the array list
fn_(ArrList_addBackOne(ArrList* self), mem_Allocator_Err$meta_Ptr);
/// Add one element to the end of the array list
fn_(ArrList_addBackOneAssumeCap(ArrList* self), meta_Ptr);
/// Add multiple elements to the end of the array list as a slice
fn_(ArrList_addBackManyAsSli(ArrList* self, usize n), mem_Allocator_Err$meta_Sli);
/// Add multiple elements to the end of the array list as a slice
fn_(ArrList_addBackManyAsSliAssumeCap(ArrList* self, usize n), meta_Sli);

/// Add one element to the front of the array list
fn_(ArrList_addFrontOne(ArrList* self), mem_Allocator_Err$meta_Ptr);
/// Add one element to the front of the array list
fn_(ArrList_addFrontOneAssumeCap(ArrList* self), meta_Ptr);
/// Add multiple elements to the front of the array list as a slice
fn_(ArrList_addFrontManyAsSli(ArrList* self, usize n), mem_Allocator_Err$meta_Sli);
/// Add multiple elements to the front of the array list as a slice
fn_(ArrList_addFrontManyAsSliAssumeCap(ArrList* self, usize n), meta_Sli);

/* Inserting Elements */
/// Insert an element at a specified index
fn_(ArrList_insert(ArrList* self, usize index, meta_Ptr item), mem_Allocator_Err$void);
/// Insert a slice of elements at a specified index
fn_(ArrList_insertSli(ArrList* self, usize index, meta_Sli items), mem_Allocator_Err$void);

/* Removing Elements */
/// Remove and return the last element from the array list
fn_(ArrList_pop(ArrList* self), void);
/// Remove and return the last element from the array list
fn_(ArrList_popOrNull(ArrList* self), Opt$meta_Ptr);
/// Remove and return the first element from the array list
fn_(ArrList_shift(ArrList* self), void);
/// Remove and return the first element from the array list
fn_(ArrList_shiftOrNull(ArrList* self), Opt$meta_Ptr);
/// Remove an element at a specified index in order
fn_(ArrList_removeOrdered(ArrList* self, usize index), meta_Ptr);
/// Remove an element at a specified index by swapping with the last element
fn_(ArrList_removeSwap(ArrList* self, usize index), meta_Ptr);

/* Clearing */
/// Clear the array list and retain the allocated capacity
fn_(ArrList_clearRetainingCap(ArrList* self), void);
/// Clear the array list and free all allocated memory
fn_(ArrList_clearAndFree(ArrList* self), void);
#endif /* CHEAT_SHEET */

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

/// @brief Dynamic array list structure
/// @details Provides a contiguous memory buffer that automatically resizes
typedef struct ArrList {
    meta_Sli      items;     ///< Slice containing the elements with meta
    usize         cap;       ///< Current capacity of the list
    mem_Allocator allocator; ///< Memory allocator to use
} ArrList;
use_Opt$(ArrList);
use_Err$(ArrList);
use_ErrSet$(mem_Allocator_Err, ArrList);

#define use_ArrList$(T)                                               \
    /**                                                               \
     * @brief Declare and implement typed array list                  \
     * @param T Type of elements to store in the list                 \
     * @example                                                       \
     *     use_ArrList$(i32); // Declare and implement i32 array list \
     */                                                               \
    comp_type_gen__use_ArrList$(T)
#define decl_ArrList$(T)                                           \
    /**                                                            \
     * @brief Declare typed array list structure                   \
     * @param T Type of elements to store in the list              \
     * @example                                                    \
     *     decl_ArrList$(i32); // Declare i32 array list structure \
     */                                                            \
    comp_type_gen__decl_ArrList$(T)
#define impl_ArrList$(T)                                                  \
    /**                                                                   \
     * @brief Implement typed array list structure                        \
     * @param T Type of elements to store in the list                     \
     * @example                                                           \
     *     impl_ArrList$(i32); // Implement previously declared structure \
     */                                                                   \
    comp_type_gen__impl_ArrList$(T)

#define ArrList$(T)                                                    \
    /**                                                                \
     * @brief Create an array list type                                \
     * @param T Type of elements to store in the list                  \
     * @return Array list type alias                                   \
     * @example                                                        \
     *     ArrList$(i32) list; // Create an array list of i32 elements \
     */                                                                \
    comp_type_alias__ArrList$(T)
#define ArrList$$(T)                                                               \
    /**                                                                            \
     * @brief Create a anonymous array list structure                              \
     * @param T Type of elements to store in the list                              \
     * @return Anonymous array list type token                                     \
     * @details Creates an anonymous union with both base ArrList and typed fields \
     * @example                                                                    \
     *     ArrList$$(i32) anon; // Create anonymous array list of i32 elements     \
     */                                                                            \
    comp_type_anon__ArrList$$(T)
#define ArrList_anonCast$(T_ArrList, var_anon...)                    \
    /**                                                              \
     * @brief Cast anonymous array list to a specific type           \
     * @param T_ArrList Target array list type                       \
     * @param var_anon Anonymous array list variable                 \
     * @return Cast array list of specified type                     \
     * @example                                                      \
     *     ArrList$i32 typed = ArrList_anonCast$(ArrList$i32, anon); \
     */                                                              \
    comp_op__ArrList_anonCast$(pp_uniqTok(anon), T_ArrList, var_anon)

/*========== Function Prototypes ============================================*/

/// @brief Initialize an empty list
/// @param type Type information for the elements
/// @param allocator Memory allocator to use
/// @return Initialized array list
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
extern fn_(ArrList_init(TypeInfo type, mem_Allocator allocator), ArrList);

/// @brief Initialize list with pre-allocated capacity
/// @param type Type information for the elements
/// @param allocator Memory allocator to use
/// @param cap Initial capacity to allocate
/// @return Result containing initialized list or error
/// @example
///     ArrList list = ArrList_initCap(typeInfo$(i32), allocator, 10);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_initCap(typeInfo$(i32), allocator, 10));
extern fn_(ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap), must_check mem_Allocator_Err$ArrList);

/// @brief Free resources used by the list
/// @param self Pointer to array list
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_fini(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_fini(list.base);
extern fn_(ArrList_fini(ArrList* self), void);

/// @brief Convert list to owned slice (caller takes ownership of memory)
/// @param self Pointer to array list
/// @return Result containing owned slice or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Sli slice = try_(ArrList_toOwnedSli(&list));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Sli$i32 slice = meta_cast$(Sli$i32, try_(ArrList_toOwnedSli(list.base)));
extern fn_(ArrList_toOwnedSli(ArrList* self), must_check mem_Allocator_Err$meta_Sli);

/// @brief Create array list from an owned slice
/// @param allocator Memory allocator to use
/// @param slice Owned slice to convert
/// @return Initialized array list
/// @example
///     ArrList list = ArrList_fromOwnedSli(allocator, slice);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_fromOwnedSli(allocator, slice));
extern fn_(ArrList_fromOwnedSli(mem_Allocator allocator, meta_Sli slice), ArrList);

/// @brief Clone array list
/// @param self Pointer to array list to clone
/// @return Result containing cloned list or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList cloned = try_(ArrList_clone(&list));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList$i32 cloned = type$(ArrList$i32, try_(ArrList_clone(list.base)));
extern fn_(ArrList_clone(const ArrList* self), must_check mem_Allocator_Err$ArrList);

/// @brief Ensure total capacity of the list
/// @param self Pointer to array list
/// @param new_cap Desired capacity
/// @return Success or error
/// @details May allocate more capacity than requested for efficiency
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_ensureTotalCap(&list, 20));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_ensureTotalCap(list.base, 20));
extern fn_(ArrList_ensureTotalCap(ArrList* self, usize new_cap), must_check mem_Allocator_Err$void);

/// @brief Ensure precise total capacity of the list
/// @param self Pointer to array list
/// @param new_cap Desired capacity
/// @return Success or error
/// @details Allocates exactly the requested capacity
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_ensureTotalCapPrecise(&list, 20));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_ensureTotalCapPrecise(list.base, 20));
extern fn_(ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap), must_check mem_Allocator_Err$void);

/// @brief Ensure there is capacity for additional elements
/// @param self Pointer to array list
/// @param additional Number of additional elements to ensure capacity for
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_ensureUnusedCap(&list, 5));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_ensureUnusedCap(list.base, 5));
extern fn_(ArrList_ensureUnusedCap(ArrList* self, usize additional), must_check mem_Allocator_Err$void);

/// @brief Resize the list to a new length
/// @param self Pointer to array list
/// @param new_len New length for the list
/// @return Success or error
/// @details May allocate if growing beyond current capacity
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_resize(&list, 10));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_resize(list.base, 10));
extern fn_(ArrList_resize(ArrList* self, usize new_len), must_check mem_Allocator_Err$void);

/// @brief Shrink the list and free excess memory
/// @param self Pointer to array list
/// @param new_len New length for the list
/// @details Frees memory if shrinking the list
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_shrinkAndFree(&list, 5);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_shrinkAndFree(list.base, 5);
extern fn_(ArrList_shrinkAndFree(ArrList* self, usize new_len), void);

/// @brief Shrink the list without freeing memory
/// @param self Pointer to array list
/// @param new_len New length for the list
/// @details Retains allocated memory for future use
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_shrinkRetainingCap(&list, 5);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_shrinkRetainingCap(list.base, 5);
extern fn_(ArrList_shrinkRetainingCap(ArrList* self, usize new_len), void);

/// @brief Expand the list to its full capacity
/// @param self Pointer to array list
/// @details Increases length to match capacity
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_expandToCap(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_expandToCap(list.base);
extern fn_(ArrList_expandToCap(ArrList* self), void);

/// @brief Add an element to the end of the list
/// @param self Pointer to array list
/// @param item Pointer to item to append
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_append(&list, pointer));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_append(list.base, pointer));
extern fn_(ArrList_append(ArrList* self, meta_Ptr item), must_check mem_Allocator_Err$void);

/// @brief Add multiple elements to the end of the list
/// @param self Pointer to array list
/// @param items Slice of items to append
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_appendSli(&list, slice));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_appendSli(list.base, slice));
extern fn_(ArrList_appendSli(ArrList* self, meta_Sli items), must_check mem_Allocator_Err$void);

/// @brief Add the same value multiple times to the end of the list
/// @param self Pointer to array list
/// @param value Pointer to value to append
/// @param n Number of times to append the value
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_appendNTimes(&list, pointer, 3));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_appendNTimes(list.base, pointer, 3));
extern fn_(ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n), must_check mem_Allocator_Err$void);

/// @brief Add an element to the front of the list
/// @param self Pointer to array list
/// @param item Pointer to item to prepend
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_prepend(&list, pointer));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_prepend(list.base, pointer));
extern fn_(ArrList_prepend(ArrList* self, meta_Ptr item), must_check mem_Allocator_Err$void);

/// @brief Add multiple elements to the front of the list
/// @param self Pointer to array list
/// @param items Slice of items to prepend
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_prependSli(&list, slice));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_prependSli(list.base, slice));
extern fn_(ArrList_prependSli(ArrList* self, meta_Sli items), must_check mem_Allocator_Err$void);

/// @brief Add the same value multiple times to the front of the list
/// @param self Pointer to array list
/// @param value Pointer to value to prepend
/// @param n Number of times to prepend the value
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_prependNTimes(&list, pointer, 3));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_prependNTimes(list.base, pointer, 3));
extern fn_(ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n), must_check mem_Allocator_Err$void);

/// @brief Add one element to the end of the list and return a pointer to it
/// @param self Pointer to array list
/// @return Result containing pointer to the new element or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = try_(ArrList_addBackOne(&list));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, try_(ArrList_addBackOne(list.base)));
extern fn_(ArrList_addBackOne(ArrList* self), must_check mem_Allocator_Err$meta_Ptr);

/// @brief Add one element to the end assuming sufficient capacity
/// @param self Pointer to array list
/// @return Pointer to the new element
/// @details Undefined behavior if capacity is insufficient
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = ArrList_addBackOneAssumeCap(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, ArrList_addBackOneAssumeCap(list.base));
extern fn_(ArrList_addBackOneAssumeCap(ArrList* self), meta_Ptr);

/// @brief Add multiple elements to the end and return them as a slice
/// @param self Pointer to array list
/// @param n Number of elements to add
/// @return Result containing slice of new elements or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Sli items = try_(ArrList_addBackManyAsSli(&list, 5));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Sli$i32 items = meta_cast$(Sli$i32, try_(ArrList_addBackManyAsSli(list.base, 5)));
extern fn_(ArrList_addBackManyAsSli(ArrList* self, usize n), must_check mem_Allocator_Err$meta_Sli);

/// @brief Add multiple elements to the end assuming sufficient capacity
/// @param self Pointer to array list
/// @param n Number of elements to add
/// @return Slice of the new elements
/// @details Undefined behavior if capacity is insufficient
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Sli items = ArrList_addBackManyAsSliAssumeCap(&list, 5);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Sli$i32 items = meta_cast$(Sli$i32, ArrList_addBackManyAsSliAssumeCap(list.base, 5));
extern fn_(ArrList_addBackManyAsSliAssumeCap(ArrList* self, usize n), meta_Sli);

/// @brief Add one element to the front and return a pointer to it
/// @param self Pointer to array list
/// @return Result containing pointer to the new element or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = try_(ArrList_addFrontOne(&list));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, try_(ArrList_addFrontOne(list.base)));
extern fn_(ArrList_addFrontOne(ArrList* self), must_check mem_Allocator_Err$meta_Ptr);

/// @brief Add one element to the front assuming sufficient capacity
/// @param self Pointer to array list
/// @return Pointer to the new element
/// @details Undefined behavior if capacity is insufficient
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = ArrList_addFrontOneAssumeCap(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, ArrList_addFrontOneAssumeCap(list.base));
extern fn_(ArrList_addFrontOneAssumeCap(ArrList* self), meta_Ptr);

/// @brief Add multiple elements to the front and return them as a slice
/// @param self Pointer to array list
/// @param n Number of elements to add
/// @return Result containing slice of new elements or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Sli items = try_(ArrList_addFrontManyAsSli(&list, 5));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Sli$i32 items = meta_cast$(Sli$i32, try_(ArrList_addFrontManyAsSli(list.base, 5)));
extern fn_(ArrList_addFrontManyAsSli(ArrList* self, usize n), must_check mem_Allocator_Err$meta_Sli);

/// @brief Add multiple elements to the front assuming sufficient capacity
/// @param self Pointer to array list
/// @param n Number of elements to add
/// @return Slice of the new elements
/// @details Undefined behavior if capacity is insufficient
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Sli items = ArrList_addFrontManyAsSliAssumeCap(&list, 5);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Sli$i32 items = meta_cast$(Sli$i32, ArrList_addFrontManyAsSliAssumeCap(list.base, 5));
extern fn_(ArrList_addFrontManyAsSliAssumeCap(ArrList* self, usize n), meta_Sli);

/// @brief Insert an element at a specific index
/// @param self Pointer to array list
/// @param index Index at which to insert
/// @param item Pointer to item to insert
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_insert(&list, 2, pointer));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_insert(list.base, 2, pointer));
extern fn_(ArrList_insert(ArrList* self, usize index, meta_Ptr item), must_check mem_Allocator_Err$void);

/// @brief Insert multiple elements at a specific index
/// @param self Pointer to array list
/// @param index Index at which to insert
/// @param items Slice of items to insert
/// @return Success or error
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     try_(ArrList_insertSli(&list, 2, slice));
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     try_(ArrList_insertSli(list.base, 2, slice));
extern fn_(ArrList_insertSli(ArrList* self, usize index, meta_Sli items), must_check mem_Allocator_Err$void);

/// @brief Remove the last element from the list
/// @param self Pointer to array list
/// @details Does nothing if the list is empty
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_pop(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_pop(list.base);
extern fn_(ArrList_pop(ArrList* self), void);

/// @brief Remove the last element and return it, or null if empty
/// @param self Pointer to array list
/// @return Optional pointer to the removed item
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     Opt$meta_Ptr item = ArrList_popOrNull(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Opt$Ptr$i32 item = meta_cast$(Opt$Ptr$i32, ArrList_popOrNull(list.base));
extern fn_(ArrList_popOrNull(ArrList* self), Opt$meta_Ptr);

/// @brief Remove the first element from the list
/// @param self Pointer to array list
/// @details Does nothing if the list is empty
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_shift(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_shift(list.base);
extern fn_(ArrList_shift(ArrList* self), void);

/// @brief Remove the first element and return it, or null if empty
/// @param self Pointer to array list
/// @return Optional pointer to the removed item
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     Opt$meta_Ptr item = ArrList_shiftOrNull(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     Opt$Ptr$i32 item = meta_cast$(Opt$Ptr$i32, ArrList_shiftOrNull(list.base));
extern fn_(ArrList_shiftOrNull(ArrList* self), Opt$meta_Ptr);

/// @brief Remove element at index preserving order
/// @param self Pointer to array list
/// @param index Index of element to remove
/// @return Pointer to the removed element (temporary)
/// @details Shifts all elements after the removed element
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = ArrList_removeOrdered(&list, 2);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, ArrList_removeOrdered(list.base, 2));
extern fn_(ArrList_removeOrdered(ArrList* self, usize index), meta_Ptr);

/// @brief Remove element at index by swapping with last element
/// @param self Pointer to array list
/// @param index Index of element to remove
/// @return Pointer to the removed element (temporary)
/// @details Fast removal but doesn't preserve order
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     meta_Ptr item = ArrList_removeSwap(&list, 2);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     i32* item = meta_cast$(i32*, ArrList_removeSwap(list.base, 2));
extern fn_(ArrList_removeSwap(ArrList* self, usize index), meta_Ptr);

/// @brief Clear list without freeing memory
/// @param self Pointer to array list
/// @details Retains allocated capacity for future use
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_clearRetainingCap(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_clearRetainingCap(list.base);
extern fn_(ArrList_clearRetainingCap(ArrList* self), void);

/// @brief Clear list and free all memory
/// @param self Pointer to array list
/// @details Frees all allocated memory
/// @example
///     ArrList list = ArrList_init(typeInfo$(i32), allocator);
///     ...
///     ArrList_clearAndFree(&list);
///     ---
///     ArrList$i32 list = type$(ArrList$i32, ArrList_init(typeInfo$(i32), allocator));
///     ...
///     ArrList_clearAndFree(list.base);
extern fn_(ArrList_clearAndFree(ArrList* self), void);

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_ArrList$(T) \
    decl_ArrList$(T);                  \
    impl_ArrList$(T)
#define comp_type_gen__decl_ArrList$(T) \
    typedef union ArrList$(T) ArrList$(T)
#define comp_type_gen__impl_ArrList$(T) \
    union ArrList$(T) {                 \
        ArrList base[1];                \
        struct {                        \
            TypeInfo type;              \
            Sli$(T) items;              \
            usize         cap;          \
            mem_Allocator allocator;    \
        };                              \
    }

#define comp_type_alias__ArrList$(T) \
    pp_join($, ArrList, T)
#define comp_type_anon__ArrList$$(T) \
    union {                          \
        ArrList base[1];             \
        struct {                     \
            TypeInfo type;           \
            Sli$$(T) items;          \
            usize         cap;       \
            mem_Allocator allocator; \
        };                           \
    }
#define comp_op__ArrList_anonCast$(__anon, T_ArrList, var_anon...) eval({                                \
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_LIST_INCLUDED */
