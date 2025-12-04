#include "dh/main.h"

#define int_sq_static(_n...) \
    ((_n) * (_n))

/* clang-format off */
#define uint_sqrt_static(_n...) \
    /* \
     * calculate approximate square root of `_n` \
     * \
     * if `_n` <= `1`, return `_n` for simplification \
     * otherwise, return result of Newton-Raphson method \
     * \
     * implementation is overflow-safe \
     */ ( \
    (_n) <= 1     ? (_n) : \
    (_n) < 256    ? __uint_sqrt_static__small(_n) : \
    (_n) < 65536  ? __uint_sqrt_static__medium(_n) : \
    __uint_sqrt_static__large(_n) \
)
#define __uint_sqrt_static__small(_n...) \
    /* small value (0-255): lookup table - 0 iterations */ ( \
    (_n) < 4   ? 1  : (_n) < 9   ? 2  : (_n) < 16  ? 3  : \
    (_n) < 25  ? 4  : (_n) < 36  ? 5  : (_n) < 49  ? 6  : \
    (_n) < 64  ? 7  : (_n) < 81  ? 8  : (_n) < 100 ? 9  : \
    (_n) < 121 ? 10 : (_n) < 144 ? 11 : (_n) < 169 ? 12 : \
    (_n) < 196 ? 13 : (_n) < 225 ? 14 : (_n) < 256 ? 15 : 16 \
)
#define __uint_sqrt_static__medium(_n...) \
    /* medium value (256-65535): 3 iterations */ \
    __uint_sqrt_static__iter3((_n), __uint_sqrt_static__guess_medium(_n))
#define __uint_sqrt_static__guess_medium(_n...) ( \
    (_n) >= (1u << 14) ? (1u << 7) : \
    (_n) >= (1u << 12) ? (1u << 6) : \
    (_n) >= (1u << 10) ? (1u << 5) : \
    (1u << 4) \
)
#define __uint_sqrt_static__iter3(_n, _g...) \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, _g)))
#define __uint_sqrt_static__large(_n...) \
    /* large value (65536+): 5 iterations + precise initial value */ \
    __uint_sqrt_static__iter5((_n), __uint_sqrt_static__guess_large(_n))
#define __uint_sqrt_static__guess_large(_n...) ( \
    (_n) >= (1ull << 56) ? (1ull << 28) : \
    (_n) >= (1ull << 52) ? (1ull << 26) : \
    (_n) >= (1ull << 48) ? (1ull << 24) : \
    (_n) >= (1ull << 44) ? (1ull << 22) : \
    (_n) >= (1ull << 40) ? (1ull << 20) : \
    (_n) >= (1ull << 36) ? (1ull << 18) : \
    (_n) >= (1ull << 32) ? (1ull << 16) : \
    (_n) >= (1ull << 28) ? (1ull << 14) : \
    (_n) >= (1ull << 24) ? (1ull << 12) : \
    (_n) >= (1ull << 20) ? (1ull << 10) : \
    (1ull << 8) \
)
#define __uint_sqrt_static__iter5(_n, _g...) \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, \
    __uint_sqrt_static__iter(_n, _g)))))
#define __uint_sqrt_static__iter(_n, _g...) \
    /* \
     * ignition step of Newton-Raphson method for square root approximation \
     * formula: `x_next = (x + n/x) / 2` \
     * \
     * `_n`: number to find square root of (as `n` in formula) \
     * `_g`: current guess (as `x` in formula) \
     * return: next guess (as `x_next` in formula) \
     * \
     * if `_g` == `0`, return `1` for division-by-zero prevention \
     * otherwise, return `(_g + _n/_g) / 2` as next guess \
     */ \
    ((_g) == 0 ? 1 : ((_g) / 2 + (_n) / (2 * (_g))))
/* clang-format on */

#define int_cb_static(_n...) \
    ((_n) * (_n) * (_n))
/* clang-format off */
#define uint_cbrt_static(_n...) \
    /* \
     * calculate approximate cube root of `_n` \
     * \
     * if `_n` <= `1`, return `_n` for simplification \
     * otherwise, return result of Newton-Raphson method \
     * \
     * implementation is overflow-safe \
     */ ( \
    (_n) <= 1     ? (_n) : \
    (_n) < 512    ? __uint_cbrt_static__small(_n) : \
    (_n) < 262144 ? __uint_cbrt_static__medium(_n) : \
    __uint_cbrt_static__large(_n) \
)
#define __uint_cbrt_static__small(_n...) \
    /* small value (0-511): lookup table - 0 iterations */ ( \
    (_n) < 8   ? 1 : (_n) < 27  ? 2 : (_n) < 64  ? 3 : \
    (_n) < 125 ? 4 : (_n) < 216 ? 5 : (_n) < 343 ? 6 : \
    (_n) < 512 ? 7 : 8 \
)
#define __uint_cbrt_static__medium(_n...) \
    /* medium value (512-262143): 3 iterations */ \
    __uint_cbrt_static__iter3((_n), __uint_cbrt_static__guess_medium(_n))
#define __uint_cbrt_static__guess_medium(_n...) ( \
    (_n) >= (1u << 15) ? (1u << 5) : \
    (_n) >= (1u << 12) ? (1u << 4) : \
    (_n) >= (1u << 9)  ? (1u << 3) : \
    (1u << 2) \
)
#define __uint_cbrt_static__iter3(_n, _g...) \
    __uint_cbrt_static__iter(_n, \
    __uint_cbrt_static__iter(_n, \
    __uint_cbrt_static__iter(_n, _g)))
#define __uint_cbrt_static__large(_n...) \
    /* large value (262144+): 4 iterations + precise initial value */ \
    __uint_cbrt_static__iter4((_n), __uint_cbrt_static__guess_large(_n))
#define __uint_cbrt_static__guess_large(_n...) ( \
    (_n) >= (1ull << 54) ? (1ull << 18) : \
    (_n) >= (1ull << 48) ? (1ull << 16) : \
    (_n) >= (1ull << 42) ? (1ull << 14) : \
    (_n) >= (1ull << 36) ? (1ull << 12) : \
    (_n) >= (1ull << 30) ? (1ull << 10) : \
    (_n) >= (1ull << 24) ? (1ull << 8)  : \
    (_n) >= (1ull << 21) ? (1ull << 7)  : \
    (1ull << 6) \
)
#define __uint_cbrt_static__iter4(_n, _g...) \
    __uint_cbrt_static__iter(_n, \
    __uint_cbrt_static__iter(_n, \
    __uint_cbrt_static__iter(_n, \
    __uint_cbrt_static__iter(_n, _g))))
#define __uint_cbrt_static__iter(_n, _g...) \
    /* \
     * ignition step of Newton-Raphson method for cube root approximation \
     * formula: `x_next = (2*x + n/(x*x)) / 3` \
     * \
     * `_n`: number to find cube root of (as `n` in formula) \
     * `_g`: current guess (as `x` in formula) \
     * return: next guess (as `x_next` in formula) \
     * \
     * if `_g` == `0`, return `1` for division-by-zero prevention \
     * otherwise, return `(2*_g + _n/(_g*_g)) / 3` as next guess \
     */ \
    ((_g) == 0 ? 1 : ((2 * (_g)) / 3 + (_n) / (3 * (_g) * (_g))))
/* clang-format on */

#define int_hypotSq_static(_x, _y...) \
    (int_sq_static(_x) + int_sq_static(_y))
#define uint_hypot_static(_x, _y...) \
    uint_sqrt_static(int_hypotSq_static(_x, _y))

#include "dh/io/stream.h"

// Static 변수 초기화
static const u32 val_small = uint_sqrt_static(100);     // 10
static const u32 val_large = uint_sqrt_static(1234567); // 1111 (실제: 1111.11)
static const u32 val_prime = uint_sqrt_static(30);      // 5 (실제: 5.47 -> 정수버림 5)
static const u32 val_27 = uint_cbrt_static(27);

fn_((someFunc(void))(void)) {
    io_stream_println(u8_l("SQRT(100)     = {:u}"), val_small);
    io_stream_println(u8_l("SQRT(1234567) = {:u}"), val_large);
    io_stream_println(u8_l("SQRT(30)      = {:u}"), val_prime);
    io_stream_println(u8_l("CBRT(27)      = {:u}"), val_27);

    // 배열 크기 선언
    A$$(uint_sqrt_static(1024), u8) buffer = A_zero(); // `A$$(32, u8)`과 동일
    io_stream_println(u8_l("Buffer length = {:u}"), A_len(buffer));
}
