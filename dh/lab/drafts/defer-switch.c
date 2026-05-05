#ifndef DEFER_BY_COUNTER_H
#define DEFER_BY_COUNTER_H

/* NOLINTBEGIN(bugprone-terminating-continue) */

#define defer_scope \
    __attribute__((unused)) int _defer_return_ = 0; \
    int _defer_curr_ = 0; \
    enum { _defer_base_ = __COUNTER__ }; \
_deferred_: \
    switch (_defer_curr_) { \
    default: __builtin_unreachable(); \
    case -1: break; \
    case 0: _defer_curr_ = -1;

#define defer_unscope \
    goto _deferred_; \
    } \
    do { /* nothing */ \
    } while (0)

#define defer_(_expr...) \
    DEFER_((__COUNTER__ - _defer_base_), _expr)

#define defer_block \
    do { \
    DEFER_BLOCK((__COUNTER__ - _defer_base_))

#define defer_unblock \
    goto _deferred_; \
    } \
    while (0)

#define break_defer \
    { goto _deferred_; }
#define return_defer \
    { \
        _defer_return_ = 1; \
        goto _deferred_; \
    }

/* NOLINTEND(bugprone-terminating-continue) */

#define DEFER_(_id, _expr...) DEFER___EMIT(_id, _expr)
#define DEFER___EMIT(_id, _expr...) \
    { \
        int _defer_prev_ = _defer_curr_; \
        _defer_curr_ = _id; \
        if (0) { \
        case _id: \
            _defer_curr_ = _defer_prev_; \
            _expr; \
            goto _deferred_; \
        } \
    }

#define DEFER_BLOCK(_id) DEFER_BLOCK__EMIT(_id)
#define DEFER_BLOCK__EMIT(_id) \
    { \
        int _defer_prev_ = _defer_curr_; \
        _defer_curr_ = _id; \
        if (0) { \
        case _id: \
            _defer_curr_ = _defer_prev_; \
            if (_defer_return_) goto _deferred_; \
            else continue; \
        } \
    }

#endif /* DEFER_BY_COUNTER_H */


#define deferred ({ \
    __label__ _deferred_; \
    __attribute__((unused)) int _defer_return_ = 0; \
    int _defer_curr_ = 0; \
    enum { _defer_base_ = __COUNTER__ }; \
_deferred_: \
    switch (_defer_curr_) { \
    default: __builtin_unreachable(); \
    case -1: break; \
    case 0: _defer_curr_ = -1;

#define undeferred \
    goto _deferred_; \
    } \
})

void test(void) {
    int a = 0;
    defer_scope {
        switch (a) {
        case 0: deferred {
            defer_(a += 1);
        } undeferred;
        }
    } defer_unscope;
}
