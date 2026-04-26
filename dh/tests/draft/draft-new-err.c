#include <stdbool.h>
#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t usize;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t isize;
#define $attr(_Attrs...) _Attrs
#define $packed __attribute__((packed))
#define $inline inline
#define $inline_always inline __attribute__((always_inline))

typedef struct Void {
} Void;
#define $opaque \
    Void



/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    CompHash.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-27 (date of creation)
 * @updated 2026-03-07 (date of last update)
 * @ingroup dal-project/da-dsl/prelude
 * @prefix  (none)
 *
 * @brief   String hashing implementation using DJB hash algorithm
 * @details This implementation uses a variant of DJB hash algorithm
 *          with a multiplier of 65599 for efficient string hashing.
 *          It provides a compile-time hash value for null-terminated strings
 *          and a runtime hash value for any string.
 */
#if CHEAT_SHEET
/* Creating Hash Values */
CompHash hash1 = CompHash_from("hello"); // From raw C string
S_const$u8 str = Str_fromRaw("world");
CompHash hash2 = CompHash_from(str); // From S_const$u8

/* Calculating Hash Values */
u32 value1 = compHash("hello"); // From raw C string
u32 value2 = compHash(data, 5); // From byte array with length
u32 value3 = CompHash_val(hash1); // Extract from CompHash

/* Hash Value Based on String Length */
u32 hash_1char = CompHash__char1('a'); // Single character hash
u32 hash_2chars = CompHash__char2((const u8*)"ab"); // 2-character string hash
u32 hash_3chars = CompHash__char3((const u8*)"abc"); // 3-character string hash
u32 hash_4chars = CompHash__char4((const u8*)"abcd"); // 4-character string hash

/* API Reference List =======================================================*/

/// String hash value container structure
typedef struct CompHash {
    u32 val; ///< The 32-bit hash value
} CompHash;

/* Creating Hash Values */
/// Calculate hash value from a null-terminated string
fn_(compHash(const char* raw_str), u32);
/// Create a CompHash from a null-terminated string
fn_(CompHash_from(const char* raw_str), CompHash);

/* Calculating Hash Values */
/// Calculate hash value from a byte array with specified length
fn_(compHash(const u8* ptr, usize len), u32);
/// Create a CompHash from a S_const$u8
fn_(CompHash_from(S_const$u8 str), CompHash);
/// Extract the hash value from a CompHash structure
fn_(CompHash_val(CompHash self), u32);

/* Base Cases for Different String Lengths */
#define CompHash__char1(ch) // Hash single character
#define CompHash__char2(str) // Hash 2-character string
#define CompHash__char3(str) // Hash 3-character string
#define CompHash__char4(str) // Hash 4-character string
#define CompHash__recur(ptr, len) // Recursively hash string of any length

/* Usage Examples ===========================================================*/

// Creating and using hash values
CompHash hash = CompHash_from("hello");
u32 value = CompHash_val(hash); // Get the numeric hash value

// Directly calculating hash values
u32 direct_hash = compHash("hello");

// Using with byte arrays
const u8* data = (const u8*)"hello";
u32 byte_hash = compHash(data, 5);

// Using with S_const$u8 strings
S_const$u8 str = Str_fromRaw("hello");
CompHash str_hash = CompHash_from(str);
#endif /* CHEAT_SHEET */
#ifndef da_dsl_prelude_CompHash__included
#define da_dsl_prelude_CompHash__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "common.h"

/*========== Macros and Declarations ========================================*/

/// @brief String hash value container structure
/// @details Holds the calculated 32-bit hash value for a string
typedef struct CompHash {
    u32 val; ///< The 32-bit hash value
} CompHash;

/// @brief  Calculate hash value from a null-terminated string
/// @param  raw_str Null-terminated string to hash
/// @return 32-bit hash value
/// @example
///   u32 hash = compHash("hello");
///   // hash now contains the 32-bit hash value for "hello"
#define compHash(_raw_str...) \
    ____compHash(_raw_str)

/// @brief  Create a CompHash from a string
/// @param  str String to hash
/// @return CompHash structure containing the hash value
/// @example
///   CompHash hash = CompHash_from(u8_l("hello"));
///   // hash.value now contains the hash for "hello"
$attr($inline_always)
static CompHash CompHash_from(const u8* str, usize len);

/// @brief  Extract the hash value from a CompHash structure
/// @param  self CompHash structure
/// @return 32-bit hash value
/// @example
///   CompHash hash = CompHash_from(u8_l("hello"));
///   u32 value = CompHash_val(hash);
///   // value now contains the 32-bit hash value
$attr($inline_always)
static u32 CompHash_val(CompHash self);

/// @brief  Calculate hash value from a string
/// @param  str String to hash
/// @return 32-bit hash value
/// @example
///   u32 hash = CompHash_calc(u8_l("hello"));
///   // hash now contains the 32-bit hash value for "hello"
$attr($inline_always)
static u32 CompHash_calc(const u8* str, usize len);

/*========== Macros and Definitions =========================================*/

/*--- Internal Declarations ---*/


/*--- External Definitions */

#define ____compHash(_raw_str...) \
    /** Calculate hash value from a compile-time string literal */ \
    CompHash_calc(u8_l(_raw_str))

CompHash CompHash_from(const u8* str, usize len) {
    return (CompHash){ .val = len == 0 ? 0 : CompHash_calc(str, len) };
};

u32 CompHash_val(CompHash self) {
    return self.val;
};

u32 CompHash_calc(const u8* str, usize len) {
    u32 hash = 0;
    for (usize i = 0; i < len; ++i) {
        hash = 65599 * hash + str[i];
    }
    return hash ^ (hash >> 16);
};

/*--- Internal Definitions ---*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_prelude_CompHash__included */

static inline u32 genHash(const char* str, usize len) {
    u32 hash = 0;
    // #pragma unroll
    for (usize i = 0; i < len; ++i) {
        hash = 65599 * hash + str[i];
    }
    return hash ^ (hash >> 16);
}

// $attr($inline_always)
// static usize mem_idxZ$u8(u8 sentinel, const u8* ptr) {
//     usize idx = 0;
//     while (ptr[idx] != sentinel) ++idx;
//     return idx;
// }
// $attr($inline_always)
// static usize mem_lenZ0$u8(const u8* ptr) {
//     return mem_idxZ$u8('\0', ptr);
// }
typedef u32 E_HashId;
typedef i16 E_Code;
$attr($inline_always)
static E_HashId E_hasher(const char* str, usize len) {
    return CompHash_calc((const u8*)str, len);
};
/* General Error - Inner */
typedef enum $packed E_Tag$General_E {
    E_Tag$NotImplemented = -2,
    E_Tag$General_E_NotImplemented = E_Tag$NotImplemented,
    E_Tag$Unexpected = -1,
    E_Tag$General_E_Unexpected = E_Tag$Unexpected,
    E_Tag$Any = 0,
    E_Tag$General_E_Any = E_Tag$Any
} E_Tag$General_E;
typedef union E_Opaq$General_E {
    E_HashId hash_id;
} E_Opaq$General_E;
typedef E_Opaq$General_E E_Type$NotImplemented;
typedef E_Opaq$General_E E_Type$Unexpected;
typedef E_Opaq$General_E E_Type$Any;
typedef union E_Payload$General_E {
    // E_Opaq$General_E;
    E_Type$NotImplemented NotImplemented;
    E_Type$Unexpected Unexpected;
    E_Type$Any Any;
} E_Payload$General_E;
/* General Error - Outer */
typedef struct General_E {
    union {
        E_Tag$General_E tag;
        E_Code code;
    };
    union {
        E_Payload$General_E payload;
        E_HashId hash_id;
    };
} General_E;
$attr($inline_always)
static const char* E_strfy$General_E(General_E self) {
    switch (self.tag) {
    case E_Tag$NotImplemented: return "NotImplemented";
    case E_Tag$Unexpected: return "Unexpected";
    case E_Tag$Any: __builtin_unreachable();
    }
}
/* General Error - Occurrences */
typedef General_E General_E_NotImplemented;
$attr($inline_always)
static General_E_NotImplemented E_cause$NotImplemented(void) {
    return (General_E_NotImplemented){
        .tag = E_Tag$NotImplemented,
        .payload = {
            .NotImplemented = {
                .hash_id = E_hasher("NotImplemented", sizeof("NotImplemented") - 1),
            },
        },
    };
}
typedef General_E General_E_Unexpected;
$attr($inline_always)
static General_E_Unexpected E_cause$Unexpected(void) {
    return (General_E_Unexpected){
        .tag = E_Tag$Unexpected,
        .payload = {
            .Unexpected = {
                .hash_id = E_hasher("Unexpected", sizeof("Unexpected") - 1),
            },
        },
    };
}
typedef General_E General_E_Any;
$attr($inline_always)
static General_E_Any E_cause$Any(General_E* self) {
    return (General_E_Any){
        .tag = E_Tag$Any,
        .payload = {
            .Any = {
                .hash_id = self->hash_id,
            },
        },
    };
}
typedef struct O$General_E {
    bool is_some;
    union {
        General_E some;
        Void none;
    } payload;
} O$General_E;
$attr($inline_always)
static O$General_E E_resolve$General_E(General_E self) {
    const General_E errs[] = {
        E_cause$NotImplemented(),
        E_cause$Unexpected(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.hash_id != errs[i].hash_id) continue;
        return (O$General_E){
            .is_some = true,
            .payload = {
                .some = errs[i],
            }
        };
    }
    return (O$General_E){ .is_some = false };
}
$attr($inline_always)
static General_E E_handle$General_E(General_E self) {
    O$General_E resolved = E_resolve$General_E(self);
    if (resolved.is_some) return resolved.payload.some;
    return (General_E)E_cause$Any(&self);
}


/* Concurrent Error - Inner */
typedef enum $packed E_Tag$Conc_E {
    E_Tag$Conc_E_Any = E_Tag$Any,
    E_Tag$ConcUnavailable = 1,
    E_Tag$Conc_E_ConcUnavailable = E_Tag$ConcUnavailable
} E_Tag$Conc_E;
typedef union E_Opaq$Conc_E {
    E_HashId hash_id;
} E_Opaq$Conc_E;
typedef E_Opaq$Conc_E E_Type$ConcUnavailable;
typedef union E_Payload$Conc_E {
    // E_Opaq$Conc_E;
    E_Type$ConcUnavailable ConcUnavailable;
} E_Payload$Conc_E;
/* Concurrent Error - Outer */
typedef union Conc_E {
    struct {
        union {
            E_Tag$Conc_E tag;
            E_Code code;
        };
        union {
            E_Payload$Conc_E payload;
            E_HashId hash_id;
        };
    };
    General_E as_general[1];
    General_E General_E;
} Conc_E;
$attr($inline_always)
static const char* E_strfy$Conc_E(Conc_E self) {
    switch (self.tag) {
    case E_Tag$ConcUnavailable: return "ConcUnavailable";
    case E_Tag$Any: __builtin_unreachable();
    }
}
/* Concurrent Error - Occurrences */
typedef Conc_E Conc_E_ConcUnavailable;
$attr($inline_always)
static Conc_E_ConcUnavailable E_cause$ConcUnavailable(void) {
    return (Conc_E_ConcUnavailable){
        .tag = E_Tag$ConcUnavailable,
        .payload = {
            .ConcUnavailable = {
                .hash_id = E_hasher("ConcUnavailable", sizeof("ConcUnavailable") - 1),
            },
        },
    };
}
typedef struct O$Conc_E {
    bool is_some;
    union {
        Conc_E some;
        Void none;
    } payload;
} O$Conc_E;
$attr($inline_always)
static O$Conc_E E_resolve$Conc_E(Conc_E self) {
    const Conc_E errs[] = {
        E_cause$ConcUnavailable(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.hash_id != errs[i].hash_id) continue;
        return (O$Conc_E){
            .is_some = true,
            .payload = {
                .some = errs[i],
            }
        };
    }
    return (O$Conc_E){ .is_some = false };
}
$attr($inline_always)
static Conc_E E_handle$Conc_E(Conc_E self) {
    O$Conc_E resolved = E_resolve$Conc_E(self);
    if (resolved.is_some) return resolved.payload.some;
    return (Conc_E)E_cause$Any(self.as_general);
}


/* Cancelable Error - Inner */
typedef enum $packed E_Tag$Cancelable {
    E_Tag$Cancelable_Any = E_Tag$Any,
    E_Tag$Canceled = 1,
    E_Tag$Cancelable_Canceled = E_Tag$Canceled
} E_Tag$Cancelable;
typedef union E_Opaq$Cancelable {
    E_HashId hash_id;
} E_Opaq$Cancelable;
typedef E_Opaq$Cancelable E_Type$Canceled;
typedef union E_Payload$Cancelable {
    // E_Opaq$Cancelable;
    E_Type$Canceled Canceled;
} E_Payload$Cancelable;
/* Cancelable Error - Outer */
typedef union Cancelable {
    struct {
        union {
            E_Tag$Cancelable tag;
            E_Code code;
        };
        union {
            E_Payload$Cancelable payload;
            E_HashId hash_id;
        };
    };
    General_E as_general[1];
    General_E General_E;
} Cancelable;
$attr($inline_always)
static const char* E_strfy$Cancelable(Cancelable self) {
    switch (self.tag) {
    case E_Tag$Canceled: return "Canceled";
    case E_Tag$Any: __builtin_unreachable();
    }
}
/* Cancelable Error - Occurrences */
typedef Cancelable Cancelable_Canceled;
$attr($inline_always)
static Cancelable_Canceled E_cause$Canceled(void) {
    return (Cancelable_Canceled){
        .tag = E_Tag$Canceled,
        .payload = {
            .Canceled = {
                .hash_id = E_hasher("Canceled", sizeof("Canceled") - 1),
            },
        },
    };
}
typedef struct O$Cancelable {
    bool is_some;
    union {
        Cancelable some;
        Void none;
    } payload;
} O$Cancelable;
$attr($inline_always)
static O$Cancelable E_resolve$Cancelable(Cancelable self) {
    const Cancelable errs[] = {
        E_cause$Canceled(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.hash_id != errs[i].hash_id) continue;
        return (O$Cancelable){
            .is_some = true,
            .payload = {
                .some = errs[i],
            }
        };
    }
    return (O$Cancelable){ .is_some = false };
}
$attr($inline_always)
static Cancelable E_handle$Cancelable(Cancelable self) {
    O$Cancelable resolved = E_resolve$Cancelable(self);
    if (resolved.is_some) return resolved.payload.some;
    return (Cancelable){
        .code = E_Tag$Any,
        .hash_id = self.hash_id
    };
}


/* Timeout Error - Inner */
typedef enum $packed E_Tag$Timeout_E {
    E_Tag$Timeout_E_Any = E_Tag$Any,
    E_Tag$Timeout = 1,
    E_Tag$Timeout_E_Timeout = E_Tag$Timeout
} E_Tag$Timeout_E;
typedef union E_Opaq$Timeout_E {
    E_HashId hash_id;
} E_Opaq$Timeout_E;
typedef E_Opaq$Timeout_E E_Type$Timeout;
typedef union E_Payload$Timeout_E {
    // E_Opaq$Timeout_E;
    E_Type$Timeout Timeout;
} E_Payload$Timeout_E;
/* Timeout Error - Outer */
typedef union Timeout_E {
    struct {
        union {
            E_Tag$Timeout_E tag;
            E_Code code;
        };
        union {
            E_Payload$Timeout_E payload;
            E_HashId hash_id;
        };
    };
    General_E as_general[1];
    General_E General_E;
} Timeout_E;
$attr($inline_always)
static const char* E_strfy$Timeout_E(Timeout_E self) {
    switch (self.tag) {
    case E_Tag$Timeout: return "Timeout";
    case E_Tag$Any: __builtin_unreachable();
    }
}
/* Timeout Error - Occurrences */
typedef Timeout_E Timeout_E_Timeout;
$attr($inline_always)
static Timeout_E_Timeout E_cause$Timeout(void) {
    return (Timeout_E_Timeout){
        .tag = E_Tag$Timeout,
        .payload = {
            .Timeout = {
                .hash_id = E_hasher("Timeout", sizeof("Timeout") - 1),
            },
        },
    };
}
typedef struct O$Timeout_E {
    bool is_some;
    union {
        Timeout_E some;
        Void none;
    } payload;
} O$Timeout_E;
$attr($inline_always)
static O$Timeout_E E_resolve$Timeout_E(Timeout_E self) {
    const Timeout_E errs[] = {
        E_cause$Timeout(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.hash_id != errs[i].hash_id) continue;
        return (O$Timeout_E){
            .is_some = true,
            .payload = {
                .some = errs[i],
            }
        };
    }
    return (O$Timeout_E){ .is_some = false };
}
$attr($inline_always)
static Timeout_E E_handle$Timeout_E(Timeout_E self) {
    O$Timeout_E resolved = E_resolve$Timeout_E(self);
    if (resolved.is_some) return resolved.payload.some;
    return (Timeout_E)E_cause$Any(self.as_general);
}


#include <stdio.h>


/* Allow Only Foo Error - Inner */
typedef enum $packed E_Tag$Foo_E {
    E_Tag$Foo_E_Any = E_Tag$Any,
    E_Tag$FooBar = 1,
    E_Tag$Foo_E_FooBar = E_Tag$FooBar,
    E_Tag$FooBas = 2,
    E_Tag$Foo_E_FooBas = E_Tag$FooBas,
    E_Tag$FooBaz = 3,
    E_Tag$Foo_E_FooBaz = E_Tag$FooBaz,
} E_Tag$Foo_E;
typedef struct E_Opaq$Foo_E {
    E_HashId hash_id;
} E_Opaq$Foo_E;
typedef E_Opaq$Foo_E E_Type$FooBar;
typedef E_Opaq$Foo_E E_Type$FooBas;
typedef E_Opaq$Foo_E E_Type$FooBaz;
typedef union E_Payload$Foo_E {
    // E_Opaq$Foo_E;
    E_Type$FooBar FooBar;
    E_Type$FooBas FooBas;
    E_Type$FooBaz FooBaz;
} E_Payload$Foo_E;
/* Allow Only Foo Error - Outer */
typedef union Foo_E {
    struct {
        union {
            E_Tag$Foo_E tag;
            E_Code code;
        };
        union {
            E_Payload$Foo_E payload;
            E_HashId hash_id;
        };
    };
    General_E as_general[1];
    General_E General_E;
    Conc_E Conc_E;
    Cancelable Cancelable;
} Foo_E;
$attr($inline_always)
static const char* E_strfy$Foo_E(Foo_E self) {
    switch (self.tag) {
    case E_Tag$FooBar: return "FooBar";
    case E_Tag$FooBas: return "FooBas";
    case E_Tag$FooBaz: return "FooBaz";
    case E_Tag$Any: __builtin_unreachable();
    }
}
/* Allow Only Foo Error - Occurrences */
typedef Foo_E Foo_E_FooBar;
$attr($inline_always)
static Foo_E_FooBar E_cause$FooBar(void) {
    return (Foo_E_FooBar){
        .tag = E_Tag$FooBar,
        .payload = {
            .FooBar = {
                .hash_id = E_hasher("FooBar", sizeof("FooBar") - 1),
            },
        },
    };
}
typedef Foo_E Foo_E_FooBas;
$attr($inline_always)
static Foo_E_FooBas E_cause$FooBas(void) {
    return (Foo_E_FooBas){
        .tag = E_Tag$FooBas,
        .payload = {
            .FooBas = {
                .hash_id = E_hasher("FooBas", sizeof("FooBas") - 1),
            },
        },
    };
}
typedef Foo_E Foo_E_FooBaz;
$attr($inline_always)
static Foo_E_FooBaz E_cause$FooBaz(void) {
    return (Foo_E_FooBaz){
        .tag = E_Tag$FooBaz,
        .payload = {
            .FooBaz = {
                .hash_id = E_hasher("FooBaz", sizeof("FooBaz") - 1),
            },
        },
    };
}
typedef struct O$Foo_E {
    bool is_some;
    union {
        Foo_E some;
        Void none;
    } payload;
} O$Foo_E;
$attr($inline_always)
static O$Foo_E E_resolve$Foo_E(Foo_E self) {
    const Foo_E errs[] = {
        E_cause$FooBar(),
        E_cause$FooBas(),
        E_cause$FooBaz(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.hash_id != errs[i].hash_id) continue;
        return (O$Foo_E){
            .is_some = true,
            .payload = {
                .some = errs[i],
            }
        };
    }
    return (O$Foo_E){ .is_some = false };
}
$attr($inline_always)
static Foo_E E_handle$Foo_E(Foo_E self) {
    O$Foo_E resolved = E_resolve$Foo_E(self);
    if (resolved.is_some) return resolved.payload.some;
    return (Foo_E)E_cause$Any(self.as_general);
}


#define let __auto_type const
#define var __auto_type

static Foo_E mustFail(void) {
    return (Foo_E)E_cause$Canceled();
}

/// resolve$E: Treats the tag as belonging to error set E and matches the hash ID. If the tag does not match error set E, returns none.
/// handle$E: If resolve returns none, returns the tag as any while keeping the hash ID.
int main(void) {
    let my_err = mustFail();
    switch (E_handle$Foo_E(my_err).tag) {
    case E_Tag$FooBar: {
        let captured = (const __typeof__(E_cause$FooBar())*)&my_err;
        printf("handler case 1: %s\n", E_strfy$Foo_E(*captured));
    } break;
    case E_Tag$FooBas: {
        let captured = (const __typeof__(E_cause$FooBas())*)&my_err;
        printf("handler case 2: %s\n", E_strfy$Foo_E(*captured));
    } break;
    case E_Tag$FooBaz: {
        let captured = (const __typeof__(E_cause$FooBaz())*)&my_err;
        printf("handler case 3: %s\n", E_strfy$Foo_E(*captured));
    } break;
    case E_Tag$Any: {
        let captured = my_err;
        {
            let maybe_conc = E_resolve$Conc_E(captured.Conc_E);
            if (maybe_conc.is_some) {
                switch (maybe_conc.payload.some.tag) {
                case E_Tag$Conc_E_ConcUnavailable: {
                    let captured = (const __typeof__(E_cause$ConcUnavailable())*)&maybe_conc.payload.some;
                    printf("handler case 4: %s\n", E_strfy$Conc_E(*captured));
                } break;
                case E_Tag$Any: __builtin_unreachable();
                }
            }
        }
        {
            let maybe_cancelable = E_resolve$Cancelable(captured.Cancelable);
            if (maybe_cancelable.is_some) {
                switch (maybe_cancelable.payload.some.tag) {
                case E_Tag$Cancelable_Canceled: {
                    let captured = (const __typeof__(E_cause$Canceled())*)&maybe_cancelable.payload.some;
                    printf("handler case 5: %s\n", E_strfy$Cancelable(*captured));
                } break;
                case E_Tag$Any: __builtin_unreachable();
                }
            }
        }
    } break;
    }
    return 0;
}
// static fn_((test(void))(void)) {
//     match_((E_handle$Foo_E(mustFail()))) {
//     pattern_((Foo_E_FooBar)(captured)) {
//         (void)captured;
//     } $end(pattern);
//     pattern_((Foo_E_FooBas)(captured)) {
//         (void)captured;
//     } $end(pattern);
//     pattern_((Foo_E_FooBaz)(captured)) {
//         (void)captured;
//     } $end(pattern);
//     pattern_((Any)(captured)) {
//         if_some((E_resolve$Conc_E(captured.Conc_E))(conc_err)) {
//             match_((conc_err)) {
//             pattern_((Conc_E_ConcUnavailable)(captured)) {
//                 (void)captured;
//             } $end(pattern);
//             pattern_((Any)(captured)) claim_unreachable $end(pattern);
//             } $end(match);
//         } else if_some((E_resolve$Cancelable(captured.Cancelable))(cancelable_err)) {
//             match_((cancelable_err)) {
//             pattern_((Cancelable_Canceled)(captured)) {
//                 (void)captured;
//             } $end(pattern);
//             pattern_((Any)(captured)) claim_unreachable $end(pattern);
//             } $end(match);
//         } else {
//             claim_unreachable;
//         }
//     } $end(pattern);
//     }$end(match);
// }
