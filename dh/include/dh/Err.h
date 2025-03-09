/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Err.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Err
 *
 * @brief
 * @details
 */

#ifndef ERR_INCLUDED
#define ERR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

typedef struct Err   Err;
typedef int          ErrCode;
typedef struct ErrVT ErrVT;

enum ErrCode {
    ErrCode_Unknown         = -6,
    ErrCode_Unexpected      = -5,
    ErrCode_Unspecified     = -4,
    ErrCode_Unsupported     = -3,
    ErrCode_NotImplemented  = -2,
    ErrCode_InvalidArgument = -1,
    ErrCode_None            = 0
};
struct ErrVT {
    const char* (*domainToCStr)(ErrCode ctx);
    const char* (*codeToCStr)(ErrCode ctx);
};
struct Err {
    ErrCode      ctx;
    const ErrVT* vt;
};
force_inline const char* Err_domainToCStr(Err self);
force_inline const char* Err_codeToCStr(Err self);
extern void              Err_print(Err self);

force_inline Err Err_Unknown(void);
force_inline Err Err_Unexpected(void);
force_inline Err Err_Unspecified(void);
force_inline Err Err_Unsupported(void);
force_inline Err Err_NotImplemented(void);
force_inline Err Err_InvalidArgument(void);
force_inline Err Err_None(void);

#define config_ErrSet(Name, members...) \
    /* Implement error interface */     \
    GEN__config_ErrSet(Name, members)

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
config_ErrSet(io_FileErr,
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
);

config_ErrSet(io_ParseErr,
    InvalidArgument,
    UnexpectedEOF,
    UnexpectedChar,
    UnexpectedToken,
    UnexpectedTokenType,
    UnexpectedTokenValue
);

config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

config_ErrSet(mem_AllocErr,
    OutOfMemory
);
#endif /* EXAMPLE_USAGE */

/*========== Implementations ================================================*/

force_inline const char* Err_domainToCStr(Err self) { return self.vt->domainToCStr(self.ctx); }
force_inline const char* Err_codeToCStr(Err self) { return self.vt->codeToCStr(self.ctx); }

static_inline const char* GeneralErr_domainToCStr(ErrCode ctx) {
    unused(ctx);
    return "GeneralErr";
}
static_inline const char* GeneralErr_codeToCStr(ErrCode ctx) {
    let code = as$(ErrCode, ctx);
    switch (code) {
    case ErrCode_Unknown:
        return "Unknown";
    case ErrCode_Unexpected:
        return "Unexpected";
    case ErrCode_Unspecified:
        return "Unspecified";
    case ErrCode_Unsupported:
        return "Unsupported";
    case ErrCode_NotImplemented:
        return "NotImplemented";
    case ErrCode_InvalidArgument:
        return "InvalidArgument";
    case ErrCode_None:
        return "None";
    default:
        claim_unreachable_fmt("Unknown error code (code: %d)", code);
    }
}
static_inline Err GeneralErr_err(ErrCode self) {
    static const ErrVT vt[1] = { {
        .domainToCStr = GeneralErr_domainToCStr,
        .codeToCStr   = GeneralErr_codeToCStr,
    } };
    return (Err){
        .ctx = self,
        .vt  = vt,
    };
}

force_inline Err Err_Unknown(void) { return GeneralErr_err(ErrCode_Unknown); }
force_inline Err Err_Unexpected(void) { return GeneralErr_err(ErrCode_Unexpected); }
force_inline Err Err_Unspecified(void) { return GeneralErr_err(ErrCode_Unspecified); }
force_inline Err Err_Unsupported(void) { return GeneralErr_err(ErrCode_Unsupported); }
force_inline Err Err_NotImplemented(void) { return GeneralErr_err(ErrCode_NotImplemented); }
force_inline Err Err_InvalidArgument(void) { return GeneralErr_err(ErrCode_InvalidArgument); }
force_inline Err Err_None(void) { return GeneralErr_err(ErrCode_None); }

#define GEN__config_ErrSet(Name, ...)                                                                                                 \
    typedef enum pp_cat(Name, Code) {                                                                                                 \
        GEN__config_ErrSet__ENUM__Code__members(Name, pp_foreach (GEN__config_ErrSet__ENUM__Code__member, Name, __VA_ARGS__))         \
    } pp_cat(Name, Code);                                                                                                             \
    typedef Err               Name;                                                                                                   \
    static_inline const char* pp_join(_, Name, domainToCStr)(ErrCode ctx) {                                                           \
        unused(ctx);                                                                                                                  \
        return #Name;                                                                                                                 \
    }                                                                                                                                 \
    static_inline const char* pp_join(_, Name, codeToCStr)(ErrCode ctx) {                                                             \
        let code = as$(pp_cat(Name, Code), ctx);                                                                                      \
        switch (code) {                                                                                                               \
            GEN__config_ErrSet__FN__codeToCStr__cases(Name, pp_foreach (GEN__config_ErrSet__FN__codeToCStr__case, Name, __VA_ARGS__)) \
        }                                                                                                                             \
    }                                                                                                                                 \
    static_inline Name pp_join(_, Name, err)(pp_cat(Name, Code) self) {                                                               \
        static const ErrVT vt[1] = { {                                                                                                \
            .domainToCStr = pp_join(_, Name, domainToCStr),                                                                           \
            .codeToCStr   = pp_join(_, Name, codeToCStr),                                                                             \
        } };                                                                                                                          \
        return (Name){                                                                                                                \
            .ctx = self,                                                                                                              \
            .vt  = vt,                                                                                                                \
        };                                                                                                                            \
    }                                                                                                                                 \
    GEN__config_ErrSet__FN__ctorTemplates(Name, pp_foreach (GEN__config_ErrSet__FN__ctorTemplate, Name, __VA_ARGS__));                \
    typedef struct pp_join($, Name, Void) {                                                                                           \
        bool is_err;                                                                                                                  \
        union {                                                                                                                       \
            Name err;                                                                                                                 \
            Void ok;                                                                                                                  \
        };                                                                                                                            \
    } pp_join($, Name, Void);                                                                                                         \
    typedef pp_join($, Name, Void) pp_join($, Name, void)


// Helper macro to generate error type enum values
#define GEN__config_ErrSet__ENUM__Code__members(Name, ...) \
    pp_cat(Name, Code_None) = 0, __VA_ARGS__

// Helper macro to generate case statements for message function
#define GEN__config_ErrSet__FN__codeToCStr__cases(Name, ...) \
    case pp_cat(Name, Code_None):                            \
        return "None";                                       \
        __VA_ARGS__                                          \
    default:                                                 \
        claim_unreachable_fmt("Unknown error code (code: %d)", code);

#define GEN__config_ErrSet__FN__ctorTemplates(Name, ...)       \
    force_inline Name pp_cat(Name, _None)(void) {              \
        return pp_join(_, Name, err)(pp_cat(Name, Code_None)); \
    }                                                          \
    __VA_ARGS__

// Helper macros for generating individual cases
#define GEN__config_ErrSet__ENUM__Code__member(Name, Value) \
    pp_cat3(Name, Code_, Value),

#define GEN__config_ErrSet__FN__codeToCStr__case(Name, Value) \
    case pp_cat3(Name, Code_, Value):                         \
        return #Value;

#define GEN__config_ErrSet__FN__ctorTemplate(Name, Value)          \
    force_inline Name pp_join(_, Name, Value)(void) {              \
        return pp_join(_, Name, err)(pp_cat3(Name, Code_, Value)); \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_INCLUDED */
