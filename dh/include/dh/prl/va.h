#pragma once
#ifndef prl_va__included
#define prl_va__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Clsr.h"

/*========== Macros and Declarations ========================================*/
/*========== Macros and Definitions =========================================*/

#define $va_args_decl S_const$TypeInfo, u_P_const$raw
#define $va_args $va_args_($va_ty_tup_fields, $va_ref_tup)
#define $va_args_(_$ty_tup_fields, _$ref_tup) \
    S_const$TypeInfo _$ty_tup_fields, u_P_const$raw _$ref_tup

#define $va_ty_tup_fields ty_tup_fields
#define $va_ref_tup ref_tup

#define va_(/*(_$fn)(_$args...)(...)*/...) __expr__va_(__VA_ARGS__)
#define __expr__va_(...) __step__va_(__pp__va___parseFn __VA_ARGS__)
#define __pp__va___parseFn(_$fn...) _$fn, __pp__va___parseArgs
#define __pp__va___parseArgs(_$args...) (_$args), __pp__va___parseVaArgs
#define __pp__va___parseVaArgs(_$va_args...) \
    pp_uniqTok(ty_typ_fields), pp_uniqTok(v_tup), (_$va_args)
#define __step__va_(...) __inline__va_(__VA_ARGS__)
#define __inline__va_(_$fn, _$args, __ty_tup_fields, __v_tup, _$va_args...) local_({ \
    let __ty_tup_fields = __inline__va___tyTupFields _$va_args; \
    let __v_tup = __inline__va___vTup _$va_args; \
    local_return_(_$fn(__inline__va___pass(_$args, __ty_tup_fields, __v_tup))); \
})
#define __inline__va___tyTupFields(_$va_args...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(__inline__va___tyTupFields__each, ~, _$va_args) }))
#define __inline__va___tyTupFields__each(_$ignored, _va_arg...) \
    typeInfo$(TypeOf(_va_arg)),
#define __inline__va___vTup(_$va_args...) \
    $tup(pp_overload(__inline__va___vTup, _$va_args)(_$va_args))
#define __inline__va___vTup_0(...)
#define __inline__va___vTup_1(_$arg0...) \
    (_$arg0)
#define __inline__va___vTup_2(_$arg0, _$arg1...) \
    __inline__va___vTup_1(_$arg0), (_$arg1)
#define __inline__va___vTup_3(_$arg0, _$arg1, _$arg2...) \
    __inline__va___vTup_2(_$arg0, _$arg1), (_$arg2)
#define __inline__va___vTup_4(_$arg0, _$arg1, _$arg2, _$arg3...) \
    __inline__va___vTup_3(_$arg0, _$arg1, _$arg2), (_$arg3)
#define __inline__va___vTup_5(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4...) \
    __inline__va___vTup_4(_$arg0, _$arg1, _$arg2, _$arg3), (_$arg4)
#define __inline__va___vTup_6(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5...) \
    __inline__va___vTup_5(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4), (_$arg5)
#define __inline__va___vTup_7(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6...) \
    __inline__va___vTup_6(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5), (_$arg6)
#define __inline__va___vTup_8(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7...) \
    __inline__va___vTup_7(_$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6), (_$arg7)
#define __inline__va___vTup_9( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8... \
) \
    __inline__va___vTup_8( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7 \
    ), \
        (_$arg8)
#define __inline__va___vTup_10( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9... \
) \
    __inline__va___vTup_9( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8 \
    ), \
        (_$arg9)
#define __inline__va___vTup_11( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10... \
) \
    __inline__va___vTup_10( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9 \
    ), \
        (_$arg10)
#define __inline__va___vTup_12( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10, _$arg11... \
) \
    __inline__va___vTup_11( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9, _$arg10 \
    ), \
        (_$arg11)
#define __inline__va___vTup_13( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10, _$arg11, _$arg12... \
) \
    __inline__va___vTup_12( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9, _$arg10, _$arg11 \
    ), \
        (_$arg12)
#define __inline__va___vTup_14( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10, _$arg11, _$arg12, _$arg13... \
) \
    __inline__va___vTup_13( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9, _$arg10, _$arg11, _$arg12 \
    ), \
        (_$arg13)
#define __inline__va___vTup_15( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10, _$arg11, _$arg12, _$arg13, _$arg14... \
) \
    __inline__va___vTup_14( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9, _$arg10, _$arg11, _$arg12, _$arg13 \
    ), \
        (_$arg14)
#define __inline__va___vTup_16( \
    _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
    _$arg8, _$arg9, _$arg10, _$arg11, _$arg12, _$arg13, _$arg14, _$arg15... \
) \
    __inline__va___vTup_15( \
        _$arg0, _$arg1, _$arg2, _$arg3, _$arg4, _$arg5, _$arg6, _$arg7, \
        _$arg8, _$arg9, _$arg10, _$arg11, _$arg12, _$arg13, _$arg14 \
    ), \
        (_$arg15)
#define __inline__va___pass(_$args, _$ty_tup_fields, _$v_tup...) \
    __inline__va___pass__args _$args _$ty_tup_fields, u_anyP(&_$v_tup)
#define __inline__va___pass__args(/*_$args*/...) __VA_OPT__( \
    __VA_ARGS__, \
)

#define va_clsr_(/*(_$rtn)(_$args...)(...)*/...) __expr__va_clsr_(__VA_ARGS__)
#define __expr__va_clsr_(...) __step__va_clsr_(__pp__va_clsr___parseRtn __VA_ARGS__)
#define __pp__va_clsr___parseRtn(_$rtn...) _$rtn, __pp__va___parseArgs
#define __step__va_clsr_(...) __inline__va_clsr_(__VA_ARGS__)
#define __inline__va_clsr_(_$rtn, _$args, __ty_tup_fields, __v_tup, _$va_args...) \
    copy(__inline__va_(Clsr_from_(_$rtn), _$args, __ty_tup_fields, __v_tup, _$va_args))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_va__included */
