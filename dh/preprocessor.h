/*========== MACROS DEFINITION ==============================================*/

#define pp_unused(...)                                                \
    /**                                                               \
     * @brief Marks variables or expressions as intentionally unused  \
    to suppress compiler warnings                                     \
     * @param ... Variable number of arguments to be marked as unused \
     */                                                               \
    RETURN_pp_unused(__VA_ARGS__)

#define pp_ignore                                                        \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    RETURN_pp_ignore

#define pp_ensureNotNull(_x, ...) \
    pp_ensureNotNull(_x, __VA_ARGS__)

#define pp_func(...)                                                     \
    /**                                                                  \
     * @brief Creates a single statement block from multiple expressions \
     * @param ... Multiple statements to be executed as a single block   \
     * @details Wraps multiple statements in a do-while(0) block         \
    for macro safety                                                     \
     */                                                                  \
    RETURN_pp_func(__VA_ARGS__)

#define pp_concat(_token, ...) \
    RETURN_pp_concat(_token, __VA_ARGS__)

/*========== MACROS IMPLEMENTATION ==========================================*/

#define RETURN_pp_unused(...) \
    ((void)(__VA_ARGS__))

#define RETURN_pp_ignore \
    (void)

#define RETURN_pp_ensureNotNull(_x, ...) \
    ((_x) ? (_x) : (__VA_ARGS__))

#define RETURN_pp_func(...) \
    do { __VA_ARGS__ } while (0)

#define RETURN_pp_concat(_token, ...) \
    _token##__VA_ARGS__
