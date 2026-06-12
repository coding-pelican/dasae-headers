#ifndef prl_va__included
#define prl_va__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl/Clsr.h"

/*========== Macros and Declarations ========================================*/
/*========== Macros and Definitions =========================================*/

#define $va_args_decl S_const$TypeInfo, u_P_const$raw
#define $va_args $va_args_($va_ty_tup_fields, $va_ref_tup)
#define $va_args_(_ty_tup_fields, _ref_tup) \
    S_const$TypeInfo _ty_tup_fields, u_P_const$raw _ref_tup

#define $va_ty_tup_fields ty_tup_fields
#define $va_ref_tup ref_tup

#define va_(/*(_fn)(_args...)(...)*/...) __expr__va_(__VA_ARGS__)
#define __expr__va_(...) __step__va_(__pp__va___parseFn __VA_ARGS__)
#define __pp__va___parseFn(_fn...) _fn, __pp__va___parseArgs
#define __pp__va___parseArgs(_args...) (_args), __pp__va___parseVaArgs
#define __pp__va___parseVaArgs(_va_args...) \
    pp_uniqTok(ty_typ_fields), pp_uniqTok(v_tup), (_va_args)
#define __step__va_(...) __inline__va_(__VA_ARGS__)
#define __inline__va_(_fn, _args, __ty_tup_fields, __v_tup, _va_args...) local_({ \
    let __ty_tup_fields = __inline__va___tyTupFields _va_args; \
    let __v_tup = __inline__va___vTup _va_args; \
    local_return_(_fn(__inline__va___pass(_args, __ty_tup_fields, __v_tup))); \
})
#define __inline__va___tyTupFields(_va_args...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(__inline__va___tyTupFields__each, ~, _va_args) }))
#define __inline__va___tyTupFields__each(_$ignored, _va_arg...) \
    typeInfo$(TypeOf(_va_arg)),
#define __inline__va___vTup(_va_args...) \
    pp_overload(__inline__va___vTup, _va_args)(_va_args)
#define __inline__va___vTup_0(...) \
    $tup()
#define __inline__va___vTup_1(_arg0...) \
    $tup((_arg0))
#define __inline__va___vTup_2(_arg0, _arg1...) \
    $tup((_arg0), (_arg1))
#define __inline__va___vTup_3(_arg0, _arg1, _arg2...) \
    $tup((_arg0), (_arg1), (_arg2))
#define __inline__va___vTup_4(_arg0, _arg1, _arg2, _arg3...) \
    $tup((_arg0), (_arg1), (_arg2), (_arg3))
#define __inline__va___vTup_5(_arg0, _arg1, _arg2, _arg3, _arg4...) \
    $tup((_arg0), (_arg1), (_arg2), (_arg3), (_arg4))
#define __inline__va___vTup_6(_arg0, _arg1, _arg2, _arg3, _arg4, _arg5...) \
    $tup((_arg0), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5))
#define __inline__va___vTup_7(_arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6...) \
    $tup((_arg0), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6))
#define __inline__va___vTup_8(_arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7...) \
    $tup((_arg0), (_arg1), (_arg2), (_arg3), (_arg4), (_arg5), (_arg6), (_arg7))
#define __inline__va___pass(_args, _ty_tup_fields, _v_tup...) \
    __inline__va___pass__args _args _ty_tup_fields, u_anyP(&_v_tup)
#define __inline__va___pass__args(/*_args*/...) __VA_OPT__( \
    __VA_ARGS__, \
)

#define va_clsr_(/*(_rtn)(_args...)(...)*/...) __expr__va_clsr_(__VA_ARGS__)
#define __expr__va_clsr_(...) __step__va_clsr_(__pp__va_clsr___parseRtn __VA_ARGS__)
#define __pp__va_clsr___parseRtn(_rtn...) _rtn, __pp__va___parseArgs
#define __step__va_clsr_(...) __inline__va_clsr_(__VA_ARGS__)
#define __inline__va_clsr_(_rtn, _args, __ty_tup_fields, __v_tup, _va_args...) \
    copy(__inline__va_(Clsr_from_(_rtn), _args, __ty_tup_fields, __v_tup, _va_args))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_va__included */
