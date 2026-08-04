#include "dh-main.h"
#include "dh/HashMap.h"
#include "dh/Ver.h"

bitfield_((test_core_pri_bitfield__Bitfield64)(u64)(
    (value, u64, 64)
));
claim_assert_static(bitfield_bits_(value, test_core_pri_bitfield__Bitfield64) == 64);
claim_assert_static(bitfield_shift_(value, test_core_pri_bitfield__Bitfield64) == 0);
claim_assert_static(bitfield_mask_(value, test_core_pri_bitfield__Bitfield64) == u64_limit_max);
claim_assert_static(bitfield_bits$(test_core_pri_bitfield__Bitfield64) == 64);
claim_assert_static(bitfield_reserved$(test_core_pri_bitfield__Bitfield64) == 0);
claim_assert_static(bitfield_count$(test_core_pri_bitfield__Bitfield64) == 1);

bitfield_((test_core_pri_bitfield__BitfieldReserved)(u32)(
    (tag, u8, 1),
    (payload, u16, 15)
));
claim_assert_static(bitfield_bits_(tag, test_core_pri_bitfield__BitfieldReserved) == 1);
claim_assert_static(bitfield_shift_(tag, test_core_pri_bitfield__BitfieldReserved) == 15);
claim_assert_static(bitfield_mask_(tag, test_core_pri_bitfield__BitfieldReserved) == 0x00008000u);
claim_assert_static(bitfield_bits_(payload, test_core_pri_bitfield__BitfieldReserved) == 15);
claim_assert_static(bitfield_shift_(payload, test_core_pri_bitfield__BitfieldReserved) == 0);
claim_assert_static(bitfield_mask_(payload, test_core_pri_bitfield__BitfieldReserved) == 0x00007fffu);
claim_assert_static(bitfield_bits$(test_core_pri_bitfield__BitfieldReserved) == 16);
claim_assert_static(bitfield_storage$(test_core_pri_bitfield__BitfieldReserved) == 32);
claim_assert_static(bitfield_reserved$(test_core_pri_bitfield__BitfieldReserved) == 16);
claim_assert_static(bitfield_count$(test_core_pri_bitfield__BitfieldReserved) == 2);

bitfield_((test_core_pri_bitfield__BitfieldReordered)(u32)(
    (payload, u16, 15),
    (tag, u8, 1)
));
claim_assert_static(bitfield_shift_(payload, test_core_pri_bitfield__BitfieldReordered) == 1);
claim_assert_static(bitfield_shift_(tag, test_core_pri_bitfield__BitfieldReordered) == 0);
claim_assert_static(bitfield_mask_(payload, test_core_pri_bitfield__BitfieldReordered) == 0x0000fffeu);
claim_assert_static(bitfield_mask_(tag, test_core_pri_bitfield__BitfieldReordered) == 0x00000001u);
claim_assert_static(sizeOf$(test_core_pri_bitfield__BitfieldReordered) == sizeOf$(u32));

bitfield_((test_core_pri_bitfield__BitfieldResized)(u32)(
    (tag, u8, 3),
    (payload, u16, 13)
));
claim_assert_static(bitfield_shift_(tag, test_core_pri_bitfield__BitfieldResized) == 13);
claim_assert_static(bitfield_shift_(payload, test_core_pri_bitfield__BitfieldResized) == 0);
claim_assert_static(bitfield_mask_(tag, test_core_pri_bitfield__BitfieldResized) == 0x0000e000u);
claim_assert_static(bitfield_mask_(payload, test_core_pri_bitfield__BitfieldResized) == 0x00001fffu);
claim_assert_static(sizeOf$(test_core_pri_bitfield__BitfieldResized) == sizeOf$(u32));

TEST_fn_("core/pri/bitfield: packed unions preserve numeric layout" $scope) {
    let bitfield64 = l$((test_core_pri_bitfield__Bitfield64){ .value = u64_limit_max });
    try_(TEST_expect(bitfield64.packed == u64_limit_max));

    let reserved = l$((test_core_pri_bitfield__BitfieldReserved){
        .tag = 1,
        .payload = 7,
    });
    try_(TEST_expect(reserved.packed == 0x00008007u));

    let reordered = l$((test_core_pri_bitfield__BitfieldReordered){
        .payload = 7,
        .tag = 1,
    });
    let reordered_expected = (as$(u32)(7 << bitfield_shift_(payload, test_core_pri_bitfield__BitfieldReordered)))
                           | (as$(u32)(1 << bitfield_shift_(tag, test_core_pri_bitfield__BitfieldReordered)));
    try_(TEST_expect(reordered.packed == reordered_expected));

    let resized = l$((test_core_pri_bitfield__BitfieldResized){
        .tag = 5,
        .payload = 0x123,
    });
    let resized_expected = (as$(u32)(5 << bitfield_shift_(tag, test_core_pri_bitfield__BitfieldResized)))
                         | (as$(u32)(0x123 << bitfield_shift_(payload, test_core_pri_bitfield__BitfieldResized)));
    try_(TEST_expect(resized.packed == resized_expected));
    let type_info = l$((TypeInfo){
        .size = 5,
        .log2_align = 3,
    });
    let type_info_expected = (as$(TypeInfoPacked)(5 << bitfield_shift_(size, TypeInfo)))
                           | (as$(TypeInfoPacked)(3 << bitfield_shift_(log2_align, TypeInfo)));
    try_(TEST_expect(type_info.packed == type_info_expected));

    let flow_cursor = l$((fn__FlowCursor){
        .curr_line = 7,
        .is_returning = 1,
    });
    let flow_cursor_expected = (as$(fn__FlowCursorPacked)(7 << bitfield_shift_(curr_line, fn__FlowCursor)))
                             | (as$(fn__FlowCursorPacked)(1 << bitfield_shift_(is_returning, fn__FlowCursor)));
    try_(TEST_expect(flow_cursor.packed == flow_cursor_expected));

    let flow_ctrl = l$((Co_FlowCtrl){
        .line = 7,
        .state = Co_State_ready,
    });
    let flow_ctrl_expected = (as$(Co_FlowCtrlPacked)(7 << bitfield_shift_(line, Co_FlowCtrl)))
                           | (as$(Co_FlowCtrlPacked)(Co_State_ready << bitfield_shift_(state, Co_FlowCtrl)));
    try_(TEST_expect(flow_ctrl.packed == flow_ctrl_expected));

    let hash_ctrl = l$((HashMap_Ctrl){
        .fingerprint = 0x12,
        .used = 1,
    });
    let hash_ctrl_expected = (as$(u8)(0x12 << bitfield_shift_(fingerprint, HashMap_Ctrl)))
                           | (as$(u8)(1u << bitfield_shift_(used, HashMap_Ctrl)));
    try_(TEST_expect(hash_ctrl.packed == hash_ctrl_expected));

    let ver = Ver_from(1, 2, 3, Ver_Label_rc, 4);
    try_(TEST_expect(ver.packed == ver_calc(1, 2, 3, ver_label_prefix_as_num_rc, 4)));
} $unscoped(TEST_fn);

/*
 * Contract probe:
 * - source fields are declared in canonical numeric order, MSB -> LSB;
 * - `packed` has the same numeric value on little- and big-endian targets;
 * - the byte representation of `packed` remains native-endian.
 */
bitfield_((test_core_pri_bitfield__EndianContract)(u16)(
    (high, u8, 4),
    (middle, u8, 8),
    (low, u8, 4)
));
claim_assert_static(bitfield_shift_(high, test_core_pri_bitfield__EndianContract) == 12);
claim_assert_static(bitfield_shift_(middle, test_core_pri_bitfield__EndianContract) == 4);
claim_assert_static(bitfield_shift_(low, test_core_pri_bitfield__EndianContract) == 0);
claim_assert_static(bitfield_mask_(high, test_core_pri_bitfield__EndianContract) == 0xf000u);
claim_assert_static(bitfield_mask_(middle, test_core_pri_bitfield__EndianContract) == 0x0ff0u);
claim_assert_static(bitfield_mask_(low, test_core_pri_bitfield__EndianContract) == 0x000fu);

TEST_fn_("core/pri/bitfield: declaration order is canonical MSB to LSB" $scope) {
    let value = l$((test_core_pri_bitfield__EndianContract){
        .high = 0xa,
        .middle = 0xbc,
        .low = 0xd,
    });

    try_(TEST_expect(value.high == 0xa));
    try_(TEST_expect(value.middle == 0xbc));
    try_(TEST_expect(value.low == 0xd));
    try_(TEST_expect(value.packed == 0xabcdu));
} $unscoped(TEST_fn);

TEST_fn_("core/pri/bitfield: packed bytes follow native endian only" $scope) {
    let value = l$((test_core_pri_bitfield__EndianContract){ .packed = 0xabcdu });
    let bytes = ptrCast$((P_const$u8)(&value.packed));

    try_(TEST_expect(value.high == 0xa));
    try_(TEST_expect(value.middle == 0xbc));
    try_(TEST_expect(value.low == 0xd));

    pp_if_(arch_endian_is_little)(
        pp_then_(
            try_(TEST_expect(bytes[0] == 0xcdu));
            try_(TEST_expect(bytes[1] == 0xabu));
        ),
        pp_else_(
            try_(TEST_expect(bytes[0] == 0xabu));
            try_(TEST_expect(bytes[1] == 0xcdu));
        ));
} $unscoped(TEST_fn);
