#include "dh/prl.h"

#define __call__pp_switch_() __pp_switch_
#define pp_switch_(/*(_pp_cond)(_pp_cases...)*/...) __step__pp_switch_(__step__pp_switch___parseCond __VA_ARGS__)
#define __step__pp_switch___parseCond(_pp_cond...) _pp_cond, __step__pp_switch___parseCases
#define __step__pp_switch___parseCases(_pp_cases...) _pp_cases
#define __step__pp_switch_(...) __pp_switch_(__VA_ARGS__)
#define __pp_switch_(_pp_cond, _pp_cases...) __pp_switch__expand(__pp_switch___eval(_pp_cond, _pp_cases))
#define __pp_switch__expand(...) /* evaluate 2^5 */ __pp_switch__expand5(__VA_ARGS__)
#define __pp_switch__expand5(...) __pp_switch__expand4(__pp_switch__expand4(__VA_ARGS__))
#define __pp_switch__expand4(...) __pp_switch__expand3(__pp_switch__expand3(__VA_ARGS__))
#define __pp_switch__expand3(...) __pp_switch__expand2(__pp_switch__expand2(__VA_ARGS__))
#define __pp_switch__expand2(...) __pp_switch__expand1(__pp_switch__expand1(__VA_ARGS__))
#define __pp_switch__expand1(...) __VA_ARGS__
#define __call__pp_switch___eval() __pp_switch___eval
#define __pp_switch___eval(_pp_cond, _tag, ...) \
    pp_cat(__pp_switch___eval, _tag)(_pp_cond, __VA_ARGS__)
#define __pp_switch___eval$_pp_case(_pp_cond, _pp_enum, _pp_expr, ...) \
    pp_defer(__call__pp_if)()(pp_Tok_eq(_pp_cond, _pp_enum))(_pp_expr, (__call__pp_switch___eval)()(_pp_cond, __VA_ARGS__))
#define __pp_switch___eval$_pp_default(_pp_cond, _pp_expr...) \
    _pp_expr

#define pp_case_(/*(_pp_enum)(_pp_expr)*/...) __step__pp_case_(__step__pp_case___parse __VA_ARGS__)
#define __step__pp_case___parse(_pp_enum...) _pp_enum,
#define __step__pp_case_(...) __pp_case_(__VA_ARGS__)
#define __pp_case_(_pp_enum, _pp_expr...) $_pp_case, _pp_enum, _pp_expr
#define pp_default_(_pp_expr...) __pp_default_(_pp_expr)
#define __pp_default_(_pp_expr...) $_pp_default, (_pp_expr)

/* static int a = pp_switch_((lang_mode)(
    pp_case_((lang_mode_c)(1)),
    pp_case_((lang_mode_cpp)(2)),
    pp_case_((lang_mode_unknown)(3)),
    pp_default_(4)
)); */

#if UNUSED_CODE
// pp_switch_((lang_mode)(
//     pp_case_((lang_mode_c)(lang_version_c)),
//     pp_case_((lang_mode_cpp)(lang_version_cpp)),
//     pp_default_(lang_version_unknown),
// ))
// #define pp_switch_(/*(_pp_cond)(_pp_cases...)*/...)
// #define pp_case_(/*(_pp_enum)(_pp_expr...)*/...)
// #define pp_default_(/*(_pp_expr...)*/...)

// #define pp_switch_(/*(_pp_cond)(_pp_case...)*/...) \
//     __step__pp_switch_(__step__pp_switch___parseCond __VA_ARGS__)
// #define __step__pp_switch_(...) __pp_switch_(__VA_ARGS__)
// #define __step__pp_switch___parseCond(_Cond...) _Cond, __step__pp_switch___parseCases
// #define __step__pp_switch___parseCases(_Cases...) _Cases
// #define __pp_switch_(_Cond, _Cases...) \
//     pp_foreach_(__step__pp_switch___case, _Cond, _Cases)
// #define __step__pp_switch___case(_Cond, _Case...) \

// lang_mode, lang_mode_c, lang_version_c, lang_mode_cpp, lang_version_cpp, lang_version_unknown
// lang_mode, (lang_mode_c)(lang_version_c), (lang_mode_cpp)(lang_version_cpp), lang_version_unknown

// pp_if_(pp_eq(lang_mode, lang_mode_c))
// pp_if_(pp_eq(lang_mode, lang_mode_cpp))
// lang_version_unknown

// pp_if_(pp_eq(lang_mode, lang_mode_c))(lang_version_c)
// pp_if_(pp_eq(lang_mode, lang_mode_cpp))(lang_version_cpp)
// (lang_version_unknown)


// pp_if_(pp_eq(lang_mode, lang_mode_c))(lang_version_c, pp_if_(pp_eq(lang_mode, lang_mode_cpp))(lang_version_cpp, lang_version_unknown))
// lang_mode, lang_mode_c, (lang_version_c), lang_mode_cpp, (lang_version_cpp), lang_version_unknow
#define __step__switch_(_cond, _cases...) _cond, _cases

#define pp_case_(/**(_pp_enum)(_pp_expr...)*/...) __step__pp_case_(__step__pp_case___parse __VA_ARGS__)
#define __step__pp_case___parse(_pp_enum...) _pp_enum,
#define __step__pp_case_(_pp_enum, _pp_expr...) $_pp_case, _pp_enum, (_pp_expr)
#define pp_default_(_pp_expr...) __step__pp_default_(_pp_expr)
#define __step__pp_default_(_pp_expr...) $_pp_default, (_pp_expr)
// __step__switch_(lang_mode, __step__case_(lang_mode_c, lang_version_c), __step__case_(lang_mode_cpp, lang_version_cpp), lang_version_unknown)
// 각 케이스마다 elem count하여 case인지 default인지 판단 가능 or '식별자 도입'
#define __call__switch___eval() __step__switch___eval
#define __call__pp_if() pp_if_
#define __switch___eval(...) __step__switch__expand(__step__switch___eval(__VA_ARGS__))
#define __step__switch___eval(_cond, _tag, ...) \
    pp_cat(__step__switch___eval, _tag)(_cond, __VA_ARGS__)
#define __step__switch___eval$_pp_case(_cond, _enum, _expr, ...) \
    pp_defer(__call__pp_if)()(pp_Tok_eq(_cond, _enum))(_expr, (__call__switch___eval)()(_cond, __VA_ARGS__))
#define __step__switch___eval$_pp_default(_cond, _expr...) \
    _expr

// pp_expand(pp_expand(pp_expand(pp_expand(__step__switch___eval(lang_mode, $_pp_case, lang_mode_c, (lang_version_c), $_pp_case, lang_mode_cpp, (lang_version_cpp), $_pp_default, (lang_version_unknown))))))

#define __pp_switch__expand(...) /* evaluate 2^5 */ __pp_switch__expand5(__VA_ARGS__)
#define __pp_switch__expand5(...) __pp_switch__expand4(__pp_switch__expand4(__VA_ARGS__))
#define __pp_switch__expand4(...) __pp_switch__expand3(__pp_switch__expand3(__VA_ARGS__))
#define __pp_switch__expand3(...) __pp_switch__expand2(__pp_switch__expand2(__VA_ARGS__))
#define __pp_switch__expand2(...) __pp_switch__expand1(__pp_switch__expand1(__VA_ARGS__))
#define __pp_switch__expand1(...) __VA_ARGS__

let a
    = __switch___eval(__step__switch_(
        my_enum,
        __step__case_(my_enum_value1, 1),
        __step__case_(my_enum_value2, 2),
        __step__case_(my_enum_value3, 3),
        __step__case_(my_enum_value4, 4),
        __step__case_(my_enum_value5, 5),
        __step__case_(my_enum_value6, 6),
        __step__case_(my_enum_value7, 7),
        __step__case_(my_enum_value8, 8),
        __step__case_(my_enum_value9, 9),
        __step__case_(my_enum_value10, 10),
        __step__case_(my_enum_value11, 11),
        __step__case_(my_enum_value12, 12),
        __step__case_(my_enum_value13, 13),
        __step__case_(my_enum_value14, 14),
        __step__case_(my_enum_value15, 15),
        __step__case_(my_enum_value16, 16),
        __step__case_(my_enum_value17, 17),
        __step__case_(my_enum_value18, 18),
        __step__case_(my_enum_value19, 19),
        __step__case_(my_enum_value20, 20),
        __step__case_(my_enum_value21, 21),
        __step__case_(my_enum_value22, 22),
        __step__case_(my_enum_value23, 23),
        __step__case_(my_enum_value24, 24),
        __step__case_(my_enum_value25, 25),
        __step__case_(my_enum_value26, 26),
        __step__case_(my_enum_value27, 27),
        __step__case_(my_enum_value28, 28),
        __step__case_(my_enum_value29, 29),
        __step__case_(my_enum_value30, 30),
        __step__case_(my_enum_value31, 31),
        __step__case_(my_enum_value32, 32),
        __step__default_(0)
    ))
#endif /* UNUSED_CODE */
