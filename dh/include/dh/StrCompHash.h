/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    StrCompHash.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-27 (date of creation)
 * @updated 2025-02-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  StrCompHash
 *
 * @brief   String hashing implementation for C
 * @details This implementation uses a variant of DJB hash algorithm
 * with a multiplier of 65599 for efficient string hashing.
 */

#ifndef STR_COMP_HASH_INCLUDED
#define STR_COMP_HASH_INCLUDED (1)

/*========== Includes =======================================================*/

#include "core.h"
#include "Str.h"

/*========== Macros and Definitions =========================================*/

/// @struct StrCompHash
/// @brief Structure to hold hash values for strings
///
/// This structure is used to store and manipulate hash values calculated from strings.
typedef struct StrCompHash {
    u32 value;
} StrCompHash;

#if !COMP_TIME
/// @brief Calculate hash from raw string literal (based char pointer)
///
/// @param raw_str String literal
/// @return Calculated hash value
force_inline u32         StrCompHash_calculateRaw(const char* raw_str);
/// @brief Create a StrCompHash object from raw string literal (based char pointer)
///
/// @param raw_str String literal
/// @return StrCompHash structure
force_inline StrCompHash StrCompHash_createRaw(const char* raw_str);
#endif /* !COMP_TIME */

/// @brief Calculate hash from String slice's pointer and length
///
/// @param ptr String slice's beginning address
/// @param len String slice's length
/// @return Calculated hash value
force_inline u32         StrCompHash_calculate(const u8* ptr, usize len);
/// @brief Create a StrCompHash object from String slice
///
/// @param str String slice
/// @return StrCompHash structure
force_inline StrCompHash StrCompHash_create(Str_const str);
/// @brief Get the u32 integer value of a StrCompHash
///
/// @param hash StrCompHash structure
/// @return u32 hash value
force_inline u32         StrCompHash_value(StrCompHash self);

/// Base cases for recursive macro-based hashing
#define StrCompHash__char1(_raw_ch)  FUNC__StrCompHash__char1(_raw_ch)
#define StrCompHash__char2(_raw_str) FUNC__StrCompHash__char2(_raw_str)
#define StrCompHash__char3(_raw_str) FUNC__StrCompHash__char3(_raw_str)
#define StrCompHash__char4(_raw_str) FUNC__StrCompHash__char4(_raw_str)
/// Recursive hashing for longer strings
static_inline u32 StrCompHash__recur(const u8* ptr, usize len);

/*========== Macros and Implementations =====================================*/

#if COMP_TIME
#define StrCompHash_calculateRaw(_raw_str) comp_inline__StrCompHash_calculateRaw(_raw_str)
#define StrCompHash_createRaw(_raw_str)    comp_inline__StrCompHash_createRaw(_raw_str)
#endif /* COMP_TIME */
#define comp_inline__StrCompHash_calculateRaw(_raw_str) ((u32)StrCompHash__recur(_raw_str, sizeof(_raw_str) - 1))
#define comp_inline__StrCompHash_createRaw(_raw_str)    ((StrCompHash){ .val = StrCompHash_calculateRaw(_raw_str) })

#define FUNC__StrCompHash__char1(_raw_ch)  (as$(u32, _raw_ch))
#define FUNC__StrCompHash__char2(_raw_str) (as$(u32, (_raw_str)[1] + 65599 * (_raw_str)[0]))
#define FUNC__StrCompHash__char3(_raw_str) (as$(u32, (_raw_str)[2] + 65599 * StrCompHash__char2(_raw_str)))
#define FUNC__StrCompHash__char4(_raw_str) (as$(u32, (_raw_str)[3] + 65599 * StrCompHash__char3(_raw_str)))
static_inline u32 StrCompHash__recur(const u8* ptr, usize len) { /* NOLINT(misc-no-recursion) */
    if (len == 1) { return StrCompHash__char1(ptr[0]); }
    if (len == 2) { return StrCompHash__char2(ptr); }
    if (len == 3) { return StrCompHash__char3(ptr); }
    if (len == 4) { return StrCompHash__char4(ptr); }
    // clang-format off
    return ptr[len - 1] + 65599
        * (ptr[len - 2] + 65599
        * (ptr[len - 3] + 65599
        * (ptr[len - 4] + 65599 * StrCompHash__recur(ptr, len - 4))
        )
    );
    // clang-format on
}

force_inline u32 StrCompHash_calculate(const u8* ptr, usize len) {
    return StrCompHash__recur(ptr, len);
}
force_inline StrCompHash StrCompHash_create(Str_const str) {
    return (StrCompHash){ .value = StrCompHash_calculate(str.ptr, str.len) };
}
force_inline u32 StrCompHash_value(StrCompHash self) {
    return self.value;
}

#endif /* STR_COMP_HASH_INCLUDED */
