#include "dh/main.h"

#define expr_(...) ({ __VA_ARGS__; })

/* Template Operators ========================================================*/

#define P_ref
#define P_deref
#define P_makeRef(expr...)       (&*(TypeOf(expr)[1]){ [0] = expr })
#define P_makeRef_mut(expr...)   (&*(TypeOfUnqual(expr)[1]){ [0] = expr })
#define P_makeRef_const(expr...) (&*(const TypeOf(expr)[1]){ [0] = expr })

#define P_constCast
#define P_mutCast
#define PZ_constCast
#define PZ_mutCast

#define S_from
#define S_asg
#define S_at
#define S_getAt
#define S_setAt
#define S_slice
#define S_sliceZ
#define S_constCast
#define S_mutCast

#define SZ_from
#define SZ_asg
#define SZ_at
#define SZ_getAt
#define SZ_setAt
#define SZ_slice
#define SZ_sliceZ
#define SZ_constCast
#define SZ_mutCast

#define E_ok
#define E_err
#define E_asg
#define E_isOk
#define E_isErr
#define E_try_
#define E_catch_

#define O_some
#define O_none
#define O_asg
#define O_isSome
#define O_isNone
#define O_unwrap
#define O_orelse

#define OP_some
#define OP_none
#define OP_asg
#define OP_isSome
#define OP_isNone
#define OP_unwrap
#define OP_orelse
#define OP_constCast
#define OP_mutCast

#define OPZ_some
#define OPZ_none
#define OPZ_asg
#define OPZ_isSome
#define OPZ_isNone
#define OPZ_unwrap
#define OPZ_orelse
#define OPZ_constCast
#define OPZ_mutCast

#define OS_some
#define OS_none
#define OS_asg
#define OS_isSome
#define OS_isNone
#define OS_unwrap
#define OS_orelse
#define OS_constCast
#define OS_mutCast

#define OSZ_some
#define OSZ_none
#define OSZ_asg
#define OSZ_isSome
#define OSZ_isNone
#define OSZ_unwrap
#define OSZ_orelse
#define OSZ_constCast
#define OSZ_mutCast

/* Template Alias ===========================================================*/

#define P_const$(T)  comp_tpl_alias__P_const$(T)
#define P$(T)        comp_tpl_alias__P$(T)
#define PZ_const$(T) comp_tpl_alias__PZ_const$(T)
#define PZ$(T)       comp_tpl_alias__PZ$(T)

#define S_const$(T)  comp_tpl_alias__S_const$(T)
#define S$(T)        comp_tpl_alias__S$(T)
#define SZ_const$(T) comp_tpl_alias__SZ_const$(T)
#define SZ$(T)       comp_tpl_alias__SZ$(T)

#define E$(T) comp_tpl_alias__E$(T)
#define O$(T) comp_tpl_alias__O$(T)

#define OP_const$(T)  comp_tpl_alias__OP_const$(T)
#define OP$(T)        comp_tpl_alias__OP$(T)
#define OPZ_const$(T) comp_tpl_alias__OPZ_const$(T)
#define OPZ$(T)       comp_tpl_alias__OPZ$(T)

#define OS_const$(T)  comp_tpl_alias__OS_const$(T)
#define OS$(T)        comp_tpl_alias__OS$(T)
#define OSZ_const$(T) comp_tpl_alias__OSZ_const$(T)
#define OSZ$(T)       comp_tpl_alias__OSZ$(T)

/* Template Instantiation ===================================================*/

#define T_use_P$(T)  comp_tpl_inst__T_use$_P$(T)
#define T_use_PZ$(T) comp_tpl_inst__T_use$_PZ$(T)

#define T_use_S$(T)  comp_tpl_inst__T_use$_S$(T)
#define T_use_SZ$(T) comp_tpl_inst__T_use$_SZ$(T)

#define T_use_E$(T) comp_tpl_inst__T_use$_E$(T)
#define T_use_O$(T) comp_tpl_inst__T_use$_O$(T)

#define T_use_OP$(T)  comp_tpl_inst__T_use$_OP$(T)
#define T_use_OPZ$(T) comp_tpl_inst__T_use$_OPZ$(T)
#define T_use_OS$(T)  comp_tpl_inst__T_use$_OS$(T)
#define T_use_OSZ$(T) comp_tpl_inst__T_use$_OSZ$(T)

// usage:
//   T_use$(T, (P, PZ, S, SZ, E, O, OP, OPZ, OS, OSZ));
// expanded:
//   T_use_P$(T);
//   T_use_PZ$(T);
//   T_use_S$(T);
//   ...
//   T_use_OSZ$(T);
// Main T_use$ macro
// Example usage:
//   T_use$(MyType, (P, PZ, S, SZ, E, O, OP, OPZ, OS, OSZ));
#define T_use$(_T, _List_Parenthesized) comp_tpl_inst__T_use$(_T, _List_Parenthesized)

/* Implementation ===========================================================*/

#define comp_tpl_alias__P_const$(T)   pp_join($, P_const, T)
#define comp_tpl_alias__P$(T)         pp_join($, P, T)
#define comp_tpl_alias__PZ_const$(T)  pp_join($, PZ_const, T)
#define comp_tpl_alias__PZ$(T)        pp_join($, PZ, T)
#define comp_tpl_alias__S_const$(T)   pp_join($, S_const, T)
#define comp_tpl_alias__S$(T)         pp_join($, S, T)
#define comp_tpl_alias__SZ_const$(T)  pp_join($, SZ_const, T)
#define comp_tpl_alias__SZ$(T)        pp_join($, SZ, T)
#define comp_tpl_alias__E$(T)         pp_join($, E, T)
#define comp_tpl_alias__O$(T)         pp_join($, O, T)
#define comp_tpl_alias__OP_const$(T)  pp_join($, OP_const, T)
#define comp_tpl_alias__OP$(T)        pp_join($, OP, T)
#define comp_tpl_alias__OPZ_const$(T) pp_join($, OPZ_const, T)
#define comp_tpl_alias__OPZ$(T)       pp_join($, OPZ, T)
#define comp_tpl_alias__OS_const$(T)  pp_join($, OS_const, T)
#define comp_tpl_alias__OS$(T)        pp_join($, OS, T)
#define comp_tpl_alias__OSZ_const$(T) pp_join($, OSZ_const, T)
#define comp_tpl_alias__OSZ$(T)       pp_join($, OSZ, T)

#define comp_tpl_inst__T_use$_P$(T) \
    typedef const rawptr$(T) P_const$(T); \
    typedef rawptr$(T) P$(T)
#define comp_tpl_inst__T_use$_PZ$(T) \
    typedef const rawptr$(T) PZ_const$(T); \
    typedef rawptr$(T) PZ$(T)
#define comp_tpl_inst__T_use$_S$(T) \
    typedef struct S_const$(T) { \
        var_(ptr, P_const$(T)); \
        var_(len, usize); \
    } S_const$(T); \
    typedef union S$(T) { \
        var_(as_const, S_const$(T)); \
        struct { \
            var_(ptr, P$(T)); \
            var_(len, usize); \
        }; \
    } S$(T)
#define comp_tpl_inst__T_use$_SZ$(T) \
    typedef struct SZ_const$(T) { \
        var_(ptr, PZ_const$(T)); \
        var_(len, usize); \
    } SZ_const$(T); \
    typedef union SZ$(T) { \
        var_(as_const, SZ_const$(T)); \
        struct { \
            var_(ptr, PZ$(T)); \
            var_(len, usize); \
        }; \
    } SZ$(T)
#define comp_tpl_inst__T_use$_E$(T) \
    typedef struct E$(T) { \
        union { \
            var_(ok, T); \
            var_(err, Err); \
        } payload; \
        var_(is_ok, bool); \
    } E$(T)
#define comp_tpl_inst__T_use$_O$(T) \
    typedef struct O$(T) { \
        union { \
            var_(some, T); \
            var_(none, Void); \
        } payload; \
        var_(has_value, bool); \
    } O$(T)
#define comp_tpl_inst__T_use$_OP$(T) \
    typedef struct OP_const$(T) { \
        var_(ptr, P_const$(T)); \
    } OP_const$(T); \
    typedef union OP$(T) { \
        var_(as_const, OP_const$(T)); \
        struct { \
            var_(ptr, P$(T)); \
        }; \
    } OP$(T)
#define comp_tpl_inst__T_use$_OPZ$(T) \
    typedef struct OPZ_const$(T) { \
        var_(ptr, PZ_const$(T)); \
    } OPZ_const$(T); \
    typedef union OPZ$(T) { \
        var_(as_const, OPZ_const$(T)); \
        struct { \
            var_(ptr, PZ$(T)); \
        }; \
    } OPZ$(T)
#define comp_tpl_inst__T_use$_OS$(T) \
    typedef struct OS_const$(T) { \
        var_(sli, S_const$(T)); \
    } OS_const$(T); \
    typedef union OS$(T) { \
        var_(as_const, OS_const$(T)); \
        struct { \
            var_(sli, S$(T)); \
        }; \
    } OS$(T)
#define comp_tpl_inst__T_use$_OSZ$(T) \
    typedef struct OSZ_const$(T) { \
        var_(sli, SZ_const$(T)); \
    } OSZ_const$(T); \
    typedef union OSZ$(T) { \
        var_(as_const, OSZ_const$(T)); \
        struct { \
            var_(sli, SZ$(T)); \
        }; \
    } OSZ$(T)

#define comp_tpl_inst__T_use$(_T, _List_Parenthesized) \
    pp_foreach (__T_use$__each, _T, __T_use$__unwrap _List_Parenthesized)
// Macro to expand a single suffix with the type
#define __T_use$__each(_T, _suffix) pp_cat3(T_use_, _suffix, $)(_T);
// Helper macro to unwrap parenthesized arguments
#define __T_use$__unwrap(_List...)  _List

/* Example usage ============================================================*/
T_use$(i32, (P, PZ, S, SZ, E, O, OP, OPZ, OS, OSZ));

fn_((sample(void))(Err$i32) $scope) {
    i32 x      = 123;
    let_ignore = P_makeRef(x);
    let_ignore = P_makeRef_mut(x);
    let_ignore = P_makeRef_const(x);
    return_ok(x);
} $unscoped_(fn);
