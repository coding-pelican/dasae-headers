#ifndef Co_Fiber__included
#define Co_Fiber__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#define Co_Fiber_supported __bool__Co_Fiber_supported

/// Stores the cpu state of an inactive fiber.
T_alias$((Co_Fiber_Context)(struct Co_Fiber_Context));
typedef fn_(((*Co_Fiber_EntryFn)(void))(void));
$attr($inline_always)
$static fn_((Co_Fiber_Context_init(Co_Fiber_Context* self, P$raw stack_arg, Co_Fiber_EntryFn entry))(void));

/// Context switch struct.
T_alias$((Co_Fiber)(struct Co_Fiber {
    var_(old, Co_Fiber_Context*);
    var_(new, Co_Fiber_Context*);
}));
/// Fills `self->old` with the current cpu state, and restores the cpu state stored in `self->new`.
$attr($inline_always)
$static fn_((Co_Fiber_contextSwitch(const Co_Fiber* self))(const Co_Fiber*));
$attr($inline_always)
$static fn_((Co_Fiber_stackAllocArg(S$u8 stack, usize size, usize align))(O$P$raw));

/*========== Macro and Definitions ==========================================*/

#define __step__Co_Fiber_supported__expand(...) __VA_ARGS__
#define __bool__Co_Fiber_supported __step__Co_Fiber_supported__expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(pp_true)), \
        pp_case_((arch_type_aarch64)(pp_true)), \
        pp_case_((arch_type_riscv64)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)

struct Co_Fiber_Context {
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

fn_((Co_Fiber_Context_init(Co_Fiber_Context* self, P$raw stack_arg, Co_Fiber_EntryFn entry))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(stack_arg), claim_assert_nonnull(entry);
    asg_l((self)(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)({
            .rsp = as$(u64)((ptrToInt(stack_arg)) - sizeOf$(usize)),
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

fn_((Co_Fiber_contextSwitch(const Co_Fiber* self))(const Co_Fiber*)) { /* NOLINTBEGIN(hicpp-no-assembler) */
    claim_assert_nonnull(self);
    register const Co_Fiber* fiber pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(asm("rsi"))),
        pp_case_((arch_type_aarch64)(asm("x1"))),
        pp_case_((arch_type_riscv64)(asm("a1")))
    )) = self;
    pp_if_(Co_Fiber_supported)((__asm__ __volatile__(pp_switch_((arch_type)(
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
            "cc", "rax", "rbx", "rcx", "rdx", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
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
            "x16", "x17", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x30"
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
            "a0", "a2", "a3", "a4", "a5", "a6", "a7", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
            "ra", "gp", "tp", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11"
        ))
    )))));
    return fiber;
}; /* NOLINTEND(hicpp-no-assembler) */

fn_((Co_Fiber_stackAllocArg(S$u8 stack, usize size, usize align))(O$P$raw) $scope) {
    claim_assert(isValidAlign(align));
    if (stack.len < size) return_none();
    let stack_begin = ptrToInt(stack.ptr);
    let stack_end = stack_begin + stack.len;
    let arg = alignBwd(stack_end - size, align);
    if (arg < stack_begin) return_none();
    return_some(intToPtr$((P$raw)(arg)));
} $unscoped(fn);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Co_Fiber__included */
