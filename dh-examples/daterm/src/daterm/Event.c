#include "daterm/Event.h"
#include <dh/mem/common.h>

// Normal tracking mode: CSI M CbCxCy
// Low 2 bits: button (0=left, 1=middle, 2=right, 3=release)
// Next 3 bits: modifiers (4=shift, 8=alt, 16=ctrl)
// Bit 6 (0x40): movement/scroll indicator
T_use_O$(daterm_Event_Mouse);
$static fn_((daterm_Event__parseMouseAction(u8 action, u8 x, u8 y))(O$daterm_Event_Mouse) $scope) {
    // Check for movement/scroll
    let btn_parsed_action = expr_(daterm_Event_MouseBtn $scope)(if ((action & n_(0b, 0100, 0000)) != 0) {
        switch (action & n_(0b, 0000, 0011)) {
        case 0: $break_(daterm_Event_MouseBtn_scroll_up);
        case 1: $break_(daterm_Event_MouseBtn_scroll_down);
        case 2:
        case 3: $break_(daterm_Event_MouseBtn_move);
        default: return_none();
        }
    } else {
        // Button press/release
        switch (action & n_(0b, 0000, 0011)) {
        case 0: $break_(daterm_Event_MouseBtn_left);
        case 1: $break_(daterm_Event_MouseBtn_middle);
        case 2: $break_(daterm_Event_MouseBtn_right);
        case 3: $break_(daterm_Event_MouseBtn_release);
        default: return_none();
        }
    }) $unscoped(expr);
    let_(mods_parsed_action, daterm_Event_KeyMods) = {
        .shift = (action & n_(0b, 0000, 0100)) != 0,
        .alt = (action & n_(0b, 0000, 1000)) != 0,
        .ctrl = (action & n_(0b, 0001, 0000)) != 0,
    };
    return_some({
        .x = x,
        .y = y,
        .btn = btn_parsed_action,
        .mods = mods_parsed_action,
    });
} $unscoped(fn);

fn_((daterm_Event_parseEscSeq(io_Reader reader))(O$daterm_Event) $scope) {
    let c1 = catch_((io_Reader_readByte(reader))($ignore, return_none()));
    switch (c1) {
    case '[': { // CSI sequence
        var_(buf, A$$(32, u8)) = A_zero();
        var i = as$(usize)(0);
        // Read until final byte (0x40-0x7E)
        while (i < A_len(buf)) {
            let c = catch_((io_Reader_readByte(reader))($ignore, return_none()));
            *A_at((buf)[i]) = c;
            i += 1;
            if (0x40 <= c && c <= 0x7E) { // Final byte
                break;
            }
        }
        if (i == 0) { return_none(); }

        let seq = A_prefix$((S$u8)(buf)(i)).as_const;
        let final_byte = *A_at((buf)[i - 1]);
        // Simple arrow keys
        if (i == 1) {
            switch (final_byte) {
            case 'A': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_up }));
            case 'B': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_down }));
            case 'C': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_right }));
            case 'D': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_left }));
            case 'H': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_home }));
            case 'F': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_end }));
            default: break;
            }
        }

        // Tilde sequences (navigation keys)
        if (final_byte == '~') {
            if (mem_eqlBytes(seq, u8_l("1~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_home })); }
            if (mem_eqlBytes(seq, u8_l("2~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_insert })); }
            if (mem_eqlBytes(seq, u8_l("3~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_delete })); }
            if (mem_eqlBytes(seq, u8_l("4~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_end })); }
            if (mem_eqlBytes(seq, u8_l("5~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_page_up })); }
            if (mem_eqlBytes(seq, u8_l("6~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_page_down })); }
            // Function keys
            if (mem_eqlBytes(seq, u8_l("11~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f1 })); }
            if (mem_eqlBytes(seq, u8_l("12~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f2 })); }
            if (mem_eqlBytes(seq, u8_l("13~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f3 })); }
            if (mem_eqlBytes(seq, u8_l("14~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f4 })); }
            if (mem_eqlBytes(seq, u8_l("15~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f5 })); }
            if (mem_eqlBytes(seq, u8_l("17~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f6 })); }
            if (mem_eqlBytes(seq, u8_l("18~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f7 })); }
            if (mem_eqlBytes(seq, u8_l("19~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f8 })); }
            if (mem_eqlBytes(seq, u8_l("20~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f9 })); }
            if (mem_eqlBytes(seq, u8_l("21~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f10 })); }
            if (mem_eqlBytes(seq, u8_l("23~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f11 })); }
            if (mem_eqlBytes(seq, u8_l("24~"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f12 })); }
        }

        // Modified arrow keys (shift=2, alt=3, ctrl=5)
        if (mem_eqlBytes(seq, u8_l("1;2A"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_up, .mods = { .shift = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;2B"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_down, .mods = { .shift = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;2C"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_right, .mods = { .shift = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;2D"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_left, .mods = { .shift = true } })); }

        if (mem_eqlBytes(seq, u8_l("1;3A"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_up, .mods = { .alt = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;3B"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_down, .mods = { .alt = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;3C"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_right, .mods = { .alt = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;3D"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_left, .mods = { .alt = true } })); }

        if (mem_eqlBytes(seq, u8_l("1;5A"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_up, .mods = { .ctrl = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;5B"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_down, .mods = { .ctrl = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;5C"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_right, .mods = { .ctrl = true } })); }
        if (mem_eqlBytes(seq, u8_l("1;5D"))) { return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_left, .mods = { .ctrl = true } })); }

        // Mouse tracking (normal mode)
        if (final_byte == 'M') {
            let action = catch_((io_Reader_readByte(reader))($ignore, return_none()));
            let x = catch_((io_Reader_readByte(reader))($ignore, return_none()));
            let y = catch_((io_Reader_readByte(reader))($ignore, return_none()));
            let parsed = orelse_((daterm_Event__parseMouseAction(action, x, y))(return_none()));
            return_some(union_of((daterm_Event_mouse)(parsed)));
        }
    } break;
    case 'O': { // SS3 sequence (function keys)
        let c2 = catch_((io_Reader_readByte(reader))($ignore, return_none()));
        switch (c2) {
        case 'P': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f1 }));
        case 'Q': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f2 }));
        case 'R': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f3 }));
        case 'S': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_f4 }));
        case 'H': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_home }));
        case 'F': return_some(union_of((daterm_Event_key){ .code = daterm_Event_KeyCode_end }));
        default: break;
        }
    } break;
    default: {
        // Alt + character combinations
        if ((0x01 <= c1 && c1 <= 0x0C) || (0x0E <= c1 && c1 <= 0x1A)) {
            // Ctrl + Alt + character
            return_some(union_of((daterm_Event_key){
                .code = daterm_Event_KeyCode_char,
                .codepoint = as$(u32)(c1 + 0x60),
                .mods = { .ctrl = 1, .alt = 1 },
            }));
        } else {
            // Alt + character
            return_some(union_of((daterm_Event_key){
                .code = daterm_Event_KeyCode_char,
                .codepoint = as$(u32)(c1),
                .mods = { .alt = 1 },
            }));
        }
    } break;
    }
    return_none();
} $unscoped(fn);
