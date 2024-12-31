#include "dh/core.h"
#include "dh/scope.h"
#include "dh/defer.h"
#include "dh/claim.h"
#include "dh/mem.h"
#include "dh/time.h"
#include "dh/ds/Vec.h"
#include "../../dh-terminal/canvas.h"

typedef enum ErrorType {
    Error_None = 0,
    Error_OutOfMemory
} ErrorType;

typedef struct Error {
    ErrorType type;
    union {
        anyptr raw;
        Ptr    ptr;
        Slice  slice;
    };
} Error;

#define pp_use(...) __VA_ARGS__

// try operator for values
#define tryPtr(var, expr)                                                \
    var;                                                                 \
    scope_if(Error _result = pp_use(expr), _result.type != Error_None) { \
    }                                                                    \
    else {                                                               \
        pp_use(var) = _result.ptr;                                       \
    }

#define trySlice(var, expr)                                              \
    var;                                                                 \
    scope_if(Error _result = pp_use(expr), _result.type != Error_None) { \
    }                                                                    \
    else {                                                               \
        pp_use(var) = _result.slice;                                     \
    }

typedef Color Pixel;
// Error mem_allocSize(const mem_Allocator*, usize, usize, Slice*);


int main(int argc, const char* argv[]) {
    unused(argc), unused(argv);

    scope_defer {

        const mem_Allocator* const gpa = &mem_general_purpose;
        claim_assert_nonnull(gpa);
        Ptr(Canvas) p_canvas = tryPtr(
            p_canvas,
            mem_create(gpa, Canvas, create(Ptr))
        );
        claim_assert_true(!Ptr_isNull(p_canvas));
        defer(mem_destroy(gpa, p_canvas));

        scope_with(Canvas* const c = Ptr_cast(p_canvas, Canvas)) {
            claim_assert_nonnull(c);

            Canvas_init(c, 80, 50);
            defer(Canvas_fini(c));

            Slice(Pixel) pixels = trySlice(
                pixels,
                mem_alloc(gpa, Pixel, 80 * 50, create(Slice))
            );
            defer(mem_free(gpa, pixels));

            Canvas_setBuffer(c, pixels);
            Canvas_fill(c, Color_white);
            Canvas_render(c);

            time_SysTime_sleep(
                time_Duration_mul(time_Duration_second, 5)
            );
            Canvas_drawSprite(c, pacman);

            ds_Vec(Pixel) unique_pixels = make(ds_Vec);
            ds_Vec_findStoreUniques(
                Canvas_data(c),
                ds_Vec_init(&unique_pixels, Pixel, gpa)
            );
            defer(ds_Vec_fini(&unique_pixels));

            foreach (const Pixel, pixel, &unique_pixels) {
                fmt_print("{:s} ", Pixel_toStr(pixel));
            }
            fmt_print("\n");
        }
    }
    scope_deferred();
    return 0;
}
