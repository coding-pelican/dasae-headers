/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-05 (date of creation)
 * @updated 2025-03-05 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef SLI_INCLUDED
#define SLI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

#define Sli_const$(T) TYPE_UNNAMED__Sli_const$(T)
#define Sli$(T)       TYPE_UNNAMED__Sli$(T)

#define use_Sli$(T)                               GEN__use_Sli$(T)
#define decl_Sli$(T)                              GEN__decl_Sli$(T)
#define impl_Sli$(T)                              GEN__impl_Sli$(T)
#define Sli_asNamed$(T_NamedSli, var_unnamed_sli) OP__Sli_asNamed$(T_NamedSli, var_unnamed_sli)

#define SliZ_const$(T) TYPE_UNNAMED__SliZ_const$(T)
#define SliZ$(T)       TYPE_UNNAMED__SliZ$(T)

#define use_SliZ$(T)                                  GEN__use_SliZ$(T)
#define decl_SliZ$(T)                                 GEN__decl_SliZ$(T)
#define impl_SliZ$(T)                                 GEN__impl_SliZ$(T)
#define SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z) OP__SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z)

/*========== Macros and Implementations =====================================*/

#define TYPE_UNNAMED__Sli_const$(T) \
    struct {                        \
        rawptr_const$(T) ptr;       \
        usize len;                  \
    }
#define TYPE_UNNAMED__Sli$(T)   \
    union {                     \
        Sli_const$(T) as_const; \
        struct {                \
            rawptr$(T) ptr;     \
            usize len;          \
        };                      \
    }

#define GEN__use_Sli$(T) \
    decl_Sli$(T);        \
    impl_Sli$(T)
#define GEN__decl_Sli$(T)                                             \
    typedef struct pp_join($, Sli_const, T) pp_join($, Sli_const, T); \
    typedef union pp_join($, Sli, T) pp_join($, Sli, T)
#define GEN__impl_Sli$(T)                  \
    struct pp_join($, Sli_const, T) {      \
        rawptr_const$(T) ptr;              \
        usize len;                         \
    };                                     \
    union pp_join($, Sli, T) {             \
        pp_join($, Sli_const, T) as_const; \
        struct {                           \
            rawptr$(T) ptr;                \
            usize len;                     \
        };                                 \
    }

#define TYPE_UNNAMED__SliZ_const$(T) \
    struct {                         \
        rawptr_const$(T) ptr;        \
    }
#define TYPE_UNNAMED__SliZ$(T)   \
    union {                      \
        SliZ_const$(T) as_const; \
        struct {                 \
            rawptr$(T) ptr;      \
        };                       \
    }

#define GEN__use_SliZ$(T) \
    decl_SliZ$(T);        \
    impl_SliZ$(T)
#define GEN__decl_SliZ$(T)                                              \
    typedef struct pp_join($, SliZ_const, T) pp_join($, SliZ_const, T); \
    typedef union pp_join($, SliZ, T) pp_join($, SliZ, T)
#define GEN__impl_SliZ$(T)                  \
    struct pp_join($, SliZ_const, T) {      \
        rawptr_const$(T) ptr;               \
    };                                      \
    union pp_join($, SliZ, T) {             \
        pp_join($, SliZ_const, T) as_const; \
        struct {                            \
            rawptr$(T) ptr;                 \
        };                                  \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */
