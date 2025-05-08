#ifndef PIPE_INCLUDED
#define PIPE_INCLUDED
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

// Check if argument list is empty
#define PIPE_HAS_ARGS(...) \
    PIPE_HAS_ARGS_IMPL(__VA_OPT__(, ) __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0) // 드디어 해냈다 ㅜㅜㅜㅜㅜ
#define PIPE_HAS_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

// Conditional comma macro
#define PIPE_EMPTY()
#define PIPE_IGNORE(...)
#define PIPE_EXPAND(...) __VA_ARGS__
#define PIPE_DEFER(...)  __VA_ARGS__ PIPE_EMPTY()
#define PIPE_COMMA()     ,

// Helper macros
#define PAREN_OPEN() (
#define PAREN_CLOSE() )

#define PIPE_EXPAND_WITH_PARENS_WRAPPED(...) (__VA_ARGS__)
#define PIPE_EXPAND_WITH_PARENS_OPEN(...) (__VA_ARGS__
#define PIPE_EXPAND_WITH_PARENS_CLOSE(...) __VA_ARGS__)

#define PIPE_IGNORE_OPEN() PIPE_IGNORE(
#define PIPE_IGNORE_CLOSE() )
#define PIPE_COMMA_IGNORE_OPEN() , PIPE_IGNORE(
#define PIPE_COMMA_IGNORE_CLOSE() )

#define PIPE_exec(...) \
    pp_join($, __PIPE_exec, __VA_ARGS__)
#define __PIPE_exec$0comma_ignore_open \
    , PIPE_DEFER(PIPE_IGNORE)(
#define __PIPE_exec$0comma_ignore_close \
    )
#define __PIPE_exec$0ignore_open \
    PIPE_DEFER(PIPE_IGNORE)(
#define __PIPE_exec$0ignore_close \
    )

#define PIPE_REPLACE_COMMA_AND_IGNORE_IF(...) PIPE_IF((PIPE_HAS_ARGS(__VA_ARGS__)), 0comma_ignore_open, 0ignore_open)
#define PIPE_IF(_cond, _t, _f)                PIPE_IF_IMPL(PIPE_EXPAND _cond, _t, _f)
#define PIPE_IF_IMPL(_cond, _t, _f)           pp_join(_, PIPE_IF, _cond)(_t, _f)
#define PIPE_IF_1(_t, _f)                     _t
#define PIPE_IF_0(_t, _f)                     _f

// Apply a function with a value and arguments
#define PIPE_MAP(F, ARG...)                     F ARG
#define PIPE_JOIN_VALUE_TO_ARGS(value, args...) PIPE_EXPAND_WITH_PARENS_OPEN value PIPE_exec(PIPE_REPLACE_COMMA_AND_IGNORE_IF args) PIPE_exec(0comma_ignore_close) PIPE_EXPAND_WITH_PARENS_CLOSE args
#define PIPE_APPLY(value, func, args...)        PIPE_MAP(func, PIPE_JOIN_VALUE_TO_ARGS(value, args))

// Process a single step in the pipe
#define PIPE_STEP(prev_result_var, step_num, func, args...)             \
    var ret##step_num = bti_Generic_match$(                             \
        TypeOf(PIPE_APPLY((prev_result_var), func, args)),              \
        bti_Generic_pattern$(void) eval({                               \
            $ignore PIPE_APPLY((prev_result_var), func, args);          \
            eval_return make$(Void);                                    \
        }),                                                             \
        bti_Generic_fallback_ PIPE_APPLY((prev_result_var), func, args) \
    );

// Generate a unique variable name for each step
#define PIPE_RESULT(step_num) ret##step_num

// Main pipe implementation
#define pipe(initial_value...)             comp_syn__pipe(initial_value)
#define comp_syn__pipe(initial_value, ...) ({ \
    var __pipe_initial = initial_value;       \
    PIPE_IMPL(__pipe_initial, ##__VA_ARGS__); \
})

// Extract function and args from pair
#define PIPE_GET_FUNC(_func, _args) _func
#define PIPE_GET_ARGS(_func, _args) _args

// Internal implementation with 1-5 steps (can be extended for more)
#define PIPE_IMPL(_initial_value, _func_args1, ...) \
    PIPE_COUNT_ARGS(_func_args1, ##__VA_ARGS__, PIPE5, PIPE4, PIPE3, PIPE2, PIPE1)(_initial_value, _func_args1, ##__VA_ARGS__)

// Counter to determine number of pipe steps
#define PIPE_COUNT_ARGS(arg1, arg2, arg3, arg4, arg5, _func, ...) _func

// Implementation for different numbers of pipe steps
#define PIPE1(_initial_value, _func_args1)                                             \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1) \
    PIPE_RESULT(0)
#define PIPE2(_initial_value, _func_args1, _func_args2)                                \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1) \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2) \
    PIPE_RESULT(1)
#define PIPE3(_initial_value, _func_args1, _func_args2, _func_args3)                   \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1) \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3) \
    PIPE_RESULT(2)
#define PIPE4(_initial_value, _func_args1, _func_args2, _func_args3, _func_args4)      \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1) \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2) \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3) \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC _func_args4, PIPE_GET_ARGS _func_args4) \
    PIPE_RESULT(3)
#define PIPE5(_initial_value, _func_args1, _func_args2, _func_args3, _func_args4, _func_args5) \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1)         \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2)         \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3)         \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC _func_args4, PIPE_GET_ARGS _func_args4)         \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC _func_args5, PIPE_GET_ARGS _func_args5)         \
    PIPE_RESULT(4)
#define PIPE6(_initial_value, _func_args1, _func_args2, _func_args3, _func_args4, _func_args5, _func_args6) \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1)                      \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2)                      \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3)                      \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC _func_args4, PIPE_GET_ARGS _func_args4)                      \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC _func_args5, PIPE_GET_ARGS _func_args5)                      \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC _func_args6, PIPE_GET_ARGS _func_args6)                      \
    PIPE_RESULT(5)
#define PIPE7(_initial_value, _func_args1, _func_args2, _func_args3, _func_args4, _func_args5, _func_args6, _func_args7) \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1)                                   \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2)                                   \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3)                                   \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC _func_args4, PIPE_GET_ARGS _func_args4)                                   \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC _func_args5, PIPE_GET_ARGS _func_args5)                                   \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC _func_args6, PIPE_GET_ARGS _func_args6)                                   \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC _func_args7, PIPE_GET_ARGS _func_args7)                                   \
    PIPE_RESULT(6)
#define PIPE8(_initial_value, _func_args1, _func_args2, _func_args3, _func_args4, _func_args5, _func_args6, _func_args7, _func_args8) \
    PIPE_STEP(_initial_value, 0, PIPE_GET_FUNC _func_args1, PIPE_GET_ARGS _func_args1)                                                \
    PIPE_STEP(PIPE_RESULT(0), 1, PIPE_GET_FUNC _func_args2, PIPE_GET_ARGS _func_args2)                                                \
    PIPE_STEP(PIPE_RESULT(1), 2, PIPE_GET_FUNC _func_args3, PIPE_GET_ARGS _func_args3)                                                \
    PIPE_STEP(PIPE_RESULT(2), 3, PIPE_GET_FUNC _func_args4, PIPE_GET_ARGS _func_args4)                                                \
    PIPE_STEP(PIPE_RESULT(3), 4, PIPE_GET_FUNC _func_args5, PIPE_GET_ARGS _func_args5)                                                \
    PIPE_STEP(PIPE_RESULT(4), 5, PIPE_GET_FUNC _func_args6, PIPE_GET_ARGS _func_args6)                                                \
    PIPE_STEP(PIPE_RESULT(5), 6, PIPE_GET_FUNC _func_args7, PIPE_GET_ARGS _func_args7)                                                \
    PIPE_STEP(PIPE_RESULT(6), 7, PIPE_GET_FUNC _func_args8, PIPE_GET_ARGS _func_args8)                                                \
    PIPE_RESULT(7)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PIPE_INCLUDED */
