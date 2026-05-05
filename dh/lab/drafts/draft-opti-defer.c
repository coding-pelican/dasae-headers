#include <stdbool.h>
#include <stdio.h>

int optExpected(int x) {
    int __result = 0;
    __result = ++x;
    return __result;
}

int optTargetLogic(int x) {
    int __result = 0;
    __result = ++x;
    x += 1;
    x += 2;
    x += 3;
    return __result;
}

/* int optTarget(int x) {
    int __result = 0;
    defer_(x += 3);
    defer_(x += 2);
    defer_(x += 1);
    __result = x;
    return __result;
} */

/* int optTarget(int x) {
    int __result = 0;
    if (false) { x += 3; }
    if (false) { x += 2; }
    if (false) { x += 1; }
    __result = x;
    return __result;
} */

/* clang-format off */
int optTargetHard(int x) {
    int __result = 0;
    if (false) { defer1: x += 3; goto defer0; }
    if (false) { defer2: x += 2; goto defer1; }
    if (false) { defer3: x += 1; goto defer2; }

    __result = ++x;

    goto defer3; defer0: return __result;
}
/* clang-format on */

/* clang-format off */
#define defer_block \
    __attribute__((unused)) bool _d_returning = false; int _d_curr = 0; \
    _d_start: switch (_d_curr) { \
        case -1: break; \
        case 0: _d_curr = -1;
#define _DEFER_SNAPSHOT(A) { int _d_prev = _d_curr; _d_curr = __LINE__; if(0) { case __LINE__: _d_curr = _d_prev; A; } }
#define defer(F) _DEFER_SNAPSHOT(F; goto _d_start)
#define run_deferred() goto _d_start; default: __builtin_unreachable(); }
#define defer_return { _d_returning = true; goto _d_start; }
/* clang-format on */

/* clang-format off */
int optTargetSwitchOnlyOne(int x) {
    int __result = 0;
    __attribute__((unused)) bool _d_returning = false; int _d_curr = 0; _d_start: switch (_d_curr) {
        case -1: break;
        case 0: _d_curr = -1; {
        { const int _d_prev = _d_curr; _d_curr = 63; if (false) { case 63: x += 3; _d_curr = _d_prev; goto _d_start; } };
        __result = ++x; { _d_returning = true; goto _d_start; };
    } goto _d_start; default: __builtin_unreachable(); };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitchOnlyTwo(int x) {
    int __result = 0;
    __attribute__((unused)) bool _d_returning = false; int _d_curr = 0; _d_start: switch (_d_curr) {
        case -1: break;
        case 0: _d_curr = -1; {
        { const int _d_prev = _d_curr; _d_curr = 63; if (false) { case 63: x += 3; _d_curr = _d_prev; goto _d_start; } };
        { const int _d_prev = _d_curr; _d_curr = 64; if (false) { case 64: x += 2; _d_curr = _d_prev; goto _d_start; } };
        __result = ++x; { _d_returning = true; goto _d_start; };
    } goto _d_start; default: __builtin_unreachable(); };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitch(int x) {
    int __result = 0;
    __attribute__((unused)) bool _d_returning = false; int _d_curr = 0; _d_start: switch (_d_curr) {
        case -1: break;
        case 0: _d_curr = -1; {
        { const int _d_prev = _d_curr; _d_curr = 63; if (false) { case 63: x += 3; _d_curr = _d_prev; goto _d_start; } };
        { const int _d_prev = _d_curr; _d_curr = 64; if (false) { case 64: x += 2; _d_curr = _d_prev; goto _d_start; } };
        { const int _d_prev = _d_curr; _d_curr = 65; if (false) { case 65: x += 1; _d_curr = _d_prev; goto _d_start; } };
        __result = ++x; { _d_returning = true; goto _d_start; };
    } goto _d_start; default: __builtin_unreachable(); };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitchBreak(int x) {
    int __result = 0;
    int _d_curr = 0; _d_start: switch (_d_curr) {
        case 0: _d_curr = -1; {
        { const int _d_prev = _d_curr; _d_curr = 63; if (false) { case 63: x += 3; _d_curr = _d_prev; goto _d_start; } };
        { const int _d_prev = _d_curr; _d_curr = 64; if (false) { case 64: x += 2; _d_curr = _d_prev; goto _d_start; } };
        { const int _d_prev = _d_curr; _d_curr = 65; if (false) { case 65: x += 1; _d_curr = _d_prev; goto _d_start; } };
        __result = ++x; { goto _d_start; };
    } goto _d_start; default: break; };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitchHint(int x) {
    int __result = 0;
    int _d_curr = 0; _d_start: switch (_d_curr) {
        case -1: break;
        case 0: { _d_curr = -1; {
        { const int _d_prev = _d_curr; _d_curr = 63; if (false) { case 63: { x += 3; _d_curr = _d_prev; goto _d_start; } __builtin_unreachable(); } };
        { const int _d_prev = _d_curr; _d_curr = 64; if (false) { case 64: { x += 2; _d_curr = _d_prev; goto _d_start; } __builtin_unreachable(); } };
        { const int _d_prev = _d_curr; _d_curr = 65; if (false) { case 65: { x += 1; _d_curr = _d_prev; goto _d_start; } __builtin_unreachable(); } };
        __result = ++x;
    } goto _d_start; } __builtin_unreachable(); default: __builtin_unreachable(); };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitchStatic(int x) {
    int __result = 0;
    __attribute__((unused)) bool _d_returning = false; int _d_curr = 0; _d_start: switch (_d_curr) {
        case -1: break;
        case 0: _d_curr = -1; {
        { static const int CASE = 1; const int _d_prev = _d_curr; _d_curr = CASE; if (false) { case CASE: x += 3; _d_curr = _d_prev; goto _d_start; } };
        { static const int CASE = 2; const int _d_prev = _d_curr; _d_curr = CASE; if (false) { case CASE: x += 2; _d_curr = _d_prev; goto _d_start; } };
        { static const int CASE = 3; const int _d_prev = _d_curr; _d_curr = CASE; if (false) { case CASE: x += 1; _d_curr = _d_prev; goto _d_start; } };
        __result = ++x; { _d_returning = true; goto _d_start; };
    } goto _d_start; default: __builtin_unreachable(); };
    return __result;
}
/* clang-format on */

/* clang-format off */
int optTargetSwitchGoto(int x) {
    int __result = 0;
    const void* __d_curr = &&defer0;
    { const void* const __d_prev = __d_curr; __d_curr = &&defer1; if (false) { defer1: x += 3; __d_curr = __d_prev; goto *__d_curr; } };
    { const void* const __d_prev = __d_curr; __d_curr = &&defer2; if (false) { defer2: x += 2; __d_curr = __d_prev; goto *__d_curr; } };
    { const void* const __d_prev = __d_curr; __d_curr = &&defer3; if (false) { defer3: x += 1; __d_curr = __d_prev; goto *__d_curr; } };
    __result = ++x;
    goto *__d_curr;
defer0: return __result;
}
/* clang-format on */
