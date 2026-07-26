#include <dh-main.h>
#include <dh/heap/Sys.h>
#include "daterm-context/ANSI.h"
#include "dansi-core/erase.h"
#include "dansi-core/cursor.h"
#include "dansi-core/style.h"
#include "dansi-core/sgr.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap))));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);

    try_(dansi_erase_inDisplayWrite(dansi_erase_Area_all, out));
    try_(dansi_cursor_moveToWrite(1, 1, out));
    try_(dansi_style_italicWrite(true, out));
    try_(io_Writer_println(out, u8_l("This is rendered through the daterm interface.")));
    try_(dansi_sgr_resetWrite(out));

    return_ok({});
} $unguarded(fn);
