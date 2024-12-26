/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Opt.h
 * @brief   Optional type handling with direct value storage
 */

#ifndef OPT_INCLUDED
#define OPT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Ptr.h"

/*========== Macros and Definitions =========================================*/

/* Core optional type */
typedef struct Opt {
    bool   has_value;
    anyptr value;
} Opt;

/* Core optional operations */
force_inline bool   Opt_isSome(const Opt* self);
force_inline anyptr Opt_value(const Opt* self);

/*========== Implementations ================================================*/

force_inline bool Opt_isSome(const Opt* self) {
    debug_assert_nonnull(self);
    return self->has_value;
}

force_inline anyptr Opt_value(const Opt* self) {
    debug_assert_nonnull(self);
    debug_assert_fmt(self->has_value, "Attempted to unwrap none optional");
    return self->value;
}

// #define IMPL_using_Opt(T, AliasOpt)                                                 \
//     /* Optional type with reference wrapper */                                      \
//     typedef union {                                                                 \
//         Opt base;                                                                   \
//         struct {                                                                    \
//             bool has_value;                                                         \
//             T    value;                                                             \
//         };                                                                          \
//     } AliasOpt; /* NOLINT */                                                        \
//     /* Optional interface */                                                        \
//     force_inline AliasOpt pp_join(_, AliasOpt, some)(T value) {                     \
//         return (AliasOpt){                                                          \
//             .has_value = true,                                                      \
//             .value     = value                                                      \
//         };                                                                          \
//     }                                                                               \
//     force_inline AliasOpt pp_join(_, AliasOpt, none)(void) {                        \
//         return (AliasOpt){ .has_value = false };                                    \
//     }                                                                               \
//     force_inline bool pp_join(_, AliasOpt, isSome)(AliasOpt self) {                 \
//         return self.has_value;                                                      \
//     }                                                                               \
//     force_inline T pp_join(_, AliasOpt, unwrap)(AliasOpt self) {                    \
//         debug_assert_fmt(self.has_value, "Attempted to unwrap none optional");      \
//         return self.value;                                                          \
//     }                                                                               \
//     force_inline T pp_join(_, AliasOpt, unwrapOr)(AliasOpt self, T default_value) { \
//         return self.has_value ? self.value : default_value;                         \
//     }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* OPT_INCLUDED */
