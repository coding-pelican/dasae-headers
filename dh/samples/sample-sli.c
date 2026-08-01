#include "dh-main.h"
#include "dh/io/stream.h"
#include "dh/mem/common.h"

T_use$((i32)(
    mem_asBytesS,
    mem_bytesAsS,
    mem_eql,
    mem_findFirstUnit
));

$static fn_((printI32s(S_const$i32 values))(void)) {
    for_(($s(values))(value)) {
        io_stream_print(u8_l("{:d} "), *value);
    } $end(for);
    io_stream_nl();
};

fn_((main(proc_Entry entry))(E$void) $scope) {
    let_ignore = entry;

    /* Length-inferred array literal: mutable storage for slice operations. */
    var numbers = T_a$((i32){
        1, 2, 3, 4, 5, $listed
        6, 7, 8, 9, 10 $listed
    });
    let all = A_ref$((S$i32)numbers);
    let third = S_at((all)[2]);
    let fourth = *S_at((all)[3]);
    asg_l((S_at((all)[4]))(50));

    io_stream_println(
        u8_l("third: {:d}, fourth: {:d}, length: {:uz}"),
        *third, fourth, S_len(all)
    );
    printI32s(S_slice((all)$r(2, 7)).as_const);
    printI32s(S_prefix((all)5).as_const);
    printI32s(S_suffix((all)5).as_const);

    /*
     * The literal length may be inferred (`T_l$`) or stated (`NT_l$`).
     * Sentinel discovery stays bounded and uses mem/common.h.
     */
    let sentinel_numbers = T_l$((i32){
        1, 2, 3, 4, 5, -1, 6, 7 $listed
    });
    let sentinel_idx = unwrap_(
        mem_findFirstUnit$i32(sentinel_numbers, -1)
    );
    let before_sentinel = S_prefix((sentinel_numbers)sentinel_idx);
    io_stream_println(
        u8_l("sentinel length: {:uz}, third: {:d}"),
        before_sentinel.len, *S_at((before_sentinel)[2])
    );
    printI32s(before_sentinel);

    let rgb = NT_l$((3, u16){ 0x12, 0x34, 0x56 });
    let signed_rgb = A_l$((A$$(3, i16)){ 0x12, 0x34, 0x56 });
    io_stream_println(
        u8_l("fixed literals: {:uz} u16s, {:uz} i16s"),
        rgb.len, signed_rgb.len
    );

    /* Null-terminated literals retain their sentinel in storage. */
    let greeting_z0 = u8_aZ0("Hello, world");
    let greeting = mem_spanZ0Bytes(A_ptr(greeting_z0));
    io_stream_println(
        u8_l("z0: {:s}, second: {:c}"),
        greeting, *S_at((greeting)[1])
    );

    /* A typed slice can be viewed as bytes and restored without a raw cast. */
    let bytes = mem_asBytesS$i32(all.as_const);
    let restored = mem_bytesAsS$i32(bytes);
    io_stream_println(
        u8_l("byte view: {:uz}, round trip: {:B}"),
        bytes.len, mem_eql$i32(all.as_const, restored)
    );
    return_ok({});
} $unscoped(fn);
