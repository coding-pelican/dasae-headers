#include <stddef.h>
#if defined(__has_include) && __has_include(<stdnullptr.h>)
#include <stdnullptr.h>
#endif /* defined(__has_include) && __has_include(<stdnullptr.h>) */
#if defined(__has_include) && __has_include(<stdbool.h>)
#include <stdbool.h>
#endif /* defined(__has_include) && __has_include(<stdbool.h>) */
#include <iso646.h>

#include <stdint.h>
#include <limits.h>
#include <float.h>

#if defined(__has_include) && __has_include(<stdbit.h>)
#include <stdbit.h>
#endif /* defined(__has_include) && __has_include(<stdbit.h>) */
#if defined(__has_include) && __has_include(<stdckdint.h>)
#include <stdckdint.h>
#endif /* defined(__has_include) && __has_include(<stdckdint.h>) */
#if defined(__has_include) && __has_include(<stdcountof.h>)
#include <stdcountof.h>
#endif /* defined(__has_include) && __has_include(<stdcountof.h>) */

#include <stdalign.h>
#include <stdnoreturn.h>

#include <stdarg.h>
#if !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#endif /* !defined(__STDC_NO_ATOMICS__) */

#include <stdio.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t usize;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t isize;
typedef float f32;
typedef double f64;

typedef struct Void {
} Void;

extern usize Thrd_currentId(void);

struct fn__ScopeCounter {
    bool is_returning;
    u32 curr_line;
};
static void report(const char* label, const char* fmt, ...) {
    do {} while (0)
    _Alignas(_Generic(((__typeof__(void*))0), __typeof__(const void*): (((usize)(1))), __typeof__(void*): (((usize)(1))), default: (usize)((_Alignof(void))))) volatile union {
        __typeof__(u8[_Generic(((__typeof__(void*))0), __typeof__(const void*): (((usize)(0))), __typeof__(void*): (((usize)(0))), default: (usize)(sizeof(void)))]) val;
    } __reserved_buf = { .val = {} };
    __auto_type const __reserved_return = ((void*)(((&*((__reserved_buf).val)))));
    __attribute__((unused)) typedef __typeof__(*__reserved_return) ReturnType;
    __attribute__((unused)) typedef ReturnType ReturnT;
    __auto_type __scope_counter = (struct fn__ScopeCounter){ .is_returning = ((bool)(0)), .curr_line = 315 };
    if (((bool)(0))) {
__step_return:
        __scope_counter.is_returning = ((bool)(1));
        goto __step_deferred;
    }
__step_deferred:
    switch (__scope_counter.curr_line) {
    default: {
        goto __step_unscope;
    } break;
    case 315:
        __scope_counter.curr_line = 315 - 1;
        {
            printf("[ThrdId(%zu): %s] ", Thrd_currentId(), label);
            {
                do {
                    {
                        const u32 __scope_counter_prev_line = __scope_counter.curr_line;
                        __scope_counter.curr_line = 339;
                        if (((bool)(0))) {
                        case 339:
                            __scope_counter.curr_line = __scope_counter_prev_line;
                            goto __339_31_snapshot_reserved;
__339_32_snapshot_deferred:
                            if (__scope_counter.is_returning) {
                                goto __step_deferred;
                            } else {
                                continue;
                            };
                        }
                    };
                    __auto_type args = (((struct { va_list val; }){}).val);
                    __builtin_va_start(args, fmt);
                    if (((bool)(0))) {
__339_31_snapshot_reserved:
                        __builtin_va_end(args);
                        goto __339_32_snapshot_deferred;
                    };
                    do {
                        ;
                        {
                            vprintf(fmt, args);
                        };
                    } while (((bool)(0)));
                    goto __step_deferred;
                } while (((bool)(0)));
            };
            printf("\n");
        }
        break;
    }
    _Generic(((__typeof__(ReturnT*))0), __typeof__(const void*): ({ goto __step_return; }), __typeof__(void*): ({ goto __step_return; }), __typeof__(const Void*): ({ goto __step_return; }), __typeof__(Void*): ({ goto __step_return; }), default: ({}));
    if (((bool)(0))) {
__step_unscope:
        if (_Generic(((__typeof__(ReturnT*))0), __typeof__(const void*): (((bool)(0))), __typeof__(void*): (((bool)(0))), default: ((bool)(1)))) { return (__reserved_return[0]); };
    }
};

static void reportRaw(const char* label, const char* fmt, ...) {
    printf("[ThrdId(%zu): %s] ", Thrd_currentId(), label);
    __auto_type args = (((struct { va_list val; }){}).val);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
};

static void reportFlat(const char* label, const char* fmt, ...) {
    __attribute__((unused)) struct fn__ScopeCounter __scope_counter = { .is_returning = ((bool)(0)), .curr_line = 320 };
__step_deferred:
    switch (__scope_counter.curr_line) {
    case 0: return;
    case 320:
        __scope_counter.curr_line = 0;
        {
            printf("[ThrdId(%zu): %s] ", Thrd_currentId(), label);
            __auto_type args = (((struct { va_list val; }){}).val);
            __builtin_va_start(args, fmt);
            {
                const u32 __scope_counter_prev_line = __scope_counter.curr_line;
                __scope_counter.curr_line = 346;
                if (((bool)(0))) {
                case 346:
                    __scope_counter.curr_line = __scope_counter_prev_line;
                    __builtin_va_end(args);
                    goto __step_deferred;
                }
            };
            vprintf(fmt, args);
            printf("\n");
        }
        __scope_counter.is_returning = ((bool)(1));
        goto __step_deferred;
    }
};
