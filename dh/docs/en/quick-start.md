---
title: Quick Start Guide
description: Getting started with dasae-headers for safer C programming
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# Quick Start Guide

This guide will help you get started with dasae-headers, a modern extension language for safer and more productive C programming.

## Table of Contents

- [Installation](#installation)
- [Project Setup](#project-setup)
- [First Program](#first-program)
- [Key Concepts](#key-concepts)
- [Building and Running](#building-and-running)
- [Debugging](#debugging)
- [Next Steps](#next-steps)

## Installation

### Prerequisites

- C17 compatible compiler (clang 11+, gcc 8+, or MSVC 19.20+)
- Git (for obtaining the source)
- CMake 3.15+ (for build setup)

### Getting the Source

Clone the repository to your local machine:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
cd dasae-headers
```

### Installation Options

#### Method 1: Direct inclusion (recommended for existing projects)

1. Copy the `dh` directory into your project source
2. Include the necessary headers in your C files:

```c
#include "dh/main.h"   // For program entry point
#include "dh/core.h"   // Core functionality
// Add other headers as needed
```

#### Method 2: CMake integration

<!-- 1. Add the repository as a submodule:

```bash
git submodule add https://github.com/coding-pelican/dasae-headers.git external/dasae-headers
```

2. Update your CMakeLists.txt:

```cmake
add_subdirectory(external/dasae-headers)
target_link_libraries(your_target PRIVATE dasae-headers)
``` -->

## Project Setup

### Directory Structure

A typical dasae-headers project follows this structure:

```
my-project/
├── src/
│   ├── main.c       # Main entry point
│   └── ...          # Other source files
├── include/         # Project-specific headers
├── dh/              # dasae-headers directory
├── tests/           # Test files
└── README.md        # Project documentation
```

### Main Entry Point

Unlike standard C, dasae-headers provides a structured entry point with built-in error handling:

```c
#include "dh/main.h"

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // Your code here
    return_ok({});  // Return success
} ext_unscoped;
```

## First Program

Let's create a simple "Hello, world!" program using dasae-headers:

```c
#include "dh/main.h"
#include "dh/Str.h"

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // Create a string literal
    let hello = Str_l("Hello, dasae-headers!");

    // Print to console with newline
    Str_println(hello);

    // Return success
    return_ok({});
} ext_unscoped;
```

### Key Features in This Example

1. `fn_ext_scope` - Function with extended scope for resource cleanup
2. `Sli$Str_const` - Slice of constant strings (command-line arguments)
3. `Err$void` - Error result type with void payload
4. `let` - Type inference for variable declaration
5. `Str_l` - String literal creation
6. `return_ok` - Successful return with payload

## Key Concepts

### Variable Declaration

dasae-headers provides two ways to declare variables:

```c
// Type inference (immutable by default)
let count = 10;  // i32 inferred

// Explicit type (mutable)
var number = 42;  // Explicitly mutable
```

### Memory Safety with defer

Automatic resource cleanup using `defer`:

```c
fn_ext_scope(readFile(Str_const path), Err$Str) {
    let_(file, FILE*) = fopen(path.ptr, "r");
    if (file == null) {
        return_err(fileError("Could not open file"));
    }

    // This will execute when the function exits
    defer_($ignore fclose(file));

    // Process file...

    return_ok(fileContents);
} ext_unscoped;
```

### Error Handling

Explicit error handling with the `try_` pattern:

```c
fn_ext_scope(processData(void), Err$void) {
    // Call function that may fail and propagate error
    let result = try_(getData());

    // Process result...

    return_ok({});
} ext_unscoped;
```

### Optional Types

Safe handling of nullable values:

```c
fn_ext_scope(findUser(i32 id), Opt$User) {
    if (id <= 0) {
        return_none();  // No user found
    }

    User user = getUserById(id);
    return_some(user);  // User found
} ext_unscoped;

// Usage
if_some(findUser(42), user) {
    // User exists, process user
} else_none {
    // Handle not found case
}
```

## Building and Running

<!-- ### Using CMake

1. Create a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.15)
project(my_dasae_project C)

set(CMAKE_C_STANDARD 17)

# Source files
add_executable(my_program
    src/main.c
    # Add other source files
)

# Include paths
target_include_directories(my_program PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dh
)
```

2. Build the project:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

3. Run your program:

```bash
./my_program
```

### Using Make or Other Build Systems

For non-CMake projects, ensure you:

1. Set the C standard to C17 or later
2. Include the dasae-headers directory in your include path
3. Compile all required source files -->

## Debugging

dasae-headers includes built-in debug features:

### Assertions

```c
debug_assert_true(condition, "Error message");
debug_assert_fmt(0 < count, "Invalid count: %d", count);
```

## Next Steps

Now that you've set up a basic project, you can explore:

- **[Core Concepts](./concepts/index.md)** - Fundamental principles like memory safety, error handling, and type system
- **[API Reference](./api/index.md)** - Comprehensive documentation for all library modules
- **[Examples](./examples/index.md)** - Practical code samples demonstrating library features
- **[Tutorials](./tutorials/index.md)** - Step-by-step guides for learning specific techniques

Happy coding with dasae-headers!
