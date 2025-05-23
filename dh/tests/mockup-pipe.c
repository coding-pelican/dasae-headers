#ifndef PIPE_INCLUDED
#define PIPE_INCLUDED

// Check if argument list is empty
#define PIPE_HAS_ARGS(...) \
    PIPE_HAS_ARGS_IMPL(__VA_OPT__(, ) __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0) // 드디어 해냈다 ㅜㅜㅜㅜㅜ
#define PIPE_HAS_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

// Conditional comma macro
#define EMPTY()
#define IGNORE(...)
#define EXPAND(...) __VA_ARGS__
#define DEFER(...)  __VA_ARGS__ EMPTY()
#define COMMA()     ,

// Helper macros
#define PAREN_OPEN() (
#define PAREN_CLOSE() )

#define PIPE_EXPAND_WITH_PARENS_WRAPPED(...) (__VA_ARGS__)
#define PIPE_EXPAND_WITH_PARENS_OPEN(...) (__VA_ARGS__
#define PIPE_EXPAND_WITH_PARENS_CLOSE(...) __VA_ARGS__)

#define IGNORE_OPEN() IGNORE(
#define IGNORE_CLOSE() )
#define COMMA_IGNORE_OPEN() , IGNORE(
#define COMMA_IGNORE_CLOSE() )

#define PIPE_exec(...) \
    pp_join($, __PIPE_exec, __VA_ARGS__)
#define __PIPE_exec$0comma_ignore_open \
    , DEFER(IGNORE)(
#define __PIPE_exec$0comma_ignore_close \
    )
#define __PIPE_exec$0ignore_open \
    DEFER(IGNORE)(
#define __PIPE_exec$0ignore_close \
    )

#define PIPE_REPLACE_COMMA_AND_IGNORE_IF(...) PIPE_IF((PIPE_HAS_ARGS(__VA_ARGS__)), 0comma_ignore_open, 0ignore_open)
#define PIPE_IF(cond, t, f)                   PIPE_IF_IMPL(EXPAND cond, t, f)
#define PIPE_IF_IMPL(cond, t, f)              pp_join(_, PIPE_IF, cond)(t, f)
#define PIPE_IF_1(t, f)                       t
#define PIPE_IF_0(t, f)                       f

// Apply a function with a value and arguments
#define PIPE_MAP(F, ARG...)                     F ARG
#define PIPE_JOIN_VALUE_TO_ARGS(value, args...) PIPE_EXPAND_WITH_PARENS_OPEN value PIPE_exec(PIPE_REPLACE_COMMA_AND_IGNORE_IF args) PIPE_exec(0comma_ignore_close) PIPE_EXPAND_WITH_PARENS_CLOSE args
#define PIPE_APPLY(value, func, args...)        PIPE_MAP(func, PIPE_JOIN_VALUE_TO_ARGS(value, args))

// Process a single step in the pipe
#define PIPE_STEP(prev_result_var, step_num, func, args...)             \
    var ret##step_num = bti_Generic_match$(                             \
        TypeOf(PIPE_APPLY((prev_result_var), func, args)),              \
        bti_Generic_pattern$(void) eval({                               \
            $ignore = PIPE_APPLY((prev_result_var), func, args);        \
            eval_return make$(Void);                                    \
        }),                                                             \
        bti_Generic_fallback_ PIPE_APPLY((prev_result_var), func, args) \
    );

// Generate a unique variable _Name for each step
#define PIPE_RESULT(step_num) ret##step_num

// Main pipe implementation
#define pipe(initial_value...)             comp_syn__pipe(initial_value)
#define comp_syn__pipe(initial_value, ...) ({                    \
    var_(__pipe_initial, TypeOf(initial_value)) = initial_value; \
    PIPE_IMPL(__pipe_initial, ##__VA_ARGS__);                    \
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
#define PIPE1(initial_value, func_args1)                                            \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1) \
    PIPE_RESULT(0)
#define PIPE2(initial_value, func_args1, func_args2)                                 \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_RESULT(1)
#define PIPE3(initial_value, func_args1, func_args2, func_args3)                     \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3) \
    PIPE_RESULT(2)
#define PIPE4(initial_value, func_args1, func_args2, func_args3, func_args4)         \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3) \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4) \
    PIPE_RESULT(3)
#define PIPE5(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5) \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)      \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)     \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)     \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)     \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)     \
    PIPE_RESULT(4)
#define PIPE6(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6) \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                  \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                 \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                 \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                 \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                 \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                 \
    PIPE_RESULT(5)
#define PIPE7(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6, func_args7) \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                              \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                             \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                             \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                             \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                             \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                             \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC func_args7, PIPE_GET_ARGS func_args7)                             \
    PIPE_RESULT(6)
#define PIPE8(initial_value, func_args1, func_args2, func_args3, func_args4, func_args5, func_args6, func_args7, func_args8) \
    PIPE_STEP(initial_value, 0, PIPE_GET_FUNC func_args1, PIPE_GET_ARGS func_args1)                                          \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC func_args2, PIPE_GET_ARGS func_args2)                                         \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC func_args3, PIPE_GET_ARGS func_args3)                                         \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC func_args4, PIPE_GET_ARGS func_args4)                                         \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC func_args5, PIPE_GET_ARGS func_args5)                                         \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC func_args6, PIPE_GET_ARGS func_args6)                                         \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC func_args7, PIPE_GET_ARGS func_args7)                                         \
    PIPE_STEP(PIPE_RESULT(6), 7, PIPE_GET_FUNC func_args8, PIPE_GET_ARGS func_args8)                                         \
    PIPE_RESULT(7)

#endif /* PIPE_INCLUDED */



#include "dh/main.h"
#include "dh/heap/Page.h"
#include <stdio.h>

// Example Foo structure
typedef struct Foo {
    var_(a, i32);
    var_(b, i32);
    var_(value, i32);
    var_(mem, mem_Allocator);
} Foo;
use_Err$(Foo);
static fn_(Foo_init(mem_Allocator allocator), Err$Ptr$Foo) $must_check;
static fn_(Foo_fini(Foo* self), void);
static fn_(Foo_setA(Foo* self, i32 a), Foo*);
static fn_(Foo_setB(Foo* self, i32 b), Foo*);
static fn_(Foo_eval(Foo* self), Foo*);
static fn_(Foo_merge(Foo* self, const Foo* other), Foo*);
static fn_(Foo_baz(const Foo* self), i32);

$maybe_unused static $inline fn_(i32_add(i32 lhs, i32 rhs), i32) { return lhs + rhs; }
$maybe_unused static $inline fn_(i32_addAsg(i32* lhs, i32 rhs), i32*) { return deref(lhs) += rhs, lhs; }



/* fn_(enhancePipeExpanding(Foo foo), void) {
    // PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC (deref_,().value), PIPE_GET_ARGS (deref_,().value))
    // PIPE_STEP(PIPE_RESULT(1), 2, deref_, ().value)
    // var ret1 = 0;
    // var ret2 = PIPE_APPLY((ret1), deref_, ().value);
    // PIPE_MAP(deref_, PIPE_EXPAND_WITH_PARENS_OPEN(ret1) PIPE_REPLACE_COMMA_AND_IGNORE_IF().value PAREN_CLOSE() PIPE_EXPAND_WITH_PARENS_CLOSE().value)
    // PIPE_MAP(deref_, (ret1 IGNORE(.value)).value)
    // PIPE_MAP(deref_, (ret1.value).value)

    let value = pipe(&foo,(Foo_setA,(10)),(&Foo_setB,(20)->value),(i32_addAsg,(123)),(deref,()));
    printf("Enhanced pipe result: %d\n", value);
} */

/* fn_(resolveIssuePipeExpanding(mem_Allocator allocator), void) {
    // #define try   try_
    // #define catch catch_

    // #define MAP(F, ...) F(__VA_ARGS__)
    //     MAP(catch, value PIPE_REPLACE_COMMA_AND_IGNORE_IF(a) EXPAND(a))

    //     PIPE_APPLY(ret, catch, (a));
    //     PIPE_STEP(ret, 0, catch, (a));
    //     var foo = meta_cast$(Foo*, pipe(pipe(allocator, (mem_Allocator_create,(typeInfo$(Foo)))), (catch,(a)), (abc,(def))));
    //     var foo = meta_cast$(Foo*, catch_(mem_Allocator_create(allocator, typeInfo$(Foo)), claim_unreachable));

    // let pipe_ret = pipe(allocator,(mem_Allocator_create,(typeInfo$(Foo))),(catch_from,(err, ({ $ignore = err; claim_unreachable;}))));
    // var foo      = meta_cast$(Foo*, pipe_ret);
    // $ignore = foo;

    let pipe_foo = pipe(pipe(meta_cast$(Foo*, pipe(allocator,
        (mem_Allocator_create,(typeInfo$(Foo))),(catch_,(claim_unreachable))
    )),(deref,())),(ref,()),
        (Foo_setA,(10)),
        (Foo_setB,(20)),
        (Foo_eval,()),
        (Foo_baz,())
    );
    $ignore = pipe_foo;
} */



fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    $ignore       = args;
    let allocator = heap_Page_allocator(create$(heap_Page));

    // Traditional approach
    block_defer {
        let bar = try_(Foo_init(allocator));
        {
            Foo_setA(bar, 5);
            Foo_setB(bar, 15);
            Foo_eval(bar);
        }
        defer_(Foo_fini(bar));

        let foo = try_(Foo_init(allocator));
        defer_(Foo_fini(foo));
        Foo_setA(foo, 10);
        Foo_setB(foo, 20);
        Foo_eval(foo);
        Foo_merge(foo, bar);
        let result = Foo_baz(foo);

        printf("Traditional result: %d\n", result);
    } block_deferral;

    // Using pipe macro
    block_defer {
        let bar = pipe(try_(Foo_init(allocator)),
            (Foo_setA,(5)),
            (Foo_setB,(15)),
            (Foo_eval,())
        );
        defer_(Foo_fini(bar));

        let foo = try_(Foo_init(allocator));
        defer_(Foo_fini(foo));
        let result = pipe(foo,
            (Foo_setA,(10)),
            (Foo_setB,(20)),
            (Foo_eval,()),
            (Foo_merge,(bar)),
            (Foo_baz,())
        );
        printf("Pipe result: %d\n", result);
    } block_deferral;

    return_ok({});
} unscoped_ext;



#define func_(_Name_With_Params, T_Return, ...) \
    pp_overload(func, __VA_ARGS__)(_Name_With_Params, T_Return, __VA_ARGS__)
#define func_0(_Name_With_Params, T_Return, ...) \
    fn_(_Name_With_Params, T_Return)
#define func_1(_Name_With_Params, T_Return, _Body...) \
    fn_scope(_Name_With_Params, T_Return) _Body unscoped
#define func_2(_Name_With_Params, T_Return, _Expand_Type, _Body...) \
    pp_cat(fn_scope_, _Expand_Type)(_Name_With_Params, T_Return) _Body pp_cat(unscoped_, _Expand_Type)

#define $scope          $scope
#define fn_scope_$scope fn_scope_ext
#define unscoped_$scope unscoped_ext

func_(runPipeExampleUsage(Sli$Str_const args), Err$void, $scope, {
    $ignore       = args;
    let allocator = heap_Page_allocator(create$(heap_Page));

    let bar = pipe(try_(Foo_init(allocator)),
        (Foo_setA,(5)),
        (Foo_setB,(15)),
        (Foo_eval,())
    );
    defer_(Foo_fini(bar));

    let foo = try_(Foo_init(allocator));
    defer_(Foo_fini(foo));
    let result = pipe(foo,
        (Foo_setA,(10)),
        (Foo_setB,(20)),
        (Foo_eval,()),
        (Foo_merge,(bar)),
        (Foo_baz,())
    );
    printf("Pipe result: %d\n", result);

    return_ok({});
});



// Example functions that would typically be used in a chain
fn_scope(Foo_init(mem_Allocator allocator), Err$Ptr$Foo) {
    let foo    = meta_cast$(Foo*,
        try_(pipe(allocator,(mem_Allocator_create,(typeInfo$(Foo)))))
    );
    foo->mem   = allocator;
    foo->a     = 0;
    foo->b     = 0;
    foo->value = 0;
    return_ok(foo);
} unscoped;
fn_(Foo_fini(Foo* self), void) {
    mem_Allocator_destroy(deref(self).mem, anyPtr(self));
}
fn_(Foo_setA(Foo* self, i32 a), Foo*) {
    return deref(self).a = a, self;
}
fn_(Foo_setB(Foo* self, i32 b), Foo*) {
    return deref(self).b = b, self;
}
fn_(Foo_eval(Foo* self), Foo*) {
    debug_assert_nonnull(self);
    return self->value = self->a + self->b, self;
}
fn_(Foo_merge(Foo* self, const Foo* other), Foo*) {
    return deref(self).value += deref(other).value, self;
}
fn_(Foo_baz(const Foo* self), i32) {
    return deref(self).value;
}
