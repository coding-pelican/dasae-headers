#include "dh/sys/libc/darwin/termios.h"

#if plat_is_darwin
#include <sys/ioctl.h>
#include <termios.h>

claim_assert_static(sizeOf$(sys_libc_darwin_termios) == sizeof(struct termios));
claim_assert_static(alignOf$(sys_libc_darwin_termios) == __alignof__(struct termios));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_iflag) == __builtin_offsetof(struct termios, c_iflag));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_oflag) == __builtin_offsetof(struct termios, c_oflag));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_cflag) == __builtin_offsetof(struct termios, c_cflag));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_lflag) == __builtin_offsetof(struct termios, c_lflag));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_cc) == __builtin_offsetof(struct termios, c_cc));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_ispeed) == __builtin_offsetof(struct termios, c_ispeed));
claim_assert_static(offsetTo(sys_libc_darwin_termios, c_ospeed) == __builtin_offsetof(struct termios, c_ospeed));
claim_assert_static(sizeOf$(sys_libc_darwin_winsize) == sizeof(struct winsize));
claim_assert_static(alignOf$(sys_libc_darwin_winsize) == __alignof__(struct winsize));
#endif /* plat_is_darwin */

#if in_analysis || plat_is_darwin
fn_((sys_libc_darwin_tcgetattr(sys_libc_darwin_fd_t fd, sys_libc_darwin_termios* termios))(i32)) {
#if plat_is_darwin
    return tcgetattr(fd, as$(struct termios*)(termios));
#else
    let_ignore = fd;
    let_ignore = termios;
    claim_unreachable_msg(nameOf(sys_libc_darwin_tcgetattr) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_tcsetattr(sys_libc_darwin_fd_t fd, const sys_libc_darwin_termios* termios))(i32)) {
#if plat_is_darwin
    return tcsetattr(fd, sys_libc_darwin_TCSANOW, as$(const struct termios*)(termios));
#else
    let_ignore = fd;
    let_ignore = termios;
    claim_unreachable_msg(nameOf(sys_libc_darwin_tcsetattr) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_tiocgwinsz(sys_libc_darwin_fd_t fd, sys_libc_darwin_winsize* size))(i32)) {
#if plat_is_darwin
    return ioctl(fd, TIOCGWINSZ, as$(struct winsize*)(size));
#else
    let_ignore = fd;
    let_ignore = size;
    claim_unreachable_msg(nameOf(sys_libc_darwin_tiocgwinsz) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_fionread(sys_libc_darwin_fd_t fd, int* count))(i32)) {
#if plat_is_darwin
    return ioctl(fd, FIONREAD, count);
#else
    let_ignore = fd;
    let_ignore = count;
    claim_unreachable_msg(nameOf(sys_libc_darwin_fionread) "is not supported on this platform");
#endif /* plat_is_darwin */
};
#endif /* in_analysis || plat_is_darwin */
