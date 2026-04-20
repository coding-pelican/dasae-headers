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

// typedef fn_(((*Fn$raw)(V$raw* ctx))(V$raw*));
// typedef struct FnCtx$raw {
//     Fn$raw fn;
//     V$raw* inner;
// } FnCtx$raw;

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

#define T_alias$(/*(_Alias)(_T)*/...) __stmt__T_alias$(__VA_ARGS__)
#define __stmt__T_alias$(...) __step__T_alias$__emit(__step__T_alias$__parseAlias __VA_ARGS__)
#define __step__T_alias$__parseAlias(_Alias...) _Alias, __step__T_alias$__parseT
#define __step__T_alias$__parseT(_T...) _T
#define __step__T_alias$__emit(...) ____T_alias$(__VA_ARGS__)
#define ____T_alias$(_Alias, _T...) typedef _T _Alias

#define T_embed$(_T...) $supress_microsoft_anon_tag(_T)
#define __stmt__T_embed$(_T...) (_T)

#define T_decl$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_decl$(__VA_ARGS__)
#define __stmt__T_decl$(...) __step__T_decl$__emit(__step__T_decl$__parseT __VA_ARGS__)
#define __step__T_decl$__parseT(_T...) _T,
#define __step__T_decl$__emit(...) ____T_decl$(__VA_ARGS__)
#define ____T_decl$(_T, _List_Parenthesized...) \
    __T__foreach(____T_decl$__each, _T, __step__T_decl$__unwrap _List_Parenthesized)
#define __step__T_decl$__unwrap(_List...) _List
#define ____T_decl$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_decl_, __VA_ARGS__, $)(_T); \
)

#define T_impl$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_impl$(__VA_ARGS__)
#define __stmt__T_impl$(...) __step__T_impl$__emit(__step__T_impl$__parseT __VA_ARGS__)
#define __step__T_impl$__parseT(_T...) _T,
#define __step__T_impl$__emit(...) ____T_impl$(__VA_ARGS__)
#define ____T_impl$(_T, _List_Parenthesized...) \
    __T__foreach(____T_impl$__each, _T, __step__T_impl$__unwrap _List_Parenthesized)
#define __step__T_impl$__unwrap(_List...) _List
#define ____T_impl$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_impl_, __VA_ARGS__, $)(_T); \
)

#define T_use$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_use$(__VA_ARGS__)
#define __stmt__T_use$(...) __step__T_use$__emit(__step__T_use$__parseT __VA_ARGS__)
#define __step__T_use$__parseT(_T...) (_T),
#define __step__T_use$__emit(...) ____T_use$(__VA_ARGS__)
#define ____T_use$(_T, _List_Parenthesized...) \
    __T__foreach(____T_use$__each, _T, __step__T_use$__unwrap _List_Parenthesized)
#define __step__T_use$__unwrap(_List...) _List
#define ____T_use$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_cat3(T_use_, __VA_ARGS__, $) _T; \
)

/* --- internal foreach --- */

#define __T__foreach(_Exec, _Iter, ...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * __T__foreach(exec, iter, x, y, z): \
     * => exec(iter, x) exec(iter, y) exec(iter, z) \
     */ \
    __T__exec_foreach(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach(_Exec, _Iter, ...) \
    pp_overload(__T__exec_foreach, __VA_ARGS__)(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_0(...)
#define __T__exec_foreach_1(_Exec, _Iter, _Arg) _Exec(_Iter, _Arg) __T__exec_foreach_0()
#define __T__exec_foreach_2(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_1(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_3(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_2(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_4(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_3(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_5(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_4(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_6(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_5(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_7(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_6(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_8(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_7(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_9(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_8(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_10(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_9(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_11(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_10(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_12(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_11(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_13(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_12(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_14(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_13(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_15(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_14(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_16(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_15(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_17(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_16(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_18(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_17(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_19(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_18(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_20(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_19(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_21(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_20(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_22(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_21(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_23(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_22(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_24(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_23(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_25(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_24(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_26(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_25(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_27(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_26(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_28(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_27(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_29(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_28(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_30(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_29(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_31(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_30(_Exec, _Iter, __VA_ARGS__)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_raw__included */
