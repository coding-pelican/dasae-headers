#ifndef PIPE_INCLUDED
#define PIPE_INCLUDED

// Helper macros
#define PIPE_EXPAND(...) __VA_ARGS__

// Check if argument list is empty
#define PIPE_HAS_ARGS(...) \
    PIPE_HAS_ARGS_HELPER(__VA_OPT__(, ) __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0) // 드디어 해냈다 ㅜㅜㅜㅜㅜ
#define PIPE_HAS_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) \
    N

// Conditional comma macro
#define EMPTY()
#define COMMA()                  ,
#define PIPE_COMMA_IF(...)       PIPE_IF((PIPE_HAS_ARGS(__VA_ARGS__)), (COMMA), (EMPTY))
#define PIPE_IF(cond, t, f)      PIPE_IF_IMPL(PIPE_EXPAND cond, t, f)
#define PIPE_IF_IMPL(cond, t, f) pp_join(_, PIPE_IF, cond)(PIPE_EXPAND t, PIPE_EXPAND f)
#define PIPE_IF_0(t, f)          f()
#define PIPE_IF_1(t, f)          t()

// Apply a function with a value and arguments
#define PIPE_MAP(F, ARG...)              F(ARG)
#define PIPE_APPLY(value, func, args...) PIPE_MAP(func, value PIPE_COMMA_IF args PIPE_EXPAND args)

// Process a single step in the pipe
#define PIPE_STEP(prev_result_var, step_num, func, args...) \
    var ret##step_num = PIPE_APPLY(prev_result_var, func, args);

// Generate a unique variable name for each step
#define PIPE_RESULT(step_num) ret##step_num

// Main pipe implementation
#define pipe(initial_value, ...) ({           \
    var __pipe_initial = (initial_value);     \
    PIPE_IMPL(__pipe_initial, ##__VA_ARGS__); \
})

// Extract function and args from pair
#define PIPE_GET_FUNC(func, args) func
#define PIPE_GET_ARGS(func, args) args

// Internal implementation with 1-5 steps (can be extended for more)
#define PIPE_IMPL(initial_value, func_args1, ...) \
    PIPE_COUNT_ARGS(func_args1, ##__VA_ARGS__, PIPE5, PIPE4, PIPE3, PIPE2, PIPE1)(initial_value, func_args1, ##__VA_ARGS__)

// Counter to determine number of pipe steps
#define PIPE_COUNT_ARGS(arg1, arg2, arg3, arg4, arg5, func, ...) func

// Implementation for different numbers of pipe steps
#define PIPE1(initial_value, func_args1) ({                                         \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1) \
    PIPE_RESULT(0);                                                                 \
})
#define PIPE2(initial_value, func_args1, func_args2) ({                              \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_RESULT(1);                                                                  \
})
#define PIPE3(initial_value, func_args1, func_args2, func_args3) ({                  \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3) \
    PIPE_RESULT(2);                                                                  \
})
#define PIPE4(initial_value, func_args1, func_args2, func_args3, func_args4) ({      \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3) \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4) \
    PIPE_RESULT(3);                                                                  \
})
#define PIPE5(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5) ({ \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)         \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)        \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)        \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)        \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)        \
    PIPE_RESULT(4);                                                                         \
})
#define PIPE6(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6) ({ \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                     \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                    \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                    \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                    \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                    \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                    \
    PIPE_RESULT(5);                                                                                     \
})
#define PIPE7(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6, func_args7) ({ \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                                 \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                                \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                                \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                                \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                                \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                                \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC func_args7, PIPE_GET_ARGS func_args7)                                \
    PIPE_RESULT(6);                                                                                                 \
})
#define PIPE8(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6, func_args7, func_args8) ({ \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                                             \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                                            \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                                            \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                                            \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                                            \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                                            \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC func_args7, PIPE_GET_ARGS func_args7)                                            \
    PIPE_STEP(PIPE_RESULT(6), 7, PIPE_GET_FUNC func_args8, PIPE_GET_ARGS func_args8)                                            \
    PIPE_RESULT(7);                                                                                                             \
})

#endif /* PIPE_INCLUDED */

#include "dh/main.h"
#include "dh/heap/Page.h"
#include <stdio.h>
// #include "pipe.h"

// Example Foo structure
typedef struct Foo {
    var_(a, i32);
    var_(b, i32);
    var_(value, i32);
    var_(mem, mem_Allocator);
} Foo;
static fn_(Foo_init(mem_Allocator allocator), Foo*);
static fn_(Foo_fini(Foo* self), void);
static fn_(Foo_setA(Foo* self, i32 a), Foo*);
static fn_(Foo_setB(Foo* self, i32 b), Foo*);
static fn_(Foo_eval(Foo* self), Foo*);
static fn_(Foo_merge(Foo* self, const Foo* other), Foo*);
static fn_(Foo_baz(const Foo* self), i32);

fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    $ignore args;
    let allocator = heap_Page_allocator(create$(heap_Page));

    // Traditional approach
    block_defer {
        let foo1 = Foo_init(allocator);
        defer_(Foo_fini(foo1));
        {
            Foo_setA(foo1, 10);
            Foo_setB(foo1, 20);
            Foo_eval(foo1);
        }

        let bar = Foo_init(allocator);
        defer_(Foo_fini(bar));
        {
            Foo_setA(bar, 5);
            Foo_setB(bar, 15);
            Foo_eval(bar);
        }

        Foo_merge(foo1, bar);
        let result1 = Foo_baz(foo1);

        printf("Traditional result: %d\n", result1);
    } block_deferral;

    // Using pipe macro
    block_defer {
        let foo2 = Foo_init(allocator);
        defer_(Foo_fini(foo2));

        let bar2 = Foo_init(allocator);
        defer_(Foo_fini(bar2));
        {
            Foo_setA(bar2, 5);
            Foo_setB(bar2, 15);
            Foo_eval(bar2);
        }

        let result2 = pipe(foo2,
            (Foo_setA,(10)),
            (Foo_setB,(20)),
            (Foo_eval,()),
            (Foo_merge,(bar2)),
            (Foo_baz,())
        );
        printf("Pipe result: %d\n", result2);
    } block_deferral;

    return_ok({});
} unscoped_ext;

// Example functions that would typically be used in a chain
fn_(Foo_init(mem_Allocator allocator), Foo*) {
    let foo    = meta_cast$(Foo*, catch_(mem_Allocator_create(allocator, typeInfo$(Foo)), claim_unreachable));
    foo->mem   = allocator;
    foo->a     = 0;
    foo->b     = 0;
    foo->value = 0;
    return foo;
}
fn_(Foo_fini(Foo* self), void) {
    mem_Allocator_destroy(self->mem, anyPtr(self));
}
fn_(Foo_setA(Foo* self, i32 a), Foo*) {
    self->a = a;
    return self;
}
fn_(Foo_setB(Foo* self, i32 b), Foo*) {
    self->b = b;
    return self;
}
fn_(Foo_eval(Foo* self), Foo*) {
    self->value = self->a + self->b;
    return self;
}
fn_(Foo_merge(Foo* self, const Foo* other), Foo*) {
    self->value += other->value;
    return self;
}
fn_(Foo_baz(const Foo* self), i32) {
    return self->value;
}

void resolveIssuePipeExpanding(mem_Allocator allocator) {
    // #define try   try_
    // #define catch catch_

    // #define MAP(F, ...) F(__VA_ARGS__)
    //     MAP(catch, value PIPE_COMMA_IF(a) PIPE_EXPAND(a))

    //     PIPE_APPLY(ret, catch, (a));
    //     PIPE_STEP(ret, 0, catch, (a));
    //     var foo = meta_cast$(Foo*, pipe(pipe(allocator, (mem_Allocator_create,(typeInfo$(Foo)))), (catch,(a)), (abc,(def))));
    //     var foo = meta_cast$(Foo*, catch_(mem_Allocator_create(allocator, typeInfo$(Foo)), claim_unreachable));

    // let pipe_ret = pipe(allocator,(mem_Allocator_create,(typeInfo$(Foo))),(catch_from,(err, ({ $ignore err; claim_unreachable;}))));
    // var foo      = meta_cast$(Foo*, pipe_ret);
    // $ignore foo;

    let pipe_foo = meta_cast$(Foo*, pipe(allocator,
        (mem_Allocator_create,(typeInfo$(Foo))),(catch_,(claim_unreachable))
    ));
    $ignore pipe_foo;
}
