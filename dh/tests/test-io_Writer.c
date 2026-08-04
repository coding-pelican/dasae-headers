#include "dh-main.h"
#include "dh/io.h"
#include "dh/mem/common.h"

T_alias$((test_io_ChunkWriter)(struct test_io_ChunkWriter {
    var_(out, S$u8);
    var_(pos, usize);
    var_(chunk_len, usize);
}));

$static fn_((test_io_ChunkWriter_write(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let self = ptrAlignCast$((test_io_ChunkWriter*)(ctx));
    if (bytes.len == 0) return_ok(0);
    if (self->pos == self->out.len) return_err(E_cause$TooSmallBuffer());

    let available = self->out.len - self->pos;
    let to_write = pri_min(pri_min(available, bytes.len), self->chunk_len);
    mem_copyBytes(
        S_slice((self->out)$r(self->pos, self->pos + to_write)),
        S_prefix((bytes)(to_write))
    );
    self->pos += to_write;
    return_ok(to_write);
} $unscoped(fn);

$static fn_((test_io_ChunkWriter_writer(test_io_ChunkWriter* self))(io_Writer)) {
    return (io_Writer){
        .ctx = ptrCast$((P$raw)(self)),
        .writeFn = test_io_ChunkWriter_write,
    };
};

$static fn_((test_io_Writer__zeroWrite(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    *P_at((calls)[0]) += 1;
    if (*P_at((calls)[0]) != 1) return_ok(ensureNonnullS(bytes).len);
    return_ok(0);
} $unscoped(fn);

TEST_fn_("io/Writer: noop consumes every discarded byte" $scope) {
    let written = try_(io_Writer_write(io_Writer_noop, u8_l("discarded")));
    try_(TEST_expect(written == u8_l("discarded").len));
    try_(io_Writer_writeBytes(io_Writer_noop, u8_l("discarded")));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: writeBytes loops across short writes" $scope) {
    var_(out, A$$(6, u8)) $undefined;
    var writer_impl = (test_io_ChunkWriter){
        .out = A_ref$((S$u8)(out)),
        .pos = 0,
        .chunk_len = 2,
    };

    try_(io_Writer_writeBytes(test_io_ChunkWriter_writer(&writer_impl), u8_l("abcdef")));

    try_(TEST_expect(writer_impl.pos == 6));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(out)), u8_l("abcdef"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: writeBytes rejects a writer that makes no progress" $scope) {
    var_(calls, usize) = 0;
    let writer = io_Writer_ensureValid((io_Writer){
        .ctx = &calls,
        .writeFn = test_io_Writer__zeroWrite,
    });

    if_err((io_Writer_writeBytes(writer, u8_l("blocked")))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$io_WriteFailed().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
    try_(TEST_expect(calls == 1));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: write exposes zero progress without promising completion" $scope) {
    var_(calls, usize) = 0;
    let writer = io_Writer_ensureValid((io_Writer){
        .ctx = &calls,
        .writeFn = test_io_Writer__zeroWrite,
    });

    try_(TEST_expect(try_(io_Writer_write(writer, u8_l("pending"))) == 0));
    try_(TEST_expect(calls == 1));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: failing reports write failure" $scope) {
    if_err((io_Writer_write(io_Writer_failing, u8_l("failed")))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$io_WriteFailed().as_any)));
    } else_ok(written) {
        let_ignore = written;
        try_(TEST_expect(false));
    }
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: writeByteN writes repeated bytes through bounded chunks" $scope) {
    var_(out, A$$(7, u8)) $undefined;
    var writer_impl = (test_io_ChunkWriter){
        .out = A_ref$((S$u8)(out)),
        .pos = 0,
        .chunk_len = 3,
    };

    try_(io_Writer_writeByteN(test_io_ChunkWriter_writer(&writer_impl), u8_c('x'), A_len(out)));

    try_(TEST_expect(writer_impl.pos == A_len(out)));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(out)), u8_l("xxxxxxx"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: writeBytes may leave a written prefix before capacity error" $scope) {
    var_(out, A$$(3, u8)) $undefined;
    var writer_impl = (test_io_ChunkWriter){
        .out = A_ref$((S$u8)(out)),
        .pos = 0,
        .chunk_len = 2,
    };

    if_err((io_Writer_writeBytes(test_io_ChunkWriter_writer(&writer_impl), u8_l("abcdef")))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$TooSmallBuffer().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }

    try_(TEST_expect(writer_impl.pos == A_len(out)));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(out)), u8_l("abc"))));
} $unscoped(TEST_fn);
