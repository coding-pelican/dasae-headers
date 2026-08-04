/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Target.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Target
 *
 * @brief   Runtime representation of a DH native target
 * @details `dh/builtin/cfg/{arch,plat,abi}.h` remain the compile-time source
 *          of truth. This module materializes those facts for binary APIs.
 */
#pragma once
#ifndef Target__included
#define Target__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Target_Arch_Kind)(enum_((Target_Arch_Kind $fits($packed))(
    Target_Arch_Kind_unknown = arch_type_unknown,
    Target_Arch_Kind_x86_64 = arch_type_x86_64,
    Target_Arch_Kind_x86 = arch_type_x86,
    Target_Arch_Kind_aarch64 = arch_type_aarch64,
    Target_Arch_Kind_arm = arch_type_arm,
    Target_Arch_Kind_riscv64 = arch_type_riscv64,
    Target_Arch_Kind_riscv32 = arch_type_riscv32,
    Target_Arch_Kind_wasm64 = arch_type_wasm64,
    Target_Arch_Kind_wasm32 = arch_type_wasm32,
    Target_Arch_Kind_avr = arch_type_avr,
    Target_Arch_Kind_mips64 = arch_type_mips64,
    Target_Arch_Kind_mips32 = arch_type_mips32,
    Target_Arch_Kind_powerpc64 = arch_type_powerpc64,
    Target_Arch_Kind_powerpc32 = arch_type_powerpc32,
    Target_Arch_Kind_s390x = arch_type_s390x
))));
T_alias$((Target_Arch_Family)(enum_((Target_Arch_Family $fits($packed))(
    Target_Arch_Family_unknown = arch_family_type_unknown,
    Target_Arch_Family_x86 = arch_family_type_x86,
    Target_Arch_Family_arm = arch_family_type_arm,
    Target_Arch_Family_riscv = arch_family_type_riscv,
    Target_Arch_Family_wasm = arch_family_type_wasm,
    Target_Arch_Family_avr = arch_family_type_avr,
    Target_Arch_Family_mips = arch_family_type_mips,
    Target_Arch_Family_powerpc = arch_family_type_powerpc,
    Target_Arch_Family_s390x = arch_family_type_s390x
))));
T_alias$((Target_Arch_Endian)(enum_((Target_Arch_Endian $fits($packed))(
    Target_Arch_Endian_unknown = arch_endian_type_unknown,
    Target_Arch_Endian_big = arch_endian_type_big,
    Target_Arch_Endian_little = arch_endian_type_little
))));
T_alias$((Target_Arch)(struct Target_Arch {
    var_(kind, Target_Arch_Kind);
    var_(family, Target_Arch_Family);
    var_(endian, Target_Arch_Endian);
    var_(word_bits, u16);
    var_(byte_bits, u8);
    var_(cache_line_bytes, u16);
}));
$extern cmp_fn_eql$((Target_Arch)(lhs, rhs));
$extern cmp_fn_neq$((Target_Arch)(lhs, rhs));

T_alias$((Target_Plat_Kind)(enum_((Target_Plat_Kind $fits($packed))(
    Target_Plat_Kind_unknown = plat_type_unknown,
    Target_Plat_Kind_windows = plat_type_windows,
    Target_Plat_Kind_linux = plat_type_linux,
    Target_Plat_Kind_android = plat_type_android,
    Target_Plat_Kind_darwin = plat_type_darwin,
    Target_Plat_Kind_freebsd = plat_type_freebsd,
    Target_Plat_Kind_netbsd = plat_type_netbsd,
    Target_Plat_Kind_openbsd = plat_type_openbsd,
    Target_Plat_Kind_dragonfly = plat_type_dragonfly,
    Target_Plat_Kind_solaris = plat_type_solaris,
    Target_Plat_Kind_illumos = plat_type_illumos,
    Target_Plat_Kind_haiku = plat_type_haiku,
    Target_Plat_Kind_serenity = plat_type_serenity,
    Target_Plat_Kind_wasi = plat_type_wasi,
    Target_Plat_Kind_emscripten = plat_type_emscripten
))));
T_alias$((Target_Plat)(struct Target_Plat {
    var_(kind, Target_Plat_Kind);
}));
$extern cmp_fn_eql$((Target_Plat)(lhs, rhs));
$extern cmp_fn_neq$((Target_Plat)(lhs, rhs));

T_alias$((Target_ABI)(struct Target_ABI {
    var_(char_bits, u8);
    var_(short_bits, u8);
    var_(int_bits, u8);
    var_(long_bits, u8);
    var_(long_long_bits, u8);
    var_(pointer_bits, u8);
    var_(size_bits, u8);
    var_(long_needs_distinct_int_cases, bool);
}));
$extern cmp_fn_eql$((Target_ABI)(lhs, rhs));
$extern cmp_fn_neq$((Target_ABI)(lhs, rhs));

T_alias$((Target)(struct Target {
    var_(arch, Target_Arch);
    var_(plat, Target_Plat);
    var_(abi, Target_ABI);
}));
#define Target_here_static(/*void*/) ____Target_here_static()
$attr($inline_always)
$static fn_((Target_here(void))(Target));
$extern fn_((Target_self(void))(Target));

$extern cmp_fn_eql$((Target)(lhs, rhs));
$extern cmp_fn_neq$((Target)(lhs, rhs));

/*========== Macros and Definitions =========================================*/

#define ____Target_here_static() l$((Target){ \
    .arch = { \
        .kind = as$(Target_Arch_Kind)(arch_type), \
        .family = as$(Target_Arch_Family)(arch_family_type), \
        .endian = as$(Target_Arch_Endian)(arch_endian_type), \
        .word_bits = as$(u16)(arch_bits_wide), \
        .byte_bits = as$(u8)(arch_bits_per_byte), \
        .cache_line_bytes = as$(u16)(arch_cache_line_bytes), \
    }, \
    .plat = { \
        .kind = as$(Target_Plat_Kind)(plat_type), \
    }, \
    .abi = { \
        .char_bits = as$(u8)(abi_char_bits), \
        .short_bits = as$(u8)(abi_short_bits), \
        .int_bits = as$(u8)(abi_int_bits), \
        .long_bits = as$(u8)(abi_long_bits), \
        .long_long_bits = as$(u8)(abi_long_long_bits), \
        .pointer_bits = as$(u8)(abi_pointer_bits), \
        .size_bits = as$(u8)(abi_size_bits), \
        .long_needs_distinct_int_cases = abi_long_needs_distinct_int_cases, \
    }, \
})
#if in_analysis_active_only || in_comptime
fn_((Target_here(void))(Target)) {
    return Target_here_static();
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Target__included */
