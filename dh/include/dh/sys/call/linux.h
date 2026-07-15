/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    linux.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-25 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dasae-headers(dh)/sys/call
 * @prefix  sys_call_linux
 */
#pragma once
#ifndef sys_call_linux__included
#define sys_call_linux__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_linux
typedef isize sys_call_linux_word;
typedef sys_call_linux_word sys_call_linux_fd_t;
typedef sys_call_linux_word sys_call_linux_pid_t;
typedef u32 sys_call_linux_mode_t;

typedef i32 (*sys_call_linux_clone_entry_fn)(P$raw arg);

/*---------- <time.h> -------------------------------------------------------*/

typedef struct sys_call_linux_timespec {
    var_(tv_sec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(sys_call_linux_word)));
    var_(tv_nsec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(sys_call_linux_word)));
} sys_call_linux_timespec;

typedef enum sys_call_linux_CLOCK {
    sys_call_linux_CLOCK_REALTIME = 0,
    sys_call_linux_CLOCK_MONOTONIC = 1,
    sys_call_linux_CLOCK_PROCESS_CPUTIME_ID = 2,
    sys_call_linux_CLOCK_THREAD_CPUTIME_ID = 3,
    sys_call_linux_CLOCK_MONOTONIC_RAW = 4,
    sys_call_linux_CLOCK_BOOTTIME = 7,
} sys_call_linux_CLOCK;
typedef sys_call_linux_CLOCK sys_call_linux_clockid_t;

/*---------- <sys/time.h> ---------------------------------------------------*/

typedef struct sys_call_linux_timeval {
    var_(tv_sec, sys_call_linux_word);
    var_(tv_usec, sys_call_linux_word);
} sys_call_linux_timeval;

/*---------- <signal.h> -----------------------------------------------------*/

typedef i32 sys_call_linux_sig_atomic_t;
typedef sys_call_linux_word sys_call_linux_signal_t;

typedef struct sys_call_linux_sigset {
    var_(bits, u64);
} sys_call_linux_sigset;

struct sys_call_linux_siginfo;
typedef struct sys_call_linux_siginfo sys_call_linux_siginfo;

typedef void (*sys_call_linux_sighandler_fn)(i32);
typedef void (*sys_call_linux_sigaction_fn)(i32, sys_call_linux_siginfo*, void*);
typedef void (*sys_call_linux_sigrestore_fn)(void);

#if arch_is_x86_64
typedef struct sys_call_linux_sigaction {
    union {
        sys_call_linux_sighandler_fn sa_handler;
        sys_call_linux_sigaction_fn sa_sigaction;
    };
    var_(sa_mask, sys_call_linux_sigset);
    var_(sa_flags, sys_call_linux_word);
    var_(sa_restorer, sys_call_linux_sigrestore_fn);
} sys_call_linux_sigaction;
#else
typedef struct sys_call_linux_sigaction {
    union {
        sys_call_linux_sighandler_fn sa_handler;
        sys_call_linux_sigaction_fn sa_sigaction;
    };
    var_(sa_flags, sys_call_linux_word);
    var_(sa_restorer, sys_call_linux_sigrestore_fn);
    var_(sa_mask, sys_call_linux_sigset);
} sys_call_linux_sigaction;
#endif /* arch_is_x86_64 */

typedef enum sys_call_linux_SIG {
    sys_call_linux_SIGILL = 4,
    sys_call_linux_SIGTRAP = 5,
    sys_call_linux_SIGABRT = 6,
    sys_call_linux_SIGBUS = 7,
    sys_call_linux_SIGFPE = 8,
    sys_call_linux_SIGSEGV = 11,
    sys_call_linux_SIGWINCH = 28,
} sys_call_linux_SIG;

typedef enum sys_call_linux_SA {
    sys_call_linux_SA_SIGINFO = 0x00000004,
    sys_call_linux_SA_RESTORER = 0x04000000,
} sys_call_linux_SA;

#define sys_call_linux_SIG_DFL as$(sys_call_linux_sighandler_fn)(0)

/*---------- <sys/stat.h> ---------------------------------------------------*/

typedef struct sys_call_linux_statx_timestamp {
    i64 tv_sec;
    u32 tv_nsec;
    i32 __reserved;
} sys_call_linux_statx_timestamp;

typedef struct sys_call_linux_statx {
    u32 stx_mask;
    u32 stx_blksize;
    u64 stx_attributes;
    u32 stx_nlink;
    u32 stx_uid;
    u32 stx_gid;
    u16 stx_mode;
    u16 __spare0;
    u64 stx_ino;
    u64 stx_size;
    u64 stx_blocks;
    u64 stx_attributes_mask;
    sys_call_linux_statx_timestamp stx_atime;
    sys_call_linux_statx_timestamp stx_btime;
    sys_call_linux_statx_timestamp stx_ctime;
    sys_call_linux_statx_timestamp stx_mtime;
    u32 stx_rdev_major;
    u32 stx_rdev_minor;
    u32 stx_dev_major;
    u32 stx_dev_minor;
    u64 __spare2[14];
} sys_call_linux_statx;

typedef enum sys_call_linux_S_IF {
    sys_call_linux_S_IFMT = 0170000,
    sys_call_linux_S_IFSOCK = 0140000,
    sys_call_linux_S_IFLNK = 0120000,
    sys_call_linux_S_IFREG = 0100000,
    sys_call_linux_S_IFBLK = 0060000,
    sys_call_linux_S_IFDIR = 0040000,
    sys_call_linux_S_IFCHR = 0020000,
    sys_call_linux_S_IFIFO = 0010000,
} sys_call_linux_S_IF;

typedef enum sys_call_linux_STATX {
    sys_call_linux_STATX_BASIC_STATS = 0x000007ff,
} sys_call_linux_STATX;
typedef sys_call_linux_word sys_call_linux_statx_mask_t;

/*---------- <errno.h> ------------------------------------------------------*/

typedef enum sys_call_linux_Errno {
    sys_call_linux_EAGAIN = 11,
    sys_call_linux_EINPROGRESS = 115,
    sys_call_linux_EINTR = 4,
    sys_call_linux_ENOENT = 2,
    sys_call_linux_ETIMEDOUT = 110,
    sys_call_linux_EWOULDBLOCK = sys_call_linux_EAGAIN,
} sys_call_linux_Errno;

/*---------- <fcntl.h> ------------------------------------------------------*/

typedef enum sys_call_linux_AT {
    sys_call_linux_AT_FDCWD = -100,
    sys_call_linux_AT_EMPTY_PATH = 0x1000,
    sys_call_linux_AT_REMOVEDIR = 0x200,
} sys_call_linux_AT;
typedef sys_call_linux_word sys_call_linux_at_flags_t;

typedef enum sys_call_linux_O {
    sys_call_linux_O_RDONLY = 0,
    sys_call_linux_O_WRONLY = 1,
    sys_call_linux_O_RDWR = 2,
    sys_call_linux_O_CREAT = 00000100,
    sys_call_linux_O_EXCL = 00000200,
    sys_call_linux_O_NOCTTY = 00000400,
    sys_call_linux_O_TRUNC = 00001000,
    sys_call_linux_O_NONBLOCK = 00004000,
    sys_call_linux_O_DIRECTORY = 000200000,
    sys_call_linux_O_CLOEXEC = 02000000,
} sys_call_linux_O;
typedef sys_call_linux_word sys_call_linux_open_flags_t;

typedef enum sys_call_linux_F {
    sys_call_linux_F_GETFL = 3,
    sys_call_linux_F_SETFL = 4,
} sys_call_linux_F;
typedef sys_call_linux_word sys_call_linux_fcntl_cmd_t;

/*---------- <unistd.h> -----------------------------------------------------*/

typedef enum sys_call_linux_SEEK {
    sys_call_linux_SEEK_SET = 0,
    sys_call_linux_SEEK_CUR = 1,
    sys_call_linux_SEEK_END = 2,
} sys_call_linux_SEEK;
typedef sys_call_linux_word sys_call_linux_seek_whence_t;
typedef sys_call_linux_word sys_call_linux_dup_flags_t;

/*---------- <sys/socket.h> -------------------------------------------------*/

typedef enum sys_call_linux_AF {
    sys_call_linux_AF_INET = 2,
    sys_call_linux_AF_INET6 = 10,
} sys_call_linux_AF;
typedef sys_call_linux_word sys_call_linux_sock_family_t;

typedef enum sys_call_linux_SOCK {
    sys_call_linux_SOCK_STREAM = 1,
    sys_call_linux_SOCK_DGRAM = 2,
    sys_call_linux_SOCK_RAW = 3,
    sys_call_linux_SOCK_NONBLOCK = sys_call_linux_O_NONBLOCK,
    sys_call_linux_SOCK_CLOEXEC = sys_call_linux_O_CLOEXEC,
} sys_call_linux_SOCK;
typedef sys_call_linux_word sys_call_linux_sock_type_t;
typedef sys_call_linux_word sys_call_linux_sock_protocol_t;
typedef u32 sys_call_linux_socklen_t;

typedef enum sys_call_linux_SOL {
    sys_call_linux_SOL_SOCKET = 1,
} sys_call_linux_SOL;
typedef sys_call_linux_word sys_call_linux_sock_level_t;

typedef enum sys_call_linux_SO {
    sys_call_linux_SO_REUSEADDR = 2,
    sys_call_linux_SO_BROADCAST = 6,
    sys_call_linux_SO_ERROR = 4,
} sys_call_linux_SO;
typedef sys_call_linux_word sys_call_linux_sock_opt_t;

typedef enum sys_call_linux_IPPROTO {
    sys_call_linux_IPPROTO_TCP = 6,
    sys_call_linux_IPPROTO_UDP = 17,
    sys_call_linux_IPPROTO_IPV6 = 41,
    sys_call_linux_IPPROTO_RAW = 255,
} sys_call_linux_IPPROTO;

typedef enum sys_call_linux_IPV6 {
    sys_call_linux_IPV6_V6ONLY = 26,
} sys_call_linux_IPV6;

typedef enum sys_call_linux_SHUT {
    sys_call_linux_SHUT_RD = 0,
    sys_call_linux_SHUT_WR = 1,
    sys_call_linux_SHUT_RDWR = 2,
} sys_call_linux_SHUT;
typedef sys_call_linux_word sys_call_linux_shutdown_how_t;

typedef struct sys_call_linux_sockaddr {
    u16 sa_family;
    A$$(14, u8) sa_data;
} sys_call_linux_sockaddr;

typedef struct sys_call_linux_in_addr {
    A$$(4, u8) s_addr;
} sys_call_linux_in_addr;

typedef struct sys_call_linux_in6_addr {
    A$$(16, u8) s6_addr;
} sys_call_linux_in6_addr;

typedef struct sys_call_linux_sockaddr_in {
    u16 sin_family;
    u16 sin_port;
    sys_call_linux_in_addr sin_addr;
    A$$(8, u8) sin_zero;
} sys_call_linux_sockaddr_in;

typedef struct sys_call_linux_sockaddr_in6 {
    u16 sin6_family;
    u16 sin6_port;
    u32 sin6_flowinfo;
    sys_call_linux_in6_addr sin6_addr;
    u32 sin6_scope_id;
} sys_call_linux_sockaddr_in6;

typedef struct sys_call_linux_sockaddr_storage {
    u16 ss_family;
    A$$(126, u8) __data;
} sys_call_linux_sockaddr_storage;

/*---------- <sys/mman.h> ---------------------------------------------------*/

typedef enum sys_call_linux_PROT {
    sys_call_linux_PROT_NONE = 0,
    sys_call_linux_PROT_READ = 1,
    sys_call_linux_PROT_WRITE = 2,
    sys_call_linux_PROT_EXEC = 4,
} sys_call_linux_PROT;
typedef sys_call_linux_word sys_call_linux_mmap_prot_t;

typedef enum sys_call_linux_MAP {
    sys_call_linux_MAP_SHARED = 0x01,
    sys_call_linux_MAP_PRIVATE = 0x02,
    sys_call_linux_MAP_ANONYMOUS = 0x20,
} sys_call_linux_MAP;
typedef sys_call_linux_word sys_call_linux_mmap_flags_t;

typedef enum sys_call_linux_MS {
    sys_call_linux_MS_ASYNC = 1,
    sys_call_linux_MS_INVALIDATE = 2,
    sys_call_linux_MS_SYNC = 4,
} sys_call_linux_MS;
typedef sys_call_linux_word sys_call_linux_msync_flags_t;

typedef enum sys_call_linux_MREMAP {
    sys_call_linux_MREMAP_MAYMOVE = 1,
} sys_call_linux_MREMAP;
typedef sys_call_linux_word sys_call_linux_mremap_flags_t;

/*---------- <termios.h> ----------------------------------------------------*/

typedef u32 sys_call_linux_tcflag_t;
typedef u8 sys_call_linux_cc_t;

typedef struct sys_call_linux_termios {
    var_(c_iflag, sys_call_linux_tcflag_t);
    var_(c_oflag, sys_call_linux_tcflag_t);
    var_(c_cflag, sys_call_linux_tcflag_t);
    var_(c_lflag, sys_call_linux_tcflag_t);
    var_(c_line, sys_call_linux_cc_t);
    var_(c_cc, A$$(19, sys_call_linux_cc_t));
} sys_call_linux_termios;

typedef enum sys_call_linux_CC {
    sys_call_linux_VTIME = 5,
    sys_call_linux_VMIN = 6,
} sys_call_linux_CC;

typedef enum sys_call_linux_IFLAG {
    sys_call_linux_BRKINT = 0000002,
    sys_call_linux_INPCK = 0000020,
    sys_call_linux_ISTRIP = 0000040,
    sys_call_linux_ICRNL = 0000400,
    sys_call_linux_IXON = 0002000,
    sys_call_linux_IXOFF = 0010000,
} sys_call_linux_IFLAG;

typedef enum sys_call_linux_OFLAG {
    sys_call_linux_OPOST = 0000001,
} sys_call_linux_OFLAG;

typedef enum sys_call_linux_LFLAG {
    sys_call_linux_ISIG = 0000001,
    sys_call_linux_ICANON = 0000002,
    sys_call_linux_ECHO = 0000010,
    sys_call_linux_IEXTEN = 0100000,
} sys_call_linux_LFLAG;

/*---------- <sys/ioctl.h> --------------------------------------------------*/

typedef struct sys_call_linux_winsize {
    var_(ws_row, u16);
    var_(ws_col, u16);
    var_(ws_xpixel, u16);
    var_(ws_ypixel, u16);
} sys_call_linux_winsize;

typedef enum sys_call_linux_IOCTL {
    sys_call_linux_TCGETS = 0x5401,
    sys_call_linux_TCSETS = 0x5402,
    sys_call_linux_TIOCSCTTY = 0x540e,
    sys_call_linux_TIOCGWINSZ = 0x5413,
    sys_call_linux_TIOCSWINSZ = 0x5414,
    sys_call_linux_FIONREAD = 0x541b,
    sys_call_linux_TIOCGPTN = 0x80045430,
    sys_call_linux_TIOCSPTLCK = 0x40045431,
} sys_call_linux_IOCTL;
typedef sys_call_linux_IOCTL sys_call_linux_ioctl_req_t;

/*---------- <linux/futex.h> ------------------------------------------------*/

typedef enum sys_call_linux_FUTEX {
    sys_call_linux_FUTEX_WAIT = 0,
    sys_call_linux_FUTEX_WAKE = 1,
    sys_call_linux_FUTEX_PRIVATE_FLAG = 128,
} sys_call_linux_FUTEX;

/*---------- <sched.h> ------------------------------------------------------*/

typedef enum sys_call_linux_CLONE {
    sys_call_linux_CLONE_VM = 0x00000100,
    sys_call_linux_CLONE_FS = 0x00000200,
    sys_call_linux_CLONE_FILES = 0x00000400,
    sys_call_linux_CLONE_SIGHAND = 0x00000800,
    sys_call_linux_CLONE_THREAD = 0x00010000,
    sys_call_linux_CLONE_SYSVSEM = 0x00040000,
    sys_call_linux_CLONE_SETTLS = 0x00080000,
    sys_call_linux_CLONE_PARENT_SETTID = 0x00100000,
    sys_call_linux_CLONE_CHILD_CLEARTID = 0x00200000,
} sys_call_linux_CLONE;
typedef sys_call_linux_word sys_call_linux_clone_flags_t;
typedef sys_call_linux_word sys_call_linux_wait_options_t;

/*---------- System Call ABI ------------------------------------------------*/

typedef enum sys_call_linux_SYS {
    sys_call_linux_SYS_read = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(0)),
        pp_case_((arch_type_x86)(3)),
        pp_case_((arch_type_aarch64)(63)),
        pp_case_((arch_type_arm)(3)),
        pp_case_((arch_type_riscv64)(63)),
        pp_case_((arch_type_riscv32)(63)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_write = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(1)),
        pp_case_((arch_type_x86)(4)),
        pp_case_((arch_type_aarch64)(64)),
        pp_case_((arch_type_arm)(4)),
        pp_case_((arch_type_riscv64)(64)),
        pp_case_((arch_type_riscv32)(64)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_close = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(3)),
        pp_case_((arch_type_x86)(6)),
        pp_case_((arch_type_aarch64)(57)),
        pp_case_((arch_type_arm)(6)),
        pp_case_((arch_type_riscv64)(57)),
        pp_case_((arch_type_riscv32)(57)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_getcwd = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(79)),
        pp_case_((arch_type_x86)(183)),
        pp_case_((arch_type_aarch64)(17)),
        pp_case_((arch_type_arm)(183)),
        pp_case_((arch_type_riscv64)(17)),
        pp_case_((arch_type_riscv32)(17)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_chdir = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(80)),
        pp_case_((arch_type_x86)(12)),
        pp_case_((arch_type_aarch64)(49)),
        pp_case_((arch_type_arm)(12)),
        pp_case_((arch_type_riscv64)(49)),
        pp_case_((arch_type_riscv32)(49)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_statx = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(332)),
        pp_case_((arch_type_x86)(383)),
        pp_case_((arch_type_aarch64)(291)),
        pp_case_((arch_type_arm)(397)),
        pp_case_((arch_type_riscv64)(291)),
        pp_case_((arch_type_riscv32)(291)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_lseek = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(8)),
        pp_case_((arch_type_x86)(19)),
        pp_case_((arch_type_aarch64)(62)),
        pp_case_((arch_type_arm)(19)),
        pp_case_((arch_type_riscv64)(62)),
        pp_case_((arch_type_riscv32)(62)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_mmap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(9)),
        pp_case_((arch_type_x86)(192)),
        pp_case_((arch_type_aarch64)(222)),
        pp_case_((arch_type_arm)(192)),
        pp_case_((arch_type_riscv64)(222)),
        pp_case_((arch_type_riscv32)(222)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_mprotect = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(10)),
        pp_case_((arch_type_x86)(125)),
        pp_case_((arch_type_aarch64)(226)),
        pp_case_((arch_type_arm)(125)),
        pp_case_((arch_type_riscv64)(226)),
        pp_case_((arch_type_riscv32)(226)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_munmap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(11)),
        pp_case_((arch_type_x86)(91)),
        pp_case_((arch_type_aarch64)(215)),
        pp_case_((arch_type_arm)(91)),
        pp_case_((arch_type_riscv64)(215)),
        pp_case_((arch_type_riscv32)(215)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_msync = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(26)),
        pp_case_((arch_type_x86)(144)),
        pp_case_((arch_type_aarch64)(227)),
        pp_case_((arch_type_arm)(144)),
        pp_case_((arch_type_riscv64)(227)),
        pp_case_((arch_type_riscv32)(227)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_ioctl = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(16)),
        pp_case_((arch_type_x86)(54)),
        pp_case_((arch_type_aarch64)(29)),
        pp_case_((arch_type_arm)(54)),
        pp_case_((arch_type_riscv64)(29)),
        pp_case_((arch_type_riscv32)(29)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_fcntl = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(72)),
        pp_case_((arch_type_x86)(55)),
        pp_case_((arch_type_aarch64)(25)),
        pp_case_((arch_type_arm)(55)),
        pp_case_((arch_type_riscv64)(25)),
        pp_case_((arch_type_riscv32)(25)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_pread64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(17)),
        pp_case_((arch_type_x86)(180)),
        pp_case_((arch_type_aarch64)(67)),
        pp_case_((arch_type_arm)(180)),
        pp_case_((arch_type_riscv64)(67)),
        pp_case_((arch_type_riscv32)(67)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_pwrite64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(18)),
        pp_case_((arch_type_x86)(181)),
        pp_case_((arch_type_aarch64)(68)),
        pp_case_((arch_type_arm)(181)),
        pp_case_((arch_type_riscv64)(68)),
        pp_case_((arch_type_riscv32)(68)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_openat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(257)),
        pp_case_((arch_type_x86)(295)),
        pp_case_((arch_type_aarch64)(56)),
        pp_case_((arch_type_arm)(322)),
        pp_case_((arch_type_riscv64)(56)),
        pp_case_((arch_type_riscv32)(56)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_readlinkat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(267)),
        pp_case_((arch_type_x86)(305)),
        pp_case_((arch_type_aarch64)(78)),
        pp_case_((arch_type_arm)(332)),
        pp_case_((arch_type_riscv64)(78)),
        pp_case_((arch_type_riscv32)(78)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_pipe2 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(293)),
        pp_case_((arch_type_x86)(331)),
        pp_case_((arch_type_aarch64)(59)),
        pp_case_((arch_type_arm)(359)),
        pp_case_((arch_type_riscv64)(59)),
        pp_case_((arch_type_riscv32)(59)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_socket = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(41)),
        pp_case_((arch_type_x86)(359)),
        pp_case_((arch_type_aarch64)(198)),
        pp_case_((arch_type_arm)(281)),
        pp_case_((arch_type_riscv64)(198)),
        pp_case_((arch_type_riscv32)(198)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_bind = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(49)),
        pp_case_((arch_type_x86)(361)),
        pp_case_((arch_type_aarch64)(200)),
        pp_case_((arch_type_arm)(282)),
        pp_case_((arch_type_riscv64)(200)),
        pp_case_((arch_type_riscv32)(200)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_listen = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(50)),
        pp_case_((arch_type_x86)(363)),
        pp_case_((arch_type_aarch64)(201)),
        pp_case_((arch_type_arm)(284)),
        pp_case_((arch_type_riscv64)(201)),
        pp_case_((arch_type_riscv32)(201)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_accept4 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(288)),
        pp_case_((arch_type_x86)(364)),
        pp_case_((arch_type_aarch64)(242)),
        pp_case_((arch_type_arm)(366)),
        pp_case_((arch_type_riscv64)(242)),
        pp_case_((arch_type_riscv32)(242)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_connect = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(42)),
        pp_case_((arch_type_x86)(362)),
        pp_case_((arch_type_aarch64)(203)),
        pp_case_((arch_type_arm)(283)),
        pp_case_((arch_type_riscv64)(203)),
        pp_case_((arch_type_riscv32)(203)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_getsockname = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(51)),
        pp_case_((arch_type_x86)(367)),
        pp_case_((arch_type_aarch64)(204)),
        pp_case_((arch_type_arm)(286)),
        pp_case_((arch_type_riscv64)(204)),
        pp_case_((arch_type_riscv32)(204)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_setsockopt = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(54)),
        pp_case_((arch_type_x86)(366)),
        pp_case_((arch_type_aarch64)(208)),
        pp_case_((arch_type_arm)(294)),
        pp_case_((arch_type_riscv64)(208)),
        pp_case_((arch_type_riscv32)(208)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_getsockopt = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(55)),
        pp_case_((arch_type_x86)(365)),
        pp_case_((arch_type_aarch64)(209)),
        pp_case_((arch_type_arm)(295)),
        pp_case_((arch_type_riscv64)(209)),
        pp_case_((arch_type_riscv32)(209)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_shutdown = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(48)),
        pp_case_((arch_type_x86)(373)),
        pp_case_((arch_type_aarch64)(210)),
        pp_case_((arch_type_arm)(293)),
        pp_case_((arch_type_riscv64)(210)),
        pp_case_((arch_type_riscv32)(210)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_sendto = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(44)),
        pp_case_((arch_type_x86)(369)),
        pp_case_((arch_type_aarch64)(206)),
        pp_case_((arch_type_arm)(290)),
        pp_case_((arch_type_riscv64)(206)),
        pp_case_((arch_type_riscv32)(206)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_recvfrom = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(45)),
        pp_case_((arch_type_x86)(371)),
        pp_case_((arch_type_aarch64)(207)),
        pp_case_((arch_type_arm)(292)),
        pp_case_((arch_type_riscv64)(207)),
        pp_case_((arch_type_riscv32)(207)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_fsync = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(74)),
        pp_case_((arch_type_x86)(118)),
        pp_case_((arch_type_aarch64)(82)),
        pp_case_((arch_type_arm)(118)),
        pp_case_((arch_type_riscv64)(82)),
        pp_case_((arch_type_riscv32)(82)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_ftruncate = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(77)),
        pp_case_((arch_type_x86)(93)),
        pp_case_((arch_type_aarch64)(46)),
        pp_case_((arch_type_arm)(93)),
        pp_case_((arch_type_riscv64)(46)),
        pp_case_((arch_type_riscv32)(46)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_fchmod = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(91)),
        pp_case_((arch_type_x86)(94)),
        pp_case_((arch_type_aarch64)(52)),
        pp_case_((arch_type_arm)(94)),
        pp_case_((arch_type_riscv64)(52)),
        pp_case_((arch_type_riscv32)(52)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_mkdirat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(258)),
        pp_case_((arch_type_x86)(296)),
        pp_case_((arch_type_aarch64)(34)),
        pp_case_((arch_type_arm)(323)),
        pp_case_((arch_type_riscv64)(34)),
        pp_case_((arch_type_riscv32)(34)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_unlinkat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(263)),
        pp_case_((arch_type_x86)(301)),
        pp_case_((arch_type_aarch64)(35)),
        pp_case_((arch_type_arm)(328)),
        pp_case_((arch_type_riscv64)(35)),
        pp_case_((arch_type_riscv32)(35)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_clock_gettime = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(228)),
        pp_case_((arch_type_x86)(265)),
        pp_case_((arch_type_aarch64)(113)),
        pp_case_((arch_type_arm)(263)),
        pp_case_((arch_type_riscv64)(113)),
        pp_case_((arch_type_riscv32)(403)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_clock_getres = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(229)),
        pp_case_((arch_type_x86)(266)),
        pp_case_((arch_type_aarch64)(114)),
        pp_case_((arch_type_arm)(264)),
        pp_case_((arch_type_riscv64)(114)),
        pp_case_((arch_type_riscv32)(406)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_clock_nanosleep_time64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(230)),
        pp_case_((arch_type_x86)(407)),
        pp_case_((arch_type_aarch64)(115)),
        pp_case_((arch_type_arm)(407)),
        pp_case_((arch_type_riscv64)(115)),
        pp_case_((arch_type_riscv32)(407)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_clone = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(56)),
        pp_case_((arch_type_x86)(120)),
        pp_case_((arch_type_aarch64)(220)),
        pp_case_((arch_type_arm)(120)),
        pp_case_((arch_type_riscv64)(220)),
        pp_case_((arch_type_riscv32)(220)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_setsid = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(112)),
        pp_case_((arch_type_x86)(66)),
        pp_case_((arch_type_aarch64)(157)),
        pp_case_((arch_type_arm)(66)),
        pp_case_((arch_type_riscv64)(157)),
        pp_case_((arch_type_riscv32)(157)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_dup3 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(292)),
        pp_case_((arch_type_x86)(330)),
        pp_case_((arch_type_aarch64)(24)),
        pp_case_((arch_type_arm)(358)),
        pp_case_((arch_type_riscv64)(24)),
        pp_case_((arch_type_riscv32)(24)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_execve = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(59)),
        pp_case_((arch_type_x86)(11)),
        pp_case_((arch_type_aarch64)(221)),
        pp_case_((arch_type_arm)(11)),
        pp_case_((arch_type_riscv64)(221)),
        pp_case_((arch_type_riscv32)(221)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_fchdir = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(81)),
        pp_case_((arch_type_x86)(133)),
        pp_case_((arch_type_aarch64)(50)),
        pp_case_((arch_type_arm)(133)),
        pp_case_((arch_type_riscv64)(50)),
        pp_case_((arch_type_riscv32)(50)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_wait4 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(61)),
        pp_case_((arch_type_x86)(114)),
        pp_case_((arch_type_aarch64)(260)),
        pp_case_((arch_type_arm)(114)),
        pp_case_((arch_type_riscv64)(260)),
        pp_case_((arch_type_riscv32)(260)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_kill = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(62)),
        pp_case_((arch_type_x86)(37)),
        pp_case_((arch_type_aarch64)(129)),
        pp_case_((arch_type_arm)(37)),
        pp_case_((arch_type_riscv64)(129)),
        pp_case_((arch_type_riscv32)(129)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_rt_sigaction = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(13)),
        pp_case_((arch_type_x86)(174)),
        pp_case_((arch_type_aarch64)(134)),
        pp_case_((arch_type_arm)(174)),
        pp_case_((arch_type_riscv64)(134)),
        pp_case_((arch_type_riscv32)(134)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_rt_sigreturn = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(15)),
        pp_case_((arch_type_x86)(173)),
        pp_case_((arch_type_aarch64)(139)),
        pp_case_((arch_type_arm)(173)),
        pp_case_((arch_type_riscv64)(139)),
        pp_case_((arch_type_riscv32)(139)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_gettimeofday = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(96)),
        pp_case_((arch_type_x86)(78)),
        pp_case_((arch_type_aarch64)(169)),
        pp_case_((arch_type_arm)(78)),
        pp_case_((arch_type_riscv64)(169)),
        pp_case_((arch_type_riscv32)(169)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_exit = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(60)),
        pp_case_((arch_type_x86)(1)),
        pp_case_((arch_type_aarch64)(93)),
        pp_case_((arch_type_arm)(1)),
        pp_case_((arch_type_riscv64)(93)),
        pp_case_((arch_type_riscv32)(93)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_exit_group = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(231)),
        pp_case_((arch_type_x86)(252)),
        pp_case_((arch_type_aarch64)(94)),
        pp_case_((arch_type_arm)(248)),
        pp_case_((arch_type_riscv64)(94)),
        pp_case_((arch_type_riscv32)(94)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_nanosleep = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(35)),
        pp_case_((arch_type_x86)(162)),
        pp_case_((arch_type_aarch64)(101)),
        pp_case_((arch_type_arm)(162)),
        pp_case_((arch_type_riscv64)(101)),
        pp_case_((arch_type_riscv32)(0)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_sched_yield = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(24)),
        pp_case_((arch_type_x86)(158)),
        pp_case_((arch_type_aarch64)(124)),
        pp_case_((arch_type_arm)(158)),
        pp_case_((arch_type_riscv64)(124)),
        pp_case_((arch_type_riscv32)(124)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_gettid = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(186)),
        pp_case_((arch_type_x86)(224)),
        pp_case_((arch_type_aarch64)(178)),
        pp_case_((arch_type_arm)(224)),
        pp_case_((arch_type_riscv64)(178)),
        pp_case_((arch_type_riscv32)(178)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_getpid = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(39)),
        pp_case_((arch_type_x86)(20)),
        pp_case_((arch_type_aarch64)(172)),
        pp_case_((arch_type_arm)(20)),
        pp_case_((arch_type_riscv64)(172)),
        pp_case_((arch_type_riscv32)(172)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_sched_getaffinity = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(204)),
        pp_case_((arch_type_x86)(242)),
        pp_case_((arch_type_aarch64)(123)),
        pp_case_((arch_type_arm)(242)),
        pp_case_((arch_type_riscv64)(123)),
        pp_case_((arch_type_riscv32)(123)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_futex = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(202)),
        pp_case_((arch_type_x86)(240)),
        pp_case_((arch_type_aarch64)(98)),
        pp_case_((arch_type_arm)(240)),
        pp_case_((arch_type_riscv64)(98)),
        pp_case_((arch_type_riscv32)(422)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_mremap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(25)),
        pp_case_((arch_type_x86)(163)),
        pp_case_((arch_type_aarch64)(216)),
        pp_case_((arch_type_arm)(163)),
        pp_case_((arch_type_riscv64)(216)),
        pp_case_((arch_type_riscv32)(216)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_renameat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(264)),
        pp_case_((arch_type_x86)(302)),
        pp_case_((arch_type_aarch64)(38)),
        pp_case_((arch_type_arm)(329)),
        pp_default_(0)
    )),
    sys_call_linux_SYS_renameat2 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(316)),
        pp_case_((arch_type_x86)(353)),
        pp_case_((arch_type_aarch64)(276)),
        pp_case_((arch_type_arm)(382)),
        pp_case_((arch_type_riscv64)(276)),
        pp_case_((arch_type_riscv32)(276)),
        pp_default_(0)
    )),
} sys_call_linux_SYS;

$attr($inline)
$static fn_((sys_call_linux_syscall0(sys_call_linux_word n))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall1(sys_call_linux_word n, sys_call_linux_word a1))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall2(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall3(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall4(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall5(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4, sys_call_linux_word a5))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall6(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4, sys_call_linux_word a5, sys_call_linux_word a6))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_syscall_isErr(sys_call_linux_word rc))(bool));
$attr($inline)
$static fn_((sys_call_linux_syscall_err(sys_call_linux_word rc))(sys_call_linux_word));

/*---------- <unistd.h> -----------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_read(sys_call_linux_fd_t fd, void* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_write(sys_call_linux_fd_t fd, const void* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_close(sys_call_linux_fd_t fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_getcwd(char* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_chdir(const char* path))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_lseek(sys_call_linux_fd_t fd, i64 offset, sys_call_linux_seek_whence_t whence))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_pread(sys_call_linux_fd_t fd, void* buf, usize len, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_pwrite(sys_call_linux_fd_t fd, const void* buf, usize len, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fsync(sys_call_linux_fd_t fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_ftruncate(sys_call_linux_fd_t fd, u64 len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fchdir(sys_call_linux_fd_t fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_dup3(sys_call_linux_fd_t old_fd, sys_call_linux_fd_t new_fd, sys_call_linux_dup_flags_t flags))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_pipe2(int fds[2], sys_call_linux_open_flags_t flags))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_readlinkat(sys_call_linux_fd_t dirfd, const char* path, char* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_execve(const char* path, char* const* argv, char* const* envp))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_setsid(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fork(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_clone_thread(
    sys_call_linux_clone_entry_fn entry,
    P$raw child_stack,
    sys_call_linux_clone_flags_t flags,
    P$raw arg,
    i32* parent_tid,
    P$raw tls,
    i32* child_tid
))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_unlinkat(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_at_flags_t flags))(sys_call_linux_word));
$attr($no_return $inline)
$static fn_((sys_call_linux_exit(i32 status))(void));
$attr($no_return $inline)
$static fn_((sys_call_linux_exit_group(i32 status))(void));
$attr($inline)
$static fn_((sys_call_linux_gettid(void))(sys_call_linux_pid_t));
$attr($inline)
$static fn_((sys_call_linux_getpid(void))(sys_call_linux_pid_t));

/*---------- <fcntl.h> ------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_openat(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_open_flags_t flags, sys_call_linux_mode_t mode))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fcntl(sys_call_linux_fd_t fd, sys_call_linux_fcntl_cmd_t cmd, sys_call_linux_word arg))(sys_call_linux_word));

/*---------- <sys/socket.h> -------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_socket(sys_call_linux_sock_family_t domain, sys_call_linux_sock_type_t type, sys_call_linux_sock_protocol_t protocol))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_bind(sys_call_linux_fd_t fd, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_listen(sys_call_linux_fd_t fd, sys_call_linux_word backlog))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_accept4(sys_call_linux_fd_t fd, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* len, sys_call_linux_open_flags_t flags))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_connect(sys_call_linux_fd_t fd, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_getsockname(sys_call_linux_fd_t fd, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_setsockopt(sys_call_linux_fd_t fd, sys_call_linux_sock_level_t level, sys_call_linux_sock_opt_t opt, const void* val, sys_call_linux_socklen_t len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_getsockopt(sys_call_linux_fd_t fd, sys_call_linux_sock_level_t level, sys_call_linux_sock_opt_t opt, void* val, sys_call_linux_socklen_t* len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_shutdown(sys_call_linux_fd_t fd, sys_call_linux_shutdown_how_t how))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_sendto(sys_call_linux_fd_t fd, const void* buf, usize len, sys_call_linux_word flags, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t addr_len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_recvfrom(sys_call_linux_fd_t fd, void* buf, usize len, sys_call_linux_word flags, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* addr_len))(sys_call_linux_word));

/*---------- <sys/stat.h> ---------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_statx_get(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_at_flags_t flags, sys_call_linux_statx_mask_t mask, sys_call_linux_statx* stat))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fchmod(sys_call_linux_fd_t fd, sys_call_linux_mode_t mode))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_mkdirat(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_mode_t mode))(sys_call_linux_word));

/*---------- <sys/ioctl.h> --------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_ioctl(sys_call_linux_fd_t fd, sys_call_linux_ioctl_req_t request, void* arg))(sys_call_linux_word));

/*---------- <sys/mman.h> ---------------------------------------------------*/

#define sys_call_linux_mmap_uses_mmap2 __comp_bool__sys_call_linux_mmap_uses_mmap2
#define sys_call_linux_MMAP2_UNIT __comp_uint__sys_call_linux_MMAP2_UNIT

$attr($inline)
$static fn_((sys_call_linux_mmap(void* addr, usize len, sys_call_linux_mmap_prot_t prot, sys_call_linux_mmap_flags_t flags, sys_call_linux_fd_t fd, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_munmap(void* addr, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_mprotect(void* addr, usize len, sys_call_linux_mmap_prot_t prot))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_msync(void* addr, usize len, sys_call_linux_msync_flags_t flags))(sys_call_linux_word));
$attr($no_return $inline)
$static fn_((sys_call_linux_munmap_exit(P$raw addr, usize len, i32 status))(void));
$attr($inline)
$static fn_((sys_call_linux_mremap(void* old_addr, usize old_size, usize new_size, sys_call_linux_mremap_flags_t flags, void* new_addr))(sys_call_linux_word));

/*---------- <time.h> -------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_clock_gettime(sys_call_linux_clockid_t clock_id, sys_call_linux_timespec* ts))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_clock_getres(sys_call_linux_clockid_t clock_id, sys_call_linux_timespec* ts))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_nanosleep(const sys_call_linux_timespec* req, sys_call_linux_timespec* rem))(sys_call_linux_word));

/*---------- <sys/time.h> ---------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_gettimeofday(sys_call_linux_timeval* tv))(sys_call_linux_word));

/*---------- <sched.h> ------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_sched_yield(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_sched_getaffinity(sys_call_linux_pid_t pid, usize size, void* set))(sys_call_linux_word));

/*---------- <sys/wait.h> ---------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_wait4(sys_call_linux_pid_t pid, int* status, sys_call_linux_wait_options_t options, void* usage))(sys_call_linux_word));

/*---------- <signal.h> -----------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_kill(sys_call_linux_pid_t pid, sys_call_linux_signal_t signal))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_sigemptyset(sys_call_linux_sigset* set))(void));
$attr($inline)
$static fn_((sys_call_linux_siginfo_addr(const sys_call_linux_siginfo* info))(void*));
$attr($inline)
$static fn_((sys_call_linux_rt_sigaction(sys_call_linux_signal_t signal, const sys_call_linux_sigaction* act, sys_call_linux_sigaction* old_act))(sys_call_linux_word));
$attr(comp_naked $no_return $maybe_unused)
$static fn_((sys_call_linux_rt_sigreturn_trampoline(void))(void));

/*---------- <linux/futex.h> ------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_futex(void* addr, sys_call_linux_word op, sys_call_linux_word value, void* timeout, void* addr2, sys_call_linux_word value3))(sys_call_linux_word));

/*---------- <stdio.h> ------------------------------------------------------*/

#define sys_call_linux_has_renameat __comp_bool__sys_call_linux_has_renameat

$attr($inline)
$static fn_((sys_call_linux_renameat(sys_call_linux_word old_dirfd, const char* old_path, sys_call_linux_word new_dirfd, const char* new_path))(sys_call_linux_word));
#endif /* in_analysis || plat_is_linux */

/*========== Macros and Definitions =========================================*/

#if in_analysis || plat_is_linux
#define __comp_bool__sys_call_linux_mmap_uses_mmap2 pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86)(pp_true)), \
        pp_case_((arch_type_arm)(pp_true)), \
        pp_case_((arch_type_riscv32)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)
#define __comp_uint__sys_call_linux_MMAP2_UNIT u64_(4096)

#define __comp_bool__sys_call_linux_has_renameat pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(pp_true)), \
        pp_case_((arch_type_x86)(pp_true)), \
        pp_case_((arch_type_aarch64)(pp_true)), \
        pp_case_((arch_type_arm)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)
#endif /* in_analysis || plat_is_linux */

#if in_analysis_active_only || in_comptime && plat_is_linux
/* NOLINTBEGIN(hicpp-no-assembler) */
fn_((sys_call_linux_syscall0(sys_call_linux_word n))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_volatile("syscall" : "+r"(rax) : : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_volatile("int $0x80" : "+r"(eax) : : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = 0;
            asm_volatile("svc #0" : "=r"(x0) : "r"(x8) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = 0;
            asm_volatile("svc 0" : "=r"(r0) : "r"(r7) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = 0;
            asm_volatile("ecall" : "=r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = 0;
            asm_volatile("ecall" : "=r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall0) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall1(sys_call_linux_word n, sys_call_linux_word a1))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_var_reg(ebx, sys_call_linux_word) = a1;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall1) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall2(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_var_reg(rsi, sys_call_linux_word) = a2;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_var_reg(ebx, sys_call_linux_word) = a1;
            asm_var_reg(ecx, sys_call_linux_word) = a2;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_var_reg(x1, sys_call_linux_word) = a2;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_var_reg(r1, sys_call_linux_word) = a2;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall2) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall3(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_var_reg(rsi, sys_call_linux_word) = a2;
            asm_var_reg(rdx, sys_call_linux_word) = a3;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_var_reg(ebx, sys_call_linux_word) = a1;
            asm_var_reg(ecx, sys_call_linux_word) = a2;
            asm_var_reg(edx, sys_call_linux_word) = a3;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_var_reg(x1, sys_call_linux_word) = a2;
            asm_var_reg(x2, sys_call_linux_word) = a3;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_var_reg(r1, sys_call_linux_word) = a2;
            asm_var_reg(r2, sys_call_linux_word) = a3;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall3) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall4(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_var_reg(rsi, sys_call_linux_word) = a2;
            asm_var_reg(rdx, sys_call_linux_word) = a3;
            asm_var_reg(r10, sys_call_linux_word) = a4;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_var_reg(ebx, sys_call_linux_word) = a1;
            asm_var_reg(ecx, sys_call_linux_word) = a2;
            asm_var_reg(edx, sys_call_linux_word) = a3;
            asm_var_reg(esi, sys_call_linux_word) = a4;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_var_reg(x1, sys_call_linux_word) = a2;
            asm_var_reg(x2, sys_call_linux_word) = a3;
            asm_var_reg(x3, sys_call_linux_word) = a4;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_var_reg(r1, sys_call_linux_word) = a2;
            asm_var_reg(r2, sys_call_linux_word) = a3;
            asm_var_reg(r3, sys_call_linux_word) = a4;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            let_ignore = a4;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall4) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall5(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4, sys_call_linux_word a5))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_var_reg(rsi, sys_call_linux_word) = a2;
            asm_var_reg(rdx, sys_call_linux_word) = a3;
            asm_var_reg(r10, sys_call_linux_word) = a4;
            asm_var_reg(r8, sys_call_linux_word) = a5;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = n;
            asm_var_reg(ebx, sys_call_linux_word) = a1;
            asm_var_reg(ecx, sys_call_linux_word) = a2;
            asm_var_reg(edx, sys_call_linux_word) = a3;
            asm_var_reg(esi, sys_call_linux_word) = a4;
            asm_var_reg(edi, sys_call_linux_word) = a5;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi), "r"(edi) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_var_reg(x1, sys_call_linux_word) = a2;
            asm_var_reg(x2, sys_call_linux_word) = a3;
            asm_var_reg(x3, sys_call_linux_word) = a4;
            asm_var_reg(x4, sys_call_linux_word) = a5;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_var_reg(r1, sys_call_linux_word) = a2;
            asm_var_reg(r2, sys_call_linux_word) = a3;
            asm_var_reg(r3, sys_call_linux_word) = a4;
            asm_var_reg(r4, sys_call_linux_word) = a5;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = a5;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = a5;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            let_ignore = a4;
            let_ignore = a5;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall5) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall6(sys_call_linux_word n, sys_call_linux_word a1, sys_call_linux_word a2, sys_call_linux_word a3, sys_call_linux_word a4, sys_call_linux_word a5, sys_call_linux_word a6))(sys_call_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = n;
            asm_var_reg(rdi, sys_call_linux_word) = a1;
            asm_var_reg(rsi, sys_call_linux_word) = a2;
            asm_var_reg(rdx, sys_call_linux_word) = a3;
            asm_var_reg(r10, sys_call_linux_word) = a4;
            asm_var_reg(r8, sys_call_linux_word) = a5;
            asm_var_reg(r9, sys_call_linux_word) = a6;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            var_(ret, sys_call_linux_word) $undefined;
            asm_volatile(
                "push %%ebp\n\t"
                "movl %[arg6], %%ebp\n\t"
                "movl %[arg5], %%edi\n\t"
                "movl %[arg4], %%esi\n\t"
                "movl %[arg3], %%edx\n\t"
                "movl %[arg2], %%ecx\n\t"
                "movl %[arg1], %%ebx\n\t"
                "movl %[num], %%eax\n\t"
                "int $0x80\n\t"
                "pop %%ebp" : "=a"(ret) : [num] "m"(n),
                [arg1] "m"(a1),
                [arg2] "m"(a2),
                [arg3] "m"(a3),
                [arg4] "m"(a4),
                [arg5] "m"(a5),
                [arg6] "m"(a6) : "ebx", "ecx", "edx", "esi", "edi", "memory"
            );
            return ret;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = n;
            asm_var_reg(x0, sys_call_linux_word) = a1;
            asm_var_reg(x1, sys_call_linux_word) = a2;
            asm_var_reg(x2, sys_call_linux_word) = a3;
            asm_var_reg(x3, sys_call_linux_word) = a4;
            asm_var_reg(x4, sys_call_linux_word) = a5;
            asm_var_reg(x5, sys_call_linux_word) = a6;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = n;
            asm_var_reg(r0, sys_call_linux_word) = a1;
            asm_var_reg(r1, sys_call_linux_word) = a2;
            asm_var_reg(r2, sys_call_linux_word) = a3;
            asm_var_reg(r3, sys_call_linux_word) = a4;
            asm_var_reg(r4, sys_call_linux_word) = a5;
            asm_var_reg(r5, sys_call_linux_word) = a6;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = a5;
            asm_var_(a5r, sys_call_linux_word) $reg(a5) = a6;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r), "r"(a5r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = n;
            asm_var_reg(a0, sys_call_linux_word) = a1;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = a2;
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = a3;
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = a4;
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = a5;
            asm_var_(a5r, sys_call_linux_word) $reg(a5) = a6;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r), "r"(a5r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            let_ignore = a4;
            let_ignore = a5;
            let_ignore = a6;
            claim_unreachable_msg(nameOf(sys_call_linux_syscall6) "is not supported on this platform");
        })
    ));
};

fn_((sys_call_linux_syscall_isErr(sys_call_linux_word rc))(bool)) {
    return rc < 0 && -4095 <= rc;
};

fn_((sys_call_linux_syscall_err(sys_call_linux_word rc))(sys_call_linux_word)) {
    return -rc;
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_read(sys_call_linux_fd_t fd, void* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(
        sys_call_linux_SYS_read,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len)
    );
};

fn_((sys_call_linux_write(sys_call_linux_fd_t fd, const void* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(
        sys_call_linux_SYS_write,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len)
    );
};

fn_((sys_call_linux_close(sys_call_linux_fd_t fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_close, fd);
};

fn_((sys_call_linux_getcwd(char* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_getcwd, (sys_call_linux_word)(buf), (sys_call_linux_word)(len));
};

fn_((sys_call_linux_chdir(const char* path))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_chdir, (sys_call_linux_word)(path));
};

/*---------- <fcntl.h> ------------------------------------------------------*/

fn_((sys_call_linux_openat(
    sys_call_linux_fd_t dirfd,
    const char* path,
    sys_call_linux_open_flags_t flags,
    sys_call_linux_mode_t mode
))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_openat,
        dirfd,
        (sys_call_linux_word)(path),
        flags,
        mode
    );
};

fn_((sys_call_linux_fcntl(sys_call_linux_fd_t fd, sys_call_linux_fcntl_cmd_t cmd, sys_call_linux_word arg))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_fcntl, fd, cmd, arg);
};

/*---------- <sys/stat.h> ---------------------------------------------------*/

fn_((sys_call_linux_statx_get(
    sys_call_linux_fd_t dirfd,
    const char* path,
    sys_call_linux_at_flags_t flags,
    sys_call_linux_statx_mask_t mask,
    sys_call_linux_statx* stat
))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(
        sys_call_linux_SYS_statx,
        dirfd,
        (sys_call_linux_word)(path),
        flags,
        mask,
        (sys_call_linux_word)(stat)
    );
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_lseek(sys_call_linux_fd_t fd, i64 offset, sys_call_linux_seek_whence_t whence))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_lseek, fd, (sys_call_linux_word)(offset), whence);
};

fn_((sys_call_linux_pread(sys_call_linux_fd_t fd, void* buf, usize len, u64 offset))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_pread64,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len),
        (sys_call_linux_word)(offset)
    );
};

fn_((sys_call_linux_pwrite(sys_call_linux_fd_t fd, const void* buf, usize len, u64 offset))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_pwrite64,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len),
        (sys_call_linux_word)(offset)
    );
};

fn_((sys_call_linux_fsync(sys_call_linux_fd_t fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_fsync, fd);
};

fn_((sys_call_linux_ftruncate(sys_call_linux_fd_t fd, u64 len))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_ftruncate, fd, (sys_call_linux_word)(len));
};

fn_((sys_call_linux_fchdir(sys_call_linux_fd_t fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_fchdir, fd);
};

fn_((sys_call_linux_dup3(sys_call_linux_fd_t old_fd, sys_call_linux_fd_t new_fd, sys_call_linux_dup_flags_t flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_dup3, old_fd, new_fd, flags);
};

fn_((sys_call_linux_pipe2(int fds[2], sys_call_linux_open_flags_t flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_pipe2, (sys_call_linux_word)(fds), flags);
};

fn_((sys_call_linux_readlinkat(sys_call_linux_fd_t dirfd, const char* path, char* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(sys_call_linux_SYS_readlinkat, dirfd, (sys_call_linux_word)(path), (sys_call_linux_word)(buf), (sys_call_linux_word)(len));
};

fn_((sys_call_linux_execve(const char* path, char* const* argv, char* const* envp))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(
        sys_call_linux_SYS_execve,
        (sys_call_linux_word)(path),
        (sys_call_linux_word)(argv),
        (sys_call_linux_word)(envp)
    );
};

fn_((sys_call_linux_setsid(void))(sys_call_linux_word)) {
    return sys_call_linux_syscall0(sys_call_linux_SYS_setsid);
};

/*---------- <sys/socket.h> -------------------------------------------------*/

fn_((sys_call_linux_socket(sys_call_linux_sock_family_t domain, sys_call_linux_sock_type_t type, sys_call_linux_sock_protocol_t protocol))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_socket, domain, type, protocol);
};

fn_((sys_call_linux_bind(sys_call_linux_fd_t fd, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_bind, fd, (sys_call_linux_word)(addr), len);
};

fn_((sys_call_linux_listen(sys_call_linux_fd_t fd, sys_call_linux_word backlog))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_listen, fd, backlog);
};

fn_((sys_call_linux_accept4(sys_call_linux_fd_t fd, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* len, sys_call_linux_open_flags_t flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(sys_call_linux_SYS_accept4, fd, (sys_call_linux_word)(addr), (sys_call_linux_word)(len), flags);
};

fn_((sys_call_linux_connect(sys_call_linux_fd_t fd, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_connect, fd, (sys_call_linux_word)(addr), len);
};

fn_((sys_call_linux_getsockname(sys_call_linux_fd_t fd, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_getsockname, fd, (sys_call_linux_word)(addr), (sys_call_linux_word)(len));
};

fn_((sys_call_linux_setsockopt(sys_call_linux_fd_t fd, sys_call_linux_sock_level_t level, sys_call_linux_sock_opt_t opt, const void* val, sys_call_linux_socklen_t len))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(sys_call_linux_SYS_setsockopt, fd, level, opt, (sys_call_linux_word)(val), len);
};

fn_((sys_call_linux_getsockopt(sys_call_linux_fd_t fd, sys_call_linux_sock_level_t level, sys_call_linux_sock_opt_t opt, void* val, sys_call_linux_socklen_t* len))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(sys_call_linux_SYS_getsockopt, fd, level, opt, (sys_call_linux_word)(val), (sys_call_linux_word)(len));
};

fn_((sys_call_linux_shutdown(sys_call_linux_fd_t fd, sys_call_linux_shutdown_how_t how))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_shutdown, fd, how);
};

fn_((sys_call_linux_sendto(sys_call_linux_fd_t fd, const void* buf, usize len, sys_call_linux_word flags, const sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t addr_len))(sys_call_linux_word)) {
    return sys_call_linux_syscall6(sys_call_linux_SYS_sendto, fd, (sys_call_linux_word)(buf), (sys_call_linux_word)(len), flags, (sys_call_linux_word)(addr), addr_len);
};

fn_((sys_call_linux_recvfrom(sys_call_linux_fd_t fd, void* buf, usize len, sys_call_linux_word flags, sys_call_linux_sockaddr* addr, sys_call_linux_socklen_t* addr_len))(sys_call_linux_word)) {
    return sys_call_linux_syscall6(sys_call_linux_SYS_recvfrom, fd, (sys_call_linux_word)(buf), (sys_call_linux_word)(len), flags, (sys_call_linux_word)(addr), (sys_call_linux_word)(addr_len));
};

/*---------- <sys/stat.h> ---------------------------------------------------*/

fn_((sys_call_linux_fchmod(sys_call_linux_fd_t fd, sys_call_linux_mode_t mode))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_fchmod, fd, (sys_call_linux_word)(mode));
};

fn_((sys_call_linux_mkdirat(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_mode_t mode))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_mkdirat, dirfd, (sys_call_linux_word)(path), (sys_call_linux_word)(mode));
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_unlinkat(sys_call_linux_fd_t dirfd, const char* path, sys_call_linux_at_flags_t flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_unlinkat, dirfd, (sys_call_linux_word)(path), flags);
};

/*---------- <sys/ioctl.h> --------------------------------------------------*/

fn_((sys_call_linux_ioctl(sys_call_linux_fd_t fd, sys_call_linux_ioctl_req_t request, void* arg))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_ioctl, fd, request, (sys_call_linux_word)(arg));
};

/*---------- <sys/mman.h> ---------------------------------------------------*/

fn_((sys_call_linux_mmap(
    void* addr,
    usize len,
    sys_call_linux_mmap_prot_t prot,
    sys_call_linux_mmap_flags_t flags,
    sys_call_linux_fd_t fd,
    u64 offset
))(sys_call_linux_word)) {
    let syscall_offset = pp_if_(sys_call_linux_mmap_uses_mmap2)(
        pp_then_(offset / sys_call_linux_MMAP2_UNIT),
        pp_else_(offset));
    return sys_call_linux_syscall6(
        sys_call_linux_SYS_mmap,
        (sys_call_linux_word)(addr),
        (sys_call_linux_word)(len),
        prot,
        flags,
        fd,
        (sys_call_linux_word)(syscall_offset)
    );
};

fn_((sys_call_linux_munmap(void* addr, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_munmap, (sys_call_linux_word)(addr), (sys_call_linux_word)(len));
};

fn_((sys_call_linux_mprotect(void* addr, usize len, sys_call_linux_mmap_prot_t prot))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_mprotect, (sys_call_linux_word)(addr), (sys_call_linux_word)(len), prot);
};

fn_((sys_call_linux_msync(void* addr, usize len, sys_call_linux_msync_flags_t flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_msync, (sys_call_linux_word)(addr), (sys_call_linux_word)(len), flags);
};

fn_((sys_call_linux_munmap_exit(P$raw addr, usize len, i32 status))(void)) pp_switch_((arch_type)(
    pp_case_((arch_type_x86_64)({
        asm_var_reg(rdi, P$raw) = addr;
        asm_var_reg(rsi, usize) = len;
        asm_var_(exit_status, i32) $reg(rdx) = status;
        asm_volatile(
            "movq %0, %%rax\n\t"
            "syscall\n\t"
            "movq %1, %%rax\n\t"
            "movslq %%edx, %%rdi\n\t"
            "syscall" : : "i"(sys_call_linux_SYS_munmap),
            "i"(sys_call_linux_SYS_exit),
            "r"(rdi),
            "r"(rsi),
            "r"(exit_status) : "memory", "rax", "rcx", "r11"
        );
        claim_unreachable;
    })),
    pp_case_((arch_type_x86)({
        asm_var_reg(ebx, P$raw) = addr;
        asm_var_reg(ecx, usize) = len;
        asm_var_(exit_status, i32) $reg(edx) = status;
        __asm__ __volatile__(
            "movl %0, %%eax\n\t"
            "int $0x80\n\t"
            "movl %1, %%eax\n\t"
            "movl %%edx, %%ebx\n\t"
            "int $0x80"
            :
            : "i"(sys_call_linux_SYS_munmap),
              "i"(sys_call_linux_SYS_exit),
              "r"(ebx),
              "r"(ecx),
              "r"(exit_status)
            : "memory", "eax"
        );
        claim_unreachable;
    })),
    pp_case_((arch_type_aarch64)({
        asm_var_reg(x0, P$raw) = addr;
        asm_var_reg(x1, usize) = len;
        asm_var_(exit_status, i32) $reg(x2) = status;
        asm_volatile(
            "mov x8, %0\n\t"
            "svc 0\n\t"
            "mov x8, %1\n\t"
            "sxtw x0, w2\n\t"
            "svc 0" : : "i"(sys_call_linux_SYS_munmap),
            "i"(sys_call_linux_SYS_exit),
            "r"(x0),
            "r"(x1),
            "r"(exit_status) : "memory", "x8"
        );
        claim_unreachable;
    })),
    pp_case_((arch_type_arm)({
        asm_var_reg(r0, P$raw) = addr;
        asm_var_reg(r1, usize) = len;
        asm_var_(exit_status, i32) $reg(r2) = status;
        asm_volatile(
            "mov r7, %0\n\t"
            "svc 0\n\t"
            "mov r7, %1\n\t"
            "mov r0, r2\n\t"
            "svc 0" : : "i"(sys_call_linux_SYS_munmap),
            "i"(sys_call_linux_SYS_exit),
            "r"(r0),
            "r"(r1),
            "r"(exit_status) : "memory", "r7"
        );
        claim_unreachable;
    })),
    pp_case_((arch_type_riscv64)({
        asm_var_reg(a0, P$raw) = addr;
        asm_var_(a1r, usize) $reg(a1) = len;
        asm_var_(exit_status, i32) $reg(a2) = status;
        asm_volatile(
            "li a7, %0\n\t"
            "ecall\n\t"
            "li a7, %1\n\t"
            "mv a0, a2\n\t"
            "ecall" : : "i"(sys_call_linux_SYS_munmap),
            "i"(sys_call_linux_SYS_exit),
            "r"(a0),
            "r"(a1r),
            "r"(exit_status) : "memory", "a7"
        );
        claim_unreachable;
    })),
    pp_case_((arch_type_riscv32)({
        asm_var_reg(a0, P$raw) = addr;
        asm_var_(a1r, usize) $reg(a1) = len;
        asm_var_(exit_status, i32) $reg(a2) = status;
        asm_volatile(
            "li a7, %0\n\t"
            "ecall\n\t"
            "li a7, %1\n\t"
            "mv a0, a2\n\t"
            "ecall" : : "i"(sys_call_linux_SYS_munmap),
            "i"(sys_call_linux_SYS_exit),
            "r"(a0),
            "r"(a1r),
            "r"(exit_status) : "memory", "a7"
        );
        claim_unreachable;
    })),
    pp_default_({
        let_ignore = addr;
        let_ignore = len;
        let_ignore = status;
        claim_unreachable_msg(nameOf(sys_call_linux_munmap_exit) "is not supported on this platform");
    })
));

/*---------- <time.h> -------------------------------------------------------*/

fn_((sys_call_linux_clock_gettime(sys_call_linux_clockid_t clock_id, sys_call_linux_timespec* ts))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_clock_gettime, clock_id, (sys_call_linux_word)(ts));
};

fn_((sys_call_linux_clock_getres(sys_call_linux_clockid_t clock_id, sys_call_linux_timespec* ts))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_clock_getres, clock_id, (sys_call_linux_word)(ts));
};

/*---------- <sys/time.h> ---------------------------------------------------*/

fn_((sys_call_linux_gettimeofday(sys_call_linux_timeval* tv))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_gettimeofday, (sys_call_linux_word)(tv), 0);
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_exit(i32 status))(void)) {
    let_ignore = sys_call_linux_syscall1(sys_call_linux_SYS_exit, as$(sys_call_linux_word)(status));
    claim_unreachable;
};

fn_((sys_call_linux_exit_group(i32 status))(void)) {
    let_ignore = sys_call_linux_syscall1(sys_call_linux_SYS_exit_group, as$(sys_call_linux_word)(status));
    claim_unreachable;
};

/*---------- <time.h> -------------------------------------------------------*/

fn_((sys_call_linux_nanosleep(const sys_call_linux_timespec* req, sys_call_linux_timespec* rem))(sys_call_linux_word)) {
    return pp_if_(arch_is_riscv32)(
        pp_then_(sys_call_linux_syscall4(
            sys_call_linux_SYS_clock_nanosleep_time64,
            sys_call_linux_CLOCK_MONOTONIC,
            0,
            (sys_call_linux_word)(req),
            (sys_call_linux_word)(rem)
        )),
        pp_else_(sys_call_linux_syscall2(
            sys_call_linux_SYS_nanosleep,
            (sys_call_linux_word)(req),
            (sys_call_linux_word)(rem)
        )));
};

/*---------- <sched.h> ------------------------------------------------------*/

fn_((sys_call_linux_sched_yield(void))(sys_call_linux_word)) {
    return sys_call_linux_syscall0(sys_call_linux_SYS_sched_yield);
};

fn_((sys_call_linux_sched_getaffinity(
    sys_call_linux_pid_t pid,
    usize size,
    void* set
))(sys_call_linux_word)) {
    let rc = sys_call_linux_syscall3(
        sys_call_linux_SYS_sched_getaffinity,
        pid,
        (sys_call_linux_word)(size),
        (sys_call_linux_word)(set)
    );
    if (rc < 0) return rc;
    if (as$(usize)(rc) < size) {
        raw_memset0(as$(u8*)(set) + rc, size - as$(usize)(rc));
    }
    return 0;
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_gettid(void))(sys_call_linux_pid_t)) {
    return as$(sys_call_linux_pid_t)(sys_call_linux_syscall0(sys_call_linux_SYS_gettid));
};

fn_((sys_call_linux_getpid(void))(sys_call_linux_pid_t)) {
    return as$(sys_call_linux_pid_t)(sys_call_linux_syscall0(sys_call_linux_SYS_getpid));
};

fn_((sys_call_linux_fork(void))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(sys_call_linux_SYS_clone, 17, 0, 0, 0, 0);
};

fn_((sys_call_linux_clone_thread(
    sys_call_linux_clone_entry_fn entry,
    P$raw child_stack,
    sys_call_linux_clone_flags_t flags,
    P$raw arg,
    i32* parent_tid,
    P$raw tls,
    i32* child_tid
))(sys_call_linux_word)) {
    claim_assert_nonnull(entry);
    claim_assert_nonnull(child_stack);
    var_(stack, sys_call_linux_word*) = as$(sys_call_linux_word*)(child_stack);
    stack -= 2;
    stack[0] = as$(sys_call_linux_word)(entry);
    stack[1] = as$(sys_call_linux_word)(arg);
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(rdi, sys_call_linux_word) = flags;
            asm_var_reg(rsi, sys_call_linux_word) = as$(sys_call_linux_word)(stack);
            asm_var_reg(rdx, sys_call_linux_word) = as$(sys_call_linux_word)(parent_tid);
            asm_var_reg(r10, sys_call_linux_word) = as$(sys_call_linux_word)(child_tid);
            asm_var_reg(r8, sys_call_linux_word) = as$(sys_call_linux_word)(tls);
            asm_volatile(
                "syscall\n\t"
                "testq %%rax, %%rax\n\t"
                "jnz 1f\n\t"
                "popq %%r12\n\t"
                "popq %%rdi\n\t"
                "call *%%r12\n\t"
                "movslq %%eax, %%rdi\n\t"
                "movq %1, %%rax\n\t"
                "syscall\n\t"
                "ud2\n"
                "1:" : "+r"(rax) : "i"(sys_call_linux_SYS_exit),
                "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8) : "rcx", "r11", "r12", "memory"
            );
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(ebx, sys_call_linux_word) = flags;
            asm_var_reg(ecx, sys_call_linux_word) = as$(sys_call_linux_word)(stack);
            asm_var_reg(edx, sys_call_linux_word) = as$(sys_call_linux_word)(parent_tid);
            asm_var_reg(esi, sys_call_linux_word) = as$(sys_call_linux_word)(tls);
            asm_var_reg(edi, sys_call_linux_word) = as$(sys_call_linux_word)(child_tid);
            asm_volatile(
                "int $0x80\n\t"
                "testl %%eax, %%eax\n\t"
                "jnz 1f\n\t"
                "popl %%ecx\n\t"
                "popl %%ebx\n\t"
                "pushl %%ebx\n\t"
                "call *%%ecx\n\t"
                "movl %%eax, %%ebx\n\t"
                "movl %1, %%eax\n\t"
                "int $0x80\n\t"
                "ud2\n"
                "1:" : "+r"(eax) : "i"(sys_call_linux_SYS_exit),
                "r"(ebx), "r"(ecx), "r"(edx), "r"(esi), "r"(edi) : "memory"
            );
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(x0, sys_call_linux_word) = flags;
            asm_var_reg(x1, sys_call_linux_word) = as$(sys_call_linux_word)(stack);
            asm_var_reg(x2, sys_call_linux_word) = as$(sys_call_linux_word)(parent_tid);
            asm_var_reg(x3, sys_call_linux_word) = as$(sys_call_linux_word)(tls);
            asm_var_reg(x4, sys_call_linux_word) = as$(sys_call_linux_word)(child_tid);
            asm_volatile(
                "svc #0\n\t"
                "cbnz x0, 1f\n\t"
                "ldp x9, x0, [sp], #16\n\t"
                "blr x9\n\t"
                "mov x8, %1\n\t"
                "svc #0\n\t"
                "brk #0\n"
                "1:" : "+r"(x0) : "i"(sys_call_linux_SYS_exit),
                "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4) : "x9", "memory"
            );
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(r0, sys_call_linux_word) = flags;
            asm_var_reg(r1, sys_call_linux_word) = as$(sys_call_linux_word)(stack);
            asm_var_reg(r2, sys_call_linux_word) = as$(sys_call_linux_word)(parent_tid);
            asm_var_reg(r3, sys_call_linux_word) = as$(sys_call_linux_word)(tls);
            asm_var_reg(r4, sys_call_linux_word) = as$(sys_call_linux_word)(child_tid);
            asm_volatile(
                "svc 0\n\t"
                "cmp r0, #0\n\t"
                "bne 1f\n\t"
                "ldr r4, [sp], #4\n\t"
                "ldr r0, [sp], #4\n\t"
                "blx r4\n\t"
                "mov r7, %1\n\t"
                "svc 0\n\t"
                ".inst 0xe7f000f0\n"
                "1:" : "+r"(r0) : "i"(sys_call_linux_SYS_exit),
                "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4) : "memory"
            );
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(a0, sys_call_linux_word) = flags;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = as$(sys_call_linux_word)(stack);
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = as$(sys_call_linux_word)(parent_tid);
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = as$(sys_call_linux_word)(tls);
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = as$(sys_call_linux_word)(child_tid);
            asm_volatile(
                "ecall\n\t"
                "bnez a0, 1f\n\t"
                "ld t0, 0(sp)\n\t"
                "ld a0, 8(sp)\n\t"
                "addi sp, sp, 16\n\t"
                "jalr ra, t0\n\t"
                "li a7, %1\n\t"
                "ecall\n\t"
                "unimp\n"
                "1:" : "+r"(a0) : "i"(sys_call_linux_SYS_exit),
                "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r) : "t0", "memory"
            );
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, sys_call_linux_word) = sys_call_linux_SYS_clone;
            asm_var_reg(a0, sys_call_linux_word) = flags;
            asm_var_(a1r, sys_call_linux_word) $reg(a1) = as$(sys_call_linux_word)(stack);
            asm_var_(a2r, sys_call_linux_word) $reg(a2) = as$(sys_call_linux_word)(parent_tid);
            asm_var_(a3r, sys_call_linux_word) $reg(a3) = as$(sys_call_linux_word)(tls);
            asm_var_(a4r, sys_call_linux_word) $reg(a4) = as$(sys_call_linux_word)(child_tid);
            asm_volatile(
                "ecall\n\t"
                "bnez a0, 1f\n\t"
                "lw t0, 0(sp)\n\t"
                "lw a0, 4(sp)\n\t"
                "addi sp, sp, 8\n\t"
                "jalr ra, t0\n\t"
                "li a7, %1\n\t"
                "ecall\n\t"
                "unimp\n"
                "1:" : "+r"(a0) : "i"(sys_call_linux_SYS_exit),
                "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r) : "t0", "memory"
            );
            return a0;
        })),
        pp_default_({
            let_ignore = flags;
            let_ignore = parent_tid;
            let_ignore = tls;
            let_ignore = child_tid;
            claim_unreachable_msg(nameOf(sys_call_linux_clone_thread) "is not supported on this platform");
        })
    ));
};

/*---------- <sys/wait.h> ---------------------------------------------------*/

fn_((sys_call_linux_wait4(sys_call_linux_pid_t pid, int* status, sys_call_linux_wait_options_t options, void* usage))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_wait4,
        pid,
        (sys_call_linux_word)(status),
        options,
        (sys_call_linux_word)(usage)
    );
};

/*---------- <signal.h> -----------------------------------------------------*/

fn_((sys_call_linux_kill(sys_call_linux_pid_t pid, sys_call_linux_signal_t signal))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_kill, pid, signal);
};

fn_((sys_call_linux_sigemptyset(sys_call_linux_sigset* set))(void)) {
    raw_memset0(set, sizeOf$(sys_call_linux_sigset));
};

fn_((sys_call_linux_siginfo_addr(const sys_call_linux_siginfo* info))(void*)) {
    let bytes = as$(const u8*)(info);
    var_(addr, void*) $undefined;
    let offset = pp_if_(arch_bits_is_64bit)(pp_then_(as$(usize)(16)), pp_else_(as$(usize)(12)));
    raw_memcpy(&addr, bytes + offset, sizeOf$(void*));
    return addr;
};

fn_((sys_call_linux_rt_sigaction(
    sys_call_linux_signal_t signal,
    const sys_call_linux_sigaction* act,
    sys_call_linux_sigaction* old_act
))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_rt_sigaction,
        signal,
        (sys_call_linux_word)(act),
        (sys_call_linux_word)(old_act),
        sizeOf$(sys_call_linux_sigset)
    );
};

fn_((sys_call_linux_rt_sigreturn_trampoline(void))(void)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(asm_volatile(
            "movq %0, %%rax\n\t"
            "syscall" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "rax",
            "rcx", "r11", "memory"
        ))),
        pp_case_((arch_type_x86)(asm_volatile(
            "movl %0, %%eax\n\t"
            "int $0x80" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "eax",
            "memory"
        ))),
        pp_case_((arch_type_aarch64)(asm_volatile(
            "mov x8, %0\n\t"
            "svc #0" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "x8",
            "memory"
        ))),
        pp_case_((arch_type_arm)(asm_volatile(
            "mov r7, %0\n\t"
            "svc 0" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "r7",
            "memory"
        ))),
        pp_case_((arch_type_riscv64)(asm_volatile(
            "li a7, %0\n\t"
            "ecall" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "a7",
            "memory"
        ))),
        pp_case_((arch_type_riscv32)(asm_volatile(
            "li a7, %0\n\t"
            "ecall" : : "i"(sys_call_linux_SYS_rt_sigreturn) : "a7",
            "memory"
        ))),
        pp_default_(claim_unreachable)
    ));
};

/*---------- <linux/futex.h> ------------------------------------------------*/

fn_((sys_call_linux_futex(
    void* addr,
    sys_call_linux_word op,
    sys_call_linux_word value,
    void* timeout,
    void* addr2,
    sys_call_linux_word value3
))(sys_call_linux_word)) {
    return sys_call_linux_syscall6(
        sys_call_linux_SYS_futex,
        (sys_call_linux_word)(addr),
        op,
        value,
        (sys_call_linux_word)(timeout),
        (sys_call_linux_word)(addr2),
        value3
    );
};

/*---------- <sys/mman.h> ---------------------------------------------------*/

fn_((sys_call_linux_mremap(
    void* old_addr,
    usize old_size,
    usize new_size,
    sys_call_linux_mremap_flags_t flags,
    void* new_addr
))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(
        sys_call_linux_SYS_mremap,
        (sys_call_linux_word)(old_addr),
        (sys_call_linux_word)(old_size),
        (sys_call_linux_word)(new_size),
        flags,
        (sys_call_linux_word)(new_addr)
    );
};

/*---------- <stdio.h> ------------------------------------------------------*/

fn_((sys_call_linux_renameat(
    sys_call_linux_word old_dirfd,
    const char* old_path,
    sys_call_linux_word new_dirfd,
    const char* new_path
))(sys_call_linux_word)) {
    return pp_if_(sys_call_linux_has_renameat)(
        pp_then_(sys_call_linux_syscall4(
            sys_call_linux_SYS_renameat,
            old_dirfd,
            (sys_call_linux_word)(old_path),
            new_dirfd,
            (sys_call_linux_word)(new_path)
        )),
        pp_else_(sys_call_linux_syscall5(
            sys_call_linux_SYS_renameat2,
            old_dirfd,
            (sys_call_linux_word)(old_path),
            new_dirfd,
            (sys_call_linux_word)(new_path),
            0
        )));
};
/* NOLINTEND(hicpp-no-assembler) */
#endif /* in_analysis_active_only || in_comptime && plat_is_linux */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_call_linux__included */
