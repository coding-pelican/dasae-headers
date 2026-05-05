/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_Event
 *
 * @brief   Terminal input event parsing
 * @details Parses keyboard and mouse input from raw terminal input.
 *          Handles ANSI escape sequences for special keys and mouse events.
 */
#ifndef daterm_Event__included
#define daterm_Event__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

/// Modifier flags
typedef union daterm_Event_KeyMods {
    struct {
        u8 shift     : 1;
        u8 ctrl      : 1;
        u8 alt       : 1;
        u8 caps_lock : 1;
        u8 num_lock  : 1;
        u8 reserved_ : 3;
    };
    u8 packed;
} daterm_Event_KeyMods;

/* --- Key Codes --- */

/// Special key codes (non-printable keys)
typedef enum_((daterm_Event_KeyCode $fits($packed))(
    // Printable characters use their Unicode codepoint directly (0-127 ASCII, 128+ UTF-8)
    // Special keys use high values to avoid conflicts

    daterm_Event_KeyCode_char = 0, // Regular character (use codepoint field)

    // Control keys
    daterm_Event_KeyCode_enter = 0x0D,
    daterm_Event_KeyCode_esc = 0x1B,
    daterm_Event_KeyCode_backspace = 0x08,
    daterm_Event_KeyCode_tab = 0x09,

    // Navigation keys
    daterm_Event_KeyCode_up = 0x10000,
    daterm_Event_KeyCode_down,
    daterm_Event_KeyCode_left,
    daterm_Event_KeyCode_right,
    daterm_Event_KeyCode_home,
    daterm_Event_KeyCode_end,
    daterm_Event_KeyCode_page_up,
    daterm_Event_KeyCode_page_down,
    daterm_Event_KeyCode_insert,
    daterm_Event_KeyCode_delete,

    // Function keys
    daterm_Event_KeyCode_f1,
    daterm_Event_KeyCode_f2,
    daterm_Event_KeyCode_f3,
    daterm_Event_KeyCode_f4,
    daterm_Event_KeyCode_f5,
    daterm_Event_KeyCode_f6,
    daterm_Event_KeyCode_f7,
    daterm_Event_KeyCode_f8,
    daterm_Event_KeyCode_f9,
    daterm_Event_KeyCode_f10,
    daterm_Event_KeyCode_f11,
    daterm_Event_KeyCode_f12,
)) daterm_Event_KeyCode;

/* --- Mouse Buttons --- */

/// Mouse button identifiers
typedef enum_((daterm_Event_MouseBtn $fits($packed))(
    daterm_Event_MouseBtn_left = 0,
    daterm_Event_MouseBtn_middle = 1,
    daterm_Event_MouseBtn_right = 2,
    daterm_Event_MouseBtn_release = 3,
    daterm_Event_MouseBtn_scroll_up = 4,
    daterm_Event_MouseBtn_scroll_down = 5,
    daterm_Event_MouseBtn_move = 6,
)) daterm_Event_MouseBtn;

/* --- Event Types --- */

/// Keyboard event
typedef struct daterm_Event_Key {
    var_(code, daterm_Event_KeyCode); // Key code or daterm_Event_KeyCode_char
    var_(codepoint, u32); // Unicode codepoint (valid when code == char)
    var_(mods, daterm_Event_KeyMods); // Modifier keys state
} daterm_Event_Key;

/// Mouse event
typedef struct daterm_Event_Mouse {
    var_(x, u16); // Column (0-based)
    var_(y, u16); // Row (0-based)
    var_(btn, daterm_Event_MouseBtn);
    var_(mods, daterm_Event_KeyMods);
} daterm_Event_Mouse;

/// Resize event
typedef struct daterm_Event_Resize {
    var_(w, u16);
    var_(h, u16);
} daterm_Event_Resize;

/// Terminal event (tagged union)
typedef variant_((daterm_Event $fits($packed))(
    (daterm_Event_key, daterm_Event_Key),
    (daterm_Event_mouse, daterm_Event_Mouse),
    (daterm_Event_resize, daterm_Event_Resize)
)) daterm_Event;
T_use_prl$(daterm_Event);
$extern fn_((daterm_Event_parseEscSeq(io_Reader reader))(O$daterm_Event));

/*========== Helper Functions ===============================================*/

/// Check if event matches a specific character with modifiers
$attr($inline_always)
$static fn_((daterm_event_matchesChar(daterm_Event event, u32 ch, daterm_Event_KeyMods mods))(bool)) {
    match_(event) {
    pattern_((daterm_Event_key)(key)) {
        if (key.code == daterm_Event_KeyCode_char && key.codepoint == ch) {
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
#endif /* daterm_Event__included */
