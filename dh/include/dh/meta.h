/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    meta.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-08 (date of creation)
 * @updated 2025-12-08 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  u
 */

#ifndef meta__included
#define meta__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/common.h"

/*========== Macros and Declarations =========================================*/

/// Compute record TypeInfo from field TypeInfos
$attr($inline_always)
$static fn_((u_typeInfoRecord(S_const$TypeInfo fields))(TypeInfo));
/// Compute record size from field TypeInfos
$attr($inline_always)
$static fn_((u_sizeOfRecord(S_const$TypeInfo fields))(usize));
/// Compute record alignment (log2) from field TypeInfos
$attr($inline_always)
$static fn_((u_alignOfRecord(S_const$TypeInfo fields))(mem_Align));

/// Compute field offset within a struct defined by TypeInfo slice
$attr($inline_always)
$static fn_((u_offsetTo(TypeInfo record, S_const$TypeInfo fields, usize field_idx))(usize));

/// Get specific field meta pointer (immutable)
$attr($inline_always)
$static fn_((u_fieldPtr(u_P_const$raw record, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw));
/// Get specific field meta pointer (mutable)
$attr($inline_always)
$static fn_((u_fieldPtrMut(u_P$raw record, S_const$TypeInfo fields, usize field_idx))(u_P$raw));
/// Get record struct from field meta pointer (immutable)
$attr($inline_always)
$static fn_((u_recordPtr(u_P_const$raw field, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw));
/// Get record struct from field meta pointer (mutable)
$attr($inline_always)
$static fn_((u_recordPtrMut(u_P$raw field, S_const$TypeInfo fields, usize field_idx))(u_P$raw));

/*========== Macros and Definitions =========================================*/

$attr($inline_always)
$static fn_((u_typeInfoRecord(S_const$TypeInfo fields))(TypeInfo)) {
    claim_assert_nonnull(fields.ptr);
    usize end_offset = 0;
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        end_offset = offset + field->size;
        max_align = prim_max(max_align, as$(mem_Align)(field->align));
    });
    return (TypeInfo){
        .size = mem_alignFwd(end_offset, mem_log2ToAlign(max_align)),
        .align = max_align
    };
};
$inline_always
$static fn_((u_sizeOfRecord(S_const$TypeInfo fields))(usize)) {
    claim_assert_nonnull(fields.ptr);
    usize end_offset = 0;
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        end_offset = offset + field->size;
        max_align = prim_max(max_align, as$(mem_Align)(field->align));
    });
    return mem_alignFwd(end_offset, mem_log2ToAlign(max_align));
};
$inline_always
$static fn_((u_alignOfRecord(S_const$TypeInfo fields))(mem_Align)) {
    claim_assert_nonnull(fields.ptr);
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        max_align = prim_max(max_align, as$(mem_Align)(field->align));
    });
    return max_align;
};

$attr($inline_always)
$static fn_((u_offsetTo(TypeInfo record, S_const$TypeInfo fields, usize field_idx))(usize)) {
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx(%zu) >= len(%zu)", field_idx, fields.len);
    debug_assert_fmt(TypeInfo_eq(record, u_typeInfoRecord(fields)), "Type mismatch: record type does not match expected type");
    usize end_offset = 0;
    for_(($s(fields))(field) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        if (field_idx == 0) { return offset; }
        end_offset = offset + field->size;
        field_idx--;
    });
    return end_offset;
};

$attr($inline_always)
$static fn_((u_fieldPtr(u_P_const$raw record, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw)) {
    // claim_assert_nonnull(record.raw);
    // claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx(%zu) >= len(%zu)", field_idx, fields.len);
    let offset = u_offsetTo(record.type, fields, field_idx);
    return (u_P_const$raw){
        .raw = as$(P_const$raw)(as$(const u8*)(record.raw) + offset),
        .type = *S_at((fields)[field_idx])
    };
};
$attr($inline_always)
$static fn_((u_fieldPtrMut(u_P$raw record, S_const$TypeInfo fields, usize field_idx))(u_P$raw)) {
    // claim_assert_nonnull(record.raw);
    // claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx(%zu) >= len(%zu)", field_idx, fields.len);
    let offset = u_offsetTo(record.type, fields, field_idx);
    return (u_P$raw){
        .raw = as$(P$raw)(as$(u8*)(record.raw) + offset),
        .type = *S_at((fields)[field_idx])
    };
};

$attr($inline_always)
$static fn_((u_recordPtr(u_P_const$raw field, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw)) {
    claim_assert_nonnull(field.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx(%zu) >= len(%zu)", field_idx, fields.len);
    claim_assert_fmt(TypeInfo_eq(field.type, *S_at((fields)[field_idx])), "Type mismatch: field type does not match expected type at index %zu", field_idx);
    let offset = u_offsetTo(field.type, fields, field_idx);
    let record_type = u_typeInfoRecord(fields);
    return (u_P_const$raw){
        .raw = as$(P_const$raw)(as$(const u8*)(field.raw) - offset),
        .type = record_type
    };
};
$attr($inline_always)
$static fn_((u_recordPtrMut(u_P$raw field, S_const$TypeInfo fields, usize field_idx))(u_P$raw)) {
    claim_assert_nonnull(field.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx(%zu) >= len(%zu)", field_idx, fields.len);
    claim_assert_fmt(TypeInfo_eq(field.type, *S_at((fields)[field_idx])), "Type mismatch: field type does not match expected type at index %zu", field_idx);
    let offset = u_offsetTo(field.type, fields, field_idx);
    let record_type = u_typeInfoRecord(fields);
    return (u_P$raw){
        .raw = as$(P$raw)(as$(u8*)(field.raw) - offset),
        .type = record_type
    };
};

// ============================================================================
// Compile-Time Field Access (using packed TypeInfo)
// ============================================================================

// Immutable field access (constant-folded offsets)
#define u_fieldP0(_parent, _tip0) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_parent).raw) + StructLayoutPacked_offset0(_tip0)), \
        .type = TypeInfo_unpack(_tip0) })

#define u_fieldP1(_parent, _tip0, _tip1) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_parent).raw) + StructLayoutPacked_offset1(_tip0, _tip1)), \
        .type = TypeInfo_unpack(_tip1) })

#define u_fieldP2(_parent, _tip0, _tip1, _tip2) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_parent).raw) + StructLayoutPacked_offset2(_tip0, _tip1, _tip2)), \
        .type = TypeInfo_unpack(_tip2) })

#define u_fieldP3(_parent, _tip0, _tip1, _tip2, _tip3) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_parent).raw) + StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3)), \
        .type = TypeInfo_unpack(_tip3) })

#define u_fieldP4(_parent, _tip0, _tip1, _tip2, _tip3, _tip4) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_parent).raw) + StructLayoutPacked_offset4(_tip0, _tip1, _tip2, _tip3, _tip4)), \
        .type = TypeInfo_unpack(_tip4) })

// Mutable field access (constant-folded offsets)
#define u_fieldP0Mut(_parent, _tip0) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_parent).raw) + StructLayoutPacked_offset0(_tip0)), \
        .type = TypeInfo_unpack(_tip0) })

#define u_fieldP1Mut(_parent, _tip0, _tip1) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_parent).raw) + StructLayoutPacked_offset1(_tip0, _tip1)), \
        .type = TypeInfo_unpack(_tip1) })

#define u_fieldP2Mut(_parent, _tip0, _tip1, _tip2) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_parent).raw) + StructLayoutPacked_offset2(_tip0, _tip1, _tip2)), \
        .type = TypeInfo_unpack(_tip2) })

#define u_fieldP3Mut(_parent, _tip0, _tip1, _tip2, _tip3) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_parent).raw) + StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3)), \
        .type = TypeInfo_unpack(_tip3) })

#define u_fieldP4Mut(_parent, _tip0, _tip1, _tip2, _tip3, _tip4) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_parent).raw) + StructLayoutPacked_offset4(_tip0, _tip1, _tip2, _tip3, _tip4)), \
        .type = TypeInfo_unpack(_tip4) })

// Immutable record recovery (constant-folded offsets)
#define u_parentP0(_field, _tip0) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_field).raw) - StructLayoutPacked_offset0(_tip0)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize1(_tip0), \
                            .align = TypeInfoPacked_align(_tip0) } })

#define u_parentP1(_field, _tip0, _tip1) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_field).raw) - StructLayoutPacked_offset1(_tip0, _tip1)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize2(_tip0, _tip1), \
                            .align = StructLayoutPacked_maxAlign2(_tip0, _tip1) } })

#define u_parentP2(_field, _tip0, _tip1, _tip2) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_field).raw) - StructLayoutPacked_offset2(_tip0, _tip1, _tip2)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize3(_tip0, _tip1, _tip2), \
                            .align = StructLayoutPacked_maxAlign2(StructLayoutPacked_maxAlign2(_tip0, _tip1) > TypeInfoPacked_align(_tip2) ? (_tip0) : packTypeInfo$(u8[1ull << TypeInfoPacked_align(_tip2)]), (_tip1)) } })

#define u_parentP3(_field, _tip0, _tip1, _tip2, _tip3) \
    ((u_P_const$raw){ \
        .raw = as$(P_const$raw)(as$(const u8*)((_field).raw) - StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize4(_tip0, _tip1, _tip2, _tip3), \
                            .align = StructLayoutPacked_maxAlign4(_tip0, _tip1, _tip2, _tip3) } })

// Mutable record recovery (constant-folded offsets)
#define u_parentP0Mut(_field, _tip0) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_field).raw) - StructLayoutPacked_offset0(_tip0)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize1(_tip0), \
                            .align = TypeInfoPacked_align(_tip0) } })

#define u_parentP1Mut(_field, _tip0, _tip1) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_field).raw) - StructLayoutPacked_offset1(_tip0, _tip1)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize2(_tip0, _tip1), \
                            .align = StructLayoutPacked_maxAlign2(_tip0, _tip1) } })

#define u_parentP2Mut(_field, _tip0, _tip1, _tip2) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_field).raw) - StructLayoutPacked_offset2(_tip0, _tip1, _tip2)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize3(_tip0, _tip1, _tip2), \
                            .align = StructLayoutPacked_maxAlign2(StructLayoutPacked_maxAlign2(_tip0, _tip1) > TypeInfoPacked_align(_tip2) ? (_tip0) : packTypeInfo$(u8[1ull << TypeInfoPacked_align(_tip2)]), (_tip1)) } })

#define u_parentP3Mut(_field, _tip0, _tip1, _tip2, _tip3) \
    ((u_P$raw){ \
        .raw = as$(P$raw)(as$(u8*)((_field).raw) - StructLayoutPacked_offset3(_tip0, _tip1, _tip2, _tip3)), \
        .type = (TypeInfo){ .size = StructLayoutPacked_structSize4(_tip0, _tip1, _tip2, _tip3), \
                            .align = StructLayoutPacked_maxAlign4(_tip0, _tip1, _tip2, _tip3) } })


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* meta__included */
