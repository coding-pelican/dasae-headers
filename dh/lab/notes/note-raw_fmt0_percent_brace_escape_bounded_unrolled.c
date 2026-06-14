#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Str { const char *ptr; size_t len; } Str;
typedef struct Buf { char *ptr; size_t len; } Buf;
typedef struct Iter { Str rest; } Iter;
typedef enum EventKind { EV_NONE = 0, EV_PLACEHOLDER, EV_ESC_PBRACE } EventKind;
typedef struct Event { EventKind kind; size_t pos; } Event;

static const size_t npos = (size_t)-1;

static __attribute__((always_inline)) inline Str str_suffix(Str s, size_t n) {
    return (n <= s.len) ? (Str){s.ptr + n, s.len - n} : (Str){s.ptr + s.len, 0};
}
static __attribute__((always_inline)) inline Str str_prefix(Str s, size_t n) {
    return (n <= s.len) ? (Str){s.ptr, n} : s;
}
static __attribute__((always_inline)) inline Buf buf_suffix(Buf b, size_t n) {
    return (n <= b.len) ? (Buf){b.ptr + n, b.len - n} : (Buf){b.ptr + b.len, 0};
}
static __attribute__((always_inline)) inline size_t copy_bytes(Buf out, Str s) {
    __builtin_memcpy(out.ptr, s.ptr, s.len);
    return s.len;
}

static __attribute__((always_inline)) inline Event find_event_bounded(Str s) {
    if (s.len > 2 && s.ptr[0] == '%' && s.ptr[1] == '%' && s.ptr[2] == '{') return (Event){ EV_ESC_PBRACE, 0 };
    if (s.len > 1 && s.ptr[0] == '%' && s.ptr[1] == '{') return (Event){ EV_PLACEHOLDER, 0 };
    if (s.len > 3 && s.ptr[1] == '%' && s.ptr[2] == '%' && s.ptr[3] == '{') return (Event){ EV_ESC_PBRACE, 1 };
    if (s.len > 2 && s.ptr[1] == '%' && s.ptr[2] == '{') return (Event){ EV_PLACEHOLDER, 1 };
    if (s.len > 4 && s.ptr[2] == '%' && s.ptr[3] == '%' && s.ptr[4] == '{') return (Event){ EV_ESC_PBRACE, 2 };
    if (s.len > 3 && s.ptr[2] == '%' && s.ptr[3] == '{') return (Event){ EV_PLACEHOLDER, 2 };
    if (s.len > 5 && s.ptr[3] == '%' && s.ptr[4] == '%' && s.ptr[5] == '{') return (Event){ EV_ESC_PBRACE, 3 };
    if (s.len > 4 && s.ptr[3] == '%' && s.ptr[4] == '{') return (Event){ EV_PLACEHOLDER, 3 };
    if (s.len > 6 && s.ptr[4] == '%' && s.ptr[5] == '%' && s.ptr[6] == '{') return (Event){ EV_ESC_PBRACE, 4 };
    if (s.len > 5 && s.ptr[4] == '%' && s.ptr[5] == '{') return (Event){ EV_PLACEHOLDER, 4 };
    if (s.len > 7 && s.ptr[5] == '%' && s.ptr[6] == '%' && s.ptr[7] == '{') return (Event){ EV_ESC_PBRACE, 5 };
    if (s.len > 6 && s.ptr[5] == '%' && s.ptr[6] == '{') return (Event){ EV_PLACEHOLDER, 5 };
    if (s.len > 8 && s.ptr[6] == '%' && s.ptr[7] == '%' && s.ptr[8] == '{') return (Event){ EV_ESC_PBRACE, 6 };
    if (s.len > 7 && s.ptr[6] == '%' && s.ptr[7] == '{') return (Event){ EV_PLACEHOLDER, 6 };
    if (s.len > 9 && s.ptr[7] == '%' && s.ptr[8] == '%' && s.ptr[9] == '{') return (Event){ EV_ESC_PBRACE, 7 };
    if (s.len > 8 && s.ptr[7] == '%' && s.ptr[8] == '{') return (Event){ EV_PLACEHOLDER, 7 };
    if (s.len > 10 && s.ptr[8] == '%' && s.ptr[9] == '%' && s.ptr[10] == '{') return (Event){ EV_ESC_PBRACE, 8 };
    if (s.len > 9 && s.ptr[8] == '%' && s.ptr[9] == '{') return (Event){ EV_PLACEHOLDER, 8 };
    if (s.len > 11 && s.ptr[9] == '%' && s.ptr[10] == '%' && s.ptr[11] == '{') return (Event){ EV_ESC_PBRACE, 9 };
    if (s.len > 10 && s.ptr[9] == '%' && s.ptr[10] == '{') return (Event){ EV_PLACEHOLDER, 9 };
    if (s.len > 12 && s.ptr[10] == '%' && s.ptr[11] == '%' && s.ptr[12] == '{') return (Event){ EV_ESC_PBRACE, 10 };
    if (s.len > 11 && s.ptr[10] == '%' && s.ptr[11] == '{') return (Event){ EV_PLACEHOLDER, 10 };
    if (s.len > 13 && s.ptr[11] == '%' && s.ptr[12] == '%' && s.ptr[13] == '{') return (Event){ EV_ESC_PBRACE, 11 };
    if (s.len > 12 && s.ptr[11] == '%' && s.ptr[12] == '{') return (Event){ EV_PLACEHOLDER, 11 };
    if (s.len > 14 && s.ptr[12] == '%' && s.ptr[13] == '%' && s.ptr[14] == '{') return (Event){ EV_ESC_PBRACE, 12 };
    if (s.len > 13 && s.ptr[12] == '%' && s.ptr[13] == '{') return (Event){ EV_PLACEHOLDER, 12 };
    if (s.len > 15 && s.ptr[13] == '%' && s.ptr[14] == '%' && s.ptr[15] == '{') return (Event){ EV_ESC_PBRACE, 13 };
    if (s.len > 14 && s.ptr[13] == '%' && s.ptr[14] == '{') return (Event){ EV_PLACEHOLDER, 13 };
    if (s.len > 16 && s.ptr[14] == '%' && s.ptr[15] == '%' && s.ptr[16] == '{') return (Event){ EV_ESC_PBRACE, 14 };
    if (s.len > 15 && s.ptr[14] == '%' && s.ptr[15] == '{') return (Event){ EV_PLACEHOLDER, 14 };
    if (s.len > 17 && s.ptr[15] == '%' && s.ptr[16] == '%' && s.ptr[17] == '{') return (Event){ EV_ESC_PBRACE, 15 };
    if (s.len > 16 && s.ptr[15] == '%' && s.ptr[16] == '{') return (Event){ EV_PLACEHOLDER, 15 };
    if (s.len > 18 && s.ptr[16] == '%' && s.ptr[17] == '%' && s.ptr[18] == '{') return (Event){ EV_ESC_PBRACE, 16 };
    if (s.len > 17 && s.ptr[16] == '%' && s.ptr[17] == '{') return (Event){ EV_PLACEHOLDER, 16 };
    if (s.len > 19 && s.ptr[17] == '%' && s.ptr[18] == '%' && s.ptr[19] == '{') return (Event){ EV_ESC_PBRACE, 17 };
    if (s.len > 18 && s.ptr[17] == '%' && s.ptr[18] == '{') return (Event){ EV_PLACEHOLDER, 17 };
    if (s.len > 20 && s.ptr[18] == '%' && s.ptr[19] == '%' && s.ptr[20] == '{') return (Event){ EV_ESC_PBRACE, 18 };
    if (s.len > 19 && s.ptr[18] == '%' && s.ptr[19] == '{') return (Event){ EV_PLACEHOLDER, 18 };
    if (s.len > 21 && s.ptr[19] == '%' && s.ptr[20] == '%' && s.ptr[21] == '{') return (Event){ EV_ESC_PBRACE, 19 };
    if (s.len > 20 && s.ptr[19] == '%' && s.ptr[20] == '{') return (Event){ EV_PLACEHOLDER, 19 };
    if (s.len > 22 && s.ptr[20] == '%' && s.ptr[21] == '%' && s.ptr[22] == '{') return (Event){ EV_ESC_PBRACE, 20 };
    if (s.len > 21 && s.ptr[20] == '%' && s.ptr[21] == '{') return (Event){ EV_PLACEHOLDER, 20 };
    if (s.len > 23 && s.ptr[21] == '%' && s.ptr[22] == '%' && s.ptr[23] == '{') return (Event){ EV_ESC_PBRACE, 21 };
    if (s.len > 22 && s.ptr[21] == '%' && s.ptr[22] == '{') return (Event){ EV_PLACEHOLDER, 21 };
    if (s.len > 24 && s.ptr[22] == '%' && s.ptr[23] == '%' && s.ptr[24] == '{') return (Event){ EV_ESC_PBRACE, 22 };
    if (s.len > 23 && s.ptr[22] == '%' && s.ptr[23] == '{') return (Event){ EV_PLACEHOLDER, 22 };
    if (s.len > 25 && s.ptr[23] == '%' && s.ptr[24] == '%' && s.ptr[25] == '{') return (Event){ EV_ESC_PBRACE, 23 };
    if (s.len > 24 && s.ptr[23] == '%' && s.ptr[24] == '{') return (Event){ EV_PLACEHOLDER, 23 };
    if (s.len > 26 && s.ptr[24] == '%' && s.ptr[25] == '%' && s.ptr[26] == '{') return (Event){ EV_ESC_PBRACE, 24 };
    if (s.len > 25 && s.ptr[24] == '%' && s.ptr[25] == '{') return (Event){ EV_PLACEHOLDER, 24 };
    if (s.len > 27 && s.ptr[25] == '%' && s.ptr[26] == '%' && s.ptr[27] == '{') return (Event){ EV_ESC_PBRACE, 25 };
    if (s.len > 26 && s.ptr[25] == '%' && s.ptr[26] == '{') return (Event){ EV_PLACEHOLDER, 25 };
    if (s.len > 28 && s.ptr[26] == '%' && s.ptr[27] == '%' && s.ptr[28] == '{') return (Event){ EV_ESC_PBRACE, 26 };
    if (s.len > 27 && s.ptr[26] == '%' && s.ptr[27] == '{') return (Event){ EV_PLACEHOLDER, 26 };
    if (s.len > 29 && s.ptr[27] == '%' && s.ptr[28] == '%' && s.ptr[29] == '{') return (Event){ EV_ESC_PBRACE, 27 };
    if (s.len > 28 && s.ptr[27] == '%' && s.ptr[28] == '{') return (Event){ EV_PLACEHOLDER, 27 };
    if (s.len > 30 && s.ptr[28] == '%' && s.ptr[29] == '%' && s.ptr[30] == '{') return (Event){ EV_ESC_PBRACE, 28 };
    if (s.len > 29 && s.ptr[28] == '%' && s.ptr[29] == '{') return (Event){ EV_PLACEHOLDER, 28 };
    if (s.len > 31 && s.ptr[29] == '%' && s.ptr[30] == '%' && s.ptr[31] == '{') return (Event){ EV_ESC_PBRACE, 29 };
    if (s.len > 30 && s.ptr[29] == '%' && s.ptr[30] == '{') return (Event){ EV_PLACEHOLDER, 29 };
    if (s.len > 32 && s.ptr[30] == '%' && s.ptr[31] == '%' && s.ptr[32] == '{') return (Event){ EV_ESC_PBRACE, 30 };
    if (s.len > 31 && s.ptr[30] == '%' && s.ptr[31] == '{') return (Event){ EV_PLACEHOLDER, 30 };
    if (s.len > 33 && s.ptr[31] == '%' && s.ptr[32] == '%' && s.ptr[33] == '{') return (Event){ EV_ESC_PBRACE, 31 };
    if (s.len > 32 && s.ptr[31] == '%' && s.ptr[32] == '{') return (Event){ EV_PLACEHOLDER, 31 };
    if (s.len > 34 && s.ptr[32] == '%' && s.ptr[33] == '%' && s.ptr[34] == '{') return (Event){ EV_ESC_PBRACE, 32 };
    if (s.len > 33 && s.ptr[32] == '%' && s.ptr[33] == '{') return (Event){ EV_PLACEHOLDER, 32 };
    if (s.len > 35 && s.ptr[33] == '%' && s.ptr[34] == '%' && s.ptr[35] == '{') return (Event){ EV_ESC_PBRACE, 33 };
    if (s.len > 34 && s.ptr[33] == '%' && s.ptr[34] == '{') return (Event){ EV_PLACEHOLDER, 33 };
    if (s.len > 36 && s.ptr[34] == '%' && s.ptr[35] == '%' && s.ptr[36] == '{') return (Event){ EV_ESC_PBRACE, 34 };
    if (s.len > 35 && s.ptr[34] == '%' && s.ptr[35] == '{') return (Event){ EV_PLACEHOLDER, 34 };
    if (s.len > 37 && s.ptr[35] == '%' && s.ptr[36] == '%' && s.ptr[37] == '{') return (Event){ EV_ESC_PBRACE, 35 };
    if (s.len > 36 && s.ptr[35] == '%' && s.ptr[36] == '{') return (Event){ EV_PLACEHOLDER, 35 };
    if (s.len > 38 && s.ptr[36] == '%' && s.ptr[37] == '%' && s.ptr[38] == '{') return (Event){ EV_ESC_PBRACE, 36 };
    if (s.len > 37 && s.ptr[36] == '%' && s.ptr[37] == '{') return (Event){ EV_PLACEHOLDER, 36 };
    if (s.len > 39 && s.ptr[37] == '%' && s.ptr[38] == '%' && s.ptr[39] == '{') return (Event){ EV_ESC_PBRACE, 37 };
    if (s.len > 38 && s.ptr[37] == '%' && s.ptr[38] == '{') return (Event){ EV_PLACEHOLDER, 37 };
    if (s.len > 40 && s.ptr[38] == '%' && s.ptr[39] == '%' && s.ptr[40] == '{') return (Event){ EV_ESC_PBRACE, 38 };
    if (s.len > 39 && s.ptr[38] == '%' && s.ptr[39] == '{') return (Event){ EV_PLACEHOLDER, 38 };
    if (s.len > 41 && s.ptr[39] == '%' && s.ptr[40] == '%' && s.ptr[41] == '{') return (Event){ EV_ESC_PBRACE, 39 };
    if (s.len > 40 && s.ptr[39] == '%' && s.ptr[40] == '{') return (Event){ EV_PLACEHOLDER, 39 };
    if (s.len > 42 && s.ptr[40] == '%' && s.ptr[41] == '%' && s.ptr[42] == '{') return (Event){ EV_ESC_PBRACE, 40 };
    if (s.len > 41 && s.ptr[40] == '%' && s.ptr[41] == '{') return (Event){ EV_PLACEHOLDER, 40 };
    if (s.len > 43 && s.ptr[41] == '%' && s.ptr[42] == '%' && s.ptr[43] == '{') return (Event){ EV_ESC_PBRACE, 41 };
    if (s.len > 42 && s.ptr[41] == '%' && s.ptr[42] == '{') return (Event){ EV_PLACEHOLDER, 41 };
    if (s.len > 44 && s.ptr[42] == '%' && s.ptr[43] == '%' && s.ptr[44] == '{') return (Event){ EV_ESC_PBRACE, 42 };
    if (s.len > 43 && s.ptr[42] == '%' && s.ptr[43] == '{') return (Event){ EV_PLACEHOLDER, 42 };
    if (s.len > 45 && s.ptr[43] == '%' && s.ptr[44] == '%' && s.ptr[45] == '{') return (Event){ EV_ESC_PBRACE, 43 };
    if (s.len > 44 && s.ptr[43] == '%' && s.ptr[44] == '{') return (Event){ EV_PLACEHOLDER, 43 };
    if (s.len > 46 && s.ptr[44] == '%' && s.ptr[45] == '%' && s.ptr[46] == '{') return (Event){ EV_ESC_PBRACE, 44 };
    if (s.len > 45 && s.ptr[44] == '%' && s.ptr[45] == '{') return (Event){ EV_PLACEHOLDER, 44 };
    if (s.len > 47 && s.ptr[45] == '%' && s.ptr[46] == '%' && s.ptr[47] == '{') return (Event){ EV_ESC_PBRACE, 45 };
    if (s.len > 46 && s.ptr[45] == '%' && s.ptr[46] == '{') return (Event){ EV_PLACEHOLDER, 45 };
    if (s.len > 48 && s.ptr[46] == '%' && s.ptr[47] == '%' && s.ptr[48] == '{') return (Event){ EV_ESC_PBRACE, 46 };
    if (s.len > 47 && s.ptr[46] == '%' && s.ptr[47] == '{') return (Event){ EV_PLACEHOLDER, 46 };
    if (s.len > 49 && s.ptr[47] == '%' && s.ptr[48] == '%' && s.ptr[49] == '{') return (Event){ EV_ESC_PBRACE, 47 };
    if (s.len > 48 && s.ptr[47] == '%' && s.ptr[48] == '{') return (Event){ EV_PLACEHOLDER, 47 };
    return (Event){ EV_NONE, npos };
}

static __attribute__((always_inline)) inline size_t find_close_brace_bounded(Str s) {
    if (s.len > 0 && s.ptr[0] == '}') return 0;
    if (s.len > 1 && s.ptr[1] == '}') return 1;
    if (s.len > 2 && s.ptr[2] == '}') return 2;
    if (s.len > 3 && s.ptr[3] == '}') return 3;
    if (s.len > 4 && s.ptr[4] == '}') return 4;
    if (s.len > 5 && s.ptr[5] == '}') return 5;
    if (s.len > 6 && s.ptr[6] == '}') return 6;
    if (s.len > 7 && s.ptr[7] == '}') return 7;
    if (s.len > 8 && s.ptr[8] == '}') return 8;
    if (s.len > 9 && s.ptr[9] == '}') return 9;
    if (s.len > 10 && s.ptr[10] == '}') return 10;
    if (s.len > 11 && s.ptr[11] == '}') return 11;
    if (s.len > 12 && s.ptr[12] == '}') return 12;
    if (s.len > 13 && s.ptr[13] == '}') return 13;
    if (s.len > 14 && s.ptr[14] == '}') return 14;
    if (s.len > 15 && s.ptr[15] == '}') return 15;
    return npos;
}

static __attribute__((always_inline)) inline size_t write_placeholder(Buf out, Str spec, Str value) {
    if (spec.len != 1) __builtin_unreachable();
    if (!(spec.ptr[0] == '*' || spec.ptr[0] == 's')) __builtin_unreachable();
    return copy_bytes(out, value);
}

static __attribute__((always_inline)) inline size_t write_next(Iter *it, Buf out, Str value) {
    Event ev = find_event_bounded(it->rest);
    if (ev.kind == EV_NONE) return 0;
    size_t written = copy_bytes(out, str_prefix(it->rest, ev.pos));
    if (ev.kind == EV_ESC_PBRACE) {
        out.ptr[written++] = '%';
        out.ptr[written++] = '{';
        it->rest = str_suffix(it->rest, ev.pos + 3);
        return written;
    }
    size_t spec_start = ev.pos + 2;
    Str after_open = str_suffix(it->rest, spec_start);
    size_t close = find_close_brace_bounded(after_open);
    if (close == npos) __builtin_unreachable();
    Str spec = str_prefix(after_open, close);
    written += write_placeholder(buf_suffix(out, written), spec, value);
    it->rest = str_suffix(it->rest, spec_start + close + 1);
    return written;
}

static __attribute__((always_inline)) inline size_t count_next(Iter *it) {
    Event ev = find_event_bounded(it->rest);
    if (ev.kind == EV_NONE) return 0;
    if (ev.kind == EV_ESC_PBRACE) {
        it->rest = str_suffix(it->rest, ev.pos + 3);
        return 1;
    }
    size_t spec_start = ev.pos + 2;
    Str after_open = str_suffix(it->rest, spec_start);
    size_t close = find_close_brace_bounded(after_open);
    if (close == npos) __builtin_unreachable();
    it->rest = str_suffix(it->rest, spec_start + close + 1);
    return 1;
}

static __attribute__((always_inline)) inline size_t count_events_bounded(Str fmt) {
    Iter it = { fmt };
    size_t count = 0;
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    count += count_next(&it);
    return count;
}

static __attribute__((always_inline)) inline Str fmt_percent_brace_escape_bounded(Buf out0, Str fmt, Str value) {
    size_t n = count_events_bounded(fmt);
    Iter it = { fmt };
    Buf out = out0;
    if (0 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (1 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (2 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (3 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (4 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (5 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (6 < n) out = buf_suffix(out, write_next(&it, out, value));
    if (7 < n) out = buf_suffix(out, write_next(&it, out, value));
    out = buf_suffix(out, copy_bytes(out, it.rest));
    if (out.len == 0) __builtin_unreachable();
    out.ptr[0] = '\0';
    return (Str){ out0.ptr, (size_t)(out.ptr - out0.ptr) };
}

int main(void) {
    char m0[128] = {0};
    char m1[128] = {0};
    char m2[128] = {0};
    char m3[128] = {0};
    char m4[128] = {0};
    Str s0 = fmt_percent_brace_escape_bounded((Buf){m0, sizeof m0}, (Str){"case0: literal ok abc", sizeof("case0: literal ok abc") - 1}, (Str){"ABC", 3});
    Str s1 = fmt_percent_brace_escape_bounded((Buf){m1, sizeof m1}, (Str){"case1: %{s} ok", sizeof("case1: %{s} ok") - 1}, (Str){"ABC", 3});
    Str s2 = fmt_percent_brace_escape_bounded((Buf){m2, sizeof m2}, (Str){"case2: literal %%{ token", sizeof("case2: literal %%{ token") - 1}, (Str){"ABC", 3});
    Str s3 = fmt_percent_brace_escape_bounded((Buf){m3, sizeof m3}, (Str){"case3: %%{pre} %{s} %%{post}", sizeof("case3: %%{pre} %{s} %%{post}") - 1}, (Str){"ABC", 3});
    Str s4 = fmt_percent_brace_escape_bounded((Buf){m4, sizeof m4}, (Str){"case4: %%{ %{*} %%{", sizeof("case4: %%{ %{*} %%{") - 1}, (Str){"ABC", 3});
    puts(s0.ptr);
    puts(s1.ptr);
    puts(s2.ptr);
    puts(s3.ptr);
    puts(s4.ptr);
    return (int)(s0.len + s1.len + s2.len + s3.len + s4.len == 0);
}
