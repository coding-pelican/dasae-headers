#include "dh-main.h"
#include "dh/HashMap.h"
#include "dh/Ver.h"

TEST_fn_("packed bitfield unions preserve numeric layout" $scope) {
    let type_info = l$((TypeInfo){
        .size = 5,
        .log2_align = 3,
    });
    let type_info_expected = as$(TypeInfoPacked)(5)
                          | (as$(TypeInfoPacked)(3) << TypeInfo_size_bits);
    try_(TEST_expect(type_info.packed == type_info_expected));

    let flow_cursor = l$((struct fn__FlowCursor){
        .curr_line = 7,
        .is_returning = 1,
    });
    let flow_cursor_expected = as$(fn__FlowCursorPacked)(7)
                            | (as$(fn__FlowCursorPacked)(1) << fn__FlowCursor_line_bits);
    try_(TEST_expect(flow_cursor.packed == flow_cursor_expected));

    let flow_ctrl = l$((Co_FlowCtrl){
        .line = 7,
        .state = Co_State_ready,
    });
    let flow_ctrl_expected = as$(Co_FlowCtrlPacked)(7)
                          | (as$(Co_FlowCtrlPacked)(Co_State_ready) << Co_FlowCtrl_line_bits);
    try_(TEST_expect(flow_ctrl.packed == flow_ctrl_expected));

    let hash_ctrl = l$((HashMap_Ctrl){
        .fingerprint = 0x12,
        .used = 1,
    });
    let hash_ctrl_expected = as$(u8)(0x12 | (1u << HashMap_Ctrl_fingerprint_bits));
    try_(TEST_expect(hash_ctrl.bits == hash_ctrl_expected));

    let ver = Ver_from(1, 2, 3, Ver_Label_rc, 4);
    try_(TEST_expect(ver.packed == ver_calc(1, 2, 3, ver_label_prefix_as_num_rc, 4)));
} $unscoped(TEST_fn);
