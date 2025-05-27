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

fn_(meta_Ptr_constCast(meta_Ptr_const self), meta_Ptr) {
    debug_assert_nonnull(self.addr);
    return (meta_Ptr){ .as_const = self };
}
fn_(meta_Ptr_mutCast(meta_Ptr self), meta_Ptr_const) {
    debug_assert_nonnull(self.addr);
    return self.as_const;
}
fn_(meta_Ptr_toSli(meta_Ptr self, TypeInfo type), meta_Sli) {
    debug_assert_nonnull(self.addr);
    debug_assert(self.type.size == sizeOf$(Sli));
    debug_assert(self.type.align == alignOf$(Sli));

    let sli_opaque = *as$(Sli*, &self.addr);
    return (meta_Sli){
        .type = type,
        .addr = sli_opaque.ptr,
        .len  = sli_opaque.len
    };
}

fn_(meta_Ptr_copy(meta_Ptr dest, meta_Ptr_const src), meta_Ptr) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    bti_memcpy(dest.addr, src.addr, dest.type.size);
    return dest;
}
fn_(meta_Ptr_move(meta_Ptr dest, meta_Ptr src), meta_Ptr) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    bti_memmove(dest.addr, src.addr, dest.type.size);
    return dest;
}

fn_(meta_Sli_constCast(meta_Sli_const self), meta_Sli) {
    debug_assert_nonnull(self.addr);
    return (meta_Sli){ .as_const = self };
}
fn_(meta_Sli_mutCast(meta_Sli self), meta_Sli_const) {
    debug_assert_nonnull(self.addr);
    return self.as_const;
}
fn_(meta_Sli_asPtr(meta_Sli self), meta_Ptr) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(self.len == 1, "Sli must have exactly one element");
    return (meta_Ptr){
        .type = self.type,
        .addr = self.addr
    };
}
fn_(meta_Sli_len(meta_Sli self), usize) {
    debug_assert_nonnull(self.addr);
    return self.len;
}
fn_(meta_Sli_at(meta_Sli self, usize index), meta_Ptr) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(index < self.len, "Index out of bounds (index: %zu, len: %zu)", index, self.len);
    return (meta_Ptr){
        .type = self.type,
        .addr = as$(u8*, self.addr) + (index * self.type.size)
    };
}
fn_(meta_Sli_slice(meta_Sli self, usize begin, usize end), meta_Sli) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(begin < end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= self.len, "Slice out of bounds (end: %zu, len: %zu)", end, self.len);
    return (meta_Sli){
        .type = self.type,
        .addr = as$(u8*, self.addr) + (begin * self.type.size),
        .len  = end - begin
    };
}

fn_(meta_Sli_set(meta_Sli dest, meta_Ptr_const value), meta_Sli) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(value.addr);
    debug_assert(dest.type.size == value.type.size);
    debug_assert(dest.type.align == value.type.align);
    for (usize i = 0; i < dest.len; ++i) {
        // meta_Ptr_copy(meta_Sli_at(dest, i), value);
        bti_memcpy(
            as$(u8*, dest.addr) + (i * dest.type.size),
            value.addr,
            dest.type.size
        );
    }
    return dest;
}
fn_(meta_Sli_copy(meta_Sli dest, meta_Sli_const src), meta_Sli) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    debug_assert(dest.len == src.len);
    // for (usize i = 0; i < dest.len; ++i) {
    //     meta_Ptr_copy(
    //         meta_Sli_at(dest, i),
    //         meta_Sli_at(meta_Sli_constCast(src), i).as_const
    //     );
    // }
    bti_memcpy(dest.addr, src.addr, dest.type.size * dest.len);
    return dest;
}
fn_(meta_Sli_move(meta_Sli dest, meta_Sli src), meta_Sli) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    debug_assert(dest.len == src.len);
    // for (usize i = 0; i < dest.len; ++i) {
    //     meta_Ptr_move(
    //         meta_Sli_at(dest, i),
    //         meta_Sli_at(src, i)
    //     );
    // }
    bti_memmove(dest.addr, src.addr, dest.type.size * dest.len);
    return dest;
}
