/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Err.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2024-12-26 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  Err
 *
 * @brief   Header for Error type
 * @details Common Error type for error presentation
 */

#ifndef ERR_INCLUDED
#define ERR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

typedef int          ErrType;
typedef struct ErrVT ErrVT;
typedef struct Err   Err;
ErrType              Err_type(Err self);             // Get error type
const char*          Err_message(Err self);          // Get error message string
bool                 Err_is(Err self, ErrType type); // Check if error is of specific type

enum ErrType {
    ErrType_NotImplemented = -2,
    ErrType_Unknown        = -1,
    ErrType_None           = 0
    // ErrType_OutOfMemory,     // Alloc
    // ErrType_InvalidArgument, // Parse
    // ErrType_NotFound,        // IO
    // ErrType_AccessDenied,    // IO
};
struct ErrVT {
    ErrType (*type)(ErrType ctx); // Could be an enum or other identifier
    const char* (*message)(ErrType ctx);
};
struct Err {
    ErrType      ctx;
    const ErrVT* vt;
};
force_inline ErrType     Err_type(Err self) { return self.vt->type(self.ctx); }
force_inline const char* Err_message(Err self) { return self.vt->message(self.ctx); }
force_inline bool        Err_is(Err self, ErrType type) { return self.vt->type(self.ctx) == type; }

#define impl_Err(Name, members...)  \
    /* Implement error interface */ \
    IMPL_impl_Err(Name, members)

/* impl_Err(
    io_FileErr,
    NotFound,
    OpenFailed,
    ReadFailed,
    WriteFailed
); */

/* impl_Err(
    io_ParseErr,
    InvalidArgument,
    UnexpectedEOF,
    UnexpectedChar,
    UnexpectedToken,
    UnexpectedTokenType,
    UnexpectedTokenValue
); */

/* impl_Err(
    math_Err,
    DivisionByZero,
    Overflow,
    Underflow
); */

/* impl_Err(
    mem_AllocErr,
    OutOfMemory
); */

/*========== Implementations ================================================*/

#define IMPL_impl_Err(Name, ...)                                                                             \
    typedef enum pp_cat(Name, Type) {                                                                        \
        IMPL_impl_Err_genEnumValues(Name, pp_foreach(IMPL_impl_Err_genEnumValue, Name, __VA_ARGS__))         \
    } pp_cat(Name, Type);                                                                                    \
    typedef pp_cat(Name, Type) Name;                                                                         \
    static ErrType pp_join(_, Name, type)(ErrType ctx) {                                                     \
        let self = (pp_cat(Name, Type))ctx;                                                                  \
        switch (self) {                                                                                      \
            IMPL_impl_Err_genTypeCases(Name, pp_foreach(IMPL_impl_Err_genTypeCase, Name, __VA_ARGS__))       \
        }                                                                                                    \
    }                                                                                                        \
    static const char* pp_join(_, Name, message)(ErrType ctx) {                                              \
        let self = (pp_cat(Name, Type))ctx;                                                                  \
        switch (self) {                                                                                      \
            IMPL_impl_Err_genMessageCases(Name, pp_foreach(IMPL_impl_Err_genMessageCase, Name, __VA_ARGS__)) \
        }                                                                                                    \
    }                                                                                                        \
    static Err pp_join(_, Name, err)(Name self) {                                                            \
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ERR_INCLUDED */
