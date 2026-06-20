#include <dh-main.h>
#include <dh/heap/Sys.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>
#include "daterm-context/ANSI.h"
#include "daterm-context/ANSI/private.h"

typedef struct daterm_ANSI_test_ChunkReader {
    var_(bytes, S_const$u8);
    var_(pos, usize);
    var_(chunk, usize);
} daterm_ANSI_test_ChunkReader;

typedef struct daterm_ANSI_test_TxnCtx {
    var_(expected, S_const$u8);
    var_(requested, bool);
} daterm_ANSI_test_TxnCtx;

$static fn_((daterm_ANSI_test_TxnCtx__requestWrite(
    P$raw ctx, io_Writer out
))(E$void) $scope) {
    let_ignore = out;
    let self = ptrAlignCast$((daterm_ANSI_test_TxnCtx*)(ctx));
    self->requested = true;
    return_ok({});
} $unscoped(fn);

$static fn_((daterm_ANSI_test_TxnCtx__match(
    P$raw ctx, dansi_Seq seq, P$raw out
))(E$daterm_TxnMatch) $scope) {
    let self = ptrAlignCast$((daterm_ANSI_test_TxnCtx*)(ctx));
    if (seq.kind != dansi_Seq_Kind_csi || !mem_eqlBytes(seq.bytes, self->expected)) {
        return_ok(daterm_TxnMatch_no);
    }
    *ptrAlignCast$((bool*)(out)) = true;
    return_ok(daterm_TxnMatch_done);
} $unscoped(fn);

$static fn_((daterm_ANSI_test_TxnCtx_txn(
    daterm_ANSI_test_TxnCtx* self, bool* matched, time_Dur timeout
))(daterm_Txn)) {
    return (daterm_Txn){
        .timeout = timeout,
        .ctx = ptrCast$((P$raw)(self)),
        .out = ptrCast$((P$raw)(matched)),
        .requestWriteFn = daterm_ANSI_test_TxnCtx__requestWrite,
        .matchFn = daterm_ANSI_test_TxnCtx__match,
    };
};

$static fn_((daterm_ANSI_test_initTxn(
    daterm_ANSI* ansi, io_Reader source, S$u8 input_buf, time_Clock clock
))(void)) {
    asg_l((ansi)(cleared()));
    ansi->input_mode = daterm_ANSI_InputMode_vt;
    ansi->mouse_pos_kind = daterm_mouse_PosKind_cell;
    ansi->input_buf.reader = io_Buf_Reader_init(source, input_buf);
    ansi->input_buf.esc_timeout = daterm_ANSI_esc_timeout_default;
    ansi->clock = clock;
};

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
#if plat_is_windows
    try_(TEST_expect(cfg.input_mode == daterm_ANSI_InputMode_native));
#else
    try_(TEST_expect(cfg.input_mode == daterm_ANSI_InputMode_vt));
#endif
    try_(TEST_expect(time_Dur_eq(cfg.esc_timeout, daterm_ANSI_esc_timeout_default)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: caps match selected input mode" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap))));
    defer_(daterm_ANSI_fini(&ansi));
    let caps = daterm_Term_caps(daterm_ANSI_term(&ansi));

    try_(TEST_expect(caps.native_screen_cells));
    try_(TEST_expect(caps.pending_event_queue));
#if plat_is_windows
    try_(TEST_expect(caps.key_action));
    try_(TEST_expect(caps.modifier_key_event));
    try_(TEST_expect(!caps.protocol_txn));
#else
    try_(TEST_expect(!caps.key_action));
    try_(TEST_expect(!caps.modifier_key_event));
    try_(TEST_expect(caps.protocol_txn));
#endif
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("daterm-context/ANSI: Windows VT mode transfers key ownership to byte input" $guard) {
#if !plat_is_windows
    try_(TEST_skipMsg(u8_l("Windows console input modes are not available on this platform")));
#else
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var cfg = daterm_ANSI_Cfg_default(heap_Sys_alctr(&heap));
    cfg.input_mode = daterm_ANSI_InputMode_vt;
    var ansi = try_(daterm_ANSI_init(cfg));
    defer_(daterm_ANSI_fini(&ansi));
    let caps = daterm_Term_caps(daterm_ANSI_term(&ansi));

    try_(TEST_expect(!caps.key_action));
    try_(TEST_expect(!caps.modifier_key_event));
    try_(TEST_expect(caps.protocol_txn));
#endif
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("daterm-context/ANSI: Windows key records preserve actions" $scope) {
#if !plat_is_windows
    try_(TEST_skipMsg(u8_l("Windows console records are not available on this platform")));
#else
    var_(ansi, daterm_ANSI) = cleared();
    let press = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                    .bKeyDown = TRUE,
                                                                    .wRepeatCount = 1,
                                                                    .wVirtualKeyCode = VK_TAB,
                                                                    .dwControlKeyState = SHIFT_PRESSED,
                                                                }));
    match_(press) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_back_tab));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_press));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let repeat = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                     .bKeyDown = TRUE,
                                                                     .wRepeatCount = 2,
                                                                     .wVirtualKeyCode = VK_LEFT,
                                                                     .dwControlKeyState = ENHANCED_KEY,
                                                                 }));
    match_(repeat) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_left));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_repeat));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let keypad_left = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                          .bKeyDown = TRUE,
                                                                          .wRepeatCount = 1,
                                                                          .wVirtualKeyCode = VK_LEFT,
                                                                      }));
    match_(keypad_left) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_keypad_4));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let keypad_enter = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                           .bKeyDown = TRUE,
                                                                           .wRepeatCount = 1,
                                                                           .wVirtualKeyCode = VK_RETURN,
                                                                           .dwControlKeyState = ENHANCED_KEY,
                                                                       }));
    match_(keypad_enter) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_keypad_enter));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let release = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                      .bKeyDown = FALSE,
                                                                      .wRepeatCount = 1,
                                                                      .wVirtualKeyCode = VK_ESCAPE,
                                                                  }));
    match_(release) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_escape));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_release));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    try_(TEST_expect(isNone(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                        .bKeyDown = TRUE,
                                                                        .wRepeatCount = 1,
                                                                        .uChar.UnicodeChar = as$(WCHAR)(0xd83d),
                                                                    }))));
    let non_bmp = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                      .bKeyDown = TRUE,
                                                                      .wRepeatCount = 1,
                                                                      .uChar.UnicodeChar = as$(WCHAR)(0xde00),
                                                                  }));
    match_(non_bmp) {
    pattern_((daterm_Event_text)(text)) {
        try_(TEST_expect(text.codepoint == 0x1f600));
        try_(TEST_expect(unwrap_(text.action) == daterm_key_Action_press));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
#endif
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: Windows modifier records preserve side and lifecycle" $scope) {
#if !plat_is_windows
    try_(TEST_skipMsg(u8_l("Windows console records are not available on this platform")));
#else
    var_(ansi, daterm_ANSI) = cleared();
    let left_shift = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                         .bKeyDown = TRUE,
                                                                         .wRepeatCount = 1,
                                                                         .wVirtualKeyCode = VK_SHIFT,
                                                                         .wVirtualScanCode = as$(WORD)(MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC)),
                                                                         .dwControlKeyState = SHIFT_PRESSED,
                                                                     }));
    match_(left_shift) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_left_shift));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_press));
        try_(TEST_expect(key.mods.shift));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let right_ctrl = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                         .bKeyDown = TRUE,
                                                                         .wRepeatCount = 2,
                                                                         .wVirtualKeyCode = VK_CONTROL,
                                                                         .dwControlKeyState = RIGHT_CTRL_PRESSED | ENHANCED_KEY,
                                                                     }));
    match_(right_ctrl) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_right_ctrl));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_repeat));
        try_(TEST_expect(key.mods.ctrl));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let left_alt = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                       .bKeyDown = FALSE,
                                                                       .wRepeatCount = 1,
                                                                       .wVirtualKeyCode = VK_MENU,
                                                                   }));
    match_(left_alt) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_left_alt));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_release));
        try_(TEST_expect(!key.mods.alt));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);

    let right_meta = unwrap_(daterm_ANSI_parseWindowsKeyEvent(&ansi, (KEY_EVENT_RECORD){
                                                                         .bKeyDown = TRUE,
                                                                         .wRepeatCount = 1,
                                                                         .wVirtualKeyCode = VK_RWIN,
                                                                     }));
    match_(right_meta) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_right_meta));
        try_(TEST_expect(unwrap_(key.action) == daterm_key_Action_press));
        try_(TEST_expect(key.mods.meta));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
#endif
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: transaction preserves unrelated key event" $scope) {
    var source = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1B[A\x1B[6n")));
    var_(input_mem, A$$(32, u8)) $undefined;
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(ansi, daterm_ANSI) $undefined;
    daterm_ANSI_test_initTxn(
        &ansi, io_Fixed_reader(&source), A_ref$((S$u8)(input_mem)), clock
    );
    try_(io_Buf_Reader_fill(&ansi.input_buf.reader));
    var_(ctx, daterm_ANSI_test_TxnCtx) = { .expected = u8_l("\x1B[6n") };
    var_(matched, bool) = false;

    try_(daterm_Term_runTxn(
        daterm_ANSI_term(&ansi),
        daterm_ANSI_test_TxnCtx_txn(&ctx, &matched, time_Dur_fromMillis(20))
    ));

    try_(TEST_expect(ctx.requested));
    try_(TEST_expect(matched));
    let event = unwrap_(daterm_Term_poll(daterm_ANSI_term(&ansi)));
    match_(event) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_up));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: transaction preserves unrelated mouse event" $scope) {
    var source = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1B[<0;2;3M\x1B[6n")));
    var_(input_mem, A$$(48, u8)) $undefined;
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(ansi, daterm_ANSI) $undefined;
    daterm_ANSI_test_initTxn(
        &ansi, io_Fixed_reader(&source), A_ref$((S$u8)(input_mem)), clock
    );
    try_(io_Buf_Reader_fill(&ansi.input_buf.reader));
    var_(ctx, daterm_ANSI_test_TxnCtx) = { .expected = u8_l("\x1B[6n") };
    var_(matched, bool) = false;

    try_(daterm_Term_runTxn(
        daterm_ANSI_term(&ansi),
        daterm_ANSI_test_TxnCtx_txn(&ctx, &matched, time_Dur_fromMillis(20))
    ));

    let event = unwrap_(daterm_Term_poll(daterm_ANSI_term(&ansi)));
    match_(event) {
    pattern_((daterm_Event_mouse)(mouse)) {
        match_(mouse) {
        pattern_((daterm_mouse_Event_press)(press)) {
            try_(TEST_expect(press.btn == daterm_mouse_Btn_left));
            try_(TEST_expect(press.pos.x == 1));
            try_(TEST_expect(press.pos.y == 2));
        } $end(pattern);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: transaction times out" $scope) {
    var source = io_Fixed_Reader_init(io_Fixed_reading(u8_l("")));
    var_(input_mem, A$$(8, u8)) $undefined;
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(ansi, daterm_ANSI) $undefined;
    daterm_ANSI_test_initTxn(
        &ansi, io_Fixed_reader(&source), A_ref$((S$u8)(input_mem)), clock
    );
    var_(ctx, daterm_ANSI_test_TxnCtx) = { .expected = u8_l("\x1B[6n") };
    var_(matched, bool) = false;

    let result = daterm_Term_runTxn(
        daterm_ANSI_term(&ansi),
        daterm_ANSI_test_TxnCtx_txn(&ctx, &matched, time_Dur_zero)
    );

    let is_timeout = eval_(bool $scope)(catch_((result)(err, {
        try_(TEST_expect(mem_eqlBytes(E_strfy(err.as_any), u8_l("Sched_Timeout"))));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(is_timeout));
    try_(TEST_expect(ctx.requested));
    try_(TEST_expect(!matched));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: transaction reports full pending queue" $scope) {
    var_(source_mem, A$$(daterm_ANSI_pending_event_cap + 1, u8)) $undefined;
    mem_setBytes(A_ref$((S$u8)(source_mem)), 'a');
    var source = io_Fixed_Reader_init(io_Fixed_reading(A_ref$((S_const$u8)(source_mem))));
    var_(input_mem, A$$(daterm_ANSI_pending_event_cap + 1, u8)) $undefined;
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(ansi, daterm_ANSI) $undefined;
    daterm_ANSI_test_initTxn(
        &ansi, io_Fixed_reader(&source), A_ref$((S$u8)(input_mem)), clock
    );
    try_(io_Buf_Reader_fill(&ansi.input_buf.reader));
    var_(ctx, daterm_ANSI_test_TxnCtx) = { .expected = u8_l("\x1B[6n") };
    var_(matched, bool) = false;

    let result = daterm_Term_runTxn(
        daterm_ANSI_term(&ansi),
        daterm_ANSI_test_TxnCtx_txn(&ctx, &matched, time_Dur_fromMillis(2000))
    );

    let is_pending_full = eval_(bool $scope)(catch_((result)(err, {
        try_(TEST_expect(mem_eqlBytes(
            E_strfy(err.as_any), u8_l("daterm_Txn_PendingFull")
        )));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(is_pending_full));
    try_(TEST_expect(ansi.pending.len == daterm_ANSI_pending_event_cap));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: transaction accepts matched report" $scope) {
    var source = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x1B[6n")));
    var_(input_mem, A$$(16, u8)) $undefined;
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(ansi, daterm_ANSI) $undefined;
    daterm_ANSI_test_initTxn(
        &ansi, io_Fixed_reader(&source), A_ref$((S$u8)(input_mem)), clock
    );
    try_(io_Buf_Reader_fill(&ansi.input_buf.reader));
    var_(ctx, daterm_ANSI_test_TxnCtx) = { .expected = u8_l("\x1B[6n") };
    var_(matched, bool) = false;

    try_(daterm_Term_runTxn(
        daterm_ANSI_term(&ansi),
        daterm_ANSI_test_TxnCtx_txn(&ctx, &matched, time_Dur_fromMillis(20))
    ));

    try_(TEST_expect(matched));
    try_(TEST_expect(ansi.pending.len == 0));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: buffered text becomes sequence without refill" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("a")));
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    try_(io_Buf_Reader_fill(&reader));
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(esc_started_at, O$time_Clock_Inst) = none();

    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_text));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("a"))));
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
    try_(TEST_expect(isNone(esc_started_at)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: buffered C1 CSI uses eight-bit framing" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("\x9B"
                                                                 "1;5A")));
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    try_(io_Buf_Reader_fill(&reader));
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(esc_started_at, O$time_Clock_Inst) = none();

    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(
        &reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default
    ));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_csi));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("\x9B"
                                                  "1;5A"))));
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: buffered PM waits for string terminator" $scope) {
    var_(reader_impl, daterm_ANSI_test_ChunkReader) = {
        .bytes = u8_l("\x1B^message\x1B\\"),
        .pos = 0,
        .chunk = 5,
    };
    var_(mem, A$$(32, u8)) $undefined;
    var reader = io_Buf_Reader_init(
        daterm_ANSI_test_ChunkReader_reader(&reader_impl), A_ref$((S$u8)(mem))
    );
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(esc_started_at, O$time_Clock_Inst) = none();

    try_(io_Buf_Reader_fill(&reader));
    try_(TEST_expect(isNone(daterm_ANSI_pollBufferedSeq(
        &reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default
    ))));
    try_(io_Buf_Reader_fill(&reader));
    try_(io_Buf_Reader_fill(&reader));
    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(
        &reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default
    ));

    try_(TEST_expect(seq.kind == dansi_Seq_Kind_pm));
    try_(TEST_expect(mem_eqlBytes(seq.bytes, u8_l("\x1B^message\x1B\\"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: DEC cursor report becomes runtime key" $scope) {
    let event = unwrap_(daterm_ANSI_parseSeqEvent(
        dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1B[A"))
    ));

    match_(event) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_up));
        try_(TEST_expect(key.mods.packed == 0));
        try_(TEST_expect(isNone(key.action)));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: xterm modified cursor preserves modifiers" $scope) {
    let event = unwrap_(daterm_ANSI_parseSeqEvent(
        dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1B[1;5A"))
    ));

    match_(event) {
    pattern_((daterm_Event_key)(key)) {
        try_(TEST_expect(key.code == daterm_key_Code_up));
        try_(TEST_expect(key.mods.ctrl));
        try_(TEST_expect(!key.mods.shift));
        try_(TEST_expect(!key.mods.alt));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: xterm CSI-u becomes runtime text" $scope) {
    let event = unwrap_(daterm_ANSI_parseSeqEvent(
        dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1B[97;3u"))
    ));

    match_(event) {
    pattern_((daterm_Event_text)(text)) {
        try_(TEST_expect(text.codepoint == u8_c('a')));
        try_(TEST_expect(text.mods.alt));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: xterm SGR mouse becomes semantic variant" $scope) {
    let event = unwrap_(daterm_ANSI_parseSeqEvent(
        dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1B[<0;10;5M"))
    ));

    match_(event) {
    pattern_((daterm_Event_mouse)(mouse)) {
        match_(mouse) {
        pattern_((daterm_mouse_Event_press)(press)) {
            try_(TEST_expect(press.btn == daterm_mouse_Btn_left));
            try_(TEST_expect(press.pos.x == 9));
            try_(TEST_expect(press.pos.y == 4));
            try_(TEST_expect(press.pos.kind == daterm_mouse_PosKind_cell));
        } $end(pattern);
        default_() try_(TEST_expect(false)) $end(default);
        } $end(match);
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: xterm focus becomes runtime focus" $scope) {
    let event = unwrap_(daterm_ANSI_parseSeqEvent(
        dansi_Seq_from(dansi_Seq_Kind_csi, u8_l("\x1B[I"))
    ));

    match_(event) {
    pattern_((daterm_Event_focus)(focus)) {
        try_(TEST_expect(focus == daterm_focus_Event_in));
    } $end(pattern);
    default_() try_(TEST_expect(false)) $end(default);
    } $end(match);
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("daterm-context/ANSI: split CSI waits for final byte" $scope) {
    var_(reader_impl, daterm_ANSI_test_ChunkReader) = {
        .bytes = u8_l("\x1B[A"),
        .pos = 0,
        .chunk = 2,
    };
    var_(mem, A$$(8, u8)) $undefined;
    var reader = io_Buf_Reader_init(daterm_ANSI_test_ChunkReader_reader(&reader_impl), A_ref$((S$u8)(mem)));
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(esc_started_at, O$time_Clock_Inst) = none();

    try_(io_Buf_Reader_fill(&reader));
    try_(TEST_expect(isNone(daterm_ANSI_pollBufferedSeq(&reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default))));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Reader_ready(reader), u8_l("\x1B["))));

    try_(io_Buf_Reader_fill(&reader));
    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default));

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
    let_(clock, time_Clock) = union_of((time_Clock_awake)(try_(time_Awake_direct())));
    var_(esc_started_at, O$time_Clock_Inst) = none();

    try_(TEST_expect(isNone(daterm_ANSI_pollBufferedSeq(&reader, clock, &esc_started_at, daterm_ANSI_esc_timeout_default))));
    try_(TEST_expect(isSome(esc_started_at)));
    let seq = unwrap_(daterm_ANSI_pollBufferedSeq(&reader, clock, &esc_started_at, time_Dur_zero));

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
