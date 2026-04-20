#ifndef dal_c_ext_dsl__included
#define dal_c_ext_dsl__included 1

/*========== Macros and Declarations ========================================*/

#define dsl_l$(/*(_T){_initial...}*/... /*(_T)*/) dsl__val__l$(__VA_ARGS__)
#define dsl_l0$(/*(_T)*/... /*(_T)*/) dsl__val__l0$(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define dsl__val__l$(...) dsl__step__l$(dsl__step__l$__parse __VA_ARGS__)
#define dsl__step__l$__parse(_T...) _T,
#define dsl__step__l$(_T, _initial...) ((_T)_initial)

#define dsl__val__l0$(...) dsl__step__l0$(dsl__step__l0$__parse __VA_ARGS__)
#define dsl__step__l0$__parse(_T...) _T
#define dsl__step__l0$(_T...) (((struct { _T val; }){}).val)

#endif /* dal_c_ext_dsl__included */
