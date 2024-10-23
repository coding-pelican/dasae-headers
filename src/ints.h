#ifndef INTS_INCLUDED
#define INTS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


i8 i8_min(i8 x, i8 y);
i8 i8_max(i8 x, i8 y);
i8 i8_clamp(i8 x, i8 low, i8 high);
i8 i8_wrap(i8 x, i8 low, i8 high);

#define i8_minLimit (INT8_MIN)
#define i8_maxLimit (INT8_MAX)


u8 u8_min(u8 x, u8 y);
u8 u8_max(u8 x, u8 y);
u8 u8_clamp(u8 x, u8 low, u8 high);
u8 u8_wrap(u8 x, u8 low, u8 high);

#define u8_maxLimit (UINT8_MAX)


i16 i16_min(i16 x, i16 y);
i16 i16_max(i16 x, i16 y);
i16 i16_clamp(i16 x, i16 low, i16 high);
i16 i16_wrap(i16 x, i16 low, i16 high);

#define i16_minLimit (INT16_MIN)
#define i16_maxLimit (INT16_MAX)


u16 u16_min(u16 x, u16 y);
u16 u16_max(u16 x, u16 y);
u16 u16_clamp(u16 x, u16 low, u16 high);
u16 u16_wrap(u16 x, u16 low, u16 high);

#define u16_maxLimit (UINT16_MAX)


i32 i32_min(i32 x, i32 y);
i32 i32_max(i32 x, i32 y);
i32 i32_clamp(i32 x, i32 low, i32 high);
i32 i32_wrap(i32 x, i32 low, i32 high);

#define i32_minLimit (INT32_MIN)
#define i32_maxLimit (INT32_MAX)


u32 u32_min(u32 x, u32 y);
u32 u32_max(u32 x, u32 y);
u32 u32_clamp(u32 x, u32 low, u32 high);
u32 u32_wrap(u32 x, u32 low, u32 high);

#define u32_maxLimit (UINT32_MAX)


i64 i64_min(i64 x, i64 y);
i64 i64_max(i64 x, i64 y);
i64 i64_clamp(i64 x, i64 low, i64 high);
i64 i64_wrap(i64 x, i64 low, i64 high);

#define i64_minLimit (INT64_MIN)
#define i64_maxLimit (INT64_MAX)


u64 u64_min(u64 x, u64 y);
u64 u64_max(u64 x, u64 y);
u64 u64_clamp(u64 x, u64 low, u64 high);
u64 u64_wrap(u64 x, u64 low, u64 high);

#define u64_maxLimit (UINT64_MAX)


isize isize_min(isize x, isize y);
isize isize_max(isize x, isize y);
isize isize_clamp(isize x, isize low, isize high);
isize isize_wrap(isize x, isize low, isize high);

#define isize_minLimit (INTPTR_MIN)
#define isize_maxLimit (INTPTR_MAX)


usize usize_min(usize x, usize y);
usize usize_max(usize x, usize y);
usize usize_clamp(usize x, usize low, usize high);
usize usize_wrap(usize x, usize low, usize high);

#define usize_maxLimit (UINTPTR_MAX)

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* INTS_INCLUDED */
