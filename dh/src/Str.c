#include "dh/Str.h"
#include "dh/mem/common.h"

#include <ctype.h>
#include <locale.h>
#include <stdarg.h>

#if bti_plat_windows
#include "dh/os/windows/mem.h"
#else /* posix */
#include <sys/mman.h>
#include <unistd.h>
#endif /* posix */

static $on_load fn_(init(void), void) {
    static bool s_initialized = false;
    if (s_initialized) { return; }
    /* Set locale for proper UTF-8 handling */
    $ignore       = setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
    s_initialized = true;
}

Str_const Str_view(const u8* ptr, usize len) {
    debug_assert_nonnull(ptr);
    return (Str_const){ .ptr = ptr, .len = len };
}

Str_const Str_viewZ(const u8* ptr) {
    debug_assert_nonnull(ptr);
    return Str_view(ptr, strlen(as$(const char*, ptr)));
}

Str Str_from(u8 ptr[], usize len) {
    debug_assert_nonnull(ptr);
    return (Str){ .ptr = ptr, .len = len };
}

Str Str_fromZ(u8 ptr[]) {
    debug_assert_nonnull(ptr);
    return Str_from(ptr, strlen(as$(char*, ptr)));
}

usize Str_len(Str_const self) {
    debug_assert_nonnull(self.ptr);
    return self.len;
}

bool Str_eql(Str_const lhs, Str_const rhs) {
    debug_assert_nonnull(lhs.ptr);
    debug_assert_nonnull(rhs.ptr);

    if (lhs.len != rhs.len) { return false; }
    return mem_eqlBytes(lhs.ptr, rhs.ptr, lhs.len);
}

bool Str_eqlNoCase(Str_const lhs, Str_const rhs) {
    debug_assert_nonnull(lhs.ptr);
    debug_assert_nonnull(rhs.ptr);

    if (lhs.len != rhs.len) { return false; }
    let len = lhs.len;
    for (usize ch = 0; ch < len; ++ch) {
        if (tolower(*Sli_at(lhs, ch)) != tolower(*Sli_at(rhs, ch))) {
            return false;
        }
    }
    return true;
}

bool Str_constCastable(Str_const self) {
    debug_assert_nonnull(self.ptr);
#if bti_plat_windows
    MEMORY_BASIC_INFORMATION mbi = cleared();
    if (!VirtualQuery(self.ptr, &mbi, sizeOf(mbi))) { return false; }
    return (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)) != 0;
#else  /* posix */
    return !mprotect(
        intToRawptr$(anyptr, rawptrToInt(self.ptr) & ~(sysconf(_SC_PAGESIZE) - 1)),
        1,
        PROT_READ | PROT_WRITE
    );
#endif /* posix */
}

Opt$Str Str_constCast(Str_const self) {
    debug_assert_nonnull(self.ptr);
    if (!Str_constCastable(self)) { return none$(Opt$Str); }
    return some$(Opt$Str, Str_from(as$(u8*, self.ptr), self.len));
}

fn_(Str_cat(mem_Allocator allocator, Str_const lhs, Str_const rhs), Err$Str, $scope) {
    debug_assert_nonnull(lhs.ptr);
    debug_assert_nonnull(rhs.ptr);

    let total_len = lhs.len + rhs.len;
    let result    = meta_cast$(Str, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), total_len)));
    mem_copyBytes(result.ptr, lhs.ptr, lhs.len);
    mem_copyBytes(result.ptr + lhs.len, rhs.ptr, rhs.len);
    return_ok(result);
} $unscoped;

fn_(Str_print(Str_const self), void) {
    debug_assert_nonnull(self.ptr);
    if (self.len <= 0) { return; }
    printf("%.*s", as$(i32, self.len), self.ptr);
}

fn_(Str_println(Str_const self), void) {
    debug_assert_nonnull(self.ptr);
    Str_print(self);
    $ignore = putchar('\n');
}

fn_(Str_format(mem_Allocator allocator, const char* format, ...), Err$Str, $guard) {
    va_list args1 = {};
    va_start(args1, format);
    defer_(va_end(args1));

    va_list args2 = {};
    va_copy(args2, args1);
    defer_(va_end(args2));

    let len = eval({
        let res = vsnprintf(null, 0, format, args1);
        if (res < 0) { return_err(mem_Allocator_Err_OutOfMemory()); }
        eval_return as$(usize, res);
    });

    var result = meta_cast$(Str, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), len + 1)));
    errdefer_(mem_Allocator_free(allocator, anySli(result)));
    {
        $ignore    = vsnprintf((char*)result.ptr, len + 1, format, args2);
        result.len = len;
    }
    return_ok(result);
} $unguarded;

Str_const Str_slice(Str_const self, usize start, usize end) {
    debug_assert_nonnull(self.ptr);
    debug_assert(start < end);
    debug_assert(end <= self.len);

    return Str_view(self.ptr + start, end - start);
}

Str_const Str_trim(Str_const self) {
    debug_assert_nonnull(self.ptr);
    usize start = 0;
    usize end   = self.len;

    while (start < end && isspace(self.ptr[start])) { start++; }
    while (end > start && isspace(self.ptr[end - 1])) { end--; }

    return Str_slice(self, start, end);
}

Str_const Str_ltrim(Str_const self) {
    debug_assert_nonnull(self.ptr);
    usize start = 0;
    while (start < self.len && isspace(self.ptr[start])) { start++; }
    return Str_slice(self, start, self.len);
}

Str_const Str_rtrim(Str_const self) {
    debug_assert_nonnull(self.ptr);
    usize end = self.len;
    while (end > 0 && isspace(self.ptr[end - 1])) { end--; }
    return Str_slice(self, 0, end);
}

fn_(Str_upper(mem_Allocator allocator, Str_const str), Err$Str, $scope) {
    debug_assert_nonnull(str.ptr);

    let result = meta_cast$(Str, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), str.len)));
    for_slice_indexed (result, ch, i) {
        *ch = as$(u8, toupper(*Sli_at(str, i)));
    }
    return_ok(result);
} $unscoped;

fn_(Str_lower(mem_Allocator allocator, Str_const str), Err$Str, $scope) {
    debug_assert_nonnull(str.ptr);

    let result = meta_cast$(Str, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), str.len)));
    for_slice_indexed (result, ch, i) {
        *ch = as$(u8, tolower(*Sli_at(str, i)));
    }
    return_ok(result);
} $unscoped;

bool Str_contains(Str_const haystack, Str_const needle) {
    debug_assert_nonnull(haystack.ptr);
    debug_assert_nonnull(needle.ptr);

    if (haystack.len < needle.len) { return false; }
    for (usize i = 0; i <= haystack.len - needle.len; ++i) {
        if (mem_eqlBytes(haystack.ptr + i, needle.ptr, needle.len)) {
            return true;
        }
    }
    return false;
}

fn_(Str_find(Str_const haystack, Str_const needle, usize start), Opt$usize, $scope) {
    debug_assert_nonnull(haystack.ptr);
    debug_assert_nonnull(needle.ptr);

    if (haystack.len <= start || haystack.len < needle.len) { return_none(); }
    for (usize i = start; i <= haystack.len - needle.len; ++i) {
        if (mem_eqlBytes(haystack.ptr + i, needle.ptr, needle.len)) {
            return_some(i);
        }
    }
    return_none();
} $unscoped;

fn_(Str_rfind(Str_const haystack, Str_const needle, usize start), Opt$usize, $scope) {
    debug_assert_nonnull(haystack.ptr);
    debug_assert_nonnull(needle.ptr);

    if (haystack.len <= start || haystack.len < needle.len) { return_none(); }
    for (usize i = prim_min(start, haystack.len - needle.len); i != as$(usize, -1); i--) {
        if (mem_eqlBytes(haystack.ptr + i, needle.ptr, needle.len)) {
            return_some(i);
        }
    }
    return_none();
} $unscoped;

bool Str_startsWith(Str_const self, Str_const prefix) {
    debug_assert_nonnull(self.ptr);
    if (self.len < prefix.len) { return false; }
    return mem_eqlBytes(self.ptr, prefix.ptr, prefix.len);
}

bool Str_endsWith(Str_const self, Str_const suffix) {
    debug_assert_nonnull(self.ptr);
    if (self.len < suffix.len) { return false; }
    return mem_eqlBytes(self.ptr + self.len - suffix.len, suffix.ptr, suffix.len);
}

static u32 hashMurmur3(const u8* data, usize len) {
    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;
    const u32 r1 = 15;
    const u32 r2 = 13;
    const u32 m  = 5;
    const u32 n  = 0xe6546b64;

    u32 hash = 0;

    let nblocks = as$(i32, len / 4);
    let blocks  = as$(const u32*, data);

    for (i32 i = 0; i < nblocks; ++i) {
        u32 k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    let tail = data + as$(ptrdiff, nblocks * 4);
    u32 k1   = 0;
    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
        $fallthrough;
    case 2:
        k1 ^= tail[1] << 8;
        $fallthrough;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    default:
        break;
    }

    hash ^= len;
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    return hash;
}

StrHash Str_hash(Str_const self) {
    debug_assert_nonnull(self.ptr);
    return hashMurmur3(self.ptr, self.len);
}

cmp_fnCmp(Str_const) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(other.ptr);

    let min_len = prim_min(self.len, other.len);
    let cmp     = mem_cmp(self.ptr, other.ptr, min_len);
    if (cmp != 0) { return cmp; }

    return prim_cmp(self.len, self.len);
}

cmp_fnCmp(Str) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(other.ptr);

    let min_len = prim_min(self.len, other.len);
    let cmp     = mem_cmp(self.ptr, other.ptr, min_len);
    if (cmp != 0) { return cmp; }

    return prim_cmp(self.len, self.len);
}

usize StrUtf8_len(Str_const self) {
    debug_assert_nonnull(self.ptr);
    usize count = 0;
    for (usize i = 0; i < self.len; ++i) {
        if ((self.ptr[i] & StrUtf8_cont_mask) != StrUtf8_cont_pattern) {
            count++;
        }
    }
    return count;
}

u8 StrUtf8_seqLen(Str_const self, usize pos) {
    debug_assert_nonnull(self.ptr);
    debug_assert(pos < self.len);

    const u8 byte = self.ptr[pos];

    if ((byte & StrUtf8_1byte_mask) == StrUtf8_1byte_pattern) { return 1; }
    if ((byte & StrUtf8_2byte_mask) == StrUtf8_2byte_pattern) { return 2; }
    if ((byte & StrUtf8_3byte_mask) == StrUtf8_3byte_pattern) { return 3; }
    if ((byte & StrUtf8_4byte_mask) == StrUtf8_4byte_pattern) { return 4; }

    return 1; // Invalid UTF-8 sequence, treat as single byte
}

bool StrUtf8_isValid(Str_const self) {
    debug_assert_nonnull(self.ptr);

    for (usize i = 0; i < self.len;) {
        let seq_len = StrUtf8_seqLen(self, i);

        if (i + seq_len > self.len) { return false; }

        for (usize j = 1; j < seq_len; ++j) {
            if ((self.ptr[i + j] & StrUtf8_cont_mask) != StrUtf8_cont_pattern) {
                return false;
            }
        }

        i += seq_len;
    }
    return true;
}

fn_(StrUtf8_codepointAt(Str_const self, usize pos), Opt$u32, $scope) {
    debug_assert_nonnull(self.ptr);
    debug_assert(pos < self.len);

    const u8 seq_len = StrUtf8_seqLen(self, pos);

    switch (seq_len) {
    case 1:
        return_some(
            self.ptr[pos]
        );
    case 2:
        return_some(
            ((self.ptr[pos] & 0x1F) << 6)
            | (self.ptr[pos + 1] & 0x3F)
        );
    case 3:
        return_some(
            ((self.ptr[pos] & 0x0F) << 12)
            | ((self.ptr[pos + 1] & 0x3F) << 6)
            | (self.ptr[pos + 2] & 0x3F)
        );
    case 4:
        return_some(
            ((self.ptr[pos] & 0x07) << 18)
            | ((self.ptr[pos + 1] & 0x3F) << 12)
            | ((self.ptr[pos + 2] & 0x3F) << 6)
            | (self.ptr[pos + 3] & 0x3F)
        );
    default:
        break;
    }

    return_none();
} $unscoped;

StrUtf8Iter StrUtf8_iter(Str_const self) {
    debug_assert_nonnull(self.ptr);
    return (StrUtf8Iter){ .str = self, .pos = 0 };
}

bool StrUtf8Iter_next(StrUtf8Iter* iter, Opt$u32* out_codepoint) {
    debug_assert_nonnull(iter);
    debug_assert_nonnull(iter->str.ptr);
    debug_assert_nonnull(out_codepoint);

    if (iter->str.len <= iter->pos) { return false; }
    *out_codepoint = StrUtf8_codepointAt(iter->str, iter->pos);
    iter->pos += StrUtf8_seqLen(iter->str, iter->pos);
    return true;
}

StrTokenizer Str_tokenizer(Str_const self, Str_const delims) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(delims.ptr);

    return (StrTokenizer){
        .str    = self,
        .delims = delims,
        .pos    = 0
    };
}

fn_(StrTokenizer_next(StrTokenizer* self), Opt$Str_const, $scope) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->str.ptr);
    debug_assert_nonnull(self->delims.ptr);

    // Skip delimiters at start
    while (self->pos < self->str.len) {
        bool is_delim = false;
        for (usize i = 0; i < self->delims.len; ++i) {
            if (self->str.ptr[self->pos] == self->delims.ptr[i]) {
                is_delim = true;
                break;
            }
        }
        if (!is_delim) { break; }
        self->pos++;
    }

    // No more tokens?
    if (self->pos >= self->str.len) { return_none(); }

    let start = self->pos;

    // Find end of token
    while (self->pos < self->str.len) {
        bool is_delim = false;
        for (usize i = 0; i < self->delims.len; ++i) {
            if (self->str.ptr[self->pos] == self->delims.ptr[i]) {
                is_delim = true;
                break;
            }
        }
        if (is_delim) { break; }
        self->pos++;
    }

    return_some(Str_slice(self->str, start, self->pos));
} $unscoped;
