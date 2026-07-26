#include <dh-main.h>
#include <dh/heap/Sys.h>
#include "daterm-context/ANSI.h"
#include "daterm-bridge/xterm.h"
#include <dh/time/Dur.h>

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var cfg = daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap));
    cfg.input_mode = daterm_ANSI_InputMode_vt;
    var ansi = try_(daterm_ANSI_init(cfg));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));
    try_(daterm_xterm_enableMouse(
        &ansi,
        (daterm_xterm_MouseCfg){
            .report_mode = dansi_xterm_mouse_ReportMode_button_event,
            .encoding = dansi_xterm_mouse_Encoding_sgr,
        }
    ));
    defer_(catch_((daterm_xterm_disableMouse(&ansi))($ignore, $do_nothing)));
    try_(daterm_xterm_setFocusTracking(&ansi, true));
    defer_(catch_((daterm_xterm_setFocusTracking(&ansi, false))($ignore, $do_nothing)));

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);

    try_(io_Writer_println(out, u8_l("Press q or Ctrl-C or ESC to exit...")));
    try_(io_Writer_println(out, u8_l("Press m to toggle mouse event printing...")));
    var_(allow_printing_mouse_events, bool) = true;
    while_(var is_running = true, is_running) {
        if_some((daterm_Term_poll(term))(event)) {
            $suppress_(switch_enum)(match_(event)) {
            patt_((daterm_Event_key)(key)) {
                if_some((key.action)(action)) {
                    if (action == daterm_key_Action_release) continue;
                }
                $suppress_(switch_enum)(switch (key.code)) {
                case_((daterm_key_Code_escape)){
                    try_(io_Writer_println(out, u8_l("ESC pressed, exiting...")));
                    is_running = false;
                } $end(case);
                default_() $do_nothing $end(default);
                }
            } $end(patt);
            patt_((daterm_Event_text)(text)) {
                if_some((text.action)(action)) {
                    if (action == daterm_key_Action_release) continue;
                }
                try_(io_Writer_println(
                    out, u8_l("Key event: codepoint={:C}, mods={:uhh}"),
                    text.codepoint, text.mods.packed
                ));
                if (text.codepoint == 'm') {
                    let captured = allow_printing_mouse_events;
                    try_(io_Writer_println(
                        out, u8_l("Mouse event printing {:B} -> {:B}"),
                        captured, !captured
                    ));
                    allow_printing_mouse_events = !captured;
                }
                let pressed_q = text.codepoint == 'q';
                if (pressed_q) try_(io_Writer_println(out, u8_l("q pressed, exiting...")));
                let pressed_ctrl_c = text.mods.ctrl && text.codepoint == 'c';
                if (pressed_ctrl_c) try_(io_Writer_println(out, u8_l("Ctrl-C pressed, exiting...")));
                is_running = !(pressed_q || pressed_ctrl_c);
            } $end(patt);
            patt_((daterm_Event_mouse)(mouse)) {
                if (allow_printing_mouse_events) match_(mouse) {
                    patt_((daterm_mouse_Event_press)(press)) {
                        try_(io_Writer_println(
                            out, u8_l("Mouse press: col={:uh}, row={:uh}, btn={:uhh}"),
                            press.pos.x, press.pos.y, press.btn
                        ));
                    } $end(patt);
                    patt_((daterm_mouse_Event_release)(release)) {
                        let btn = isSome(release.btn)
                                    ? as$(u8)(unwrap_(release.btn))
                                    : u8_limit_max;
                        try_(io_Writer_println(
                            out, u8_l("Mouse release: col={:uh}, row={:uh}, btn={:uhh}"),
                            release.pos.x, release.pos.y, btn
                        ));
                    } $end(patt);
                    patt_((daterm_mouse_Event_motion)(motion)) {
                        let btn = isSome(motion.btn)
                                    ? as$(u8)(unwrap_(motion.btn))
                                    : u8_limit_max;
                        try_(io_Writer_println(
                            out, u8_l("Mouse motion: col={:uh}, row={:uh}, btn={:uhh}"),
                            motion.pos.x, motion.pos.y, btn
                        ));
                    } $end(patt);
                    patt_((daterm_mouse_Event_wheel)(wheel)) {
                        try_(io_Writer_println(
                            out, u8_l("Mouse wheel: col={:uh}, row={:uh}, wheel={:uhh}"),
                            wheel.pos.x, wheel.pos.y, wheel.wheel
                        ));
                    } $end(patt);
                    } $end(match);
            } $end(patt);
            patt_((daterm_Event_focus)(focus)) {
                try_(io_Writer_println(out, u8_l("Focus event: state={:uhh}"), focus));
            } $end(patt);
            patt_((daterm_Event_resize)(size)) {
                try_(io_Writer_println(out, u8_l("Resize event: cols={:uh}, rows={:uh}"), size.cols, size.rows));
            } $end(patt);
            default_() $do_nothing $end(default);
            } $end(match);
        }
        catch_((time_Clock_sleep(ansi.clock, time_Dur_fromMillis(16)))($ignore, $do_nothing));
    }

    return_ok({});
} $unguarded(fn);
