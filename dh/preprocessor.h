#define pp_unused(...)                                                                             \
    /**                                                                                            \
     * @brief Marks variables or expressions as intentionally unused to suppress compiler warnings \
     * @param ... Variable number of arguments to be marked as unused                              \
     */                                                                                            \
    ((void)(__VA_ARGS__))

#define pp_ignore                                                      \
    /**                                                                \
     * @brief Explicitly ignores an expression or return value         \
     * @details Used to suppress compiler warnings about unused values \
     */                                                                \
    (void)

#define pp_func(...)                                                              \
    /**                                                                           \
     * @brief Creates a single statement block from multiple expressions          \
     * @param ... Multiple statements to be executed as a single block            \
     * @details Wraps multiple statements in a do-while(0) block for macro safety \
     */                                                                           \
    do { __VA_ARGS__ } while (0)
