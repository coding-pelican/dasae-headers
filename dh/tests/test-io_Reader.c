#include "dh-main.h"
#include "dh/io.h"
#include "dh/mem/common.h"

T_alias$((test_io_ChunkReader)(struct test_io_ChunkReader {
    var_(input, S_const$u8);
    var_(pos, usize);
    var_(chunk_len, usize);
}));

$static fn_((test_io_ChunkReader_read(P$raw ctx, S$u8 out_buf))(io_ReadE$usize) $scope) {
    let self = ptrAlignCast$((test_io_ChunkReader*)(ctx));
    if (self->pos == self->input.len) return_ok(0);

    let available = self->input.len - self->pos;
    let to_read = pri_min(pri_min(available, out_buf.len), self->chunk_len);
    mem_copyBytes(
        S_prefix((out_buf)(to_read)),
        S_slice((self->input)$r(self->pos, self->pos + to_read))
    );
    self->pos += to_read;
    return_ok(to_read);
} $unscoped(fn);

$static fn_((test_io_ChunkReader_reader(test_io_ChunkReader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .readFn = test_io_ChunkReader_read,
    };
};

TEST_fn_("io/Reader: readAtLeast returns partial count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    let reader = io_Fixed_reader(&reader_impl);
    var_(out, A$$(5, u8)) $undefined;

    let bytes_read = try_(io_Reader_readAtLeast(reader, A_ref$((S$u8)(out)), 5));

    try_(TEST_expect(bytes_read == 3));
    try_(TEST_expect(mem_eqlBytes(A_slice$((S_const$u8)(out)$r(0, bytes_read)), u8_l("abc"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: readExact loops across short reads" $scope) {
    var chunked = (test_io_ChunkReader){
        .input = u8_l("abcdef"),
        .pos = 0,
        .chunk_len = 2,
    };
    var_(out, A$$(6, u8)) $undefined;

    try_(io_Reader_readExact(test_io_ChunkReader_reader(&chunked), A_ref$((S$u8)(out))));

    try_(TEST_expect(chunked.pos == 6));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(out)), u8_l("abcdef"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: readByte reports UnexpectedEOF at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("")));

    if_err((io_Reader_readByte(io_Fixed_reader(&reader_impl)))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$UnexpectedEOF().as_any)));
    } else_ok(byte) {
        let_ignore = byte;
        try_(TEST_expect(false));
    }
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: skipAtLeast returns skipped count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    let skipped_len = try_(io_Reader_skipAtLeast(io_Fixed_reader(&reader_impl), 5));

    try_(TEST_expect(skipped_len == 3));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: skip requires the exact requested length" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));

    if_err((io_Reader_skip(io_Fixed_reader(&reader_impl), 4))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$UnexpectedEOF().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copy moves bytes into any writer" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abcdef")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(out))));

    let copied = try_(io_Reader_copy(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl)));
    let written = io_Fixed_written(writer_impl.stream);

    try_(TEST_expect(copied == 6));
    try_(TEST_expect(mem_eqlBytes(written.as_const, u8_l("abcdef"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copyExact fails when source ends early" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(out))));

    let result = io_Reader_copyExact(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl), 4);

    try_(TEST_expect(isErr(result)));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copyAtLeast returns copied count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(out))));

    let copied_len = try_(io_Reader_copyAtLeast(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl), 5));
    let written = io_Fixed_written(writer_impl.stream);

    try_(TEST_expect(copied_len == 3));
    try_(TEST_expect(mem_eqlBytes(written.as_const, u8_l("abc"))));
} $unscoped(TEST_fn);
