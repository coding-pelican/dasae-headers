#include "dh/async.h"

#include "dh/time.h"
#include "dh/Rand.h"
#include "dh/io/stream.h"

/// \brief Task to be executed
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
T_use$((Task)(P, S, O));
T_use$((Co_Ctx)(P, S, O));
T_use$((P$Co_Ctx)(O));

/// \brief List of tasks to be executed
static var_(exec_s_task_list, A$$(10, O$Task)) = {};
/// \brief Run the event loop
/// \param endless Whether to run the loop endlessly
static fn_((exec_runLoop(bool endless))(void)) {
    // io_stream_println(u8_l("looping events y'all"));
    while (true) {
        let now = time_Instant_now();
        var any = false;
        var frame = eval_(O$P$Co_Ctx $scope)(for_(($s(ref$A(exec_s_task_list)))(task_remaining) {
            let task = orelse_((O_asP(task_remaining))(continue));
            any = true;
            if (time_Instant_le(task->expires, now)) {
                // io_stream_println(u8_l("sleep over y'all"));
                let frame = task->frame;
                asg_lit((task_remaining)(none()));
                $break_(some(frame));
            }
        })) eval_(else)($break_(none())) $unscoped_(eval);
        if_some((frame)(ctx)) {
            resume_(ctx);
        }
        if (!(endless || any)) { break; }
    }
}

/// \brief Find a slot for a task
/// \return The slot for the task
static fn_((exec_findSlot(void))(O$Task*)) {
    let slot = eval_(P$$(O$Task) $scope)(
        for_(($s(ref$A(exec_s_task_list)))(task) {
            if_none(*task)
                $break_(task);
        })
    ) eval_(else)(claim_unreachable) $unscoped_(eval);
    return slot;
}

use_Co_Ctx$(Void);
/// \brief Sleep for a specified duration
/// \param caller The caller context
/// \param ms The duration to sleep in milliseconds
async_fn_(exec_sleep, (var_(caller, O$$(Co_Ctx*)); var_(ms, u64);), Void);
async_fn_scope(exec_sleep, {}) {
    let_ignore = locals;
    suspend_({
        let slot = exec_findSlot();
        let time = blk({
            static let fromMs = time_Duration_fromMillis;
            static let addDur = time_Instant_addDuration;
            static let now = time_Instant_now;
            blk_return addDur(now(), fromMs(args->ms));
        });
        asg_lit((slot)(some({ .frame = orelse_((args->caller)(ctx->anyraw)), .expires = time })));
    });
    areturn_({});
} $unscoped_(async_fn);

#include "dh/main.h"
#include "dh/Thrd.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void)) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_getCurrentId(), label);
    va_list args = {};
    with_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_printVaArgs(fmt, args);
    }
    io_stream_nl();
}

fn_((Terminal_clear(void))(void)) { io_stream_print(u8_l("\x1b[2J\x1b[H")); };
fn_((Terminal_home(void))(void)) { io_stream_print(u8_l("\x1b[1;1H")); };
fn_((Terminal_nl(void))(void)) { io_stream_nl(); };
fn_((Terminal_moveCursor(u32 x, u32 y))(void)) { io_stream_print(u8_l("\x1b[{:u};{:u}H"), y + 1, x + 1); };
fn_((Terminal_writeTypo(u8 typo))(void)) { io_stream_print(u8_l("{:c}"), typo); };
fn_((Terminal_writeTypoAt(u32 x, u32 y, u8 typo))(void)) {
    Terminal_moveCursor(x, y);
    Terminal_writeTypo(typo);
};
fn_((Terminal_writeText(S_const$u8 text))(void)) { io_stream_print(u8_l("{:s}"), text); };
fn_((Terminal_writeTextAt(u32 x, u32 y, S_const$u8 text))(void)) {
    Terminal_moveCursor(x, y);
    Terminal_writeText(text);
};

#include "dh/fs/File.h"
#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/io/Reader.h"

T_use_A$(1024, u8);
fn_((Terminal_readBytes(S$u8 mem))(S$u8)) {
    let stream_in = fs_File_reader(io_getStdIn());
    return S_prefix((mem)(catch_((io_Reader_read(stream_in, mem))($ignore, claim_unreachable))));
};

/// \brief Types out a string character by character with specified interval between characters
/// \param caller The caller context
/// \param text The text to type out
/// \param interval Seconds to wait between each character
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectWithInterval, (var_(caller, O$$(Co_Ctx*)); var_(text, S_const$u8); var_(interval, f64); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectWithInterval, {
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    locals->delay_ms = as$(u64)(args->interval * time_millis_per_sec);
    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        Terminal_writeTypoAt(args->x + as$(u32)(locals->iter_typo), args->y, *S_at((args->text)[locals->iter_typo]));
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse_((args->caller)(ctx->anyraw))), locals->delay_ms));
    }
    areturn_({});
} $unscoped_(async_fn);

/// \brief Types out a string over a specified total duration
/// \param caller The caller context
/// \param text The text to type out
/// \param duration Total time in seconds to complete typing
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectOverDuration, (var_(caller, O$$(Co_Ctx*)); var_(text, S_const$u8); var_(duration, f64); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectOverDuration, {
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    let interval = args->duration / as$(f64)(args->text.len);
    locals->delay_ms = as$(u64)(interval * time_millis_per_sec);
    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        Terminal_writeTypoAt(args->x + as$(u32)(locals->iter_typo), args->y, *S_at((args->text)[locals->iter_typo]));
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse_((args->caller)(ctx->anyraw))), locals->delay_ms));
    }
    areturn_({});
} $unscoped_(async_fn);

/// \brief Advanced typing effect with realistic variable speeds
/// \param caller The caller context
/// \param text The text to type out
/// \param base_interval Base delay in seconds
/// \param add_randomness Whether to add random variation to typing speed
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectRealistic, (var_(caller, O$$(Co_Ctx*)); var_(text, S_const$u8); var_(base_interval, f64); var_(add_randomness, bool); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectRealistic, {
    var_(rand, Rand);
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    // Initialize random number generator if randomness is enabled
    locals->rand = expr_(Rand $scope)(if (args->add_randomness) {
        $break_(Rand_init());
    }) expr_(else)({
        $break_(Rand_default);
    }) $unscoped_(expr);

    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        let current_char = *S_at((args->text)[locals->iter_typo]);
        Terminal_writeTypoAt(args->x + as$(u32)(locals->iter_typo), args->y, current_char);

        locals->delay_ms = as$(u64)(args->base_interval * time_millis_per_sec);
        if (args->add_randomness) {
            // Add randomness to make it feel more natural (-30 to +50 ms variation)
            let random_variation = Rand_rangeIInt(&locals->rand, -30, 50);
            locals->delay_ms = as$(u64)(prim_max(10, as$(i64)((locals->delay_ms) + random_variation)));

            // Longer pauses after punctuation
            if (current_char == '.' || current_char == '!' || current_char == '?') {
                let punctuation_delay = Rand_rangeUInt(&locals->rand, 200, 500);
                locals->delay_ms += punctuation_delay;
            } else if (current_char == ',' || current_char == ';') {
                let comma_delay = Rand_rangeUInt(&locals->rand, 100, 200);
                locals->delay_ms += comma_delay;
            } else if (current_char == ' ') {
                let space_delay = Rand_rangeUInt(&locals->rand, 20, 80);
                locals->delay_ms += space_delay;
            }
        }
        // Ensure delay is not negative (minimum 10ms)
        locals->delay_ms = prim_max(10, locals->delay_ms);
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse_((args->caller)(ctx->anyraw))), locals->delay_ms));
    }
    areturn_({});
} $unscoped_(async_fn);

#include "dh/mem/common.h"
#include "dh/fmt/common.h"

T_use$((u8)(
    mem_TokenIter,
    mem_tokenizeValue,
    mem_TokenIter_next
));

/// \brief Run the main function
/// \param args The arguments to the main function
async_fn_(runMain, (S$S_const$u8 args;), Void);
async_fn_scope(runMain, {
    var_(sample_text, S_const$u8);
    var_(line, u32);
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectWithInterval));
    } demo1;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectOverDuration));
    } demo2;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectWithInterval));
    } demo3;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectOverDuration));
    } demo4;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectWithInterval));
    } demo5;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectOverDuration));
    } demo6;
    struct {
        var_(type_ctx, Co_CtxFn$(typeEffectRealistic));
    } demo7;
    var_(read_mem, A$1024$u8);
    var_(user_text, S$u8);
    var_(interval, f64);
    union {
        var_(type_interval, Co_CtxFn$(typeEffectWithInterval));
        var_(type_duration, Co_CtxFn$(typeEffectOverDuration));
        var_(type_realistic, Co_CtxFn$(typeEffectRealistic));
    } interactive_ctx;
}) {
    let_ignore = args;

    locals->line = 0;
    Terminal_clear();
    Terminal_home();
    Terminal_writeText(u8_l("=== Typing Effect Demo ==="));
    Terminal_nl();

    locals->sample_text = u8_l("Hello, World! This is a typing effect demonstration.");

    // Demo 1: Fixed interval between characters
    locals->demo1.type_ctx = *async_ctx((typeEffectWithInterval)(none(), locals->sample_text, 0.1, .x = 0, .y = 1));
    resume_(&locals->demo1.type_ctx);
    locals->demo3.type_ctx = *async_ctx((typeEffectWithInterval)(none(), locals->sample_text, 0.125, .x = 0, .y = 3));
    resume_(&locals->demo3.type_ctx);
    locals->demo5.type_ctx = *async_ctx((typeEffectWithInterval)(none(), locals->sample_text, 0.075, .x = 0, .y = 5));
    resume_(&locals->demo5.type_ctx);

    // Demo 2: Complete typing over specific duration
    locals->demo2.type_ctx = *async_ctx((typeEffectOverDuration)(none(), locals->sample_text, 3.0, .x = 0, .y = 2));
    resume_(&locals->demo2.type_ctx);
    locals->demo4.type_ctx = *async_ctx((typeEffectOverDuration)(none(), locals->sample_text, 5.0, .x = 0, .y = 4));
    resume_(&locals->demo4.type_ctx);
    locals->demo6.type_ctx = *async_ctx((typeEffectOverDuration)(none(), locals->sample_text, 4.0, .x = 0, .y = 6));
    resume_(&locals->demo6.type_ctx);

    // Demo 3: Realistic typing with randomness
    locals->demo7.type_ctx = *async_ctx((typeEffectRealistic)(none(), locals->sample_text, 0.08, true, .x = 0, .y = 7));
    resume_(&locals->demo7.type_ctx);
    locals->line = 8;

    await_(&locals->demo1.type_ctx);
    await_(&locals->demo2.type_ctx);
    await_(&locals->demo3.type_ctx);
    await_(&locals->demo4.type_ctx);
    await_(&locals->demo5.type_ctx);
    await_(&locals->demo6.type_ctx);
    await_(&locals->demo7.type_ctx);

    Terminal_moveCursor(0, locals->line);
    // let_ignore = getchar(); // TODO: use a better way to wait for user input

    // Interactive example
    Terminal_moveCursor(0, locals->line);
    Terminal_nl();
    locals->line++;
    Terminal_writeText(u8_l("=== Interactive Mode ==="));
    Terminal_nl();
    locals->line++;

    Terminal_writeText(u8_l("Enter text to type: "));
    locals->user_text = Terminal_readBytes(A_ref$((S$u8)(locals->read_mem)));
    locals->line++;

    if (0 < locals->user_text.len) {
        Terminal_writeText(u8_l("Enter interval in seconds (e.g., 0.1): "));
        let input = Terminal_readBytes(suffixS(A_ref$((S$u8)(locals->read_mem)), locals->user_text.len));
        var iter = mem_tokenizeValue$u8(input.as_const, u8_c('\n'));
        let interval = mem_TokenIter_next$u8(&iter);
        if_ok((fmt_parse$f64(unwrap_(interval)))(interval)) {
            locals->interval = interval;
            locals->line++;
            Terminal_nl();
            locals->line++;
            let message = u8_l("Typing your text: ");
            Terminal_writeText(message);
            locals->interactive_ctx.type_interval = *async_ctx(
                (typeEffectWithInterval)(none(), locals->user_text.as_const, locals->interval, .x = as$(u32)(message.len), .y = locals->line++)
            );
            resume_(&locals->interactive_ctx.type_interval);
            exec_runLoop(false);
            await_(&locals->interactive_ctx.type_interval);
        } else_err(e) {
            let_ignore = e;
            locals->line++;
            Terminal_nl();
            locals->line++;
            let message = u8_l("Using default realistic typing: ");
            Terminal_writeText(message);
            locals->interactive_ctx.type_realistic = *async_ctx(
                (typeEffectRealistic)(none(), locals->user_text.as_const, 0.08, true, .x = as$(u32)(message.len), .y = locals->line++)
            );
            resume_(&locals->interactive_ctx.type_realistic);
            exec_runLoop(false);
            await_(&locals->interactive_ctx.type_realistic);
        }
        // let_ignore = getchar();
    }

    Terminal_nl();
    Terminal_writeText(u8_l("Press any key to exit..."));
    Terminal_nl();
    // let_ignore = getchar(); // TODO: use a better way to wait for user input

    areturn_({});
} $unscoped_(async_fn);

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    var task = async_((runMain)(args));
    exec_runLoop(false);
    nosuspend_(await_(resume_(task)));
    return_ok({});
} $unscoped_(fn);
