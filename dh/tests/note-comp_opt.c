/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-18 (date of creation)
 * @updated 2025-10-18 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/ps
 * @prefix  NONE
 */

#ifndef dh_ps_common__included
#define dh_ps_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/* clang-format off */
#if defined(__GNUC__)
    #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wunknown-pragmas"
    #endif /* defined(__clang__) */
    #pragma GCC optimize("O3,unroll-loops")
    #pragma GCC target("avx,avx2,bmi,bmi2,fma,lzcnt,popcnt")
    #if defined(__clang__)
        #pragma clang diagnostic pop
    #endif /* defined(__clang__) */
#endif /* defined(__GNUC__) */
/* clang-format on */

#define pp_nothing(...) pp_exec_nothing(__VA_ARGS__)
#define pp_exec_nothing(...)
#define pp_ignore(...) pp_exec_ignore(__VA_ARGS__)
#define pp_exec_ignore(...)
#define pp_expand(...)      pp_exec_expand(__VA_ARGS__)
#define pp_exec_expand(...) __VA_ARGS__
#define pp_defer(...)       pp_exec_defer(__VA_ARGS__)
#define pp_exec_defer(...)  __VA_ARGS__ pp_exec_nothing()

#define pp_cat(_LhsTok, _RhsTok...)                 pp_exec_cat(_LhsTok, _RhsTok)
#define pp_exec_cat(_LhsTok, _RhsTok...)            _LhsTok##_RhsTok
#define pp_cat2(_LhsTok, _RhsTok...)                pp_exec_cat2(_LhsTok, _RhsTok)
#define pp_exec_cat2(_LhsTok, _RhsTok...)           _LhsTok##_RhsTok
#define pp_cat3(_LhsTok, _Tok_mid, _RhsTok...)      pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok)
#define pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_Tok_mid##_RhsTok

#define pp_join(_SepTok, _LhsTok, _RhsTok...)                 pp_exec_join(_SepTok, _LhsTok, _RhsTok)
#define pp_exec_join(_SepTok, _LhsTok, _RhsTok...)            _LhsTok##_SepTok##_RhsTok
#define pp_join2(_SepTok, _LhsTok, _RhsTok...)                pp_exec_join2(_SepTok, _LhsTok, _RhsTok)
#define pp_exec_join2(_SepTok, _LhsTok, _RhsTok...)           _LhsTok##_SepTok##_RhsTok
#define pp_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...)      pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok)
#define pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_SepTok##_Tok_mid##_SepTok##_RhsTok

#define pp_uniqTok(_Tok...)      pp_exec_uniqTok(_Tok)
#define pp_exec_uniqTok(_Tok...) pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)

/* #include <stdbool.h> */
typedef _Bool bool;
#define true  as$((bool)(1))
#define false as$((bool)(0))
#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t usize;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t isize;
typedef float f32;
typedef double f64;
typedef struct Void Void;
struct Void {
    u8 _bytes_zero[0];
};

#define as$(/*(T)(Expr)*/...)             pp_expand(pp_defer(__block_inline__as$)(__param_expand__as$ __VA_ARGS__))
#define __param_expand__as$(...)          __VA_ARGS__, pp_expand
#define __block_inline__as$(_T, _Expr...) ((_T)(_Expr))

#define var __auto_type
#define let __auto_type const

#define var_const const var*
#define let_const let* const

#define var_(_ident, _T...) _T _ident
#define let_(_ident, _T...) _T const _ident

#define var_const_(_ident, _T...) const _T _ident
#define let_const_(_ident, _T...) const _T const _ident

#define fn_(/*(_ident(_Params...))(_T_Return)*/...) \
    pp_expand(pp_defer(__block_inline__fn_)(__param_expand__fn_ __VA_ARGS__))
#define __param_expand__fn_(...)                      __VA_ARGS__, pp_expand
#define __block_inline__fn_(_ident_Params, _T_Return) _T_Return _ident_Params

#define TypeOf(_Expr...)       __typeof__(_Expr)
#define TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)

#define sizeOf$(_T...)  as$((usize)(sizeof(_T)))
#define alignOf$(_T...) as$((usize)(__alignof(_T)))
#define countOf$(_T...) (sizeOf$(_T) / sizeOf$(TypeOf((*as$((_T*)(0)))[0])))

typedef struct TypeInfo {
    u32 size, align;
} TypeInfo;
#define typeInfo$(T...) ((TypeInfo){ .size = sizeOf$(T), .align = alignOf$(T) })

#define p$(_T)       _T*
#define p_const$(_T) const _T*
#define null         ((void*)0)

#define $on_load       __attribute__((constructor))
#define $on_exit       __attribute__((destructor))
#define $static        static
#define $extern        extern
#define $inline        inline
#define $inline_always __attribute__((always_inline)) inline
#define $inline_never  __attribute__((noinline)) inline

#define let_ignore \
    __attribute__((unused)) let pp_uniqTok(ignored)
#define $ignore \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    let_ignore
#define $ignore_void (void)

#define swap$(/*(T)(p_lhs, p_rhs)*/...)           pp_expand(pp_defer(__block_inline__swap$)(__param_expand__swap$ __VA_ARGS__))
#define __param_expand__swap$(...)                __VA_ARGS__, pp_expand
#define __block_inline__swap$(_T, _p_lhs, _p_rhs) ({ \
    p$(_T) __lhs = (__p_lhs); \
    p$(_T) __rhs = (__p_rhs); \
    _T __tmp     = *__lhs; \
    *__lhs       = *__rhs; \
    *__rhs       = __tmp; \
})

#define min$(/*(T)(lhs, rhs)*/...)                             pp_expand(pp_defer(__block_inline__min2$)(__param_expand__min2$ __VA_ARGS__))
#define min2$(/*(T)(lhs, rhs)*/...)                            pp_expand(pp_defer(__block_inline__min2$)(__param_expand__min2$ __VA_ARGS__))
#define __param_expand__min2$(...)                             pp_uniqTok(lhs), pp_uniqTok(rhs), __VA_ARGS__, pp_expand
#define __block_inline__min2$(__lhs, __rhs, _T, _lhs, _rhs...) ({ \
    const _T __lhs = _lhs; \
    const _T __rhs = _rhs; \
    __rhs < __lhs ? __rhs : __lhs; \
})
#define min3$(/*(T)(1st, 2nd, 3rd)*/...)                     pp_expand(pp_defer(__block_inline__min3$)(__param_expand__min3$ __VA_ARGS__))
#define __param_expand__min3$(...)                           __VA_ARGS__, pp_expand
#define __block_inline__min3$(_T, _1st, _2nd, _3rd...)       min$((_T)(min$((_T)(_1st, _2nd)), (_3rd)))
#define min4$(/*(T)(1st, 2nd, 3rd, 4th)*/...)                pp_expand(pp_defer(__block_inline__min4$)(__param_expand__min4$ __VA_ARGS__))
#define __param_expand__min4$(...)                           __VA_ARGS__, pp_expand
#define __block_inline__min4$(_T, _1st, _2nd, _3rd, _4th...) min$((_T)(min$((_T)(min$((_T)(_1st, _2nd)), (_3rd))), (_4th)))

#define max$(/*(T)(lhs, rhs)*/...)                             pp_expand(pp_defer(__block_inline__max2$)(__param_expand__max2$ __VA_ARGS__))
#define max2$(/*(T)(lhs, rhs)*/...)                            pp_expand(pp_defer(__block_inline__max2$)(__param_expand__max2$ __VA_ARGS__))
#define __param_expand__max2$(...)                             pp_uniqTok(lhs), pp_uniqTok(rhs), __VA_ARGS__, pp_expand
#define __block_inline__max2$(__lhs, __rhs, _T, _lhs, _rhs...) ({ \
    const _T __lhs = _lhs; \
    const _T __rhs = _rhs; \
    __rhs > __lhs ? __rhs : __lhs; \
})
#define max3$(/*(T)(1st, 2nd, 3rd)*/...)                     pp_expand(pp_defer(__block_inline__max3$)(__param_expand__max3$ __VA_ARGS__))
#define __param_expand__max3$(...)                           __VA_ARGS__, pp_expand
#define __block_inline__max3$(_T, _1st, _2nd, _3rd...)       max$((_T)(max$((_T)(_1st, _2nd)), (_3rd)))
#define max4$(/*(T)(1st, 2nd, 3rd, 4th)*/...)                pp_expand(pp_defer(__block_inline__max4$)(__param_expand__max4$ __VA_ARGS__))
#define __param_expand__max4$(...)                           __VA_ARGS__, pp_expand
#define __block_inline__max4$(_T, _1st, _2nd, _3rd, _4th...) max$((_T)(max$((_T)(max$((_T)(_1st, _2nd)), (_3rd))), (_4th)))

#define clamp$(/*(T)(val, min, max)*/...)               pp_expand(pp_defer(__block_inline__clamp$)(__param_expand__clamp$ __VA_ARGS__))
#define __param_expand__clamp$(...)                     __VA_ARGS__, pp_expand
#define __block_inline__clamp$(_T, _val, _min, _max...) min$((_T)(max$((_T)(_min, _val)), (_max)))

#define mem_set(_dest, _val, _len)  __builtin_memset(_dest, as$((u8)(_val)), _len)
#define mem_copy(_dest, _src, _len) __builtin_memcpy(_dest, _src, _len)
#define mem_move(_dest, _src, _len) __builtin_memmove(_dest, _src, _len)
#define mem_allocStack(_len)        as$((u8*)(__builtin_alloca(_len)))

#define defer_(_stmt...) \
    for (bool _once = false; !_once; ({ _once = true; _stmt; }))

#define with_(_init, _fini...) \
    for (bool _once = false; !_once;) \
        for (_init; !_once; ({ _once = true; _fini; }))

#define for_range(_val_begin, _val_end, _id_iter, _body...) ({ \
    const usize __begin = _val_begin; \
    const usize __end   = _val_end; \
    for (usize __iter = __begin; __iter < __end; ++__iter) { \
        let _id_iter = __iter; \
        _body \
    } \
})
#define for_range_rev(_val_begin, _val_end, _id_iter, _body...) ({ \
    const usize __begin = _val_begin; \
    const usize __end   = _val_end; \
    for (usize __iter = __end; __iter > __begin; --__iter) { \
        let _id_iter = __iter - 1; \
        _body \
    } \
})

#include <unistd.h>
#if defined(__GNUC__) && !defined(__clang__)
#define fn_main() \
    $static fn_((dh_main(void))(i32)); \
    fn_((main())(i32)) {} \
    fn_((__libc_start_main())(void)) { _exit(dh_main()); } \
    fn_((dh_main(void))(i32))
#else
#define fn_main() \
    $static fn_((dh_main(void))(i32)); \
    fn_((main())(i32)) { return 0; } \
    $on_load $static fn_((__start_main(void))(void)) { _exit(dh_main()); } \
    fn_((dh_main(void))(i32))
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_ps_common__included */

#if !defined(NDEBUG)
#define DEBUG_DEFAULT_ENABLED (1)
#else
#define DEBUG_DEFAULT_ENABLED (0)
#endif

#if !defined(debug_comp_enabled)
#define debug_comp_enabled DEBUG_DEFAULT_ENABLED
#endif /* !defined(debug_comp_enabled) */

#define debug_assert(_Expr) \
    comp_inline__debug_assert(_Expr, #_Expr)
#if debug_comp_enabled
#define comp_inline__debug_assert(_Expr, _ExprStr) \
    $ignore_void((!!(_Expr)) || (debug_assert_fail(_ExprStr, __func__, __FILE__, __LINE__), 0))
$inline_always
$static fn_((debug_assert_fail(const char* expr, const char* func, const char* file, i32 line))(void)) {
    let_ignore = expr;
    let_ignore = func;
    let_ignore = file;
    let_ignore = line;
    __builtin_trap();
}
#else
#define comp_inline__debug_assert(_Expr, _ExprStr) ((void)(0))
#endif /* debug_comp_enabled */

#define claim_unreachable \
    (debug_assert_fmt(false), __builtin_unreachable())

typedef const void* P_const$raw;
typedef void* P$raw;

typedef struct S_const$raw {
    P_const$raw ptr;
    usize       len;
} S_const$raw;

typedef union S_const$u8 {
    struct {
        const u8* ptr;
        usize     len;
    };
    S_const$raw as_raw;
} S_const$u8;

typedef union S_const$i64 {
    struct {
        const i64* ptr;
        usize      len;
    };
    S_const$raw as_raw;
} S_const$i64;

$inline_always
$static fn_((mem_eql0$raw(usize size, const u8* lhs, usize lhs_len, const u8* rhs, usize rhs_len))(bool)) {
    if (lhs_len != rhs_len) { return false; }
    if (lhs_len == 0) { return true; }
    debug_assert(lhs != null);
    debug_assert(rhs != null);
    return __builtin_memcmp(lhs, rhs, (lhs_len * size)) == 0;
}
$extern fn_((mem_eql0$i64(S_const$i64 lhs, S_const$i64 rhs))(bool)) {
    return mem_eql0$raw(sizeof(i64), lhs.as_raw.ptr, lhs.len, rhs.as_raw.ptr, rhs.len);
}

$inline_always
$static fn_((mem_eql1$raw(TypeInfo type, S_const$raw lhs, S_const$raw rhs))(bool)) {
    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0) { return true; }
    debug_assert(lhs.ptr != null);
    debug_assert(rhs.ptr != null);
    return __builtin_memcmp(lhs.ptr, rhs.ptr, (lhs.len * type.size)) == 0;
}
$extern fn_((mem_eql1$i64(S_const$i64 lhs, S_const$i64 rhs))(bool)) {
    return mem_eql1$raw(typeInfo$(TypeOf(i64)), lhs.as_raw, rhs.as_raw);
}

typedef struct meta_S_const$raw {
    TypeInfo type;
    union {
        struct {
            P_const$raw ptr;
            usize       len;
        };
        S_const$raw raw;
    };
} meta_S_const$raw;

#define metaRef$S(_s...) ((meta_S_const$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })

$inline_always
$static fn_((mem_eql2$raw(TypeInfo type, meta_S_const$raw lhs, meta_S_const$raw rhs))(bool)) {
    debug_assert(lhs.type.size == type.size);
    debug_assert(rhs.type.size == type.size);
    debug_assert(lhs.type.align == type.align);
    debug_assert(rhs.type.align == type.align);
    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0) { return true; }
    debug_assert(lhs.ptr != null);
    debug_assert(rhs.ptr != null);
    return __builtin_memcmp(lhs.ptr, rhs.ptr, (lhs.len * type.size)) == 0;
}
$extern fn_((mem_eql2$i64(S_const$i64 lhs, S_const$i64 rhs))(bool)) {
    return mem_eql2$raw(typeInfo$(TypeOf(i64)), metaRef$S(lhs), metaRef$S(rhs));
}

/*
mem_eql0$i64:
        cmp     rsi, rcx
        jne     .LBB0_1
        test    rsi, rsi
        je      .LBB0_3
        push    rax
        shl     rsi, 3
        mov     rax, rsi
        mov     rsi, rdx
        mov     rdx, rax
        call    memcmp@PLT
        test    eax, eax
        sete    al
        add     rsp, 8
        ret
.LBB0_1:
        xor     eax, eax
        ret
.LBB0_3:
        mov     al, 1
        ret

mem_eql1$i64:
        cmp     rsi, rcx
        jne     .LBB1_1
        test    rsi, rsi
        je      .LBB1_3
        push    rax
        shl     rsi, 3
        mov     rax, rsi
        mov     rsi, rdx
        mov     rdx, rax
        call    memcmp@PLT
        test    eax, eax
        sete    al
        add     rsp, 8
        ret
.LBB1_1:
        xor     eax, eax
        ret
.LBB1_3:
        mov     al, 1
        ret

mem_eql2$i64:
        cmp     rsi, rcx
        jne     .LBB2_1
        test    rsi, rsi
        je      .LBB2_3
        push    rax
        shl     rsi, 3
        mov     rax, rsi
        mov     rsi, rdx
        mov     rdx, rax
        call    memcmp@PLT
        test    eax, eax
        sete    al
        add     rsp, 8
        ret
.LBB2_1:
        xor     eax, eax
        ret
.LBB2_3:
        mov     al, 1
        ret
*/
