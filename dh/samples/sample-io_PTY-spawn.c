/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    example-pty-spawn.c
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-24 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dal-project/examples
 * @prefix  (none)
 *
 * @brief   Spawn a child process attached to a pseudo terminal
 */
#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/io/PTY.h"

fn_((main(proc_Entry entry))(E$void) $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var_(argv, A$$(4, S_const$u8)) = A_init(pp_if_(plat_is_windows)(
        pp_then_({
            [0] = u8_l("cmd.exe"),
            [1] = u8_l("/D"),
            [2] = u8_l("/C"),
            [3] = u8_l("exit 0"),
        }),
        pp_else_({
            [0] = u8_l("/bin/sh"),
            [1] = u8_l("-c"),
            [2] = u8_l("exit 0"),
            [3] = u8_l("example-pty-spawn"),
        })
    ));
    let_(cmd, proc_Cmd) = {
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = none(),
        .cwd = union_of((proc_Cwd_inherit){}),
        .std_in = union_of((proc_std_IO_inherit){}),
        .std_out = union_of((proc_std_IO_inherit){}),
        .std_err = union_of((proc_std_IO_inherit){}),
        .expand_arg0 = proc_ArgExpsn_no_expand,
        .start_suspended = false,
        .create_no_window = false,
    };

    let cfg = with_((io_PTY_SpawnCfg_default(heap_Sys_alctr(&heap), cmd))(
        (.pty.size)({ .cols = u16_(120), .rows = u16_(40) })
    ));
    var session = try_(io_PTY_spawn(cfg));
    defer_(io_PTY_Session_close(entry.proc, &session));

    let_(size, io_PTY_Size) = { .cols = u16_(120), .rows = u16_(40) };
    try_(io_PTY_Session_resize(&session, size));
    let_ignore = try_(io_PTY_Session_wait(entry.proc, &session));

    return_ok({});
} $unguarded(fn);
