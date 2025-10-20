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
  <a href="https://github.com/coding-pelican/dasae-headers">
    <img src="./.github/logo-dasae_headers.svg" alt="dasae-headers Logo" width="150"/>
  </a>
  <h1>dasae-headers: Modern and Safe C programming</h1>
  <p><strong>A modern extension language for safer and more productive C programming</strong></p>

  <div>
    <a href="./README.md"><img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English"></a>
    <a href="./README.ko.md"><img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean"></a>
  </div>

  <div style="margin-top: 8px;">
    <a href="https://en.wikipedia.org/wiki/C17_(C_standard_revision)"><img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17"></a>
    <a href="./dh/include/dh/builtin/plat_cfg.h"><img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square" alt="Platform: Windows | Linux | macOS"></a>
    <a href="./LICENSE"><img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT"></a>
  </div>
</div>

## Table of Contents

- [dasae-headers](#dasae-headers)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Key Features](#key-features)
  - [Platform Support](#platform-support)
  - [🚀 Getting Started](#-getting-started)
    - [💽 Installation](#-installation)
    - [🔨 Building and Running](#-building-and-running)
  - [Code Samples](#code-samples)
    - [Hello, world!](#hello-world)
    - [Optional Values Example](#optional-values-example)
    - [Error Results Example](#error-results-example)
    - [Pattern Matching Example](#pattern-matching-example)
    - [Test Code Example](#test-code-example)
  - [Documentation](#documentation)
  - [Current Status](#current-status)
  - [Contributing](#contributing)
  - [Contact](#contact)
  - [License](#license)

## Introduction

> "Make C Great Again"

dasae-headers aims to apply modern programming paradigms to the C language, greatly enhancing safety, expressiveness, and productivity. It provides memory safety, type safety, and improved error handling while maintaining the simplicity of C.

Currently in its early development stage, dasae-headers utilizes C's preprocessor as a transpiler and addresses chronic issues in the C standard library while gradually evolving into an independent language.

## Key Features

dasae-headers was developed with inspiration from the syntax and standard libraries of Zig and Rust:

**Memory Safety & Resource Management**
- Custom allocators for selective memory usage and control
- Built-in memory tracking for detecting memory leaks and related bugs
- Array safety through checked access methods
- Boundary-checked slices for safe memory manipulation
- Automated resource management through `defer` and `errdefer` statements

**Type System Enhancements**
- Enhanced type safety through compile-time checks
- Algebraic data types (variant) with pattern matching and `match` statements
- Null safety through optional types with `some`/`none` keywords and `unwrap`/`orelse` patterns

**Error Handling & Debugging**
- Error-wrapped types for explicit error handling with `ok`/`err` keywords
- Structured error propagation with `try`, `catch` patterns
- Comprehensive error tracing with stack traces for debugging

**Modern Programming Paradigms**
- Type inference through `let` and `var` keywords
- Modern function syntax using `fn` declarations
- Lambda expressions and callback types
- Consistent code style and naming conventions
- Expressive macros for improved code clarity

**Development Tools**
- Built-in testing framework for unit and integration tests
- Multi-platform support for various architectures
- Major compiler compatibility (clang, gcc (planned), msvc-cl (planned))

## Platform Support

- **Operating Systems**: Windows, Unix, Linux, macOS, etc.
- **CPU Architectures**: Considers 64/32-bit architectures
- **Compiler Compatibility**: clang, gcc (planned), msvc-cl (planned), etc.

## 🚀 Getting Started

### 💽 Installation

> Detailed installation and build guide is in preparation. Please wait a moment!

1. Clone this repository:

```sh
git clone https://github.com/coding-pelican/dasae-headers.git
```

2. Install the dh-c build tool:

On Windows:

```ps1
cd dasae-headers
.\install-dh-c.ps1
```

On Linux/macOS:

```sh
cd dasae-headers
chmod +x install-dh-c.sh
./install-dh-c.sh
```

3. Create a new project:

```sh
dh-c project myproject
cd myproject
```

4. Start coding by including the necessary headers:

```c
#include "dh/main.h"
#include "dh/io/stream.h"
#include "dh/ArrList.h"
// Include additional headers according to needed functionality
```

### 🔨 Building and Running

Build your project:

```sh
dh-c build dev
```

Run your project:

```sh
dh-c run dev
```

Run tests:

```sh
dh-c test
```

See the [Quick Start Guide](./dh/docs/en/quick-start.md) for more details.

## Code Samples

### Hello, world!

```c
// Include the main header that provides program entry point
#include "dh/main.h"
#include "dh/io/stream.h"

// Define the main function with scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    // Create a string literal using u8_l
    let message = u8_l("Hello");
    // Print the string to the terminal with a newline
    io_stream_println(u8_l("{:s}, world!"), message);
    // Return success (void value with no error)
    return_ok({});
} $unscoped_(fn); // End the scope block
```

### Optional Values Example

```c
fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_($s(items, item)) {
        if (*item != value) { return_some(index); } // Return with a value
    }
    return_none(); // Return with no value
} $unscoped_(fn);

fn_((example(void))(void)) {
    // Create an array with 5 elements
    var nums = init$A$$((5, i32)({ 10, 20, 30, 40, 50 }));

    // Create optional values
    let opt_value = some$((i32)(42));
    let opt_empty = none$(i32);

    // Find a value in array
    let found = findValueIndex(30, ref$A$((i32)(nums)).as_const);
    // Check if option has value
    if_some(found, index) {
        io_stream_println(u8_("Found at: {:d}"), index);
    } else_none {
        io_stream_println(u8_("Not found"));
    }

    // Default values
    let value = orelse_((found)(-1)); // Use -1 if not found
    // Asserted extraction (assertion if option might be none)
    let value_assumed = unwrap_(opt_value);
}
```

### Error Results Example

```c
config_ErrSet((math_Err)(
    math_Err_DivisionByZero,
    math_Err_Overflow,
    math_Err_Underflow
));

use_ErrSet$(math_Err, i32); // or Generally `tpl$E$(i32)`
fn_((safeDivide(i32 lhs, i32 rhs))(math_Err$i32) $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} $unscoped_(fn);

fn_((example(void))(E$void) $guard) {
    // Allocate resources
    var buffer = meta_s$((i32)(try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 100))));
    // Always executed when function returns
    defer_(mem_Allocator_free(allocator, meta_any(buffer)));
    // Only executed when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_("Occurred error!: {:s}"), Err_codeToStr(err)));

    // Error propagation (early return)
    let result_invalid = try_(safeDivide(10, 0));
    // Error handling with default value
    let result_default = catch_((safeDivide(10, 0))($ignore, 1));
    // Error handling with error payload capture
    let result_handling = catch_((safeDivide(10, 0))(err, {
        Err_print(err);   // Print the error
        ErrTrace_print(); // Print the error trace
        return_err(err);  // Return with an error
    }));

    // Return a normally
    return_ok({});
} $unguarded_(fn);
```

### Pattern Matching Example

```c
typedef variant_((InputEvent)(
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
)) InputEvent;
tpl$O$(InputEvent);
fn_((pullInputEvent(void))(O$InputEvent));

fn_((example(void))(void)) {
    if_some(pullInputEvent(), event) {
        match_(event) {
            pattern_(InputEvent_press_key, on_pressed, {
                debug_assert_true_fmt(
                    -1 < on_pressed->key && on_pressed->key <= 255,
                    "key is out of range"
                );
            }) break;
            pattern_(InputEvent_release_button, on_released, {
                debug_assert_true_fmt(
                    -1 < on_released->button && on_released->button <= 5,
                    "button is out of range"
                );
            }) break;
            fallback_(claim_unreachable);
        }
    }
}
```

### Test Code Example

dasae-headers provides a simple yet powerful built-in testing framework. You can easily write and run unit tests through the `TEST.h` header.

```c
#include "dh/main.h"
#include "dh/TEST.h"

// Define functions to test
fn_((mathAdd(i32 a, i32 b))(i32)) {
    return a + b;
}

fn_((mathMul(i32 a, i32 b))(i32)) {
    return a * b;
}

// Define test unit
TEST_fn_("Basic Math Operations Test" $scope) {
    // Addition test
    let a = 5;
    let b = 7;
    let sum = mathAdd(a, b);

    // Validate results
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(sum > 10, "Sum should be greater than 10"));

    // Multiplication test
    let product = mathMul(a, b);
    try_(TEST_expect(product == 35));

    // Failing test (intentional error)
    // try_(TEST_expect(product == 30)); // Fails: 35 != 30
} $unscoped_(TEST_fn);
```

## Documentation

Detailed documentation can be found in our wiki:
- [Wiki Home](./dh/docs/en/home.md) - Main documentation portal
- [Quick Start Guide](./dh/docs/en/quick-start.md) - Detailed guide for beginners
- [API Reference](./dh/docs/en/api/index.md) - Module documentation

Additional resources:
- [Header files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/include)
- [Source files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/src)
- [Examples](https://github.com/coding-pelican/dasae-headers/tree/main/dh/samples)
- [Tests](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)

## Current Status

This project is actively under development, and the API is not yet stabilized. We would appreciate if you could use it in experimental projects and provide feedback.
If you are interested in improving the project, please refer to [contribution.md](./dh/docs/en/contributing.md).

The name "dasae-headers" originated from its beginnings as a header-only library that collected personal favorite C utility code. I haven't decided on a name for it yet. If you have a good name for it, please suggest it :D

## Contributing

Issues, pull requests, and feedback are always welcome!

1. Issue reporting: Bug reports / feature requests
2. Code contributions: Feature additions / bug fixes via Pull Requests
3. Documentation: Improving documentation / adding example code

## Contact

For any questions or inquiries, please contact the project author:
- Gyeongtae Kim (dev-dasae) <<codingpelican@gmail.com>>

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

Copyright © 2024-2025 Gyeongtae Kim
