#include "dh/builtin.h"
#include "dh/core.h"

/* Generic error interface */

typedef enum ErrorType ErrorType; // Common error types
typedef struct ErrorVT ErrorVT;
typedef struct Error   Error;
ErrorType              Error_type(Error self);               // Get error type
const char*            Error_message(Error self);            // Get error message string
bool                   Error_is(Error self, ErrorType type); // Check if error is of specific type

enum ErrorType {
    ErrorType_None         = 0,
    ErrorType_OutOfMemory  = 1, // Allocation
    ErrorType_NotFound     = 2, // IO
    ErrorType_InvalidInput = 3, // Parse
    // Add more error types
};
struct ErrorVT {
    ErrorType (*type)(anyptr ctx); // Could be an enum or other identifier
    const char* (*message)(anyptr ctx);
};
struct Error {
    anyptr         ctx;
    const ErrorVT* vt;
};
ErrorType   Error_type(Error self) { return self.vt->type(self.ctx); }
const char* Error_message(Error self) { return self.vt->message(self.ctx); }
bool        Error_is(Error self, ErrorType type) { return self.vt->type(self.ctx) == type; }

/* Generic result type */

typedef enum ResultTag  ResultTag;
typedef struct ResultVT ResultVT;
typedef struct Result   Result;
bool                    Result_isErr(Result self);
bool                    Result_isOk(Result self);
anyptr                  Result_err(Result self);
anyptr                  Result_ok(Result self);

enum ResultTag {
    ResultTag_err = 0,
    ResultTag_ok  = 1
};
struct ResultVT {
    bool (*isErr)(anyptr ctx);
    bool (*isOk)(anyptr ctx);
    anyptr (*err)(anyptr ctx);
    anyptr (*ok)(anyptr ctx);
};
struct Result {
    anyptr          ctx;
    const ResultVT* vt;
};
bool   Result_isErr(Result self) { return self.vt->isErr(self.ctx); }
bool   Result_isOk(Result self) { return self.vt->isOk(self.ctx); }
anyptr Result_err(Result self) { return self.vt->err(self.ctx); }
anyptr Result_ok(Result self) { return self.vt->ok(self.ctx); }

/* Generic error result type */

// typedef enum ErrorResultTag  ErrorResultTag;
// typedef struct ErrorResultVT ErrorResultVT;
// typedef struct ErrorResult   ErrorResult;
// bool                         ErrorResult_isErr(ErrorResult self);
// bool                         ErrorResult_isOk(ErrorResult self);
// Error                        ErrorResult_err(ErrorResult self);
// anyptr                       ErrorResult_ok(ErrorResult self);

// enum ErrorResultTag {
//     ErrorResultTag_err = 0,
//     ErrorResultTag_ok  = 1
// };
// struct ErrorResultVT {
//     bool (*isErr)(anyptr ctx);
//     bool (*isOk)(anyptr ctx);
//     Error (*err)(anyptr ctx);
//     anyptr (*ok)(anyptr ctx);
// };
// struct ErrorResult {
//     anyptr               ctx;
//     const ErrorResultVT* vt;
// };
// bool   ErrorResult_isErr(ErrorResult self) { return self.vt->isErr(self.ctx); }
// bool   ErrorResult_isOk(ErrorResult self) { return self.vt->isOk(self.ctx); }
// Error  ErrorResult_err(ErrorResult self) { return self.vt->err(self.ctx); }
// anyptr ErrorResult_ok(ErrorResult self) { return self.vt->ok(self.ctx); }

typedef struct ErrorResult ErrorResult;
typedef struct ErrorResult {
    ResultTag tag;
    union {
        Error  err; // Error interface
        anyptr ok;  // Generic value
    };
} ErrorResult;
bool ErrorResult_isErr(ErrorResult self) { return self.tag == ResultTag_err; }
bool ErrorResult_isOk(ErrorResult self) { return self.tag == ResultTag_ok; }
#define ErrorResult_err(_val_err...) { .tag = ResultTag_err, .err = (_val_err) }
#define ErrorResult_ok(_val_ok...)   { .tag = ResultTag_ok, .ok = (_val_ok) }

typedef union ErrorVoid {
    ErrorResult base;
    struct {
        ResultTag tag;
        union {
            Error err;
            Void  ok;
        };
    };
} ErrorVoid;
bool ErrorVoid_isErr(ErrorVoid self) { return self.tag == ResultTag_err; }
bool ErrorVoid_isOk(ErrorVoid self) { return self.tag == ResultTag_ok; }
#define ErrorVoid_err(_val_err...) ((ErrorVoid){ .base = ErrorResult_err(_val_err) })
#define ErrorVoid_ok()             ((ErrorVoid){ .tag = ResultTag_ok, .ok = {} })

// /* Implementing ErrorVoid with the ErrorResult interface */

// typedef struct ErrorVoid ErrorVoid;
// ErrorResult               ErrorVoid_errorResult(ErrorVoid* self);
// bool                      ErrorVoid_isErr(anyptr ctx);
// bool                      ErrorVoid_isOk(anyptr ctx);
// Error                     ErrorVoid_err(anyptr ctx);
// anyptr                    ErrorVoid_ok(anyptr ctx);

// struct ErrorVoid {
//     ErrorResultTag tag;
//     union {
//         Error err; // Error interface
//         Void  ok;
//     };
// };
// ErrorResult ErrorVoid_errorResult(ErrorVoid* self) {
//     static const ErrorResultVT vt[1] = { {
//         .isErr = ErrorVoid_isErr,
//         .isOk  = ErrorVoid_isOk,
//         .err   = ErrorVoid_err,
//         .ok    = ErrorVoid_ok,
//     } };
//     return (ErrorResult){
//         .ctx = self,
//         .vt  = vt
//     };
// }
// bool ErrorVoid_isErr(anyptr ctx) {
//     let self = (ErrorVoid*)ctx;
//     return self->tag == ErrorResultTag_err;
// }
// bool ErrorVoid_isOk(anyptr ctx) {
//     let self = (ErrorVoid*)ctx;
//     return self->tag == ErrorResultTag_ok;
// }
// Error ErrorVoid_err(anyptr ctx) {
//     let self = (ErrorVoid*)ctx;
//     return self->err;
// }
// anyptr ErrorVoid_ok(anyptr ctx) {
//     let self = (ErrorVoid*)ctx;
//     return &self->ok;
// }

// Define error sets as structures that can be merged
#define ERROR_SET(name, ...) \
    typedef struct name {    \
        ErrorType type;      \
    } name;                  \
    enum {                   \
        __VA_ARGS__          \
    }

// Example error sets
ERROR_SET(
    FileOpenError,
    FileOpenError_AccessDenied = 1,
    FileOpenError_OutOfMemory,
    FileOpenError_FileNotFound
);

ERROR_SET(
    AllocationError,
    AllocationError_OutOfMemory = FileOpenError_OutOfMemory // Shared error value
);

/* Implementing AllocError with the Error interface */

typedef struct mem_AllocError mem_AllocError;
Error                         mem_AllocError_error(mem_AllocError* self);
ErrorType                     mem_AllocError_type(anyptr ctx);
const char*                   mem_AllocError_message(anyptr ctx);

struct mem_AllocError {
    ErrorType type;
};
Error mem_AllocError_error(mem_AllocError* self) {
    static const ErrorVT vt[1] = { {
        .type    = mem_AllocError_type,
        .message = mem_AllocError_message,
    } };
    return (Error){
        .ctx = self,
        .vt  = vt
    };
}
ErrorType mem_AllocError_type(anyptr ctx) {
    let self = (mem_AllocError*)ctx;
    return self->type;
}
const char* mem_AllocError_message(anyptr ctx) {
    let self = (mem_AllocError*)ctx;
    switch (self->type) {
    case ErrorType_OutOfMemory:
        return "Out of memory";
    default:
        return "Unknown allocator error";
    }
}

/* Example custom error type */

typedef struct io_ParseError io_ParseError;
Error                        io_ParseError_error(io_ParseError* self);
ErrorType                    io_ParseError_type(anyptr ctx);
const char*                  io_ParseError_message(anyptr ctx);

struct io_ParseError {
    ErrorType type;
    i32       line;
    i32       column;
};
Error io_ParseError_error(io_ParseError* self) {
    static const ErrorVT vt[1] = { {
        .type    = io_ParseError_type,
        .message = io_ParseError_message,
    } };
    return (Error){
        .ctx = self,
        .vt  = vt
    };
}
ErrorType io_ParseError_type(anyptr ctx) {
    let self = (io_ParseError*)ctx;
    return self->type;
}
const char* io_ParseError_message(anyptr ctx) {
    let self = (io_ParseError*)ctx;
    unused(self);
    // Could use a static buffer for formatting if needed
    return "Invalid input";
}

/* Error handling macros */

#define try(_Expr)                               \
    ({                                           \
        var _result = (_Expr);                   \
        if (_result.is_err) {                    \
            return ErrorResult_err(_result.err); \
        }                                        \
        _result.ok;                              \
    })

#define catch(_Expr, _result, _Body...) \
    ({                                  \
        var _result = (_Expr);          \
        if (_result.is_err) {           \
            _Body                       \
        }                               \
        result.ok;                      \
    })

/* Example usage */

#include "dh/core.h"

// #include "dh/builtin.h"
// #include <stdbool.h>
// #include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// typedef uint8_t  u8;
// typedef uint64_t usize;
// #define anyptr anyptr
// typedef struct Void {
//     u8 unused_[1];
// } Void;
// #define unused(...) ((void)(__VA_ARGS__))
// #define null        NULL

typedef struct mem_Allocator         mem_Allocator;
typedef struct mem_AllocatorVT       mem_AllocatorVT;
typedef struct mem_AllocError$anyptr mem_AllocError$anyptr;

struct mem_Allocator {
    anyptr                 ctx;
    const mem_AllocatorVT* vt;
};

struct mem_AllocatorVT {
    anyptr (*alloc)(anyptr ctx, usize size);
    anyptr (*realloc)(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
    void (*free)(anyptr ctx, anyptr ptr, usize size);
};

struct mem_AllocError$anyptr {
    bool is_err;
    union {
        mem_AllocError err;
        anyptr         ok;
    };
};

typedef struct heap_C heap_C;

mem_Allocator heap_C_allocator(heap_C* self);

static anyptr heap_C_alloc(anyptr ctx, usize size);
static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
static void   heap_C_free(anyptr ctx, anyptr ptr, usize size);

typedef struct heap_C {
    Void unused_;
} heap_C;

mem_Allocator heap_C_allocator(heap_C* self) {
    static const mem_AllocatorVT vt[1] = { {
        .alloc   = heap_C_alloc,
        .realloc = heap_C_realloc,
        .free    = heap_C_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt  = vt,
    };
}

static anyptr heap_C_alloc(anyptr ctx, usize size) {
    unused(ctx);
    return malloc(size);
}

static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size) {
    unused(ctx);
    unused(old_size);
    return realloc(ptr, new_size);
}

static void heap_C_free(anyptr ctx, anyptr ptr, usize size) {
    unused(ctx);
    unused(size);
    free(ptr);
}

typedef struct Slice Slice;
struct Slice {
    anyptr ptr;
    usize  len;
};

typedef struct ArrayList                ArrayList;
typedef struct mem_AllocError$ArrayList mem_AllocError$ArrayList;
typedef struct mem_AllocError$Void      mem_AllocError$Void;

static mem_AllocError$ArrayList ArrayList_init(usize item_size, mem_Allocator allocator);
static void                     ArrayList_fini(ArrayList* self);
static mem_AllocError$Void      ArrayList_append(ArrayList* self, const anyptr item);
static Slice                    ArrayList_slice(const ArrayList* self);

struct ArrayList {
    mem_Allocator allocator;
    Slice         items;
    usize         capacity;
    usize         item_size;
};

struct mem_AllocError$ArrayList {
    bool is_err;
    union {
        mem_AllocError err;
        ArrayList      ok;
    };
};
static mem_AllocError$ArrayList ArrayList_init(usize item_size, mem_Allocator allocator) {
    anyptr const ptr = allocator.vt->alloc(allocator.ctx, item_size * 8); // Initial capacity of 8
    if (!ptr) {
        return (mem_AllocError$ArrayList){
            .is_err = true,
            .err    = {
                   .type = ErrorType_OutOfMemory,
            }
        };
    }
    return (mem_AllocError$ArrayList){
        .ok = {
            .allocator = allocator,
            .items     = {
                    .ptr = ptr,
                    .len = 0,
            },
            .capacity  = 8,
            .item_size = item_size,
        }
    };
}

static void ArrayList_fini(ArrayList* self) {
    self->allocator.vt->free(self->allocator.ctx, self->items.ptr, self->capacity * self->item_size);
    self->items.ptr = null;
    self->items.len = 0;
    self->capacity  = 0;
}

struct mem_AllocError$Void {
    bool is_err;
    union {
        mem_AllocError err;
        Void           ok;
    };
};
static mem_AllocError$Void ArrayList_append(ArrayList* self, const anyptr item) {
    if (self->items.len >= self->capacity) {
        const usize  new_capacity = self->capacity * 2;
        anyptr const new_ptr      = self->allocator.vt->realloc(
            self->allocator.ctx,
            self->items.ptr,
            self->capacity * self->item_size,
            new_capacity * self->item_size
        );
        if (!new_ptr) {
            return (mem_AllocError$Void){
                .err = {
                    .type = ErrorType_OutOfMemory,
                }
            };
        }

        self->items.ptr = new_ptr;
        self->capacity  = new_capacity;
    }

    anyptr const dest = (u8*)self->items.ptr + (self->items.len * self->item_size);
    memcpy(dest, item, self->item_size);
    self->items.len++;

    return (mem_AllocError$Void){
        .ok = (Void){}
    };
}

static Slice ArrayList_slice(const ArrayList* self) {
    return self->items;
}

ErrorVoid dh_main(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {
    const ErrorVoid result = dh_main(argc, argv);
    if (ErrorVoid_isOk(result)) { return 0; }
    ignore fprintf(
        stderr,
        "Program failed: %s (type: %d)\n",
        Error_message(result.err),
        Error_type(result.err)
    );
    return 1;
}

// Example usage in main
ErrorVoid dh_main(int argc, const char* argv[]) {
    unused(argc);
    unused(argv);

    // Initialize the C heap allocator
    var heap      = (heap_C){};
    let allocator = heap_C_allocator(&heap);

    // Create an ArrayList of integers
    var list = catch (ArrayList_init(sizeof(int), allocator), result, {
        let    err = mem_AllocError_error(&result.err);
        ignore fprintf(stderr, "Error: %s\n", Error_message(err));
        return ErrorVoid_err(err);
    });

    // Add some numbers
    for (usize i = 0; i < 15; ++i) {
        catch (ArrayList_append(&list, &i), result, {
            let    err = mem_AllocError_error(&result.err);
            ignore fprintf(stderr, "Failed to append item: %s\n", Error_message(err));
            ArrayList_fini(&list);
            return ErrorVoid_err(mem_AllocError_error(&result.err));
        });
    }

    // Get a slice of the data
    let        slice = ArrayList_slice(&list);
    int* const items = slice.ptr;

    // Print all numbers
    for (usize i = 0; i < slice.len; ++i) {
        printf("%d ", items[i]);
    }
    printf("\n");

    // Clean up
    ArrayList_fini(&list);
    return ErrorVoid_ok();
}

// void example(void) {
//     // Initialize the C heap allocator
//     heap_C heap      = {};
//     let    allocator = heap_C_allocator(&heap);

//     // Create an ArrayList of integers
//     let list_result = ArrayList_init(sizeof(int), allocator);
//     if (list_result.is_err) {
//         printf("Failed to create ArrayList: out of memory\n");
//         return;
//     }
//     var list = list_result.ok;

//     // Add some numbers
//     for (usize i = 0; i < 15; ++i) {
//         let append_result = ArrayList_append(&list, &i);
//         if (append_result.is_err) {
//             printf("Failed to append item: out of memory\n");
//             ArrayList_fini(&list);
//             return;
//         }
//     }

//     // Get a slice of the data
//     let        slice = ArrayList_slice(&list);
//     int* const items = slice.ptr;

//     // Print all numbers
//     for (usize i = 0; i < slice.len; ++i) {
//         printf("%d ", items[i]);
//     }
//     printf("\n");

//     // Clean up
//     ArrayList_fini(&list);
// }
