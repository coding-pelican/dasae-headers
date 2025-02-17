#ifndef ENGINE_KEY_CODE_INCLUDED
#define ENGINE_KEY_CODE_INCLUDED (1)

#include "engine-wip/common.h"

/// Key code identifiers
enum {
    engine_KeyCode_none = -1,

    /* Special keys */
    engine_KeyCode_backspace = 0x08,
    engine_KeyCode_tab       = 0x09,
    engine_KeyCode_enter     = 0x0D,

    /* Modifiers */
    engine_KeyCode_shift_left  = 0xA0,
    engine_KeyCode_shift_right = 0xA1,
    engine_KeyCode_shift       = engine_KeyCode_shift_left,
    engine_KeyCode_ctrl_left   = 0xA2,
    engine_KeyCode_ctrl_right  = 0xA3,
    engine_KeyCode_ctrl        = engine_KeyCode_ctrl_left,
    engine_KeyCode_alt_left    = 0xA4,
    engine_KeyCode_alt_right   = 0xA5,
    engine_KeyCode_alt         = engine_KeyCode_alt_left,

    /* Special Keys */
    engine_KeyCode_esc   = 0x1B,
    engine_KeyCode_space = 0x20,

    /* Arrow keys */
    engine_KeyCode_arrow_left  = 0x25,
    engine_KeyCode_arrow_up    = 0x26,
    engine_KeyCode_arrow_right = 0x27,
    engine_KeyCode_arrow_down  = 0x28,

    /* Number keys */
    engine_KeyCode_n1 = 0x31,
    engine_KeyCode_n2 = 0x32,
    engine_KeyCode_n3 = 0x33,
    engine_KeyCode_n4 = 0x34,
    engine_KeyCode_n5 = 0x35,
    engine_KeyCode_n6 = 0x36,
    engine_KeyCode_n7 = 0x37,
    engine_KeyCode_n8 = 0x38,
    engine_KeyCode_n9 = 0x39,
    engine_KeyCode_n0 = 0x30,

    /* Alphabetical keys */
    engine_KeyCode_a = 0x41,
    engine_KeyCode_b = 0x42,
    engine_KeyCode_c = 0x43,
    engine_KeyCode_d = 0x44,
    engine_KeyCode_e = 0x45,
    engine_KeyCode_f = 0x46,
    engine_KeyCode_g = 0x47,
    engine_KeyCode_h = 0x48,
    engine_KeyCode_i = 0x49,
    engine_KeyCode_j = 0x4A,
    engine_KeyCode_k = 0x4B,
    engine_KeyCode_l = 0x4C,
    engine_KeyCode_m = 0x4D,
    engine_KeyCode_n = 0x4E,
    engine_KeyCode_o = 0x4F,
    engine_KeyCode_p = 0x50,
    engine_KeyCode_q = 0x51,
    engine_KeyCode_r = 0x52,
    engine_KeyCode_s = 0x53,
    engine_KeyCode_t = 0x54,
    engine_KeyCode_u = 0x55,
    engine_KeyCode_v = 0x56,
    engine_KeyCode_w = 0x57,
    engine_KeyCode_x = 0x58,
    engine_KeyCode_y = 0x59,
    engine_KeyCode_z = 0x5A,

    /* Numpad keys */
    engine_KeyCode_kp_n0      = 0x60,
    engine_KeyCode_kp_n1      = 0x61,
    engine_KeyCode_kp_n2      = 0x62,
    engine_KeyCode_kp_n3      = 0x63,
    engine_KeyCode_kp_n4      = 0x64,
    engine_KeyCode_kp_n5      = 0x65,
    engine_KeyCode_kp_n6      = 0x66,
    engine_KeyCode_kp_n7      = 0x67,
    engine_KeyCode_kp_n8      = 0x68,
    engine_KeyCode_kp_n9      = 0x69,
    engine_KeyCode_kp_mul     = 0x6A,
    engine_KeyCode_kp_add     = 0x6B,
    engine_KeyCode_kp_sub     = 0x6D,
    engine_KeyCode_kp_decimal = 0x6E,
    engine_KeyCode_kp_div     = 0x6F,

    engine_KeyCode_count = (i16)256
};
typedef i16 engine_KeyCode;
use_Sli$(engine_KeyCode);
use_Opt$(engine_KeyCode);
use_Err$(engine_KeyCode);

#endif /* ENGINE_KEY_CODE_INCLUDED */
