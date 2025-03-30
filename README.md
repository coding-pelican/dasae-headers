# dasae-headers

<!--
***Description***
dasae-headers: A modern extension language for safer and more productive C programming

***Topic***
- c-programming
- memory-safety
- modern-c
- safe-c
- generic
- c17
- c-extension
- error-handling
- type-safety
- defer-pattern
- optional-type
- transpiler
- cross-platform
- rust-inspired
- zig-inspired
- testing-framework
 -->

<div align="center">
  <h1>dasae-headers: Modern and Safe C programming</h1>
  <p><strong>A modern extension language for safer and more productive C programming</strong></p>

  <div>
    <a href="./README.md"><img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English"></a>
    <a href="./README.ko.md"><img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean"></a>
  </div>

  <div style="margin-top: 8px;">
    <img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17">
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square" alt="Platform: Windows | Linux | macOS">
    <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT">
  </div>
</div>

## 📋 Table of Contents

- [dasae-headers](#dasae-headers)
  - [📋 Table of Contents](#-table-of-contents)
  - [🌟 Introduction](#-introduction)
  - [🛡️ Key Features](#️-key-features)
  - [🌐 Platform Support](#-platform-support)
  - [🚀 Getting Started](#-getting-started)
    - [💽 Installation](#-installation)
  - [⚡ Code Samples](#-code-samples)
    - [👋 Hello, world!](#-hello-world)
    - [🔍 Optional Values Example](#-optional-values-example)
    - [🔄 Error Results Example](#-error-results-example)
    - [🤝 Pattern Matching Example](#-pattern-matching-example)
    - [🧪 Test Code Example](#-test-code-example)
  - [📚 Documentation](#-documentation)
  - [🚧 Current Status](#-current-status)
  - [🙏 Contributing](#-contributing)
  - [📧 Contact](#-contact)
  - [📜 License](#-license)

## 🌟 Introduction

> "Make C Great Again"

dasae-headers aims to apply modern programming paradigms to the C language, greatly enhancing safety, expressiveness, and productivity. It provides memory safety, type safety, and improved error handling while maintaining the simplicity of C.

Currently in its early development stage, dasae-headers utilizes C's preprocessor as a transpiler and addresses chronic issues in the C standard library while gradually evolving into an independent language.

## 🛡️ Key Features

dasae-headers was developed with inspiration from the syntax and standard libraries of Zig and Rust:

**Enhanced Safety**
- Automated memory management through `defer` statements
- Null safety through `Optional` types
- Enhanced type safety through compile-time checks
- Built-in memory tracking for detecting memory leaks and related bugs

**Modern Programming Structures**
- Type inference through `let` and `var` keywords
- Boundary-checked slices for safe memory manipulation
- Result types for explicit error handling with `try_`, `catch_` patterns
- Algebraic data types (union enum) with pattern matching
- Array safety through checked access methods

**Development Productivity**
- Expressive macros for improved code clarity
- Modern function syntax using `fn_` declarations
- Cross-platform support for various architectures
- Major compiler compatibility (clang, gcc, msvc)
- Consistent code style and naming conventions

## 🌐 Platform Support

- **Operating Systems**: Windows, Unix, Linux, macOS, Android, etc.
- **CPU Architectures**: Considers 64/32-bit architectures
- **Compiler Compatibility**: clang (preferred), gcc, msvc, etc.

## 🚀 Getting Started

### 💽 Installation

> Detailed installation and build guide is in preparation. Please wait a moment!

1. Clone this repository:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
```

2. Include the necessary headers:

```c
#include "dh/main.h"
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
// Include additional headers according to needed functionality
```

## ⚡ Code Samples

### 👋 Hello, world!

```c
// Include the main header that provides program entry point
#include "dh/main.h"
// Include string utilities for working with text
#include "dh/Str.h"

// Define the main function with extended scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // Create a string literal using Str_l
    let hello_world = Str_l("Hello, world!");

    // Print the string to the console with a newline
    Str_println(hello_world);

    // Return success (void value with no error)
    return_ok({});
} ext_unscoped; // End the extended scope block
```

### 🔍 Optional Values Example

```c
fn_ext_scope(findValueIndex(i32 value, Sli_const$i32 items), Opt$i32) {
    for_slice_indexed (items, item, index) {
        if (*item != value) { continue; }
        return_some(index); // Return with a value
    }
    return_none(); // Return with no value
} ext_unscoped;

fn_(example(void), void) {
    Arr$$(5, i32) nums = Arr_init({ 10, 20, 30, 40, 50 });

    // Create optional values
    let opt_value = some$(Opt$i32, 42);
    let opt_empty = none$(Opt$i32);

    // Find a value in array
    let found = findValueIndex(30, Sli_arr$(Sli_const$i32, nums));

    // Check if option has value
    if_some(found, index) {
        printf("Found at: %d\n", index);
    } else_none {
        printf("Not found\n");
    }

    // Default values
    let value = orelse(found, -1); // Use -1 if not found

    // Unsafe extraction (assertion if option might be none)
    let unsafe_value = unwrap(opt_value);
}
```

### 🔄 Error Results Example

```c
config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_ErrSet$(math_Err, i32); // or Generally `use_Err$(i32)`
fn_ext_scope(safeDivide(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} ext_unscoped;

fn_ext_scope(example(void), Err$void) {
    // Allocate resources
    var buffer = meta_cast$(Sli$i32,
        try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 100))
    );
    // Cleanup always when function returns
    defer_(mem_Allocator_free(allocator, anySli(buffer)));
    // Cleanup only when an error occurs and propagates
    errdefer_(log_error("Occurred error!"));

    // Error propagation (early return)
    let result_invalid = try_(safeDivide(10, 0));

    // Error handling with default value
    let result_default = catch_(safeDivide(10, 0), 1);

    // Error handling with error payload capture
    let result_handling = catch_from(safeDivide(10, 0), err, eval({
        Err_print(err);   // Print the error
        ErrTrace_print(); // Print the error trace
        return_err(err);  // Return with an error
    }));

    // Return a normally
    return_ok({});
} ext_unscoped;
```

### 🤝 Pattern Matching Example

```c
config_UnionEnum(InputEvent,
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
);
use_Opt$(InputEvent);
fn_(pullInputEvent(void), Opt$InputEvent);

fn_(example(void), void) {
    if_some(pullInputEvent(), event) {
        match_(event) {
        pattern_(InputEvent_press_key, on_pressed) {
            debug_assert_true_fmt(
                -1 < on_pressed->key && on_pressed->key <= 255,
                "key is out of range"
            );
        } break;
        pattern_(InputEvent_release_button, on_released) {
            debug_assert_true_fmt(
                -1 < on_released->button && on_released->button <= 5,
                "button is out of range"
            );
        } break;
        fallback_()
            claim_unreachable;
        }
    }
}
```

### 🧪 Test Code Example

dasae-headers provides a simple yet powerful built-in testing framework. You can easily write and run unit tests through the `TEST.h` header.

```c
#include "dh/main.h"
#include "dh/TEST.h"

// Define functions to test
fn_(mathAdd(i32 a, i32 b), i32) {
    return a + b;
}

fn_(mathMultiply(i32 a, i32 b), i32) {
    return a * b;
}

// Define test unit
fn_TEST_scope("Basic Math Operations Test") {
    // Addition test
    let a = 5;
    let b = 7;
    let sum = mathAdd(a, b);

    // Validate results
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(sum > 10, "Sum should be greater than 10"));

    // Multiplication test
    let product = mathMultiply(a, b);
    try_(TEST_expect(product == 35));

    // Failing test (intentional error)
    // try_(TEST_expect(product == 30)); // Fails: 35 != 30
} TEST_unscoped;
```

## 📚 Documentation

Detailed documentation can be found at:
- [API documentation](https://github.com/coding-pelican/dasae-headers/tree/main/dh/docs)
- [Header files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/include)
- [Source files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/src)
- [Examples](https://github.com/coding-pelican/dasae-headers/tree/main/dh/samples)
- [Tests](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)

## 🚧 Current Status

This project is actively under development, and the API is not yet stabilized. We would appreciate if you could use it in experimental projects and provide feedback.
<!-- If you are interested in improving the project, please refer to contribution.md. -->

## 🙏 Contributing

Issues, pull requests, and feedback are always welcome!

1. Issue reporting: Bug reports / feature requests
2. Code contributions: Feature additions / bug fixes via Pull Requests
3. Documentation: Improving documentation / adding example code

## 📧 Contact

For any questions or inquiries, please contact the project author:
- Gyeongtae Kim(dev-dasae) <<codingpelican@gmail.com>>

## 📜 License

Copyright © 2024-2025. Gyeongtae Kim. All rights reserved.
