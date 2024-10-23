#include "ints.h"


usize usize_wrap(usize x, usize low, usize high) {
    if (low >= high) {
        return low;
    }
    usize range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

usize usize_clamp(usize x, usize low, usize high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

usize usize_max(usize x, usize y) {
    return x > y ? x : y;
}

usize usize_min(usize x, usize y) {
    return x < y ? x : y;
}

isize isize_wrap(isize x, isize low, isize high) {
    if (low >= high) {
        return low;
    }
    isize range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

isize isize_clamp(isize x, isize low, isize high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

isize isize_max(isize x, isize y) {
    return x > y ? x : y;
}

isize isize_min(isize x, isize y) {
    return x < y ? x : y;
}

u64 u64_wrap(u64 x, u64 low, u64 high) {
    if (low >= high) {
        return low;
    }
    u64 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

u64 u64_clamp(u64 x, u64 low, u64 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

u64 u64_max(u64 x, u64 y) {
    return x > y ? x : y;
}

u64 u64_min(u64 x, u64 y) {
    return x < y ? x : y;
}

i64 i64_wrap(i64 x, i64 low, i64 high) {
    if (low >= high) {
        return low;
    }
    i64 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

i64 i64_clamp(i64 x, i64 low, i64 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

i64 i64_max(i64 x, i64 y) {
    return x > y ? x : y;
}

i64 i64_min(i64 x, i64 y) {
    return x < y ? x : y;
}

u32 u32_wrap(u32 x, u32 low, u32 high) {
    if (low >= high) {
        return low;
    }
    u32 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

u32 u32_clamp(u32 x, u32 low, u32 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

u32 u32_max(u32 x, u32 y) {
    return x > y ? x : y;
}

u32 u32_min(u32 x, u32 y) {
    return x < y ? x : y;
}

i32 i32_wrap(i32 x, i32 low, i32 high) {
    if (low >= high) {
        return low;
    }
    i32 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

i32 i32_clamp(i32 x, i32 low, i32 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

i32 i32_max(i32 x, i32 y) {
    return x > y ? x : y;
}

i32 i32_min(i32 x, i32 y) {
    return x < y ? x : y;
}

u16 u16_wrap(u16 x, u16 low, u16 high) {
    if (low >= high) {
        return low;
    }
    u16 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

u16 u16_clamp(u16 x, u16 low, u16 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

u16 u16_max(u16 x, u16 y) {
    return x > y ? x : y;
}

u16 u16_min(u16 x, u16 y) {
    return x < y ? x : y;
}

i16 i16_wrap(i16 x, i16 low, i16 high) {
    if (low >= high) {
        return low;
    }
    i16 range = (i16)(high - low);
    if (x < low) {
        x = (i16)(high - ((low - x - 1) % range) - 1);
    } else {
        x = (i16)(low + ((x - low) % range));
    }
    return x;
}

i16 i16_clamp(i16 x, i16 low, i16 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

i16 i16_max(i16 x, i16 y) {
    return (i16)(x > y ? x : y);
}

i16 i16_min(i16 x, i16 y) {
    return (i16)(x < y ? x : y);
}

u8 u8_wrap(u8 x, u8 low, u8 high) {
    if (low >= high) {
        return low;
    }
    u8 range = high - low;
    if (x < low) {
        x = high - ((low - x - 1) % range) - 1;
    } else {
        x = low + ((x - low) % range);
    }
    return x;
}

u8 u8_clamp(u8 x, u8 low, u8 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

u8 u8_max(u8 x, u8 y) {
    return x > y ? x : y;
}

u8 u8_min(u8 x, u8 y) {
    return x < y ? x : y;
}

i8 i8_wrap(i8 x, i8 low, i8 high) {
    if (low >= high) {
        return low;
    }
    i8 range = (i8)(high - low);
    if (x < low) {
        x = (i8)(high - ((low - x - 1) % range) - 1);
    } else {
        x = (i8)(low + ((x - low) % range));
    }
    return x;
}

i8 i8_clamp(i8 x, i8 low, i8 high) {
    if (x < low) {
        return low;
    }
    if (x > high) {
        return high;
    }
    return x;
}

i8 i8_max(i8 x, i8 y) {
    return (i8)(x > y ? x : y);
}

i8 i8_min(i8 x, i8 y) {
    return (i8)(x < y ? x : y);
}
