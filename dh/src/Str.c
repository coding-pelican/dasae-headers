#include "dh/Str.h"
#include "dh/mem/common.h"

#include <locale.h>
#include <memoryapi.h>
#include <winnt.h>

static void __attribute__((constructor)) StrUtf8__init(void) {
    static bool initialized = false;
    if (initialized) { return; }
    /* Set locale for proper UTF-8 handling */
    ignore setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
    initialized = true;
}

StrConst Str_const(const u8* ptr, usize len) {
    debug_assert_nonnull(ptr);
    return (StrConst){ .ptr = ptr, .len = len };
}

Str Str_from(u8 ptr[], usize len) {
    debug_assert_nonnull(ptr);
    return (Str){ .ptr = ptr, .len = len };
}

usize Str_len(StrConst str) {
    return str.len;
}

bool Str_eq(StrConst lhs, StrConst rhs) {
    if (lhs.len != rhs.len) { return false; }
    return mem_eqlBytes(lhs.ptr, rhs.ptr, lhs.len);
}

bool Str_constCastable(StrConst str) {
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mbi = cleared();
    if (!VirtualQuery(str.ptr, &mbi, sizeof(mbi))) { return false; }
    return (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)) != 0;
#else
    // POSIX systems
    return !mprotect(
        (void*)((uintptr_t)str.ptr & ~(sysconf(_SC_PAGESIZE) - 1)),
        1,
        PROT_READ | PROT_WRITE
    )
#endif
}

Opt$Str Str_mut(StrConst str) {
    reserveReturn(Opt$Str);
    if (!Str_constCastable(str)) { return_none(); }
    return_some(Str_from((u8*)str.ptr, str.len));
}

StrConst Str_slice(StrConst str, usize start, usize end) {
    debug_assert(start <= end);
    debug_assert(end <= str.len);
    return Str_const(str.ptr + start, end - start);
}

bool Str_contains(StrConst haystack, StrConst needle) {
    if (haystack.len < needle.len) { return false; }
    for (usize index = 0; index <= haystack.len - needle.len; ++index) {
        if (mem_eqlBytes(haystack.ptr + index, needle.ptr, needle.len)) {
            return true;
        }
    }
    return false;
}

usize StrUtf8_len(StrConst str) {
    usize count = 0;
    for (usize index = 0; index < str.len; ++index) {
        if ((str.ptr[index] & StrUtf8_cont_mask) != StrUtf8_cont_pattern) {
            count++;
        }
    }
    return count;
}

u8 StrUtf8_sequenceLen(StrConst str, usize pos) {
    debug_assert(pos < str.len);
    const u8 byte = str.ptr[pos];

    if ((byte & StrUtf8_1byte_mask) == StrUtf8_1byte_pattern) {
        return 1;
    }
    if ((byte & StrUtf8_2byte_mask) == StrUtf8_2byte_pattern) {
        return 2;
    }
    if ((byte & StrUtf8_3byte_mask) == StrUtf8_3byte_pattern) {
        return 3;
    }
    if ((byte & StrUtf8_4byte_mask) == StrUtf8_4byte_pattern) {
        return 4;
    }

    return 1; // Invalid UTF-8 sequence, treat as single byte
}

bool StrUtf8_isValid(StrConst str) {
    for (usize i = 0; i < str.len;) {
        u8 seq_len = StrUtf8_sequenceLen(str, i);

        if (i + seq_len > str.len) {
            return false;
        }

        for (usize j = 1; j < seq_len; ++j) {
            if ((str.ptr[i + j] & StrUtf8_cont_mask) != StrUtf8_cont_pattern) {
                return false;
            }
        }

        i += seq_len;
    }
    return true;
}

Opt$u32 StrUtf8_codepointAt(StrConst str, usize pos) {
    reserveReturn(Opt$u32);
    debug_assert(pos < str.len);

    const u8 seq_len = StrUtf8_sequenceLen(str, pos);

    switch (seq_len) {
    case 1:
        return_some(
            str.ptr[pos]
        );
    case 2:
        return_some(
            ((str.ptr[pos] & 0x1F) << 6)
            | (str.ptr[pos + 1] & 0x3F)
        );
    case 3:
        return_some(
            ((str.ptr[pos] & 0x0F) << 12)
            | ((str.ptr[pos + 1] & 0x3F) << 6)
            | (str.ptr[pos + 2] & 0x3F)
        );
    case 4:
        return_some(
            ((str.ptr[pos] & 0x07) << 18)
            | ((str.ptr[pos + 1] & 0x3F) << 12)
            | ((str.ptr[pos + 2] & 0x3F) << 6)
            | (str.ptr[pos + 3] & 0x3F)
        );
    default:
        break;
    }
    return_none();
}

StrUtf8Iter StrUtf8_iter(StrConst str) {
    return (StrUtf8Iter){ .str = str, .pos = 0 };
}

bool StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint) {
    if (iter->str.len <= iter->pos) { return false; }
    *out_codepoint = StrUtf8_codepointAt(iter->str, iter->pos);
    iter->pos += StrUtf8_sequenceLen(iter->str, iter->pos);
    return true;
}
