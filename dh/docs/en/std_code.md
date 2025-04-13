# DH-C Standard Coding Convention

## Preface

This is a guideline for safe and modern C programming. It prioritizes readability and safety above all else.

## Basic Principles

1. Code readability is the top priority. The code itself should serve as documentation.
2. When problems occur, write code that detects errors and fails as quickly as possible.
3. Prioritize patterns that prevent potential memory-related errors.
4. Treat all warnings as errors and fix them.

## I. Naming Conventions

### 1. Type Names

All type names use PascalCase.

```c
typedef struct Rectangle {
    i32 width;
    i32 height;
} Rectangle;

typedef struct Circle {
    f32 radius;
} Circle;
```

### 2. Variable Names

Local variables and function parameters use snake_case.

```c
fn_(calculateArea(const Rectangle* rectangle), i32) {
    let width = rectangle->width;
    let height = rectangle->height;
    return width * height;
}
```

#### 2.1. Constants

Constants use snake_case.

```c
#define max_buffer_size (1024)
const i32 max_retry_count = 5;
```

### 3. Function Names

Function names start with a verb and use camelCase.

```c
fn_(calculateSum(i32 a, i32 b), i32);
fn_(validateInput(Str_const input), bool);
fn_(initializeSystem(void), void);
```

Functions that return boolean values use prefixes such as `is`, `has`, `can`, `should`, etc.

```c
fn_(isValidInput(Str_const input), bool);
fn_(hasPermission(const User* user), bool);
```

### 4. Struct Fields

Struct fields use snake_case.

```c
typedef struct User {
    Sli$u8 user_name;
    u64    user_id;
    i32    access_level;
} User;
```

### 5. Enumerations

Enumeration names use PascalCase, and enumeration values use snake_case.

```c
// enum example
typedef enum Direction {
    Direction_north = 0,
    Direction_south = 1,
    Direction_east = 2,
    Direction_west = 3,
    Direction_count
} Direction;
```

#### 5.1. Error Sets

Error set names use PascalCase, and as an exception, error set values also use PascalCase.

```c
// ErrSet example
config_ErrSet(FileErr
    FileNotFound,
    AccessDenied,
    InvalidFormat,
);
```

#### 5.2. Union Enumerations

```c
// UnionEnum example
config_UnionEnum(Value
    (Value_integer_number, struct { i32 value; }),
    (Value_floating_point, struct { f64 value; }),
    (Value_string_literal, Str_const)
);
```

## II. Code Structure and Formatting

### 1. Indentation and Braces

Use 4 spaces for indentation.

```c
if (condition) {
    doSomething();
}
```

Use braces for conditional statements or loops even for single statements.

```c
// Incorrect example
if (condition)
    doSomething();

// Correct example
if (condition) {
    doSomething();
}
```

### 2. Function Definitions

Use `fn_` or `fn_scope` for function definitions.

```c
// Basic function definition
fn_(calculateSum(i32 a, i32 b), i32) {
    return a + b;
}

// Function definition requiring extended scope (Optional return)
use_Opt$(i32);
fn_scope(divideNumbers(i32 lhs, i32 rhs), Opt$i32) {
    if (rhs == 0) {
        return_none();
    }
    return_some(lhs / rhs);
} unscoped;

// Function requiring error handling
use_Err$(Str_const);
fn_(readFile(Str_const path), $must_check Err$Str_const);
fn_scope(readFile(Str_const path), Err$Str_const) {
    if (Str_isEmpty(path)) {
        return_err(Err_EmptyPathProvided());
    }
    // Processing...
    return_ok(content);
} unscoped;
```

### 3. Pointer Variable Declarations

Attach the pointer symbol (*) to the type.
Or use `Ptr$(T)`.

```c
// Incorrect example
i32 *data;

// Correct example
i32* data;
// Or
Ptr$(i32) data;
```

### 4. Variable Initialization

Variables must be initialized at the time of declaration. Use cleared() for zero initialization.

```c
// Incorrect example
i32 counter;
counter = 0;

// Correct example
let counter = 0;

// Struct initialization - zero initialization
User user = cleared();

// Using compound literals
let rect = make$(Rectangle, .width = 10, .height = 20);
```

## III. Safe Programming Practices

### 1. Memory Management

After resource allocation, use defer to specify cleanup operations.

```c
// Incorrect example
fn_scope(processData(void), Err$void) {
    let buffer = meta_cast$(Sli$i8, try_(mam_Allocator_alloc(typeInfo$(i8), buffer_size)));

    // Processing logic...

    mem_Allocator_free(anySli(buffer)); // Memory leak if there's a return in the middle
} unscoped;

// Correct example
fn_scope_ext(processData(void), Err$void) {
    let buffer = meta_cast$(Sli$i8, try_(mam_Allocator_alloc(typeInfo$(i8), buffer_size)));
    defer_(mem_Allocator_free(anySli(buffer)));

    // Processing logic...
    // Buffer is always freed regardless of where the return occurs
} unscoped_ext;
```

### 2. File Handling

After opening a file, use defer to specify closing operations.

```c
fn_scope_ext(readFileContents(Str_const filename), io_Err$Str_const) {
    let file = fopen(filename.ptr, "r");
    if (file == null) {
        return_err(io_Err_OpenFailed());
    }
    defer_($ignore fclose(file));

    // File processing...

    return_ok(content); // File is automatically closed when the function ends
} unscoped_ext;
```

### 3. Optional Value Handling

Use Optional return values instead of out parameters.

```c
// Incorrect example
fn_(findUser(i32 user_id, User** out_user), bool) {
    if (user_id <= 0 || out_user == null) {
        return false;
    }

    *out_user = lookupUserById(user_id);
    return (*out_user != null);
}

// Correct example
fn_scope(findUser(i32 user_id), Opt$Ptr$User) {
    if (user_id <= 0) {
        return_none();
    }

    const User* user = lookupUserById(user_id);
    if (user == null) {
        return_none();
    }

    return_some(user);
} unscoped;

// Usage example
fn_(processUserData(i32 user_id), bool) {
    if_some(findUser(user_id), user) {
        processData(user);
        return true;
    }
    log_error("User not found");
    return false;
}
```

### 4. Safe Array Access

Use safe array techniques instead of raw array and raw pointer index access.

```c
// Incorrect example
fn_(processArray(i32* data, usize size), void) {
    for (usize i = 0; i < size; ++i) {
        process(data[i]); // Potential boundary error
    }
}

// Correct example
fn_(processArray(Sli$i32 data), void) {
    // Using array traversal method
    for_slice (data, item) {
        process(deref(item));
    }

    // Or when direct access is needed, use safe access functions
    for (usize i = 0; i < data.len; ++i) {
        process(Arr_getAt(data, i)); // Triggers assertion on invalid access
    }
}
```
