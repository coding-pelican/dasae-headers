/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    start.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-27 (date of creation)
 * @updated 2026-05-27 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  start
 */
#pragma once
#ifndef start__included
#define start__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

#if plat_is_windows
#include "sys/api/windows/proc.h"
#elif plat_is_linux
#include "sys/call/linux.h"
#endif /* plat_is_linux */

/*========== Definitions ====================================================*/

T_alias$((start_ExitCode)(i32));
T_alias$((start_Fn)(fn_(((*)(void))(void) $T)));
T_alias$((start_InitFn)(fn_(((*)(void))(i32) $T)));

$attr($maybe_unused $no_return $inline)
$static fn_((start_exit(start_ExitCode status))(void)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_windows)({
            ExitProcess(as$(UINT)(status));
        })),
        pp_case_((plat_type_linux)({
            sys_call_linux_exit_group(status);
        })),
        pp_default_({})
    ));
    claim_unreachable;
};

pp_if_(pp_not(env_start_files_linked))((
    pp_if_(plat_is_windows)(pp_then_(
        $extern var_(__xi_a, start_InitFn);
        $extern var_(__xi_z, start_InitFn);
        $extern var_(__xc_a, start_Fn);
        $extern var_(__xc_z, start_Fn);
        $extern var_(__xp_a, start_Fn);
        $extern var_(__xp_z, start_Fn);
        $extern var_(__xt_a, start_Fn);
        $extern var_(__xt_z, start_Fn);
        $extern var_(__CTOR_LIST__, start_Fn)[];
        $extern var_(__DTOR_LIST__, start_Fn)[];
        $attr($maybe_unused $inline)
        $static fn_((start__win32_callInitFns(P$$(start_InitFn) first, P$$(start_InitFn) last))(void)) {
            for (var it = first + 1; it != last; ++it) {
                let fn = *it;
                if (fn == null) continue;
                let code = call((fn)());
                if (code != 0) start_exit(code);
            };
        };
        $attr($maybe_unused $inline)
        $static fn_((start__win32_callFnsForward(P$$(start_Fn) first, P$$(start_Fn) last))(void)) {
            for (var it = first + 1; it != last; ++it) {
                let fn = *it;
                if (fn != null) call((fn)());
            };
        };
        $attr($maybe_unused $inline)
        $static fn_((start__win32_callCtorList(void))(void)) {
            let count_or_marker = as$(usize)(__CTOR_LIST__[0]);
            if (count_or_marker != usize_limit_max) {
                for (var i = count_or_marker; i != 0; --i) {
                    let fn = __CTOR_LIST__[i];
                    if (fn != null) call((fn)());
                };
                return;
            };
            var_(count, usize) = 0;
            while (__CTOR_LIST__[count + 1] != null) ++count;
            while (count != 0) {
                let fn = __CTOR_LIST__[count];
                if (fn != null) call((fn)());
                --count;
            };
        };
        $attr($maybe_unused $inline)
        $static fn_((start__win32_callDtorList(void))(void)) {
            let count_or_marker = as$(usize)(__DTOR_LIST__[0]);
            if (count_or_marker != usize_limit_max) {
                for (var_(i, usize) = 1; i <= count_or_marker; ++i) {
                    let fn = __DTOR_LIST__[i];
                    if (fn != null) call((fn)());
                };
                return;
            };
            for (var_(i, usize) = 1; __DTOR_LIST__[i] != null; ++i) {
                let fn = __DTOR_LIST__[i];
                if (fn != null) call((fn)());
            };
        };
    ));

    pp_if_(plat_is_linux)(pp_then_(
        $extern var_(__preinit_array_start, start_Fn)[];
        $extern var_(__preinit_array_end, start_Fn)[];
        $extern var_(__init_array_start, start_Fn)[];
        $extern var_(__init_array_end, start_Fn)[];
        $extern var_(__fini_array_start, start_Fn)[];
        $extern var_(__fini_array_end, start_Fn)[];
        $attr($maybe_unused $inline)
        $static fn_((start__linux_callPreInitArray(void))(void)) {
            for (var it = __preinit_array_start; it != __preinit_array_end; ++it) {
                call((*it)());
            };
        };
        $attr($maybe_unused $inline)
        $static fn_((start__linux_callInitArray(void))(void)) {
            for (var it = __init_array_start; it != __init_array_end; ++it) {
                call((*it)());
            };
        };
        $attr($maybe_unused $inline)
        $static fn_((start__linux_callFiniArray(void))(void)) {
            for (var it = __fini_array_end; it-- != __fini_array_start;) {
                call((*it)());
            };
        };
    ));

    $attr($maybe_unused $inline)
    $static fn_((start_callInitArray(void))(void)) {
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)({
                start__win32_callInitFns(&__xi_a, &__xi_z);
                start__win32_callFnsForward(&__xc_a, &__xc_z);
                start__win32_callCtorList();
            })),
            pp_case_((plat_type_linux)({
                start__linux_callPreInitArray();
                start__linux_callInitArray();
            })),
            pp_default_({})
        ));
    };
    $attr($maybe_unused $inline)
    $static fn_((start_callFiniArray(void))(void)) {
        pp_switch_((plat_type)(
            pp_case_((plat_type_windows)({
                start__win32_callDtorList();
                start__win32_callFnsForward(&__xp_a, &__xp_z);
                start__win32_callFnsForward(&__xt_a, &__xt_z);
            })),
            pp_case_((plat_type_linux)({
                start__linux_callFiniArray();
            })),
            pp_default_({})
        ));
    };
));

/*========== Macros =========================================================*/

#define start_emitEntry(_$Entry...) /* clang-format off */ \
    pp_switch_((plat_type)( \
        pp_case_((plat_type_windows)(start__win32_emitEntry)), \
        pp_case_((plat_type_linux)(start__linux_emitEntry)), \
        pp_default_(claim_assert_static_trap_msg( \
            "target does not have dh start entry support" \
        )) \
    ))(_$Entry)

#define start__win32_emitEntry(_$Entry...) \
    T_alias$((start_win32_TLSCallback)(fn_((($callconv_stdcall*)(P$raw inst, u32 reason, P$raw reserved))(void) $T))); \
    T_alias$((start_win32_IMAGE_TLS_DIRECTORY)(struct start_win32_IMAGE_TLS_DIRECTORY { \
        var_(StartAddressOfRawData, P$raw*); \
        var_(EndAddressOfRawData, P$raw*); \
        var_(AddressOfIndex, u32*); \
        var_(AddressOfCallBacks, start_win32_TLSCallback*); \
        var_(SizeOfZeroFill, u32); \
        var_(Characteristics, u32); \
    })); \
    $extern var_(_tls_index, u32); \
    $extern var_(_tls_start, P$raw); \
    $extern var_(_tls_end, P$raw); \
    $extern var_(__xl_a, start_win32_TLSCallback); \
    $extern var_(__xl_z, start_win32_TLSCallback); \
    $extern let_(_tls_used, start_win32_IMAGE_TLS_DIRECTORY); \
    $extern var_(__xi_a, start_InitFn); \
    $extern var_(__xi_z, start_InitFn); \
    $extern var_(__xc_a, start_Fn); \
    $extern var_(__xc_z, start_Fn); \
    $extern var_(__xp_a, start_Fn); \
    $extern var_(__xp_z, start_Fn); \
    $extern var_(__xt_a, start_Fn); \
    $extern var_(__xt_z, start_Fn); \
    var_(_tls_index, u32) = u32_(0xffffffffu); \
    var_(_tls_start, P$raw) $section(".tls") = null; \
    var_(_tls_end, P$raw) $section(".tls$ZZZ") = null; \
    var_(__xl_a, start_win32_TLSCallback) $section(".CRT$XLA") = null; \
    var_(__xl_z, start_win32_TLSCallback) $section(".CRT$XLZ") = null; \
    var_(__xi_a, start_InitFn) $section(".CRT$XIA") = null; \
    var_(__xi_z, start_InitFn) $section(".CRT$XIZ") = null; \
    var_(__xc_a, start_Fn) $section(".CRT$XCA") = null; \
    var_(__xc_z, start_Fn) $section(".CRT$XCZ") = null; \
    var_(__xp_a, start_Fn) $section(".CRT$XPA") = null; \
    var_(__xp_z, start_Fn) $section(".CRT$XPZ") = null; \
    var_(__xt_a, start_Fn) $section(".CRT$XTA") = null; \
    var_(__xt_z, start_Fn) $section(".CRT$XTZ") = null; \
    let_(_tls_used, start_win32_IMAGE_TLS_DIRECTORY) $keep_symbol $section(".rdata$T") = { \
        .StartAddressOfRawData = &_tls_start, \
        .EndAddressOfRawData = &_tls_end, \
        .AddressOfIndex = &_tls_index, \
        .AddressOfCallBacks = &__xl_a + 1, \
        .SizeOfZeroFill = 0, \
        .Characteristics = 0, \
    }; \
    $attr($no_return) \
    $extern fn_((mainCRTStartup(void))(void)); \
    fn_((mainCRTStartup(void))(void)) { _$Entry(null); }; \
    $attr($no_return) \
    $extern fn_((WinMainCRTStartup(void))(void)); \
    fn_((WinMainCRTStartup(void))(void)) { _$Entry(null); }

#define start__linux_emitEntry(_$Entry...) \
    $attr($callconv_naked $no_return) \
    $extern fn_((_start(void))(void)); \
    fn_((_start(void))(void)) { \
        pp_switch_((arch_type)( \
            pp_case_((arch_type_x86_64)(asm_volatile( \
                "xorl %%ebp, %%ebp\n" \
                "movq %%rsp, %%rdi\n" \
                "andq $-16, %%rsp\n" \
                "callq " #_$Entry "\n" : : : "memory" \
            );)), \
            pp_case_((arch_type_x86)(asm_volatile( \
                "xorl %%ebp, %%ebp\n" \
                "movl %%esp, %%eax\n" \
                "andl $-16, %%esp\n" \
                "subl $12, %%esp\n" \
                "pushl %%eax\n" \
                "calll " #_$Entry "\n" : : : "memory" \
            );)), \
            pp_case_((arch_type_aarch64)(asm_volatile( \
                "mov x29, #0\n" \
                "mov x30, #0\n" \
                "mov x0, sp\n" \
                "and x1, x0, #-16\n" \
                "mov sp, x1\n" \
                "b " #_$Entry "\n" : : : "memory" \
            );)), \
            pp_case_((arch_type_arm)(asm_volatile( \
                "mov r7, #0\n" \
                "mov r11, #0\n" \
                "mov lr, #0\n" \
                "mov r0, sp\n" \
                "bic sp, r0, #15\n" \
                "b " #_$Entry "\n" : : : "memory" \
            );)), \
            pp_case_((arch_type_riscv64)(asm_volatile( \
                ".option push\n" \
                ".option norelax\n" \
                "mv fp, zero\n" \
                "mv ra, zero\n" \
                "mv a0, sp\n" \
                "andi sp, sp, -16\n" \
                "tail " #_$Entry "\n" \
                ".option pop\n" : : : "memory" \
            );)), \
            pp_case_((arch_type_riscv32)(asm_volatile( \
                ".option push\n" \
                ".option norelax\n" \
                "mv fp, zero\n" \
                "mv ra, zero\n" \
                "mv a0, sp\n" \
                "andi sp, sp, -16\n" \
                "tail " #_$Entry "\n" \
                ".option pop\n" : : : "memory" \
            );)), \
            pp_default_(claim_assert_static_trap_msg( \
                "linux target architecture does not have dh start entry support" \
            )) \
        )) \
    } /* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start__included */
