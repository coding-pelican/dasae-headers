#include "dh/main.h"

typedef struct m_OutVal {
    TypeInfo info;
    u8       bytes[];
} m_OutVal;

#define typeUnit$(_T) ((union { \
    m_OutVal base[1]; \
    struct { \
        TypeInfo info; \
        _T       value; \
    }; \
}){ .info = typeInfo$(_T) })


fn_((someNeededSameStackMemFn(m_OutVal* out_val))(m_OutVal*)) {
    struct {
        TypeInfo info;
        u8*      bytes;
    } value = { .info = out_val->info, .bytes = bti_alloca(out_val->info.size) };
    bti_memset(value.bytes, 0xAA, out_val->info.size);
    bti_memcpy(out_val->bytes, value.bytes, out_val->info.size);
    return out_val;
}

TEST_fn_("out_mem: test" $scope) {
    // as$((typeUnit$(i32)*)(someNeededSameStackMemFn(typeUnit$(i32).base)))->value;
    //
    // i32 a = castOut$((i32)(type, someNeededSameStackMemFn(type)));

    let a = ({
        var type = typeUnit$(i32).base;
        as$((TypeOf(typeUnit$(i32))*)(someNeededSameStackMemFn(type)))->value;
    });
    try_(TEST_expect(a == as$((i32)(0xAAAAAAAA))));
} $unscoped_TEST_fn;
