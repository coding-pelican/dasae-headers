/**
 * @file slice.h
 * View into sequence of elements with type information
 *
 * A Slice represents a view into a sequence of elements, consisting of:
 * - A Ptr to the first element
 * - Length (number of elements)
 *
 * Key features:
 * - No ownership or memory management
 * - Type-safe access to elements
 * - Bounds checking in debug builds
 */

#include "dh/claim/assert_static.h"
#include "prim/ptr.h"
#ifndef CORE_SLICE_INCLUDED
#define CORE_SLICE_INCLUDED (1)

/*========== Includes =======================================================*/

#include "ptr.h"

/*========== Type Definition ======================================*/

typedef struct Slice {
    Ptr   ptr; // Pointer to first element
    usize len; // Number of elements
} Slice;
claim_assert_static_msg(sizeof(Slice) == sizeof(usize) * 2, "Slice size mismatch");

/*========== Constants ===========================================*/

#define Slice_null (literal(Slice, .ptr = Ptr_null, .len = 0))

/*========== Core Functions =====================================*/

/**
 * Create a slice from Ptr and length
 */
extern Slice Slice_from(Ptr ptr, usize len);

/**
 * Create a slice from raw pointer, element size, and length
 */
extern Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len);

/**
 * Check if slice is null
 */
extern bool Slice_isNull(Slice s);

/**
 * Check if slice is empty
 */
extern bool Slice_isEmpty(Slice s);

/**
 * Get number of elements
 */
extern usize Slice_len(Slice s);

/**
 * Get element size
 */
extern usize Slice_elemSize(Slice s);

/**
 * Get pointer to first element
 */
extern Ptr Slice_ptr(Slice s);

/**
 * Get raw pointer to first element
 */
extern anyptr Slice_raw(Slice s);

/*========== Element Access ======================================*/

/**
 * Get pointer to element at index (raw)
 */
extern anyptr Slice_atRaw(Slice s, usize index);

extern anyptr Slice_firstRaw(Slice s);
extern anyptr Slice_lastRaw(Slice s);

extern anyptr Slice_beginRaw(Slice s);
extern anyptr Slice_endRaw(Slice s);

/**
 * Get typed pointer to element at index (safe)
 */
#define Slice_at(s, index, T) \
    (((index) < Slice_len(s) && Ptr_elemSize(Slice_ptr(s)) == sizeof(T)) ? &((T*)Slice_raw(s))[index] : null)

#define Slice_first(s, T)

#define Slice_last(s, T)

#define Slice_begin(s, T)

#define Slice_end(s, T)

/*========== Type-Safe Access ===============================================*/

/**
 * Access entire slice as array of type (unsafe)
 */

#define Slice_as(s, T) \
    ((T*)Slice_raw(s))

/**
 * Access entire slice as array of type (safe)
 */
#define Slice_cast(s, T) \
    ((Ptr_elemSize(Slice_ptr(s)) == sizeof(T)) ? (T*)Slice_raw(s) : null)

/*========== Slicing Operations =================================*/

/**
 * Create sub-slice from range
 */
extern Slice Slice_subslice(Slice s, usize begin, usize end);

/**
 * Create sub-slice from start to end of slice
 */
extern Slice Slice_suffix(Slice s, usize begin);

/**
 * Create sub-slice from start of slice to end
 */
extern Slice Slice_prefix(Slice s, usize end);


/*========== Iteration =========================================*/

// NOLINTBEGIN(bugprone-macro-parentheses)
/**
 * Iterate over elements of slice
 */
#define Slice_iter(s, T, var)         \
    for (                             \
        T* var = Slice_begin(s, T);   \
        var && var < (Ptr_end(s, T)); \
        ++var                         \
    )

// NOLINTEND(bugprone-macro-parentheses)

/*========== Memory Operations =================================*/

/**
 * Copy slice contents to destination
 */
extern bool Slice_copy(Slice dest, Slice src);

/**
 * Set all bytes in slice to value
 */
extern void Slice_set(Slice s, i32 value);

/**
 * Compare two slices
 */
extern i32 Slice_cmp(Slice lhs, Slice rhs);

/*========== Debug Support ====================================*/

#ifdef DEBUG_SLICE_CHECKS
extern bool Slice_isValid(Slice s);
#define Slice_check(_s) debug_assert(Slice_isValid(_s))
#else
#define Slice_check(_s) unused(0)
#endif

#endif // CORE_SLICE_INCLUDED
