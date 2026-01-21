#include "dh/main.h"
#include "dh/meta.h"
#include "dh/heap/Page.h"

// =============================================================================
// Control structs for u_typeInfoRecord / u_offsets / u_offsetTo tests
// These are actual C structs that the meta module should simulate correctly
// =============================================================================

typedef struct Record_u8_u32_u64 {
    u8 field0;
    u32 field1;
    u64 field2;
} Record_u8_u32_u64;

typedef struct Record_u64_u8_u32 {
    u64 field0;
    u8 field1;
    u32 field2;
} Record_u64_u8_u32;

typedef struct Record_ptr_usize {
    void* field0;
    usize field1;
} Record_ptr_usize;

// =============================================================================
// Control structs for u_typeInfoRecordN / u_offsetsN / u_offsetToN tests
// These are N-replicated records (SoA layout): struct { T0[N]; T1[N]; ... }
// =============================================================================

typedef struct RecordN_u8_u32_u64 {
    A$$(10, u8) field0;
    A$$(10, u32) field1;
    A$$(10, u64) field2;
} RecordN_u8_u32_u64;

typedef struct RecordN_ptr_usize {
    A$$(15, void*) field0;
    A$$(15, usize) field1;
} RecordN_ptr_usize;

// =============================================================================
// Tests for basic record layout (simulating regular structs)
// =============================================================================

TEST_fn_("u_typeInfoRecord matches actual struct TypeInfo" $scope) {
    // Test 1: {u8, u32, u64}
    {
        let field_types = typeInfos$(u8, u32, u64);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record_u8_u32_u64);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.align == expected.align));
    }

    // Test 2: {u64, u8, u32}
    {
        let field_types = typeInfos$(u64, u8, u32);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record_u64_u8_u32);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.align == expected.align));
    }

    // Test 3: {void*, usize}
    {
        let field_types = typeInfos$(void*, usize);
        let computed = u_typeInfoRecord(field_types);
        let expected = typeInfo$(Record_ptr_usize);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.align == expected.align));
    }
} $unscoped_(TEST_fn);

TEST_fn_("u_offsetTo matches actual struct field offsets" $scope) {
    // Test: {u8, u32, u64}
    {
        let field_types = typeInfos$(u8, u32, u64);
        let record_ti = u_typeInfoRecord(field_types);

        usize computed_0 = u_offsetTo(record_ti, field_types, 0);
        usize computed_1 = u_offsetTo(record_ti, field_types, 1);
        usize computed_2 = u_offsetTo(record_ti, field_types, 2);

        usize expected_0 = as$(usize)(offsetTo(Record_u8_u32_u64, field0));
        usize expected_1 = as$(usize)(offsetTo(Record_u8_u32_u64, field1));
        usize expected_2 = as$(usize)(offsetTo(Record_u8_u32_u64, field2));

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }

    // Test: {u64, u8, u32}
    {
        let field_types = typeInfos$(u64, u8, u32);
        let record_ti = u_typeInfoRecord(field_types);

        usize computed_0 = u_offsetTo(record_ti, field_types, 0);
        usize computed_1 = u_offsetTo(record_ti, field_types, 1);
        usize computed_2 = u_offsetTo(record_ti, field_types, 2);

        usize expected_0 = as$(usize)(offsetTo(Record_u64_u8_u32, field0));
        usize expected_1 = as$(usize)(offsetTo(Record_u64_u8_u32, field1));
        usize expected_2 = as$(usize)(offsetTo(Record_u64_u8_u32, field2));

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }
} $unscoped_(TEST_fn);

TEST_fn_("u_offsets matches all actual struct field offsets" $scope) {
    let field_types = typeInfos$(u8, u32, u64);

    var_(offsets, A$$(3, usize)) = A_zero();
    u_offsets(field_types, A_ref$((S$usize)(offsets)));

    usize expected_0 = as$(usize)(offsetTo(Record_u8_u32_u64, field0));
    usize expected_1 = as$(usize)(offsetTo(Record_u8_u32_u64, field1));
    usize expected_2 = as$(usize)(offsetTo(Record_u8_u32_u64, field2));

    try_(TEST_expect(*A_at((offsets)[0]) == expected_0));
    try_(TEST_expect(*A_at((offsets)[1]) == expected_1));
    try_(TEST_expect(*A_at((offsets)[2]) == expected_2));
} $unscoped_(TEST_fn);

// =============================================================================
// Tests for N-replicated record layout (SoA: struct { T0[N]; T1[N]; ... })
// =============================================================================

TEST_fn_("u_typeInfoRecordN matches actual SoA struct TypeInfo" $scope) {
    // Test: {u8[10], u32[10], u64[10]}
    {
        let field_types = typeInfos$(u8, u32, u64);
        let computed = u_typeInfoRecordN(10, field_types);
        let expected = typeInfo$(RecordN_u8_u32_u64);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.align == expected.align));
    }

    // Test: {void*[15], usize[15]}
    {
        let field_types = typeInfos$(void*, usize);
        let computed = u_typeInfoRecordN(15, field_types);
        let expected = typeInfo$(RecordN_ptr_usize);

        try_(TEST_expect(computed.size == expected.size));
        try_(TEST_expect(computed.align == expected.align));
    }
} $unscoped_(TEST_fn);

TEST_fn_("u_offsetToN matches actual SoA struct field offsets" $scope) {
    // Test: {u8[10], u32[10], u64[10]}
    {
        let field_types = typeInfos$(u8, u32, u64);

        usize computed_0 = u_offsetToN(10, field_types, 0);
        usize computed_1 = u_offsetToN(10, field_types, 1);
        usize computed_2 = u_offsetToN(10, field_types, 2);

        usize expected_0 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field0));
        usize expected_1 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field1));
        usize expected_2 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field2));

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
        try_(TEST_expect(computed_2 == expected_2));
    }

    // Test: {void*[15], usize[15]}
    {
        let field_types = typeInfos$(void*, usize);

        usize computed_0 = u_offsetToN(15, field_types, 0);
        usize computed_1 = u_offsetToN(15, field_types, 1);

        usize expected_0 = as$(usize)(offsetTo(RecordN_ptr_usize, field0));
        usize expected_1 = as$(usize)(offsetTo(RecordN_ptr_usize, field1));

        try_(TEST_expect(computed_0 == expected_0));
        try_(TEST_expect(computed_1 == expected_1));
    }
} $unscoped_(TEST_fn);

TEST_fn_("u_offsetsN matches all actual SoA struct field offsets" $scope) {
    let field_types = typeInfos$(u8, u32, u64);

    var_(offsets, A$$(3, usize)) = A_zero();
    u_offsetsN(10, field_types, A_ref$((S$usize)(offsets)));

    usize expected_0 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field0));
    usize expected_1 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field1));
    usize expected_2 = as$(usize)(offsetTo(RecordN_u8_u32_u64, field2));

    try_(TEST_expect(*A_at((offsets)[0]) == expected_0));
    try_(TEST_expect(*A_at((offsets)[1]) == expected_1));
    try_(TEST_expect(*A_at((offsets)[2]) == expected_2));
} $unscoped_(TEST_fn);

// =============================================================================
// Tests for u_fieldSlisMut (slices into N-replicated record)
// =============================================================================

TEST_fn_("u_fieldSlisMut produces correctly positioned slices" $scope) {
    let field_types = typeInfos$(u8, u32, u64);
    const usize N = 10;

    // Use actual SoA struct as reference
    var_(control, RecordN_u8_u32_u64) = cleared();

    // Compute using meta module
    let record_ptr = u_anyP(&control);
    let fields_out = A_ref$((S$u_S$raw)((A$$(3, u_S$raw)){}));
    u_fieldSlisMut(record_ptr, N, field_types, fields_out);

    let sli_0 = *S_at((fields_out)[0]);
    let sli_1 = *S_at((fields_out)[1]);
    let sli_2 = *S_at((fields_out)[2]);

    // Compare slice pointers against actual struct field addresses
    usize expected_0 = as$(usize)(&control.field0);
    usize expected_1 = as$(usize)(&control.field1);
    usize expected_2 = as$(usize)(&control.field2);

    try_(TEST_expect(as$(usize)(sli_0.ptr) == expected_0));
    try_(TEST_expect(as$(usize)(sli_1.ptr) == expected_1));
    try_(TEST_expect(as$(usize)(sli_2.ptr) == expected_2));

    // Verify lengths
    try_(TEST_expect(sli_0.len == N));
    try_(TEST_expect(sli_1.len == N));
    try_(TEST_expect(sli_2.len == N));
} $unscoped_(TEST_fn);

TEST_fn_("u_fieldSlisMut with heap allocation" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);

    let field_types = typeInfos$(void*, usize);
    const usize N = 15;

    let computed_ti = u_typeInfoRecordN(N, field_types);
    let expected_ti = typeInfo$(RecordN_ptr_usize);

    // Verify computed size matches actual struct
    try_(TEST_expect(computed_ti.size == expected_ti.size));

    let field_mem = try_(mem_Allocator_create(gpa, computed_ti));
    defer_(mem_Allocator_destroy(gpa, field_mem));

    let fields = u_fieldSlisMut(field_mem, N, field_types, A_ref$((S$u_S$raw)((A$$(2, u_S$raw)){})));

    let sli_0 = *S_at((fields)[0]);
    let sli_1 = *S_at((fields)[1]);

    // Verify slice offsets match expected struct offsets
    usize base = as$(usize)(field_mem.raw);
    usize offset_0 = as$(usize)(sli_0.ptr) - base;
    usize offset_1 = as$(usize)(sli_1.ptr) - base;

    usize expected_off_0 = as$(usize)(offsetTo(RecordN_ptr_usize, field0));
    usize expected_off_1 = as$(usize)(offsetTo(RecordN_ptr_usize, field1));

    try_(TEST_expect(offset_0 == expected_off_0));
    try_(TEST_expect(offset_1 == expected_off_1));

    // Verify lengths
    try_(TEST_expect(sli_0.len == N));
    try_(TEST_expect(sli_1.len == N));
} $unguarded_(TEST_fn);
