#ifndef Limit__included
#define Limit__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Limit_Bound)(enum_((Limit_Bound $fits($packed))(
    Limit_Bound_incl = 0,
    Limit_Bound_excl = 1,
))));

#define Limit$(_T...) tpl$(Limit, _T)
#define Limit$$(_T...) \
    struct { \
        var_(point, _T); \
        var_(bound, Limit_Bound); \
    }
#define T_decl_Limit$(_T...) \
    typedef struct Limit$(_T) Limit$(_T)
#define T_impl_Limit$(_T...) \
    struct Limit$(_T) { \
        var_(point, _T); \
        var_(bound, Limit_Bound); \
    }
#define T_use_Limit$(_T...) \
    T_decl_Limit$(_T); \
    T_impl_Limit$(_T)

#define Limit_incl$(_T, _point...) l$((Limit$(_T)){ .point = _point, .bound = Limit_Bound_incl })
#define Limit_excl$(_T, _point...) l$((Limit$(_T)){ .point = _point, .bound = Limit_Bound_excl })

T_use_Limit$(usize);
T_use_Limit$(u64);
T_use_Limit$(ulong);
T_use_Limit$(u32);
T_use_Limit$(u16);
T_use_Limit$(u8);
T_use_Limit$(isize);
T_use_Limit$(i64);
T_use_Limit$(ilong);
T_use_Limit$(i32);
T_use_Limit$(i16);
T_use_Limit$(i8);
T_use_Limit$(f64);
T_use_Limit$(f32);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Limit__included */
