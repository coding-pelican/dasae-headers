#include <dh-main.h>
#include <dh/io/common.h>
#include <dh/heap/Sys.h>
#include "daterm.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap))));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));
    try_(daterm_ANSI_enableMouseTracking(&ansi));
    defer_(daterm_ANSI_disableMouseTracking(&ansi));

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);

    try_(io_Writer_println(out, u8_l("Press q or Ctrl-C or ESC to exit...")));
    try_(io_Writer_println(out, u8_l("Press m to toggle mouse event printing...")));
    var_(allow_printing_mouse_events, bool) = true;
    while_(var is_running = true, is_running) {
        if_some((daterm_Term_poll(term))(event)) {
            match_(event) {
            pattern_((daterm_Event_key)(key)) {
                switch ($suppress_(switch_enum)(key.code)) {
                case_((dansi_Event_KeyCode_esc)){
                    try_(io_Writer_println(out, u8_l("ESC pressed, exiting...")));
                    is_running = false;
                } $end(case);
                case_((dansi_Event_KeyCode_char)){
                    try_(io_Writer_println(
                        out, u8_l("Key event: codepoint={:C}, mods={:uhh}"),
                        key.codepoint, key.mods.packed
                    ));
                    if (key.codepoint == 'm') {
                        let captured = allow_printing_mouse_events;
                        try_(io_Writer_println(
                            out, u8_l("Mouse event printing {:B} -> {:B}"),
                            captured, !captured
                        ));
                        allow_printing_mouse_events = !captured;
                    }
                    let pressed_q = key.codepoint == 'q';
                    if (pressed_q) try_(io_Writer_println(out, u8_l("q pressed, exiting...")));
                    let pressed_ctrl_c = key.mods.ctrl && key.codepoint == 'c';
                    if (pressed_ctrl_c) try_(io_Writer_println(out, u8_l("Ctrl-C pressed, exiting...")));
                    is_running = !(pressed_q || pressed_ctrl_c);
                } $end(case);
                default_() $do_nothing $end(default);
                }
            }$end(pattern);
            pattern_((daterm_Event_mouse)(mouse)) {
                if (allow_printing_mouse_events) try_(io_Writer_println(
                    out, u8_l("Mouse event: col={:uh}, row={:uh}, button={:uhh}, action={:uhh}, wheel={:uhh}"),
                    mouse.col, mouse.row, mouse.button, mouse.action, mouse.wheel
                ));
            } $end(pattern);
            default_() $do_nothing $end(default);
            } $end(match);
        }
        time_sleep(time_Dur_fromMillis(16));
    }

    return_ok({});
} $unguarded(fn);
