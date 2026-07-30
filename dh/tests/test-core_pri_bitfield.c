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
claim_assert_static(bitfield_reserved$(test_core_pri_bitfield__BitfieldReserved) == 0);
claim_assert_static(bitfield_count$(test_core_pri_bitfield__BitfieldReserved) == 2);

TEST_fn_("core/pri/bitfield: packed unions preserve numeric layout" $scope) {
    let bitfield64 = l$((test_core_pri_bitfield__Bitfield64){ .value = u64_limit_max });
    try_(TEST_expect(bitfield64.packed == u64_limit_max));

    let reserved = l$((test_core_pri_bitfield__BitfieldReserved){
        .tag = 1,
        .payload = 7,
    });
    try_(TEST_expect(reserved.packed == 0x00008007u));
    let type_info = l$((TypeInfo){
        .size = 5,
        .log2_align = 3,
    });
    let type_info_expected = as$(TypeInfoPacked)(5)
                          | (as$(TypeInfoPacked)(3) << TypeInfo_size_bits);
    try_(TEST_expect(type_info.packed == type_info_expected));

    let flow_cursor = l$((fn__FlowCursor){
        .curr_line = 7,
        .is_returning = 1,
    });
    let flow_cursor_expected = as$(fn__FlowCursorPacked)(7)
                            | (as$(fn__FlowCursorPacked)(1) << bitfield_shift_(curr_line, fn__FlowCursor));
    try_(TEST_expect(flow_cursor.packed == flow_cursor_expected));

    let flow_ctrl = l$((Co_FlowCtrl){
        .line = 7,
        .state = Co_State_ready,
    });
    let flow_ctrl_expected = as$(Co_FlowCtrlPacked)(7)
                          | (as$(Co_FlowCtrlPacked)(Co_State_ready) << bitfield_shift_(line, Co_FlowCtrl));
    try_(TEST_expect(flow_ctrl.packed == flow_ctrl_expected));

    let hash_ctrl = l$((HashMap_Ctrl){
        .fingerprint = 0x12,
        .used = 1,
    });
    let hash_ctrl_expected = as$(u8)(0x12 | (1u << bitfield_shift_(fingerprint, HashMap_Ctrl)));
    try_(TEST_expect(hash_ctrl.packed == hash_ctrl_expected));

    let ver = Ver_from(1, 2, 3, Ver_Label_rc, 4);
    try_(TEST_expect(ver.packed == ver_calc(1, 2, 3, ver_label_prefix_as_num_rc, 4)));
} $unscoped(TEST_fn);
