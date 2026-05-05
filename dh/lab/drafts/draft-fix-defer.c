#include "dh-main.h"
#include "dh/mem/Alctr.h"
#include "dh/heap/Page.h"

$extern fn_((doSomethingAndMustContinueWhenFalse(const S$u8* arg))(bool));
$extern fn_((doSomethingAndMustBreakWhenFalse(const S$u8* arg))(bool));

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(mem_Alctr_dupe, mem_Alctr_free));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    var_(heap, heap_Page) = (heap_Page){};
    let gpa = heap_Page_alctr(&heap);
    var_(cursor, usize) = 0;
    for (; cursor < args.len; ++cursor) blk_defer {
        let dupe = catch_((mem_Alctr_dupe$u8($trace gpa, *S_at((args)[cursor])))($ignore, continue));
        defer_(mem_Alctr_free$u8($trace gpa, dupe));
        if (!doSomethingAndMustContinueWhenFalse(&dupe)) continue;
        if (!doSomethingAndMustBreakWhenFalse(&dupe)) break; //<- `break` is not supported in `blk_defer`
    } blk_deferral;

    for (; cursor < args.len; ++cursor) {
        bool __is_broken = true;
        /*begin blk_defer*/ {
            do {
                {
                    const fn__FlowCursorPacked __flow_cursor_prev_line = __flow_cursor.curr_line;
                    __flow_cursor.curr_line = 22;
                    if (((bool)(0))) {
                    case 22:
                        __flow_cursor.curr_line = __flow_cursor_prev_line;
                        if (__flow_cursor.is_returning) {
                            goto __step_deferred;
                        } else {
                            continue;
                        };
                    }
                };
                do /*end blk_defer*/ {
                    let dupe = catch_((mem_Alctr_dupe$u8($trace gpa, *S_at((args)[cursor])))($ignore, continue));
                    defer_(mem_Alctr_free$u8($trace gpa, dupe));
                    if (!doSomethingAndMustContinueWhenFalse(&dupe)) continue;
                    if (!doSomethingAndMustBreakWhenFalse(&dupe)) break;
                } /*begin blk_deferral*/ while ((__is_broken = false), false);
                goto __step_deferred;
            } while (((bool)(0)));
        } /*end blk_deferral*/;
        if (__is_broken) break;
    }

    for (; cursor < args.len; ++cursor) loop_defer {
        let dupe = catch_((mem_Alctr_dupe$u8($trace gpa, *S_at((args)[cursor])))($ignore, continue));
        defer_(mem_Alctr_free$u8($trace gpa, dupe));
        if (!doSomethingAndMustContinueWhenFalse(&dupe)) continue;
        if (!doSomethingAndMustBreakWhenFalse(&dupe)) break;
    } loop_deferral;

    return_ok({});
} $unguarded(fn);
