/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Slice.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Slice
 *
 * @brief   View into sequence of elements with type information
 * @details A Slice represents a view into a sequence of elements, consisting of:
 *          - A Ptr to the first element with type metadata
 *          - Length (number of elements)
 *
 * Key features:
 * - No ownership or memory management
 * - Type-safe access to elements
 * - Bounds checking in debug builds
 * - Iterator support
 */

#ifndef CORE_SLICE_INCLUDED
#define CORE_SLICE_INCLUDED (1)

/*========== Includes =======================================================*/

#include "dh/claim/assert_static.h"
#include "dh/core/prim/ptr.h"
#include "dh/core/Ptr.h"

/*========== Type Definition ===============================================*/

typedef struct Slice {
    Ptr   ptr_; // Pointer to first element with type info
    usize len_; // Number of elements
} Slice;

claim_assert_static_msg(sizeof(Slice) == sizeof(usize) * 2, "Slice size mismatch");
// claim_assert_static_msg(alignof(Slice) >= alignof(Ptr), "Slice alignment mismatch");

#define Slice(_T) Slice

/*========== Constants ===================================================*/

#define Slice_null (literal(Slice, .ptr_ = Ptr_null, .len_ = 0))

/*========== Core Functions ==============================================*/

/**
 * Create a slice from Ptr and length
 *
 * @param ptr Pointer with type information
 * @param len Number of elements
 * @return Slice view of the elements
 */
extern Slice Slice_fromPtr(Ptr ptr, usize len);

/**
 * Create a slice from raw pointer, element size, and length
 *
 * @param raw Raw pointer to first element
 * @param elem_size Size of each element in bytes
 * @param len Number of elements
 * @return Slice view of the elements
 */
extern Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len);

/**
 * Create a slice from literal array
 *
 * @param _T Type of array elements
 * @param _elemList Array literal
 * @return Slice view of the array
 */
#define Slice_fromLiteral(_T, _elemList) \
    Slice_from(Ptr_from((_elemList), sizeof(_T), false), sizeof(_elemList) / sizeof(_T))

/**
 * Check if slice is null
 *
 * @param s Slice to check
 * @return true if null pointer or zero length
 */
extern bool Slice_isNull(Slice s);

/**
 * Check if slice is empty
 *
 * @param s Slice to check
 * @return true if zero length
 */
extern bool Slice_isEmpty(Slice s);

/**
 * Get number of elements
 *
 * @param s Slice to query
 * @return Number of elements
 */
extern usize Slice_len(Slice s);

/**
 * Get element size in bytes
 *
 * @param s Slice to query
 * @return Size of each element
 */
extern usize Slice_elemSize(Slice s);

/**
 * Get pointer to first element with type info
 *
 * @param s Slice to query
 * @return Typed pointer to first element
 */
extern Ptr Slice_ptr(Slice s);

/**
 * Get raw pointer to first element
 *
 * @param s Slice to query
 * @return Raw pointer to first element
 */
extern anyptr Slice_raw(Slice s);

/*========== Element Access Core Functions ====================================*/

/**
 * Get raw pointer to element at index
 * Raises debug assertion if index out of bounds
 *
 * @param s Slice to access
 * @param index Element index
 * @return Raw pointer to element
 */
extern anyptr Slice_atRaw(Slice s, usize index);

/**
 * Try to get raw pointer to element at index
 * Returns false if index out of bounds
 *
 * @param s Slice to access
 * @param index Element index
 * @param[out] out_ptr Output pointer if successful
 * @return true if successful
 */
extern bool Slice_tryAtRaw(Slice s, usize index, anyptr* out_ptr);

/**
 * Get raw pointer to element at index
 * Returns null if index out of bounds
 *
 * @param s Slice to access
 * @param index Element index
 * @return Raw pointer to element or null
 */
extern anyptr Slice_atRawOrNull(Slice s, usize index);

/**
 * Get raw pointer to first element
 * Raises debug assertion if slice is empty
 *
 * @param s Slice to access
 * @return Raw pointer to first element
 */
extern anyptr Slice_firstRaw(Slice s);

/**
 * Get raw pointer to first element
 * Returns null if slice is empty
 *
 * @param s Slice to access
 * @return Raw pointer to first element or null
 */
extern anyptr Slice_firstRawOrNull(Slice s);

/**
 * Get raw pointer to last element
 * Raises debug assertion if slice is empty
 *
 * @param s Slice to access
 * @return Raw pointer to last element
 */
extern anyptr Slice_lastRaw(Slice s);

/**
 * Get raw pointer to last element
 * Returns null if slice is empty
 *
 * @param s Slice to access
 * @return Raw pointer to last element or null
 */
extern anyptr Slice_lastRawOrNull(Slice s);

/*========== Type-Safe Element Access Macros =================================*/

/**
 * Get typed pointer to element at index
 * Raises debug assertion if index out of bounds or type mismatch
 */
#define Slice_at(_T, _s, _index) \
    ((_T*)Slice_atRaw(_s, _index))

/**
 * Try to get typed pointer to element at index
 * Returns false if index out of bounds or type mismatch
 */
#define Slice_tryAt(_T, _s, _index, _out_ptr) \
    Slice_tryAtRaw(_s, _index, (anyptr*)(_out_ptr))

/**
 * Get typed pointer to element at index
 * Returns null if index out of bounds or type mismatch
 */
#define Slice_atOrNull(_T, _s, _index) \
    ((_T*)Slice_atRawOrNull(_s, _index))

/**
 * Get typed pointer to first element
 * Raises debug assertion if empty or type mismatch
 */
#define Slice_first(_T, _s) \
    ((_T*)Slice_firstRaw(_s))

/**
 * Get typed pointer to first element
 * Returns null if empty or type mismatch
 */
#define Slice_firstOrNull(_T, _s) \
    ((_T*)Slice_firstRawOrNull(_s))

/**
 * Get typed pointer to last element
 * Raises debug assertion if empty or type mismatch
 */
#define Slice_last(_T, _s) \
    ((_T*)Slice_lastRaw(_s))

/**
 * Get typed pointer to last element
 * Returns null if empty or type mismatch
 */
#define Slice_lastOrNull(_T, _s) \
    ((_T*)Slice_lastRawOrNull(_s))

/*========== Iterator Methods ===============================================*/

/**
 * Get raw pointer to beginning of slice for iteration
 * Raises debug assertion if slice is null
 *
 * @param s Slice to iterate
 * @return Raw pointer to first element
 */
extern anyptr Slice_beginRaw(Slice s);

/**
 * Get raw pointer to beginning of slice for iteration
 * Returns null if slice is null
 *
 * @param s Slice to iterate
 * @return Raw pointer to first element or null
 */
extern anyptr Slice_beginRawOrNull(Slice s);

/**
 * Get raw pointer to one past end of slice for iteration
 * Raises debug assertion if slice is null
 *
 * @param s Slice to iterate
 * @return Raw pointer to one past last element
 */
extern anyptr Slice_endRaw(Slice s);

/**
 * Get raw pointer to one past end of slice for iteration
 * Returns null if slice is null
 *
 * @param s Slice to iterate
 * @return Raw pointer to one past last element or null
 */
extern anyptr Slice_endRawOrNull(Slice s);

/*========== Type-Safe Iterator Macros =====================================*/

/**
 * Get typed pointer to beginning of slice for iteration
 * Raises debug assertion if slice is null or type mismatch
 */
#define Slice_begin(_T, _s) \
    ((_T*)Slice_beginRaw(_s))

/**
 * Get typed pointer to beginning of slice for iteration
 * Returns null if slice is null or type mismatch
 */
#define Slice_beginOrNull(_T, _s) \
    ((_T*)Slice_beginRawOrNull(_s))

/**
 * Get typed pointer to one past end of slice for iteration
 * Raises debug assertion if slice is null or type mismatch
 */
#define Slice_end(_T, _s) \
    ((_T*)Slice_endRaw(_s))

/**
 * Get typed pointer to one past end of slice for iteration
 * Returns null if slice is null or type mismatch
 */
#define Slice_endOrNull(_T, _s) \
    ((_T*)Slice_endRawOrNull(_s))

/*========== Type-Safe Access ===========================================*/

/**
 * Access entire slice as array of type (unsafe)
 * No size checking - use with caution
 */
#define Slice_as(T, s) \
    ((T*)Slice_raw(s))

/**
 * Access entire slice as array of type (safe)
 * Checks element size matches
 */
#define Slice_cast(T, s) \
    (Ptr_size(Slice_ptr(s)) == sizeof(T) ? (T*)Slice_raw(s) : null)

/*========== Slicing Operations ========================================*/

/**
 * Create sub-slice from range
 *
 * @param s Original slice
 * @param begin Start index (inclusive)
 * @param end End index (exclusive)
 * @return New slice view or null slice if invalid range
 */
extern Slice Slice_subslice(Slice s, usize begin, usize end);

/**
 * Create sub-slice from start to end of slice
 *
 * @param s Original slice
 * @param begin Start index
 * @return New slice view or null slice if invalid
 */
extern Slice Slice_suffix(Slice s, usize begin);

/**
 * Create sub-slice from start of slice to end
 *
 * @param s Original slice
 * @param end End index
 * @return New slice view or null slice if invalid
 */
extern Slice Slice_prefix(Slice s, usize end);

/*========== Iteration ================================================*/

/**
 * Iterate over elements of slice
 *
 * @param T Element type
 * @param var Iterator variable name
 * @param s Slice to iterate
 */
#define Slice_iter(T, var, s)                     \
    for (                                         \
        rawptr(T) used(var) = Slice_begin(T, s);  \
        used(var) && used(var) < Slice_end(T, s); \
        ++used(var)                               \
    )

/*========== Memory Operations =========================================*/

/**
 * Copy slice contents to destination
 *
 * @param dest Destination slice
 * @param src Source slice
 * @return true if copy successful
 */
extern bool Slice_copy(Slice dest, Slice src);

/**
 * Set all bytes in slice to value
 *
 * @param s Slice to modify
 * @param value Byte value to set
 */
extern void Slice_set(Slice s, i32 value);

/**
 * Compare two slices
 *
 * @param lhs First slice
 * @param rhs Second slice
 * @return <0 if lhs < rhs, 0 if equal, >0 if lhs > rhs
 */
extern i32 Slice_cmp(Slice lhs, Slice rhs);

/*========== Debug Support ============================================*/

#ifdef DEBUG_SLICE_CHECKS
/**
 * Check if slice is valid
 *
 * @param s Slice to validate
 * @return true if slice is valid
 */
extern bool Slice_isValid(Slice s);

/**
 * Assert slice is valid
 */
#define Slice_check(s) debug_assert(Slice_isValid(s))
#else
#define Slice_check(s) unused(0)
#endif

#endif /* CORE_SLICE_INCLUDED */
