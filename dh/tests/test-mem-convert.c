#include "dh-main.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_bytesAsS,
    mem_bytesAsMutS
));

TEST_fn_("mem: convert - typed wrappers instantiate over bytes" $scope) {
    let typed_const = mem_bytesAsS$u8(u8_l("xy"));
    try_(TEST_expect(mem_eqlBytes(typed_const, u8_l("xy"))));

    var bytes = u8_a("pq");
    let typed_mut = mem_bytesAsMutS$u8(A_ref$((S$u8)(bytes)));
    try_(TEST_expect(mem_eqlBytes(typed_mut.as_const, u8_l("pq"))));
} $unscoped(TEST_fn)

TEST_fn_("mem: convert - byte conversions canonicalize empty slices" $scope) {
    let u8_type = typeInfo$(u8);
    let empty_bytes = u_castS$((S$u8)(u_allocA(0, u8_type).ref));

    let typed_const = mem_bytesAsS$u8(empty_bytes.as_const);
    try_(TEST_expect(typed_const.len == 0));
    try_(TEST_expect(ptrToInt(typed_const.ptr) == mem_emptyAddr(u8_type.log2_align)));

    let typed_mut = mem_bytesAsMutS$u8(empty_bytes);
    try_(TEST_expect(typed_mut.len == 0));
    try_(TEST_expect(ptrToInt(typed_mut.ptr) == mem_emptyAddr(u8_type.log2_align)));

    let void_type = typeInfo$(Void);
    let empty_as_bytes = mem_asBytesS(mem_bytesAsS(void_type, u8_l("x")));
    try_(TEST_expect(empty_as_bytes.len == 0));
    try_(TEST_expect(ptrToInt(empty_as_bytes.ptr) == mem_emptyAddr(void_type.log2_align)));
} $unscoped(TEST_fn)
