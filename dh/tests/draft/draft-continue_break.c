#include "dh/main.h"
#include "dh/io/stream.h"



fn_((getNext(void))(O$i32) $scope) {
    return_some(1);
} $unscoped_(fn);

typedef enum_(Loop__ControlFlow $bits(8)) {
    Loop__ControlFlow_continue = 0,
    Loop__ControlFlow_break = 1
} Loop__ControlFlow;

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    {
        while (true) {
            if_some((getNext())(value)) {
                io_stream_println(u8_l("value: {:i}"), value);
                continue;
            };
        }
    }

    {
#if UNUSED_CODE
        while (true) {
            $local_label __loop_control;
            $attr($maybe_unused)
            Loop__ControlFlow __control_flow = Loop__ControlFlow_continue;

            _body;

            goto __loop_control;
__loop_control:
            if (__control_flow == Loop__ControlFlow_break) { break; }
        }
#endif /* UNUSED_CODE */
    }

    {
        while (true) {
            $local_label __loop_control;
            $attr($maybe_unused)
            Loop__ControlFlow __control_flow = Loop__ControlFlow_continue;
            /* begin */
            if_some((getNext())(value)) {
                io_stream_println(u8_l("value: {:i}"), value);
                goto __loop_control;
            } else {
                __control_flow = Loop__ControlFlow_break;
                goto __loop_control;
            }
            /* end */
            goto __loop_control;
__loop_control:
            if (__control_flow == Loop__ControlFlow_break) { break; }
        }
    }


    while (true) {
        $local_label __loop_control;
        $attr($maybe_unused)
        Loop__ControlFlow __control_flow = Loop__ControlFlow_continue;
        /* begin */
        eval_(i32 $scope)(if_some((getNext())(value)) {
            io_stream_println(u8_l("value: {:i}"), value);
            goto __loop_control;
        } else_none {
            $break_(123);
        }) $unscoped_(eval);
        /* end */
        goto __loop_control;
__loop_control:
        if (__control_flow == Loop__ControlFlow_break) { break; }
    }

#if UNUSED_CODE
    {
        init;
        for (bool __87_8_run_once = ((bool)0); !__87_8_run_once; ({ __87_8_run_once = true; fini; })) {
        }
    }
#endif /* UNUSED_CODE */

    // with_((init)$defer(fini)) {
    // } $end(with)

    return_ok({});
} $unscoped_(fn);
