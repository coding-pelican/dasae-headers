#include "dh-main.h"
#include "dh/u-meta.h"
#include "dh/heap/Sys.h"

// =============================================================================
// Control structs for u_typeInfoRecord / u_offsets / u_offsetTo tests
// These are actual C structs that the meta module should simulate correctly
// =============================================================================

T_alias$((Record$1u8$2u32$3u64)(struct Record$1u8$2u32$3u64 {
    var_(field0, u8);
    var_(field1, u32);
    var_(field2, u64);
}));

T_alias$((Record$1u64$2u8$3u32)(struct Record$1u64$2u8$3u32 {
    var_(field0, u64);
    var_(field1, u8);
    var_(field2, u32);
}));

T_alias$((Record$1P$raw$2usize)(struct Record$1P$raw$2usize {
    var_(field0, P$raw);
    var_(field1, usize);
}));

// =============================================================================
// Control structs for u_typeInfoRecordN / u_offsetsN / u_offsetToN tests
// These are N-replicated records (SoA layout): struct { T0[N]; T1[N]; ... }
// =============================================================================

T_alias$((Record$10$1u8$2u32$3u64)(struct Record$10$1u8$2u32$3u64 {
    var_(field0, A$$(10, u8));
    var_(field1, A$$(10, u32));
    var_(field2, A$$(10, u64));
}));

T_alias$((Record$15$1P$raw$2usize)(struct Record$15$1P$raw$2usize {
    var_(field0, A$$(15, P$raw));
    var_(field1, A$$(15, usize));
}));

T_use$((u32)(u_V, u_P, u_S));

TEST_fn_("meta: u_anyP preserves raw pointer mutability" $scope) {
    var value = as$(u32)(1234);
    let_(mutable_ptr, P$raw) = &value;
    let_(const_ptr, P_const$raw) = &value;

    let mutable_meta = u_anyP(mutable_ptr);
    let const_meta = u_anyP(const_ptr);

    try_(TEST_expect(eqlType$(TypeOf(mutable_meta), u_P$raw)));
    try_(TEST_expect(eqlType$(TypeOf(const_meta), u_P_const$raw)));
    try_(TEST_expect(mutable_meta.raw == mutable_ptr));
    try_(TEST_expect(const_meta.raw == const_ptr));
    try_(TEST_expect(TypeInfo_eql(mutable_meta.type, typeInfo$(void))));
    try_(TEST_expect(TypeInfo_eql(const_meta.type, typeInfo$(const void))));
} $unscoped(TEST_fn);

TEST_fn_("meta: target-aware wrappers preserve requested shape" $scope) {
    var value = as$(u32)(1234);

    let v_meta = u_anyV$((u_V$u32)(value));
    try_(TEST_expect(TypeInfo_eql(v_meta.type, typeInfo$(u32))));
    try_(TEST_expect(*v_meta.inner == value));

    let p_meta = u_anyP$((u_P$u32)(&value));
    try_(TEST_expect(TypeInfo_eql(p_meta.type, typeInfo$(u32))));
    try_(TEST_expect(p_meta.raw == &value));

    var items = A_from$((u32){ 1, 2, 3 });
    let items_s = A_ref$((S$u32)(items));
    let s_meta = u_anyS$((u_S$u32)(items_s));
    try_(TEST_expect(TypeInfo_eql(s_meta.type, typeInfo$(u32))));
    try_(TEST_expect(s_meta.ptr == items_s.ptr));
    try_(TEST_expect(s_meta.len == items_s.len));

    let slices = A_ref$((S_const$S_const$u32)A_from$((S_const$u32){ items_s.as_const, S_slice((items_s.as_const)$r(1, 3)) }));
    let shaped_slices = u_anyS$((u_S_const$S_const$raw)(slices));
    try_(TEST_expect(TypeInfo_eql(shaped_slices.type, typeInfo$(S_const$u32))));
    try_(TEST_expect(shaped_slices.len == slices.len));
} $unscoped(TEST_fn);

TEST_fn_("meta: target-aware meta casts preserve requested shape" $scope) {
    var value = as$(u32)(5678);
    let raw_v = u_anyV(value);
    let typed_v = V_meta$((u_V$u32)(typeInfo$(u32))(&raw_v));
    try_(TEST_expect(TypeInfo_eql(typed_v.type, typeInfo$(u32))));
    try_(TEST_expect(*typed_v.inner == value));

    let typed_p = P_meta$((u_P$u32)(typeInfo$(u32))(&value));
    try_(TEST_expect(TypeInfo_eql(typed_p.type, typeInfo$(u32))));
    try_(TEST_expect(typed_p.raw == &value));

    var items = A_from$((u32){ 5, 6, 7 });
    let items_s = A_ref$((S$u32)(items));
    let typed_s = S_meta$((u_S$u32)(typeInfo$(u32))(items_s));
    try_(TEST_expect(TypeInfo_eql(typed_s.type, typeInfo$(u32))));
    try_(TEST_expect(typed_s.ptr == items_s.ptr));
    try_(TEST_expect(typed_s.len == items_s.len));
} $unscoped(TEST_fn);

TEST_fn_("meta: u_as preserves type while refining wrapper shape" $scope) {
    var value = as$(u32)(9012);

    let raw_v = u_anyV(value);
    let as_v = u_as$((u_V$u32)(raw_v));
    try_(TEST_expect(TypeInfo_eql(as_v.type, raw_v.type)));
    try_(TEST_expect(*as_v.inner == value));

    let raw_p = u_anyP(&value);
    let as_p = u_asP$((u_P$u32)(raw_p));
    try_(TEST_expect(TypeInfo_eql(as_p.type, raw_p.type)));
    try_(TEST_expect(as_p.raw == &value));

    var items = A_from$((u32){ 8, 9, 10, 11 });
    let items_s = A_ref$((S$u32)(items));
    let slices = A_ref$((S_const$S_const$u32)A_from$((S_const$u32){
        items_s.as_const,
        S_slice((items_s.as_const)$r(1, 3)),
    }));
    let raw_slices = u_anyS(slices);
    let as_slices = u_asS$((u_S_const$S_const$raw)(raw_slices));
    try_(TEST_expect(TypeInfo_eql(as_slices.type, raw_slices.type)));
    try_(TEST_expect(as_slices.ptr == raw_slices.ptr));
    try_(TEST_expect(as_slices.len == raw_slices.len));

    let first = *u_castP$((const S_const$u32*)(u_atS(as_slices.as_raw, 0)));
    try_(TEST_expect(first.ptr == items_s.ptr));
    try_(TEST_expect(first.len == items_s.len));
} $unscoped(TEST_fn);

// =============================================================================
// Tests for basic record layout (simulating regular structs)
// =============================================================================

TEST_fn_("meta: u_typeInfoRecord matches actual struct TypeInfo" $scope) {
    /* Test 1: {u8, u32, u64} */ {
        let field_types = typeInfos$(u8, u32, u64);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record$1u8$2u32$3u64);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.log2_align == expected.log2_align));
    }

    /* Test 2: {u64, u8, u32} */ {
        let field_types = typeInfos$(u64, u8, u32);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record$1u64$2u8$3u32);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.log2_align == expected.log2_align));
    }

    /* Test 3: {P$raw, usize} */ {
        let field_types = typeInfos$(P$raw, usize);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record$1P$raw$2usize);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.log2_align == expected.log2_align));
    }
} $unscoped(TEST_fn);

TEST_fn_("meta: u_offsetTo matches actual struct field offsets" $scope) {
    /* Test: {u8, u32, u64} */ {
        let field_types = typeInfos$(u8, u32, u64);
        let record_ti = u_typeInfoRecord(field_types);

        let computed_0 = u_offsetTo(record_ti, field_types, 0);
        let computed_1 = u_offsetTo(record_ti, field_types, 1);
        let computed_2 = u_offsetTo(record_ti, field_types, 2);

        let expected_0 = offsetTo(Record$1u8$2u32$3u64, field0);
        let expected_1 = offsetTo(Record$1u8$2u32$3u64, field1);
        let expected_2 = offsetTo(Record$1u8$2u32$3u64, field2);

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }

    /* Test: {u64, u8, u32} */ {
        let field_types = typeInfos$(u64, u8, u32);
        let record_ti = u_typeInfoRecord(field_types);

        let computed_0 = u_offsetTo(record_ti, field_types, 0);
        let computed_1 = u_offsetTo(record_ti, field_types, 1);
        let computed_2 = u_offsetTo(record_ti, field_types, 2);

        let expected_0 = offsetTo(Record$1u64$2u8$3u32, field0);
        let expected_1 = offsetTo(Record$1u64$2u8$3u32, field1);
        let expected_2 = offsetTo(Record$1u64$2u8$3u32, field2);

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }
} $unscoped(TEST_fn);

TEST_fn_("meta: u_offsets matches all actual struct field offsets" $scope) {
    let field_types = typeInfos$(u8, u32, u64);

    let computed_offsets = u_offsets(
        field_types,
        A_ref$((S$usize)l0$((A$$(3, usize))))
    );

    let expected_offsets = A_from$((usize){
        [0] = offsetTo(Record$1u8$2u32$3u64, field0),
        [1] = offsetTo(Record$1u8$2u32$3u64, field1),
        [2] = offsetTo(Record$1u8$2u32$3u64, field2),
    });

    for_(($s(computed_offsets), $a(expected_offsets))(computed, expected)) {
        try_(TEST_expect(*computed == *expected));
    } $end(for);
} $unscoped(TEST_fn);

// =============================================================================
// Tests for N-replicated record layout (SoA: struct { T0[N]; T1[N]; ... })
// =============================================================================

TEST_fn_("meta: u_typeInfoRecordN matches actual SoA struct TypeInfo" $scope) {
    /* Test: {u8[10], u32[10], u64[10]} */ {
        let field_types = typeInfos$(u8, u32, u64);
        let computed = u_typeInfoRecordN(10, field_types);
        let expected = typeInfo$(Record$10$1u8$2u32$3u64);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.log2_align == expected.log2_align));
    }

    /* Test: {P$raw[15], usize[15]} */ {
        let field_types = typeInfos$(P$raw, usize);
        let computed = u_typeInfoRecordN(15, field_types);
        let expected = typeInfo$(Record$15$1P$raw$2usize);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.log2_align == expected.log2_align));
    }
} $unscoped(TEST_fn);

TEST_fn_("meta: u_offsetToN matches actual SoA struct field offsets" $scope) {
    // Test: {u8[10], u32[10], u64[10]}
    {
        let field_types = typeInfos$(u8, u32, u64);

        let computed_0 = u_offsetToN(10, field_types, 0);
        let computed_1 = u_offsetToN(10, field_types, 1);
        let computed_2 = u_offsetToN(10, field_types, 2);

        let expected_0 = offsetTo(Record$10$1u8$2u32$3u64, field0);
        let expected_1 = offsetTo(Record$10$1u8$2u32$3u64, field1);
        let expected_2 = offsetTo(Record$10$1u8$2u32$3u64, field2);

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }

    // Test: {P$raw[15], usize[15]}
    {
        let field_types = typeInfos$(P$raw, usize);

        let computed_0 = u_offsetToN(15, field_types, 0);
        let computed_1 = u_offsetToN(15, field_types, 1);

        let expected_0 = offsetTo(Record$15$1P$raw$2usize, field0);
        let expected_1 = offsetTo(Record$15$1P$raw$2usize, field1);

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
    }
} $unscoped(TEST_fn);

TEST_fn_("meta: u_offsetsN matches all actual SoA struct field offsets" $scope) {
    let field_types = typeInfos$(u8, u32, u64);

    let computed_offsets = u_offsetsN(
        10, field_types,
        A_ref$((S$usize)l0$((A$$(3, usize))))
    );

    let expected_offsets = A_from$((usize){
        [0] = offsetTo(Record$10$1u8$2u32$3u64, field0),
        [1] = offsetTo(Record$10$1u8$2u32$3u64, field1),
        [2] = offsetTo(Record$10$1u8$2u32$3u64, field2),
    });

    for_(($s(computed_offsets), $a(expected_offsets))(computed, expected)) {
        try_(TEST_expect(*computed == *expected));
    } $end(for);
} $unscoped(TEST_fn);

// =============================================================================
// Tests for u_fieldSlisMut (slices into N-replicated record)
// =============================================================================

TEST_fn_("meta: u_fieldSlisMut produces correctly positioned slices" $scope) {
    let field_types = typeInfos$(u8, u32, u64);
    let_(N, usize) = 10;

    // Use actual SoA struct as reference
    var_(control, Record$10$1u8$2u32$3u64) = cleared();

    // Compute using meta module
    let record_ptr = u_anyP(&control);
    let fields_out = A_ref$((S$u_S$raw)l0$((A$$(3, u_S$raw))));
    u_fieldSlisMut(record_ptr, N, field_types, fields_out);

    let sli_0 = *S_at((fields_out)[0]);
    let sli_1 = *S_at((fields_out)[1]);
    let sli_2 = *S_at((fields_out)[2]);

    // Compare slice pointers against actual struct field addresses
    let expected_0 = ptrToInt(&control.field0);
    let expected_1 = ptrToInt(&control.field1);
    let expected_2 = ptrToInt(&control.field2);

    try_(TEST_expect(ptrToInt(sli_0.ptr) == expected_0));
    try_(TEST_expect(ptrToInt(sli_1.ptr) == expected_1));
    try_(TEST_expect(ptrToInt(sli_2.ptr) == expected_2));

    // Verify lengths
    try_(TEST_expect(sli_0.len == N));
    try_(TEST_expect(sli_1.len == N));
    try_(TEST_expect(sli_2.len == N));
} $unscoped(TEST_fn);

TEST_fn_("meta: u_fieldSlisMut with heap allocation" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);

    let field_types = typeInfos$(P$raw, usize);
    let_(N, usize) = 15;

    let computed_ti = u_typeInfoRecordN(N, field_types);
    let expected_ti = typeInfo$(Record$15$1P$raw$2usize);

    // Verify computed size matches actual struct
    try_(TEST_expect(computed_ti.size == expected_ti.size));

    let field_mem = try_(mem_Alctr_create($trace gpa, computed_ti));
    defer_(mem_Alctr_destroy($trace gpa, field_mem));

    let fields = u_fieldSlisMut(field_mem, N, field_types, A_ref$((S$u_S$raw)l0$((A$$(2, u_S$raw)))));

    let sli_0 = *S_at((fields)[0]);
    let sli_1 = *S_at((fields)[1]);

    // Verify slice offsets match expected struct offsets
    let base = ptrToInt(field_mem.raw);
    let offset_0 = ptrToInt(sli_0.ptr) - base;
    let offset_1 = ptrToInt(sli_1.ptr) - base;

    let expected_off_0 = offsetTo(Record$15$1P$raw$2usize, field0);
    let expected_off_1 = offsetTo(Record$15$1P$raw$2usize, field1);

    try_(TEST_expect(offset_0 == expected_off_0));
    try_(TEST_expect(offset_1 == expected_off_1));

    // Verify lengths
    try_(TEST_expect(sli_0.len == N));
    try_(TEST_expect(sli_1.len == N));
} $unguarded(TEST_fn);
