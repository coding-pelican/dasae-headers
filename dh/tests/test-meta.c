#include "dh-main.h"
#include "dh/meta.h"
#include "dh/heap/Page.h"

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

// =============================================================================
// Tests for basic record layout (simulating regular structs)
// =============================================================================

TEST_fn_("u_typeInfoRecord matches actual struct TypeInfo" $scope) {
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

TEST_fn_("u_offsetTo matches actual struct field offsets" $scope) {
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

TEST_fn_("u_offsets matches all actual struct field offsets" $scope) {
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

    for_(($s(computed_offsets), $a(expected_offsets))(computed, expected) {
        try_(TEST_expect(*computed == *expected));
    });
} $unscoped(TEST_fn);

// =============================================================================
// Tests for N-replicated record layout (SoA: struct { T0[N]; T1[N]; ... })
// =============================================================================

TEST_fn_("u_typeInfoRecordN matches actual SoA struct TypeInfo" $scope) {
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

TEST_fn_("u_offsetToN matches actual SoA struct field offsets" $scope) {
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

TEST_fn_("u_offsetsN matches all actual SoA struct field offsets" $scope) {
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

    for_(($s(computed_offsets), $a(expected_offsets))(computed, expected) {
        try_(TEST_expect(*computed == *expected));
    });
} $unscoped(TEST_fn);

// =============================================================================
// Tests for u_fieldSlisMut (slices into N-replicated record)
// =============================================================================

TEST_fn_("u_fieldSlisMut produces correctly positioned slices" $scope) {
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

TEST_fn_("u_fieldSlisMut with heap allocation" $guard) {
    var_(heap, heap_Page) = cleared();
    let gpa = heap_Page_alctr(&heap);

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
