#ifndef dal_c_ext_pp__included
#define dal_c_ext_pp__included 1

/*========== Macros and Declarations ========================================*/

#define pp_expand(...) pp__step__expand(__VA_ARGS__)
#define pp_strfy(_code...) pp__val__strfy(_code)

/*========== Macros and Definitions =========================================*/

#define pp__step__expand(...) __VA_ARGS__
#define pp__val__strfy(_code...) #_code

#endif /* dal_c_ext_pp__included */
