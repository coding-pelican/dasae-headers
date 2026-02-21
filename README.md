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
    <a href="./README.md">
      <img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English">
    </a>
    <a href="./README.ko.md">
      <img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean">
    </a>
  </div>

  <div style="margin-top: 8px;">
    <a href="https://en.wikipedia.org/wiki/C17_(C_standard_revision)">
      <img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17">
    </a>
    <a href="./dh/include/dh/builtin/plat_cfg.h">
      <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square"
           alt="Platform: Windows | Linux | macOS">
    </a>
    <a href="./LICENSE">
      <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT">
    </a>
  </div>
</div>

<details>
<summary><strong>Table of Contents</strong></summary>

- [dasae-headers](#dasae-headers)
  - [🚀 Getting Started](#-getting-started)
    - [💽 Installation](#-installation)
      - [Prerequisites](#prerequisites)
      - [Step 1: Clone the Repository](#step-1-clone-the-repository)
      - [Step 2: Build the `dh-c` Tool](#step-2-build-the-dh-c-tool)
      - [Step 3: Set Up Environment Variables](#step-3-set-up-environment-variables)
      - [Step 4: Verify Installation](#step-4-verify-installation)
      - [Step 5: Create a New Project](#step-5-create-a-new-project)
      - [Hello, world!](#hello-world)
    - [🔨 Build and Run](#-build-and-run)
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
  - [Modules Reference](#modules-reference)
      - [`builtin` — Compiler \& Platform Abstraction](#builtin--compiler--platform-abstraction)
      - [`core` — Language Primitives \& Syntax Extensions](#core--language-primitives--syntax-extensions)
      - [`prl` — Prelude Types](#prl--prelude-types)
      - [`simd` — SIMD Vector Operations](#simd--simd-vector-operations)
      - [`cmp` — Comparison Utilities](#cmp--comparison-utilities)
      - [`math` — Mathematical Functions](#math--mathematical-functions)
      - [`Rand` — Random Number Generation](#rand--random-number-generation)
      - [`search` — Searching Algorithms](#search--searching-algorithms)
      - [`sort` — Sorting Algorithms](#sort--sorting-algorithms)
      - [Linked Lists](#linked-lists)
      - [Tree Structures *(planned)*](#tree-structures-planned)
      - [Array-Based Containers](#array-based-containers)
      - [Hash-Based Containers](#hash-based-containers)
      - [`mem` — Memory Utilities](#mem--memory-utilities)
      - [`heap` — Heap Allocators](#heap--heap-allocators)
      - [`meta` — Runtime Record/Type Reflection](#meta--runtime-recordtype-reflection)
      - [`atom` — Atomic Operations](#atom--atomic-operations)
      - [`Thrd` — Threading](#thrd--threading)
      - [`Co` / `async` — Stackless Coroutines](#co--async--stackless-coroutines)
      - [`mp` — Multi-Processing *(planned)*](#mp--multi-processing-planned)
      - [`os` — OS-Specific APIs](#os--os-specific-apis)
      - [`posix` — POSIX Compatibility](#posix--posix-compatibility)
      - [`proc` — Process Management *(planned)*](#proc--process-management-planned)
      - [`time` — Time \& Duration](#time--time--duration)
      - [`io` — Input/Output](#io--inputoutput)
      - [`fmt` — Formatting](#fmt--formatting)
      - [`log` — Logging](#log--logging)
      - [`fs` — File System](#fs--file-system)
      - [`net` — Networking *(planned)*](#net--networking-planned)
      - [`http` — HTTP *(planned)*](#http--http-planned)
      - [`TEST` — Testing Framework](#test--testing-framework)
      - [`main` — Entry Point](#main--entry-point)
  - [Meta System](#meta-system)
  - [Platform Support](#platform-support)
  - [Code Samples](#code-samples)
    - [Optional Values](#optional-values)
    - [Error Results \& Defer](#error-results--defer)
    - [Pattern Matching](#pattern-matching)
    - [Chaining - Filter, Map, Fold, Reduce](#chaining---filter-map-fold-reduce)
    - [Threads vs Stackless-Coroutines](#threads-vs-stackless-coroutines)
    - [Atomic Values](#atomic-values)
    - [SIMD Vectors](#simd-vectors)
    - [Meta System](#meta-system-1)
    - [Data Structures \& Algorithms](#data-structures--algorithms)
    - [Testing](#testing)
  - [Documentation](#documentation)
  - [Naming](#naming)
  - [Contribution and Contact](#contribution-and-contact)
  - [License](#license)

</details>

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

> **Note:** Currently, only **Clang** is supported as the compiler.
> GCC support is planned but not yet available.

#### Prerequisites

- **Clang** 16.0.0+ (Recommended: 19.1.0+)
- **Make** (GNU Make or compatible)

#### Step 1: Clone the Repository

```sh
git clone https://github.com/coding-pelican/dasae-headers
cd dasae-headers
```

#### Step 2: Build the `dh-c` Tool

```sh
cd dh-c
./gen-makefile.sh
make
```

This compiles the `dh-c` build tool to `dh-c/build/dh-c`
(or `dh-c/build/dh-c.exe` on Windows).

#### Step 3: Set Up Environment Variables

You need to configure two environment variables:

| Variable  | Description                                   | Example Value                       |
| --------- | --------------------------------------------- | ----------------------------------- |
| `DH_HOME` | Path to the `dh` directory containing headers | `/path/to/dasae-headers/dh`         |
| `PATH`    | Add the `dh-c` binary location                | `/path/to/dasae-headers/dh-c/build` |

**Linux/macOS (bash/zsh):**

```sh
# Add to ~/.bashrc or ~/.zshrc
export DH_HOME="/path/to/dasae-headers/dh"
export PATH="/path/to/dasae-headers/dh-c/build:$PATH"
```

Then reload your shell configuration:

```sh
source ~/.bashrc  # or source ~/.zshrc
```

**Windows (MSYS2/MinGW):**

```sh
# Add to ~/.bashrc
export DH_HOME="/c/path/to/dasae-headers/dh"
export PATH="/c/path/to/dasae-headers/dh-c/build:$PATH"
```

**Windows (PowerShell):**

```powershell
# Add to your PowerShell profile or set as system environment variables
$env:DH_HOME = "C:\path\to\dasae-headers\dh"
$env:PATH = "C:\path\to\dasae-headers\dh-c\build;$env:PATH"
```

#### Step 4: Verify Installation

```sh
dh-c --version
dh-c --help
```

#### Step 5: Create a New Project

```sh
dh-c project myproject
cd myproject
dh-c run
```

#### Hello, world!

```c
#include <dh/main.h>
#include <dh/io/stream.h>

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let message = u8_l("Hello");
    io_stream_println(u8_l("{:s}, world!"), message);
    return_ok({});
} $unscoped_(fn);
```

### 🔨 Build and Run

```sh
# Run directly
dh-c run

# Build executable
dh-c build

# Run tests
dh-c test

# Clean build artifacts
dh-c clean
```

---

## Introduction

**dasae-headers** aims to improve the safety, expressiveness, and productivity of the C language
by introducing modern programming paradigms.
While maintaining the core principle of C—simplicity—it strengthens memory and type safety
and provides a structured error-handling mechanism.

Currently in the early stages of development, it provides a transpiler-like environment
leveraging the C preprocessor.
This project aims to compensate for the structural limitations of the standard C library
and is in the process of gradually establishing an independent development ecosystem.

### Why dasae-headers?

dasae-headers adheres to the following design principles to overcome the constraints
of traditional C environments and provide a modern development experience:

<details>
<summary><strong>Design Principles</strong></summary>

- **Seamless Coexistence with C Ecosystem:**
  Immediately introduce modern syntax and safety features
  without modifying existing C libraries or legacy codebases.
- **Zero-cost Abstractions:**
  Provide high-level features while minimizing runtime overhead through optimizations
  such as inlining, preprocessing-stage evaluation, and constant-folding.
- **Incremental Adoption:**
  Use only the necessary modules (e.g., error handling, allocators) selectively
  without needing to convert the entire project.
- **Freestanding and Bare-metal Support:**
  Support for freestanding environments—such as embedded systems or kernel development—
  is a top priority on the roadmap, making it effective at the lowest levels of the system.
- **Minimal User-defined Macros:**
  Users do not need to write additional complex macros to implement core features
  in general use cases.
  Generic instantiation is handled automatically during preprocessing
  as long as established patterns are followed.
- **Debug-friendly Design:**
  Macros are meticulously designed not to interfere with runtime debugging
  (call stack tracing, step-by-step execution), maintaining development productivity.
- **Consistent Conventions:**
  Strict and consistent code conventions and naming schemes ensure readability
  and maintainability even in large-scale codebases.

</details>

---

### How does it differ from traditional C?

<details>
<summary><strong>Show details</strong></summary>

While maintaining the flexibility of C, dasae-headers brings modern language safety features
and productivity tools in a form optimized for the system layer.
Rather than simply adding features, it focuses on structurally addressing C's chronic design flaws
and fragmented conventions.

#### 1. Code Pattern & Platform Abstraction

Unifies fragmented language/architecture/OS/compiler-specific APIs and complex syntax
of standard C into a single interface.

| Aspect                      | Traditional C (Standard C)                                  | dasae-headers                                                                       |
| --------------------------- | ----------------------------------------------------------- | ----------------------------------------------------------------------------------- |
| **Variables and Functions** | Explicit type declarations and repetitive signatures        | `let` (constant), `var` (mutable) type inference and `fn_` modern syntax            |
| **Lambda/Closures**         | GCC nested functions or Clang blocks, platform-dependent    | `la_` syntax unifying both compiler extensions, `Callable` type for `fn_` and `la_` |
| **Platform Support**        | Fragmented branching with `#ifdef`                          | **Unified API** (`{lang/arch/plat/comp}_cfg.h`, `os.h`, `posix.h`) provided         |
| **Preprocessor Branching**  | Separate `#ifdef` branch definitions even for simple values | Single definition with preprocessor branching via `pp_if_`, `pp_switch_`            |

#### 2. Memory & Argument Patterns

Avoids unsafe patterns of returning success as `bool` and receiving result values
via `out` parameters.
`Optional` and `Error Result` enforce validation of absent values or error conditions
at the type system level.

| Aspect               | Traditional C (Standard C)                   | dasae-headers                                                      |
| -------------------- | -------------------------------------------- | ------------------------------------------------------------------ |
| **Resource Release** | `goto cleanup` or scattered manual cleanup   | Automatic scope-based cleanup with `defer_`, `errdefer_`           |
| **Result Return**    | `bool`/error code return + `out` pointer     | **Optional** (`O$`) / **Error Result** (`E$`) return               |
| **Result Branching** | Manual validation with `if (err)` branches   | Result control with `orelse_`, `unwrap_` / `try_`, `catch_` syntax |
| **Data Transfer**    | Pointer and length (`len`) passed separately | **Slice** (`S$`) or **array as value** (`A$`) transfer             |
| **Strings/Arrays**   | `0(NULL)` sentinel-based implicit length     | Explicit length field-based **Boundary Check**                     |

#### 3. Concurrency

Provides ultra-lightweight asynchronous environments capable of handling tens of thousands of tasks
simultaneously without OS thread overhead.
Expresses state-machine-based control flow concisely with `async_/await_` patterns.

| Aspect              | Traditional C (Standard C)                           | dasae-headers                                                           |
| ------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------- |
| **Async Model**     | OS native thread-centric design                      | OS native threads + **State-machine-based Stackless Coroutines**        |
| **Sync Primitives** | Reliant on primitive `mutex`, `cond`                 | `RWLock`, `ResetEvent`, `WaitGroup`, etc.                               |
| **Control Flow**    | Callback hell or manually implemented state machines | Coroutine control with `async_`, `await_`, `suspend_`, `resume_` syntax |

#### 4. Meta Type & Generic System

Escapes macro hell and validates type safety at compile time through the meta type system.
Provides a differentiated layer that statically guarantees logical compatibility
between anonymous user types not allowed in the C standard.
See [Meta System](#meta-system) for how the meta type system and the `meta` module
(record reflection) work and relate.

| Aspect              | Traditional C (Standard C)                                           | dasae-headers                                                                                     |
| ------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| **Implementation**  | `void*` casting (type information loss)                              | High-level abstraction based on **Meta System** (`u_*`)                                           |
| **Anonymous Types** | Assignment impossible even with identical structure due to anonymity | Compatibility guaranteed when field memory layout and logical structure (Size/Align/Offset) match |
| **API Exposure**    | Data structure API implementation directly embedded in macros        | **Macro-less API**: Adheres to standard function definition format                                |

#### 5. Numerical & Safety

Abstracts hardware architecture-specific SIMD instructions and statically detects
arithmetic overflow and inappropriate type casting.
All safety features are optimized in release mode to ensure zero runtime overhead.

| Aspect                | Traditional C (Standard C)                                                | dasae-headers                                                                             |
| --------------------- | ------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| **Vector Ops**        | Manual loop-based operations or platform-dependent APIs                   | Architecture-independent accelerated operations via `simd.h`                              |
| **Arithmetic Safety** | Vulnerable to runtime exceptions like `Overflow`, `DivisionByZero`, `NaN` | Compile-time **Overflow/NaN static detection** layer                                      |
| **Type Casting**      | Risk of data loss due to implicit type conversion                         | Mismatch tracking for `Signed`/`Unsigned` and size conversion, `int`<->`float` conversion |

#### 6. Ecosystem & Infrastructure

Manages the entire process from project creation to build and test with built-in tools
without additional dependencies.
When errors occur, preserves call stack information beyond simple return values
to immediately pinpoint the cause.

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

Beyond simple syntax extensions, dasae-headers leverages deep understanding
of compiler and static analyzer behavior to provide a user-centric development experience
differentiated from other projects.

#### 1. Maximizing Static Analysis and IDE Compatibility

**Static Analyzer Transparency:**

Designed so that major analysis tools like `clang-tidy` can understand the source code
the same way as regular C code.
In particular, it blocks static analyzer malfunctions (e.g. type mismatches from checking
all branches) that occur when using `_Generic`, allowing developers to focus only
on "real" warnings and errors, not "recognition errors."

**Transparent API Exposure (Macro-less API):**

Core APIs, including data structures, are not wrapped in complex macro functions.
Macros focus on acting as operators that replace special symbols,
while actual logic follows standard function definition formats.
This eliminates the burden of writing separate macros or understanding complex macro structures
to call libraries, defining a predictable standard for "macros as operators"
vs "actual function usage" in core logic.

#### 2. Meta System and Anonymous Type Compatibility

**Meta Type-based Generic System:**

Implements generic logic through the meta type system (`u_*`) rather than macros,
which are blind spots for static analysis.
Provides a zero-cost abstraction layer that optimizes like inlined code via LTO,
constant folding, and constant propagation, even without inlining via macros.
See [Meta System](#meta-system) for the full picture
(meta type system + `meta` module and their relationship).

**Structural Anonymous Type Compatibility Validation:**

Unlike other libraries that require type aliasing via `typedef` to allow generics,
effectively accommodates anonymous user types.
Statically validates safety at compile time when field layout (Size, Align, Offset) matches,
ensuring safe interaction between anonymous types.

#### 3. Zero-overhead Safety

**Compile-time Evaluation Priority for Operations and Casting Validation:**

Statically detects arithmetic `Overflow`, `DivisionByZero`, `NaN`, and inappropriate type casting.
All safety validations prioritize compile-time evaluation and function as `assertions` at runtime.
In release mode, these validation items become branch optimization targets,
securing safety without binary overhead, while allowing faster and more accurate identification
of problem causes during development compared to traditional C.

</details>

---

## Key Features

This project was developed with inspiration from the syntax structures
and standard library designs of Zig and Rust.

- **Memory Safety:**
  Custom allocators, memory tracking, boundary checking,
  `defer_`/`errdefer_`-based automatic resource management
- **Enhanced Type System:**
  Compile-time validation, meta type system, algebraic data types (Variant), optional types
- **Explicit Error Handling:**
  `ok`/`err` keywords, `try_`/`catch_` patterns, error tracing with call stack information
- **Modern Syntax:**
  Type inference (`let`/`var`), function definition (`fn_`), lambda expressions (`la_`),
  first-class `Callable` type
- **Development Tools:**
  Built-in testing framework, support for major C compilers (Clang, GCC)
  and multi-platform environments

---

## Modules Reference

> Legend: Items marked with *(planned)* are under development and not yet publicly available.

<details>
<summary><strong>Core Language Extensions</strong></summary>

#### `builtin` — Compiler & Platform Abstraction

Preprocessor utilities, platform/compiler detection, source location tracking,
type and container introspection.

- **Submodules:**
  `pp`, `lang_cfg`, `arch_cfg`, `plat_cfg`, `comp_cfg`, `comp`, `src_loc`,
  `static_assert`, `auto`, `lambda`, `type_info`, `container_info`, `mem`, `atom`
- **`pp` (preprocessor):** Implements compile-time metaprogramming as preprocessor macros:
  - **Token comparison:**
    `pp_eq`, `pp_ne`, `pp_and`, `pp_or`, `pp_xor`, `pp_not`, `pp_Tok_eq`
    — token equality and logic
  - **Control flow:**
    `pp_if_` / `pp_then_` / `pp_else_` — conditional;
    `pp_switch_` / `pp_case_` / `pp_default_` — dispatch
  - **Optionals:**
    `pp_some`, `pp_none`, `pp_isSome`, `pp_isNone`, `pp_orelse_`
    — preprocessor optional values
  - **Utilities:**
    `pp_strfy`, `pp_cat`, `pp_join`, `pp_join2`/`pp_join3`/`pp_join4`, `pp_uniqTok`,
    `pp_overload`, `pp_foreach`, `pp_countArg`, `pp_defer`, `pp_expand`, `nameOf`
- **Platform/compiler:**
  `lang_cfg`, `arch_cfg`, `plat_cfg`, `comp_cfg` — detection macros
- **Source location:**
  `srcLoc$` — compile-time source location capture
- **Type introspection (builtin):**
  `typeInfo$`, `sizeOf$`, `alignOf$`, `TypeOf`, `TypeOfUnqual`, `Type_eq$`,
  `RefType$`, `DerefType$`
- **Container/field introspection:**
  `FieldType$`, `FieldTypeUnqual$`, `FieldType_eq$`, `offsetTo`, `fieldPtr`
  (in `container_info`)

#### `core` — Language Primitives & Syntax Extensions

Primitives, function syntax, assertions, debugging, scoped resource management,
and safe arithmetic.

- **Submodules:**
  `prim`, `fn`, `Callable`, `claim`, `debug`, `range`, `op`, `cmp`, `pipe`, `chain`,
  `blk`, `scope`, `src_loc`, `type_info`, `struct_layout`
- **Primitive types:**
  `bool`, `i8`..`i64`, `u8`..`u64`, `isize`, `usize`, `f32`, `f64`
- **Syntax:**
  `fn_`, `la_` — function and lambda; `Callable` — first-class callable
- **Assertions:**
  `claim_assert`, `claim_unreachable`; `debug_assert`, `debug_only`
- **Control/scope:**
  `R`, `for_` — range iteration; `defer_`, `errdefer_` — scoped cleanup
- **Chaining:**
  `pipe_`, `chain$`, `each_`, `filter_`, `map$`, `fold_`, `reduce_`, `all_`, `any_`
- **Core primitives (arithmetic, bitwise, comparison):**
  `prim_add`, `prim_sub`, `prim_mul`, `prim_div`, `prim_rem`, `prim_neg`, `prim_abs`,
  `prim_sgn`, `prim_not`, `prim_shl`, `prim_shr`, `prim_and`, `prim_xor`, `prim_or`,
  `prim_eql`, `prim_neq`, `prim_ord`, `prim_min`, `prim_max`, `prim_clamp`,
  `prim_bitSet`, `prim_bitReset`, `prim_maskLo$`, `prim_maskHi$`,
  `prim_memcpy`, `prim_memset`, `prim_memmove`, `prim_memeql`, `bitCast$`
- **Safe integer ops (overflow-checked, in `prim`):**
  `int_add`, `int_sub`, `int_mul`, `int_div`, `int_rem`, `int_divTrunc`, `int_divRound`,
  `int_divCeil`, `iint_divFloor`, `iint_divEuclid`, `iint_mod`, `intCast$`
- **Integer/float division variants:**
  `int_remRound`, `int_modCeil`; `flt_divTrunc`, `flt_divFloor`, `flt_rem`, `flt_mod`
- **Type predicates & limits:**
  `isBool$`, `isUInt$`, `isIInt$`, `isFlt$`, `int_bits$`, `uint_limit$`,
  `int_limit_min$`, `int_limit_max$`, `flt_limit_min$`, `flt_limit_max$`,
  `flt_nan$`, `flt_inf$`
- **Type info (core):**
  `typeInfo$` — runtime `TypeInfo` (size, align) for a type

#### `prl` — Prelude Types

Core algebraic types: Optional, Error Result, Slice, Array, Variant.
(Safe arithmetic lives in `core`/`prim`.)

- **Submodules:**
  `types` (`raw`, `Val`, `Ptr`, `Arr`, `Sli`, `Opt`, `ErrRes`, `Err`, `variant`, `meta`),
  `common`, `ErrTrace`, `int`, `flt`
- **Key Types:**
  - `O$T` (Optional) — `some(v)`, `none()`, `if_some`, `orelse_`, `unwrap_`
  - `E$T` (Error Result) — `ok(v)`, `err(e)`, `try_`, `catch_`, `return_ok`, `return_err`
  - `S$T` (Slice) — `S_at`, `S_slice`, `S_first`, `S_last`
  - `A$N$T` (Array) — `A_from$`, `A_ref$`
  - `variant_` — Tagged union (algebraic data type)
  - `ErrTrace` — Error tracing with call stack information
- **prl/int, prl/flt:**
  Per-type safe wrappers (e.g. `u8_add`, `u32_div`, `i64_mod`) with debug overflow checks;
  see `core`/`prim` for generic `int_add`, `intCast$`, etc.
- **Zero-cost meta type system** (`prl/types/meta`):
  Type-erased generic layer over PRL types so algorithms can work on values
  without knowing the concrete type at compile time.
  How it works and how it relates to the `meta` module (record/type reflection)
  are described in [Meta System](#meta-system).

</details>

<details>
<summary><strong>SIMD & Math</strong></summary>

#### `simd` — SIMD Vector Operations

Architecture-independent SIMD operations (AVX, SSE, NEON) using compiler vector extensions.

- **Types:**
  `Vec$(N, T)` — e.g., `Vec$(4, f32)`, `Vec$(8, f32)`, `Vec$(4, i32)`, `Vec$(2, f64)`
- **Initialization:**
  `Vec_init$`, `Vec_splat$`, `Vec_from$`, `Vec_fromA$`, `Vec_toA$`, `Vec_cat$`
- **Arithmetic:**
  `Vec_add`, `Vec_sub`, `Vec_mul`, `Vec_div`, `Vec_neg`, `Vec_abs`, `Vec_fma`
- **Comparison:**
  `Vec_eq`, `Vec_ne`, `Vec_lt`, `Vec_le`, `Vec_gt`, `Vec_ge`
- **Min/Max:**
  `Vec_min`, `Vec_max`, `Vec_clamp`
- **Reduction:**
  `Vec_reduceAdd`, `Vec_reduceMul`, `Vec_reduceMin`, `Vec_reduceMax`
- **Shuffle:**
  `Vec_shuffle$`, `Vec_select`
- **Math:**
  `Vec_sqrt`, `Vec_floor`, `Vec_ceil`, `Vec_round`, `Vec_trunc`

#### `cmp` — Comparison Utilities

- **`cmp_Ord`** — Three-way ordering type:
  `cmp_Ord_lt` (−1), `cmp_Ord_eq` (0), `cmp_Ord_gt` (1).
  Represents the mathematical sign of (lhs − rhs): strictly less, equal, or strictly greater.
  Used when a total (or partial) order exists;
  all ordering-derived predicates are defined in terms of `cmp_ord$(_T)(lhs, rhs)`.
- **`cmp_Eql`** — Equality-only comparison (no order).
  Used when only equivalence is defined:
  `cmp_eql$(_T)(lhs, rhs)` and `cmp_neq$(_T)(lhs, rhs)`.
- **Derived names and defaults:**
  - **ord** — Three-way comparison; default direction is *ascending* (smaller-first).
    `cmp_OrdFn_defaultAsc` / `cmp_OrdFn_defaultDesc` (in `cmp.h`)
    select ascending or descending.
  - **eql** / **neq** — Equality and its negation; *not* derived from order.
    Implement `cmp_eql$(_T)` (and optionally `cmp_neq$(_T)`;
    the other is defaulted as its negation).
  - **eq** / **ne** — Equality/inequality *derived from* `cmp_ord$`:
    `eq` ⇔ (ord == cmp_Ord_eq), `ne` ⇔ (ord != cmp_Ord_eq).
    So for ordered types, `cmp_eq$`/`cmp_ne$` are defaulted from `cmp_ord$`;
    for equality-only types you implement `cmp_eql$`/`cmp_neq$` only.
- **Defining ordering (ord vs lt):**
  You can supply ordering for a type in either of two ways.
  **(1) ord defined:** Implement `cmp_ord$(_T)(lhs, rhs)` returning `cmp_Ord` (three-way).
  Then `cmp_eq$`, `cmp_ne$`, `cmp_lt$`, `cmp_gt$`, `cmp_le$`, `cmp_ge$` are defaulted from it
  via the `cmp_fn_*_default$` macros.
  **(2) lt only:** Implement only `cmp_lt$(_T)(lhs, rhs)` (strict less-than, returns bool).
  Use `cmp_fn_ord_default$(_T)` to derive ord from lt
  (ord = lt(lhs,rhs) ? lt : lt(rhs,lhs) ? gt : eq);
  then eq/ne/lt/gt/le/ge are defaulted from that ord as in (1).
  The same pattern applies to context and approximate variants: `ordCtx`/`ltCtx`, `ordApx`/`ltApx`.
- **Distinction:**
  **eql**/**neq** are the *primitive* equality interface
  (reflexive, symmetric, and for consistent types transitive).
  **eq**/**ne** are the *order-derived* equality:
  they coincide with eql/neq when the order is total and consistent with equality,
  but eq/ne are defined only when `cmp_ord$` exists.
  Use eql/neq for types that have equality but no order (e.g. slices by content);
  use eq/ne when you have an order and want (ord == cmp_Ord_eq) as equality.
- **Predicates on `cmp_Ord`:**
  `cmp_Ord_isLt`, `cmp_Ord_isEq`, `cmp_Ord_isGt`, `cmp_Ord_isLe`, `cmp_Ord_isGe`, `cmp_Ord_inv`

#### `math` — Mathematical Functions

- **Submodules:**
  `common`, `vec`/`vec_types`, `mat`/`mat_types`, `quat`/`quat_types`,
  `geom`/`geom_types`, `interp`, `ease`
- **Common:**
  `math_abs`, `math_min`, `math_max`, `math_clamp`, `math_sign`,
  `math_floor`, `math_ceil`, `math_round`, `math_sqrt`,
  `math_pow`, `math_sin`, `math_cos`, `math_tan`
- **Vectors:**
  `Vec2`, `Vec3`, `Vec4` —
  `add`, `sub`, `mul`, `div`, `dot`, `cross`, `normalize`, `length`, `distance`
- **Matrices:**
  `Mat2`, `Mat3`, `Mat4` —
  `identity`, `mul`, `transpose`, `inverse`, `determinant`, `translate`, `rotate`, `scale`
- **Quaternions:**
  `Quat` —
  `identity`, `mul`, `conjugate`, `inverse`, `normalize`, `slerp`, `fromAxisAngle`, `toMat`
- **Geometry:**
  `geom_Point`, `geom_Line`, `geom_Ray`, `geom_Plane`, `geom_AABB`, `geom_Sphere`
  *(types defined)*
- **Interpolation:**
  `interp_linear`, `interp_smoothstep`, `interp_bezier`
- **Easing:**
  `ease_inQuad`, `ease_outQuad`, `ease_inOutQuad`, `ease_inCubic`, etc.

#### `Rand` — Random Number Generation

Random number generator (struct `Rand`, prefix `Rand_`).

- **Init/seed:**
  `Rand_init`, `Rand_initSeed`, `Rand_withSeed`, `Rand_setSeed`
- **Next:**
  `Rand_nextUInt`, `Rand_next$usize`, `Rand_next$u64`, `Rand_next$u32`,
  `Rand_next$u16`, `Rand_next$u8`, `Rand_nextIInt`, `Rand_next$isize`,
  `Rand_next$i64`/…, `Rand_nextFlt`, `Rand_next$f64`, `Rand_next$f32`
- **Ranges:**
  `Rand_rangeUInt`, `Rand_rangeIInt`, `Rand_rangeFlt`

</details>

<details>
<summary><strong>Algorithms</strong></summary>

#### `search` — Searching Algorithms

| Function             | Time     | Description                              |
| -------------------- | -------- | ---------------------------------------- |
| `search_linearFirst` | O(N)     | Find first matching index (forward scan) |
| `search_linearLast`  | O(N)     | Find last matching index (backward scan) |
| `search_binary`      | O(log N) | Binary search on partitioned sequence    |
| `search_lowerBound`  | O(log N) | First index where element ≥ target       |
| `search_upperBound`  | O(log N) | First index where element > target       |
| `search_partPoint`   | O(log N) | Partition point where predicate changes  |
| `search_eqRange`     | O(log N) | Range of indices equal to target         |

#### `sort` — Sorting Algorithms

Optimal stable and unstable sorting functions isolated by auxiliary memory constraints.

| Function          | Stability | Time       | Space    | Description                                      |
| ----------------- | --------- | ---------- | -------- | ------------------------------------------------ |
| `sort_insert`     | Stable    | O(N²)      | O(1)     | Insertion sort for small/partially sorted arrays |
| `sort_heap`       | Unstable  | O(N log N) | O(1)     | Heapsort for strict O(1) memory constraints      |
| `sort_pdq`        | Unstable  | O(N log N) | O(log N) | Pattern-Defeating Quicksort, adaptive            |
| `sort_block`      | Stable    | O(N log N) | O(1)     | Block sort (WikiSort) with internal buffers      |
| `sort_blockCache` | Stable    | O(N log N) | O(K)     | Block sort with provided cache buffer            |
| `sort_blockAlloc` | Stable    | O(N log N) | O(K)     | Block sort with allocator-provided buffer        |

- **Context variants:**
  `sort_insertCtx`, `sort_heapCtx`, `sort_pdqCtx`, `sort_blockCtx`
- **Index-based variants:**
  `sort_insertIdx`, `sort_heapIdx`, `sort_pdqIdx` (for non-contiguous layouts)
- **Utilities:**
  `sort_inOrdd` (check if sorted)

</details>

<details>
<summary><strong>Data Structures</strong></summary>

#### Linked Lists

| Module        | Description        | Key Functions                                          |
| ------------- | ------------------ | ------------------------------------------------------ |
| **`ListSgl`** | Singly linked list | `empty`, `prepend`, `remove`, `shift`                  |
| **`ListDbl`** | Doubly linked list | `empty`, `prepend`, `append`, `remove`, `shift`, `pop` |

#### Tree Structures *(planned)*

| Module         | Description                          | Key Functions |
| -------------- | ------------------------------------ | ------------- |
| **`BTree`**    | B-tree *(planned)*                   | —             |
| **`BTreeMap`** | B-tree based ordered map *(planned)* | —             |
| **`BTreeSet`** | B-tree based ordered set *(planned)* | —             |
| **`SegTree`**  | Segment tree *(planned)*             | —             |

#### Array-Based Containers

| Module        | Description                                   | Key Functions                                                                                           |
| ------------- | --------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| **`ArrList`** | Dynamic array list with amortized O(1) append | `init`, `fini`, `append`, `appendS`, `pop`, `insert`, `removeOrdd`, `removeSwap`, `resize`, `ensureCap` |
| **`ArrStk`**  | Array-based stack (LIFO)                      | `init`, `fini`, `push`, `pop`, `top`, `isEmpty`                                                         |
| **`ArrDeq`**  | Array-based double-ended queue                | `init`, `fini`, `append`, `prepend`, `pop`, `shift`                                                     |
| **`ArrQue`**  | Array-based queue (FIFO)                      | `init`, `fini`, `enque`, `deque`, `front`                                                               |
| **`ArrPDeq`** | Array-based priority double-ended queue       | `init`, `fini`, `enque`, `popMin`, `popMax`, `peekMin`, `peekMax`                                       |
| **`ArrPQue`** | Array-based priority queue (binary heap)      | `init`, `fini`, `enque`, `deque`, `peek`, `update`                                                      |

#### Hash-Based Containers

| Module           | Description                                             | Key Functions                                                                      |
| ---------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| **`Hash`**       | Hash utilities *(planned)*                              | —                                                                                  |
| **`HashMap`**    | Hash map with open addressing                           | `init`, `fini`, `put`, `by`, `ptrBy`, `for`, `entry`, `contains`, `remove`, `iter` |
| **`HashSet`**    | Hash set with open addressing                           | `init`, `fini`, `put`, `ensure`, `contains`, `remove`, `iter`                      |
| **`HashMapSeq`** | Ordered hash map preserving insertion order *(planned)* | —                                                                                  |
| **`HashSetSeq`** | Ordered hash set preserving insertion order *(planned)* | —                                                                                  |

</details>

<details>
<summary><strong>Memory Management</strong></summary>

#### `mem` — Memory Utilities

Low-level memory operations with type-safe wrappers.

- **Submodules:** `common`, `Allocator`, `Tracker`
- **Bit Operations:**
  `mem_trailingZeros{8,16,32,64,Size}`, `mem_leadingZeros{8,16,32,64,Size}`
- **Byte Swap:**
  `mem_byteSwap{16,32,64,Size}`
- **Endian Conversion:**
  `mem_littleToNative*`, `mem_bigToNative*`, `mem_nativeToLittle*`, `mem_nativeToBig*`
- **Alignment:**
  `mem_isValidAlign`, `mem_isAligned`, `mem_alignFwd`, `mem_alignBwd`, `mem_alignToLog2`
- **Memory Operations:**
  - `mem_copy`, `mem_move`, `mem_set`, `mem_set0` — Copy, move, fill
  - `mem_eql`, `mem_ord`, `mem_eq`, `mem_ne`, `mem_lt`, `mem_gt`, `mem_le`, `mem_ge`
    — Comparison
  - `mem_swap`, `mem_reverse`, `mem_rotate` — Manipulation
  - `mem_startsWith`, `mem_endsWith` — Pattern matching
- **Iterators:**
  - `mem_TokenIter` — Tokenizer with value/pattern/choice delimiters
  - `mem_tokenizeValue`, `mem_tokenizePattern`, `mem_tokenizeChoice`
  - `mem_SplitIter`, `mem_WindowIter` *(planned)*
- **Allocator Interface:**
  `mem_Allocator` — Unified allocator interface
- **Tracker:**
  `mem_Tracker` — Memory leak and double-free detection

#### `heap` — Heap Allocators

| Allocator      | Description                                              |
| -------------- | -------------------------------------------------------- |
| **`Classic`**  | Traditional heap allocation (C runtime `malloc`/`free`)  |
| **`Page`**     | Page-aligned allocation for OS-mapped memory blocks      |
| **`Sbrk`**     | Sbrk-based allocation for linear memory growth and reuse |
| **`Sys`**      | System-dependent allocation (Page or Sbrk)               |
| **`Fixed`**    | Fixed-size block allocator for bulk operations           |
| **`ThrdSafe`** | Thread-safe wrapper for any allocator                    |
| **`Smp`**      | SMP-aware allocation with per-core caching               |
| **`Arena`**    | Region-based allocation for bulk operations              |
| **`Pool`**     | Pool-based allocation for object reuse                   |

#### `meta` — Runtime Record/Type Reflection

Record layout and field access from `TypeInfo`, operating on the **meta type system**
(`u_P$raw`, `u_S$raw`).
For the relationship between this module and the meta type system (prl/types/meta),
see [Meta System](#meta-system).
Compile-time type info is in `builtin`/`core` (`typeInfo$`, `sizeOf$`, `alignOf$`).

- **Record from fields:**
  `u_typeInfoRecord`, `u_sizeOfRecord`, `u_alignOfRecord`
- **Field offsets:**
  `u_offsetTo`, `u_offsets`
- **Field/record pointers (meta):**
  `u_fieldPtr`, `u_fieldPtrMut`, `u_fieldPtrs`, `u_fieldPtrsMut`,
  `u_recordPtr`, `u_recordPtrMut`
- **Array type info:**
  `u_typeInfoA`, `u_sizeOfA`, `u_alignOfA`
- **N-replicated records:**
  `u_typeInfoRecordN`, `u_offsetToN`, `u_offsetsN`, `u_fieldSli`, `u_fieldSliMut`,
  `u_fieldSlis`, `u_fieldSlisMut`, `u_recordNPtr`, `u_recordNPtrMut`

</details>

<details>
<summary><strong>Concurrency</strong></summary>

#### `atom` — Atomic Operations

C11 atomics and type-safe atomic value wrappers (prefix `atom_`).

- **Ordering:**
  `atom_MemOrd` — `unordered`, `monotonic`, `acquire`, `release`, `acq_rel`, `seq_cst`
- **Operations:**
  `atom_fence`, `atom_load`, `atom_store`, `atom_fetchXchg`,
  `atom_cmpXchgWeak$`, `atom_cmpXchgStrong$`,
  `atom_fetchAdd`, `atom_fetchSub`, `atom_fetchAnd`, `atom_fetchOr`,
  `atom_fetchXor`, `atom_fetchNand`
- **Atomic value wrapper:**
  `atom_V$(_T)`, `atom_V_zero$`, `atom_V_init$`, `atom_V_from`
  — generic atomic variable type

#### `Thrd` — Threading

OS thread management and synchronization primitives.

| Primitive        | Description                          |
| ---------------- | ------------------------------------ |
| **`Thrd`**       | Thread creation, spawn, join, detach |
| **`Ftx`**        | Futex (fast userspace mutex)         |
| **`Mtx`**        | Mutex                                |
| **`Sem`**        | Semaphore                            |
| **`Cond`**       | Condition variable                   |
| **`RWLock`**     | Read-write lock                      |
| **`ResetEvent`** | Manual/auto reset event              |
| **`WaitGroup`**  | Wait for multiple tasks              |

- **Key Functions:**
  `Thrd_spawn`, `Thrd_join`, `Thrd_detach`, `Thrd_current`, `Thrd_yield`, `Thrd_sleep`

#### `Co` / `async` — Stackless Coroutines

State-machine-based coroutines for ultra-lightweight async processing.

- **Syntax:**
  `async_fn_`, `await_`, `suspend_`, `resume_`, `areturn_`
- **Context:**
  `Co_Ctx`, `Co_CtxFn$`

#### `mp` — Multi-Processing *(planned)*

Multi-processing utilities for parallel workloads.

</details>

<details>
<summary><strong>Text & Encoding</strong></summary>

| Module        | Description                        | Key Functions                                                                                            |
| ------------- | ---------------------------------- | -------------------------------------------------------------------------------------------------------- |
| **`ascii`**   | ASCII character utilities          | `ascii_isAlpha`, `ascii_isDigit`, `ascii_isSpace`, `ascii_toLower`, `ascii_toUpper`                      |
| **`utf8`**    | UTF-8 encoding/decoding            | `utf8_encode`, `utf8_decode`, `utf8_isValid`, `utf8_validate`, `utf8_count`, `utf8_view`, `utf8_iter`    |
| **`utf16`**   | UTF-16 encoding/decoding           | `utf16_encode`, `utf16_decode`, `utf16_codepointSeqLen`, `utf16_isHighSurrogate`, `utf16_isLowSurrogate` |
| **`wtf8`**    | WTF-8 (UTF-8 superset for Windows) | `wtf8_encode`, `wtf8_decode`, `wtf8_view`, `wtf8_iter`                                                   |
| **`wtf16`**   | WTF-16 encoding                    | `wtf16_iter`, `wtf16_Iter_next`                                                                          |
| **`unicode`** | Unicode conversion hub             | `unicode_utf8ToUTF16`, `unicode_utf16ToUTF8`, `unicode_wtf8ToWTF16`, `unicode_wtf16ToWTF8`               |

</details>

<details>
<summary><strong>System & OS</strong></summary>

#### `os` — OS-Specific APIs

- **Windows:**
  `cfg`, `base`, `handle`, `debug`, `nls`, `sysinfo`, `mem`, `file`, `io`, `dll`,
  `console`, `proc`, `thrd`, `sync`, `nt`, `auth`, `crypt`, `sock`

#### `posix` — POSIX Compatibility

POSIX API compatibility layer for cross-platform code.

#### `proc` — Process Management *(planned)*

Process management utilities for cross-platform code.

#### `time` — Time & Duration

- **Submodules:**
  `cfg`, `common`, `Duration`, `Instant`, `SysTime`
- **Duration:**
  `time_Duration_fromSecs`, `time_Duration_fromMillis`, `time_Duration_fromNanos`,
  `time_Duration_add`, `time_Duration_sub`
- **Instant:**
  `time_Instant_now`, `time_Instant_elapsed`, `time_Instant_durationSince`
- **SysTime:**
  `time_SysTime_now`
- **Sleep:**
  `time_sleep`

</details>

<details>
<summary><strong>I/O & Formatting, Filesystem</strong></summary>

#### `io` — Input/Output

- **Submodules:** `common`, `stream`, `Reader`, `Writer`
- **Stream:**
  `io_stream_print`, `io_stream_println`, `io_stream_eprint`, `io_stream_eprintln`
- **Reader:**
  `io_Reader_read`, `io_Reader_readExact`, `io_Reader_readByte`, `io_Reader_skip`
- **Writer:**
  `io_Writer_write`, `io_Writer_writeBytes`, `io_Writer_writeByte`,
  `io_Writer_print`, `io_Writer_println`, `io_Writer_nl`

#### `fmt` — Formatting

String formatting and parsing with a spec system (prefix `fmt_`).

- **Submodules:** `common`, `cfg`
- **Format API:**
  `fmt_format`, `fmt_formatVaArgs` — format to writer;
  `fmt_Spec`, `fmt_Type`, `fmt_Size`, `fmt_Align`, `fmt_Sign`
- **Format by type:**
  `fmt_formatBool`, `fmt_format$bool`,
  `fmt_formatUInt`, `fmt_format$usize`/`$u64`/`$u32`/…,
  `fmt_formatIInt`, `fmt_format$isize`/…,
  `fmt_formatFlt`, `fmt_formatPtr`, `fmt_formatErr`,
  `fmt_formatStr`, `fmt_formatStrZ`, `fmt_formatASCII`, `fmt_formatUTF8`
- **Parse:**
  `fmt_parseBool`, `fmt_parse$bool`,
  `fmt_parseUInt`, `fmt_parse$usize`/…,
  `fmt_parseIInt`, `fmt_parseFlt`, …
- **Type specifiers (fmt_Type):**
  `{:x}`/`{:X}` (hex), `{:o}` (octal), `{:b}` (binary), `{:B}` (boolean),
  `{:u}` (unsigned), `{:di}` (signed), `{:f}`/`{:F}` (float), `{:p}`/`{:P}` (pointer),
  `{:e}` (error), `{:0}` (void), `{:c}` (ASCII code), `{:C}` (UTF-8 codepoint),
  `{:z}` (null-terminated string), `{:s}` (slice string)
- **Float conversion:**
  Ryu-based fast float-to-string (configurable via `fmt_cfg`)

#### `log` — Logging

- **Levels:** `log_debug`, `log_info`, `log_warn`, `log_error`

#### `fs` — File System

- **Submodules:** `common`, `path`, `File`, `Dir`
- **File:**
  `fs_File_close`, `fs_File_reader`, `fs_File_writer`, `fs_File_Handle_promote`
- **Dir:**
  `fs_Dir_create`, `fs_Dir_close`, `fs_Dir_openDir`, `fs_Dir_openFile`,
  `fs_Dir_createFile`, `fs_Dir_deleteFile`, `fs_Dir_deleteDir`,
  `fs_Dir_readFile`, `fs_Dir_readFileAlloc`
- **Path:**
  `fs_path_join`, `fs_path_dirname`, `fs_path_basename`
  *(partial — `fs_path_extension` planned)*

#### `net` — Networking *(planned)*

Networking utilities for cross-platform code.

#### `http` — HTTP *(planned)*

HTTP client/server utilities for cross-platform code.

</details>

<details>
<summary><strong>Testing & Entry Point</strong></summary>

#### `TEST` — Testing Framework

- **Macros:**
  `TEST_fn_`, `TEST_expect`, `TEST_expectMsg`, `TEST_expectEq`, `TEST_expectNe`
- **Usage:**
  Define tests with `TEST_fn_`, run with `dh-c test`

#### `main` — Entry Point

- **Macro:**
  `fn_((main(S$S_const$u8 args))(E$void) $scope)`
  — Standard entry point with argument parsing and error handling

</details>

---

## Meta System

The meta system has two related parts: the **meta type system** (type-erased generics)
and the **meta module** (record/type reflection).
Both use the same `u_` prefix and share `TypeInfo` from `core`/`type_info.h`.

---

**1. Meta type system** (`prl/types/meta.h`, included via `prl/types.h`)

A **zero-cost generic layer** over PRL types so algorithms can operate on values
without knowing the concrete type at compile time.

- **What it is:**
  Type-erased wrappers that carry a raw representation (pointer, slice, array, optional, or result)
  plus runtime **type information** (`TypeInfo`: size, align).
  A single implementation can handle any type that fits the same shape
  (e.g. any slice `S$T` as `u_S$raw`).

- **How it works:**
  - **Unified meta types:**
    `u_V$raw` (value), `u_P$raw` / `u_P_const$raw` (pointer),
    `u_S$raw` / `u_S_const$raw` (slice), `u_A$raw` (array), `u_O$raw` (optional),
    `u_E$raw` (error result).
    Each stores the same underlying pointer/slice/layout as the typed form plus `TypeInfo`,
    so there is no extra indirection.
  - **Conversion:**
    Typed → meta: `u_retV$(_T)` etc. from a type;
    `u_anyP(_p)`, `u_anyS(_s)`, `u_anyV(_v)`, `u_anyA(_a)`, `u_anyO(_o)`, `u_anyE(_e)` from a value.
    Meta → typed: `u_castP$(_T)(meta)`, `u_castV$(_T)(meta)`, `u_castS$(_T)(meta)`,
    `u_castA$(_N,_T)(meta)`, `u_castO$(_OT)(meta)`, `u_castE$(_ET)(meta)`.
  - **Generic operations:**
    Slicing/indexing use `TypeInfo` for stride and length
    (`u_atS`, `u_sliceP`, `u_sliceS`, `u_prefixP`, `u_suffixS`, …).
    Memory and comparison use size/align and optional custom eql/ord
    (`u_memcpy`, `u_memeql`, `u_memord`, `u_eql`, `u_ord`, `u_lt`, …;
    `u_*By` and `u_*Ctx` for custom comparators).
    `TypeInfo.align` is log2-encoded; use `mem_log2ToAlign()` when you need byte alignment.

- **Zero-cost:**
  No vtables or runtime dispatch; only `TypeInfo` and inlined logic.
  With LTO and constant folding, meta-based code optimizes like hand-written typed code.
  In release builds, assertions become compiler optimization hints.
  **Anonymous type compatibility:** when layout (size, align, offsets) matches,
  anonymous types interoperate with meta without a typedef.

- **Allocation, type safety, and value semantics:**

  **1. Dynamic allocation**

  Stack allocation is built in: `u_allocV(_type)`, `u_make(_type)`, `u_create(_type)`
  allocate a single value with `alloca`;
  `u_allocA(_len, _type)` allocates an array of values on the stack.
  All return meta wrappers (e.g. `u_V$raw`, `u_P$raw`, `u_A$raw`)
  whose referenced memory lives in the current stack frame.
  For heap allocation, the meta system does not allocate itself:
  you obtain a pointer (or slice) from your allocator,
  then wrap it with the meta layer using `u_init$S`, `u_anyP`, `u_anyS`, etc.
  So stack vs heap is determined by where the underlying pointer comes from;
  the meta layer only attaches `TypeInfo` and the same operations apply.

  **2. Type safety**

  The meta system is **not fully type-safe** in the static sense:
  you can cast to a wrong type with `u_cast*` and misuse memory.
  It provides **weak type safety** in that it
  **preserves the memory range and boundaries of a type**:
  `TypeInfo` (size, align) is carried with every pointer and slice,
  and operations (copy, compare, index, slice) use that information
  so that accesses stay within the described size and alignment.
  Unlike `void*`, meta types carry layout information, enabling bounds-checked operations.
  The responsibility to use the correct target type when casting remains with the caller.

  **3. Values vs pointers/slices; avoiding dangling pointers; lifetime**

  Elements allocated on the stack (via `u_allocV`, `u_make`, etc.) are exposed as pointers
  in the meta representation (e.g. `u_V$raw` holds a pointer `inner` plus `TypeInfo`).
  To prevent dangling pointers and keep lifetime clear across stack frames,

  **meta values are treated as copy semantics**.

  - **Values are copied.**
    When a value is passed to another function,
    the *content* referred to by the meta value is copied:
    the callee receives (or constructs) a meta value that refers to **its own** memory—
    e.g. the callee uses `u_load(_v)` to get a new `u_V$raw`
    whose `inner` points to a buffer in the callee's frame
    (or the caller allocates and copies before the call).
    So although the meta representation is "pointer + type",
    each logical copy has its own memory.
    That gives **immutability between original and copies**:
    the caller's and callee's storage are independent.

    When the callee returns a value,
    the referenced memory is copied back to the caller's side for use
    (e.g. into the caller's stack or a buffer the caller owns),
    so the caller again has its own copy.
    The **lifetime of the referenced pointer is therefore clear**:
    the callee's meta value refers to storage that lives in the callee's scope
    (or storage the callee owns); the caller's refers to the caller's.
    No shared reference to the other frame's stack.

  - **Why meta-value types when we have pointer and slice types?**
    Pointers (`u_P$raw`) and slices (`u_S$raw`) are **by-reference**:
    they share the same underlying memory.
    They are used when the lifetime of the referent is known to outlive the use
    (e.g. caller-owned buffer, or heap).
    The **value type** (`u_V$raw`) exists to express **by-value** semantics:
    "this is a value that, when passed across boundaries, is copied."
    So for passing data across stack frames or to callees
    that should not alias the caller's storage,
    you use the value type and the copy convention
    (e.g. `u_load`, or copy-in/copy-out at call boundaries).
    That way you avoid dangling references and keep ownership and lifetime explicit.

    **Reference vs copy:**
    `u_deref(p)` gives reference access to the pointee without copying;
    `u_load(v)` creates an independent stack copy for safe cross-boundary passing.
    For indirection patterns (e.g. storing indices or handles with context-based comparison),
    stack usage is bounded by the handle size, not the referenced data.

---

**2. Meta module** (`dh/meta.h`)

**Record/type reflection** built on top of the meta type system.
The module includes `prl.h` (and thus gets `u_P$raw`, `u_S$raw`, etc.)
and provides layout and field access **in terms of meta pointers and slices**.

- **Record layout from fields:**
  Given a slice of field `TypeInfo`s, it computes record `TypeInfo`, size, alignment,
  and field offsets: `u_typeInfoRecord`, `u_sizeOfRecord`, `u_alignOfRecord`,
  `u_offsetTo`, `u_offsets`.
  Array-shaped records: `u_typeInfoA`, `u_sizeOfA`, `u_alignOfA`;
  N-replicated records: `u_typeInfoRecordN`, `u_offsetToN`, `u_offsetsN`.
- **Field access as meta:**
  `u_fieldPtr` / `u_fieldPtrMut` take a **meta pointer** to a record
  (`u_P_const$raw` / `u_P$raw`) and a slice of field `TypeInfo`s,
  and return a meta pointer to the chosen field.
  `u_fieldPtrs` / `u_fieldPtrsMut` fill a slice of meta pointers for all fields.
  `u_recordPtr` / `u_recordPtrMut` go from a field meta pointer back to the record meta pointer.
  So the meta module is the layer that lets you describe structs by their field types
  and then read/write fields through the same type-erased `u_*` representation
  that the meta type system uses.
- **N-replicated (SoA-style) access:**
  `u_fieldSli`, `u_fieldSliMut`, `u_fieldSlis`, `u_fieldSlisMut` expose a field across N records
  as `u_S_const$raw` / `u_S$raw`;
  `u_recordNPtr` / `u_recordNPtrMut` go from a field slice back to the record meta pointer.

**Relationship:**
The meta **type system** defines the generic representation
(pointer/slice/array/option/result + `TypeInfo`).
The meta **module** uses that representation for reflection:
it computes layout from field `TypeInfo`s
and gives you field and record access as `u_P$raw` / `u_S$raw`,
so generic code can walk records without knowing the concrete struct type.
Both rely on `TypeInfo` from `core`/`type_info.h`
(and compile-time `typeInfo$(_T)` from `builtin`/`core`).

---

## Platform Support

| Category         | Support Range                                                                          |
| ---------------- | -------------------------------------------------------------------------------------- |
| **OS**           | Windows, Unix, Linux, macOS                                                            |
| **Architecture** | x86 (32-bit), x64 (64-bit)                                                             |
| **Clang**        | 19.1.0+ (Recommended) / 16.0.0+ (Supported) / 9.0.0+ (Minimum, Requires -std=gnu11)    |
| **GCC**          | 15.1.0+ (Recommended) / 13.1.0+ (Supported) / N/A (TBU) (Minimum, Requires -std=gnu11) |
| **MSVC**         | Planned (TBD)                                                                          |

---

## Code Samples

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

In addition to traditional OS threads, state-machine-based coroutines are provided
for ultra-lightweight asynchronous processing.

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

<details>
<summary><strong>More Code Samples</strong></summary>

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
by leveraging compile-time type information (`typeInfo$`) and reflection.
See [Meta System](#meta-system) for more details.

> *TODO: document*
<!-- TODO: document -->

### Data Structures & Algorithms

Designs all data structures and functions to be dynamically allocated,
accepting allocators or memory buffers to fully control memory layout.

> *TODO: document*
<!-- TODO: document -->

</details>

### Testing

```c
#include <dh/main.h>
#include <dh/TEST.h>

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

---

## Documentation

> **Note:** Comprehensive documentation is a work in progress. For now, please refer to:
> - The code samples in this README
> - Header files in `dh/include/dh/` which contain detailed comments and API declarations
> - Example projects in the repository

---

## Naming

<!--
The project is currently in the early stages of development, and API specifications are experimental.
 -->

The name **"dasae-headers"** originates from the project's roots as a header-only library
designed to collect frequently used C utility code.

Due to continuous evolution and functional expansion, it has now adopted a structure
that includes dedicated build tools and source files,
moving beyond the scope of a simple "header-only" library.
We maintain structural flexibility to enhance the user experience
and allow for further high-level optimization.

Consequently, the current name does not yet fully fix the final identity of the project.
If you have suggestions for a unique name that better reflects the philosophy
dasae-headers aims to achieve, please let us know :D

---

## Contribution and Contact

We welcome issues, feature requests, and pull requests!
Please refer to the [Contributing Guide](./dh/docs/en/contributing.md) for more details.

- **Author:** Gyeongtae Kim (dev-dasae)
- **Email:** [codingpelican@gmail.com](mailto:codingpelican@gmail.com)

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

Copyright © 2024-2026 Gyeongtae Kim.
