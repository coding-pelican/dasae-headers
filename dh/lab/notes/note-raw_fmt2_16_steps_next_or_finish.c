#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ATTR_AI __attribute__((always_inline)) inline
#define ATTR_NOINLINE __attribute__((noinline,cold))
#define FMT_MAX_ARG_COUNT 16
#define FMT_MAX_FOLD_STEPS 16
#define FMT_MAX_SCAN_BYTES 32
#define FMT_MAX_SPEC_BYTES 32

typedef struct { char *ptr; size_t len; } Buf;
typedef struct { const char *ptr; size_t len; } Str;
#define STR_(s) ((Str){ (s), sizeof(s) - 1 })

typedef struct {
    Str rest;
    size_t occ_idx;
    int done;
} Iter;

typedef enum { EV_NONE=0, EV_PLACEHOLDER=1, EV_ESCAPED=2, EV_FALLBACK=3 } EvKind;
typedef struct { EvKind kind; size_t pos; } Event;
typedef enum { RS_OK=0, RS_FALLBACK=1, RS_ERR=2 } ResStatus;
typedef struct { ResStatus status; size_t len; } Res;

static ATTR_AI Buf buf_suffix(Buf b, size_t n) { return (Buf){ b.ptr + n, b.len - n }; }
static ATTR_AI Str str_suffix(Str s, size_t n) { return (Str){ s.ptr + n, s.len - n }; }
static ATTR_AI Str str_prefix(Str s, size_t n) { return (Str){ s.ptr, n }; }
static ATTR_AI size_t copy_bytes(Buf out, Str text) {
    for (size_t i = 0; i < text.len; ++i) out.ptr[i] = text.ptr[i];
    return text.len;
}

static ATTR_AI Event find_event_bounded(Str s) {
#define CHECK_AT(i) \
    if (s.len > (size_t)((i) + 2) && s.ptr[(i)] == '%' && s.ptr[(i)+1] == '%' && s.ptr[(i)+2] == '{') return (Event){EV_ESCAPED, (i)}; \
    if (s.len > (size_t)((i) + 1) && s.ptr[(i)] == '%' && s.ptr[(i)+1] == '{') return (Event){EV_PLACEHOLDER, (i)}
    CHECK_AT(0);  CHECK_AT(1);  CHECK_AT(2);  CHECK_AT(3);
    CHECK_AT(4);  CHECK_AT(5);  CHECK_AT(6);  CHECK_AT(7);
    CHECK_AT(8);  CHECK_AT(9);  CHECK_AT(10); CHECK_AT(11);
    CHECK_AT(12); CHECK_AT(13); CHECK_AT(14); CHECK_AT(15);
    CHECK_AT(16); CHECK_AT(17); CHECK_AT(18); CHECK_AT(19);
    CHECK_AT(20); CHECK_AT(21); CHECK_AT(22); CHECK_AT(23);
    CHECK_AT(24); CHECK_AT(25); CHECK_AT(26); CHECK_AT(27);
    CHECK_AT(28); CHECK_AT(29); CHECK_AT(30); CHECK_AT(31);
#undef CHECK_AT
    if (s.len > FMT_MAX_SCAN_BYTES) return (Event){EV_FALLBACK, 0};
    return (Event){EV_NONE, 0};
}

static ATTR_AI Res find_close_bounded(Str s) {
#define CLOSE_AT(i) if (s.len > (size_t)(i) && s.ptr[(i)] == '}') return (Res){RS_OK, (i)}
    CLOSE_AT(0);  CLOSE_AT(1);  CLOSE_AT(2);  CLOSE_AT(3);
    CLOSE_AT(4);  CLOSE_AT(5);  CLOSE_AT(6);  CLOSE_AT(7);
    CLOSE_AT(8);  CLOSE_AT(9);  CLOSE_AT(10); CLOSE_AT(11);
    CLOSE_AT(12); CLOSE_AT(13); CLOSE_AT(14); CLOSE_AT(15);
    CLOSE_AT(16); CLOSE_AT(17); CLOSE_AT(18); CLOSE_AT(19);
    CLOSE_AT(20); CLOSE_AT(21); CLOSE_AT(22); CLOSE_AT(23);
    CLOSE_AT(24); CLOSE_AT(25); CLOSE_AT(26); CLOSE_AT(27);
    CLOSE_AT(28); CLOSE_AT(29); CLOSE_AT(30); CLOSE_AT(31);
#undef CLOSE_AT
    return (s.len > FMT_MAX_SPEC_BYTES) ? (Res){RS_FALLBACK,0} : (Res){RS_ERR,0};
}

static ATTR_AI int digit16(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static ATTR_AI Res arg_idx_from_body(Str body, size_t occ_idx) {
    // Supported bounded grammar: %{s}, %{[0]s} .. %{[F]s}
    if (body.len == 1 && body.ptr[0] == 's') return (Res){RS_OK, occ_idx};
    if (body.len == 4 && body.ptr[0] == '[' && body.ptr[2] == ']' && body.ptr[3] == 's') {
        int d = digit16(body.ptr[1]);
        if (d >= 0 && d < FMT_MAX_ARG_COUNT) return (Res){RS_OK, (size_t)d};
    }
    return (Res){RS_FALLBACK, 0};
}

static ATTR_AI const Str *arg_ptr(size_t idx,
    const Str *a0,const Str *a1,const Str *a2,const Str *a3,
    const Str *a4,const Str *a5,const Str *a6,const Str *a7,
    const Str *a8,const Str *a9,const Str *a10,const Str *a11,
    const Str *a12,const Str *a13,const Str *a14,const Str *a15)
{
    switch (idx) {
    case 0: return a0;  case 1: return a1;  case 2: return a2;  case 3: return a3;
    case 4: return a4;  case 5: return a5;  case 6: return a6;  case 7: return a7;
    case 8: return a8;  case 9: return a9;  case 10: return a10; case 11: return a11;
    case 12: return a12; case 13: return a13; case 14: return a14; case 15: return a15;
    default: return a0;
    }
}

static ATTR_AI Res next_or_finish(Iter *it, Buf out,
    const Str *a0,const Str *a1,const Str *a2,const Str *a3,
    const Str *a4,const Str *a5,const Str *a6,const Str *a7,
    const Str *a8,const Str *a9,const Str *a10,const Str *a11,
    const Str *a12,const Str *a13,const Str *a14,const Str *a15)
{
    if (it->done) return (Res){RS_OK, 0};
    Event ev = find_event_bounded(it->rest);
    if (ev.kind == EV_FALLBACK) return (Res){RS_FALLBACK, 0};
    if (ev.kind == EV_NONE) {
        size_t n = copy_bytes(out, it->rest);
        it->rest = (Str){it->rest.ptr + it->rest.len, 0};
        it->done = 1;
        return (Res){RS_OK, n};
    }

    size_t written = copy_bytes(out, str_prefix(it->rest, ev.pos));
    if (ev.kind == EV_ESCAPED) {
        out.ptr[written++] = '%';
        out.ptr[written++] = '{';
        it->rest = str_suffix(it->rest, ev.pos + 3);
        if (it->rest.len == 0) it->done = 1;
        return (Res){RS_OK, written};
    }

    size_t body_start = ev.pos + 2;
    Res close = find_close_bounded(str_suffix(it->rest, body_start));
    if (close.status != RS_OK) return close;
    Str body = str_prefix(str_suffix(it->rest, body_start), close.len);
    Res idx_r = arg_idx_from_body(body, it->occ_idx);
    if (idx_r.status != RS_OK) return idx_r;
    const Str *arg = arg_ptr(idx_r.len, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
    written += copy_bytes(buf_suffix(out, written), *arg);
    it->rest = str_suffix(it->rest, body_start + close.len + 1);
    it->occ_idx++;
    if (it->rest.len == 0) it->done = 1;
    return (Res){RS_OK, written};
}

static ATTR_NOINLINE size_t fmt_runtime(Buf out, Str fmt,
    Str a0,Str a1,Str a2,Str a3,Str a4,Str a5,Str a6,Str a7,
    Str a8,Str a9,Str a10,Str a11,Str a12,Str a13,Str a14,Str a15)
{
    size_t w = 0, occ = 0;
    for (size_t i = 0; i < fmt.len;) {
        if (i + 2 < fmt.len && fmt.ptr[i] == '%' && fmt.ptr[i+1] == '%' && fmt.ptr[i+2] == '{') {
            out.ptr[w++] = '%'; out.ptr[w++] = '{'; i += 3; continue;
        }
        if (i + 1 < fmt.len && fmt.ptr[i] == '%' && fmt.ptr[i+1] == '{') {
            size_t body = i + 2;
            size_t j = body;
            while (j < fmt.len && fmt.ptr[j] != '}') ++j;
            if (j >= fmt.len) { out.ptr[w] = 0; return w; }
            size_t idx = occ;
            if (j - body == 4 && fmt.ptr[body] == '[' && fmt.ptr[body+2] == ']' && fmt.ptr[body+3] == 's') {
                int d = digit16(fmt.ptr[body+1]);
                if (d >= 0 && d < FMT_MAX_ARG_COUNT) idx = (size_t)d;
            }
            const Str *arg = arg_ptr(idx, &a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14,&a15);
            memcpy(out.ptr + w, arg->ptr, arg->len);
            w += arg->len;
            ++occ;
            i = j + 1;
            continue;
        }
        out.ptr[w++] = fmt.ptr[i++];
    }
    out.ptr[w] = 0;
    return w;
}

static ATTR_AI size_t fmt_print(Buf out, Str fmt,
    Str a0,Str a1,Str a2,Str a3,Str a4,Str a5,Str a6,Str a7,
    Str a8,Str a9,Str a10,Str a11,Str a12,Str a13,Str a14,Str a15)
{
    if (!__builtin_constant_p(fmt.len) || !(fmt.len == 0 || __builtin_constant_p(fmt.ptr[0]))) {
        return fmt_runtime(out, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
    }

    Iter it = { fmt, 0, 0 };
    size_t written = 0;
#define STEP() do { \
        Res r = next_or_finish(&it, buf_suffix(out, written), &a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14,&a15); \
        if (r.status != RS_OK) return fmt_runtime(out, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15); \
        written += r.len; \
    } while (0)
    STEP(); STEP(); STEP(); STEP();
    STEP(); STEP(); STEP(); STEP();
    STEP(); STEP(); STEP(); STEP();
    STEP(); STEP(); STEP(); STEP();
#undef STEP
    if (!it.done) {
        // Final tail-only pass: after 16 fold steps, only a no-event tail may be copied.
        // If another event remains, this callsite exceeds the fold budget and falls back.
        Event tail_ev = find_event_bounded(it.rest);
        if (tail_ev.kind != EV_NONE) {
            return fmt_runtime(out, fmt, a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
        }
        written += copy_bytes(buf_suffix(out, written), it.rest);
        it.done = 1;
    }
    out.ptr[written] = 0;
    return written;
}

__attribute__((noinline)) void folded_16(void) {
    char out[256] = {0};
    fmt_print((Buf){out, sizeof out},
        STR_("%{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s} %{s}"),
        STR_("a0"),STR_("a1"),STR_("a2"),STR_("a3"),STR_("a4"),STR_("a5"),STR_("a6"),STR_("a7"),
        STR_("a8"),STR_("a9"),STR_("aA"),STR_("aB"),STR_("aC"),STR_("aD"),STR_("aE"),STR_("aF"));
    puts(out);
}

__attribute__((noinline)) void folded_few(void) {
    char out[128] = {0};
    fmt_print((Buf){out, sizeof out}, STR_("case {ok} %%{ %{s} %{[0]s}"),
        STR_("X"),STR_("unused"),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),
        STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""));
    puts(out);
}

__attribute__((noinline)) void fallback_17(void) {
    char out[256] = {0};
    fmt_print((Buf){out, sizeof out},
        STR_("%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}%{[0]s}"),
        STR_("Z"),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),
        STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""));
    puts(out);
}

__attribute__((noinline)) void dynamic_case(const char *p) {
    char out[128] = {0};
    Str fmt = { p, strlen(p) };
    fmt_print((Buf){out, sizeof out}, fmt,
        STR_("D"),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),
        STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""),STR_(""));
    puts(out);
}

int main(int argc, char **argv) {
    folded_16();
    folded_few();
    fallback_17();
    dynamic_case(argc > 1 ? argv[1] : "dyn %{s} %%{");
    return 0;
}
