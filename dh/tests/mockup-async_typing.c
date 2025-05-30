#include "mockup-async_ex.h"

#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/time.h"
#include "dh/Random.h"
#define u8_s Str_m
#define u8_l Str_l

#include <stdio.h>

/// \brief Task to be executed
typedef struct Task {
    var_(frame, Co_Ctx*);
    var_(expires, time_Instant);
} Task;
use_Ptr$(Task);
use_Sli$(Task);
use_Opt$(Task);

/// \brief List of tasks to be executed
static var_(exec_s_task_list, Arr$$(32, Opt$Task)) = {};
/// \brief Run the event loop
/// \param endless Whether to run the loop endlessly
static fn_(exec_runLoop(bool endless), void) {
    use_Sli$(Opt$Task);
    use_Opt$(Co_Ctx);

    // printf("looping events y'all\n");
    while (true) {
        let  now   = time_Instant_now();
        bool any   = false;
        var  frame = none$(Opt$Ptr$Co_Ctx);
        for_slice (Sli_arr$(Sli$Opt$Task, exec_s_task_list), task_remaining) {
            if_some(Opt_asPtr(task_remaining), task) {
                any = true;
                if (time_Instant_le(task->expires, now)) {
                    // printf("sleep over y'all\n");
                    Opt_asg(&frame, some(task->frame));
                    Opt_asg(task_remaining, none());
                    break;
                }
            }
        }
        if_some(frame, ctx) {
            resume_(ctx);
        }
        if (!(endless || any)) { break; }
    }
}

/// \brief Find a slot for a task
/// \return The slot for the task
static fn_(exec_findSlot(void), Opt$Task*) {
    use_Sli$(Opt$Task);

    Opt$Task* slot = null;
    for_slice (Sli_arr$(Sli$Opt$Task, exec_s_task_list), task) {
        if_none(*task) {
            slot = task;
            break;
        }
    }
    if (slot == null) { claim_unreachable; }
    return slot;
}

use_Co_Ctx$(Void);
/// \brief Sleep for a specified duration
/// \param caller The caller context
/// \param ms The duration to sleep in milliseconds
async_fn_(exec_sleep, (var_(caller, Opt$$(Co_Ctx*)); var_(ms, u64);), Void);
async_fn_scope(exec_sleep, {}) {
    $ignore = locals;
    suspend_({
        let slot = exec_findSlot();
        let time = eval({
            static let fromMs = time_Duration_fromMillis;
            static let addDur = time_Instant_addDuration;
            static let now    = time_Instant_now;
            eval_return addDur(now(), fromMs(args->ms));
        });
        Opt_asg(slot, some({ .frame = orelse(args->caller, ctx->anyraw), .expires = time }));
    });
    areturn_({});
} async_unscoped;

#include "dh/main.h"
#include "dh/Thrd.h"
#include "dh/callback.h"

/// \brief Report a message
/// \param label The label to report
/// \param fmt The format string
/// \param ... The arguments to the format string
fn_(report(Str_const label, const char* fmt, ...), void) {
    printf("[ThrdId(%zu): %*s] ", Thrd_getCurrentId(), as$(i32, label.len), label.ptr);
    va_list args = {};
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

fn_(Terminal_clear(void), void) { printf("\x1b[2J\x1b[H"); }
fn_(Terminal_home(void), void) { printf("\x1b[1;1H"); }
fn_(Terminal_feedLine(void), void) { printf("\n"); }
fn_(Terminal_moveCursor(u32 x, u32 y), void) { printf("\x1b[%u;%uH", y + 1, x + 1); }
fn_(Terminal_writeByte(u8 byte), void) { printf("%c", byte); }
fn_(Terminal_writeByteAt(u32 x, u32 y, u8 byte), void) {
    Terminal_moveCursor(x, y);
    Terminal_writeByte(byte);
}
fn_(Terminal_writeBytes(Sli_const$u8 bytes), void) { printf("%*s", as$(i32, bytes.len), bytes.ptr); }
fn_(Terminal_writeBytesAt(u32 x, u32 y, Sli_const$u8 bytes), void) {
    Terminal_moveCursor(x, y);
    Terminal_writeBytes(bytes);
}
fn_(Terminal_writeTypo(u8 typo), void) { printf("%c", typo); }
fn_(Terminal_writeTypoAt(u32 x, u32 y, u8 typo), void) {
    Terminal_moveCursor(x, y);
    Terminal_writeTypo(typo);
}
fn_(Terminal_writeText(Sli_const$u8 text), void) { printf("%*s", as$(i32, text.len), text.ptr); }
fn_(Terminal_writeTextAt(u32 x, u32 y, Sli_const$u8 text), void) {
    Terminal_moveCursor(x, y);
    Terminal_writeText(text);
}

use_Arr$(1024, u8);
fn_(Terminal_readBytes(Arr$1024$u8* mem), Sli$u8) {
    $ignore = fgets(as$(char*, mem->buf), as$(i32, Arr_len(*mem)), stdin);
    return Str_fromZ(mem->buf);
}


/// \brief Types out a string character by character with specified interval between characters
/// \param caller The caller context
/// \param text The text to type out
/// \param interval Seconds to wait between each character
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectWithInterval, (var_(caller, Opt$$(Co_Ctx*)); var_(text, Str_const); var_(interval, f64); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectWithInterval, {
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    locals->delay_ms = as$(u64, (args->interval * time_millis_per_sec));
    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        Terminal_writeTypoAt(args->x + as$(u32, locals->iter_typo), args->y, Sli_getAt(args->text, locals->iter_typo));
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse(args->caller, ctx->anyraw)), locals->delay_ms));
    }

    areturn_({});
} async_unscoped;

/// \brief Types out a string over a specified total duration
/// \param caller The caller context
/// \param text The text to type out
/// \param duration Total time in seconds to complete typing
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectOverDuration, (var_(caller, Opt$$(Co_Ctx*)); var_(text, Str_const); var_(duration, f64); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectOverDuration, {
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    let interval     = args->duration / as$(f64, args->text.len);
    locals->delay_ms = as$(u64, (interval * time_millis_per_sec));
    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        Terminal_writeTypoAt(args->x + as$(u32, locals->iter_typo), args->y, Sli_getAt(args->text, locals->iter_typo));
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse(args->caller, ctx->anyraw)), locals->delay_ms));
    }

    areturn_({});
} async_unscoped;

/// \brief Advanced typing effect with realistic variable speeds
/// \param caller The caller context
/// \param text The text to type out
/// \param base_interval Base delay in seconds
/// \param add_randomness Whether to add random variation to typing speed
/// \param x The x coordinate (0-based)
/// \param y The y coordinate (0-based)
async_fn_(typeEffectRealistic, (var_(caller, Opt$$(Co_Ctx*)); var_(text, Str_const); var_(base_interval, f64); var_(add_randomness, bool); var_(x, u32); var_(y, u32);), Void);
async_fn_scope(typeEffectRealistic, {
    var_(delay_ms, u64);
    var_(iter_typo, usize);
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
}) {
    if (args->text.len == 0) { areturn_({}); }
    debug_assert_nonnull(args->text.ptr);

    // Initialize random number generator if randomness is enabled
    if (args->add_randomness) { Random_init(); }

    for (locals->iter_typo = 0; locals->iter_typo < args->text.len; ++locals->iter_typo) {
        let current_char = Sli_getAt(args->text, locals->iter_typo);
        Terminal_writeTypoAt(args->x + as$(u32, locals->iter_typo), args->y, current_char);

        locals->delay_ms = as$(u64, (args->base_interval * time_millis_per_sec));
        if (args->add_randomness) {
            // Add randomness to make it feel more natural (-30 to +50 ms variation)
            let random_variation = Random_range_i64(-30, 50);
            locals->delay_ms     = as$(u64, prim_max(10, as$(i64, locals->delay_ms) + random_variation));

            // Longer pauses after punctuation
            if (current_char == '.' || current_char == '!' || current_char == '?') {
                let punctuation_delay = Random_range_u64(200, 500);
                locals->delay_ms += punctuation_delay;
            } else if (current_char == ',' || current_char == ';') {
                let comma_delay = Random_range_u64(100, 200);
                locals->delay_ms += comma_delay;
            } else if (current_char == ' ') {
                let space_delay = Random_range_u64(20, 80);
                locals->delay_ms += space_delay;
            }
        }
        // Ensure delay is not negative (minimum 10ms)
        locals->delay_ms = prim_max(10, locals->delay_ms);
        callAsync(&locals->sleep_ctx, (exec_sleep)(some(orelse(args->caller, ctx->anyraw)), locals->delay_ms));
    }

    areturn_({});
} async_unscoped;

/// \brief Run the main function
/// \param args The arguments to the main function
async_fn_(runMain, (Sli$Str_const args;), Void);
async_fn_scope(runMain, {
    var_(sample_text, Str_const);
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
    var_(read_mem, Arr$1024$u8);
    var_(user_text, Sli$u8);
    var_(interval, f64);
    union {
        var_(type_interval, Co_CtxFn$(typeEffectWithInterval));
        var_(type_duration, Co_CtxFn$(typeEffectOverDuration));
        var_(type_realistic, Co_CtxFn$(typeEffectRealistic));
    } interactive_ctx;
}) {
    $ignore = args;

    locals->line = 0;
    Terminal_clear();
    Terminal_home();
    Terminal_writeText(u8_l("=== Typing Effect Demo ==="));
    Terminal_feedLine();

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
    $ignore = getchar(); // TODO: use a better way to wait for user input

    // Interactive example
    Terminal_moveCursor(0, locals->line);
    Terminal_feedLine();
    locals->line++;
    Terminal_writeText(u8_l("=== Interactive Mode ==="));
    Terminal_feedLine();
    locals->line++;

    Terminal_writeText(u8_l("Enter text to type: "));
    locals->user_text = Terminal_readBytes(&locals->read_mem);
    locals->line++;

    if (0 < locals->user_text.len) {
        Terminal_writeText(u8_l("Enter interval in seconds (e.g., 0.1): "));
        if (scanf("%lf", &locals->interval) == 1) {
            locals->line++;
            Terminal_feedLine();
            locals->line++;
            let message = u8_l("Typing your text: ");
            Terminal_writeText(message);
            locals->interactive_ctx.type_interval = *async_ctx(
                (typeEffectWithInterval)(none(), locals->user_text.as_const, locals->interval, .x = as$(u32, message.len), .y = locals->line++)
            );
            resume_(&locals->interactive_ctx.type_interval);
            exec_runLoop(false);
            await_(&locals->interactive_ctx.type_interval);
        } else {
            locals->line++;
            Terminal_feedLine();
            locals->line++;
            let message = u8_l("Using default realistic typing: ");
            Terminal_writeText(message);
            locals->interactive_ctx.type_realistic = *async_ctx(
                (typeEffectRealistic)(none(), locals->user_text.as_const, 0.08, true, .x = as$(u32, message.len), .y = locals->line++)
            );
            resume_(&locals->interactive_ctx.type_realistic);
            exec_runLoop(false);
            await_(&locals->interactive_ctx.type_realistic);
        }
        $ignore = getchar();
    }

    Terminal_feedLine();
    Terminal_writeText(u8_l("Press any key to exit..."));
    Terminal_feedLine();
    $ignore = getchar(); // TODO: use a better way to wait for user input

    areturn_({});
} async_unscoped;

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    var task = async_((runMain)(args));
    exec_runLoop(false);
    nosuspend_(await_(resume_(task)));
    return_ok({});
} $unscoped;

/*
public
class TypingEffect {

public
    static async Task TypeWithIntervalAsync(string text, double intervalSeconds) {
        if (string.IsNullOrEmpty(text)) {
            return;
        }

        int delayMs = (int)(intervalSeconds * 1000);

        foreach(char c in text) {
            Console.Write(c);
            await Task.Delay(delayMs);
        }
        Console.WriteLine(); // Add newline at the end
    }

    /// <summary>
    /// Types out a string over a specified total duration
    /// </summary>
    /// <param name="text">The text to type out</param>
    /// <param name="totalSeconds">Total time in seconds to complete typing</param>
public
    static async Task TypeOverDurationAsync(string text, double totalSeconds) {
        if (string.IsNullOrEmpty(text) || text.Length == 0) {
            return;
        }

        double intervalSeconds = totalSeconds / text.Length;
        int    delayMs         = (int)(intervalSeconds * 1000);

        foreach(char c in text) {
            Console.Write(c);
            await Task.Delay(delayMs);
        }
        Console.WriteLine(); // Add newline at the end
    }

    /// <summary>
    /// Advanced typing effect with realistic variable speeds
    /// </summary>
    /// <param name="text">The text to type out</param>
    /// <param name="baseIntervalMs">Base delay in milliseconds</param>
    /// <param name="addRandomness">Whether to add random variation to typing speed</param>
public
    static async Task TypeRealisticAsync(string text, int baseIntervalMs = 100, bool addRandomness = true) {
        if (string.IsNullOrEmpty(text)) {
            return;
        }

        Random random = addRandomness ? new Random() : null;

        foreach(char c in text) {
            Console.Write(c);

            int delay = baseIntervalMs;

            if (addRandomness) {
                // Add randomness to make it feel more natural
                delay += random.Next(-30, 50);

                // Longer pauses after punctuation
                if (c == '.' || c == '!' || c == '?') {
                    delay += random.Next(200, 500);
                } else if (c == ',' || c == ';') {
                    delay += random.Next(100, 200);
                } else if (c == ' ') {
                    delay += random.Next(20, 80);
                }
            }

            // Ensure delay is not negative
            delay = Math.Max(delay, 10);

            await Task.Delay(delay);
        }
        Console.WriteLine(); // Add newline at the end
    }

public
    static async Task Main(string[] args) {
        Console.WriteLine("=== Typing Effect Demo ===\n");

        string sampleText = "Hello, World! This is a typing effect demonstration.";

        // Demo 1: Fixed interval between characters
        Console.WriteLine("1. Typing with 0.1 second intervals:");
        await TypeWithIntervalAsync(sampleText, 0.1);

        await Task.Delay(1000); // Pause between demos

        // Demo 2: Complete typing over specific duration
        Console.WriteLine("\n2. Typing over 3 seconds total:");
        await TypeOverDurationAsync(sampleText, 3.0);

        await Task.Delay(1000); // Pause between demos

        // Demo 3: Realistic typing with randomness
        Console.WriteLine("\n3. Realistic typing effect:");
        await TypeRealisticAsync(sampleText, 80, true);

        // Interactive example
        Console.WriteLine("\n=== Interactive Mode ===");
        Console.Write("Enter text to type: ");
        string userText = Console.ReadLine();

        if (!string.IsNullOrEmpty(userText)) {
            Console.Write("Enter interval in seconds (e.g., 0.1): ");
            if (double.TryParse(Console.ReadLine(), out double interval)) {
                Console.WriteLine("\nTyping your text:");
                await TypeWithIntervalAsync(userText, interval);
            } else {
                Console.WriteLine("\nUsing default realistic typing:");
                await TypeRealisticAsync(userText);
            }
        }

        Console.WriteLine("\nPress any key to exit...");
        Console.ReadKey();
    }
}

// Alternative static class for easier usage
public static class ConsoleTyping {
    /// <summary>
    /// Simple typing effect with character interval
    /// </summary>
public
    static async Task TypeAsync(this string text, double intervalSeconds) {
        await TypingEffect.TypeWithIntervalAsync(text, intervalSeconds);
    }

    /// <summary>
    /// Typing effect over total duration
    /// </summary>
public
    static async Task TypeOverAsync(this string text, double totalSeconds) {
        await TypingEffect.TypeOverDurationAsync(text, totalSeconds);
    }
}

// Usage examples in separate methods:
public class UsageExamples {
public
    static async Task Example1() {
        // Type with 0.05 seconds between each character
        await TypingEffect.TypeWithIntervalAsync("This appears slowly...", 0.05);
    }

public
    static async Task Example2() {
        // Type the entire string over 2 seconds
        await TypingEffect.TypeOverDurationAsync("This takes exactly 2 seconds!", 2.0);
    }

public
    static async Task Example3() {
        // Using extension methods
        await "Hello World!".TypeAsync(0.1);
        await "This takes 3 seconds total.".TypeOverAsync(3.0);
    }

public
    static async Task Example4() {
        // Realistic typing with natural pauses
        await TypingEffect.TypeRealisticAsync("This feels like real typing, with natural pauses and rhythm.");
    }
}
 */
