#ifndef prl_types_raw__included
#define prl_types_raw__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)
#define $A$(_N, _T...) TypeOf(_T[_N])

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

typedef union V$raw {
    u8 inner $flexible;
} V$raw;

typedef struct S_const$raw {
    P_const$raw ptr;
    usize len;
} S_const$raw;
typedef union S$raw {
    struct {
        P$raw ptr;
        usize len;
    };
    S_const$raw as_const;
} S$raw;

typedef struct A$raw {
    u8 inner $flexible;
} A$raw;

typedef union O_Payload$raw {
    Void none;
    u8 some $flexible;
} O_Payload$raw;
typedef struct O$raw {
    bool is_some;
    union {
        Void none;
        u8 some $flexible;
        O_Payload$raw raw[1];
    } payload[1];
} O$raw;

typedef i32 ErrCode;
enum ErrCode {
    ErrCode_Unknown = -6,
    ErrCode_Unexpected = -5,
    ErrCode_Unspecified = -4,
    ErrCode_Unsupported = -3,
    ErrCode_NotImplemented = -2,
    ErrCode_InvalidArgument = -1,
    ErrCode_None = 0
};
typedef struct Err_VT {
    fn_(((*domainToStrZ)(ErrCode ctx))(const u8*));
    fn_(((*codeToStrZ)(ErrCode ctx))(const u8*));
} Err_VT;
typedef struct Err {
    ErrCode ctx;
    const Err_VT* vt;
} Err;

typedef union E_Payload$raw E_Payload$raw;
union E_Payload$raw {
    u32 err;
    u8 ok $flexible;
};

typedef struct E$raw E$raw;
struct E$raw {
    bool is_ok;
    union {
        u32 err;
        u8 ok $flexible;
        E_Payload$raw raw[1];
    } payload[1];
};

#define T_decl$(/*(_T)(_List_Parenthesized)*/...) __tpl_inst__T_decl$(__tpl_inst__T_decl$__parseT __VA_ARGS__)
#define __tpl_inst__T_decl$(...) __tpl_inst__T_decl$__emit(__VA_ARGS__)
#define __tpl_inst__T_decl$__parseT(_T...) _T,
#define __tpl_inst__T_decl$__emit(_T, _List_Parenthesized...) \
    pp_foreach(__tpl_inst__T_decl$__each, _T, __tpl_inst__T_decl$__unwrap _List_Parenthesized)
#define __tpl_inst__T_decl$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_decl_, __VA_ARGS__, $)(_T); \
)
#define __tpl_inst__T_decl$__unwrap(_List...) _List

#define T_impl$(/*(_T)(_List_Parenthesized)*/...) __tpl_inst__T_impl$(__tpl_inst__T_impl$__parseT __VA_ARGS__)
#define __tpl_inst__T_impl$(...) __tpl_inst__T_impl$__emit(__VA_ARGS__)
#define __tpl_inst__T_impl$__parseT(_T...) _T,
#define __tpl_inst__T_impl$__emit(_T, _List_Parenthesized...) \
    pp_foreach(__tpl_inst__T_impl$__each, _T, __tpl_inst__T_impl$__unwrap _List_Parenthesized)
#define __tpl_inst__T_impl$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_impl_, __VA_ARGS__, $)(_T); \
)
#define __tpl_inst__T_impl$__unwrap(_List...) _List

#define T_use$(/*(_T)(_List_Parenthesized)*/...) __tpl_inst__T_use$(__tpl_inst__T_use$__parseT __VA_ARGS__)
#define __tpl_inst__T_use$(...) __tpl_inst__T_use$__emit(__VA_ARGS__)
#define __tpl_inst__T_use$__parseT(_T...) (_T),
#define __tpl_inst__T_use$__emit(_T, _List_Parenthesized...) \
    pp_foreach(__tpl_inst__T_use$__each, _T, __tpl_inst__T_use$__unwrap _List_Parenthesized)
#define __tpl_inst__T_use$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_use_, __VA_ARGS__, $) _T; \
)
#define __tpl_inst__T_use$__unwrap(_List...) _List

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_raw__included */
