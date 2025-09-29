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

## 📋 Table of Contents

- [dasae-headers](#dasae-headers)
  - [📋 Table of Contents](#-table-of-contents)
  - [🌟 Introduction](#-introduction)
  - [🛡️ Key Features](#️-key-features)
  - [🌐 Platform Support](#-platform-support)
  - [🚀 Getting Started](#-getting-started)
    - [💽 Installation](#-installation)
    - [🔨 Building and Running](#-building-and-running)
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

**Memory Safety & Resource Management**
- Custom allocators for selective memory usage and control
- Built-in memory tracking for detecting memory leaks and related bugs
- Array safety through checked access methods
- Boundary-checked slices for safe memory manipulation
- Automated resource management through `defer` and `errdefer` statements

**Type System Enhancements**
- Enhanced type safety through compile-time checks
- Algebraic data types (union enum) with pattern matching and `match` statements
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
- Major compiler compatibility (clang, gcc, msvc)

## 🌐 Platform Support

- **Operating Systems**: Windows, Unix, Linux, macOS, Android, etc.
- **CPU Architectures**: Considers 64/32-bit architectures
- **Compiler Compatibility**: clang (preferred), gcc, msvc, etc.

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
#include "dh/opt.h"
#include "dh/err_res.h"
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

## ⚡ Code Samples

### 👋 Hello, world!

```c
// Include the main header that provides program entry point
#include "dh/main.h"
#include "dh/io/stream.h"

// Define the main function with scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_((dh_main(Sli$Sli_const$u8 args))(Err$void $scope)) {
    $ignore = args;

    // Create a string literal using u8_l
    let hello = u8_l("Hello");
    // Print the string to the standard output with a newline
    io_stream_println(u8_l("{:s}, world!"), hello);

    // Return success (void value with no error)
    return_ok({});
} $unscoped_(fn); // End the scope block
```

### 🔍 Optional Values Example

```c
fn_((findValueIndex(i32 value, Sli_const$i32 items))(Opt$usize $scope)) {
    for_s((items), (item, index)) {
        if (*item != value) { continue; }
        return_some(index); // Return index
    }
    return_none(); // Return no index
} $unscoped_(fn);

fn_((example(void))(void)) {
    var_(nums, Arr$$(5, i32)) = Arr_init({ 10, 20, 30, 40, 50 });

    // Create optional values
    let opt_value = some$((Opt$usize)(42));
    let opt_empty = none$((Opt$usize));

    // Find a value in array
    let found = findValueIndex(30, Arr_ref$(Sli$usize, nums).as_const);
    // Check if option has value
    if_some((found), (index)) {
        io_stream_println(u8_l("Found at: {:zu}"), index);
    } else_none {
        io_stream_println(u8_l("Not found"));
    }

    // Default values
    let value = orelse_((found)(-1)); // Use -1 if not found
    // Unsafe extraction (assertion if option might be none)
    let unsafe_value = unwrap_(opt_value);
}
```

### 🔄 Error Results Example

```c
use_ErrSet_((math_Err)(
    math_Err_DivisionByZero,
    math_Err_Overflow,
    math_Err_Underflow
));

Errset_useT$(math_Err, i32); // or Generally `Err_useT$(i32)`
fn_((safeDivI32(i32 lhs, i32 rhs))(math_Err$i32 $scope)) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} $unscoped_(fn);

fn_((example(void))(Err$void $guard)) {
    // Allocate resources
    var buffer = meta_cast$((Sli$i32)(
        try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 128))
    ));
    // Cleanup always when function returns
    defer_(mem_Allocator_free(allocator, anySli(buffer)));
    // Cleanup only when an error occurs and propagates
    errdefer_($ignore_capture, io_stream_eprintln(u8_l("Occurred error!")));

    // Error propagation (early return)
    let result_invalid = try_(safeDivI32(10, 0));
    // Error handling with default value
    let result_default = catch_((safeDivI32(10, 0))($ignore_capture, 1));
    // Error handling with error payload capture
    let result_handling = catch_((safeDivI32(10, 0))(err, {
        printErr(err);
        printErrTrace();
        return_err(err); // Return with an error
    }));

    // Return a normally
    return_ok({});
} $unguarded_(fn);
```

### 🤝 Pattern Matching Example

```c
typedef variant_((InputEvent)(
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
)) InputEvent;
Opt_useT$(InputEvent);
fn_((pullInputEvent(void))(Opt$InputEvent));

fn_((example(void))(void)) {
    if_some(pullInputEvent(), event) {
        match_(event) {
        pattern_(InputEvent_press_key, on_pressed, {
            debug_assert_true_fmt(
                -1 < on_pressed->key && on_pressed->key <= 255,
                u8_l("key is out of range")
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

### 🧪 Test Code Example

dasae-headers provides a simple yet powerful built-in testing framework. You can easily write and run unit tests through the `TEST.h` header.

```c
#include "dh/main.h"
#include "dh/TEST.h"

// Define functions to test
fn_((math_addI32(i32 a, i32 b))(i32)) {
    return a + b;
}

fn_((math_addI32(i32 a, i32 b))(i32)) {
    return a * b;
}

// Define test unit
TEST_fn_("Basic Math Operations Test" $scope) {
    // Addition test
    let_(a, i32)   = 5;
    let_(b, i32)   = 7;
    let_(sum, i32) = math_addI32(a, b);

    // Validate results
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(10 < sum , "Sum should be greater than 10"));

    // Multiplication test
    let_(product, i32) = math_mulI32(a, b);
    try_(TEST_expect(product == 35));

    // Failing test (intentional error)
    catch_((TEST_expect(product == 30))($ignore_capture, {
        // Fails: 35 != 30
        io_stream_eprintln(u8_l("Product should be 30, but got {:d}"), product);
    }));
} $unscoped_(TEST_fn);
```

## 📚 Documentation

Detailed documentation can be found in our wiki:
- [Wiki Home](./dh/docs/en/home.md) - Main documentation portal
- [Quick Start Guide](./dh/docs/en/quick-start.md) - Detailed guide for beginners
- [API Reference](./dh/docs/en/api/index.md) - Module documentation

Additional resources:
- [Header files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/include)
- [Source files](https://github.com/coding-pelican/dasae-headers/tree/main/dh/src)
- [Examples](https://github.com/coding-pelican/dasae-headers/tree/main/dh/samples)
- [Tests](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)

## 🚧 Current Status

This project is actively under development, and the API is not yet stabilized. We would appreciate if you could use it in experimental projects and provide feedback.
If you are interested in improving the project, please refer to [contribution.md](./dh/docs/en/contributing.md).

The name "dasae-headers" originated from its beginnings as a header-only library that collected personal favorite C utility code. I haven't decided on a name for it yet. If you have a good name for it, please suggest it :D

## 🙏 Contributing

Issues, pull requests, and feedback are always welcome!

1. Issue reporting: Bug reports / feature requests
2. Code contributions: Feature additions / bug fixes via Pull Requests
3. Documentation: Improving documentation / adding example code

## 📧 Contact

For any questions or inquiries, please contact the project author:
- Gyeongtae Kim(dev-dasae) <<codingpelican@gmail.com>>

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

Copyright © 2024-2025 Gyeongtae Kim
