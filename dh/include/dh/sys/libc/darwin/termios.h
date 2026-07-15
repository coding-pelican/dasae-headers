/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    termios.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-26 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_termios__included
#define sys_libc_darwin_termios__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_darwin
typedef usize sys_libc_darwin_tcflag_t;
typedef u8 sys_libc_darwin_cc_t;
typedef usize sys_libc_darwin_speed_t;

typedef struct sys_libc_darwin_termios {
    var_(c_iflag, sys_libc_darwin_tcflag_t);
    var_(c_oflag, sys_libc_darwin_tcflag_t);
    var_(c_cflag, sys_libc_darwin_tcflag_t);
    var_(c_lflag, sys_libc_darwin_tcflag_t);
    var_(c_cc, A$$(20, sys_libc_darwin_cc_t));
    var_(c_ispeed, sys_libc_darwin_speed_t);
    var_(c_ospeed, sys_libc_darwin_speed_t);
} sys_libc_darwin_termios;

typedef struct sys_libc_darwin_winsize {
    var_(ws_row, u16);
    var_(ws_col, u16);
    var_(ws_xpixel, u16);
    var_(ws_ypixel, u16);
} sys_libc_darwin_winsize;

typedef enum sys_libc_darwin_CC {
    sys_libc_darwin_VMIN = 16,
    sys_libc_darwin_VTIME = 17,
} sys_libc_darwin_CC;

typedef enum sys_libc_darwin_IFLAG {
    sys_libc_darwin_BRKINT = 0x00000002,
    sys_libc_darwin_INPCK = 0x00000010,
    sys_libc_darwin_ISTRIP = 0x00000020,
    sys_libc_darwin_ICRNL = 0x00000100,
    sys_libc_darwin_IXON = 0x00000200,
    sys_libc_darwin_IXOFF = 0x00000400,
} sys_libc_darwin_IFLAG;

typedef enum sys_libc_darwin_OFLAG {
    sys_libc_darwin_OPOST = 0x00000001,
} sys_libc_darwin_OFLAG;

typedef enum sys_libc_darwin_LFLAG {
    sys_libc_darwin_ECHO = 0x00000008,
    sys_libc_darwin_ISIG = 0x00000080,
    sys_libc_darwin_ICANON = 0x00000100,
    sys_libc_darwin_IEXTEN = 0x00000400,
} sys_libc_darwin_LFLAG;

typedef enum sys_libc_darwin_TCSA {
    sys_libc_darwin_TCSANOW = 0,
} sys_libc_darwin_TCSA;

$extern fn_((sys_libc_darwin_tcgetattr(sys_libc_darwin_fd_t fd, sys_libc_darwin_termios* termios))(i32));
$extern fn_((sys_libc_darwin_tcsetattr(sys_libc_darwin_fd_t fd, const sys_libc_darwin_termios* termios))(i32));
$extern fn_((sys_libc_darwin_tiocgwinsz(sys_libc_darwin_fd_t fd, sys_libc_darwin_winsize* size))(i32));
$extern fn_((sys_libc_darwin_fionread(sys_libc_darwin_fd_t fd, int* count))(i32));
#endif /* in_analysis || plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_termios__included */
