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

typedef isize sys_call_linux_word;
typedef u32 sys_call_linux_mode_t;

/*---------- <time.h> -------------------------------------------------------*/

typedef struct sys_call_linux_timespec {
    var_(tv_sec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(sys_call_linux_word)));
    var_(tv_nsec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(sys_call_linux_word)));
} sys_call_linux_timespec;

typedef enum sys_call_linux_CLOCK {
    sys_call_linux_CLOCK_REALTIME = 0,
    sys_call_linux_CLOCK_MONOTONIC = 1,
} sys_call_linux_CLOCK;
typedef sys_call_linux_CLOCK sys_call_linux_clockid_t;

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

/*---------- <errno.h> ------------------------------------------------------*/

typedef enum sys_call_linux_Errno {
    sys_call_linux_EINTR = 4,
    sys_call_linux_ETIMEDOUT = 110,
} sys_call_linux_Errno;

/*---------- <fcntl.h> ------------------------------------------------------*/

typedef enum sys_call_linux_AT {
    sys_call_linux_AT_FDCWD = -100,
    sys_call_linux_AT_EMPTY_PATH = 0x1000,
    sys_call_linux_AT_REMOVEDIR = 0x200,
} sys_call_linux_AT;

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
} sys_call_linux_O;

/*---------- <unistd.h> -----------------------------------------------------*/

typedef enum sys_call_linux_SEEK {
    sys_call_linux_SEEK_SET = 0,
    sys_call_linux_SEEK_CUR = 1,
    sys_call_linux_SEEK_END = 2,
} sys_call_linux_SEEK;

/*---------- <sys/mman.h> ---------------------------------------------------*/

typedef enum sys_call_linux_PROT {
    sys_call_linux_PROT_NONE = 0,
    sys_call_linux_PROT_READ = 1,
    sys_call_linux_PROT_WRITE = 2,
    sys_call_linux_PROT_EXEC = 4,
} sys_call_linux_PROT;

typedef enum sys_call_linux_MAP {
    sys_call_linux_MAP_SHARED = 0x01,
    sys_call_linux_MAP_PRIVATE = 0x02,
    sys_call_linux_MAP_ANONYMOUS = 0x20,
} sys_call_linux_MAP;

typedef enum sys_call_linux_MS {
    sys_call_linux_MS_ASYNC = 1,
    sys_call_linux_MS_INVALIDATE = 2,
    sys_call_linux_MS_SYNC = 4,
} sys_call_linux_MS;

typedef enum sys_call_linux_MREMAP {
    sys_call_linux_MREMAP_MAYMOVE = 1,
} sys_call_linux_MREMAP;

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
$static fn_((sys_call_linux_read(sys_call_linux_word fd, void* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_write(sys_call_linux_word fd, const void* buf, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_close(sys_call_linux_word fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_lseek(sys_call_linux_word fd, i64 offset, sys_call_linux_word whence))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_pread(sys_call_linux_word fd, void* buf, usize len, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_pwrite(sys_call_linux_word fd, const void* buf, usize len, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fsync(sys_call_linux_word fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_ftruncate(sys_call_linux_word fd, u64 len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fchdir(sys_call_linux_word fd))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_dup3(sys_call_linux_word old_fd, sys_call_linux_word new_fd, sys_call_linux_word flags))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_execve(const char* path, char* const* argv, char* const* envp))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_setsid(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fork(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_unlinkat(sys_call_linux_word dirfd, const char* path, sys_call_linux_word flags))(sys_call_linux_word));
$attr($no_return $inline)
$static fn_((sys_call_linux_exit(i32 status))(void));
$attr($no_return $inline)
$static fn_((sys_call_linux_exit_group(i32 status))(void));
$attr($inline)
$static fn_((sys_call_linux_gettid(void))(sys_call_linux_word));

/*---------- <fcntl.h> ------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_openat(sys_call_linux_word dirfd, const char* path, sys_call_linux_word flags, sys_call_linux_word mode))(sys_call_linux_word));

/*---------- <sys/stat.h> ---------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_statx_get(sys_call_linux_word dirfd, const char* path, sys_call_linux_word flags, sys_call_linux_word mask, sys_call_linux_statx* stat))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_fchmod(sys_call_linux_word fd, sys_call_linux_mode_t mode))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_mkdirat(sys_call_linux_word dirfd, const char* path, sys_call_linux_mode_t mode))(sys_call_linux_word));

/*---------- <sys/ioctl.h> --------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_ioctl(sys_call_linux_word fd, sys_call_linux_word request, void* arg))(sys_call_linux_word));

/*---------- <sys/mman.h> ---------------------------------------------------*/

#define sys_call_linux_mmap_uses_mmap2 __comp_bool__sys_call_linux_mmap_uses_mmap2
#define sys_call_linux_MMAP2_UNIT __comp_uint__sys_call_linux_MMAP2_UNIT

$attr($inline)
$static fn_((sys_call_linux_mmap(void* addr, usize len, sys_call_linux_word prot, sys_call_linux_word flags, sys_call_linux_word fd, u64 offset))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_munmap(void* addr, usize len))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_mprotect(void* addr, usize len, sys_call_linux_word prot))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_msync(void* addr, usize len, sys_call_linux_word flags))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_mremap(void* old_addr, usize old_size, usize new_size, sys_call_linux_word flags, void* new_addr))(sys_call_linux_word));

/*---------- <time.h> -------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_clock_gettime(sys_call_linux_word clock_id, void* ts))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_nanosleep(void* req, void* rem))(sys_call_linux_word));

/*---------- <sched.h> ------------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_sched_yield(void))(sys_call_linux_word));
$attr($inline)
$static fn_((sys_call_linux_sched_getaffinity(sys_call_linux_word pid, usize size, void* set))(sys_call_linux_word));

/*---------- <sys/wait.h> ---------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_wait4(sys_call_linux_word pid, int* status, sys_call_linux_word options, void* usage))(sys_call_linux_word));

/*---------- <signal.h> -----------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_kill(sys_call_linux_word pid, sys_call_linux_word signal))(sys_call_linux_word));

/*---------- <linux/futex.h> ------------------------------------------------*/

$attr($inline)
$static fn_((sys_call_linux_futex(void* addr, sys_call_linux_word op, sys_call_linux_word value, void* timeout, void* addr2, sys_call_linux_word value3))(sys_call_linux_word));

/*---------- <stdio.h> ------------------------------------------------------*/

#define sys_call_linux_has_renameat __comp_bool__sys_call_linux_has_renameat

$attr($inline)
$static fn_((sys_call_linux_renameat(sys_call_linux_word old_dirfd, const char* old_path, sys_call_linux_word new_dirfd, const char* new_path))(sys_call_linux_word));

/*========== Macros and Definitions =========================================*/

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

#if on_analysis_active_only || on_comptime
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

fn_((sys_call_linux_read(sys_call_linux_word fd, void* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(
        sys_call_linux_SYS_read,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len)
    );
};

fn_((sys_call_linux_write(sys_call_linux_word fd, const void* buf, usize len))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(
        sys_call_linux_SYS_write,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len)
    );
};

fn_((sys_call_linux_close(sys_call_linux_word fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_close, fd);
};

/*---------- <fcntl.h> ------------------------------------------------------*/

fn_((sys_call_linux_openat(
    sys_call_linux_word dirfd,
    const char* path,
    sys_call_linux_word flags,
    sys_call_linux_word mode
))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_openat,
        dirfd,
        (sys_call_linux_word)(path),
        flags,
        mode
    );
};

/*---------- <sys/stat.h> ---------------------------------------------------*/

fn_((sys_call_linux_statx_get(
    sys_call_linux_word dirfd,
    const char* path,
    sys_call_linux_word flags,
    sys_call_linux_word mask,
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

fn_((sys_call_linux_lseek(sys_call_linux_word fd, i64 offset, sys_call_linux_word whence))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_lseek, fd, (sys_call_linux_word)(offset), whence);
};

fn_((sys_call_linux_pread(sys_call_linux_word fd, void* buf, usize len, u64 offset))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_pread64,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len),
        (sys_call_linux_word)(offset)
    );
};

fn_((sys_call_linux_pwrite(sys_call_linux_word fd, const void* buf, usize len, u64 offset))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_pwrite64,
        fd,
        (sys_call_linux_word)(buf),
        (sys_call_linux_word)(len),
        (sys_call_linux_word)(offset)
    );
};

fn_((sys_call_linux_fsync(sys_call_linux_word fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_fsync, fd);
};

fn_((sys_call_linux_ftruncate(sys_call_linux_word fd, u64 len))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_ftruncate, fd, (sys_call_linux_word)(len));
};

fn_((sys_call_linux_fchdir(sys_call_linux_word fd))(sys_call_linux_word)) {
    return sys_call_linux_syscall1(sys_call_linux_SYS_fchdir, fd);
};

fn_((sys_call_linux_dup3(sys_call_linux_word old_fd, sys_call_linux_word new_fd, sys_call_linux_word flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_dup3, old_fd, new_fd, flags);
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

/*---------- <sys/stat.h> ---------------------------------------------------*/

fn_((sys_call_linux_fchmod(sys_call_linux_word fd, sys_call_linux_mode_t mode))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_fchmod, fd, (sys_call_linux_word)(mode));
};

fn_((sys_call_linux_mkdirat(sys_call_linux_word dirfd, const char* path, sys_call_linux_mode_t mode))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_mkdirat, dirfd, (sys_call_linux_word)(path), (sys_call_linux_word)(mode));
};

/*---------- <unistd.h> -----------------------------------------------------*/

fn_((sys_call_linux_unlinkat(sys_call_linux_word dirfd, const char* path, sys_call_linux_word flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_unlinkat, dirfd, (sys_call_linux_word)(path), flags);
};

/*---------- <sys/ioctl.h> --------------------------------------------------*/

fn_((sys_call_linux_ioctl(sys_call_linux_word fd, sys_call_linux_word request, void* arg))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_ioctl, fd, request, (sys_call_linux_word)(arg));
};

/*---------- <sys/mman.h> ---------------------------------------------------*/

fn_((sys_call_linux_mmap(
    void* addr,
    usize len,
    sys_call_linux_word prot,
    sys_call_linux_word flags,
    sys_call_linux_word fd,
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

fn_((sys_call_linux_mprotect(void* addr, usize len, sys_call_linux_word prot))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_mprotect, (sys_call_linux_word)(addr), (sys_call_linux_word)(len), prot);
};

fn_((sys_call_linux_msync(void* addr, usize len, sys_call_linux_word flags))(sys_call_linux_word)) {
    return sys_call_linux_syscall3(sys_call_linux_SYS_msync, (sys_call_linux_word)(addr), (sys_call_linux_word)(len), flags);
};

/*---------- <time.h> -------------------------------------------------------*/

fn_((sys_call_linux_clock_gettime(sys_call_linux_word clock_id, void* ts))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_clock_gettime, clock_id, (sys_call_linux_word)(ts));
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

fn_((sys_call_linux_nanosleep(void* req, void* rem))(sys_call_linux_word)) {
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
    sys_call_linux_word pid,
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

fn_((sys_call_linux_gettid(void))(sys_call_linux_word)) {
    return sys_call_linux_syscall0(sys_call_linux_SYS_gettid);
};

fn_((sys_call_linux_fork(void))(sys_call_linux_word)) {
    return sys_call_linux_syscall5(sys_call_linux_SYS_clone, 17, 0, 0, 0, 0);
};

/*---------- <sys/wait.h> ---------------------------------------------------*/

fn_((sys_call_linux_wait4(sys_call_linux_word pid, int* status, sys_call_linux_word options, void* usage))(sys_call_linux_word)) {
    return sys_call_linux_syscall4(
        sys_call_linux_SYS_wait4,
        pid,
        (sys_call_linux_word)(status),
        options,
        (sys_call_linux_word)(usage)
    );
};

/*---------- <signal.h> -----------------------------------------------------*/

fn_((sys_call_linux_kill(sys_call_linux_word pid, sys_call_linux_word signal))(sys_call_linux_word)) {
    return sys_call_linux_syscall2(sys_call_linux_SYS_kill, pid, signal);
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
    sys_call_linux_word flags,
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
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_call_linux__included */
