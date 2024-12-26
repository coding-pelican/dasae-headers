/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Res.h
 * @brief   Result type handling with direct value storage
 */

#ifndef RES_INCLUDED
#define RES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Ptr.h"

/*========== Macros and Definitions =========================================*/

/* Result tag type */
typedef enum ResTag {
    ResTag_err = 0,
    ResTag_ok
} ResTag;

/* Core result type */
typedef struct Res {
    ResTag tag;
    union {
        anyptr value;
        anyptr error;
    };
} Res;

/* Core result default error type */
typedef struct ResErr {
    ResTag tag;
    union {
        anyptr value;
        Err    error;
    };
} ResErr;

/* Core result operations */
force_inline bool   Res_isOk(const Res* self);
force_inline anyptr Res_value(const Res* self);
force_inline anyptr Res_error(const Res* self);

/* Core result default error operations */
force_inline bool   ResErr_isOk(const ResErr* self);
force_inline anyptr ResErr_value(const ResErr* self);
force_inline Err    ResErr_error(const ResErr* self);

/*========== Implementations ================================================*/

force_inline bool Res_isOk(const Res* self) {
    debug_assert_nonnull(self);
    return self->tag == ResTag_ok;
}

force_inline anyptr Res_value(const Res* self) {
    debug_assert_nonnull(self);
    debug_assert_fmt(self->tag == ResTag_ok, "Attempted to unwrap error result");
    return (anyptr)self->value;
}

force_inline anyptr Res_error(const Res* self) {
    debug_assert_nonnull(self);
    debug_assert_fmt(self->tag == ResTag_err, "Attempted to unwrap ok result as error");
    return self->error;
}

force_inline bool ResErr_isOk(const ResErr* self) {
    debug_assert_nonnull(self);
    return self->tag == ResTag_ok;
}

force_inline anyptr ResErr_value(const ResErr* self) {
    debug_assert_nonnull(self);
    debug_assert_fmt(self->tag == ResTag_ok, "Attempted to unwrap error result");
    return self->value;
}

force_inline Err ResErr_error(const ResErr* self) {
    debug_assert_nonnull(self);
    debug_assert_fmt(self->tag == ResTag_err, "Attempted to unwrap ok result as error");
    return self->error;
}

// #define IMPL_using_Res(T, E, AliasRes)                                                      \
//     /* Result type with reference wrapper */                                                \
//     typedef struct {                                                                        \
//         union {                                                                             \
//             Res base;                                                                       \
//             struct {                                                                        \
//                 ResTag tag;                                                                 \
//                 union {                                                                     \
//                     T value;                                                                \
//                     E error;                                                                \
//                 };                                                                          \
//             };                                                                              \
//         };                                                                                  \
//     } AliasRes; /* NOLINT */                                                                \
//     /* Result interface */                                                                  \
//     force_inline AliasRes pp_join(_, AliasRes, ok)(T value) {                               \
//         return (AliasRes){                                                                  \
//             .tag   = ResTag_ok,                                                             \
//             .value = value                                                                  \
//         };                                                                                  \
//     }                                                                                       \
//     force_inline AliasRes pp_join(_, AliasRes, err)(E error) {                              \
//         return (AliasRes){                                                                  \
//             .tag   = ResTag_err,                                                            \
//             .error = error                                                                  \
//         };                                                                                  \
//     }                                                                                       \
//     force_inline bool pp_join(_, AliasRes, isOk)(AliasRes self) {                           \
//         return self.tag == ResTag_ok;                                                       \
//     }                                                                                       \
//     force_inline T pp_join(_, AliasRes, unwrap)(AliasRes self) {                            \
//         debug_assert_fmt(self.tag == ResTag_ok, "Attempted to unwrap error result");        \
//         return self.value;                                                                  \
//     }                                                                                       \
//     force_inline T pp_join(_, AliasRes, unwrapOr)(AliasRes self, T default_value) {         \
//         return self.tag == ResTag_ok ? self.value : default_value;                          \
//     }                                                                                       \
//     force_inline E pp_join(_, AliasRes, unwrapErr)(AliasRes self) {                         \
//         debug_assert_fmt(self.tag == ResTag_err, "Attempted to unwrap ok result as error"); \
//         return self.error;                                                                  \
//     }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RES_INCLUDED */
