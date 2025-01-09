/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    StrConst.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-18 (date of creation)
 * @updated 2024-12-18 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Implementation of Zig-style strings with reference wrapper
 * @details
 */

#ifndef STR_INCLUDED
#define STR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"
#include "Ptr.h"
#include "Sli.h"
#include "ext_types.h"

#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

/*========== Macros and Definitions =========================================*/

/* String literal types - mimicking Zig's string literal behavior */
typedef PtrSConst_u8 CStrConst; /* [*:0]const u8 - Null-terminated string */
typedef SliConst_u8  StrConst;  /* []const u8 - String slice */
typedef SliSConst_u8 StrSConst; /* [:0]const u8 - Sentinel-terminated slice */

/* String literal conversion macros */
#define cstrL(literal) ((CStrConst){ .base = PtrSConst_u8_from((u8*)(literal), '\0').base })
#define strL(literal)  ((StrConst){ .base = SliConst_u8_from((u8*)(literal), 0, sizeof(literal) - 1).base })
#define strSL(literal) ((StrSConst){ .base = SliSConst_u8_from((u8*)(literal), 0, sizeof(literal) - 1, '\0').base })

/* UTF-8 string operations */
force_inline bool  utf8_isStartByte(u8 byte);
force_inline usize utf8_seqLen(u8 byte);

/* String iteration */
typedef struct StrIter {
    StrConst str;
    usize    pos;
} StrIter;

force_inline StrIter Str_iter(StrConst str);
force_inline bool    Str_next(StrIter* iter, StrConst* out_char);

/* string format functions */
force_inline void  Str_print(StrConst str);
force_inline void  Str_println(StrConst str);
force_inline usize Str_len(StrConst str);
force_inline usize Str_utf8Len(StrConst str);

/*========== Implementations ================================================*/

static_inline void __attribute__((constructor)) utf8__init(void) {
    static bool initialized = false;
    if (initialized) { return; }
    /* Set locale for proper UTF-8 handling */
    ignore setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
    initialized = true;
}

force_inline bool utf8_isStartByte(u8 byte) {
    return (byte & 0xC0) != 0x80;
}

force_inline usize utf8_seqLen(u8 byte) {
    if ((byte & 0x80) == 0x00) {
        return 1; /* 0xxxxxxx */
    }
    if ((byte & 0xE0) == 0xC0) {
        return 2; /* 110xxxxx */
    }
    if ((byte & 0xF0) == 0xE0) {
        return 3; /* 1110xxxx */
    }
    if ((byte & 0xF8) == 0xF0) {
        return 4; /* 11110xxx */
    }
    return 1; /* Invalid UTF-8 byte, treat as single byte */
}

force_inline StrIter Str_iter(StrConst str) {
    return (StrIter){ .str = str, .pos = 0 };
}

force_inline bool Str_next(StrIter* iter, StrConst* const out_ch) {
    if (iter->str.len <= iter->pos) { return false; }

    const usize start      = iter->pos;
    const u8    first_byte = *((u8*)Sli_at(typeInfo(const u8), iter->str.base, start));
    usize       ch_len     = utf8_seqLen(first_byte);

    /* Ensure we don't read past the end */
    if (iter->str.len < start + ch_len) {
        ch_len = 1; /* Invalid sequence, consume one byte */
    }

    *out_ch = SliConst_u8_from(iter->str.base.addr, start, start + ch_len);
    iter->pos += ch_len;
    return true;
}

force_inline void Str_print(StrConst str) {
#if defined(_WIN32)
    /* Windows: Use wide char conversion for proper UTF-8 display */
    for (usize index = 0; index < str.len;) {
        const u8    byte = SliConst_u8_at(str, index)->value;
        const usize len  = utf8_seqLen(byte);
        if (1 < len && len + index <= str.len) {
            wchar_t wc     = 0;
            int     result = mbtowc(&wc, (const char*)SliConst_u8_at(str, index), len);
            if (0 < result) {
                putwchar(wc);
                index += len;
                continue;
            }
        }
        putchar(byte);
        index++;
    }
#else
    /* Unix-like systems: Direct UTF-8 output */
    for (usize i = 0; i < str.len; ++i) {
        putchar(SliConst_u8_at(str, i)->value);
    }
#endif
}

force_inline void Str_println(StrConst str) {
    Str_print(str);
    putchar('\n');
}

force_inline usize Str_len(StrConst str) {
    return str.len;
}

force_inline usize Str_utf8Len(StrConst str) {
    usize count = 0;
    {
        StrIter  iter = Str_iter(str);
        StrConst ch   = cleared();
        while (Str_next(&iter, &ch)) {
            count++;
        }
    }
    return count;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STR_INCLUDED */
