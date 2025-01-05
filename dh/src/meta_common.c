#include "dh/meta/common.h"

Ptr Ptr_constCast(PtrConst self) {
    claim_assert_nonnull(self);
    return (Ptr)self;
}

Sli Sli_constCast(SliConst self) {
    claim_assert_nonnull(self.ptr);
    return (Sli){ .as_const = self };
}

const anyptr Sli_rawAt(TypeInfo type, const anyptr ptr, usize len, usize index) {
    claim_assert_nonnull(ptr);
    claim_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return (u8*)ptr + (index * type.size);
}

anyptr Sli_rawAt_mut(TypeInfo type, anyptr ptr, usize len, usize index) {
    claim_assert_nonnull(ptr);
    claim_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return (u8*)ptr + (index * type.size);
}

const anyptr Sli_rawSlice(TypeInfo type, const anyptr ptr, usize len, usize begin, usize end) {
    claim_assert_nonnull(ptr);
    claim_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    claim_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return (u8*)ptr + (begin * type.size);
}

anyptr Sli_rawSlice_mut(TypeInfo type, anyptr ptr, usize len, usize begin, usize end) {
    claim_assert_nonnull(ptr);
    claim_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    claim_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return (u8*)ptr + (begin * type.size);
}

meta_Ptr meta_Ptr_constCast(meta_PtrConst self) {
    claim_assert_nonnull(self.addr);
    return (meta_Ptr){ .as_const = self };
}

meta_Sli meta_Sli_constCast(meta_SliConst self) {
    claim_assert_nonnull(self.addr);
    return (meta_Sli){ .as_const = self };
}
