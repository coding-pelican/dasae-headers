#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct { char *ptr; size_t len; } Buf;
typedef struct { const char *ptr; size_t len; } Str;
typedef enum { EV_NONE=0, EV_PLACEHOLDER=1, EV_ESCAPED=2, EV_FALLBACK=3 } EventKind;
typedef struct { EventKind kind; size_t pos; } Event;
typedef struct { int ok; size_t pos; } Close;
typedef struct { Str rest; size_t occ; } Iter;
typedef struct { int status; size_t len; } FmtRes; /* status 0 ok/continue, 1 fallback */

static inline __attribute__((always_inline)) Str str_lit(const char *p, size_t n) { return (Str){p,n}; }
static inline __attribute__((always_inline)) Buf buf_suffix(Buf b, size_t n) { return (Buf){b.ptr+n, b.len-n}; }
static inline __attribute__((always_inline)) Str str_suffix(Str s, size_t n) { return (Str){s.ptr+n, s.len-n}; }
static inline __attribute__((always_inline)) Str str_prefix(Str s, size_t n) { return (Str){s.ptr,n}; }

static inline __attribute__((always_inline)) Event find_event32(Str s) {
    if (s.len > 2u && s.ptr[0] == '%' && s.ptr[1] == '%' && s.ptr[2] == '{') return (Event){EV_ESCAPED, 0u};
    if (s.len > 1u && s.ptr[0] == '%' && s.ptr[1] == '{') return (Event){EV_PLACEHOLDER, 0u};
    if (s.len > 3u && s.ptr[1] == '%' && s.ptr[2] == '%' && s.ptr[3] == '{') return (Event){EV_ESCAPED, 1u};
    if (s.len > 2u && s.ptr[1] == '%' && s.ptr[2] == '{') return (Event){EV_PLACEHOLDER, 1u};
    if (s.len > 4u && s.ptr[2] == '%' && s.ptr[3] == '%' && s.ptr[4] == '{') return (Event){EV_ESCAPED, 2u};
    if (s.len > 3u && s.ptr[2] == '%' && s.ptr[3] == '{') return (Event){EV_PLACEHOLDER, 2u};
    if (s.len > 5u && s.ptr[3] == '%' && s.ptr[4] == '%' && s.ptr[5] == '{') return (Event){EV_ESCAPED, 3u};
    if (s.len > 4u && s.ptr[3] == '%' && s.ptr[4] == '{') return (Event){EV_PLACEHOLDER, 3u};
    if (s.len > 6u && s.ptr[4] == '%' && s.ptr[5] == '%' && s.ptr[6] == '{') return (Event){EV_ESCAPED, 4u};
    if (s.len > 5u && s.ptr[4] == '%' && s.ptr[5] == '{') return (Event){EV_PLACEHOLDER, 4u};
    if (s.len > 7u && s.ptr[5] == '%' && s.ptr[6] == '%' && s.ptr[7] == '{') return (Event){EV_ESCAPED, 5u};
    if (s.len > 6u && s.ptr[5] == '%' && s.ptr[6] == '{') return (Event){EV_PLACEHOLDER, 5u};
    if (s.len > 8u && s.ptr[6] == '%' && s.ptr[7] == '%' && s.ptr[8] == '{') return (Event){EV_ESCAPED, 6u};
    if (s.len > 7u && s.ptr[6] == '%' && s.ptr[7] == '{') return (Event){EV_PLACEHOLDER, 6u};
    if (s.len > 9u && s.ptr[7] == '%' && s.ptr[8] == '%' && s.ptr[9] == '{') return (Event){EV_ESCAPED, 7u};
    if (s.len > 8u && s.ptr[7] == '%' && s.ptr[8] == '{') return (Event){EV_PLACEHOLDER, 7u};
    if (s.len > 10u && s.ptr[8] == '%' && s.ptr[9] == '%' && s.ptr[10] == '{') return (Event){EV_ESCAPED, 8u};
    if (s.len > 9u && s.ptr[8] == '%' && s.ptr[9] == '{') return (Event){EV_PLACEHOLDER, 8u};
    if (s.len > 11u && s.ptr[9] == '%' && s.ptr[10] == '%' && s.ptr[11] == '{') return (Event){EV_ESCAPED, 9u};
    if (s.len > 10u && s.ptr[9] == '%' && s.ptr[10] == '{') return (Event){EV_PLACEHOLDER, 9u};
    if (s.len > 12u && s.ptr[10] == '%' && s.ptr[11] == '%' && s.ptr[12] == '{') return (Event){EV_ESCAPED, 10u};
    if (s.len > 11u && s.ptr[10] == '%' && s.ptr[11] == '{') return (Event){EV_PLACEHOLDER, 10u};
    if (s.len > 13u && s.ptr[11] == '%' && s.ptr[12] == '%' && s.ptr[13] == '{') return (Event){EV_ESCAPED, 11u};
    if (s.len > 12u && s.ptr[11] == '%' && s.ptr[12] == '{') return (Event){EV_PLACEHOLDER, 11u};
    if (s.len > 14u && s.ptr[12] == '%' && s.ptr[13] == '%' && s.ptr[14] == '{') return (Event){EV_ESCAPED, 12u};
    if (s.len > 13u && s.ptr[12] == '%' && s.ptr[13] == '{') return (Event){EV_PLACEHOLDER, 12u};
    if (s.len > 15u && s.ptr[13] == '%' && s.ptr[14] == '%' && s.ptr[15] == '{') return (Event){EV_ESCAPED, 13u};
    if (s.len > 14u && s.ptr[13] == '%' && s.ptr[14] == '{') return (Event){EV_PLACEHOLDER, 13u};
    if (s.len > 16u && s.ptr[14] == '%' && s.ptr[15] == '%' && s.ptr[16] == '{') return (Event){EV_ESCAPED, 14u};
    if (s.len > 15u && s.ptr[14] == '%' && s.ptr[15] == '{') return (Event){EV_PLACEHOLDER, 14u};
    if (s.len > 17u && s.ptr[15] == '%' && s.ptr[16] == '%' && s.ptr[17] == '{') return (Event){EV_ESCAPED, 15u};
    if (s.len > 16u && s.ptr[15] == '%' && s.ptr[16] == '{') return (Event){EV_PLACEHOLDER, 15u};
    if (s.len > 18u && s.ptr[16] == '%' && s.ptr[17] == '%' && s.ptr[18] == '{') return (Event){EV_ESCAPED, 16u};
    if (s.len > 17u && s.ptr[16] == '%' && s.ptr[17] == '{') return (Event){EV_PLACEHOLDER, 16u};
    if (s.len > 19u && s.ptr[17] == '%' && s.ptr[18] == '%' && s.ptr[19] == '{') return (Event){EV_ESCAPED, 17u};
    if (s.len > 18u && s.ptr[17] == '%' && s.ptr[18] == '{') return (Event){EV_PLACEHOLDER, 17u};
    if (s.len > 20u && s.ptr[18] == '%' && s.ptr[19] == '%' && s.ptr[20] == '{') return (Event){EV_ESCAPED, 18u};
    if (s.len > 19u && s.ptr[18] == '%' && s.ptr[19] == '{') return (Event){EV_PLACEHOLDER, 18u};
    if (s.len > 21u && s.ptr[19] == '%' && s.ptr[20] == '%' && s.ptr[21] == '{') return (Event){EV_ESCAPED, 19u};
    if (s.len > 20u && s.ptr[19] == '%' && s.ptr[20] == '{') return (Event){EV_PLACEHOLDER, 19u};
    if (s.len > 22u && s.ptr[20] == '%' && s.ptr[21] == '%' && s.ptr[22] == '{') return (Event){EV_ESCAPED, 20u};
    if (s.len > 21u && s.ptr[20] == '%' && s.ptr[21] == '{') return (Event){EV_PLACEHOLDER, 20u};
    if (s.len > 23u && s.ptr[21] == '%' && s.ptr[22] == '%' && s.ptr[23] == '{') return (Event){EV_ESCAPED, 21u};
    if (s.len > 22u && s.ptr[21] == '%' && s.ptr[22] == '{') return (Event){EV_PLACEHOLDER, 21u};
    if (s.len > 24u && s.ptr[22] == '%' && s.ptr[23] == '%' && s.ptr[24] == '{') return (Event){EV_ESCAPED, 22u};
    if (s.len > 23u && s.ptr[22] == '%' && s.ptr[23] == '{') return (Event){EV_PLACEHOLDER, 22u};
    if (s.len > 25u && s.ptr[23] == '%' && s.ptr[24] == '%' && s.ptr[25] == '{') return (Event){EV_ESCAPED, 23u};
    if (s.len > 24u && s.ptr[23] == '%' && s.ptr[24] == '{') return (Event){EV_PLACEHOLDER, 23u};
    if (s.len > 26u && s.ptr[24] == '%' && s.ptr[25] == '%' && s.ptr[26] == '{') return (Event){EV_ESCAPED, 24u};
    if (s.len > 25u && s.ptr[24] == '%' && s.ptr[25] == '{') return (Event){EV_PLACEHOLDER, 24u};
    if (s.len > 27u && s.ptr[25] == '%' && s.ptr[26] == '%' && s.ptr[27] == '{') return (Event){EV_ESCAPED, 25u};
    if (s.len > 26u && s.ptr[25] == '%' && s.ptr[26] == '{') return (Event){EV_PLACEHOLDER, 25u};
    if (s.len > 28u && s.ptr[26] == '%' && s.ptr[27] == '%' && s.ptr[28] == '{') return (Event){EV_ESCAPED, 26u};
    if (s.len > 27u && s.ptr[26] == '%' && s.ptr[27] == '{') return (Event){EV_PLACEHOLDER, 26u};
    if (s.len > 29u && s.ptr[27] == '%' && s.ptr[28] == '%' && s.ptr[29] == '{') return (Event){EV_ESCAPED, 27u};
    if (s.len > 28u && s.ptr[27] == '%' && s.ptr[28] == '{') return (Event){EV_PLACEHOLDER, 27u};
    if (s.len > 30u && s.ptr[28] == '%' && s.ptr[29] == '%' && s.ptr[30] == '{') return (Event){EV_ESCAPED, 28u};
    if (s.len > 29u && s.ptr[28] == '%' && s.ptr[29] == '{') return (Event){EV_PLACEHOLDER, 28u};
    if (s.len > 31u && s.ptr[29] == '%' && s.ptr[30] == '%' && s.ptr[31] == '{') return (Event){EV_ESCAPED, 29u};
    if (s.len > 30u && s.ptr[29] == '%' && s.ptr[30] == '{') return (Event){EV_PLACEHOLDER, 29u};
    if (s.len > 32u && s.ptr[30] == '%' && s.ptr[31] == '%' && s.ptr[32] == '{') return (Event){EV_ESCAPED, 30u};
    if (s.len > 31u && s.ptr[30] == '%' && s.ptr[31] == '{') return (Event){EV_PLACEHOLDER, 30u};
    if (s.len > 33u && s.ptr[31] == '%' && s.ptr[32] == '%' && s.ptr[33] == '{') return (Event){EV_ESCAPED, 31u};
    if (s.len > 32u && s.ptr[31] == '%' && s.ptr[32] == '{') return (Event){EV_PLACEHOLDER, 31u};
    if (s.len > 32u) return (Event){EV_FALLBACK, 0};
    return (Event){EV_NONE, 0};
}

static inline __attribute__((always_inline)) Close find_close16(Str s) {
    if (s.len > 0u && s.ptr[0] == '}') return (Close){1, 0u};
    if (s.len > 1u && s.ptr[1] == '}') return (Close){1, 1u};
    if (s.len > 2u && s.ptr[2] == '}') return (Close){1, 2u};
    if (s.len > 3u && s.ptr[3] == '}') return (Close){1, 3u};
    if (s.len > 4u && s.ptr[4] == '}') return (Close){1, 4u};
    if (s.len > 5u && s.ptr[5] == '}') return (Close){1, 5u};
    if (s.len > 6u && s.ptr[6] == '}') return (Close){1, 6u};
    if (s.len > 7u && s.ptr[7] == '}') return (Close){1, 7u};
    if (s.len > 8u && s.ptr[8] == '}') return (Close){1, 8u};
    if (s.len > 9u && s.ptr[9] == '}') return (Close){1, 9u};
    if (s.len > 10u && s.ptr[10] == '}') return (Close){1, 10u};
    if (s.len > 11u && s.ptr[11] == '}') return (Close){1, 11u};
    if (s.len > 12u && s.ptr[12] == '}') return (Close){1, 12u};
    if (s.len > 13u && s.ptr[13] == '}') return (Close){1, 13u};
    if (s.len > 14u && s.ptr[14] == '}') return (Close){1, 14u};
    if (s.len > 15u && s.ptr[15] == '}') return (Close){1, 15u};
    return (Close){0, 0};
}


static inline __attribute__((always_inline)) size_t copy_bytes(Buf out, Str s) {
    __builtin_memcpy(out.ptr, s.ptr, s.len);
    return s.len;
}

static inline __attribute__((always_inline)) size_t write_i32(Buf out, int v) {
    char tmp[16];
    unsigned x;
    size_t pos = sizeof tmp;
    bool neg = v < 0;
    x = neg ? (unsigned)(-(long long)v) : (unsigned)v;
    if (x == 0) tmp[--pos] = '0';
    while (x != 0) { tmp[--pos] = (char)('0' + (x % 10)); x /= 10; }
    if (neg) tmp[--pos] = '-';
    return copy_bytes(out, (Str){tmp + pos, sizeof tmp - pos});
}

static inline __attribute__((always_inline)) size_t write_body(Buf out, Str body, Str s0, int i0) {
    if (body.len == 1 && body.ptr[0] == 's') return copy_bytes(out, s0);
    if (body.len == 1 && body.ptr[0] == 'i') return write_i32(out, i0);
    return (size_t)-1; /* unsupported -> fallback */
}

static inline __attribute__((always_inline)) FmtRes next_or_zero(Iter *it, Buf out0, size_t *written, Str s0, int i0) {
    Event ev = find_event32(it->rest);
    if (ev.kind == EV_NONE) return (FmtRes){0, *written};
    if (ev.kind == EV_FALLBACK) return (FmtRes){1, 0};

    Buf out = buf_suffix(out0, *written);
    *written += copy_bytes(out, str_prefix(it->rest, ev.pos));

    if (ev.kind == EV_ESCAPED) {
        out0.ptr[(*written)++] = '%';
        out0.ptr[(*written)++] = '{';
        it->rest = str_suffix(it->rest, ev.pos + 3);
        return (FmtRes){0, *written};
    }

    size_t body_start = ev.pos + 2;
    Close c = find_close16(str_suffix(it->rest, body_start));
    if (!c.ok) return (FmtRes){1, 0};
    Str body = str_prefix(str_suffix(it->rest, body_start), c.pos);
    size_t n = write_body(buf_suffix(out0, *written), body, s0, i0);
    if (n == (size_t)-1) return (FmtRes){1, 0};
    *written += n;
    it->rest = str_suffix(it->rest, body_start + c.pos + 1);
    it->occ++;
    return (FmtRes){0, *written};
}

__attribute__((noinline,cold)) static size_t fmt_runtime(Buf out, Str fmt, Str s0, int i0) {
    size_t p=0, w=0, occ=0;
    (void)occ;
    while (p < fmt.len) {
        if (p+2 < fmt.len && fmt.ptr[p]=='%' && fmt.ptr[p+1]=='%' && fmt.ptr[p+2]=='{') { out.ptr[w++]='%'; out.ptr[w++]='{'; p += 3; continue; }
        if (p+1 < fmt.len && fmt.ptr[p]=='%' && fmt.ptr[p+1]=='{') {
            size_t b = p+2, e=b;
            while (e < fmt.len && fmt.ptr[e] != '}') ++e;
            if (e >= fmt.len) break;
            Str body = {fmt.ptr+b, e-b};
            if (body.len == 1 && body.ptr[0] == 's') w += copy_bytes((Buf){out.ptr+w, out.len-w}, s0);
            else if (body.len == 1 && body.ptr[0] == 'i') w += write_i32((Buf){out.ptr+w, out.len-w}, i0);
            else { out.ptr[w++]='?'; }
            p = e+1; continue;
        }
        out.ptr[w++] = fmt.ptr[p++];
    }
    out.ptr[w] = 0;
    return w;
}

static inline __attribute__((always_inline)) FmtRes fmt_bounded(Buf out, Str fmt, Str s0, int i0) {
    Iter it = {fmt,0};
    size_t written = 0;
    FmtRes r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;
    r = next_or_zero(&it, out, &written, s0, i0); if (r.status) return r;

    Event more = find_event32(it.rest);
    if (more.kind == EV_PLACEHOLDER || more.kind == EV_ESCAPED || more.kind == EV_FALLBACK) return (FmtRes){1,0};
    written += copy_bytes(buf_suffix(out, written), it.rest);
    out.ptr[written] = 0;
    return (FmtRes){0,written};
}

static inline __attribute__((always_inline)) size_t fmt_print(Buf out, Str fmt, Str s0, int i0) {
    if (!__builtin_constant_p(fmt.len) || !__builtin_constant_p(fmt.ptr[0])) {
        return fmt_runtime(out, fmt, s0, i0);
    }
    FmtRes r = fmt_bounded(out, fmt, s0, i0);
    if (r.status == 0) return r.len;
    return fmt_runtime(out, fmt, s0, i0);
}

__attribute__((noinline)) void folded_cases(void) {
    char a[96] = {0}, b[96] = {0}, c[96] = {0};
    fmt_print((Buf){a,sizeof a}, str_lit("case0 {ok} %%{ %{s} %{i}", sizeof("case0 {ok} %%{ %{s} %{i}")-1), str_lit("world",5), 123);
    fmt_print((Buf){b,sizeof b}, str_lit("case1 %{s} jkl%{i}mno", sizeof("case1 %{s} jkl%{i}mno")-1), str_lit("ABC",3), -5);
    fmt_print((Buf){c,sizeof c}, str_lit("case2 %%{pre} %{s} %%{post}", sizeof("case2 %%{pre} %{s} %%{post}")-1), str_lit("X",1), 77);
    puts(a); puts(b); puts(c);
}

__attribute__((noinline)) void runtime_case(const char *p) {
    char a[160] = {0};
    fmt_print((Buf){a,sizeof a}, (Str){p, strlen(p)}, str_lit("R",1), 456);
    puts(a);
}

int main(int argc, char **argv) {
    folded_cases();
    if (argc > 1) runtime_case(argv[1]);
    return 0;
}
