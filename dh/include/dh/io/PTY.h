/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    PTY.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-24 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_PTY
 *
 * @brief   Pseudo terminal I/O and process-session bridge.
 * @details PTY is modeled as terminal-shaped I/O plus an optional child process
 *          session. The byte protocol is not interpreted here.
 */
#pragma once
#ifndef io_PTY__included
#define io_PTY__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "TTY.h"
#include "../mem/Alctr.h"
#include "../proc/Child.h"

/*========== Macros and Declarations ========================================*/

/*---------- Errors ---------------------------------------------------------*/

errset_((io_PTY_E)(
    io_PTY_Unsupported,
    io_PTY_OpenFailed,
    io_PTY_SpawnFailed,
    io_PTY_InvalidCommand,
    io_PTY_InvalidSize,
    io_PTY_ResizeFailed,
    io_PTY_SystemResources
));

/*---------- PTY Configuration ----------------------------------------------*/

T_alias$((io_PTY_Size)(struct io_PTY_Size {
    var_(cols, u16);
    var_(rows, u16);
}));
T_use_prl$(io_PTY_Size);
T_use_E$($set(io_PTY_E)(io_PTY_Size));
$attr($inline_always)
$static fn_((io_PTY_Size_default(void))(io_PTY_Size));

T_alias$((io_PTY_OpenCfg)(struct io_PTY_OpenCfg {
    var_(size, io_PTY_Size);
    var_(nonblocking, bool);
    var_(slave_mode, O$io_TTY_ModePatch);
}));
T_use_prl$(io_PTY_OpenCfg);
$attr($inline_always)
$static fn_((io_PTY_OpenCfg_default(void))(io_PTY_OpenCfg));

/*---------- PTY Instance ---------------------------------------------------*/

T_alias$((io_PTY)(struct io_PTY {
    var_(read_file, fs_File);
    var_(write_file, O$fs_File);
    var_(slave_file, O$fs_File);
    var_(platform_resource, O$P$raw);
}));
T_use_prl$(io_PTY);
T_use_E$($set(io_PTY_E)(io_PTY));

/*---------- PTY Lifecycle --------------------------------------------------*/

$attr($must_check)
$extern fn_((io_PTY_open(io_PTY_OpenCfg cfg))(io_PTY_E$io_PTY));
$extern fn_((io_PTY_close(io_PTY* self))(void));
$extern fn_((io_PTY_reader(io_PTY* self))(io_Reader));
$extern fn_((io_PTY_writer(io_PTY* self))(io_Writer));
$attr($must_check)
$extern fn_((io_PTY_resize(io_PTY* self, io_PTY_Size size))(E$void));

/*---------- PTY Session ----------------------------------------------------*/

T_alias$((io_PTY_Session)(struct io_PTY_Session {
    var_(pty, io_PTY);
    var_(child, proc_Child);
}));
T_use_prl$(io_PTY_Session);
T_use_E$($set(io_PTY_E)(io_PTY_Session));

/*---------- PTY Session Configuration --------------------------------------*/

T_alias$((io_PTY_SpawnCfg)(struct io_PTY_SpawnCfg {
    var_(gpa, mem_Alctr);
    var_(cmd, proc_Cmd);
    var_(pty, io_PTY_OpenCfg);
}));
T_use_prl$(io_PTY_SpawnCfg);
$attr($inline_always)
$static fn_((io_PTY_SpawnCfg_default(mem_Alctr gpa, proc_Cmd cmd))(io_PTY_SpawnCfg));

/*---------- PTY Session Lifecycle ------------------------------------------*/

$attr($must_check)
$extern fn_((io_PTY_spawn(io_PTY_SpawnCfg cfg))(io_PTY_E$io_PTY_Session));
$extern fn_((io_PTY_Session_close(io_PTY_Session* self))(void));
$extern fn_((io_PTY_Session_reader(io_PTY_Session* self))(io_Reader));
$extern fn_((io_PTY_Session_writer(io_PTY_Session* self))(io_Writer));
$attr($must_check)
$extern fn_((io_PTY_Session_resize(io_PTY_Session* self, io_PTY_Size size))(E$void));
$attr($must_check)
$extern fn_((io_PTY_Session_wait(io_PTY_Session* self))(E$proc_Ter));
$extern fn_((io_PTY_Session_kill(io_PTY_Session* self))(void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
/*---------- Default Configurations -----------------------------------------*/

fn_((io_PTY_Size_default(void))(io_PTY_Size)) {
    return (io_PTY_Size){ .cols = u16_(80), .rows = u16_(24) };
};
fn_((io_PTY_OpenCfg_default(void))(io_PTY_OpenCfg)) {
    return (io_PTY_OpenCfg){
        .size = io_PTY_Size_default(),
        .nonblocking = false,
        .slave_mode = none(),
    };
};
fn_((io_PTY_SpawnCfg_default(mem_Alctr gpa, proc_Cmd cmd))(io_PTY_SpawnCfg)) {
    return (io_PTY_SpawnCfg){
        .gpa = gpa,
        .cmd = cmd,
        .pty = io_PTY_OpenCfg_default(),
    };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_PTY__included */
