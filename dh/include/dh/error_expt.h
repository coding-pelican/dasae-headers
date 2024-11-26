/**
 * @file error.h
 */
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include "core.h"
#include "scope.h"

/*========== Error Sets =================================================*/

// Define error sets as enums with unique bits
// This allows merging via bitwise OR
typedef enum {
    // File errors
    Error_AccessDenied = 1 << 0,
    Error_OutOfMemory  = 1 << 1,
    Error_FileNotFound = 1 << 2,

    // Other errors
    Error_IoError    = 1 << 3,
    Error_InvalidArg = 1 << 4,
    Error_Overflow   = 1 << 5,

    // Merged error sets (examples)
    ErrorSet_FileOpen   = Error_AccessDenied | Error_OutOfMemory | Error_FileNotFound,
    ErrorSet_Allocation = Error_OutOfMemory,
} Error;

// Error union type
#define ErrorUnion(T)    \
    struct {             \
        bool has_error;  \
        union {          \
            T     value; \
            Error error; \
        };               \
    }

// Special case for void
typedef struct ErrorUnionVoid {
    bool  has_error;
    Error error;
} ErrorUnionVoid;
typedef ErrorUnionVoid ErrorVoid;



// void test(){
//     i32 value = /* value; // macro begin
//     scope_if(Error _result = getValue((i32){ 0 }); _result.has_error) {
//         // handle error
//     }
//     else {
//         // handle value
//         value = *Ptr_cast(_result.value, i32);
//     } */
//                 // macro end
// }


/*========== Error Creation ============================================*/

// Create success result
#define Ok(ErrT, x) ((ErrT){ \
    .has_error = false,      \
    .value     = (x) })

// Create void success
#define OkVoid() ((ErrorVoid){ \
    .has_error = false,        \
    .error     = 0 })

// Create error result
#define Err(ErrT, e) ((ErrT){ \
    .has_error = true,        \
    .error     = (e) })

// Create void error
#define ErrVoid(e) ((ErrorVoid){ \
    .has_error = true,           \
    .error     = (e) })

/*========== Error Handling ===========================================*/

#define pp_use(...) __VA_ARGS__

// try operator for values
#define try(ErrT, expr, var)                              \
    var;                                                  \
    scope_if(ErrT _result = &(expr), _result.has_error) { \
        return Err(ErrT, _result.error);                  \
    }                                                     \
    else {                                                \
        pp_use(var) = _result.value;                      \
    }

// try operator for void
#define try_void(expr)                     \
    do {                                   \
        ErrorUnionVoid _result = (expr);   \
        if (_result.has_error) {           \
            return ErrVoid(_result.error); \
        }                                  \
    } while (0)

// catch operator for values
#define catch(ErrT, expr, handler) ({                             \
    ErrT _result = (expr);                                        \
    _result.has_error ? (handler)(_result.error) : _result.value; \
})

// catch operator for void
#define catch_void(expr, handler)        \
    do {                                 \
        ErrorUnionVoid _result = (expr); \
        if (_result.has_error) {         \
            (handler)(_result.error);    \
        }                                \
    } while (0)

// errdefer cleanup
#define errdefer(cleanup)                         \
    for (bool _once = true; _once; _once = false) \
        for (bool _error = false;; _error = true) \
            if (_error) {                         \
                cleanup;                          \
                break;                            \
            } else

/*========== Error Set Operations =====================================*/

// Check if error is in set
#define error_in_set(err, set) \
    (((err) & (set)) == (err))

// Merge error sets
#define error_set_merge(a, b) ((a) | (b))

/*========== Usage Example ============================================*/

// // Example usage:
// typedef ErrorUnion(i32) ErrorI32;

// // Function that might fail
// ErrorI32 open_file(const char* path) {
//     if (access_denied) {
//         return Err(ErrorI32, Error_AccessDenied);
//     }
//     if (out_of_memory) {
//         return Err(ErrorI32, Error_OutOfMemory);
//     }
//     return Ok(ErrorI32, file_descriptor);
// }

// // Using try
// ErrorVoid process_file(const char* path) {
//     i32 fd = try(ErrorI32, open_file(path), fd);

//     errdefer(close(fd)) {
//         // Do something that might fail
//         if (/* error_condition */ false) {
//             return ErrVoid(Error_IoError);
//         }
//     }

//     return OkVoid();
// }

// // Using catch
// void example_catch(void) {
//     i32 fd = catch (i32, open_file("test.txt"), ^i32(Error err) {
//         switch (err) {
//         case Error_AccessDenied:
//             printf("Access denied\n");
//             return -1;
//         case Error_OutOfMemory:
//             printf("Out of memory\n");
//             return -1;
//         default:
//             printf("Unknown error\n");
//             return -1;
//         }
//     });
// }

// // Error set coercion example
// void example_coercion(void) {
//     Error alloc_err = Error_OutOfMemory;
//     // This works because FileOpen error set includes OutOfMemory
//     Error file_err  = alloc_err;

//     // Verify coercion
//     assert(error_in_set(file_err, ErrorSet_FileOpen));
// }

#endif
