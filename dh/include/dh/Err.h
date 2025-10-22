/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Err.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Err
 *
 * @brief   Error utilities
 * @details Provides error utilities for the project.
 */

#ifndef ERR_INCLUDED
#define ERR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "fn.h"
#include "sli.h"

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
    fn_(((*domainToCStr)(ErrCode ctx))(const char*));
    fn_(((*codeToCStr)(ErrCode ctx))(const char*));
};
struct Err {
    ErrCode      ctx;
    const ErrVT* vt;
};
decl_Sli$(u8);
static $inline fn_((Err_domainToCStr(Err self))(const char*));
extern fn_((Err_domainToStr(Err self))(Sli_const$u8));
static $inline fn_((Err_codeToCStr(Err self))(const char*));
extern fn_((Err_codeToStr(Err self))(Sli_const$u8));
extern fn_((Err_print(Err self))(void));

static $inline fn_((Err_Unknown(void))(Err));
static $inline fn_((Err_Unexpected(void))(Err));
static $inline fn_((Err_Unspecified(void))(Err));
static $inline fn_((Err_Unsupported(void))(Err));
static $inline fn_((Err_NotImplemented(void))(Err));
static $inline fn_((Err_InvalidArgument(void))(Err));
static $inline fn_((Err_None(void))(Err));

#define config_ErrSet(Name, members...) \
    /* Implement error interface */ \
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

static $inline fn_((Err_domainToCStr(Err self))(const char*)) { return self.vt->domainToCStr(self.ctx); }
static $inline fn_((Err_codeToCStr(Err self))(const char*)) { return self.vt->codeToCStr(self.ctx); }

static $inline fn_((GeneralErr_domainToCStr(ErrCode ctx))(const char*)) {
    let_ignore = ctx;
    return "GeneralErr";
}
static $inline fn_((GeneralErr_codeToCStr(ErrCode ctx))(const char*)) {
    let code = as$((ErrCode)(ctx));
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
static $inline fn_((GeneralErr_err(ErrCode self))(Err)) {
    static const ErrVT vt[1] = { {
        .domainToCStr = GeneralErr_domainToCStr,
        .codeToCStr   = GeneralErr_codeToCStr,
    } };
    return (Err){
        .ctx = self,
        .vt  = vt,
    };
}

static $inline fn_((Err_Unknown(void))(Err)) { return GeneralErr_err(ErrCode_Unknown); }
static $inline fn_((Err_Unexpected(void))(Err)) { return GeneralErr_err(ErrCode_Unexpected); }
static $inline fn_((Err_Unspecified(void))(Err)) { return GeneralErr_err(ErrCode_Unspecified); }
static $inline fn_((Err_Unsupported(void))(Err)) { return GeneralErr_err(ErrCode_Unsupported); }
static $inline fn_((Err_NotImplemented(void))(Err)) { return GeneralErr_err(ErrCode_NotImplemented); }
static $inline fn_((Err_InvalidArgument(void))(Err)) { return GeneralErr_err(ErrCode_InvalidArgument); }
static $inline fn_((Err_None(void))(Err)) { return GeneralErr_err(ErrCode_None); }

#define GEN__config_ErrSet(Name, ...) \
    typedef enum pp_cat(Name, Code) { \
        GEN__config_ErrSet__ENUM__Code__members( \
            Name, \
            pp_foreach ( \
                GEN__config_ErrSet__ENUM__Code__member, \
                Name, \
                __VA_ARGS__ \
            ) \
        ) \
    } pp_cat(Name, Code); \
    typedef Err    Name; \
    static $inline fn_((pp_join(_, Name, domainToCStr)(ErrCode ctx))(const char*)) { \
        $unused(ctx); \
        return #Name; \
    } \
    static $inline fn_((pp_join(_, Name, codeToCStr)(ErrCode ctx))(const char*)) { \
        let code = as$((pp_cat(Name, Code))(ctx)); \
        switch (code) { \
            GEN__config_ErrSet__FN__codeToCStr__cases( \
                Name, \
                pp_foreach ( \
                    GEN__config_ErrSet__FN__codeToCStr__case, \
                    Name, \
                    __VA_ARGS__ \
                ) \
            ) \
        } \
    } \
    static $inline fn_((pp_join(_, Name, err)(pp_cat(Name, Code) self))(Err)) { \
        static const ErrVT vt[1] = { { \
            .domainToCStr = pp_join(_, Name, domainToCStr), \
            .codeToCStr   = pp_join(_, Name, codeToCStr), \
        } }; \
        return (Name){ \
            .ctx = self, \
            .vt  = vt, \
        }; \
    } \
    GEN__config_ErrSet__FN__ctorTemplates( \
        Name, \
        pp_foreach ( \
            GEN__config_ErrSet__FN__ctorTemplate, \
            Name, \
            __VA_ARGS__ \
        ) \
    ); \
    typedef struct pp_join($, Name, Void) { \
        union { \
            Name err; \
            Void ok; \
        } data; \
        bool is_err; \
    } pp_join($, Name, Void); \
    typedef pp_join($, Name, Void) pp_join($, Name, void)


// Helper macro to generate error type enum values
#define GEN__config_ErrSet__ENUM__Code__members(Name, ...) \
    pp_cat(Name, Code_None) = 0, __VA_ARGS__

// Helper macro to generate case statements for message function
#define GEN__config_ErrSet__FN__codeToCStr__cases(Name, ...) \
    case pp_cat(Name, Code_None): \
        return "None"; \
        __VA_ARGS__ \
    default: \
        claim_unreachable_fmt("Unknown error code (code: %d)", code);

#define GEN__config_ErrSet__FN__ctorTemplates(Name, ...) \
    static $inline $maybe_unused fn_((pp_cat(Name, _None)(void))(Err)) { \
        return pp_join(_, Name, err)(pp_cat(Name, Code_None)); \
    } \
    __VA_ARGS__

// Helper macros for generating individual cases
#define GEN__config_ErrSet__ENUM__Code__member(Name, Value) \
    pp_cat3(Name, Code_, Value),

#define GEN__config_ErrSet__FN__codeToCStr__case(Name, Value) \
    case pp_cat3(Name, Code_, Value): \
        return #Value;

#define GEN__config_ErrSet__FN__ctorTemplate(Name, Value) \
    static $inline $maybe_unused fn_((pp_join(_, Name, Value)(void))(Err)) { \
        return pp_join(_, Name, err)(pp_cat3(Name, Code_, Value)); \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_INCLUDED */
