// #ifndef SLICE_INCLUDED
// #define SLICE_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */


// /*========== Includes =======================================================*/

// #include "array.h"
// #include "mem.h"
// #include "primitive.h"

// #include "debug/debug_assert.h"

// /*========== Macros and Definitions =========================================*/

// typedef struct Slice Slice;


// /* Slice structure */
// struct Slice {
//     any   data_;
//     usize length_;
// };
// #define Slice_(...)                       RETURN_Slice_(__VA_ARGS__)
// /* Macros for generic slice operations */
// #define Slice(_T)                         RETURN_Slice(_T)
// /* Create a slice from an array */
// #define Slice_fromArray(_T, _a)           RETURN_Slice_fromArray(_T, _a)
// /* Create a subslice [start, end) */
// #define Slice_slice(_T, _s, _start, _end) RETURN_Slice_slice(_T, _s, _start, _end)
// /* Get slice length */
// #define Slice_length(_T, _s)              RETURN_Slice_length(_T, _s)
// /* Get value at index */
// #define Slice_get(_T, _s, _index)         RETURN_Slice_get(_T, _s, _index)
// /* Set value at index */
// #define Slice_set(_T, _s, _index, _value) RETURN_Slice_set(_T, _s, _index, _value)
// /* Copy slice contents to destination */
// #define Slice_copy(_T, _dest, _src)       RETURN_Slice_copy(_T, _dest, _src)

// /*========== Macros Implementation ==========================================*/

// #define RETURN_Slice_(...) makeWith(Slice, { __VA_ARGS__ })

// #define RETURN_Slice(_T) Slice

// #define RETURN_Slice_fromArray(_T, _a) \
//     Slice_(.data_ = (_a).data_, .length_ = (_a).length_)

// #define RETURN_Slice_slice(_T, _s, _start, _end) \
//     Slice__slice(_s, _start, _end, sizeof(_T))

// #define RETURN_Slice_length(_T, _s) \
//     Slice__length(_s)

// #define RETURN_Slice_get(_T, _s, _index) \
//     (*((const _T*)Slice__at(_s, _index, sizeof(_T))))

// #define RETURN_Slice_set(_T, _s, _index, _value) \
//     (*((_T*)Slice__at(_s, _index, sizeof(_T)))) = (_value)

// #define RETURN_Slice_copy(_T, _dest, _src) \
//     Slice__copy(_dest, _src, sizeof(_T))

// /*========== Inline Function Implementations ================================*/

// force_inline Slice Slice__slice(Slice s, usize start, usize end, usize type_size) {
//     debug_assert(start <= end);
//     debug_assert(end <= s.length_);
//     return Slice_(
//             .data_   = (u8*)s.data_ + (start * type_size),
//             .length_ = end - start
//     );
// }

// force_inline usize Slice__length(Slice s) { return s.length_; }

// force_inline any Slice__at(Slice s, usize index, usize type_size) {
//     debug_assert(index < s.length_);
//     return (u8*)s.data_ + (index * type_size);
// }

// force_inline void Slice__copy(Slice dest, Slice src, usize type_size) {
//     debug_assert(dest.length_ == src.length_);
//     mem_copy(dest.data_, src.data_, dest.length_ * type_size);
// }

// /*========== Extern Constant and Variable Declarations ======================*/

// /*========== Extern Function Prototypes =====================================*/

// /*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

// #ifdef UNIT_TEST

// static void TEST_Slice(void) {
//     Array(i32) array = Array_makeFrom(i32, { 1, 2, 3, 4, 5 });
//     Slice(i32) slice = Slice_fromArray(i32, array);
//     i32 value        = Slice_get(i32, slice, 2);         // value = 3
//     Slice_set(i32, slice, 2, 10);                        // array now contains {1, 2, 10, 4, 5}
//     Slice(i32) subslice = Slice_slice(i32, slice, 1, 4); // subslice contains elements at indices 1 to 3
// }

// #endif /* UNIT_TEST */


// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* SLICE_INCLUDED */
