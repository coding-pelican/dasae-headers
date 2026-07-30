/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    example-tty-raw-vt.c
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-24 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dal-project/examples
 * @prefix  (none)
 *
 * @brief   Enter raw virtual-terminal mode until one input byte is received
 */
#include "dh-main.h"
#include "dh/io/TTY.h"

fn_((main(proc_Self self))(E$void) $guard) {
    let args = self.args.items;
    let_ignore = args;
    var tty = io_TTY_init(io_TTY_Cfg_std());
    defer_(io_TTY_fini(&tty));

    try_(io_TTY_enterMode(&tty, io_TTY_ModePatch_rawVT()));
    let out = io_TTY_writer(&tty);
    try_(io_Writer_println(out, u8_l("Raw VT mode enabled. Press any byte to exit.")));

    let input = io_TTY_reader(&tty);
    let_ignore = try_(io_Reader_readByte(input));
    return_ok({});
} $unguarded(fn);
