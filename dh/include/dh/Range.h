#ifndef Range__included
#define Range__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Limit.h"

/*========== Macros and Declarations ========================================*/

#define Range$(_T...) tpl$(Range, _T)
#define Range$$(_T...) \
    struct { \
        var_(begin, Limit$$(_T)); \
        var_(end, Limit$$(_T)); \
    }
#define T_decl_Range$(_T...) \
    typedef struct Range$(_T) Range$(_T)
#define T_impl_Range$(_T...) \
    struct Range$(_T) { \
        var_(begin, Limit$(_T)); \
        var_(end, Limit$(_T)); \
    }
#define T_use_Range$(_T...) \
    T_decl_Range$(_T); \
    T_impl_Range$(_T)

#define Range_from$(_T, _begin, _end...) \
    l$((Range$(_T)){ .begin = _begin, .end = _end })
#define Range_inclExcl$(_T, _begin, _end...) \
    Range_from$(_T, Limit_incl$(_T, _begin), Limit_excl$(_T, _end))
#define Range_inclIncl$(_T, _begin, _end...) \
    Range_from$(_T, Limit_incl$(_T, _begin), Limit_incl$(_T, _end))
#define Range_exclExcl$(_T, _begin, _end...) \
    Range_from$(_T, Limit_excl$(_T, _begin), Limit_excl$(_T, _end))
#define Range_exclIncl$(_T, _begin, _end...) \
    Range_from$(_T, Limit_excl$(_T, _begin), Limit_incl$(_T, _end))

T_use_Range$(usize);
T_use_Range$(u64);
T_use_Range$(ulong);
T_use_Range$(u32);
T_use_Range$(u16);
T_use_Range$(u8);
T_use_Range$(isize);
T_use_Range$(i64);
T_use_Range$(ilong);
T_use_Range$(i32);
T_use_Range$(i16);
T_use_Range$(i8);
T_use_Range$(f64);
T_use_Range$(f32);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Range__included */
