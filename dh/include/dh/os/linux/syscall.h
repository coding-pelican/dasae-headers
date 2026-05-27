/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    syscall.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-25 (date of creation)
 * @updated 2026-05-25 (date of last update)
 * @ingroup dasae-headers(dh)/os/linux
 * @prefix  os_linux
 */
#ifndef os_linux_syscall__included
#define os_linux_syscall__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../core.h"

/*========== Types and Declarations =========================================*/

/* NOLINTBEGIN(hicpp-no-assembler) */

typedef isize os_linux_word;
typedef u32 os_linux_mode_t;

typedef struct os_linux_timespec {
    var_(tv_sec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(os_linux_word)));
    var_(tv_nsec, pp_if_(arch_is_riscv32)(pp_then_(i64), pp_else_(os_linux_word)));
} os_linux_timespec;

typedef struct os_linux_statx_timestamp {
    i64 tv_sec;
    u32 tv_nsec;
    i32 __reserved;
} os_linux_statx_timestamp;

typedef struct os_linux_statx {
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
    os_linux_statx_timestamp stx_atime;
    os_linux_statx_timestamp stx_btime;
    os_linux_statx_timestamp stx_ctime;
    os_linux_statx_timestamp stx_mtime;
    u32 stx_rdev_major;
    u32 stx_rdev_minor;
    u32 stx_dev_major;
    u32 stx_dev_minor;
    u64 __spare2[14];
} os_linux_statx;

typedef enum os_linux_Errno {
    os_linux_EINTR = 4,
    os_linux_ETIMEDOUT = 110,
} os_linux_Errno;

typedef enum os_linux_CLOCK {
    os_linux_CLOCK_REALTIME = 0,
    os_linux_CLOCK_MONOTONIC = 1,
} os_linux_CLOCK;

typedef enum os_linux_AT {
    os_linux_AT_FDCWD = -100,
    os_linux_AT_EMPTY_PATH = 0x1000,
    os_linux_AT_REMOVEDIR = 0x200,
} os_linux_AT;

typedef enum os_linux_O {
    os_linux_O_RDONLY = 0,
    os_linux_O_WRONLY = 1,
    os_linux_O_RDWR = 2,
    os_linux_O_CREAT = 00000100,
    os_linux_O_EXCL = 00000200,
    os_linux_O_TRUNC = 00001000,
    os_linux_O_NONBLOCK = 00004000,
    os_linux_O_DIRECTORY = 000200000,
} os_linux_O;

typedef enum os_linux_SEEK {
    os_linux_SEEK_SET = 0,
    os_linux_SEEK_CUR = 1,
    os_linux_SEEK_END = 2,
} os_linux_SEEK;

typedef enum os_linux_S_IF {
    os_linux_S_IFMT = 0170000,
    os_linux_S_IFSOCK = 0140000,
    os_linux_S_IFLNK = 0120000,
    os_linux_S_IFREG = 0100000,
    os_linux_S_IFBLK = 0060000,
    os_linux_S_IFDIR = 0040000,
    os_linux_S_IFCHR = 0020000,
    os_linux_S_IFIFO = 0010000,
} os_linux_S_IF;

typedef enum os_linux_STATX {
    os_linux_STATX_BASIC_STATS = 0x000007ff,
} os_linux_STATX;

typedef enum os_linux_PROT {
    os_linux_PROT_NONE = 0,
    os_linux_PROT_READ = 1,
    os_linux_PROT_WRITE = 2,
    os_linux_PROT_EXEC = 4,
} os_linux_PROT;

typedef enum os_linux_MAP {
    os_linux_MAP_SHARED = 0x01,
    os_linux_MAP_PRIVATE = 0x02,
    os_linux_MAP_ANONYMOUS = 0x20,
} os_linux_MAP;

typedef enum os_linux_MS {
    os_linux_MS_ASYNC = 1,
    os_linux_MS_INVALIDATE = 2,
    os_linux_MS_SYNC = 4,
} os_linux_MS;

typedef enum os_linux_IOCTL {
    os_linux_TCGETS = 0x5401,
} os_linux_IOCTL;

typedef enum os_linux_FUTEX {
    os_linux_FUTEX_WAIT = 0,
    os_linux_FUTEX_WAKE = 1,
    os_linux_FUTEX_PRIVATE_FLAG = 128,
} os_linux_FUTEX;

typedef enum os_linux_CLONE {
    os_linux_CLONE_VM = 0x00000100,
    os_linux_CLONE_FS = 0x00000200,
    os_linux_CLONE_FILES = 0x00000400,
    os_linux_CLONE_SIGHAND = 0x00000800,
    os_linux_CLONE_THREAD = 0x00010000,
    os_linux_CLONE_SYSVSEM = 0x00040000,
    os_linux_CLONE_SETTLS = 0x00080000,
    os_linux_CLONE_PARENT_SETTID = 0x00100000,
    os_linux_CLONE_CHILD_CLEARTID = 0x00200000,
} os_linux_CLONE;

typedef enum os_linux_MREMAP {
    os_linux_MREMAP_MAYMOVE = 1,
} os_linux_MREMAP;

typedef enum os_linux_SYS {
    os_linux_SYS_read = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(0)),
        pp_case_((arch_type_x86)(3)),
        pp_case_((arch_type_aarch64)(63)),
        pp_case_((arch_type_arm)(3)),
        pp_case_((arch_type_riscv64)(63)),
        pp_case_((arch_type_riscv32)(63)),
        pp_default_(0)
    )),
    os_linux_SYS_write = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(1)),
        pp_case_((arch_type_x86)(4)),
        pp_case_((arch_type_aarch64)(64)),
        pp_case_((arch_type_arm)(4)),
        pp_case_((arch_type_riscv64)(64)),
        pp_case_((arch_type_riscv32)(64)),
        pp_default_(0)
    )),
    os_linux_SYS_close = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(3)),
        pp_case_((arch_type_x86)(6)),
        pp_case_((arch_type_aarch64)(57)),
        pp_case_((arch_type_arm)(6)),
        pp_case_((arch_type_riscv64)(57)),
        pp_case_((arch_type_riscv32)(57)),
        pp_default_(0)
    )),
    os_linux_SYS_statx = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(332)),
        pp_case_((arch_type_x86)(383)),
        pp_case_((arch_type_aarch64)(291)),
        pp_case_((arch_type_arm)(397)),
        pp_case_((arch_type_riscv64)(291)),
        pp_case_((arch_type_riscv32)(291)),
        pp_default_(0)
    )),
    os_linux_SYS_lseek = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(8)),
        pp_case_((arch_type_x86)(19)),
        pp_case_((arch_type_aarch64)(62)),
        pp_case_((arch_type_arm)(19)),
        pp_case_((arch_type_riscv64)(62)),
        pp_case_((arch_type_riscv32)(62)),
        pp_default_(0)
    )),
    os_linux_SYS_mmap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(9)),
        pp_case_((arch_type_x86)(192)),
        pp_case_((arch_type_aarch64)(222)),
        pp_case_((arch_type_arm)(192)),
        pp_case_((arch_type_riscv64)(222)),
        pp_case_((arch_type_riscv32)(222)),
        pp_default_(0)
    )),
    os_linux_SYS_mprotect = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(10)),
        pp_case_((arch_type_x86)(125)),
        pp_case_((arch_type_aarch64)(226)),
        pp_case_((arch_type_arm)(125)),
        pp_case_((arch_type_riscv64)(226)),
        pp_case_((arch_type_riscv32)(226)),
        pp_default_(0)
    )),
    os_linux_SYS_munmap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(11)),
        pp_case_((arch_type_x86)(91)),
        pp_case_((arch_type_aarch64)(215)),
        pp_case_((arch_type_arm)(91)),
        pp_case_((arch_type_riscv64)(215)),
        pp_case_((arch_type_riscv32)(215)),
        pp_default_(0)
    )),
    os_linux_SYS_msync = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(26)),
        pp_case_((arch_type_x86)(144)),
        pp_case_((arch_type_aarch64)(227)),
        pp_case_((arch_type_arm)(144)),
        pp_case_((arch_type_riscv64)(227)),
        pp_case_((arch_type_riscv32)(227)),
        pp_default_(0)
    )),
    os_linux_SYS_ioctl = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(16)),
        pp_case_((arch_type_x86)(54)),
        pp_case_((arch_type_aarch64)(29)),
        pp_case_((arch_type_arm)(54)),
        pp_case_((arch_type_riscv64)(29)),
        pp_case_((arch_type_riscv32)(29)),
        pp_default_(0)
    )),
    os_linux_SYS_pread64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(17)),
        pp_case_((arch_type_x86)(180)),
        pp_case_((arch_type_aarch64)(67)),
        pp_case_((arch_type_arm)(180)),
        pp_case_((arch_type_riscv64)(67)),
        pp_case_((arch_type_riscv32)(67)),
        pp_default_(0)
    )),
    os_linux_SYS_pwrite64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(18)),
        pp_case_((arch_type_x86)(181)),
        pp_case_((arch_type_aarch64)(68)),
        pp_case_((arch_type_arm)(181)),
        pp_case_((arch_type_riscv64)(68)),
        pp_case_((arch_type_riscv32)(68)),
        pp_default_(0)
    )),
    os_linux_SYS_openat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(257)),
        pp_case_((arch_type_x86)(295)),
        pp_case_((arch_type_aarch64)(56)),
        pp_case_((arch_type_arm)(322)),
        pp_case_((arch_type_riscv64)(56)),
        pp_case_((arch_type_riscv32)(56)),
        pp_default_(0)
    )),
    os_linux_SYS_fsync = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(74)),
        pp_case_((arch_type_x86)(118)),
        pp_case_((arch_type_aarch64)(82)),
        pp_case_((arch_type_arm)(118)),
        pp_case_((arch_type_riscv64)(82)),
        pp_case_((arch_type_riscv32)(82)),
        pp_default_(0)
    )),
    os_linux_SYS_ftruncate = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(77)),
        pp_case_((arch_type_x86)(93)),
        pp_case_((arch_type_aarch64)(46)),
        pp_case_((arch_type_arm)(93)),
        pp_case_((arch_type_riscv64)(46)),
        pp_case_((arch_type_riscv32)(46)),
        pp_default_(0)
    )),
    os_linux_SYS_fchmod = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(91)),
        pp_case_((arch_type_x86)(94)),
        pp_case_((arch_type_aarch64)(52)),
        pp_case_((arch_type_arm)(94)),
        pp_case_((arch_type_riscv64)(52)),
        pp_case_((arch_type_riscv32)(52)),
        pp_default_(0)
    )),
    os_linux_SYS_mkdirat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(258)),
        pp_case_((arch_type_x86)(296)),
        pp_case_((arch_type_aarch64)(34)),
        pp_case_((arch_type_arm)(323)),
        pp_case_((arch_type_riscv64)(34)),
        pp_case_((arch_type_riscv32)(34)),
        pp_default_(0)
    )),
    os_linux_SYS_unlinkat = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(263)),
        pp_case_((arch_type_x86)(301)),
        pp_case_((arch_type_aarch64)(35)),
        pp_case_((arch_type_arm)(328)),
        pp_case_((arch_type_riscv64)(35)),
        pp_case_((arch_type_riscv32)(35)),
        pp_default_(0)
    )),
    os_linux_SYS_clock_gettime = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(228)),
        pp_case_((arch_type_x86)(265)),
        pp_case_((arch_type_aarch64)(113)),
        pp_case_((arch_type_arm)(263)),
        pp_case_((arch_type_riscv64)(113)),
        pp_case_((arch_type_riscv32)(403)),
        pp_default_(0)
    )),
    os_linux_SYS_clock_nanosleep_time64 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(230)),
        pp_case_((arch_type_x86)(407)),
        pp_case_((arch_type_aarch64)(115)),
        pp_case_((arch_type_arm)(407)),
        pp_case_((arch_type_riscv64)(115)),
        pp_case_((arch_type_riscv32)(407)),
        pp_default_(0)
    )),
    os_linux_SYS_nanosleep = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(35)),
        pp_case_((arch_type_x86)(162)),
        pp_case_((arch_type_aarch64)(101)),
        pp_case_((arch_type_arm)(162)),
        pp_case_((arch_type_riscv64)(101)),
        pp_case_((arch_type_riscv32)(0)),
        pp_default_(0)
    )),
    os_linux_SYS_sched_yield = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(24)),
        pp_case_((arch_type_x86)(158)),
        pp_case_((arch_type_aarch64)(124)),
        pp_case_((arch_type_arm)(158)),
        pp_case_((arch_type_riscv64)(124)),
        pp_case_((arch_type_riscv32)(124)),
        pp_default_(0)
    )),
    os_linux_SYS_gettid = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(186)),
        pp_case_((arch_type_x86)(224)),
        pp_case_((arch_type_aarch64)(178)),
        pp_case_((arch_type_arm)(224)),
        pp_case_((arch_type_riscv64)(178)),
        pp_case_((arch_type_riscv32)(178)),
        pp_default_(0)
    )),
    os_linux_SYS_sched_getaffinity = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(204)),
        pp_case_((arch_type_x86)(242)),
        pp_case_((arch_type_aarch64)(123)),
        pp_case_((arch_type_arm)(242)),
        pp_case_((arch_type_riscv64)(123)),
        pp_case_((arch_type_riscv32)(123)),
        pp_default_(0)
    )),
    os_linux_SYS_futex = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(202)),
        pp_case_((arch_type_x86)(240)),
        pp_case_((arch_type_aarch64)(98)),
        pp_case_((arch_type_arm)(240)),
        pp_case_((arch_type_riscv64)(98)),
        pp_case_((arch_type_riscv32)(422)),
        pp_default_(0)
    )),
    os_linux_SYS_mremap = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(25)),
        pp_case_((arch_type_x86)(163)),
        pp_case_((arch_type_aarch64)(216)),
        pp_case_((arch_type_arm)(163)),
        pp_case_((arch_type_riscv64)(216)),
        pp_case_((arch_type_riscv32)(216)),
        pp_default_(0)
    )),
    os_linux_SYS_renameat2 = pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(316)),
        pp_case_((arch_type_x86)(353)),
        pp_case_((arch_type_aarch64)(276)),
        pp_case_((arch_type_arm)(382)),
        pp_case_((arch_type_riscv64)(276)),
        pp_case_((arch_type_riscv32)(276)),
        pp_default_(0)
    )),
} os_linux_SYS;

$attr($inline_always)
$static fn_((os_linux_syscall0(os_linux_word n))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_volatile("syscall" : "+r"(rax) : : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_volatile("int $0x80" : "+r"(eax) : : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = 0;
            asm_volatile("svc #0" : "=r"(x0) : "r"(x8) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = 0;
            asm_volatile("svc 0" : "=r"(r0) : "r"(r7) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = 0;
            asm_volatile("ecall" : "=r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = 0;
            asm_volatile("ecall" : "=r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall1(os_linux_word n, os_linux_word a1))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_var_reg(ebx, os_linux_word) = a1;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall2(os_linux_word n, os_linux_word a1, os_linux_word a2))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_var_reg(rsi, os_linux_word) = a2;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_var_reg(ebx, os_linux_word) = a1;
            asm_var_reg(ecx, os_linux_word) = a2;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_var_reg(x1, os_linux_word) = a2;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_var_reg(r1, os_linux_word) = a2;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall3(os_linux_word n, os_linux_word a1, os_linux_word a2, os_linux_word a3))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_var_reg(rsi, os_linux_word) = a2;
            asm_var_reg(rdx, os_linux_word) = a3;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_var_reg(ebx, os_linux_word) = a1;
            asm_var_reg(ecx, os_linux_word) = a2;
            asm_var_reg(edx, os_linux_word) = a3;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_var_reg(x1, os_linux_word) = a2;
            asm_var_reg(x2, os_linux_word) = a3;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_var_reg(r1, os_linux_word) = a2;
            asm_var_reg(r2, os_linux_word) = a3;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall4(os_linux_word n, os_linux_word a1, os_linux_word a2, os_linux_word a3, os_linux_word a4))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_var_reg(rsi, os_linux_word) = a2;
            asm_var_reg(rdx, os_linux_word) = a3;
            asm_var_reg(r10, os_linux_word) = a4;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_var_reg(ebx, os_linux_word) = a1;
            asm_var_reg(ecx, os_linux_word) = a2;
            asm_var_reg(edx, os_linux_word) = a3;
            asm_var_reg(esi, os_linux_word) = a4;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_var_reg(x1, os_linux_word) = a2;
            asm_var_reg(x2, os_linux_word) = a3;
            asm_var_reg(x3, os_linux_word) = a4;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_var_reg(r1, os_linux_word) = a2;
            asm_var_reg(r2, os_linux_word) = a3;
            asm_var_reg(r3, os_linux_word) = a4;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r) : "memory");
            return a0;
        })),
        pp_default_({
            let_ignore = n;
            let_ignore = a1;
            let_ignore = a2;
            let_ignore = a3;
            let_ignore = a4;
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall5(os_linux_word n, os_linux_word a1, os_linux_word a2, os_linux_word a3, os_linux_word a4, os_linux_word a5))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_var_reg(rsi, os_linux_word) = a2;
            asm_var_reg(rdx, os_linux_word) = a3;
            asm_var_reg(r10, os_linux_word) = a4;
            asm_var_reg(r8, os_linux_word) = a5;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            asm_var_reg(eax, os_linux_word) = n;
            asm_var_reg(ebx, os_linux_word) = a1;
            asm_var_reg(ecx, os_linux_word) = a2;
            asm_var_reg(edx, os_linux_word) = a3;
            asm_var_reg(esi, os_linux_word) = a4;
            asm_var_reg(edi, os_linux_word) = a5;
            asm_volatile("int $0x80" : "+r"(eax) : "r"(ebx), "r"(ecx), "r"(edx), "r"(esi), "r"(edi) : "memory");
            return eax;
        })),
        pp_case_((arch_type_aarch64)({
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_var_reg(x1, os_linux_word) = a2;
            asm_var_reg(x2, os_linux_word) = a3;
            asm_var_reg(x3, os_linux_word) = a4;
            asm_var_reg(x4, os_linux_word) = a5;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_var_reg(r1, os_linux_word) = a2;
            asm_var_reg(r2, os_linux_word) = a3;
            asm_var_reg(r3, os_linux_word) = a4;
            asm_var_reg(r4, os_linux_word) = a5;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_var_(a4r, os_linux_word) $reg(a4) = a5;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_var_(a4r, os_linux_word) $reg(a4) = a5;
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
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall6(os_linux_word n, os_linux_word a1, os_linux_word a2, os_linux_word a3, os_linux_word a4, os_linux_word a5, os_linux_word a6))(os_linux_word)) {
    pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            asm_var_reg(rax, os_linux_word) = n;
            asm_var_reg(rdi, os_linux_word) = a1;
            asm_var_reg(rsi, os_linux_word) = a2;
            asm_var_reg(rdx, os_linux_word) = a3;
            asm_var_reg(r10, os_linux_word) = a4;
            asm_var_reg(r8, os_linux_word) = a5;
            asm_var_reg(r9, os_linux_word) = a6;
            asm_volatile("syscall" : "+r"(rax) : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
            return rax;
        })),
        pp_case_((arch_type_x86)({
            var_(ret, os_linux_word) $undefined;
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
            asm_var_reg(x8, os_linux_word) = n;
            asm_var_reg(x0, os_linux_word) = a1;
            asm_var_reg(x1, os_linux_word) = a2;
            asm_var_reg(x2, os_linux_word) = a3;
            asm_var_reg(x3, os_linux_word) = a4;
            asm_var_reg(x4, os_linux_word) = a5;
            asm_var_reg(x5, os_linux_word) = a6;
            asm_volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory");
            return x0;
        })),
        pp_case_((arch_type_arm)({
            asm_var_reg(r7, os_linux_word) = n;
            asm_var_reg(r0, os_linux_word) = a1;
            asm_var_reg(r1, os_linux_word) = a2;
            asm_var_reg(r2, os_linux_word) = a3;
            asm_var_reg(r3, os_linux_word) = a4;
            asm_var_reg(r4, os_linux_word) = a5;
            asm_var_reg(r5, os_linux_word) = a6;
            asm_volatile("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5) : "memory");
            return r0;
        })),
        pp_case_((arch_type_riscv64)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_var_(a4r, os_linux_word) $reg(a4) = a5;
            asm_var_(a5r, os_linux_word) $reg(a5) = a6;
            asm_volatile("ecall" : "+r"(a0) : "r"(a7), "r"(a1r), "r"(a2r), "r"(a3r), "r"(a4r), "r"(a5r) : "memory");
            return a0;
        })),
        pp_case_((arch_type_riscv32)({
            asm_var_reg(a7, os_linux_word) = n;
            asm_var_reg(a0, os_linux_word) = a1;
            asm_var_(a1r, os_linux_word) $reg(a1) = a2;
            asm_var_(a2r, os_linux_word) $reg(a2) = a3;
            asm_var_(a3r, os_linux_word) $reg(a3) = a4;
            asm_var_(a4r, os_linux_word) $reg(a4) = a5;
            asm_var_(a5r, os_linux_word) $reg(a5) = a6;
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
            claim_unreachable;
        })
    ));
};

$attr($inline_always)
$static fn_((os_linux_syscall_isErr(os_linux_word rc))(bool)) {
    return rc < 0 && -4095 <= rc;
};

$attr($inline_always)
$static fn_((os_linux_syscall_err(os_linux_word rc))(os_linux_word)) {
    return -rc;
};

$attr($inline_always)
$static fn_((os_linux_read(os_linux_word fd, void* buf, usize len))(os_linux_word)) {
    return os_linux_syscall3(
        os_linux_SYS_read,
        fd,
        (os_linux_word)(buf),
        (os_linux_word)(len)
    );
};

$attr($inline_always)
$static fn_((os_linux_write(os_linux_word fd, const void* buf, usize len))(os_linux_word)) {
    return os_linux_syscall3(
        os_linux_SYS_write,
        fd,
        (os_linux_word)(buf),
        (os_linux_word)(len)
    );
};

$attr($inline_always)
$static fn_((os_linux_close(os_linux_word fd))(os_linux_word)) {
    return os_linux_syscall1(os_linux_SYS_close, fd);
};

$attr($inline_always)
$static fn_((os_linux_openat(
    os_linux_word dirfd,
    const char* path,
    os_linux_word flags,
    os_linux_word mode
))(os_linux_word)) {
    return os_linux_syscall4(
        os_linux_SYS_openat,
        dirfd,
        (os_linux_word)(path),
        flags,
        mode
    );
};

$attr($inline_always)
$static fn_((os_linux_statx_get(
    os_linux_word dirfd,
    const char* path,
    os_linux_word flags,
    os_linux_word mask,
    os_linux_statx* stat
))(os_linux_word)) {
    return os_linux_syscall5(
        os_linux_SYS_statx,
        dirfd,
        (os_linux_word)(path),
        flags,
        mask,
        (os_linux_word)(stat)
    );
};

$attr($inline_always)
$static fn_((os_linux_lseek(os_linux_word fd, i64 offset, os_linux_word whence))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_lseek, fd, (os_linux_word)(offset), whence);
};

$attr($inline_always)
$static fn_((os_linux_pread(os_linux_word fd, void* buf, usize len, u64 offset))(os_linux_word)) {
    return os_linux_syscall4(
        os_linux_SYS_pread64,
        fd,
        (os_linux_word)(buf),
        (os_linux_word)(len),
        (os_linux_word)(offset)
    );
};

$attr($inline_always)
$static fn_((os_linux_pwrite(os_linux_word fd, const void* buf, usize len, u64 offset))(os_linux_word)) {
    return os_linux_syscall4(
        os_linux_SYS_pwrite64,
        fd,
        (os_linux_word)(buf),
        (os_linux_word)(len),
        (os_linux_word)(offset)
    );
};

$attr($inline_always)
$static fn_((os_linux_fsync(os_linux_word fd))(os_linux_word)) {
    return os_linux_syscall1(os_linux_SYS_fsync, fd);
};

$attr($inline_always)
$static fn_((os_linux_ftruncate(os_linux_word fd, u64 len))(os_linux_word)) {
    return os_linux_syscall2(os_linux_SYS_ftruncate, fd, (os_linux_word)(len));
};

$attr($inline_always)
$static fn_((os_linux_fchmod(os_linux_word fd, os_linux_mode_t mode))(os_linux_word)) {
    return os_linux_syscall2(os_linux_SYS_fchmod, fd, (os_linux_word)(mode));
};

$attr($inline_always)
$static fn_((os_linux_mkdirat(os_linux_word dirfd, const char* path, os_linux_mode_t mode))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_mkdirat, dirfd, (os_linux_word)(path), (os_linux_word)(mode));
};

$attr($inline_always)
$static fn_((os_linux_unlinkat(os_linux_word dirfd, const char* path, os_linux_word flags))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_unlinkat, dirfd, (os_linux_word)(path), flags);
};

$attr($inline_always)
$static fn_((os_linux_ioctl(os_linux_word fd, os_linux_word request, void* arg))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_ioctl, fd, request, (os_linux_word)(arg));
};

$attr($inline_always)
$static fn_((os_linux_mmap(
    void* addr,
    usize len,
    os_linux_word prot,
    os_linux_word flags,
    os_linux_word fd,
    u64 offset
))(os_linux_word)) {
    return os_linux_syscall6(
        os_linux_SYS_mmap,
        (os_linux_word)(addr),
        (os_linux_word)(len),
        prot,
        flags,
        fd,
        (os_linux_word)(offset)
    );
};

$attr($inline_always)
$static fn_((os_linux_munmap(void* addr, usize len))(os_linux_word)) {
    return os_linux_syscall2(os_linux_SYS_munmap, (os_linux_word)(addr), (os_linux_word)(len));
};

$attr($inline_always)
$static fn_((os_linux_mprotect(void* addr, usize len, os_linux_word prot))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_mprotect, (os_linux_word)(addr), (os_linux_word)(len), prot);
};

$attr($inline_always)
$static fn_((os_linux_msync(void* addr, usize len, os_linux_word flags))(os_linux_word)) {
    return os_linux_syscall3(os_linux_SYS_msync, (os_linux_word)(addr), (os_linux_word)(len), flags);
};

$attr($inline_always)
$static fn_((os_linux_clock_gettime(os_linux_word clock_id, void* ts))(os_linux_word)) {
    return os_linux_syscall2(os_linux_SYS_clock_gettime, clock_id, (os_linux_word)(ts));
};

$attr($inline_always)
$static fn_((os_linux_nanosleep(void* req, void* rem))(os_linux_word)) {
    return pp_if_(arch_is_riscv32)(
        pp_then_(os_linux_syscall4(
            os_linux_SYS_clock_nanosleep_time64,
            os_linux_CLOCK_MONOTONIC,
            0,
            (os_linux_word)(req),
            (os_linux_word)(rem)
        )),
        pp_else_(os_linux_syscall2(
            os_linux_SYS_nanosleep,
            (os_linux_word)(req),
            (os_linux_word)(rem)
        )));
};

$attr($inline_always)
$static fn_((os_linux_sched_yield(void))(os_linux_word)) {
    return os_linux_syscall0(os_linux_SYS_sched_yield);
};

$attr($inline_always)
$static fn_((os_linux_gettid(void))(os_linux_word)) {
    return os_linux_syscall0(os_linux_SYS_gettid);
};

$attr($inline_always)
$static fn_((os_linux_sched_getaffinity(
    os_linux_word pid,
    usize size,
    void* set
))(os_linux_word)) {
    return os_linux_syscall3(
        os_linux_SYS_sched_getaffinity,
        pid,
        (os_linux_word)(size),
        (os_linux_word)(set)
    );
};

$attr($inline_always)
$static fn_((os_linux_futex(
    void* addr,
    os_linux_word op,
    os_linux_word value,
    void* timeout,
    void* addr2,
    os_linux_word value3
))(os_linux_word)) {
    return os_linux_syscall6(
        os_linux_SYS_futex,
        (os_linux_word)(addr),
        op,
        value,
        (os_linux_word)(timeout),
        (os_linux_word)(addr2),
        value3
    );
};

$attr($inline_always)
$static fn_((os_linux_mremap(
    void* old_addr,
    usize old_size,
    usize new_size,
    os_linux_word flags,
    void* new_addr
))(os_linux_word)) {
    return os_linux_syscall5(
        os_linux_SYS_mremap,
        (os_linux_word)(old_addr),
        (os_linux_word)(old_size),
        (os_linux_word)(new_size),
        flags,
        (os_linux_word)(new_addr)
    );
};

$attr($inline_always)
$static fn_((os_linux_renameat(
    os_linux_word old_dirfd,
    const char* old_path,
    os_linux_word new_dirfd,
    const char* new_path
))(os_linux_word)) {
    return os_linux_syscall5(
        os_linux_SYS_renameat2,
        old_dirfd,
        (os_linux_word)(old_path),
        new_dirfd,
        (os_linux_word)(new_path),
        0
    );
};

/* NOLINTEND(hicpp-no-assembler) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_linux_syscall__included */
