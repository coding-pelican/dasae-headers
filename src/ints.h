#ifndef INTS_INCLUDED
#define INTS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


bool i8_eq(i8 x, i8 y);
bool i8_ne(i8 x, i8 y);
bool i8_lt(i8 x, i8 y);
bool i8_le(i8 x, i8 y);
bool i8_gt(i8 x, i8 y);
bool i8_ge(i8 x, i8 y);

i8 i8_neg(i8 x);
i8 i8_abs(i8 x);

i8 i8_add(i8 x, i8 y);
i8 i8_sub(i8 x, i8 y);
i8 i8_mul(i8 x, i8 y);
i8 i8_div(i8 x, i8 y);
i8 i8_mod(i8 x, i8 y);

i8 i8_min(i8 x, i8 y);
i8 i8_max(i8 x, i8 y);
i8 i8_clamp(i8 x, i8 low, i8 high);
i8 i8_wrap(i8 x, i8 low, i8 high);

#define i8_limit_min (INT8_MIN)
#define i8_limit_max (INT8_MAX)

bool u8_eq(u8 x, u8 y);
bool u8_ne(u8 x, u8 y);
bool u8_lt(u8 x, u8 y);
bool u8_le(u8 x, u8 y);
bool u8_gt(u8 x, u8 y);
bool u8_ge(u8 x, u8 y);

u8 u8_add(u8 x, u8 y);
u8 u8_sub(u8 x, u8 y);
u8 u8_mul(u8 x, u8 y);
u8 u8_div(u8 x, u8 y);
u8 u8_mod(u8 x, u8 y);

u8 u8_min(u8 x, u8 y);
u8 u8_max(u8 x, u8 y);
u8 u8_clamp(u8 x, u8 low, u8 high);
u8 u8_wrap(u8 x, u8 low, u8 high);

#define u8_limit_max (UINT8_MAX)


bool i16_eq(i16 x, i16 y);
bool i16_ne(i16 x, i16 y);
bool i16_lt(i16 x, i16 y);
bool i16_le(i16 x, i16 y);
bool i16_gt(i16 x, i16 y);
bool i16_ge(i16 x, i16 y);

i16 i16_neg(i16 x);
i16 i16_abs(i16 x);

i16 i16_add(i16 x, i16 y);
i16 i16_sub(i16 x, i16 y);
i16 i16_mul(i16 x, i16 y);
i16 i16_div(i16 x, i16 y);
i16 i16_mod(i16 x, i16 y);

i16 i16_min(i16 x, i16 y);
i16 i16_max(i16 x, i16 y);
i16 i16_clamp(i16 x, i16 low, i16 high);
i16 i16_wrap(i16 x, i16 low, i16 high);

#define i16_limit_min (INT16_MIN)
#define i16_limit_max (INT16_MAX)

bool u16_eq(u16 x, u16 y);
bool u16_ne(u16 x, u16 y);
bool u16_lt(u16 x, u16 y);
bool u16_le(u16 x, u16 y);
bool u16_gt(u16 x, u16 y);
bool u16_ge(u16 x, u16 y);

u16 u16_add(u16 x, u16 y);
u16 u16_sub(u16 x, u16 y);
u16 u16_mul(u16 x, u16 y);
u16 u16_div(u16 x, u16 y);
u16 u16_mod(u16 x, u16 y);

u16 u16_min(u16 x, u16 y);
u16 u16_max(u16 x, u16 y);
u16 u16_clamp(u16 x, u16 low, u16 high);
u16 u16_wrap(u16 x, u16 low, u16 high);

#define u16_limit_max (UINT16_MAX)


bool i32_eq(i32 x, i32 y);
bool i32_ne(i32 x, i32 y);
bool i32_lt(i32 x, i32 y);
bool i32_le(i32 x, i32 y);
bool i32_gt(i32 x, i32 y);
bool i32_ge(i32 x, i32 y);

i32 i32_neg(i32 x);
i32 i32_abs(i32 x);

i32 i32_add(i32 x, i32 y);
i32 i32_sub(i32 x, i32 y);
i32 i32_mul(i32 x, i32 y);
i32 i32_div(i32 x, i32 y);
i32 i32_mod(i32 x, i32 y);

i32 i32_min(i32 x, i32 y);
i32 i32_max(i32 x, i32 y);
i32 i32_clamp(i32 x, i32 low, i32 high);
i32 i32_wrap(i32 x, i32 low, i32 high);

#define i32_limit_min (INT32_MIN)
#define i32_limit_max (INT32_MAX)

bool u32_eq(u32 x, u32 y);
bool u32_ne(u32 x, u32 y);
bool u32_lt(u32 x, u32 y);
bool u32_le(u32 x, u32 y);
bool u32_gt(u32 x, u32 y);
bool u32_ge(u32 x, u32 y);

u32 u32_add(u32 x, u32 y);
u32 u32_sub(u32 x, u32 y);
u32 u32_mul(u32 x, u32 y);
u32 u32_div(u32 x, u32 y);
u32 u32_mod(u32 x, u32 y);

u32 u32_min(u32 x, u32 y);
u32 u32_max(u32 x, u32 y);
u32 u32_clamp(u32 x, u32 low, u32 high);
u32 u32_wrap(u32 x, u32 low, u32 high);

#define u32_limit_max (UINT32_MAX)


bool i64_eq(i64 x, i64 y);
bool i64_ne(i64 x, i64 y);
bool i64_lt(i64 x, i64 y);
bool i64_le(i64 x, i64 y);
bool i64_gt(i64 x, i64 y);
bool i64_ge(i64 x, i64 y);

i64 i64_neg(i64 x);
i64 i64_abs(i64 x);

i64 i64_add(i64 x, i64 y);
i64 i64_sub(i64 x, i64 y);
i64 i64_mul(i64 x, i64 y);
i64 i64_div(i64 x, i64 y);
i64 i64_mod(i64 x, i64 y);

i64 i64_min(i64 x, i64 y);
i64 i64_max(i64 x, i64 y);
i64 i64_clamp(i64 x, i64 low, i64 high);
i64 i64_wrap(i64 x, i64 low, i64 high);

#define i64_limit_min (INT64_MIN)
#define i64_limit_max (INT64_MAX)

bool u64_eq(u64 x, u64 y);
bool u64_ne(u64 x, u64 y);
bool u64_lt(u64 x, u64 y);
bool u64_le(u64 x, u64 y);
bool u64_gt(u64 x, u64 y);
bool u64_ge(u64 x, u64 y);

u64 u64_add(u64 x, u64 y);
u64 u64_sub(u64 x, u64 y);
u64 u64_mul(u64 x, u64 y);
u64 u64_div(u64 x, u64 y);
u64 u64_mod(u64 x, u64 y);

u64 u64_min(u64 x, u64 y);
u64 u64_max(u64 x, u64 y);
u64 u64_clamp(u64 x, u64 low, u64 high);
u64 u64_wrap(u64 x, u64 low, u64 high);

#define u64_limit_max (UINT64_MAX)


bool isize_eq(isize x, isize y);
bool isize_ne(isize x, isize y);
bool isize_lt(isize x, isize y);
bool isize_le(isize x, isize y);
bool isize_gt(isize x, isize y);
bool isize_ge(isize x, isize y);

isize isize_neg(isize x);
isize isize_abs(isize x);

isize isize_add(isize x, isize y);
isize isize_sub(isize x, isize y);
isize isize_mul(isize x, isize y);
isize isize_div(isize x, isize y);
isize isize_mod(isize x, isize y);

isize isize_min(isize x, isize y);
isize isize_max(isize x, isize y);
isize isize_clamp(isize x, isize low, isize high);
isize isize_wrap(isize x, isize low, isize high);

#define isize_limit_min (INTPTR_MIN)
#define isize_limit_max (INTPTR_MAX)

bool usize_eq(usize x, usize y);
bool usize_ne(usize x, usize y);
bool usize_lt(usize x, usize y);
bool usize_le(usize x, usize y);
bool usize_gt(usize x, usize y);
bool usize_ge(usize x, usize y);

usize usize_add(usize x, usize y);
usize usize_sub(usize x, usize y);
usize usize_mul(usize x, usize y);
usize usize_div(usize x, usize y);
usize usize_mod(usize x, usize y);

usize usize_min(usize x, usize y);
usize usize_max(usize x, usize y);
usize usize_clamp(usize x, usize low, usize high);
usize usize_wrap(usize x, usize low, usize high);

#define usize_limit_max (UINTPTR_MAX)


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* INTS_INCLUDED */
