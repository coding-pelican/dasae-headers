#include <dh-main.h>
#include <dh/io/common.h>
#include <dh/io/Buf.h>
#include <dh/fs/File.h>
#include "daterm.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let in_file = io_getStdIn();
    let out_file = io_getStdOut();
    // Enable terminal raw mode, its very recommended when listening for events
    var term = try_(daterm_Ctx_init(in_file.handle, out_file.handle));
    defer_(catch_((daterm_Ctx_fini(&term))($ignore, $do_nothing)));

    var_(out_buf, A$$(1024, u8)) = A_zero();
    var out_bufd = io_Buf_Writer_init(fs_File_writer(out_file), A_ref$((S$u8)(out_buf)));
    defer_(catch_((io_Buf_Writer_flush(&out_bufd))($ignore, $do_nothing)));
    let out_writer = io_Buf_writer(&out_bufd);

    // To listen mouse events, we need to enable mouse tracking
    try_(daterm_Ctx_enableMouse(&term, out_writer));
    defer_(catch_((daterm_Ctx_disableMouse(&term, out_writer))($ignore, $do_nothing)));

    try_(io_Writer_println(out_writer, u8_l("Press q or Ctrl-C to exit...")));
    bool running = true;
    while (running) { /* clang-format off */
        if_ok((daterm_Ctx_timedWait(&term, time_Duration_fromSecs(1)))(next)) match_(next) {
            pattern_((daterm_Event_key)(on_key)) switch (on_key.code) {
                case_((daterm_Event_KeyCode_char)) {
                    let is_pressed_q = on_key.codepoint == 'q';
                    let is_pressed_ctrl_c = on_key.mods.ctrl && on_key.codepoint == 'c';
                    if (is_pressed_q || is_pressed_ctrl_c) { running = false; }
                    try_(io_Writer_println(out_writer, u8_l("Pressed: {:C}"), on_key.codepoint));
                } $end(case);
                default_() break $end(default);
            } $end(pattern);
            pattern_((daterm_Event_mouse)(on_mouse)) {
                try_(io_Writer_println(out_writer, u8_l("Mouse: {:dhh}"), on_mouse.btn));
            } $end(pattern);
            default_() break $end(default);
        } $end(match) else_err((e)) {
            let_ignore = e;
            try_(io_Writer_println(out_writer, u8_l("Timeout.")));
        };
        try_(io_Buf_Writer_flush(&out_bufd));
    } /* clang-format on */
    try_(io_Writer_println(out_writer, u8_l("Bye bye")));

    return_ok({});
} $unguarded(fn);
