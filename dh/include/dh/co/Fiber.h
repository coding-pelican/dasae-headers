#pragma once
#ifndef co_Fiber__included
#define co_Fiber__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#define co_Fiber_supported __bool__co_Fiber_supported

#define __step__co_Fiber_stackArgAlign__expand(...) __VA_ARGS__
#define __comp_int__co_Fiber_stackArgAlign __step__co_Fiber_stackArgAlign__expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(16)), \
        pp_default_(8) \
    ) pp_end \
)
#define co_Fiber_stackArgAlign __comp_int__co_Fiber_stackArgAlign

/// Stores the cpu state of an inactive fiber.
T_alias$((co_Fiber_Context)(struct co_Fiber_Context));
typedef fn_(((*co_Fiber_EntryFn)(void))(void));
$attr($inline_always)
$static fn_((co_Fiber_Context_from(co_Fiber_Context* self, P$raw stack_arg, co_Fiber_EntryFn entry))(void));

/// Context switch struct.
T_alias$((co_Fiber)(struct co_Fiber {
    var_(old, co_Fiber_Context*);
    var_(new, co_Fiber_Context*);
}));
/// Fills `self->old` with the current cpu state, and restores the cpu state stored in `self->new`.
$attr($inline_always)
$static fn_((co_Fiber_contextSwitch(const co_Fiber* self))(const co_Fiber*));
$attr($inline_always)
$static fn_((co_Fiber_stackAllocArg(S$u8 stack, usize size, usize align))(O$P$raw));
$attr($inline_always)
$static fn_((co_Fiber_Context_stackPtr(const co_Fiber_Context* self))(usize));

/*========== Macro and Definitions ==========================================*/

#define __step__co_Fiber_supported__expand(...) __VA_ARGS__
#define __bool__co_Fiber_supported __step__co_Fiber_supported__expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(pp_true)), \
        pp_case_((arch_type_aarch64)(pp_true)), \
        pp_case_((arch_type_riscv64)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)

struct co_Fiber_Context {
    T_embed$(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(struct {
            var_(rsp, u64);
            var_(rbp, u64);
            var_(rip, u64);
        })),
        pp_case_((arch_type_aarch64)(struct {
            var_(sp, u64);
            var_(fp, u64);
            var_(pc, u64);
        })),
        pp_case_((arch_type_riscv64)(struct {
            var_(sp, u64);
            var_(fp, u64);
            var_(pc, u64);
        })),
        pp_default_(Void),
    )));
};

#if in_analysis_active_only || in_comptime
fn_((co_Fiber_Context_from(co_Fiber_Context* self, P$raw stack_arg, co_Fiber_EntryFn entry))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(stack_arg), claim_assert_nonnull(entry);
    asg_l((self)(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            .rsp = as$(u64)(alignBwd(ptrToInt(stack_arg), co_Fiber_stackArgAlign) - sizeOf$(usize)),
            .rbp = 0,
            .rip = as$(u64)(ptrToInt(entry)),
        })),
        pp_case_((arch_type_aarch64)({
            .sp = as$(u64)(ptrToInt(stack_arg)),
            .fp = 0,
            .pc = as$(u64)(ptrToInt(entry)),
        })),
        pp_case_((arch_type_riscv64)({
            .sp = as$(u64)(ptrToInt(stack_arg)),
            .fp = 0,
            .pc = as$(u64)(ptrToInt(entry)),
        })),
        pp_default_(cleared())
    ))));
};

fn_((co_Fiber_contextSwitch(const co_Fiber* self))(const co_Fiber*)) { /* NOLINTBEGIN(hicpp-no-assembler) */
    claim_assert_nonnull(self);
    asm_var_(fiber, const co_Fiber*) $reg(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(rsi)),
        pp_case_((arch_type_aarch64)(x1)),
        pp_case_((arch_type_riscv64)(a1))
    ))) = self;
    pp_if_(co_Fiber_supported)((asm_volatile(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(
            "movq 0(%%rsi), %%rax\n\t"
            "movq 8(%%rsi), %%rcx\n\t"
            "leaq 0f(%%rip), %%rdx\n\t"
            "movq %%rsp, 0(%%rax)\n\t"
            "movq %%rbp, 8(%%rax)\n\t"
            "movq %%rdx, 16(%%rax)\n\t"
            "movq 0(%%rcx), %%rsp\n\t"
            "movq 8(%%rcx), %%rbp\n\t"
            "jmpq *16(%%rcx)\n\t"
            "0:" : "+S"(fiber) : : "memory",
            "cc", "rax", "rcx", "rdx", "rbx", "rdi", "r8", "r9", "r10", "r11",
            "r12", "r13", "r14", "r15",
            "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
            "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
            "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7",
            "ymm8", "ymm9", "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15",
            "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7",
            "zmm8", "zmm9", "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15"
        )),
        pp_case_((arch_type_aarch64)(
            "ldp x0, x2, [x1]\n\t"
            "ldr x3, [x2, #16]\n\t"
            "mov x4, sp\n\t"
            "stp x4, x29, [x0]\n\t"
            "adr x5, 0f\n\t"
            "ldp x4, x29, [x2]\n\t"
            "str x5, [x0, #16]\n\t"
            "mov sp, x4\n\t"
            "br x3\n\t"
            "0:" : "+r"(fiber) : : "memory",
            "cc", "x0", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
            "x16", "x17", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x30",
            "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15",
            "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
        )),
        pp_case_((arch_type_riscv64)(
            "ld a0, 0(a1)\n\t"
            "ld a2, 8(a1)\n\t"
            "lla a3, 0f\n\t"
            "sd sp, 0(a0)\n\t"
            "sd fp, 8(a0)\n\t"
            "sd a3, 16(a0)\n\t"
            "ld sp, 0(a2)\n\t"
            "ld fp, 8(a2)\n\t"
            "ld a3, 16(a2)\n\t"
            "jr a3\n\t"
            "0:" : "+r"(fiber) : : "memory",
            "a0", "a2", "a3", "a4", "a5", "a6", "a7",
            "t0", "t1", "t2", "t3", "t4", "t5", "t6",
            "ra", "gp", "tp", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
            "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
            "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
            "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",
            "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
        ))
    )))));
    return fiber;
}; /* NOLINTEND(hicpp-no-assembler) */

fn_((co_Fiber_stackAllocArg(S$u8 stack, usize size, usize align))(O$P$raw) $scope) {
    claim_assert(isValidAlign(align));
    if (stack.len < size) return_none();
    let stack_begin = ptrToInt(stack.ptr);
    let stack_end = stack_begin + stack.len;
    let arg = alignBwd(stack_end - size, align);
    if (arg < stack_begin) return_none();
    return_some(intToPtr$((P$raw)(arg)));
} $unscoped(fn);

fn_((co_Fiber_Context_stackPtr(const co_Fiber_Context* self))(usize)) {
    claim_assert_nonnull(self);
    return pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(as$(usize)(self->rsp))),
        pp_case_((arch_type_aarch64)(as$(usize)(self->sp))),
        pp_case_((arch_type_riscv64)(as$(usize)(self->sp))),
        pp_default_(claim_unreachable)
    ));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* co_Fiber__included */
