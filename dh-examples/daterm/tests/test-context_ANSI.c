#include <dh-main.h>
#include <daterm-context/ANSI/private.h>
#include <dh/heap/Sys.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

typedef struct daterm_ANSI_test_ChunkReader {
    var_(bytes, S_const$u8);
    var_(pos, usize);
    var_(chunk, usize);
} daterm_ANSI_test_ChunkReader;

$static fn_((daterm_ANSI_test_ChunkReader__read(P$raw ctx, S$u8 out))(E$usize) $scope) {
    let self = ptrAlignCast$((daterm_ANSI_test_ChunkReader*)(ctx));
    let available = self->bytes.len - self->pos;
    if (available == 0) { return_ok(0); }
    let to_read = pri_min(pri_min(available, out.len), self->chunk);
    mem_copyBytes(
        S_prefix((out)(to_read)),
        S_prefix((S_suffix((self->bytes)(self->pos)))(to_read))
    );
    self->pos += to_read;
    return_ok(to_read);
} $unscoped(fn);

$static fn_((daterm_ANSI_test_ChunkReader_reader(daterm_ANSI_test_ChunkReader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .readFn = daterm_ANSI_test_ChunkReader__read,
    };
};

TEST_fn_("daterm-context/ANSI: default config keeps processed output" $scope) {
    var_(gpa, mem_Alctr) = cleared();
    let cfg = daterm_ANSI_Cfg_default(gpa);

    try_(TEST_expect(cfg.output_mode == daterm_ANSI_OutputMode_processed));
    try_(TEST_expect(time_Dur_eq(cfg.esc_timeout, daterm_ANSI_esc_timeout_default)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: buffered raw byte becomes key sequence without refill" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("a")));
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    try_(io_Buf_Reader_fill(&reader));
    var_(esc_started_at, O$time_Inst) = none();

    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, &esc_started_at, daterm_ANSI_esc_timeout_default));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_raw));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("a"))));
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
    try_(TEST_expect(isNone(esc_started_at)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: split CSI waits for final byte" $scope) {
    var reader_impl = (daterm_ANSI_test_ChunkReader){
        .bytes = u8_l("\x1B[A"),
        .pos = 0,
        .chunk = 2,
    };
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(daterm_ANSI_test_ChunkReader_reader(&reader_impl), A_ref$((S$u8)(mem)));
    var_(esc_started_at, O$time_Inst) = none();

    try_(io_Buf_Reader_fill(&reader));
    try_(TEST_expect(isNone(daterm_ANSI_pollBufferedSeq(&reader, &esc_started_at, daterm_ANSI_esc_timeout_default))));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Reader_ready(reader), u8_l("\x1B["))));

    try_(io_Buf_Reader_fill(&reader));
    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, &esc_started_at, daterm_ANSI_esc_timeout_default));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_csi));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("\x1B[A"))));
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: single escape follows timeout policy" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1B")));
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    try_(io_Buf_Reader_fill(&reader));
    var_(esc_started_at, O$time_Inst) = none();

    try_(TEST_expect(isNone(daterm_ANSI_pollBufferedSeq(&reader, &esc_started_at, daterm_ANSI_esc_timeout_default))));
    try_(TEST_expect(isSome(esc_started_at)));
    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, &esc_started_at, time_Dur_zero));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_esc));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("\x1B"))));
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
    try_(TEST_expect(isNone(esc_started_at)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: POSIX processed output keeps output postprocessing" $guard) {
#if !plat_is_posix
    try_(TEST_skipMsg(u8_l("POSIX termios is not available on this platform")));
#else
    let input = io_getStdIn();
    if (!try_(fs_File_isTty(input))) {
        try_(TEST_skipMsg(u8_l("stdin is not a TTY")));
    }

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap))));
    defer_(daterm_ANSI_fini(&ansi));

    try_(daterm_ANSI_enableRawMode(&ansi));

    struct termios mode = cleared();
    try_(TEST_expect(tcgetattr(as$(int)(input.handle), &mode) == 0));
    try_(TEST_expect((mode.c_oflag & OPOST) != 0));
#endif
} $unguarded(TEST_fn);

TEST_fn_("daterm-context/ANSI: POSIX raw output disables output postprocessing" $guard) {
#if !plat_is_posix
    try_(TEST_skipMsg(u8_l("POSIX termios is not available on this platform")));
#else
    let input = io_getStdIn();
    if (!try_(fs_File_isTty(input))) {
        try_(TEST_skipMsg(u8_l("stdin is not a TTY")));
    }

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var cfg = daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap));
    cfg.output_mode = daterm_ANSI_OutputMode_raw;
    var ansi = try_(daterm_ANSI_init(cfg));
    defer_(daterm_ANSI_fini(&ansi));

    try_(daterm_ANSI_enableRawMode(&ansi));

    struct termios mode = cleared();
    try_(TEST_expect(tcgetattr(as$(int)(input.handle), &mode) == 0));
    try_(TEST_expect((mode.c_oflag & OPOST) == 0));
#endif
} $unguarded(TEST_fn);
