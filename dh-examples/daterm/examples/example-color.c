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

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);
    try_(dansi_color_fg24bitWrite(255, 80, 80, out));
    try_(io_Writer_println(out, u8_l("red text")));
    try_(dansi_color_fg24bitWrite(80, 140, 255, out));
    try_(io_Writer_println(out, u8_l("blue text")));
    try_(dansi_attr_resetWrite(out));

    return_ok({});
} $unguarded(fn);
