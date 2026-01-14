/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    meta.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-08 (date of creation)
 * @updated 2025-12-13 (date of last update)
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
/// Compute all field offsets within a struct defined by TypeInfo slice
$attr($inline_always)
$static fn_((u_offsets(S_const$TypeInfo fields, S$usize out))(S$usize));

/// Get specific field meta pointer (immutable)
$attr($inline_always)
$static fn_((u_fieldPtr(u_P_const$raw record, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw));
/// Get specific field meta pointer (mutable)
$attr($inline_always)
$static fn_((u_fieldPtrMut(u_P$raw record, S_const$TypeInfo fields, usize field_idx))(u_P$raw));
/// Get all field meta pointers (immutable)
$attr($inline_always)
$static fn_((u_fieldPtrs(u_P_const$raw record, S_const$TypeInfo fields, S$u_P_const$raw out))(S$u_P_const$raw));
/// Get all field meta pointers (mutable)
$attr($inline_always)
$static fn_((u_fieldPtrsMut(u_P$raw record, S_const$TypeInfo fields, S$u_P$raw out))(S$u_P$raw));
/// Get record struct from field meta pointer (immutable)
$attr($inline_always)
$static fn_((u_recordPtr(u_P_const$raw field, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw));
/// Get record struct from field meta pointer (mutable)
$attr($inline_always)
$static fn_((u_recordPtrMut(u_P$raw field, S_const$TypeInfo fields, usize field_idx))(u_P$raw));

/// Array TypeInfo: T[N]
$attr($inline_always)
$static fn_((u_typeInfoA(usize n, TypeInfo elem))(TypeInfo));
/// Size of T[N]
$attr($inline_always)
$static fn_((u_sizeOfA(usize n, TypeInfo elem))(usize));
/// Alignment of T[N]
$attr($inline_always)
$static fn_((u_alignOfA(usize n, TypeInfo elem))(mem_Align));

// ============================================================================
// N-Replicated Record: {T0[N], T1[N], T2[N], ...}
// Same as existing API but with uniform N for all fields.
// When N=1, equivalent to existing functions.
// ============================================================================

/// TypeInfo for {T0[N], T1[N], ...}
$attr($inline_always)
$static fn_((u_typeInfoRecordN(usize n, S_const$TypeInfo fields))(TypeInfo));
/// Size of {T0[N], T1[N], ...}
$attr($inline_always)
$static fn_((u_sizeOfRecordN(usize n, S_const$TypeInfo fields))(usize));
/// Alignment of record (independent of N)
$attr($inline_always)
$static fn_((u_alignOfRecordN(usize n, S_const$TypeInfo fields))(mem_Align));

/// Offset to field[idx] array within {T0[N], T1[N], ...}
$attr($inline_always)
$static fn_((u_offsetToN(usize n, S_const$TypeInfo fields, usize field_idx))(usize));
/// All offsets at once for {T0[N], T1[N], ...}
$attr($inline_always)
$static fn_((u_offsetsN(usize n, S_const$TypeInfo fields, S$usize out))(S$usize));

/// Get field slice (immutable) from record pointer
$attr($inline_always)
$static fn_((u_fieldSli(u_P_const$raw record, usize n, S_const$TypeInfo fields, usize field_idx))(u_S_const$raw));
/// Get field slice (mutable) from record pointer
$attr($inline_always)
$static fn_((u_fieldSliMut(u_P$raw record, usize n, S_const$TypeInfo fields, usize field_idx))(u_S$raw));
/// Get all field slices (immutable) from record pointer
$attr($inline_always)
$static fn_((u_fieldSlis(u_P_const$raw record, usize n, S_const$TypeInfo fields, S$u_S_const$raw out))(S$u_S_const$raw));
/// Get all field slices (mutable) from record pointer
$attr($inline_always)
$static fn_((u_fieldSlisMut(u_P$raw record, usize n, S_const$TypeInfo fields, S$u_S$raw out))(S$u_S$raw));
/// Get record pointer (immutable) from field pointer
$attr($inline_always)
$static fn_((u_recordNPtr(u_S_const$raw field, usize n, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw));
/// Get record pointer (mutable) from field pointer
$attr($inline_always)
$static fn_((u_recordNPtrMut(u_S$raw field, usize n, S_const$TypeInfo fields, usize field_idx))(u_P$raw));

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
        max_align = prim_max(max_align, TypeInfo_align(*field));
    });
    return (TypeInfo){
        .size = mem_alignFwd(end_offset, mem_log2ToAlign(max_align)),
        .align = max_align
    };
};
$attr($inline_always)
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
$attr($inline_always)
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
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
    debug_assert_fmt(TypeInfo_eq(record, u_typeInfoRecord(fields)), "Type mismatch: record type does not match expected type");
    let_ignore = record;
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
$static fn_((u_offsets(S_const$TypeInfo fields, S$usize out))(S$usize)) {
    claim_assert_nonnull(fields.ptr);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_offset) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        *out_offset = offset;
        end_offset = offset + field->size;
    });
    return out;
};

$attr($inline_always)
$static fn_((u_fieldPtr(u_P_const$raw record, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw)) {
    // claim_assert_nonnull(record.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
    let offset = u_offsetTo(record.type, fields, field_idx);
    return (u_P_const$raw){
        .raw = as$(P_const$raw)(as$(const u8*)(record.raw) + offset),
        .type = *S_at((fields)[field_idx])
    };
};
$attr($inline_always)
$static fn_((u_fieldPtrMut(u_P$raw record, S_const$TypeInfo fields, usize field_idx))(u_P$raw)) {
    // claim_assert_nonnull(record.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
    let offset = u_offsetTo(record.type, fields, field_idx);
    return (u_P$raw){
        .raw = as$(P$raw)(as$(u8*)(record.raw) + offset),
        .type = *S_at((fields)[field_idx])
    };
};
$attr($inline_always)
$static fn_((u_fieldPtrs(u_P_const$raw record, S_const$TypeInfo fields, S$u_P_const$raw out))(S$u_P_const$raw)) {
    claim_assert_nonnull(record.raw);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_field) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        *out_field = lit$((u_P_const$raw){
            .raw = as$(P_const$raw)(as$(const u8*)(record.raw) + offset),
            .type = *field,
        });
        end_offset = offset + field->size;
    });
    return out;
};
$attr($inline_always)
$static fn_((u_fieldPtrsMut(u_P$raw record, S_const$TypeInfo fields, S$u_P$raw out))(S$u_P$raw)) {
    claim_assert_nonnull(record.raw);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_field) {
        let align_val = mem_log2ToAlign(field->align);
        let offset = mem_alignFwd(end_offset, align_val);
        *out_field = lit$((u_P$raw){
            .raw = as$(P$raw)(as$(u8*)(record.raw) + offset),
            .type = *field,
        });
        end_offset = offset + field->size;
    });
    return out;
};
$attr($inline_always)
$static fn_((u_recordPtr(u_P_const$raw field, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw)) {
    // claim_assert_nonnull(field.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
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
    // claim_assert_nonnull(field.raw);
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
    claim_assert_fmt(TypeInfo_eq(field.type, *S_at((fields)[field_idx])), "Type mismatch: field type does not match expected type at index {:uz}", field_idx);
    let offset = u_offsetTo(field.type, fields, field_idx);
    let record_type = u_typeInfoRecord(fields);
    return (u_P$raw){
        .raw = as$(P$raw)(as$(u8*)(field.raw) - offset),
        .type = record_type
    };
};

$attr($inline_always)
$static fn_((u_typeInfoA(usize n, TypeInfo elem))(TypeInfo)) {
    return (TypeInfo){ .size = n * elem.size, .align = elem.align };
};
$attr($inline_always)
$static fn_((u_sizeOfA(usize n, TypeInfo elem))(usize)) {
    return n * elem.size;
};
$attr($inline_always)
$static fn_((u_alignOfA(usize n, TypeInfo elem))(mem_Align)) {
    let_ignore = n;
    return elem.align;
};

$attr($inline_always)
$static fn_((u_typeInfoRecordN(usize n, S_const$TypeInfo fields))(TypeInfo)) {
    claim_assert_nonnull(fields.ptr);
    usize end_offset = 0;
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        end_offset = offset + arr_type.size;
        max_align = prim_max(max_align, as$(mem_Align)(arr_type.align));
    });
    return (TypeInfo){
        .size = mem_alignFwd(end_offset, mem_log2ToAlign(max_align)),
        .align = max_align
    };
};
$attr($inline_always)
$static fn_((u_sizeOfRecordN(usize n, S_const$TypeInfo fields))(usize)) {
    claim_assert_nonnull(fields.ptr);
    usize end_offset = 0;
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        end_offset = offset + arr_type.size;
        max_align = prim_max(max_align, as$(mem_Align)(arr_type.align));
    });
    return mem_alignFwd(end_offset, mem_log2ToAlign(max_align));
};
$attr($inline_always)
$static fn_((u_alignOfRecordN(usize n, S_const$TypeInfo fields))(mem_Align)) {
    claim_assert_nonnull(fields.ptr);
    mem_Align max_align = 0;
    for_(($s(fields))(field) {
        let arr_type = u_typeInfoA(n, *field);
        max_align = prim_max(max_align, as$(mem_Align)(arr_type.align));
    });
    return max_align;
};

$attr($inline_always)
$static fn_((u_offsetToN(usize n, S_const$TypeInfo fields, usize field_idx))(usize)) {
    claim_assert_nonnull(fields.ptr);
    claim_assert_fmt(field_idx < fields.len, "Field index out of bounds: idx({:uz}) >= len({:uz})", field_idx, fields.len);
    usize end_offset = 0;
    for_(($s(fields))(field) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        if (field_idx == 0) { return offset; }
        end_offset = offset + arr_type.size;
        field_idx--;
    });
    return end_offset;
};
$attr($inline_always)
$static fn_((u_offsetsN(usize n, S_const$TypeInfo fields, S$usize out))(S$usize)) {
    claim_assert_nonnull(fields.ptr);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_offset) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        *out_offset = offset;
        end_offset = offset + arr_type.size;
    });
    return out;
};

$attr($inline_always)
$static fn_((u_fieldSli(u_P_const$raw record, usize n, S_const$TypeInfo fields, usize field_idx))(u_S_const$raw)) {
    claim_assert_nonnull(record.raw);
    let offset = u_offsetToN(n, fields, field_idx);
    return (u_S_const$raw){
        .ptr = as$(P_const$raw)(as$(const u8*)(record.raw) + offset),
        .len = n,
        .type = *S_at((fields)[field_idx])
    };
};
$attr($inline_always)
$static fn_((u_fieldSliMut(u_P$raw record, usize n, S_const$TypeInfo fields, usize field_idx))(u_S$raw)) {
    claim_assert_nonnull(record.raw);
    let offset = u_offsetToN(n, fields, field_idx);
    return (u_S$raw){
        .ptr = as$(P$raw)(as$(u8*)(record.raw) + offset),
        .len = n,
        .type = *S_at((fields)[field_idx])
    };
};
$attr($inline_always)
$static fn_((u_fieldSlis(u_P_const$raw record, usize n, S_const$TypeInfo fields, S$u_S_const$raw out))(S$u_S_const$raw)) {
    claim_assert_nonnull(record.raw);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_field) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        *out_field = lit$((u_S_const$raw){
            .ptr = as$(P_const$raw)(as$(const u8*)(record.raw) + offset),
            .len = n,
            .type = *field,
        });
        end_offset = offset + arr_type.size;
    });
    return out;
};
$attr($inline_always)
$static fn_((u_fieldSlisMut(u_P$raw record, usize n, S_const$TypeInfo fields, S$u_S$raw out))(S$u_S$raw)) {
    claim_assert_nonnull(record.raw);
    claim_assert_nonnull(out.ptr);
    claim_assert_fmt(fields.len <= out.len, "fields({:uz}) > out({:uz})", fields.len, out.len);
    usize end_offset = 0;
    for_(($s(fields), $s(out))(field, out_field) {
        let arr_type = u_typeInfoA(n, *field);
        let offset = mem_alignFwd(end_offset, mem_log2ToAlign(arr_type.align));
        *out_field = lit$((u_S$raw){
            .ptr = as$(P$raw)(as$(u8*)(record.raw) + offset),
            .len = n,
            .type = *field,
        });
        end_offset = offset + arr_type.size;
    });
    return out;
};
$static fn_((u_recordNPtr(u_S_const$raw field, usize n, S_const$TypeInfo fields, usize field_idx))(u_P_const$raw)) {
    claim_assert_nonnull(field.ptr);
    claim_assert_fmt(field.len == n, "Field length mismatch: len({:uz}) != n({:uz})", field.len, n);
    claim_assert_nonnull(fields.ptr);
    let offset = u_offsetToN(n, fields, field_idx);
    return (u_P_const$raw){
        .raw = as$(P_const$raw)(as$(const u8*)(field.ptr) - offset),
        .type = u_typeInfoRecordN(n, fields)
    };
};
$attr($inline_always)
$static fn_((u_recordNPtrMut(u_S$raw field, usize n, S_const$TypeInfo fields, usize field_idx))(u_P$raw)) {
    claim_assert_nonnull(field.ptr);
    claim_assert_fmt(field.len == n, "Field length mismatch: len({:uz}) != n({:uz})", field.len, n);
    claim_assert_nonnull(fields.ptr);
    let offset = u_offsetToN(n, fields, field_idx);
    return (u_P$raw){
        .raw = as$(P$raw)(as$(u8*)(field.ptr) - offset),
        .type = u_typeInfoRecordN(n, fields)
    };
};

#if UNUSED_CODE
// ============================================================================
// Scatter/Gather: AoS <-> SoA
// ============================================================================

/// Scatter: records -> field arrays (AoS -> SoA)
$attr($inline_always)
$static fn_((u_scatter(u_S_const$raw records, S_const$TypeInfo fields, S$u_S$raw out))(void)) {
    let ti = u_typeInfoRecord(fields);
    let N = records.len;

    for (usize i = 0; i < N; i++) {
        let base = as$(const u8*)(records.ptr) + (i * ti.size);
        usize end = 0;
        for (usize f = 0; f < fields.len; f++) {
            let ft = fields.ptr[f];
            let offset = mem_alignFwd(end, mem_log2ToAlign(ft.align));
            __builtin_memcpy(as$(u8*)(out.ptr[f].ptr) + (i * ft.size), base + offset, ft.size);
            end = offset + ft.size;
        }
    }
}

/// Gather: field arrays -> records (SoA -> AoS)
$attr($inline_always)
$static fn_((u_gather(S_const$u_S_const$raw arrays, S_const$TypeInfo fields, u_S$raw out))(void)) {
    let ti = u_typeInfoRecord(fields);
    let N = out.len;

    for (usize i = 0; i < N; i++) {
        let base = as$(u8*)(out.ptr) + (i * ti.size);
        usize end = 0;
        for (usize f = 0; f < fields.len; f++) {
            let ft = fields.ptr[f];
            let offset = mem_alignFwd(end, mem_log2ToAlign(ft.align));
            __builtin_memcpy(base + offset, as$(const u8*)(arrays.ptr[f].ptr) + (i * ft.size), ft.size);
            end = offset + ft.size;
        }
    }
}

// ============================================================================
// Strided: zero-copy field view in AoS
// ============================================================================

typedef struct u_Strided {
    P$raw ptr;
    usize len;
    usize stride;
    TypeInfo type;
} u_Strided;

typedef struct u_Strided_const {
    P_const$raw ptr;
    usize len;
    usize stride;
    TypeInfo type;
} u_Strided_const;

/// Get strided view of field[idx] within AoS
$attr($inline_always)
$static fn_((u_strided(u_S$raw records, S_const$TypeInfo fields, usize idx))(u_Strided)) {
    let ti = u_typeInfoRecord(fields);
    let offset = u_offsetTo(ti, fields, idx);
    return (u_Strided){
        .ptr = as$(P$raw)(as$(u8*)(records.ptr) + offset),
        .len = records.len,
        .stride = ti.size,
        .type = fields.ptr[idx]
    };
}

#define u_Strided_at$(_T, _s, _i) (*as$(_T*)(as$(u8*)((_s).ptr) + ((_i) * (_s).stride)))
#define u_Strided_set$(_s, _i, _v) (*as$(TypeOf(_v)*)(as$(u8*)((_s).ptr) + ((_i) * (_s).stride)) = (_v))
#endif /* UNUSED_CODE */

#if UNUSED_CODE
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

#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* meta__included */
