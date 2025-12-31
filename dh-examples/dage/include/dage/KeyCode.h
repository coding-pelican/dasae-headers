/* include/dage/KeyCode.h */
#ifndef dage_KeyCode__included
#define dage_KeyCode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dage/common.h"

/// Key code identifiers
typedef enum_(dage_KeyCode $bits(16)) {
    dage_KeyCode_unknown = -1,

    /* Special keys */
    dage_KeyCode_backspace = 0x08,
    dage_KeyCode_tab = 0x09,
    dage_KeyCode_enter = 0x0D,
    dage_KeyCode_esc = 0x1B,
    dage_KeyCode_space = 0x20,

    /* Navigation & Editing */
    dage_KeyCode_page_up = 0x21,
    dage_KeyCode_page_down = 0x22,
    dage_KeyCode_end = 0x23,
    dage_KeyCode_home = 0x24,
    dage_KeyCode_arrow_left = 0x25,
    dage_KeyCode_arrow_up = 0x26,
    dage_KeyCode_arrow_right = 0x27,
    dage_KeyCode_arrow_down = 0x28,
    dage_KeyCode_insert = 0x2D,
    dage_KeyCode_delete = 0x2E,

    /* Numbers (0-9) */
    dage_KeyCode_n0 = 0x30,
    dage_KeyCode_n1 = 0x31,
    dage_KeyCode_n2 = 0x32,
    dage_KeyCode_n3 = 0x33,
    dage_KeyCode_n4 = 0x34,
    dage_KeyCode_n5 = 0x35,
    dage_KeyCode_n6 = 0x36,
    dage_KeyCode_n7 = 0x37,
    dage_KeyCode_n8 = 0x38,
    dage_KeyCode_n9 = 0x39,

    /* Alphabetical keys (A-Z) */
    dage_KeyCode_a = 0x41,
    dage_KeyCode_b = 0x42,
    dage_KeyCode_c = 0x43,
    dage_KeyCode_d = 0x44,
    dage_KeyCode_e = 0x45,
    dage_KeyCode_f = 0x46,
    dage_KeyCode_g = 0x47,
    dage_KeyCode_h = 0x48,
    dage_KeyCode_i = 0x49,
    dage_KeyCode_j = 0x4A,
    dage_KeyCode_k = 0x4B,
    dage_KeyCode_l = 0x4C,
    dage_KeyCode_m = 0x4D,
    dage_KeyCode_n = 0x4E,
    dage_KeyCode_o = 0x4F,
    dage_KeyCode_p = 0x50,
    dage_KeyCode_q = 0x51,
    dage_KeyCode_r = 0x52,
    dage_KeyCode_s = 0x53,
    dage_KeyCode_t = 0x54,
    dage_KeyCode_u = 0x55,
    dage_KeyCode_v = 0x56,
    dage_KeyCode_w = 0x57,
    dage_KeyCode_x = 0x58,
    dage_KeyCode_y = 0x59,
    dage_KeyCode_z = 0x5A,

    /* Function Keys (F1-F12) */
    dage_KeyCode_f1 = 0x70,
    dage_KeyCode_f2 = 0x71,
    dage_KeyCode_f3 = 0x72,
    dage_KeyCode_f4 = 0x73,
    dage_KeyCode_f5 = 0x74,
    dage_KeyCode_f6 = 0x75,
    dage_KeyCode_f7 = 0x76,
    dage_KeyCode_f8 = 0x77,
    dage_KeyCode_f9 = 0x78,
    dage_KeyCode_f10 = 0x79,
    dage_KeyCode_f11 = 0x7A,
    dage_KeyCode_f12 = 0x7B,

    /* Modifiers */
    dage_KeyCode_shift_left = 0xA0,
    dage_KeyCode_shift_right = 0xA1,
    dage_KeyCode_ctrl_left = 0xA2,
    dage_KeyCode_ctrl_right = 0xA3,
    dage_KeyCode_alt_left = 0xA4,
    dage_KeyCode_alt_right = 0xA5,

    /* Aliases for convenience */
    dage_KeyCode_shift = dage_KeyCode_shift_left,
    dage_KeyCode_ctrl = dage_KeyCode_ctrl_left,
    dage_KeyCode_alt = dage_KeyCode_alt_left,

    dage_KeyCode_count = 256
} dage_KeyCode;

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* dage_KeyCode__included */
