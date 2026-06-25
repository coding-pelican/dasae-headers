#include "dh/prl.h"

#define A_from_static$(/*(_T){ _initial...}*/... /*(A(n: usize,_T))*/) \
    __step__A_from_static$__emit(__step__A_from_static$__parseT __VA_ARGS__)
#define __step__A_from_static$__parseT(_T...) _T,
#define __step__A_from_static$__emit(...) \
    __inline__A_from_static$(__VA_ARGS__)
#define __inline__A_from_static$(_T, _initial...) \
    l$((A$$((sizeOf$(TypeOf((_T[])_initial)) / sizeOf$(_T)), _T)){ .val = _initial })
#define fromA_static$ A_from_static$

#define A_from$(/*(_T){ _initial...}*/... /*(A(n: usize,_T))*/) \
    __step__A_from$__emit(__step__A_from$__parseT __VA_ARGS__)
#define __step__A_from$__parseT(_T...) _T,
#define __step__A_from$__emit(...) \
    __inline__A_from$(__VA_ARGS__)
#define __inline__A_from$(_T, _initial...) local_({ \
    typedef _T A_T; \
    typedef TypeOf(l$((A_T[])_initial)) A_Val; \
    typedef A$$(sizeOf$(A_Val) / sizeOf$(A_T), A_T) A_Self; \
    local_return_( \
        *ptrCast$((A_Self*)(raw_memcpy( \
            &l0$((A_Self)), \
            &l$((A_T[])_initial), \
            sizeOf$(A_Self) \
        ))) \
    ); \
})
#define fromA$ A_from$
