/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    key.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_key
 */
#pragma once
#ifndef daterm_key__included
#define daterm_key__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "input.h"

typedef enum_((daterm_key_Code $fits($packed))(
    daterm_key_Code_up,
    daterm_key_Code_down,
    daterm_key_Code_right,
    daterm_key_Code_left,
    daterm_key_Code_home,
    daterm_key_Code_end,
    daterm_key_Code_find,
    daterm_key_Code_insert,
    daterm_key_Code_delete,
    daterm_key_Code_remove,
    daterm_key_Code_select,
    daterm_key_Code_page_up,
    daterm_key_Code_page_down,
    daterm_key_Code_begin,
    daterm_key_Code_tab,
    daterm_key_Code_back_tab,
    daterm_key_Code_enter,
    daterm_key_Code_escape,
    daterm_key_Code_backspace,
    daterm_key_Code_pf1,
    daterm_key_Code_pf2,
    daterm_key_Code_pf3,
    daterm_key_Code_pf4,
    daterm_key_Code_f1,
    daterm_key_Code_f2,
    daterm_key_Code_f3,
    daterm_key_Code_f4,
    daterm_key_Code_f5,
    daterm_key_Code_f6,
    daterm_key_Code_f7,
    daterm_key_Code_f8,
    daterm_key_Code_f9,
    daterm_key_Code_f10,
    daterm_key_Code_f11,
    daterm_key_Code_f12,
    daterm_key_Code_f13,
    daterm_key_Code_f14,
    daterm_key_Code_f15,
    daterm_key_Code_f16,
    daterm_key_Code_f17,
    daterm_key_Code_f18,
    daterm_key_Code_f19,
    daterm_key_Code_f20,
    daterm_key_Code_f21,
    daterm_key_Code_f22,
    daterm_key_Code_f23,
    daterm_key_Code_f24,
    daterm_key_Code_keypad_0,
    daterm_key_Code_keypad_1,
    daterm_key_Code_keypad_2,
    daterm_key_Code_keypad_3,
    daterm_key_Code_keypad_4,
    daterm_key_Code_keypad_5,
    daterm_key_Code_keypad_6,
    daterm_key_Code_keypad_7,
    daterm_key_Code_keypad_8,
    daterm_key_Code_keypad_9,
    daterm_key_Code_keypad_decimal,
    daterm_key_Code_keypad_add,
    daterm_key_Code_keypad_minus,
    daterm_key_Code_keypad_multiply,
    daterm_key_Code_keypad_divide,
    daterm_key_Code_keypad_comma,
    daterm_key_Code_keypad_enter
)) daterm_key_Code;
T_use_prl$(daterm_key_Code);

typedef enum_((daterm_key_Action $fits($packed))(
    daterm_key_Action_press,
    daterm_key_Action_repeat,
    daterm_key_Action_release
)) daterm_key_Action;
T_use_prl$(daterm_key_Action);

typedef struct daterm_key_Event {
    var_(code, daterm_key_Code);
    var_(mods, daterm_input_Mods);
    var_(action, O$daterm_key_Action);
} daterm_key_Event;
T_use_prl$(daterm_key_Event);

typedef struct daterm_key_Text {
    var_(codepoint, u32);
    var_(mods, daterm_input_Mods);
    var_(action, O$daterm_key_Action);
} daterm_key_Text;
T_use_prl$(daterm_key_Text);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_key__included */
