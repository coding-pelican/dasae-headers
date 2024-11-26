// /*========== Includes =======================================================*/

// #include "dh/core/Slice.h"
// #include "dh/claim/unreachable.h"
// #include <string.h>

// /*========== Core Implementation ============================================*/

// Slice Slice_from(Ptr ptr, usize len) {
//     if (Ptr_isNull(ptr) || len == 0) {
//         return Slice_null;
//     }
//     return (Slice){
//         .ptr_ = ptr,
//         .len_ = len
//     };
// }

// Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len) {
//     if (raw == null || elem_size == 0 || len == 0) {
//         return Slice_null;
//     }
//     return Slice_from(Ptr_from(raw, elem_size, false), len);
// }

// bool Slice_isNull(Slice s) {
//     return Ptr_isNull(s.ptr_) || s.len_ == 0;
// }

// bool Slice_isEmpty(Slice s) {
//     return s.len_ == 0;
// }

// usize Slice_len(Slice s) {
//     return s.len_;
// }

// usize Slice_elemSize(Slice s) {
//     return Ptr_size(s.ptr_);
// }

// Ptr Slice_ptr(Slice s) {
//     return s.ptr_;
// }

// anyptr Slice_raw(Slice s) {
//     return Ptr_raw(s.ptr_);
// }

// /*========== Element Access Implementation =================================*/


// anyptr Slice_atRaw(Slice s, usize index) {
//     claim_assert_fmt(!Slice_isNull(s), "Cannot access null slice");
//     claim_assert_fmt(index < s.len_, "Index %zu out of bounds for slice of length %zu", index, s.len_);

//     return (u8*)Slice_raw(s) + (index * Ptr_size(s.ptr_));
// }

// bool Slice_tryAtRaw(Slice s, usize index, anyptr* out_ptr) {
//     if (Slice_isNull(s) || index >= s.len_) {
//         return false;
//     }

//     *out_ptr = (u8*)Slice_raw(s) + (index * Ptr_size(s.ptr_));
//     return true;
// }

// anyptr Slice_atRawOrNull(Slice s, usize index) {
//     if (Slice_isNull(s) || index >= s.len_) {
//         return null;
//     }

//     return (u8*)Slice_raw(s) + (index * Ptr_size(s.ptr_));
// }

// anyptr Slice_firstRaw(Slice s) {
//     claim_assert_fmt(!Slice_isNull(s), "Cannot get first element of null slice");
//     return Slice_raw(s);
// }

// anyptr Slice_firstRawOrNull(Slice s) {
//     return Slice_isNull(s) ? null : Slice_raw(s);
// }

// anyptr Slice_lastRaw(Slice s) {
//     claim_assert_fmt(!Slice_isNull(s), "Cannot get last element of null slice");
//     return (u8*)Slice_raw(s) + ((s.len_ - 1) * Ptr_size(s.ptr_));
// }

// anyptr Slice_lastRawOrNull(Slice s) {
//     return Slice_isNull(s) ? null : (u8*)Slice_raw(s) + ((s.len_ - 1) * Ptr_size(s.ptr_));
// }

// anyptr Slice_beginRaw(Slice s) {
//     claim_assert_fmt(!Slice_isNull(s), "Cannot iterate null slice");
//     return Slice_raw(s);
// }

// anyptr Slice_beginRawOrNull(Slice s) {
//     return Slice_isNull(s) ? null : Slice_raw(s);
// }

// anyptr Slice_endRaw(Slice s) {
//     claim_assert_fmt(!Slice_isNull(s), "Cannot iterate null slice");
//     return (u8*)Slice_raw(s) + (s.len_ * Ptr_size(s.ptr_));
// }

// anyptr Slice_endRawOrNull(Slice s) {
//     return Slice_isNull(s) ? null : (u8*)Slice_raw(s) + (s.len_ * Ptr_size(s.ptr_));
// }

// /*========== Slicing Operations Implementation ============================*/

// Slice Slice_subslice(Slice s, usize begin, usize end) {
//     if (begin > end || end > s.len_) {
//         return Slice_null;
//     }

//     usize  elem_size = Ptr_size(s.ptr_);
//     anyptr start     = (u8*)Slice_raw(s) + (begin * elem_size);
//     return Slice_from(
//         Ptr_from(start, elem_size, Ptr_flags(s.ptr_)),
//         end - begin
//     );
// }

// Slice Slice_suffix(Slice s, usize begin) {
//     return Slice_subslice(s, begin, s.len_);
// }

// Slice Slice_prefix(Slice s, usize end) {
//     return Slice_subslice(s, 0, end);
// }

// /*========== Memory Operations Implementation ============================*/

// bool Slice_copy(Slice dest, Slice src) {
//     if (Slice_isNull(dest) || Slice_isNull(src)) {
//         return false;
//     }

//     if (dest.len_ < src.len_) {
//         return false;
//     }

//     usize dest_size = Ptr_size(dest.ptr_);
//     usize src_size  = Ptr_size(src.ptr_);

//     if (dest_size != src_size) {
//         return false;
//     }

//     memcpy(
//         Slice_raw(dest),
//         Slice_raw(src),
//         src.len_ * src_size
//     );
//     return true;
// }

// void Slice_set(Slice s, i32 value) {
//     if (!Slice_isNull(s)) {
//         memset(
//             Slice_raw(s),
//             value,
//             s.len_ * Ptr_size(s.ptr_)
//         );
//     }
// }

// i32 Slice_cmp(Slice lhs, Slice rhs) {
//     if (Slice_isNull(lhs) || Slice_isNull(rhs)) {
//         return Slice_isNull(lhs) - Slice_isNull(rhs);
//     }

//     usize lhs_size = Ptr_size(lhs.ptr_);
//     usize rhs_size = Ptr_size(rhs.ptr_);

//     if (lhs_size != rhs_size) {
//         return (i32)(lhs_size - rhs_size);
//     }

//     usize min_len = lhs.len_ < rhs.len_ ? lhs.len_ : rhs.len_;
//     i32   result  = memcmp(
//         Slice_raw(lhs),
//         Slice_raw(rhs),
//         min_len * lhs_size
//     );

//     if (result != 0) {
//         return result;
//     }

//     return (i32)(lhs.len_ - rhs.len_);
// }

// /*========== Debug Support Implementation ================================*/

// #ifdef DEBUG_SLICE_CHECKS
// bool Slice_isValid(Slice s) {
//     if (Slice_isNull(s)) {
//         return true; // Null slice is valid
//     }

//     // Check underlying pointer validity
//     if (!Ptr_isValid(s.ptr_)) {
//         return false;
//     }

//     // Check element size
//     if (Ptr_size(s.ptr_) == 0) {
//         return false;
//     }

//     // Check length
//     if (s.len_ == 0) {
//         return false;
//     }

//     // Check total size doesn't overflow
//     usize total_size;
//     if (__builtin_mul_overflow(s.len_, Ptr_size(s.ptr_), &total_size)) {
//         return false;
//     }

//     return true;
// }
// #endif
