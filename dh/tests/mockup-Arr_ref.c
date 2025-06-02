#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/sli.h"

#define Arr_ref$(_Sli$T, _val_arr...) comp_op__Arr_ref$(_Sli$T, _val_arr)
#define Arr_ref(_val_arr...)          comp_op__Arr_ref(_val_arr)

#define comp_op__Arr_ref$(_Sli$T, _val_arr...) \
    ((_Sli$T){ .ptr = Arr_ptr(_val_arr), Arr_len(_val_arr) })
#define comp_op__Arr_ref(_val_arr...) \
    Arr_ref$(Sli$$(TypeOf(*Arr_ptr(_val_arr))), _val_arr)

TEST_fn_("test Arr_ref", $scope) {
    use_Arr$(10, i32);
    let arr = (Arr$10$i32){ .buf = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    let sli = Arr_ref(arr);
    try_(TEST_expect(sli.ptr == Arr_ptr(arr)));
    try_(TEST_expect(sli.len == Arr_len(arr)));
    try_(TEST_expect(Arr_getAt(arr, 0) == 1));
    try_(TEST_expect(Arr_getAt(arr, 9) == 10));
} $unscoped_TEST_fn;

#define Sli_deref$(_Arr$N$T, _val_sli...) comp_op__Sli_deref$(_Arr$N$T, _val_sli)

#define comp_op__Sli_deref$(_Arr$N$T, _val_sli...) (*eval({ \
    let __sli = _val_sli; \
    debug_assert_nonnull(__sli.ptr); \
    let __arr_ptr = as$(_Arr$N$T*, __sli.ptr); \
    debug_assert(__sli.len == Arr_len(*__arr_ptr)); \
    eval_return __arr_ptr; \
}))

#define mem_asBytes(_ptr...)                   comp_inline__mem_asBytes(_ptr)
#define mem_asBytes_const(_ptr...)             comp_inline__mem_asBytes_const(_ptr)
#define mem_toBytes$(_Arr$N$u8, _val...)       comp_inline__mem_toBytes$(pp_uniqTok(val), _Arr$N$u8, _val)
#define mem_toBytes(_val...)                   comp_inline__mem_toBytes(_val)
#define mem_toBytes_const$(_Arr$N$u8, _val...) comp_inline__mem_toBytes_const$(pp_uniqTok(val), _Arr$N$u8, _val)
#define mem_toBytes_const(_val...)             comp_inline__mem_toBytes_const(_val)

#define comp_inline__mem_asBytes(_ptr...) \
    Sli_from$(Sli$(u8), as$(u8*, _ptr), sizeOf(*_ptr))
#define comp_inline__mem_asBytes_const(_ptr...) \
    Sli_from$(Sli_const$(u8), as$(const u8*, _ptr), sizeOf(*_ptr))
#define comp_inline__mem_toBytes$(__val, _Arr$N$u8, _val...) eval({ \
    var __val = _val; \
    eval_return Sli_deref$(_Arr$N$u8, mem_asBytes(&__val)); \
})
#define comp_inline__mem_toBytes_const$(__val, _Arr$N$u8, _val...) eval({ \
    let __val = _val; \
    eval_return Sli_deref$(_Arr$N$u8, mem_asBytes_const(&__val)); \
})
#define comp_inline__mem_toBytes(_val...) \
    mem_toBytes$(Arr$$(sizeOf(_val), u8), _val)
#define comp_inline__mem_toBytes_const(_val...) \
    mem_toBytes_const$(Arr$$(sizeOf(_val), u8), _val)

static fn_(printByteBits(u8 byte), void) {
    for (var i = 0; i < 8; ++i) { printf("%d", (byte >> (7 - i)) & 1); }
}

TEST_fn_("test Sli_deref", $scope) {
    use_Arr$(10, i32);
    let arr = (Arr$10$i32){ .buf = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

    let ref_arr = Arr_ref(arr);
    try_(TEST_expect(Sli_ptr(ref_arr) == Arr_ptr(arr)));
    try_(TEST_expect(Sli_len(ref_arr) == Arr_len(arr)));
    try_(TEST_expect(Sli_getAt(ref_arr, 0) == 1));
    try_(TEST_expect(Sli_getAt(ref_arr, 9) == 10));

    var deref_sli = Sli_deref$(Arr$10$i32, ref_arr);
    try_(TEST_expect(Arr_ptr(deref_sli) != Sli_ptr(ref_arr)));
    try_(TEST_expect(Arr_len(deref_sli) == Sli_len(ref_arr)));
    try_(TEST_expect(Arr_ptr(deref_sli) != Arr_ptr(arr)));
    try_(TEST_expect(Arr_len(deref_sli) == Arr_len(arr)));
    try_(TEST_expect(Arr_getAt(deref_sli, 0) == 1));
    try_(TEST_expect(Arr_getAt(deref_sli, 9) == 10));

    let as_bytes = mem_asBytes(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Sli_getAt(as_bytes, i)); }
    printf("\n");
    let as_bytes_const = mem_asBytes_const(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Sli_getAt(as_bytes_const, i)); }
    printf("\n");
    let to_bytes = mem_toBytes(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Arr_getAt(to_bytes, i)); }
    printf("\n");
    let to_bytes_const = mem_toBytes_const(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Arr_getAt(to_bytes_const, i)); }
    printf("\n");
} $unscoped_TEST_fn;
