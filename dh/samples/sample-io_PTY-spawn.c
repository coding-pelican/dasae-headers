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
    let gpa = heap_Sys_alctr(&heap);
    let proc = try_(proc_direct());
    let exit_cmd = u8_l("exit 0");
    var argv = A_from$((S_const$u8){
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)(
                u8_l("cmd.exe"),
                u8_l("/D"),
                u8_l("/C"),
                exit_cmd
            )),
            pp_case_((plat_type_linux)(
                u8_l("/bin/sh"),
                u8_l("-c"),
                exit_cmd,
                u8_l("example-pty-spawn")
            )),
            pp_default_(()(local_({
                io_stream_eprintln(u8_l("unsupported platform: {:s}"), u8_l(plat_name));
                proc_exit(proc, 1);
                local_return_(u8_l(""));
            })))
        )),
    });
    let_(cmd, proc_Spawn_Opts) = {
        .argv = A_ref$((S$S_const$u8)(argv)),
        .env = none(),
        .cwd = union_of((proc_cmd_CWD_inherit){}),
        .std_in = union_of((proc_cmd_StdIO_inherit){}),
        .std_out = union_of((proc_cmd_StdIO_inherit){}),
        .std_err = union_of((proc_cmd_StdIO_inherit){}),
        .expand_arg0 = proc_cmd_ArgExpsn_no_expand,
        .start_suspended = false,
        .create_no_window = false,
    };

    let cfg = with_((io_PTY_SpawnCfg_default(gpa, entry.env, cmd))(
        (.pty.size)({ .cols = u16_(120), .rows = u16_(40) })
    ));
    var session = try_(io_PTY_spawn(cfg));
    defer_(io_PTY_Session_close(&session, proc));

    let_(size, io_PTY_Size) = { .cols = u16_(120), .rows = u16_(40) };
    try_(io_PTY_Session_resize(&session, size));
    let_ignore = try_(io_PTY_Session_wait(&session, proc));

    return_ok({});
} $unguarded(fn);
