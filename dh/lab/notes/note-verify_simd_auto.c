#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef float f32;
typedef struct {
    f32 val[4];
} A$4$f32;
typedef union simd_V$4$f32 {
    __attribute__((unused)) A$4$f32 arr;
    __attribute__((vector_size(4 * sizeof(f32)))) f32 val;
} simd_V$4$f32;

A$4$f32 testA1(A$4$f32 a, A$4$f32 b) {
    for (size_t i = 0; i < (sizeof(a.val) / sizeof(a.val[0])); ++i) {
        f32* const va = &a.val[i];
        const f32* const vb = &b.val[i];
        *va *= *vb;
    }
    return a;
}
A$4$f32 testA2(A$4$f32 a, A$4$f32 b) {
    A$4$f32 c = {};
    for (size_t i = 0; i < (sizeof(a.val) / sizeof(a.val[0])); ++i) {
        f32* const vc = &c.val[i];
        const f32* const va = &a.val[i];
        const f32* const vb = &b.val[i];
        *vc = *va * *vb;
    }
    return c;
}
A$4$f32 testB(const A$4$f32* a, const A$4$f32* b) {
    A$4$f32 c = {};
    for (size_t i = 0; i < (sizeof(a->val) / sizeof(a->val[0])); ++i) {
        f32* const vc = &c.val[i];
        const f32* const va = &a->val[i];
        const f32* const vb = &b->val[i];
        *vc = *va * *vb;
    }
    return c;
}

simd_V$4$f32 testC1(simd_V$4$f32 a, simd_V$4$f32 b) {
    for (size_t i = 0; i < (sizeof(a.arr.val) / sizeof(a.arr.val[0])); ++i) {
        a.val[i] *= b.val[i];
    }
    return a;
}
simd_V$4$f32 testC2(simd_V$4$f32 a, simd_V$4$f32 b) {
    return (simd_V$4$f32){ .val = a.val * b.val };
}
simd_V$4$f32 testD1(const simd_V$4$f32* a, const simd_V$4$f32* b) {
    simd_V$4$f32 c = {};
    for (size_t i = 0; i < (sizeof(a->val) / sizeof(a->val[0])); ++i) {
        c.val[i] = a->val[i] * b->val[i];
    }
    return c;
}
simd_V$4$f32 testD2(const simd_V$4$f32* a, const simd_V$4$f32* b) {
    return (simd_V$4$f32){ .val = a->val * b->val };
}

typedef struct {
    f32 val[2];
} A$2$f32;
typedef union simd_V$2$f32 {
    __attribute__((unused)) A$2$f32 arr;
    __attribute__((vector_size(2 * sizeof(f32)))) f32 val;
} simd_V$2$f32;

typedef struct S_const$A$2$f32 {
    const A$2$f32* ptr;
    size_t len;
} S_const$A$2$f32;
typedef union S$A$2$f32 {
    struct {
        A$2$f32* ptr;
        size_t len;
    };
    S_const$A$2$f32 as_const;
} S$A$2$f32;

typedef struct S_const$simd_V$2$f32 {
    const simd_V$2$f32* ptr;
    size_t len;
} S_const$simd_V$2$f32;
typedef union S$simd_V$2$f32 {
    struct {
        simd_V$2$f32* ptr;
        size_t len;
    };
    S_const$simd_V$2$f32 as_const;
} S$simd_V$2$f32;

void testE_assert(S$A$2$f32 vel, S_const$A$2$f32 acc) {
    (!!(vel.len == acc.len)) || (__builtin_unreachable(), 0);
    (!!(vel.ptr != 0)) || (__builtin_unreachable(), 0);
    (!!(acc.ptr != 0)) || (__builtin_unreachable(), 0);
    (!!(acc.ptr != vel.ptr)) || (__builtin_unreachable(), 0);
    for (size_t i = 0; i < vel.len; ++i) {
        A$2$f32* const v = &vel.ptr[i];
        const A$2$f32* const a = &acc.ptr[i];
        simd_V$2$f32 vv = { .arr = *v };
        const simd_V$2$f32 aa = { .arr = *a };
        vv.val += aa.val;
        *v = vv.arr;
    }
}
void testF_assert(S$simd_V$2$f32 vel, S_const$simd_V$2$f32 acc) {
    (!!(vel.len == acc.len)) || (__builtin_unreachable(), 0);
    (!!(vel.ptr != 0)) || (__builtin_unreachable(), 0);
    (!!(acc.ptr != 0)) || (__builtin_unreachable(), 0);
    (!!(acc.ptr != vel.ptr)) || (__builtin_unreachable(), 0);
    for (size_t i = 0; i < vel.len; ++i) {
        simd_V$2$f32* const v = &vel.ptr[i];
        const simd_V$2$f32* const a = &acc.ptr[i];
        v->val += a->val;
    }
}

void testE_assume(S$A$2$f32 vel, S_const$A$2$f32 acc) {
    __builtin_assume(vel.len == acc.len);
    __builtin_assume(vel.ptr != 0);
    __builtin_assume(acc.ptr != 0);
    __builtin_assume(acc.ptr != vel.ptr);
    for (size_t i = 0; i < vel.len; ++i) {
        A$2$f32* const v = &vel.ptr[i];
        const A$2$f32* const a = &acc.ptr[i];
        simd_V$2$f32 vv = { .arr = *v };
        const simd_V$2$f32 aa = { .arr = *a };
        vv.val += aa.val;
        *v = vv.arr;
    }
}
void testF_assume(S$simd_V$2$f32 vel, S_const$simd_V$2$f32 acc) {
    __builtin_assume(vel.len == acc.len);
    __builtin_assume(vel.ptr != 0);
    __builtin_assume(acc.ptr != 0);
    __builtin_assume(acc.ptr != vel.ptr);
    for (size_t i = 0; i < vel.len; ++i) {
        simd_V$2$f32* const v = &vel.ptr[i];
        const simd_V$2$f32* const a = &acc.ptr[i];
        v->val += a->val;
    }
}

void testE_raw(A$2$f32* restrict vel, size_t vel_len, const A$2$f32* restrict acc, size_t acc_len) {
    __builtin_assume(vel_len == acc_len);
    __builtin_assume(vel != 0);
    __builtin_assume(acc != 0);
    __builtin_assume(acc != vel);
    for (size_t i = 0; i < vel_len; ++i) {
        vel[i].val[0] += acc[i].val[0];
        vel[i].val[1] += acc[i].val[1];
    }
}
void testF_raw(simd_V$2$f32* restrict vel, size_t vel_len, const simd_V$2$f32* restrict acc, size_t acc_len) {
    __builtin_assume(vel_len == acc_len);
    __builtin_assume(vel != 0);
    __builtin_assume(acc != 0);
    __builtin_assume(acc != vel);
    for (size_t i = 0; i < vel_len; ++i) {
        vel[i].val += acc[i].val;
    }
}

/* void testE_restrict(S$A$2$f32 vel, S_const$A$2$f32 acc) {
    if (!(acc.ptr != vel.ptr)) { __builtin_unreachable(); }
    if (!(acc.len == vel.len)) { __builtin_unreachable(); }
    for (size_t i = 0; i < vel.len; ++i) {
        A$2$f32* restrict const v = &vel.ptr[i];
        const A$2$f32* restrict const a = &acc.ptr[i];
        v->val[0] += a->val[0];
        v->val[1] += a->val[1];
    }
}
void testF_restrict(S$simd_V$2$f32 val, S_const$simd_V$2$f32 acc) {
    if (!(acc.ptr != val.ptr)) { __builtin_unreachable(); }
    if (!(acc.len == val.len)) { __builtin_unreachable(); }
    for (size_t i = 0; i < val.len; ++i) {
        simd_V$2$f32* restrict const v = &val.ptr[i];
        const simd_V$2$f32* restrict const a = &acc.ptr[i];
        v->val += a->val;
    }
} */

/*
testE_assert:
        test    rsi, rsi
        je      .LBB7_6
        mov     eax, esi
        and     eax, 3
        cmp     rsi, 4
        jae     .LBB7_7
        xor     ecx, ecx
        jmp     .LBB7_3
.LBB7_7:
        and     rsi, -4
        xor     ecx, ecx
.LBB7_8:
        movsd   xmm0, qword ptr [rdx + 8*rcx]
        movsd   xmm1, qword ptr [rdi + 8*rcx]
        addps   xmm1, xmm0
        movsd   xmm0, qword ptr [rdi + 8*rcx + 8]
        movsd   xmm2, qword ptr [rdi + 8*rcx + 16]
        movsd   xmm3, qword ptr [rdi + 8*rcx + 24]
        movlps  qword ptr [rdi + 8*rcx], xmm1
        movsd   xmm1, qword ptr [rdx + 8*rcx + 8]
        addps   xmm1, xmm0
        movlps  qword ptr [rdi + 8*rcx + 8], xmm1
        movsd   xmm0, qword ptr [rdx + 8*rcx + 16]
        addps   xmm0, xmm2
        movlps  qword ptr [rdi + 8*rcx + 16], xmm0
        movsd   xmm0, qword ptr [rdx + 8*rcx + 24]
        addps   xmm0, xmm3
        movlps  qword ptr [rdi + 8*rcx + 24], xmm0
        add     rcx, 4
        cmp     rsi, rcx
        jne     .LBB7_8
.LBB7_3:
        test    rax, rax
        je      .LBB7_6
        lea     rdx, [rdx + 8*rcx]
        lea     rcx, [rdi + 8*rcx]
        xor     esi, esi
.LBB7_5:
        movsd   xmm0, qword ptr [rcx + 8*rsi]
        movsd   xmm1, qword ptr [rdx + 8*rsi]
        addps   xmm1, xmm0
        movlps  qword ptr [rcx + 8*rsi], xmm1
        inc     rsi
        cmp     rax, rsi
        jne     .LBB7_5
.LBB7_6:
        ret
*/

/*
testE_assume:
        test    rsi, rsi
        je      .LBB9_6
        mov     eax, esi
        and     eax, 3
        cmp     rsi, 4
        jae     .LBB9_7
        xor     ecx, ecx
        jmp     .LBB9_3
.LBB9_7:
        and     rsi, -4
        xor     ecx, ecx
.LBB9_8:
        movsd   xmm0, qword ptr [rdx + 8*rcx]
        movsd   xmm1, qword ptr [rdi + 8*rcx]
        addps   xmm1, xmm0
        movsd   xmm0, qword ptr [rdi + 8*rcx + 8]
        movsd   xmm2, qword ptr [rdi + 8*rcx + 16]
        movsd   xmm3, qword ptr [rdi + 8*rcx + 24]
        movlps  qword ptr [rdi + 8*rcx], xmm1
        movsd   xmm1, qword ptr [rdx + 8*rcx + 8]
        addps   xmm1, xmm0
        movlps  qword ptr [rdi + 8*rcx + 8], xmm1
        movsd   xmm0, qword ptr [rdx + 8*rcx + 16]
        addps   xmm0, xmm2
        movlps  qword ptr [rdi + 8*rcx + 16], xmm0
        movsd   xmm0, qword ptr [rdx + 8*rcx + 24]
        addps   xmm0, xmm3
        movlps  qword ptr [rdi + 8*rcx + 24], xmm0
        add     rcx, 4
        cmp     rsi, rcx
        jne     .LBB9_8
.LBB9_3:
        test    rax, rax
        je      .LBB9_6
        lea     rdx, [rdx + 8*rcx]
        lea     rcx, [rdi + 8*rcx]
        xor     esi, esi
.LBB9_5:
        movsd   xmm0, qword ptr [rcx + 8*rsi]
        movsd   xmm1, qword ptr [rdx + 8*rsi]
        addps   xmm1, xmm0
        movlps  qword ptr [rcx + 8*rsi], xmm1
        inc     rsi
        cmp     rax, rsi
        jne     .LBB9_5
.LBB9_6:
        ret
*/
