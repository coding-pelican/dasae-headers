#include <dh-main.h>
#include <dh/heap/Sys.h>
#include "daterm-context/ANSI.h"
#include "dansi-xterm/color.h"

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var ansi = try_(daterm_ANSI_init(unwrap_(daterm_ANSI_Cfg_direct(
        heap_Sys_alctr(&heap)
    ))));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);
    try_(dansi_xterm_color_fg24bitWrite(
        (dansi_xterm_color_RGB8){
            .r = 255,
            .g = 80,
            .b = 80,
        },
        out
    ));
    try_(io_Writer_println(out, u8_l("red text")));
    try_(dansi_xterm_color_fg24bitWrite(
        (dansi_xterm_color_RGB8){
            .r = 80,
            .g = 140,
            .b = 255,
        },
        out
    ));
    try_(io_Writer_println(out, u8_l("blue text")));
    try_(dansi_sgr_resetWrite(out));

    return_ok({});
} $unguarded(fn);
