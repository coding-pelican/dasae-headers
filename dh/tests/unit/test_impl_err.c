#include "dh/claim/unreachable.h"
#include "dh/core.h"

/* Error definition macro */

typedef int          ErrType;
typedef struct ErrVT ErrVT;
typedef struct Err   Err;
ErrType              Err_type(Err self);             // Get error type
const char*          Err_message(Err self);          // Get error message string
bool                 Err_is(Err self, ErrType type); // Check if error is of specific type

enum ErrType {
    ErrType_NotImplemented = -2,
    ErrType_Unknown        = -1,
    ErrType_None           = 0,
    // ErrType_OutOfMemory,     // Alloc
    // ErrType_InvalidArgument, // Parse
    // ErrType_NotFound,        // IO
    // ErrType_AccessDenied,    // IO
    // Add more standard error types as needed
};
struct ErrVT {
    ErrType (*type)(ErrType ctx); // Could be an enum or other identifier
    const char* (*message)(ErrType ctx);
};
struct Err {
    ErrType      ctx;
    const ErrVT* vt;
};
ErrType     Err_type(Err self) { return self.vt->type(self.ctx); }
const char* Err_message(Err self) { return self.vt->message(self.ctx); }
bool        Err_is(Err self, ErrType type) { return self.vt->type(self.ctx) == type; }

// Main macro to implement error type
#define impl_Err(Name, members...) \
    IMPL_impl_Err(Name, members)

#define IMPL_impl_Err(Name, ...)                                                                             \
    typedef enum pp_cat(Name, Type) {                                                                        \
        IMPL_impl_Err_genEnumValues(Name, pp_foreach(IMPL_impl_Err_genEnumValue, Name, __VA_ARGS__))         \
    } pp_cat(Name, Type);                                                                                    \
    typedef pp_cat(Name, Type) Name;                                                                         \
    force_inline ErrType pp_join(_, Name, type)(ErrType ctx) {                                               \
        let self = (pp_cat(Name, Type))ctx;                                                                  \
        switch (self) {                                                                                      \
            IMPL_impl_Err_genTypeCases(Name, pp_foreach(IMPL_impl_Err_genTypeCase, Name, __VA_ARGS__))       \
        }                                                                                                    \
    }                                                                                                        \
    force_inline const char* pp_join(_, Name, message)(ErrType ctx) {                                        \
        let self = (pp_cat(Name, Type))ctx;                                                                  \
        switch (self) {                                                                                      \
            IMPL_impl_Err_genMessageCases(Name, pp_foreach(IMPL_impl_Err_genMessageCase, Name, __VA_ARGS__)) \
        }                                                                                                    \
    }                                                                                                        \
    force_inline Err pp_join(_, Name, err)(Name self) {                                                      \
        static const ErrVT vt[1] = { {                                                                       \
            .type    = pp_join(_, Name, type),                                                               \
            .message = pp_join(_, Name, message),                                                            \
        } };                                                                                                 \
        return (Err){                                                                                        \
            .ctx = self,                                                                                     \
            .vt  = vt,                                                                                       \
        };                                                                                                   \
    }

// Helper macro to generate error type enum values
#define IMPL_impl_Err_genEnumValues(Name, ...) \
    pp_cat(Name, Type_None) = 0,               \
                 __VA_ARGS__
// Helper macro to generate case statements for type function
#define IMPL_impl_Err_genTypeCases(Name, ...) \
    case pp_cat(Name, Type_None):             \
        __VA_ARGS__                           \
        return self;                          \
    default:                                  \
        return claim_unreachable_fmt("Unknown error type (err: %d)", self), ErrType_Unknown;
// Helper macro to generate case statements for message function
#define IMPL_impl_Err_genMessageCases(Name, ...) \
    case pp_cat(Name, Type_None):                \
        return "None";                           \
        __VA_ARGS__                              \
    default:                                     \
        return claim_unreachable_fmt("Unknown error type (err: %d)", self), "UnknownError";
// Helper macros for generating individual cases
#define IMPL_impl_Err_genEnumValue(Name, Value) \
    pp_cat3(Name, Type_, Value),
#define IMPL_impl_Err_genTypeCase(Name, Value) \
    case pp_cat3(Name, Type_, Value):
#define IMPL_impl_Err_genMessageCase(Name, Value) \
    case pp_cat3(Name, Type_, Value):             \
        return #Value;

// Usage example:
impl_Err(MathErr, DivisionByZero, Overflow, Underflow);

// // And to use it:
// Error$(i32) divide(i32 lhs, i32 rhs) {
//     if (rhs == 0) {
//         return err(MathErr_err(MathErrType_DivisionByZero));
//     }
//     return ok(lhs / rhs);
// }

impl_Err(AllocErr, OutOfMemory);

/* impl_Err(MathErr, DivisionByZero);
typedef enum MathErrType {
    MathErrType_None = 0,
    MathErrType_DivisionByZero,
} MathErrType;
typedef MathErrType  MathErr;
force_inline ErrType MathErr_type(ErrType ctx) {
    let self = (MathErrType)ctx;
    switch (self) {
    case MathErrType_None:
    case MathErrType_DivisionByZero:
        return self;
    default:
        return claim_unreachable_fmt("Unknown error type (err: %d)", self), ErrType_Unknown;
    }
}
force_inline const char* MathErr_message(ErrType ctx) {
    let self = (MathErrType)ctx;
    switch (self) {
    case MathErrType_None:
        return "None";
    case MathErrType_DivisionByZero:
        return "DivisionByZero";
    default:
        claim_unreachable_fmt("Unknown error type (err: %d)", self), "UnknownError";
    }
}
force_inline Err MathErr_err(MathErr self) {
    static const ErrVT vt[1] = { {
        .type    = MathErr_type,
        .message = MathErr_message,
    } };
    return (Err){
        .ctx = self,
        .vt  = vt,
    };
} */

#define using_ErrRes(T, TErrRes) \
    typedef_struct(TErrRes) {    \
        bool is_err;             \
        union {                  \
            Err err;             \
            T   ok;              \
        };                       \
    }

using_ErrRes(Void, ErrVoid);

/* Error propagation macros */

#define ErrRes_reserveReturn(TErrRes) \
    reserveReturn(TErrRes)

#define ErrRes_err(val_err...)             \
    ({                                     \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = true,            \
                .err    = val_err,         \
            }                              \
        );                                 \
        getReservedReturn();               \
    })

#define ErrRes_ok(val_ok...)               \
    ({                                     \
        setReservedReturn(                 \
            (TypeOf(getReservedReturn())){ \
                .is_err = false,           \
                .ok     = val_ok,          \
            }                              \
        );                                 \
        getReservedReturn();               \
    })

#define ErrVoid_reserveReturn() \
    reserveReturn(ErrVoid)

#define ErrVoid_err(val_err...) \
    ErrRes_err(val_err)

#define ErrVoid_ok() \
    ErrRes_ok({})

/* Error handling macros */

#define try(_Expr)                          \
    ({                                      \
        let _result = (_Expr);              \
        if (_result.is_err) {               \
            return ErrRes_err(_result.err); \
        }                                   \
        _result.ok;                         \
    })

#define catch(_Expr, var_err, _Expr_Handling...) \
    ({                                           \
        let _result = (_Expr);                   \
        if (_result.is_err) {                    \
            let var_err = _result.err;           \
            _Expr_Handling                       \
        }                                        \
        _result.ok;                              \
    })

/* Example usage */
extern ErrVoid dh_main(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {
    const ErrVoid result = dh_main(argc, argv);
    if (!result.is_err) { return 0; }
    ignore fprintf(
        stderr,
        "Program failed: %s (type: %d)\n",
        Err_message(result.err),
        Err_type(result.err)
    );
    return 1;
}

using_ErrRes(i32, Err$i32);
Err$i32 divide(i32 lhs, i32 rhs) {
    ErrRes_reserveReturn(Err$i32);
    if (rhs == 0) {
        return ErrRes_err(MathErr_err(MathErrType_DivisionByZero));
    }
    return ErrRes_ok(lhs / rhs);
}

ErrVoid dh_main(int argc, const char* argv[]) {
    ErrVoid_reserveReturn();
    unused(argc);
    unused(argv);

    try(divide(1, 0));
    catch (divide(1, 0), err, {
        printf("1 / 0 = %s\n", Err_message(err));
    });

    return ErrVoid_ok();
}
