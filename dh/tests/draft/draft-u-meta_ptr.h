/**
 * meta_ptr.h - Meta pointer operations for dasae-headers
 *
 * Provides type-erased pointer manipulation with TypeInfo metadata.
 * All functions are static inline to facilitate constant folding.
 *
 * Design principles:
 * - Preconditions are assumed valid; violations are __builtin_unreachable()
 * - No fallback returns; all paths produce valid results or are unreachable
 * - Inline TypeInfo literals preferred over globals for better optimization
 * - const correctness throughout
 */

#ifndef META_PTR_H
#define META_PTR_H

#include "struct_repr.h"

/* ============================================================================
 * Slice Types
 * ============================================================================ */

/**
 * S_const$TypeInfo - Slice of const TypeInfo (for struct field descriptions)
 */
typedef struct S_const_TypeInfo {
    const TypeInfo* ptr;
    usize len;
} S_const$TypeInfo;

/**
 * Create inline TypeInfo slice literal
 * Preferred over global arrays for better constant folding
 */
#define tiSlice$(...) \
    ((S_const$TypeInfo){ \
        .ptr = (const TypeInfo[]){ __VA_ARGS__ }, \
        .len = sizeof((const TypeInfo[]){ __VA_ARGS__ }) / sizeof(TypeInfo) })

/* ============================================================================
 * Meta Pointer Types
 * ============================================================================ */

/**
 * u_P$T - Universal/Untyped meta pointer
 * Contains a raw byte pointer and its TypeInfo metadata
 */
typedef struct u_P {
    TypeOf(u8[0]) * ptr; /* Raw pointer to data */
    TypeInfo ti;         /* Type information (size + alignment) */
} u_P$T;

/**
 * S$u_P$T - Slice of meta pointers
 */
typedef struct S_u_P {
    u_P$T* ptr;
    usize len;
} S$u_P$T;

#define metaPtrSlice$(_arr, _len) \
    ((S$u_P$T){ .ptr = (_arr), .len = (_len) })

/* ============================================================================
 * TypeInfo Slice Operations (pure computation, no pointers dereferenced)
 *
 * These functions compute offsets/sizes from TypeInfo alone.
 * No null checks needed - only TypeInfo values are read.
 * ============================================================================ */

/**
 * Compute field offset within a struct defined by TypeInfo slice
 *
 * Preconditions (unreachable if violated):
 * - field_idx < fields.len
 */
__attribute__((always_inline)) static inline usize
TypeInfo_fieldOffset(const S_const$TypeInfo fields, const usize field_idx) {
    if (field_idx >= fields.len) __builtin_unreachable();

    usize end = 0;

    for (usize i = 0; i <= field_idx; ++i) {
        const usize align_val = alignVal$(fields.ptr[i].align);
        const usize offset = alignUp$(end, align_val);
        if (i == field_idx) return offset;
        end = offset + fields.ptr[i].size;
    }

    __builtin_unreachable();
}

/**
 * Compute total struct size from TypeInfo slice
 *
 * Preconditions (unreachable if violated):
 * - fields.len > 0
 */
__attribute__((always_inline)) static inline usize
TypeInfo_structSize(const S_const$TypeInfo fields) {
    if (fields.len == 0) __builtin_unreachable();

    usize end = 0;
    u32 max_align = 0;

    for (usize i = 0; i < fields.len; ++i) {
        const usize align_val = alignVal$(fields.ptr[i].align);
        const usize offset = alignUp$(end, align_val);
        end = offset + fields.ptr[i].size;

        if (fields.ptr[i].align > max_align) {
            max_align = fields.ptr[i].align;
        }
    }

    return alignUp$(end, alignVal$(max_align));
}

/**
 * Compute struct alignment (log2) from TypeInfo slice
 */
__attribute__((always_inline)) static inline u32
TypeInfo_structAlign(const S_const$TypeInfo fields) {
    if (fields.len == 0) __builtin_unreachable();

    u32 max_align = 0;

    for (usize i = 0; i < fields.len; ++i) {
        if (fields.ptr[i].align > max_align) {
            max_align = fields.ptr[i].align;
        }
    }

    return max_align;
}

/**
 * Create TypeInfo for a struct from its field TypeInfos
 */
__attribute__((always_inline)) static inline TypeInfo
TypeInfo_struct(const S_const$TypeInfo fields) {
    return (TypeInfo){
        .size = TypeInfo_structSize(fields),
        .align = TypeInfo_structAlign(fields)
    };
}

/* ============================================================================
 * Meta Pointer Constructors
 * ============================================================================ */

/**
 * Create a meta pointer from raw pointer and TypeInfo
 */
#define metaPtr$(_ptr, _ti) \
    ((u_P$T){ .ptr = (TypeOf(u8[0])*)(_ptr), .ti = (_ti) })

/**
 * Create a meta pointer for a typed value
 */
#define metaPtrOf$(_ptr) \
    metaPtr$((_ptr), typeInfo$(TypeOf(*(_ptr))))

/* ============================================================================
 * Function 1: Convert byte slice to struct meta pointer
 *
 * Preconditions (unreachable if violated):
 * - bytes.len >= struct size
 * - bytes.ptr != NULL
 * - fields.len > 0
 * ============================================================================ */

__attribute__((always_inline)) static inline u_P$T
metaPtr_fromBytes(const S$u8 bytes, const S_const$TypeInfo fields) {
    if (fields.len == 0) __builtin_unreachable();

    const TypeInfo struct_ti = TypeInfo_struct(fields);

    if (bytes.len < struct_ti.size) __builtin_unreachable();
    if (bytes.ptr == (u8*)0) __builtin_unreachable();

    return (u_P$T){
        .ptr = (TypeOf(u8[0])*)bytes.ptr,
        .ti = struct_ti
    };
}

/* ============================================================================
 * Function 2: Get slice of field meta pointers
 *
 * Preconditions (unreachable if violated):
 * - out.len >= fields.len
 * - out.ptr != NULL
 * - parent.ptr != NULL
 * - fields.len > 0
 *
 * Returns: Subslice of out with len = fields.len
 * ============================================================================ */

__attribute__((always_inline)) static inline S$u_P$T
metaPtr_fields(const u_P$T parent, const S_const$TypeInfo fields, const S$u_P$T out) {
    if (fields.len == 0) __builtin_unreachable();
    if (out.len < fields.len) __builtin_unreachable();
    if (out.ptr == (u_P$T*)0) __builtin_unreachable();
    if (parent.ptr == (TypeOf(u8[0])*)0) __builtin_unreachable();

    usize end = 0;

    for (usize i = 0; i < fields.len; ++i) {
        const usize align_val = alignVal$(fields.ptr[i].align);
        const usize offset = alignUp$(end, align_val);

        out.ptr[i] = (u_P$T){
            .ptr = (TypeOf(u8[0])*)((u8*)parent.ptr + offset),
            .ti = fields.ptr[i]
        };

        end = offset + fields.ptr[i].size;
    }

    return (S$u_P$T){
        .ptr = out.ptr,
        .len = fields.len
    };
}

/* ============================================================================
 * Function 3: Get specific field meta pointer
 *
 * Preconditions (unreachable if violated):
 * - field_idx < fields.len
 * - parent.ptr != NULL
 * ============================================================================ */

__attribute__((always_inline)) static inline u_P$T
metaPtr_field(const u_P$T parent, const S_const$TypeInfo fields, const usize field_idx) {
    if (field_idx >= fields.len) __builtin_unreachable();
    if (parent.ptr == (TypeOf(u8[0])*)0) __builtin_unreachable();

    const usize offset = TypeInfo_fieldOffset(fields, field_idx);

    return (u_P$T){
        .ptr = (TypeOf(u8[0])*)((u8*)parent.ptr + offset),
        .ti = fields.ptr[field_idx]
    };
}

/**
 * Get field with explicit TypeInfo verification
 *
 * Preconditions (unreachable if violated):
 * - field_idx < fields.len
 * - parent.ptr != NULL
 * - fields.ptr[field_idx].packed == field_ti.packed (type must match)
 */
__attribute__((always_inline)) static inline u_P$T
metaPtr_fieldTyped(const u_P$T parent, const S_const$TypeInfo fields, const TypeInfo field_ti, const usize field_idx) {
    if (field_idx >= fields.len) __builtin_unreachable();
    if (parent.ptr == (TypeOf(u8[0])*)0) __builtin_unreachable();
    if (fields.ptr[field_idx].packed != field_ti.packed) __builtin_unreachable();

    const usize offset = TypeInfo_fieldOffset(fields, field_idx);

    return (u_P$T){
        .ptr = (TypeOf(u8[0])*)((u8*)parent.ptr + offset),
        .ti = field_ti
    };
}

/* ============================================================================
 * Function 4: Get parent struct from field meta pointer
 *
 * Preconditions (unreachable if violated):
 * - field_idx < fields.len
 * - field.ptr != NULL
 * - field.ti.packed == fields.ptr[field_idx].packed (type must match)
 * ============================================================================ */

__attribute__((always_inline)) static inline u_P$T
metaPtr_parent(const u_P$T field, const S_const$TypeInfo fields, const usize field_idx) {
    if (field_idx >= fields.len) __builtin_unreachable();
    if (field.ptr == (TypeOf(u8[0])*)0) __builtin_unreachable();
    if (field.ti.packed != fields.ptr[field_idx].packed) __builtin_unreachable();

    const usize offset = TypeInfo_fieldOffset(fields, field_idx);
    const TypeInfo struct_ti = TypeInfo_struct(fields);

    return (u_P$T){
        .ptr = (TypeOf(u8[0])*)((u8*)field.ptr - offset),
        .ti = struct_ti
    };
}

/* ============================================================================
 * Offset-Only Operations (no pointer involved, inherently null-safe)
 *
 * These compute offsets/sizes from TypeInfo alone.
 * ============================================================================ */

__attribute__((always_inline)) static inline usize
metaPtr_offsetOf(const S_const$TypeInfo fields, const usize field_idx) {
    return TypeInfo_fieldOffset(fields, field_idx);
}

__attribute__((always_inline)) static inline usize
metaPtr_sizeOf(const S_const$TypeInfo fields) {
    return TypeInfo_structSize(fields);
}

__attribute__((always_inline)) static inline u32
metaPtr_alignOf(const S_const$TypeInfo fields) {
    return TypeInfo_structAlign(fields);
}

/* ============================================================================
 * Value Access - Read/Write through meta pointers
 *
 * Precondition: mp.ptr != NULL (unreachable otherwise)
 * ============================================================================ */

#define metaPtr_read$(_T, _mp) ({ \
    if ((_mp).ptr == (TypeOf(u8[0])*)0) __builtin_unreachable(); \
    _T _val_; \
    __builtin_memcpy(&_val_, (_mp).ptr, sizeof(_T)); \
    _val_; \
})

#define metaPtr_write$(_mp, _value) \
    do { \
        if ((_mp).ptr == (TypeOf(u8[0])*)0) __builtin_unreachable(); \
        const TypeOf(_value) _tmp_ = (_value); \
        __builtin_memcpy((_mp).ptr, &_tmp_, sizeof(_tmp_)); \
    } while (0)

#define metaPtr_readChecked$(_T, _mp) ({ \
    if ((_mp).ptr == (TypeOf(u8[0])*)0) __builtin_unreachable(); \
    if (sizeof(_T) != (_mp).ti.size) __builtin_unreachable(); \
    _T _val_; \
    __builtin_memcpy(&_val_, (_mp).ptr, sizeof(_T)); \
    _val_; \
})

#define metaPtr_writeChecked$(_mp, _value) \
    do { \
        if ((_mp).ptr == (TypeOf(u8[0])*)0) __builtin_unreachable(); \
        if (sizeof(_value) != (_mp).ti.size) __builtin_unreachable(); \
        const TypeOf(_value) _tmp_ = (_value); \
        __builtin_memcpy((_mp).ptr, &_tmp_, sizeof(_tmp_)); \
    } while (0)

/* ============================================================================
 * Compile-Time Field Access (indices and types are constants)
 *
 * Uses packed TypeInfo for maximum constant folding.
 * No runtime checks - offsets computed at compile time.
 * ============================================================================ */

#define metaPtr_field0$(_parent, _tip0) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_parent).ptr + CT_OFFSET_0$(_tip0)), \
        .ti = { .packed = (_tip0) } })

#define metaPtr_field1$(_parent, _tip0, _tip1) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_parent).ptr + CT_OFFSET_1$(_tip0, _tip1)), \
        .ti = { .packed = (_tip1) } })

#define metaPtr_field2$(_parent, _tip0, _tip1, _tip2) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_parent).ptr + CT_OFFSET_2$(_tip0, _tip1, _tip2)), \
        .ti = { .packed = (_tip2) } })

#define metaPtr_field3$(_parent, _tip0, _tip1, _tip2, _tip3) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_parent).ptr + CT_OFFSET_3$(_tip0, _tip1, _tip2, _tip3)), \
        .ti = { .packed = (_tip3) } })

#define metaPtr_field4$(_parent, _tip0, _tip1, _tip2, _tip3, _tip4) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_parent).ptr + CT_OFFSET_4$(_tip0, _tip1, _tip2, _tip3, _tip4)), \
        .ti = { .packed = (_tip4) } })

/* Compile-time parent recovery */
#define metaPtr_parent0$(_field, _tip0) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_field).ptr - CT_OFFSET_0$(_tip0)), \
        .ti = { .size = CT_STRUCT_SIZE_1$(_tip0), \
                .align = TI_ALIGN$(_tip0) } })

#define metaPtr_parent1$(_field, _tip0, _tip1) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_field).ptr - CT_OFFSET_1$(_tip0, _tip1)), \
        .ti = { .size = CT_STRUCT_SIZE_2$(_tip0, _tip1), \
                .align = CT_MAX_ALIGN2$(_tip0, _tip1) } })

#define metaPtr_parent3$(_field, _tip0, _tip1, _tip2, _tip3) \
    ((u_P$T){ \
        .ptr = (TypeOf(u8[0])*)((u8*)(_field).ptr - CT_OFFSET_3$(_tip0, _tip1, _tip2, _tip3)), \
        .ti = { .size = CT_STRUCT_SIZE_4$(_tip0, _tip1, _tip2, _tip3), \
                .align = CT_MAX_ALIGN4$(_tip0, _tip1, _tip2, _tip3) } })

#endif /* META_PTR_H */
