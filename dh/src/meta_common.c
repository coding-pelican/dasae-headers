#if UNUSED_CODE
#include "dh/meta/common.h"

P_const$raw Sli_rawAt(TypeInfo type, P_const$raw ptr, usize len, usize index) {
    $unused(len);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return as$((u8*)(ptr)) + (index * type.size);
}

P$raw Sli_rawAt_mut(TypeInfo type, P$raw ptr, usize len, usize index) {
    $unused(len);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(index < len, "Index out of bounds (index: %zu, len: %zu)", index, len);
    return as$((u8*)(ptr)) + (index * type.size);
}

P_const$raw Sli_rawSlice(TypeInfo type, P_const$raw ptr, usize len, usize begin, usize end) {
    $unused(len, end);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return as$((u8*)(ptr)) + (begin * type.size);
}

P$raw Sli_rawSlice_mut(TypeInfo type, P$raw ptr, usize len, usize begin, usize end) {
    $unused(len, end);
    debug_assert_nonnull(ptr);
    debug_assert_fmt(begin <= end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= len, "Slice out of bounds (end: %zu, len: %zu)", end, len);
    return as$((u8*)(ptr)) + (begin * type.size);
}

fn_((meta_P_constCast(meta_P_const$raw self))(meta_P$raw)) {
    debug_assert_nonnull(self.addr);
    return (meta_P$raw){ .as_const = self };
}

fn_((meta_mutCast$P(meta_P$raw self))(meta_P_const$raw)) {
    debug_assert_nonnull(self.ptr);
    return self.as_const;
}

fn_((meta_P_toSli(meta_P$raw self, TypeInfo type))(meta_S$raw)) {
    debug_assert_nonnull(self.ptr);
    debug_assert(self.type.size == sizeOf$(S$raw));
    debug_assert(self.type.align == alignOf$(S$raw));

    let sli_opaque = *as$((S$raw*)(&self.ptr));
    return (meta_S$raw){
        .type = type,
        .addr = sli_opaque.ptr,
        .len = sli_opaque.len
    };
}

fn_((meta_P_copy(meta_P$raw dest, meta_P_const$raw src))(meta_P$raw)) {
    debug_assert_nonnull(dest.ptr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    memcpy(dest.ptr, src.addr, dest.type.size);
    return dest;
} fn_((meta_P_move(meta_P$raw dest, meta_P$raw src))(meta_P$raw)) {
    debug_assert_nonnull(dest.ptr);
    debug_assert_nonnull(src.ptr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    memmove(dest.ptr, src.ptr, dest.type.size);
    return dest;
}

fn_((meta_S_constCast(meta_S_const$raw self))(meta_S$raw)) {
    debug_assert_nonnull(self.addr);
    return (meta_S$raw){ .as_const = self };
} fn_((meta_S_mutCast(meta_S$raw self))(meta_S_const$raw)) {
    debug_assert_nonnull(self.addr);
    return self.as_const;
} fn_((meta_S_asPtr(meta_S$raw self))(meta_P$raw)) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(self.len == 1, "Sli must have exactly one element");
    return (meta_P$raw){
        .type = self.type,
        .ptr = self.addr
    };
} fn_((meta_S_len(meta_S$raw self))(usize)) {
    debug_assert_nonnull(self.addr);
    return self.len;
} fn_((meta_S_at(meta_S$raw self, usize index))(meta_P$raw)) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(index < self.len, "Index out of bounds (index: %zu, len: %zu)", index, self.len);
    return (meta_P$raw){
        .type = self.type,
        .ptr = as$((u8*)(self.addr)) + (index * self.type.size)
    };
} fn_((meta_S_slice(meta_S$raw self, usize begin, usize end))(meta_S$raw)) {
    debug_assert_nonnull(self.addr);
    debug_assert_fmt(begin < end, "Invalid range (begin: %zu, end: %zu)", begin, end);
    debug_assert_fmt(end <= self.len, "Slice out of bounds (end: %zu, len: %zu)", end, self.len);
    return (meta_S$raw){
        .type = self.type,
        .addr = as$((u8*)(self.addr)) + (begin * self.type.size),
        .len = end - begin
    };
}

fn_((meta_S_set(meta_S$raw dest, meta_P_const$raw value))(meta_S$raw)) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(value.addr);
    debug_assert(dest.type.size == value.type.size);
    debug_assert(dest.type.align == value.type.align);
    for (usize i = 0; i < dest.len; ++i) {
        // meta_P_copy(meta_S_at(dest, i), value);
        memcpy(
            as$((u8*)(dest.addr)) + (i * dest.type.size), value.addr, dest.type.size);
    }
    return dest;
} fn_((meta_S_copy(meta_S$raw dest, meta_S_const$raw src))(meta_S$raw)) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    debug_assert(dest.len == src.len);
    // for (usize i = 0; i < dest.len; ++i) {
    //     meta_P_copy(
    //         meta_S_at(dest, i),
    //         meta_S_at(meta_S_constCast(src), i).as_const
    //     );
    // }
    memcpy(dest.addr, src.addr, dest.type.size * dest.len);
    return dest;
} fn_((meta_S_move(meta_S$raw dest, meta_S$raw src))(meta_S$raw)) {
    debug_assert_nonnull(dest.addr);
    debug_assert_nonnull(src.addr);
    debug_assert(dest.type.size == src.type.size);
    debug_assert(dest.type.align == src.type.align);
    debug_assert(dest.len == src.len);
    // for (usize i = 0; i < dest.len; ++i) {
    //     meta_P_move(
    //         meta_S_at(dest, i),
    //         meta_S_at(src, i)
    //     );
    // }
    memmove(dest.addr, src.addr, dest.type.size * dest.len);
    return dest;
}

#endif /* UNUSED_CODE */
