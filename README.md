# dasae-headers

<!--
***Description***
dasae-headers: Modern, Better safety and productivity to C

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
  <h1>dasae-headers: C to Modern and Safe</h1>
  <p>Modern Programming Paradigm C Language Extension for Productivity</p>

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

<details>
<summary><strong>Table of Contents</strong></summary>

- [dasae-headers](#dasae-headers)
  - [Introduction](#introduction)
    - [Why dasae-headers?](#why-dasae-headers)
    - [How does it differ from traditional C?](#how-does-it-differ-from-traditional-c)
      - [1. Code Pattern \& Platform Abstraction](#1-code-pattern--platform-abstraction)
      - [2. Memory \& Argument Patterns](#2-memory--argument-patterns)
      - [3. Concurrency](#3-concurrency)
      - [4. Meta Type \& Generic System](#4-meta-type--generic-system)
      - [5. Numerical \& Safety](#5-numerical--safety)
      - [6. Ecosystem \& Infrastructure](#6-ecosystem--infrastructure)
    - [What makes dasae-headers special?](#what-makes-dasae-headers-special)
      - [1. Maximizing Static Analysis and IDE Compatibility](#1-maximizing-static-analysis-and-ide-compatibility)
      - [2. Meta System and Anonymous Type Compatibility](#2-meta-system-and-anonymous-type-compatibility)
      - [3. Zero-overhead Safety](#3-zero-overhead-safety)
  - [Key Features](#key-features)
  - [Platform Support](#platform-support)
  - [🚀 Getting Started](#-getting-started)
    - [💽 Installation](#-installation)
    - [🔨 Build and Run](#-build-and-run)
  - [Code Samples](#code-samples)
    - [Hello, world!](#hello-world)
    - [Optional Values](#optional-values)
    - [Error Results \& Defer](#error-results--defer)
    - [Pattern Matching](#pattern-matching)
    - [Chaining - Filter, Map, Fold, Reduce](#chaining---filter-map-fold-reduce)
    - [Threads vs Stackless-Coroutines](#threads-vs-stackless-coroutines)
    - [Atomic Values](#atomic-values)
    - [SIMD Vectors](#simd-vectors)
    - [Meta System](#meta-system)
    - [Data Structures \& Algorithms](#data-structures--algorithms)
    - [Testing](#testing)
  - [Documentation](#documentation)
  - [Naming](#naming)
  - [Contribution and Contact](#contribution-and-contact)
  - [License](#license)

</details>

---

## Introduction

**dasae-headers** aims to improve the safety, expressiveness, and productivity of the C language by introducing modern programming paradigms.
While maintaining the core principle of C—simplicity—it strengthens memory and type safety and provides a structured error-handling mechanism.

Currently in the early stages of development, it provides a transpiler-like environment leveraging the C preprocessor.
This project aims to compensate for the structural limitations of the standard C library and is in the process of gradually establishing an independent development ecosystem.

### Why dasae-headers?

dasae-headers adheres to the following design principles to overcome the constraints of traditional C environments and provide a modern development experience:

<details>
<summary><strong>Design Principles</strong></summary>

- **Seamless Coexistence with C Ecosystem:** Immediately introduce modern syntax and safety features without modifying existing C libraries or legacy codebases.
- **Zero-cost Abstractions:** Provide high-level features while minimizing runtime overhead through optimizations such as inlining, preprocessing-stage evaluation, and constant-folding.
- **Incremental Adoption:** Use only the necessary modules (e.g., error handling, allocators) selectively without needing to convert the entire project.
- **Freestanding and Bare-metal Support:** Support for freestanding environments—such as embedded systems or kernel development—is a top priority on the roadmap, making it effective at the lowest levels of the system.
- **Minimal User-defined Macros:** Users do not need to write additional complex macros to implement core features in general use cases. Generic instantiation is handled automatically during preprocessing as long as established patterns are followed.
- **Debug-friendly Design:** Macros are meticulously designed not to interfere with runtime debugging (call stack tracing, step-by-step execution), maintaining development productivity.
- **Consistent Conventions:** Strict and consistent code conventions and naming schemes ensure readability and maintainability even in large-scale codebases.

</details>

---

### How does it differ from traditional C?

<details>
<summary><strong>Show details</strong></summary>

While maintaining the flexibility of C, dasae-headers brings modern language safety features and productivity tools in a form optimized for the system layer.
Rather than simply adding features, it focuses on structurally addressing C's chronic design flaws and fragmented conventions.

#### 1. Code Pattern & Platform Abstraction

Unifies fragmented language/architecture/OS/compiler-specific APIs and complex syntax of standard C into a single interface.

| Aspect                      | Traditional C (Standard C)                                  | dasae-headers                                                                       |
| --------------------------- | ----------------------------------------------------------- | ----------------------------------------------------------------------------------- |
| **Variables and Functions** | Explicit type declarations and repetitive signatures        | `let` (constant), `var` (mutable) type inference and `fn_` modern syntax            |
| **Lambda/Closures**         | GCC nested functions or Clang blocks, platform-dependent    | `la_` syntax unifying both compiler extensions, `Callable` type for `fn_` and `la_` |
| **Platform Support**        | Fragmented branching with `#ifdef`                          | **Unified API** (`{lang/arch/plat/comp}_cfg.h`, `os.h`, `posix.h`) provided         |
| **Preprocessor Branching**  | Separate `#ifdef` branch definitions even for simple values | Single definition with preprocessor branching via `pp_if_`, `pp_switch_`            |

#### 2. Memory & Argument Patterns

Avoids unsafe patterns of returning success as `bool` and receiving result values via `out` parameters.
`Optional` and `Error Result` enforce validation of absent values or error conditions at the type system level.

| Aspect               | Traditional C (Standard C)                   | dasae-headers                                                      |
| -------------------- | -------------------------------------------- | ------------------------------------------------------------------ |
| **Resource Release** | `goto cleanup` or scattered manual cleanup   | Automatic scope-based cleanup with `defer_`, `errdefer_`           |
| **Result Return**    | `bool`/error code return + `out` pointer     | **Optional** (`O$`) / **Error Result** (`E$`) return               |
| **Result Branching** | Manual validation with `if (err)` branches   | Result control with `orelse_`, `unwrap_` / `try_`, `catch_` syntax |
| **Data Transfer**    | Pointer and length (`len`) passed separately | **Slice** (`S$`) or **array as value** (`A$`) transfer             |
| **Strings/Arrays**   | `0(NULL)` sentinel-based implicit length     | Explicit length field-based **Boundary Check**                     |

#### 3. Concurrency

Provides ultra-lightweight asynchronous environments capable of handling tens of thousands of tasks simultaneously without OS thread overhead.
Expresses state-machine-based control flow concisely with `async_/await_` patterns.

| Aspect              | Traditional C (Standard C)                           | dasae-headers                                                           |
| ------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------- |
| **Async Model**     | OS native thread-centric design                      | OS native threads + **State-machine-based Stackless Coroutines**        |
| **Sync Primitives** | Reliant on primitive `mutex`, `cond`                 | `RWLock`, `ResetEvent`, `WaitGroup`, etc.                               |
| **Control Flow**    | Callback hell or manually implemented state machines | Coroutine control with `async_`, `await_`, `suspend_`, `resume_` syntax |

#### 4. Meta Type & Generic System

Escapes macro hell and validates type safety at compile time through the meta type system.
Provides a differentiated layer that statically guarantees logical compatibility between anonymous user types not allowed in the C standard.

| Aspect              | Traditional C (Standard C)                                           | dasae-headers                                                                                     |
| ------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| **Implementation**  | `void*` casting (type information loss)                              | High-level abstraction based on **Meta System** (`u_*`)                                           |
| **Anonymous Types** | Assignment impossible even with identical structure due to anonymity | Compatibility guaranteed when field memory layout and logical structure (Size/Align/Offset) match |
| **API Exposure**    | Data structure API implementation directly embedded in macros        | **Macro-less API**: Adheres to standard function definition format                                |

#### 5. Numerical & Safety

Abstracts hardware architecture-specific SIMD instructions and statically detects arithmetic overflow and inappropriate type casting.
All safety features are optimized in release mode to ensure zero runtime overhead.

| Aspect                | Traditional C (Standard C)                                                | dasae-headers                                                                             |
| --------------------- | ------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| **Vector Ops**        | Manual loop-based operations or platform-dependent APIs                   | Architecture-independent accelerated operations via `simd.h`                              |
| **Arithmetic Safety** | Vulnerable to runtime exceptions like `Overflow`, `DivisionByZero`, `NaN` | Compile-time **Overflow/NaN static detection** layer                                      |
| **Type Casting**      | Risk of data loss due to implicit type conversion                         | Mismatch tracking for `Signed`/`Unsigned` and size conversion, `int`<->`float` conversion |

#### 6. Ecosystem & Infrastructure

Manages the entire process from project creation to build and test with built-in tools without additional dependencies.
When errors occur, preserves call stack information beyond simple return values to immediately pinpoint the cause.

| Aspect                         | Traditional C (Standard C)                             | dasae-headers                                                                    |
| ------------------------------ | ------------------------------------------------------ | -------------------------------------------------------------------------------- |
| **Data Structures/Algorithms** | Manual implementation or fragmented external libraries | Standard containers and algorithms like `ArrList`, `ArrPDeq`, `HashMap` provided |
| **Memory Control**             | Dependent on fixed global allocation                   | **Allocator or memory buffer injection** possible for all APIs                   |
| **Testing/Analysis**           | External framework integration required                | Sophisticated tracking based on built-in `TEST.h` and `ErrTrace.h`               |

</details>

---

### What makes dasae-headers special?

<details>
<summary><strong>Show details</strong></summary>

Beyond simple syntax extensions, dasae-headers leverages deep understanding of compiler and static analyzer behavior
to provide a user-centric development experience differentiated from other projects.

#### 1. Maximizing Static Analysis and IDE Compatibility

**Static Analyzer Transparency:**

Designed so that major analysis tools like `clang-tidy` can understand the source code the same way as regular C code.
In particular, it blocks static analyzer malfunctions (such as type mismatches from checking all branches) that occur when using `_Generic`,
allowing developers to focus only on "real" warnings and errors, not "recognition errors."

**Transparent API Exposure (Macro-less API):**

Core APIs, including data structures, are not wrapped in complex macro functions.
Macros focus on acting as operators that replace special symbols, while actual logic follows standard function definition formats.
This eliminates the burden of users having to write separate macros or understand complex macro structures to call libraries,
defining a predictable standard for distinguishing between "macros as operators" and "actual function usage" in core logic.

#### 2. Meta System and Anonymous Type Compatibility

**Meta Type-based Generic System:**

Implements generic logic through the meta type system (`u_*`) rather than macros, which are blind spots for static analysis.
Provides a zero-cost abstraction layer that optimizes identically to inlined code through LTO, constant folding, and constant propagation,
even without directly inlining code via macros.

**Structural Anonymous Type Compatibility Validation:**

Unlike other libraries that require type aliasing via `typedef` to allow generics, effectively accommodates anonymous user types.
Statically validates safety at compile time when field memory layout and logical structure (Size, Align, Offset) match,
ensuring safe interaction between anonymous types.

#### 3. Zero-overhead Safety

**Compile-time Evaluation Priority for Operations and Casting Validation:**

Statically detects arithmetic `Overflow`, `DivisionByZero`, `NaN`, and inappropriate type casting.
All safety validations prioritize compile-time evaluation and function as `assertions` at runtime.
In release mode, these validation items become branch optimization targets, securing safety without binary overhead,
while allowing faster and more accurate identification of problem causes during development compared to traditional C.

</details>

---

## Key Features

This project was developed with inspiration from the syntax structures and standard library designs of Zig and Rust.

- **Memory Safety:** Custom allocators, memory tracking, boundary checking, `defer_`/`errdefer_`-based automatic resource management
- **Enhanced Type System:** Compile-time validation, meta type system, algebraic data types (Variant), optional types
- **Explicit Error Handling:** `ok`/`err` keywords, `try_`/`catch_` patterns, error tracing with call stack information
- **Modern Syntax:** Type inference (`let`/`var`), function definition (`fn_`), lambda expressions (`la_`), first-class `Callable` type
- **Development Tools:** Built-in testing framework, support for major C compilers (Clang, GCC) and multi-platform environments

---

## Platform Support

| Category         | Support Range                                                                 |
| ---------------- | ----------------------------------------------------------------------------- |
| **OS**           | Windows, Unix, Linux, macOS                                                   |
| **Architecture** | x86 (32-bit), x64 (64-bit)                                                    |
| **Clang**        | 19.1.0+ (Recommended) / 16.0.0+ (Supported) / 9.0.0+ (Requires -std=gnu11)    |
| **GCC**          | 15.1.0+ (Recommended) / 13.1.0+ (Supported) / N/A (TBU) (Requires -std=gnu11) |
| **MSVC**         | Planned (TBD)                                                                 |

---

## 🚀 Getting Started

<!-- Interactive Quick Start -->
<!--
> **"Experience dasae-headers in 10 seconds"**
> ```sh
> curl -sSL https://raw.githubusercontent.com/coding-pelican/dasae-headers/main/try.sh | bash
> ```
-->

### 💽 Installation

1. Clone the repository:
```sh
git clone https://github.com/coding-pelican/dasae-headers
```

2. Install the dedicated build tool (`dh-c`):
```sh
cd dasae-headers
chmod +x install.sh
./install.sh
```

3. Create a new project:
```sh
dh-c project myproject
cd myproject
```

### 🔨 Build and Run

- **Build:** `dh-c build dev`
- **Run:** `dh-c run dev`
- **Test:** `dh-c test`

For further details, please refer to the [Quick Start Guide](./dh/docs/en/quick-start.md).

---

## Code Samples

### Hello, world!

```c
#include "dh/main.h"
#include "dh/io/stream.h"

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let message = u8_l("Hello");
    io_stream_println(u8_l("{:s}, world!"), message);
    return_ok({});
} $unscoped_(fn);
```

### Optional Values

```c
fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_(($s(items), $rf(0))(item, index) {
        if (*item != value) return_some(index);
    });
    return_none();
} $unscoped_(fn);

fn_((example(void))(void)) {
    var nums = A_from$((i32){ 10, 20, 30, 40, 50 });
    let found = findValueIndex(30, A_ref$((S$i32)(nums)).as_const);

    io_stream_println(u8_("found = {:?d}"), found);
    if_some((found)(index)) {
        io_stream_println(u8_("- Found at: {:d}"), index);
    } else_none {
        io_stream_println(u8_("- Not found"));
    }

    let value = orelse_((found)(-1));
    let value_assumed = unwrap_(found);
}
```

### Error Results & Defer

```c
errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));

T_use_E$($set(math_Err)(i32));
$attr($must_check)
fn_((safeDivide(i32 num, i32 denom))(math_Err$i32) $scope) {
    if (denom == 0) return_err(math_Err_DivisionByZero());
    return_ok(num / denom);
} $unscoped_(fn);

$attr($must_check)
fn_((example(mem_Allocator gpa))(E$void) $guard) {
    // Allocate resources
    var buffer = u_castS$((S$i32)(try_(mem_Allocator_alloc(gpa, typeInfo$(i32), 100))));
    defer_(mem_Allocator_free(gpa, u_anyS(buffer)));

    // Only executed when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_l("Occurred error!: {:e}"), err));

    // Error propagation (try_) and handling (catch_)
    let divided = try_(safeDivide(10, 0));
    let divided_handled = catch_((safeDivide(10, 0))($ignore, 1)); // Use default value 1 when error occurs

    return_ok({});
} $unguarded_(fn);
```

<details>
<summary><strong>More Code Samples</strong></summary>

### Pattern Matching

```c
typedef variant_((InputEvent $bits(8))(
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
)) InputEvent;
T_use_O$(InputEvent);
fn_((pullInputEvent(void))(O$InputEvent));

fn_((example(void))(void)) {
    if_some((pullInputEvent())(event)) match_(event) {
    pattern_((InputEvent_press_key)(on_pressed)) {
        debug_assert_true_fmt(
            -1 < on_pressed->key && on_pressed->key <= 255,
            "key is out of range"
        );
        break;
    } $end(pattern);
    pattern_((InputEvent_release_button)(on_released)) {
        debug_assert_true_fmt(
            -1 < on_released->button && on_released->button <= 5,
            "button is out of range"
        );
        break;
    } $end(pattern);
    fallback_(claim_unreachable);
} $end(match);
} $unscoped_(fn);
```

### Chaining - Filter, Map, Fold, Reduce

```c
T_use$((i32)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_appendWithin
));

fn_((collectEvenSq(S_const$i32 items, mem_Allocator gpa))(mem_Err$ArrList$i32) $scope) {
    let init = ArrList_init$i32;
    let appendWithin = ArrList_appendWithin$i32;
    return_ok(chain$((ArrList$i32)(items)(
        filter_((x)(int_isEven(*x))),
        map$((i32)(x)(int_sq(*x))),
        fold_(try_(init(gpa, items.len)), (collect, x)(appendWithin(&collect, *x), collect))
    )));
} $unscoped_(fn);

fn_((reduceSumEvenSq(S_const$i32 items))(O$i32)) {
    return chain$((O$i32)(items)(
        filter_((x)(int_isEven(*x))),
        map$((i32)(x)(int_sq(*x))),
        reduce_((acc, x)(acc + *x))
    ));
};

$attr($must_check)
fn_((example(void))(E$void) $guard) {
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);
    let nums = A_ref$((S$i32)(A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8 }))).as_const;

    let even_sqs = try_(collectEvenSq(nums, gpa));
    defer_(ArrList_fini$i32(&even_sqs, gpa));
    let sum = chain$((i32)(even_sqs.items)(fold_((0), (acc, item)(acc + *item))));
    let sum_even_sqs = orelse_((reduceSumEvenSq(nums))(0));
    claim_assert(sum == sum_even_sqs);

    return_ok({});
} $unguarded_(fn);
```

### Threads vs Stackless-Coroutines

In addition to traditional OS threads, state-machine-based coroutines are provided for ultra-lightweight asynchronous processing.

```c
Thrd_fn_(((timesTwoThread)(i32 input))(i32) $scope($ignore, args)) {
    time_sleep(time_Duration_fromMillis(10));
    return_(args->input * 2);
} $unscoped_(Thrd_fn);

fn_((mainThread(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    var task = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from$((timesTwoThread)(10)).as_raw));
    let result = Thrd_FnCtx_ret$((timesTwoThread)(Thrd_join(task)));
    io_stream_println(u8_l("result: {:d}"), result);
    return_ok({});
} $unscoped_(fn);

async_fn_(((timesTwoAsync)(O$$(Co_Ctx*) caller, i32 input))(i32) $scope({
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
})(self_ctx, args, locals)) {
    callAsync((locals->sleep_ctx)((exec_sleep)(
        some(orelse_((caller)(self_ctx->anyraw))), time_Duration_fromMillis(10)
    )));
    areturn_(args->input * 2);
} $unscoped_(async_fn);

async_fn_(((mainAsync)(S$S_const$u8 args))(Void) $scope({
    var_(task, Co_CtxFn$(timesTwoAsync));
})($ignore, $ignore, $ignore)) {
    locals->task = async_ctx((timesTwoAsync)(none(), 10));
    await_(resume_(locals->task));
    io_stream_println(u8_l("result: {:d}"), Co_Ctx_returned(task));
    areturn_({});
} $unscoped_(async_fn);
```

### Atomic Values

Provides type-safe and intuitive API for load, store, CAS operations
by wrapping C11 Atomics.
> *TODO: document*
<!-- TODO: document -->

### SIMD Vectors

Provides vector parallel operation acceleration through a unified interface
independent of CPU architectures (AVX, NEON, etc.).
> *TODO: document*
<!-- TODO: document -->

### Meta System

Provides a generic data structure processing and serialization foundation
by leveraging compile-time type information (typeInfo$) and reflection.
> *TODO: document*
<!-- TODO: document -->

### Data Structures & Algorithms

Designs all data structures and functions to be dynamically allocated,
accepting allocators or memory buffers to fully control memory layout.
> *TODO: document*
<!-- TODO: document -->

### Testing

```c
#include "dh/main.h"
#include "dh/TEST.h"

// Define functions to test
fn_((mathAdd(i32 a, i32 b))(i32)) { return a + b; }
fn_((mathMul(i32 a, i32 b))(i32)) { return a * b; }

TEST_fn_("Basic Math Operations Test" $scope) {
    // Addition test
    let_(a, i32) = 5;
    let_(b, i32) = 7;
    let_(sum, i32) = mathAdd(a, b);

    // Validate results
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(sum > 10, "Sum should be greater than 10"));

    // Multiplication test
    let product = mathMul(a, b);
    try_(TEST_expect(product == 35));

    // Failing test (intentional error)
    let should_fail = TEST_expect(product == 30); // Fails: 35 != 30
    try_(TEST_expect(isErr(should_fail)));
} $unscoped_(TEST_fn);
```

</details>

---

## Documentation

- [Wiki Home](./dh/docs/en/home.md)
- [Quick Start Guide](./dh/docs/en/quick-start.md)
- [API Reference](./dh/docs/en/api/index.md)

---

## Naming

<!--
The project is currently in the early stages of development, and API specifications are experimental.
 -->

The name **"dasae-headers"** originates from the project's roots as a header-only library
designed to collect frequently used C utility code.

Due to continuous evolution and functional expansion, it has now adopted a structure that includes dedicated build tools and source files,
moving beyond the scope of a simple "header-only" library.
We maintain structural flexibility to enhance the user experience and allow for further high-level optimization.

Consequently, the current name does not yet fully fix the final identity of the project.
If you have suggestions for a unique name that better reflects the philosophy dasae-headers aims to achieve, please let us know :D

---

## Contribution and Contact

We welcome issues, feature requests, and pull requests!
Please refer to the [Contributing Guide](./dh/docs/en/contributing.md) for more details.

- **Author:** Gyeongtae Kim (dev-dasae)
- **Email:** [codingpelican@gmail.com](mailto:codingpelican@gmail.com)

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

Copyright © 2024-2025 Gyeongtae Kim.
