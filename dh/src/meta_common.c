#include "dh/meta/common.h"

const anyptr Sli_rawAt(TypeInfo type, const anyptr ptr, usize len, usize index) {
    $unused(len);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return as$(u8*, ptr) + (index * type.size);
}

anyptr Sli_rawAt_mut(TypeInfo type, anyptr ptr, usize len, usize index) {
    $unused(len);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return as$(u8*, ptr) + (index * type.size);
}

const anyptr Sli_rawSlice(TypeInfo type, const anyptr ptr, usize len, usize begin, usize end) {
    $unused(len, end);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return as$(u8*, ptr) + (begin * type.size);
}

anyptr Sli_rawSlice_mut(TypeInfo type, anyptr ptr, usize len, usize begin, usize end) {
    $unused(len, end);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return as$(u8*, ptr) + (begin * type.size);
}

meta_Ptr meta_Ptr_constCast(meta_Ptr_const self) {
    debug_assert_nonnull(self.addr);
    return (meta_Ptr){ .as_const = self };
}

meta_Sli meta_Sli_constCast(meta_Sli_const self) {
    debug_assert_nonnull(self.addr);
    return (meta_Sli){ .as_const = self };
}
