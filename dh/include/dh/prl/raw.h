#ifndef prl_tpl_raw__included
#define prl_tpl_raw__included 1
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

union O$Void {
    struct {
        var_(is_some, bool);
        union {
            var_(none, Void);
            var_(some, Void);
        } payload;
    };
    var_(as_raw $like_ref, O$raw);
};

typedef struct E_CtxAny {
    var_(inner, const Opaq*);
} E_CtxAny;
typedef struct EAny {
    var_(ctx, E_CtxAny);
} EAny;

typedef union E_Payload$raw E_Payload$raw;
union E_Payload$raw {
    EAny err;
    u8 ok $flexible;
};

typedef struct E$raw E$raw;
struct E$raw {
    bool is_ok;
    union {
        EAny err;
        u8 ok $flexible;
        E_Payload$raw raw[1];
    } payload[1];
};

union E$Void {
    struct {
        var_(is_ok, bool);
        union {
            var_(err, EAny);
            var_(ok, Void);
        } payload;
    };
    var_(as_raw $like_ref, E$raw);
};

typedef struct Tup$raw {
    u8 inner $flexible;
} Tup$raw;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_raw__included */
