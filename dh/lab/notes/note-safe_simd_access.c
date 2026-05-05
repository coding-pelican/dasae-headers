#include <stdio.h>

#define pp_exec_cat(_LhsTok, _RhsTok...)            _LhsTok##_RhsTok
#define pp_exec_cat2(_LhsTok, _RhsTok...)           _LhsTok##_RhsTok
#define pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_Tok_mid##_RhsTok

#define pp_exec_join(_SepTok, _LhsTok, _RhsTok...)            _LhsTok##_SepTok##_RhsTok
#define pp_exec_join2(_SepTok, _LhsTok, _RhsTok...)           _LhsTok##_SepTok##_RhsTok
#define pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) _LhsTok##_SepTok##_Tok_mid##_SepTok##_RhsTok

#define pp_exec_uniqTok(_Tok...)       pp_join(_, pp_join3(_, _, __LINE__, __COUNTER__), _Tok)
#define pp_exec_uniqTokByLine(_Tok...) pp_join(_, pp_join2(_, _, __LINE__), _Tok)

#define pp_cat(_LhsTok, _RhsTok...)            pp_exec_cat(_LhsTok, _RhsTok)
#define pp_cat2(_LhsTok, _RhsTok...)           pp_exec_cat2(_LhsTok, _RhsTok)
#define pp_cat3(_LhsTok, _Tok_mid, _RhsTok...) pp_exec_cat3(_LhsTok, _Tok_mid, _RhsTok)

#define pp_join(_SepTok, _LhsTok, _RhsTok...)            pp_exec_join(_SepTok, _LhsTok, _RhsTok)
#define pp_join2(_SepTok, _LhsTok, _RhsTok...)           pp_exec_join2(_SepTok, _LhsTok, _RhsTok)
#define pp_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok...) pp_exec_join3(_SepTok, _LhsTok, _Tok_mid, _RhsTok)

#define pp_uniqTok(_Tok...)       pp_exec_uniqTok(_Tok)
#define pp_uniqTokByLine(_Tok...) pp_exec_uniqTokByLine(_Tok)

#define as$

#define let          __auto_type const
#define var          __auto_type
#define var_(id, _T) _T id
#define let_(id, _T) _T const id

#define TypeOf(...)                 __typeof__(__VA_ARGS__)
#define sizeOf$(...)                sizeof(__VA_ARGS__)
#define countOf$(_T... /*(usize)*/) (sizeOf$(_T) / sizeOf$(TypeOf((*as$(_T*)(0))[0])))

#define $Vec$(_N, _T...) TypeOf(_T __attribute__((vector_size(_N * sizeOf$(_T)))))
#define Vec$$(_N, _T...) $Vec$(_N, _T)

/// Type declaration helpers
#define T_use_Vec$(_N, _T) \
    T_impl_Vec$(_N, _T)
#define T_decl_Vec$(_N, _T) \
    typedef $Vec$(_N, _T) Vec$(_N, _T)
#define T_impl_Vec$(_N, _T) \
    typedef $Vec$(_N, _T) Vec$(_N, _T)

#define simd_splat(_N, _T, _val...) \
    __op__simd_splat$($Vec$(_N, _T), _val)
#define __op__simd_splat$(_VT, _val) ({ \
    var_(__result, _VT); \
    let __scalar = (_val); \
    for (usize __i = 0; __i < sizeof(_VT) / sizeof(__scalar); ++__i) { \
        __result[__i] = __scalar; \
    } \
    __result; \
})

#define Vec_len$(_T... /*(usize)*/)                      __op__Vec_len$(_T)
#define __op__Vec_len$(_T...)                            countOf$(_T)
#define Vec_len(_self /*: Vec$(_N, _T)*/... /*(usize)*/) __op__Vec_len(_self)
#define __op__Vec_len(_self...)                          Vec_len$(TypeOf(_self))

#define Vec_at(/*(_self: Vec$(_N, _T))[_idx]*/... /*(P(_T))*/) \
    __op__Vec_at(__op__Vec_at__parse __VA_ARGS__)
#define __op__Vec_at__parse(_self...)                     pp_uniqTok(self), pp_uniqTok(idx), _self,
#define __op__Vec_at(...)                                 __op__Vec_at__emit(__VA_ARGS__)
#define __op__Vec_at__emit(__self, __idx, _self, _idx...) ({ \
    let_(__self, TypeOf(_self)*) = &_self; \
    typedef TypeOf((*__self)[0]) ScalarType; \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    if (__idx >= Vec_len(*__self)) { __builtin_unreachable(); } \
    as$(ScalarType*)(__self) + __idx; \
})

typedef float f32;
typedef unsigned char u8;
typedef unsigned long long usize;

void example_basic_arithmetic(void) {
    /* Create two 4-element float vectors */
    let a = simd_splat(4, f32, 1.0f);
    var b = simd_splat(4, f32, 2.0f);

    var cnt = 0;
    printf("cnt = %d\n", cnt);
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("cnt = %d\n", cnt);

    let b0 = Vec_at((b)[0]);
    *b0 = 3.0f;
    let b1 = Vec_at((b)[1]);
    *b1 = 4.0f;
    let b2 = Vec_at((b)[2]);
    *b2 = 5.0f;
    let b3 = Vec_at((b)[3]);
    *b3 = 6.0f;
    printf("%f\n", *Vec_at((b)[0]));
    printf("%f\n", *Vec_at((b)[1]));
    printf("%f\n", *Vec_at((b)[2]));
    printf("%f\n", *Vec_at((b)[3]));
}

void example_basic_arithmetic2(void) {
    /* Create two 4-element float vectors */
    let a = simd_splat(4, f32, 1.0f);
    var b = simd_splat(4, f32, 2.0f);

    var cnt = 0;
    printf("cnt = %d\n", cnt);
    printf("%f\n", a[cnt++]);
    printf("%f\n", a[cnt++]);
    printf("%f\n", a[cnt++]);
    printf("%f\n", a[cnt++]);
    printf("cnt = %d\n", cnt);

    b[0] = 3.0f;
    b[1] = 4.0f;
    b[2] = 5.0f;
    b[3] = 6.0f;
    printf("%f\n", *Vec_at((b)[0]));
    printf("%f\n", *Vec_at((b)[1]));
    printf("%f\n", *Vec_at((b)[2]));
    printf("%f\n", *Vec_at((b)[3]));
}
