#include "dh/except.h"

// Initialize thread-local storage
__thread struct ExceptionContext exception_context = {
    .buf       = { { 0 } },
    .exception = { ExceptionType_no_exception, null, null, 0 },
    .try_index = 0
};

void throw_exception(ExceptionType type, const char* message, const char* file, i32 line) {
    if (exception_context.try_index <= 0) {
        ignore fprintf(stderr, "Unhandled exception at %s:%d: %s\n", file, line, message);
        exit(1);
    }

    exception_context.exception.type    = type;
    exception_context.exception.message = message;
    exception_context.exception.file    = file;
    exception_context.exception.line    = line;

    longjmp(exception_context.buf[exception_context.try_index - 1], 1);
}
