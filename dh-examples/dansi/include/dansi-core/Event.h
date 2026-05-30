/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_Event
 *
 * @brief   Standard ANSI raw extraction and event parsing.
 */
#ifndef dansi_Event__included
#define dansi_Event__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Standard Events ===============================================*/

typedef union dansi_Event_KeyMods {
    struct {
        u8 shift    : 1;
        u8 ctrl     : 1;
        u8 alt      : 1;
        u8 reserved : 5;
    };
    u8 packed;
} dansi_Event_KeyMods;

typedef enum_((dansi_Event_KeyCode $fits($packed))(
    dansi_Event_KeyCode_char = 0,

    dansi_Event_KeyCode_enter = 0x0D,
    dansi_Event_KeyCode_esc = 0x1B,
    dansi_Event_KeyCode_backspace = 0x08,
    dansi_Event_KeyCode_tab = 0x09,

    dansi_Event_KeyCode_up = 0x10000,
    dansi_Event_KeyCode_down,
    dansi_Event_KeyCode_left,
    dansi_Event_KeyCode_right,
    dansi_Event_KeyCode_home,
    dansi_Event_KeyCode_end,
    dansi_Event_KeyCode_page_up,
    dansi_Event_KeyCode_page_down,
    dansi_Event_KeyCode_insert,
    dansi_Event_KeyCode_delete,

    dansi_Event_KeyCode_f1,
    dansi_Event_KeyCode_f2,
    dansi_Event_KeyCode_f3,
    dansi_Event_KeyCode_f4,
    dansi_Event_KeyCode_f5,
    dansi_Event_KeyCode_f6,
    dansi_Event_KeyCode_f7,
    dansi_Event_KeyCode_f8,
    dansi_Event_KeyCode_f9,
    dansi_Event_KeyCode_f10,
    dansi_Event_KeyCode_f11,
    dansi_Event_KeyCode_f12,
)) dansi_Event_KeyCode;

typedef struct dansi_Event_Key {
    var_(code, dansi_Event_KeyCode);
    /// Valid only when `code == dansi_Event_KeyCode_char`.
    var_(codepoint, u32);
    var_(mods, dansi_Event_KeyMods);
} dansi_Event_Key;

typedef variant_((dansi_Event $fits($packed))(
    (dansi_Event_key, dansi_Event_Key)
)) dansi_Event;
T_use_prl$(dansi_Event);

errset_((dansi_Event_E)(
    dansi_Event_UnknownSeq,
    dansi_Event_InvalidSeq
));
T_use_E$($set(dansi_Event_E)(dansi_Event));

$attr($must_check)
$extern fn_((dansi_Event_parse(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$extern fn_((dansi_Event_tryParse(dansi_Seq seq))(O$dansi_Event));

$attr($inline_always)
$static fn_((dansi_Event_matchesChar(dansi_Event event, u32 ch, dansi_Event_KeyMods mods))(bool)) {
    match_(event) {
    pattern_((dansi_Event_key)(key)) {
        if (key.code == dansi_Event_KeyCode_char && key.codepoint == ch) {
            return key.mods.shift == mods.shift
                && key.mods.alt == mods.alt
                && key.mods.ctrl == mods.ctrl;
        }
    } $end(pattern);
    default_() break $end(default);
    } $end(match);
    return false;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_Event__included */
