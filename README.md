# dasae-headers

<!--
***Description***
"Make C Great Again": A modern language extension for safer and more productive C programming

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
  <p><strong>A modern language extension for safer and more productive C programming</strong></p>

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

## Table of Contents

- [dasae-headers](#dasae-headers)
  - [Table of Contents](#table-of-contents)
  - [🌟 Introduction](#-introduction)
  - [🛡️ Key Features](#️-key-features)
  - [🌐 Platform Support](#-platform-support)
  - [🚀 Getting Started](#-getting-started)
    - [Installation](#installation)
  - [⚡ Code Samples](#-code-samples)
    - [Basic Example](#basic-example)
    - [Error Handling Example](#error-handling-example)
    - [🧪 Test Code Example](#-test-code-example)
  - [📚 Documentation](#-documentation)
  - [🚧 Current Status](#-current-status)
  - [🙏 Contributing](#-contributing)
  - [📜 License](#-license)

## 🌟 Introduction

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

### Installation

1. Clone this repository:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
```

2. Include the necessary headers:

```c
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
// Include additional headers according to needed functionality
```

## ⚡ Code Samples

### Basic Example

```c
#include "dh/main.h"
#include "dh/core.h"
#include "dh/opt.h"

// Function that returns an optional value
fn_ext_scope(findValue(i32 id), Opt$i32) {
    if (id > 0) {
        return_some(id * 10);
    }
    return_none();
} ext_unscoped;

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // Using optional values
    let result = findValue(5);

    if_some(result, value) {
        printf("Found value: %d\n", value);
    } else_none {
        printf("Value not found.\n");
    }

    return_void();
} ext_unscoped;
```

### Error Handling Example

```c
// safe division - handling division by zero error
fn_ext_scope(safeDivide(i32 a, i32 b), math_Err$i32) {
    if (b == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(a / b);
} ext_unscoped;

// Processing Optional values
fn_(processNumber(Opt$i32 maybe_num), void) {
    if_some(maybe_num, num) {
        printf("Got number: %d\n", num);
    } else_none {
        printf("No number provided\n");
    }
}

// Ensuring memory safety with boundary-checked slices
fn_(sumSlice(Sli_const$i32 numbers), i32) {
    i32 sum = 0;
    for_slice(numbers, num) {
        sum += deref(num);
    }
    return sum;
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

// Define test module
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
- [Source code and examples](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)
- [API documentation](https://github.com/coding-pelican/dasae-headers/tree/main/dh/docs)

## 🚧 Current Status

This project is actively under development, and the API is not yet stabilized. We would appreciate if you could use it in experimental projects and provide feedback.
<!-- If you are interested in improving the project, please refer to contribution.md. -->

## 🙏 Contributing

Issues, pull requests, and feedback are always welcome!

1. Issue reporting: Bug reports / feature requests
2. Code contributions: Feature additions / bug fixes via Pull Requests
3. Documentation: Improving documentation / adding example code

## 📜 License

Copyright © 2024-2025. Gyeongtae Kim. All rights reserved.
