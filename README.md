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

<!-- markdownlint-disable MD060 -->

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
      <img src="https://img.shields.io/badge/readme-ko%20deprecated-lightgrey?style=flat-square" alt="Korean README deprecated">
    </a>
  </div>

  <div style="margin-top: 8px;">
    <a href="https://en.wikipedia.org/wiki/C17_(C_standard_revision)">
      <img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17">
    </a>
    <a href="./dh/include/dh/builtin/cfg/plat.h">
      <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square"
           alt="Platform: Windows | Linux | macOS">
    </a>
    <a href="./LICENSE">
      <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT">
    </a>
  </div>
</div>

<!-- markdownlint-disable MD007 -->
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
      - [Step 5: Build a File or Project](#step-5-build-a-file-or-project)
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
      - [`atom` — Atomic Operations](#atom--atomic-operations)
      - [`simd` — SIMD Vector Operations](#simd--simd-vector-operations)
      - [`clsr` — Typed Closure Invocation](#clsr--typed-closure-invocation)
      - [`cmp` — Comparison Utilities](#cmp--comparison-utilities)
      - [`m-math` — Mathematical Functions](#m-math--mathematical-functions)
      - [`mem` — Memory Utilities](#mem--memory-utilities)
      - [`u-meta` — Runtime Record/Type Reflection](#u-meta--runtime-recordtype-reflection)
      - [`Rand` — Random Number Generation](#rand--random-number-generation)
      - [`hash` — Hash Utilities](#hash--hash-utilities)
      - [`search` — Searching Algorithms](#search--searching-algorithms)
      - [`sort` — Sorting Algorithms](#sort--sorting-algorithms)
      - [Linked Lists](#linked-lists)
      - [Tree Structures *(planned)*](#tree-structures-planned)
      - [Bit Sets *(planned)*](#bit-sets-planned)
      - [Array-Based Containers](#array-based-containers)
      - [Hash-Based Containers](#hash-based-containers)
      - [`heap` — Heap Allocators](#heap--heap-allocators)
      - [`thrd` — Threading](#thrd--threading)
      - [`co` — Stackful Fiber Contexts](#co--stackful-fiber-contexts)
      - [`Future` / `Sched` / `exec` — Execution](#future--sched--exec--execution)
      - [`conc` — Experimental Concurrency](#conc--experimental-concurrency)
      - [`ascii` — ASCII character utilities](#ascii--ascii-character-utilities)
      - [`utf8` — UTF-8 encoding/decoding](#utf8--utf-8-encodingdecoding)
      - [`utf16` — UTF-16 encoding/decoding](#utf16--utf-16-encodingdecoding)
      - [`wtf8` — WTF-8 (UTF-8 superset for Windows)](#wtf8--wtf-8-utf-8-superset-for-windows)
      - [`wtf16` — WTF-16 encoding](#wtf16--wtf-16-encoding)
      - [`unicode` — Unicode conversion hub](#unicode--unicode-conversion-hub)
      - [`sys` — System and Platform APIs](#sys--system-and-platform-apis)
      - [`proc` — Process Management](#proc--process-management)
      - [`time` — Time \& Duration](#time--time--duration)
      - [`io` — Input/Output](#io--inputoutput)
      - [`fmt` — Formatting](#fmt--formatting)
      - [`Ver` / `SemVer` — Version Values](#ver--semver--version-values)
      - [`log` — Logging](#log--logging)
      - [`fs` — File System](#fs--file-system)
      - [`net` — Networking](#net--networking)
      - [`http` — HTTP *(planned)*](#http--http-planned)
      - [Planned Module Bundles](#planned-module-bundles)
      - [`TEST` — Testing Framework](#test--testing-framework)
      - [`start` / `main` — Entry Points](#start--main--entry-points)
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
<!-- markdownlint-enable MD007 -->

---

## 🚀 Getting Started

### 💽 Installation

> **Note:** `dh-c` supports Clang- and GCC-compatible compiler drivers. Clang
> remains the recommended toolchain and is required for optional `clang-tidy`
> and `clang-format` commands.

#### Prerequisites

- **Clang or GCC-compatible C compiler**
- **Make** (GNU Make or compatible)
- **POSIX shell and core utilities**
- **llvm-ar** (or set `DH_C_AR` / `AR` to another archiver)
- **LLD** on Windows

#### Step 1: Clone the Repository

```sh
git clone https://github.com/coding-pelican/dasae-headers
cd dasae-headers
```

#### Step 2: Build the `dh-c` Tool

```sh
cd dh-c
sh gen-makefile.sh
make PROFILE=release
```

This compiles the `dh-c` build tool to `dh-c/build/release/dh-c`
(or `dh-c/build/release/dh-c.exe` on Windows).

#### Step 3: Set Up Environment Variables

Add the selected profile directory to `PATH`. `DH_HOME` is optional when
working inside this repository because `dh-c` discovers a current or ancestor
`dh` directory before consulting the environment.

| Variable  | Description                                   | Example Value                               |
| --------- | --------------------------------------------- | ------------------------------------------- |
| `PATH`    | Add the profile directory containing `dh-c`   | `/path/to/dasae-headers/dh-c/build/release` |
| `DH_HOME` | Fallback path to the `dh` installation        | `/path/to/dasae-headers/dh`                 |

**Linux/macOS (bash/zsh):**

```sh
# Add to ~/.bashrc or ~/.zshrc
export DH_HOME="/path/to/dasae-headers/dh"
export PATH="/path/to/dasae-headers/dh-c/build/release:$PATH"
```

Then reload your shell configuration:

```sh
source ~/.bashrc  # or source ~/.zshrc
```

**Windows (MSYS2/MinGW):**

```sh
# Add to ~/.bashrc
export DH_HOME="/c/path/to/dasae-headers/dh"
export PATH="/c/path/to/dasae-headers/dh-c/build/release:$PATH"
```

**Windows (PowerShell):**

```powershell
# Add to your PowerShell profile or set as system environment variables
$env:DH_HOME = "C:\path\to\dasae-headers\dh"
$env:PATH = "C:\path\to\dasae-headers\dh-c\build\release;$env:PATH"
```

#### Step 4: Verify Installation

```sh
dh-c --version
dh-c --help
```

#### Step 5: Build a File or Project

Create a minimal project safely:

```bash
dh-c project app
cd app
dh-c build
```

For multiple projects sharing defaults and cache scope:

```bash
dh-c workspace my-workspace
cd my-workspace
dh-c project app
```

The scaffold commands refuse to overwrite existing configuration files.
Direct-source builds still require no project file.

The command line and selected source files are first-class build input:

```sh
dh-c build main.c
dh-c build main.c util.c
```

If `main.dh` or `util.dh` exists beside a selected source, `dh-c` loads it as a
strict source companion. In a projectless build, the first source is the unit
owner: its companion may declare dependencies and `dh-c update main.c` creates
`main.lock.dh` beside it. Secondary companions remain flat. A named project may
instead add a root `project.dh`; a `workspace.dh` may provide shared defaults and
the preferred workspace cache.

```sh
dh-c help files
dh-c help project-dh
dh-c help dh-file
dh-c help dependencies
dh-c help lock-dh
```

See [BUILD.md](./BUILD.md) and
[`dh-c/docs/dh-files.md`](./dh-c/docs/dh-files.md).

#### Hello, world!

```c
#include "dh-main.h"
#include "dh/io/self.h"
#include "dh/fs/File/self.h"
#include "dh/io/Writer.h"

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let out = fs_File_writer(io_handleStdOut());
    let_(msg, O$S_const$u8) = some(u8_l("world"));
    try_(io_Writer_print(out, u8_l("Hello, {:?s}!\n"), msg));
    return_ok({});
} $unscoped(fn);
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

| Aspect                      | Traditional C (Standard C)                           | dasae-headers                                                                       |
| --------------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------------------- |
| **Variables and Functions** | Explicit type declarations and repetitive signatures | `let` (constant), `var` (mutable) inference and `fn_` function syntax               |
| **Closures**                | Compiler-specific closure extensions                 | Typed function and stackless-coroutine adapters through `Clsr`                     |
| **Platform Support**        | Fragmented branching with `#ifdef`                   | Unified detection in `builtin/cfg` and system contracts under `sys`                |
| **Preprocessor Branching**  | Separate `#ifdef` definitions even for simple values | One definition with expression-style branching via `pp_if_` and `pp_switch_`       |

#### 2. Memory & Argument Patterns

Avoids unsafe patterns of returning success as `bool` and receiving result values
via `out` parameters.
`Optional` and `Error Result` enforce validation of absent values or error conditions
at the type system level.

| Aspect               | Traditional C (Standard C)      | dasae-headers                                       |
| -------------------- | ------------------------------- | --------------------------------------------------- |
| **Resource Release** | `goto cleanup` or manual cleanup | Automatic scope cleanup with `defer_` and `errdefer_` |
| **Result Return**    | Error code plus `out` pointer   | Direct **Optional** (`O$`) or **Error Result** (`E$`) return |
| **Result Branching** | Manual `if (err)` branches      | Result control with `orelse_`, `unwrap_`, `try_`, and `catch_` |
| **Data Transfer**    | Separate pointer and length     | **Slice** (`S$`) or value **array** (`A$`) transfer |
| **Strings/Arrays**   | Sentinel or implicit length     | Explicit lengths with checked indexing and slicing helpers |

#### 3. Concurrency

Provides OS threads, typed channels, stackless `Co` coroutine frames, scheduler
tasks, and fibers. `Clsr` gives functions and coroutine frames a shared typed
invocation surface. The separate `conc` module remains experimental.

| Aspect              | Traditional C (Standard C)     | dasae-headers                                                   |
| ------------------- | ------------------------------ | --------------------------------------------------------------- |
| **Async Model**     | OS-thread-centric design       | Threads, stackless coroutines, schedulers, and fibers           |
| **Sync Primitives** | Primitive `mutex` and `cond`   | `Mtx`, `RWLock`, `ResetEvt`, `Latch`, `Group`, and channels     |
| **Control Flow**    | Callbacks or manual state machines | `co_fn_`, `suspend_`, `resume_`, and `co_return_`            |

#### 4. Meta Type & Generic System

Escapes macro hell and validates type safety at compile time through the meta type system.
Provides a differentiated layer that statically guarantees logical compatibility
between anonymous user types not allowed in the C standard.
See [Meta System](#meta-system) for how the meta type system and `u-meta`
(record reflection) work and relate.

| Aspect              | Traditional C (Standard C)                                           | dasae-headers                                                                                     |
| ------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| **Implementation**  | `void*` casting (type information loss)                              | High-level abstraction based on **Meta System** (`u_*`)                                           |
| **Anonymous Types** | Assignment impossible even with identical structure due to anonymity | Compatibility guaranteed when field memory layout and logical structure (Size/Align/Offset) match |
| **API Exposure**    | Data structure API implementation directly embedded in macros        | **Macro-less API**: Adheres to standard function definition format                                |

#### 5. Numerical & Safety

Abstracts hardware architecture-specific SIMD instructions and statically detects
arithmetic overflow and inappropriate type casting.
Safety validations prioritize compile-time evaluation and remain available as
assertions during development. Release builds optimize them away, preserving
the same runtime cost as the underlying primitive operations.

| Aspect                | Traditional C (Standard C)                                                | dasae-headers                                                                             |
| --------------------- | ------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| **Vector Ops**        | Manual loop-based operations or platform-dependent APIs                   | Architecture-independent accelerated operations via `simd.h`                              |
| **Arithmetic Safety** | Unchecked overflow and division edge cases                                | Compile-time validation, development assertions, and zero-overhead release assumptions    |
| **Type Casting**      | Risk of data loss due to implicit conversion                              | Checked signed/unsigned, width, and integer/floating-point conversions                     |

#### 6. Ecosystem & Infrastructure

Manages project-local build and test workflows with built-in tools
and a `project.dh` contract.
When errors occur, preserves call stack information beyond simple return values
to immediately pinpoint the cause.

| Aspect                         | Traditional C (Standard C)        | dasae-headers                                                            |
| ------------------------------ | --------------------------------- | ------------------------------------------------------------------------ |
| **Data Structures/Algorithms** | Manual or external implementations | Containers and algorithms such as `ArrList`, `ArrPDeq`, and `HashMap`    |
| **Memory Control**             | Fixed global allocation patterns | Allocator or caller-buffer injection where supported                     |
| **Testing/Analysis**           | External framework integration   | Built-in `TEST.h` and optional `ETrace` call-stack tracking              |

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
(type-erased values plus `u-meta` record layout and their relationship).

**Structural Anonymous Type Compatibility Validation:**

Unlike other libraries that require type aliasing via `typedef` to allow generics,
effectively accommodates anonymous user types.
Statically validates safety at compile time when field layout (Size, Align, Offset) matches,
ensuring safe interaction between anonymous types.

#### 3. Zero-overhead Safety

**Compile-time Evaluation Priority for Operations and Casting Validation:**

Statically detects arithmetic `Overflow`, `DivisionByZero`, `NaN`, and invalid
type conversions. Safety validations prioritize compile-time evaluation and
act as assertions for dynamic inputs during development. In release builds,
they become optimization assumptions, preserving zero runtime overhead while
making failures faster and easier to diagnose during development.

</details>

---

## Key Features

This project was developed with inspiration from the syntax structures
and standard library designs of Zig and Rust.

- **Memory Safety:**
  Custom allocators, optional allocation tracing, checked indexing and slicing,
  layout-aware meta operations, and `defer_`/`errdefer_`-based automatic
  resource management
- **Enhanced Type System:**
  Compile-time validation, meta type system, algebraic data types (Variant), optional types
- **Explicit Error Handling:**
  `ok`/`err` values, `try_`/`catch_` patterns, optional `ETrace` call-stack tracing
- **Modern Syntax:**
  Type inference (`let`/`var`), function definition (`fn_`), typed closure adapters
  (`Clsr`), and coroutine frames (`Co`)
- **Development Tools:**
  Built-in testing framework, Clang-centered build tooling,
  and multi-platform environment support

---

## Modules Reference

> Legend: Items marked with *(planned)* are under development and not yet publicly available.

<!-- markdownlint-disable MD001 -->

<details>
<summary><strong>Core Language Extensions</strong></summary>

#### `builtin` — Compiler & Platform Abstraction

The dependency-free compiler boundary used by every higher layer: preprocessor
metaprogramming, build/target detection, declaration and expression syntax,
compiler attributes, source locations, and primitive target operations.

- **Submodules:**
  `pp`, `cfg` (`ver`, `lang`, `comp`, `eval`, `arch`, `plat`), `auto`, `comp`,
  `type_info`, `container_info`, `asm`, `wasm`, `mem`, `prefetch`, `atom`,
  `simd`, `src_loc`, `static_assert`

> **Low-level `raw_*` APIs:** `builtin` exposes raw operations for its own
> low-level implementation boundaries. Source code that uses the `prl` layer or
> any higher layer should avoid calling them directly. Prefer the corresponding
> operations in `core/pri.h` or `mem/common.h`.

- **`pp` (preprocessor):** Implements compile-time metaprogramming as preprocessor macros:
  - **Token comparison:**
    `pp_eql`, `pp_neq`, `pp_and`, `pp_or`, `pp_xor`, `pp_not`,
    `pp_Tok_eql`, `pp_Tok_neq` — boolean and registered-token comparison
  - **Control flow:**
    `pp_if_` / `pp_then_` / `pp_else_` — conditional;
    `pp_switch_` / `pp_case_` / `pp_default_` — dispatch
  - **Optionals:**
    `pp_some`, `pp_none`, `pp_isSome`, `pp_isNone`, `pp_orelse_`
    — preprocessor optional values
  - **Utilities:**
    `pp_begin`, `pp_end`, `pp_nothing`, `pp_ignore`, `pp_strfy`, `pp_cat`
    through `pp_cat5`, `pp_join` through `pp_join5`, `pp_uniqTok`,
    `pp_uniqTokByLine`, `pp_overload`, `pp_foreach`, `pp_foldl`, `pp_foldr`,
    `pp_countArg`, `pp_defer`, and `pp_expand`
  - **Preprocessor tuples:**
    `pp_Tuple_wrap`, `pp_Tuple_unwrap`, `pp_Tuple_len`,
    `pp_Tuple_get1st` through `pp_Tuple_get8th`, `pp_Tuple_pushFront`, and
    `pp_Tuple_pushBack`
- **Build and target configuration (`builtin/cfg`):**
  - `lang_*` reports the C/C++ mode, standard version, strict conformance, and
    GNU or Microsoft extension availability.
  - `comp_*` reports compiler identity and version, hosted/freestanding and
    runtime-library linkage facts, and compiler capabilities.
  - `arch_*` reports architecture/family, word width, byte order, cache-line
    size, spin-loop support, and SIMD capabilities.
  - `plat_*` reports operating system, POSIX ancestry, data model, integer and
    pointer widths, and calling conventions.
  `builtin/cfg/comp.h` owns these compiler facts; `builtin/comp.h` is the
  separate syntax/attribute layer described below.
- **Version configuration (`builtin/cfg/ver.h`):**
  - `ver_self_core_major`, `ver_self_core_minor`, `ver_self_core_patch`,
    `ver_self_core_num`, `ver_self_core_str` — build-provided `dh` version
  - `ver_self_num`, `ver_self_str`, `ver_self_str_with_build` — prerelease and
    build-aware self-version queries
  - `ver_self_label_*` and `ver_self_build_*` expose the prerelease and build
    components used by those complete version queries.
  - `ver_core_calc`, `ver_calc`, `ver_core_strfy`, `ver_strfy`,
    `ver_strfyWithBuild` — packed numeric and semantic-version string utilities
- **Evaluation modes (`builtin/cfg/eval.h`):**
  - `eval_in_analysis`, `eval_in_analysis_active_only`, and
    `eval_in_analysis_full` distinguish static-analysis expansion from
    ordinary compilation; `eval_analysis_mode` and `eval_analysis_is_*`
    identify the selected analysis strategy.
  - `eval_in_comptime` and `eval_in_runtime` distinguish compile-time
    implementation passes from runtime builds.
  - `eval_analysis_only`, `eval_comptime_only`, `eval_runtime_only` and their
    `*_unless` counterparts select syntax for the active mode.
  - `builtin/comp.h` exports the shorter `in_*`, `*_enabled`, `*_only`, and
    `*_unless` aliases used by higher layers.
  - `eval_include_level` and `eval_include_level_is_0` through
    `eval_include_level_is_15` expose include depth to analysis-only
    expansion.
- **Declaration and storage syntax:**
  `auto`, `var`, `let`, `var_`, `let_`, `var_const`, `let_const`,
  `var_const_`, and `let_const_` provide inferred or explicit mutable,
  immutable, and const-qualified declarations.
- **Literal and value-update syntax:**
  - `n_` joins comma-separated numeric fragments into one literal;
    `n$(_T)` applies an explicit type, allowing readable forms such as
    `n_(1, 000, 000)` and `n$(usize)(8)`.
  - `l$`, `l0$`, `from$`, `type$`, `typeO$`, `typeE$`, `asg`, `asg_l`,
    `as$`, `comp_const_`, `move`, and `copy` provide typed literals, explicit
    C casts, representation adaptation, assignment, and value-transfer
    syntax.
  - `cleared` / `cleared$`, `initial` / `initial$`, `$init`, `$field`,
    `$in_field`, and `$asg` build explicit initializers; `with_` copies a
    value, applies designated field updates, and returns the updated value.
- **Type-directed selection:**
  `T_switch$` dispatches with C `_Generic`; `T_case$` matches a type across
  mutable and const-qualified forms, while `T_qual$` preserves an exact
  qualifier match. `T_default_` supplies the fallback and `T_delim()` provides
  an unreachable separator for conditionally generated case lists.
- **Local expression scopes:**
  `local_`, `local_label`, and `local_return_` name compound expression scopes,
  block-local labels, and the value produced by such a scope.
- **Compile-time expression selection:**
  `comp_when_` with `comp_provide_` / `comp_instead_` chooses one expression
  without requiring both branches to be valid.
- **Compiler and assembly syntax:**
  - `$attr`, `$inline`, `$must_check`, `$on_load`, `$branch_likely`, and related
    macros normalize compiler attributes, linkage, calling conventions, data
    layout, branch prediction, lifecycle hooks, and optimization hints.
  - `$static`, `$extern`, `$thrd_local`, `$import`, `$export`, `$packed`,
    `$bits`, `$align`, and the `$callconv_*` family make storage, visibility,
    representation, alignment, and ABI choices explicit.
  - `not`, `and`, `or`, `$fallthrough`, `$unreachable`, `$loop_unroll`,
    `$loop_inline`, and `$loop_rolled` provide readable control and loop hints
    used by the higher-level scope syntax.
  - `$suppress_`, `$suppressing_`, and `$suppressed` scope compiler diagnostic
    suppression; `let_ignore`, `$ignore_void`, `$used`, and `$unused` make
    intentional value handling explicit.
  - `asm`, `asm_volatile`, `$reg`, `asm_var_`, `asm_let_`, `asm_var_reg`, and
    `asm_let_reg` normalize inline assembly and register-bound declarations.
- **Primitive memory and target operations:**
  - `builtin/mem.h` defines bit-unit constants, `raw_countOnes*`,
    `raw_leadingRedundantSgnBits*`, `raw_leadingZeros*`,
    `raw_trailingZeros*`, `raw_firstSetBit*`, `raw_parity*`,
    `raw_swapBytes*`, `raw_rotateLeft*`, `raw_rotateRight*`,
    `raw_reverseBits*`, `raw_memset*`, `raw_memcpy`, `raw_memmove`,
    `raw_memeql`, `raw_memord`, `raw_alloca`, and `raw_allocaAlign`.
  - `builtin/atom.h` defines `atom_MemOrd`, fences, loads/stores,
    compare-exchange, exchange, arithmetic min/max, and bitwise atomic
    primitives. `dh/atom.h` adds spin locks and typed `atom_V$` wrappers.
  - `builtin/simd.h` exposes `simd_supported`, width/alignment, and per-target
    capability flags while selecting the matching intrinsic header. Portable
    vector operations live in `prl/simd.h`.
  - `prefetch`, `prefetch_read`, and `prefetch_write` express
    `prefetch_RW` read/write requests with `prefetch_Locality`.
  - `builtin/wasm.h` exposes WebAssembly capability flags and feature-gated
    memory, numeric, TLS, exception, atomic, reference, and table builtins.
- **Source location and static assertions:**
  `src_loc_filePath`, `src_loc_fileName`, `src_loc_fnName`, `src_loc_line`,
  and `src_loc_column` expose compiler locations; `static_assert`,
  `static_assert_msg`, and their trap forms provide portable compile-time
  assertions. `core/src_loc.h` combines the fields as `srcLoc()`.
- **Compile-time type introspection:**
  `nameOf`, `isComptimeExpr`, `isRuntimeExpr`, `TypeOf`, `TypeOfUnqual`,
  `null`, `null$`, `$ref`, `$deref`, `raw_ref`, `raw_deref`, `RefType$`,
  `RefTypeUnqual$`, `DerefType$`, `DerefTypeUnqual$`, `eqlType$`,
  `eqlTypeUnqual$`, `neqType$`, `neqTypeUnqual$`, `sizeOf$`, `countOf$`,
  `alignOf$`, `alignOfLog2$`, `$alignAs`, and `$alignAsLog2`
- **Container/field introspection:**
  `memberName$`, `fieldName$`, `FieldType$`, `FieldTypeUnqual$`,
  `eqlFieldType$`, `offsetTo`, `fieldPtr`, `recordPtr`, and `fieldPadding$`

#### `core` — Language Primitives & Syntax Extensions

Primitives, function syntax, assertions, debugging, scoped resource management,
and safe arithmetic.

- **Submodules:**
  `pri`, `fn`, `claim`, `debug`, `range`, `op`, `cmp`, `pipe`, `chain`,
  `scope`, `src_loc`, `type_info`
- **Primitive types:**
  `bool`, `i8`..`i64`, `u8`..`u64`, `isize`, `usize`, `f32`, `f64`
- **Enumeration syntax (`core/pri/enum.h`):**
  `enum_` defines named enum types and values; `$fits($packed)` or
  `$fits($bits(8|16|32|64))` controls the generated representation.
  `enum_of$((_EnumType)(_value))` constructs an explicitly typed enum value.
- **Assertions:**
  `claim_assert`, `claim_unreachable`; `debug_assert`, `debug_only`
- **Function and scope syntax:**
  `fn_`, `$scope`, `$guard`, `return_`, `defer_`, and `errdefer_` provide typed
  functions, expression-aware returns, and deterministic scope cleanup.
- **Scoped initialization and control:**
  `using_` and `using_fini_` bind initialization to a nested statement;
  `if_` / `else_` and `while_` add initializer syntax; `when_` with
  `provide_` / `instead_` provides expression-form conditional selection.
- **Iteration and loop control (`core/scope/common.h`):**
  - `for_` traverses ranges, arrays, slices, and untyped containers, binding up
    to eight sources in lockstep. `$a`, `$s`, `$ua`, and `$us` tag container
    sources; `$fwd`, `$bwd`, `$rev`, `$asc`, and `$desc` select direction.
  - `loop_unroll_`, `loop_inline_`, and `loop_rolled_` apply compiler loop
    transformation hints to `do`, `while`, `for`, or `for_`.
  - `loop_labeled`, `loop_continue_`, and `loop_break_` provide explicit
    continue and break targets for nested loops.
- **Index range syntax (`core/range.h`):**
  - `R` is a validated half-open `usize` range `[begin, end)`.
  - `$r`, `$rf`, and `$rt` construct bounded, from-index, and to-index ranges;
    `$incl` and `$excl` adjust explicit endpoint bounds.
  - `R_contains`, `R_len`, `R_at`, `R_slice`, `R_prefix`, and `R_suffix`
    provide checked range operations used by `for_` and slice traversal.
- **Pipeline syntax (`core/pipe.h`):**
  `pipe_` evaluates an initial expression once, binds each step's capture to
  the preceding result, and returns the final expression.
- **Functional chaining (`core/chain.h`):**
  `chain$` composes slice operations without materializing intermediate
  containers: `each_`, `filter_`, `map$`, `fold_`, `reduce_`, `all_`, `any_`.
- **Debug layer:**
  - `debug_enabled`, `debug_only`, `debug_unless`, `debug_break`, and
    `$debug_point` control debug-only behavior and debugger breakpoints.
  - `debug_srcLoc()`, `$traced`, `$trace`, `$tracing`, `debug_typeInfo$`,
    `$typing`, and `$typed` retain source and type metadata only in debug builds.
  - `debug_StackTrace_setupCrashHandler` installs platform crash handling;
    `debug_StackTrace_print` emits the current symbolized stack trace.
- **Comparison contracts:**
  - **Mathematical sign (`cmp_Sgn`):**
    `cmp_Sgn_neg` (−1), `cmp_Sgn_zero` (0), and `cmp_Sgn_pos` (1) represent
    negative, zero, and positive signs independently of any ordered type.
  - **Mathematical ordering (`cmp_Ord`):**
    `cmp_Ord_lt` (−1), `cmp_Ord_eq` (0), `cmp_Ord_gt` (1).
    It represents the mathematical sign of comparing `lhs` with `rhs`:
    strictly less, equal, or strictly greater. Where subtraction is meaningful,
    this is the sign of `lhs - rhs`, not the subtraction result itself.
    All order-derived predicates are defined in terms of
    `cmp_ord$(_T)(lhs, rhs)`.
    A consistent ordering satisfies `ord(x, x) == cmp_Ord_eq`,
    `ord(x, y) == cmp_Ord_inv(ord(y, x))`, and transitivity of its strict
    less-than relation.
  - **Ordering domain:**
    The three states directly represent a total order or strict-weak-order
    equivalence classes. A partial order can use the same contract only when
    incomparable pairs are intentionally represented by `cmp_Ord_eq`, because
    `cmp_Ord` has no separate unordered state.
  - **Equality-only contract (`eql` / `neq`):**
    `cmp_eql$(_T)` defines an equivalence relation without requiring an order;
    `cmp_neq$(_T)` is its negation. The expected equality laws are reflexivity,
    symmetry, and transitivity. Implement either operation and derive the other
    with `cmp_fn_neq_default$` or `cmp_fn_eql_default$`.
  - **Order-derived equality (`eq` / `ne`):**
    `cmp_eq$(_T)` means `cmp_ord$(_T)(lhs, rhs) == cmp_Ord_eq`;
    `cmp_ne$(_T)` means the result is not `cmp_Ord_eq`.
  - **Defining ordering from `ord`:**
    Implement `cmp_ord$(_T)(lhs, rhs)` and derive `cmp_eq$`, `cmp_ne$`,
    `cmp_lt$`, `cmp_gt$`, `cmp_le$`, and `cmp_ge$` through the
    `cmp_fn_*_default$` macros.
  - **Defining ordering from `lt`:**
    Implement `cmp_lt$(_T)(lhs, rhs)` and use `cmp_fn_ord_default$(_T)` to
    derive three-way ordering:
    `lt(lhs, rhs) ? cmp_Ord_lt : lt(rhs, lhs) ? cmp_Ord_gt : cmp_Ord_eq`.
    Its remaining predicates can then be derived from that ordering.
  - **Runtime default direction (`cmp.h`):**
    The higher-layer `cmp_OrdFn_defaultAsc` orders smaller values first;
    `cmp_OrdFn_defaultDesc` reverses that direction.
  - **Context and approximate variants:**
    The same derivation model extends to `eqlCtx` / `ordCtx` / `ltCtx`,
    `ordApx` / `ltApx`, and the absolute/relative forms `ordApxAbs` /
    `ordApxRel`.
  - **Equality distinction:**
    `eql` / `neq` and `eq` / `ne` coincide only when the order is total and
    consistent with equality. Use the primitive equality interface for types
    that define equivalence without order; use order-derived equality when an
    ordering contract is present. Content equality for a slice is one case
    where an equality contract can exist without defining an ordering.
  - **Predicates on `cmp_Ord`:**
    `cmp_Ord_isEq`, `cmp_Ord_isNe`, `cmp_Ord_isLt`, `cmp_Ord_isGt`,
    `cmp_Ord_isLe`, `cmp_Ord_isGe`, `cmp_Ord_inv`
- **Core primitives (arithmetic, bitwise, comparison):**
  `pri_add`, `pri_sub`, `pri_mul`, `pri_div`, `pri_rem`, `pri_neg`, `pri_abs`,
  `pri_sgn`, `pri_not`, `pri_shl`, `pri_shr`, `pri_and`, `pri_xor`, `pri_or`,
  `pri_eql`, `pri_neq`, `pri_ord`, `pri_min`, `pri_max`, `pri_clamp`, `bitCast$`
- **Cast hierarchy:**
  - **Raw language cast:** `as$` expresses an unchecked C cast explicitly.
  - **Representation and alignment:** `bitCast$` reinterprets equal-sized
    object representations; `alignCast` asserts a pointer's required alignment.
  - **Boolean and numeric:** `intToBool` / `boolToInt`, `intCast$`,
    `intToFlt$` / `fltFromInt$`, `fltToInt$` / `intFromFlt$`, and `fltCast$`
    distinguish boolean, integer, integer-to-float, float-to-integer, and
    float-width conversions. `boolFromInt` and `intFromBool` are directional
    aliases for the boolean conversions.
  - **Enumeration:** `intToEnum$` / `enumFromInt$` and
    `enumToInt$` / `intFromEnum$` make integer-enum conversions explicit.
  - **Pointer and address:** `ptrToInt` / `intFromPtr`,
    `intToPtr$` / `ptrFromInt$`, `ptrCast$`,
    `ptrAlignCast$`, `ptrQualCast$`, and `ptrAlignQualCast$` distinguish
    address conversion, pointee type, alignment, and qualifier changes.
  - **Mutability:** `mutCast` adds const qualification and `constCast` removes
    it explicitly.
- **Core primitives (memory):**
  `pri_memset0`, `pri_memset`, `pri_memcpy`, `pri_memmove`, `pri_memeql`,
  `pri_memord`, and their slice forms
- **Safe integer ops (overflow-checked, in `core/pri.h`):**
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

Prelude type construction and syntax: Optional, Error Result, Slice, Array,
Tuple, typed Range and Limit, Variant, typed variadics, and compile-time-capable
hashes. (Safe arithmetic lives in `core/pri.h`.)

- **Submodules:**
  `raw`, `tpl`, `u-meta`, `base`, `simd`, `L-Limit`, `R-Range`, `Co`, `Clsr`,
  `va`, `CompHash`, `E-Err`, `ETrace`, `common`, `int`, `flt`
- **Key Types:**
  - `O$(T)` (Optional) — `some(v)`, `none()`, `if_some((opt)(capture))`, `orelse_((opt)(default))`, `unwrap_(opt)`
  - `E$(T)` (Error Result) — `ok(v)`, `err(e)`, `try_(expr)`, `catch_((expr)(err, block))`, `return_ok`, `return_err`
  - `S$(T)` (Slice) — `S_deref$`, `S_at`, `S_slice`, `S_prefix`, `S_suffix`, `S_len`
  - `A$(N, T)` (Array) — `A_zero`, `A_init$`, `A_from$`, `A_ref$`, `A_len`
  - `Tup$(_Ts...)` (Tuple) — positional heterogeneous values `$0` through `$15`
  - `L$(_T)` / `R$(_T)` — inclusive/exclusive typed limits and typed ranges
  - `variant_` — Tagged union; create with `union_of$`, match with `match_`, `patt_`
  - `simd_V$(N, T)` — Primitive vector type and lane-wise `simd_V_*` operations
  - `Co` — Stackless coroutine frame representation and control primitives
  - `Clsr` — Typed common representation for function and coroutine closures
  - `ETrace` — Optional error tracing with call stack information
- **Type application layer (`prl/tpl.h`):**
  `T_alias$`, `T_decl$`, `T_impl$`, `T_use$`, `T_declBy$`, `T_implBy$`, and
  `T_useBy$` declare, implement, or enable template families for concrete types;
  `$spec` and `$via` customize an application path.
- **Tuple and destructuring syntax (`prl/tpl/Tup.h`):**
  `Tup$`, `Tup$$`, and `T_use_Tup$` define heterogeneous tuples; `tie_` and
  `tie$` initialize positional fields; `$tup` infers them; `untie_` destructures
  by position; `bind_` destructures record fields, with `$skip` for omissions.
- **Tagged variant syntax (`prl/tpl/variant.h`):**
  - `variant_` defines a tag and its corresponding payload union.
    `$fits(...)` controls an internally generated tag representation;
    `$maps(_Tag)` maps payload cases onto an existing tag type and verifies
    that each case uses the corresponding external tag value.
  - `union_of` / `union_of$` and `union_with$` construct cases; `matches`,
    `union_as`, and `union_to` inspect and extract payloads.
  - `match_`, `case_`, `cases_`, `patt_`, `patts_`, and `default_` provide
    statement matching, payload capture, and grouped cases; `matchedEnum()` and
    `union_matched` expose or rebuild the active case inside a match.
- **Typed limit and range syntax:**
  `incl_` / `excl_` and `L_incl$` / `L_excl$` construct typed endpoints.
  `range_` / `range$` combine them into `R$(_T)`, preserving endpoint
  inclusivity for integer and floating-point domains.
- **Typed variadic syntax (`prl/va.h`):**
  `$va_args` declares a per-argument compile-time mask and borrowed `u_Tup`
  view; `va_` materializes up to 16 heterogeneous arguments once, associates
  their `TypeInfo` fields, and invokes the receiver. The tuple view is valid
  only until that receiver returns.
- **Compile-time-capable string hashes (`prl/CompHash.h`):**
  `compHash`, `compHash32`, and `compHash64` hash literals; `CompHash_from`,
  `CompHash_calc`, and their 32/64-bit variants handle slices and explicit
  result widths.
- **Literal layers (`prl/base.h`):**
  `T_a$`, `T_s$`, `T_l$`, `NT_a$`, `NT_s$`, and `NT_l$` construct typed arrays
  and slices; `u8_a`, `u8_s`, `u8_l` and `*Z0` variants distinguish counted
  byte strings from null-terminated storage.
- **Prelude cast adapters:**
  `P_mutCast` / `mutCastP`, `P_constCast` / `constCastP`,
  `mutCastS` / `S_mutCast`, `constCastS$` / `constCastS` /
  `S_constCast$` / `S_constCast`, `simd_V_intCast$` /
  `simd_V_intToFlt$` / `simd_V_fltToInt$` / `simd_V_fltCast$`,
  `u_cast$` / `u_castP$` / `u_castV$` / `u_castS$` / `u_castA$` /
  `u_castO$` / `u_castE$`, and `union_cast$` adapt pointer, slice, SIMD,
  meta-value, optional/result, and variant representations without conflating
  their contracts.
- **SIMD primitive operations:**
  - **Construction:**
    `simd_V_init$`, `simd_V_splat$`, `simd_V_from$`, `simd_V_fromA$`,
    `simd_V_toA$`, `simd_V_cat`
  - **Arithmetic:**
    `simd_V_add`, `simd_V_sub`, `simd_V_mul`, `simd_V_div`, `simd_V_neg`,
    `simd_V_abs`, `simd_V_mulAdd`
  - **Comparison:**
    `simd_V_eq`, `simd_V_ne`, `simd_V_lt`, `simd_V_le`, `simd_V_gt`,
    `simd_V_ge`
  - **Min, max, and reduction:**
    `simd_V_min`, `simd_V_max`, `simd_V_clamp`, `simd_V_reduce`,
    `simd_V_findMin`, `simd_V_findMax`
  - **Vector and boolean reduction:**
    `simd_V_dot`, `simd_V_cross3`, `simd_V_bool_any`, `simd_V_bool_all`,
    `simd_V_bool_none`, `simd_V_bool_bitMask`
  - **Lane composition:**
    `simd_V_shuffle`, `simd_V_select`
  - **Integer specialization:**
    `simd_V_int_add`, `simd_V_int_addWrap`, `simd_V_int_addSat`,
    `simd_V_int_div*`, `simd_V_int_mod*`, `simd_V_int_setBit`,
    `simd_V_int_resetBit`, `simd_V_int_toggleBit`, `simd_V_int_hasBit`
  - **Floating-point specialization:**
    `simd_V_flt_add`, `simd_V_flt_div*`, `simd_V_flt_min`,
    `simd_V_flt_max`, `simd_V_flt_sqrt`, `simd_V_flt_floor`,
    `simd_V_flt_ceil`, `simd_V_flt_round`, `simd_V_flt_trunc`
- **Stackless coroutine and closure primitives:**
  - `co_fn_`, `co_fn_frame_scope`, `co_fn_scope`, `suspend_`, `resume_`,
    `co_return_` define and drive state-machine-based stackless coroutine frames.
  - `fn_use_Clsr_`, `co_use_Clsr_`, and `clsr_` adapt functions and coroutine
    frames to the common typed `Clsr` representation.
  - `Clsr_Kind_fn` and `Clsr_Kind_co` distinguish the represented routine kind.
- **prl/int, prl/flt:**
  Per-type safe wrappers (e.g. `u8_add`, `u32_div`, `i64_mod`) with debug overflow checks;
  see `core/pri.h` for generic `int_add`, `intCast$`, etc.
- **Zero-cost meta type system** (`prl/u-meta` and `prl/tpl`):
  Type-erased generic layer over PRL types so algorithms can work on values
  without knowing the concrete type at compile time.
  How it works and how it relates to the `u-meta` record-layout module
  are described in [Meta System](#meta-system).

</details>

<details>
<summary><strong>Foundation Libraries</strong></summary>

#### `atom` — Atomic Operations

C11 atomics and type-safe atomic value wrappers (prefix `atom_`).

- **Ordering:**
  `atom_MemOrd` — `unordered`, `monotonic`, `acquire`, `release`, `acq_rel`, `seq_cst`
- **Operations:**
  `atom_fence`, `atom_load`, `atom_store`, `atom_fetchXchg`,
  `atom_cmpXchgWeak$`, `atom_cmpXchgStrong$`,
  `atom_pri_fetchAdd`, `atom_pri_fetchSub`, `atom_int_fetchAnd`, `atom_int_fetchOr`,
  `atom_int_fetchXor`, `atom_int_fetchNand`
- **Atomic value wrapper:**
  `atom_V$(_T)`, `atom_V_zero$`, `atom_V_init$`, `atom_V_from`
  — generic atomic variable type

#### `simd` — SIMD Vector Operations

Architecture-independent vector operations built on compiler vector support.
`prl/simd.h` supplies the primitive `simd_V$` types and lane-wise operations;
this module adds higher-level lane, search, and memory operations.

- **Lane sequence:**
  `simd_shiftElemsLeft`, `simd_shiftElemsRight`, `simd_rotateElemsLeft`,
  `simd_rotateElemsRight`, `simd_reverseOrder`
- **Search:**
  `simd_findFirst`, `simd_findLast`, `simd_count`
- **Memory:**
  `simd_gather$`, `simd_scatter`, `simd_loadMask`, `simd_storeMask`

#### `clsr` — Typed Closure Invocation

Typed operations over the common `Clsr$(_T)` representation supplied by
`prl/Clsr.h`.

- `T_use_clsr_kind$(_T)` provides typed inspection such as `clsr_kind$i32`.
- `T_use_clsr_invokeToStep$(_T)` provides step-wise invocation such as
  `clsr_invokeToStep$i32`.
- `T_use_clsr_invokeToComplete$(_T)` provides invocation to completion such as
  `clsr_invokeToComplete$i32`.

#### `cmp` — Comparison Utilities

Runtime comparison over meta values, pointers, and slices. The primitive
`cmp_Ord` type and compile-time type contracts belong to `core/cmp.h`; this
module supplies runtime comparator selection and type-erased dispatch.

- **Equality:**
  `cmp_EqlFn_default`, `cmp_eql`, `cmp_eqlP`, `cmp_eqlS`, and `cmp_neq*`
- **Ordering:**
  `cmp_OrdFn_defaultAsc`, `cmp_OrdFn_defaultDesc`, `cmp_ord`, `cmp_ordP`,
  `cmp_ordS`, and the derived `cmp_eq*`, `cmp_lt*`, `cmp_ge*` families
  - `cmp_OrdFn_defaultAsc` places smaller values first.
  - `cmp_OrdFn_defaultDesc` reverses that ordering.
- **Context variants:**
  `cmp_EqlCtxFn_default`, `cmp_OrdCtxFn_default`, `cmp_eqlCtx*`, `cmp_ordCtx*`

#### `m-math` — Mathematical Functions

- **Current modules:** `m-math`, `m-math-linalg`
- **Planned modules:** `m-math-geom`, `m-math-interp`, `m-math-ease`
  currently expose no public implementation contract.
- **Common (prefix `math_`):**
  `math_abs`, `math_min`, `math_max`, `math_clamp`, `math_sign`, `math_wrap`,
  `math_floor`, `math_ceil`, `math_round`, `math_trunc`, `math_sqrt`, `math_pow`, `math_rsqrt`,
  `math_sin`, `math_cos`, `math_tan`, `math_asin`, `math_acos`, `math_atan`, `math_atan2`;
  constants e.g. `math_pi`, `math_f32_pi`, `math_limit_min$`, `math_limit_max$`.
- **Vectors (types `m_V2f32`, `m_V3f32`, `m_V4f32`, and f64/i32/i64 variants; prefix `m_V*`):**
  `m_V*_of`, `m_V*_splat`, `m_V*_add`, `m_V*_sub`, `m_V*_mul`, `m_V*_div`, `m_V*_dot`, `m_V*_cross`,
  `m_V*_norm`, `m_V*_len`, `m_V*_lenSq`, `m_V*_dist`, `m_V*_min`, `m_V*_max`, `m_V*_clamp`, `m_V*_rotate`
- **Matrices (types `m_M2f32`, `m_M3f32`, `m_M4f32`; prefix `m_M*`):**
  `m_M*_identity`, `m_M*_ofCols`, `m_M*_ofRows`, `m_M*_mulM`, `m_M*_mulV`, `m_M*_transpose`,
  `m_M*_det`, `m_M*_inv`, `m_M*_rotate`, `m_M*_scale`, `m_M*_scaleUniform`
- **Quaternions (type `m_Q4f32`; prefix `m_Q4f32_`):**
  `m_Q4f32_identity`, `m_Q4f32_of`, `m_Q4f32_mul`, `m_Q4f32_mulQ`, `m_Q4f32_conj`, `m_Q4f32_invQ`,
  `m_Q4f32_norm`, `m_Q4f32_slerp`, `m_Q4f32_fromAxisAngle`, `m_Q4f32_fromM3`/`fromM4`, `m_Q4f32_toM3`/`toM4`

#### `mem` — Memory Utilities

Typed memory representation, sequence algorithms, growth helpers, and
allocator contracts. Compiler primitives remain in `builtin/mem.h`;
`core/pri.h` and `prl/u-meta.h` build the `pri_mem*` and `u_mem*` layers, and
`mem` adds checked byte, pointer, meta-type, and generated typed surfaces.

- **Submodules:**
  `cfg`, `base`, `common`, `dyn`, `seq`, `ring`, `Alctr`, `AlcTrace`
- **Sizes, bits, and byte order:**
  - `mem_byte_size`, `mem_kb_size`, `mem_kib_size`, `mem_mb_size`,
    `mem_mib_size`, `mem_gb_size`, `mem_gib_size`, and `mem_page_size`
  - `mem_trailingZeros*`, `mem_leadingZeros*`, and `mem_swapBytes*` for
    `8`, `16`, `32`, `Long`, `64`, and `Size` forms where applicable
  - `mem_littleToNative*`, `mem_bigToNative*`, `mem_nativeToLittle*`, and
    `mem_nativeToBig*`
  - `mem_readLE*`, `mem_readBE*`, `mem_writeLE*`, and `mem_writeBE*` convert
    fixed-width byte arrays without unaligned typed loads
- **Alignment and empty storage:**
  `mem_Align` stores log2 alignment; `mem_isValidAlign`, `mem_isAligned`,
  `mem_isAlignedLog2`, `mem_alignFwd`, `mem_alignBwd`, their `Log2` forms,
  `mem_alignToLog2`, `mem_log2ToAlign`, and `mem_emptyAddr` validate and
  construct aligned addresses.
- **Representation conversion:**
  `mem_idxZ$u8`, `mem_lenZ0$u8`, `mem_spanZ0$u8`, and
  `mem_spanZ0Mut$u8` bridge sentinel-terminated bytes and counted slices.
  `mem_asBytes*` views objects and slices as bytes; `mem_bytesAs*` validates a
  byte span as typed storage; `mem_Bytes$`, `mem_toBytes`, and `mem_bytesTo`
  copy value representations through caller-provided storage.
- **Bulk operations:**
  - `mem_set0*`, `mem_set*`, `mem_copy*`, and `mem_move*`
  - `mem_eql*` / `mem_neq*` for representation equality and `mem_ord*` with
    `mem_eq*`, `mem_ne*`, `mem_lt*`, `mem_gt*`, `mem_le*`, `mem_ge*` for
    bytewise ordering
  - `mem_swap*`, `mem_reverse*`, and `mem_rotate*`
  Each family has byte, object-pointer, meta-slice, and generated typed forms
  where that shape is meaningful.
- **Sequence queries and views:**
  `mem_findFirst*`, `mem_findLast*`, `mem_count*`, `mem_contains*`,
  `mem_startsWith*`, and `mem_endsWith*` return indices, counts, or
  predicates. `mem_trim*` returns non-owning subviews; `mem_cut*` returns an
  optional `mem_Cutted` pair exposing `before` and `after` views.
- **Sequence composition:**
  `mem_cat*`, `mem_join*`, `mem_padLeft*`, `mem_padRight*`, and
  `mem_padCenter*` provide checked caller-buffer, `*Within`, and
  allocator-backed `*Alloc` forms.
- **Iterators:**
  - `mem_window` creates `mem_WindowIter` values that traverse fixed-size,
    explicitly advanced windows.
  - `mem_TokzIter` / `mem_TokzBwdIter` tokenize in either direction with
    `mem_tokz*` factories and `mem_Delim` unit, sequence, or any-of
    delimiters.
  - `mem_SplitIter` / `mem_SplitBwdIter` preserve split boundaries in either
    direction through the corresponding `mem_split*` factories.
- **Container support:**
  `mem_dyn_initCapForSize`, `mem_dyn_initCap`, `mem_dyn_growCapFrom`,
  `mem_dyn_growCap`, and `mem_dyn_addOrOOM` define dynamic capacity growth;
  `mem_seq_set0Range`, `mem_seq_copyRange`, `mem_seq_moveRange`, and
  `mem_seq_swapAt` operate on validated element ranges; `mem_ring_idx` and
  `mem_ring_idxFrom` map logical ring positions.
- **Allocator interface:**
  `mem_Alctr` separates raw allocation, in-place resize, relocating remap, and
  free operations, then provides typed create/destroy/clone and
  alloc/resize/remap/realloc/free/duplicate families. Fallible allocation
  returns `mem_E` with `OutOfMemory`.
- **Allocation tracing:**
  When `mem_tracing_enabled`, `mem_AlcTrace` records allocation, resize,
  remap, and free source locations through `mem_AlcTrace_register*`.
  `mem_AlcTrace_initWithLogPath` and `mem_AlcTrace_finiWithGenerateReport`
  manage the trace and its final leak/invalid-free report.

#### `u-meta` — Runtime Record/Type Reflection

Record layout and field access from `TypeInfo`, operating on the **meta type system**
(`u_P$raw`, `u_S$raw`).
The type-erased value layer lives in `prl/u-meta.h`; record and field layout
utilities live in `dh/u-meta.h`.
For the relationship between these layers,
see [Meta System](#meta-system).
Compile-time type queries such as `sizeOf$` and `alignOf$` live in `builtin`;
the runtime `TypeInfo` value and `typeInfo$` constructor live in `core`.

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

#### `hash` — Hash Utilities

Non-cryptographic hash functions used by the hash containers and available for
direct use, including `hash_fnv1a32` and `hash_wyhash`.

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
| **`ListSgl`** | Singly linked list | `ListSgl_empty`, `ListSgl_prepend`, `ListSgl_remove`, `ListSgl_shift` |
| **`ListDbl`** | Doubly linked list | `ListDbl_empty`, `ListDbl_prepend`, `ListDbl_append`, `ListDbl_remove`, `ListDbl_shift`, `ListDbl_pop` |

#### Tree Structures *(planned)*

| Module         | Description                          | Key Functions |
| -------------- | ------------------------------------ | ------------- |
| **`BTree`**    | B-tree *(planned)*                   | —             |
| **`BTreeMap`** | B-tree based ordered map *(planned)* | —             |
| **`BTreeSet`** | B-tree based ordered set *(planned)* | —             |
| **`SegTree`**  | Segment tree *(planned)*             | —             |
| **`Treap`**    | Randomized search tree *(planned)*   | —             |

#### Bit Sets *(planned)*

`BitSet` is included by `dh.h`, but its public header currently contains only
the planned-module marker and exposes no contract.

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

`HashMap` and `HashSet` use the comparison and hashing contracts documented in
[`hash` — Hash Utilities](#hash--hash-utilities).

| Module           | Description                                             | Key Functions                                                                      |
| ---------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| **`HashMap`**    | Hash map with open addressing                           | `init`, `fini`, `put`, `by`, `ptrBy`, `for`, `entry`, `contains`, `remove`, `iter` |
| **`HashSet`**    | Hash set with open addressing                           | `init`, `fini`, `put`, `ensure`, `contains`, `remove`, `iter`                      |
| **`HashMapSeq`** | Ordered hash map preserving insertion order *(planned)* | —                                                                                  |
| **`HashSetSeq`** | Ordered hash set preserving insertion order *(planned)* | —                                                                                  |

</details>

<details>
<summary><strong>Memory Management</strong></summary>

#### `heap` — Heap Allocators

| Allocator      | Description                                              |
| -------------- | -------------------------------------------------------- |
| **`VMap`**     | Virtual address mapping and mapped-region management     |
| **`VMem`**     | Virtual-memory allocator support                         |
| **`Classic`**  | Traditional heap allocation (C runtime `malloc`/`free`)  |
| **`Page`**     | Page-aligned allocation for OS-mapped memory blocks      |
| **`Sbrk`**     | Sbrk-based allocation for linear memory growth and reuse |
| **`Sys`**      | System-dependent allocation (Page or Sbrk)               |
| **`Fixed`**    | Fixed-size block allocator for bulk operations           |
| **`Arena`**    | Region-based allocation for bulk operations              |
| **`Pool`**     | Pool-based allocation for object reuse                   |
| **`ThrdSafe`** | Thread-safe wrapper for any allocator                    |
| **`Smp`**      | SMP-aware allocation with per-core caching               |

</details>

<details>
<summary><strong>Concurrency</strong></summary>

#### `thrd` — Threading

OS thread management and synchronization primitives.

| Primitive        | Description                          |
| ---------------- | ------------------------------------ |
| **`thrd`**       | Thread creation, spawn, join, detach |
| **`thrd_ftx`**   | Futex wait and wake API              |
| **`Mtx`**        | Mutex                                |
| **`Sem`**        | Semaphore                            |
| **`Cond`**       | Condition variable                   |
| **`RWLock`**     | Read-write lock                      |
| **`ResetEvt`**   | Manual/auto reset event              |
| **`Group`**      | Spawned thread group and latch       |

- **Key Functions:**
  `thrd_spawn`, `thrd_join`, `thrd_detach`, `thrd_currId`, `thrd_yield`
- **Channels and coordination:**
  `SPSC`, `MPSC`, `SPMC`, `MPMC`, `Select`, `Batch`, `Latch`, `Group`
- **Typed closure integration:**
  `T_use_thrd_spawn$(_T)` and `T_use_thrd_join$(_T)` expose typed thread
  boundaries over `Clsr$(_T)` without requiring callers to inspect a concrete
  function or coroutine closure layout.

#### `co` — Stackful Fiber Contexts

`dh/co.h` exposes the `co/Fiber.h` stackful-fiber coroutine layer. The
stackless `Co` frame primitives and typed `Clsr` representation belong to
`prl/Co.h` and `prl/Clsr.h`; higher-level typed closure invocation belongs to
`dh/clsr.h`.

- **Architecture support:**
  `co_Fiber_supported` reports the compile-time availability of the context
  switch implementation for x86_64, AArch64, and RV64 targets.
- **Context construction and switching:**
  `co_Fiber_Context_from` initializes a suspended fiber context and
  `co_Fiber_contextSwitch` saves the active context before restoring another.
- **Stack preparation:**
  `co_Fiber_stackAllocArg` places an aligned entry argument in caller-provided
  stack storage; `co_Fiber_Context_stackPtr` exposes the saved stack pointer.

#### `Future` / `Sched` / `exec` — Execution

- **`Future$(_T)`:**
  Typed result storage with `Future_await$T`, `Future_cancel$T`,
  `Future_result$T`, and `Future_resultMut$T`.
- **`Sched`:**
  Type-erased scheduler contract for `async`, fallible `spawn`, cancellation,
  and cooperative idle points. `Sched_seq`, `Sched_coop`, `Sched_preem`, and
  `Sched_para` adapt concrete executors.
- **`exec`:**
  Concrete execution building blocks: `Task`, `Seq`, `Coop`, `Preem`, `Para`,
  `Lane`, `LaneTimed`, `Timer`, fixed-stack `Fiber`, and growable `Fiber`.

#### `conc` — Experimental Concurrency

`conc` is the scheduler-aware counterpart to the OS-thread synchronization
layer. It currently contains `Waker`, `Waiter`, `Mtx`, `Cond`, `Sem`, `RWLock`,
`Once`, `OnceLock`, `LazeLock`, `OnceEvt`, `ResetEvt`, `Latch`, `Group`,
channels, `Select`, queues, and `Batch`. The module is still in preparation:
these interfaces are experimental rather than stable public contracts.

</details>

<details>
<summary><strong>Text & Encoding</strong></summary>

#### `ascii` — ASCII character utilities

`ascii_isAlpha`, `ascii_isDigit`, `ascii_isSpace`, `ascii_toLower`, `ascii_toUpper`

#### `utf8` — UTF-8 encoding/decoding

`utf8_encode`, `utf8_decode`, `utf8_isValid`, `utf8_validate`, `utf8_count`, `utf8_view`, `utf8_iter`

#### `utf16` — UTF-16 encoding/decoding

`utf16_encode`, `utf16_decode`, `utf16_codepointSeqLen`, `utf16_isHighSurrogate`, `utf16_isLowSurrogate`

#### `wtf8` — WTF-8 (UTF-8 superset for Windows)

`wtf8_encode`, `wtf8_decode`, `wtf8_view`, `wtf8_iter`

#### `wtf16` — WTF-16 encoding

`wtf16_iter`, `wtf16_Iter_next`

#### `unicode` — Unicode conversion hub

`unicode_utf8ToUTF16`, `unicode_utf16ToUTF8`, `unicode_wtf8ToWTF16`, `unicode_wtf16ToWTF8`

</details>

<details>
<summary><strong>System & OS</strong></summary>

#### `sys` — System and Platform APIs

- **Common surfaces:**
  `sys.h`, `sys/posix.h`, `sys/win32.h`, `sys/wasi.h`
- **POSIX compatibility:**
  `sys/posix.h` provides the POSIX-facing contract used by cross-platform
  system code.
- **Windows API layer:**
  `sys/api/windows` contains handle, file, I/O, process, thread, synchronization,
  console, NLS, networking, and other Windows contracts.

#### `proc` — Process Management

Process management utilities for cross-platform code.

#### `time` — Time & Duration

- **Submodules:**
  `cfg`, `common`, `Dur`, `Inst`, `Clock`, `epoch`, `self`
- **Duration:**
  `time_Dur_fromSecs`, `time_Dur_fromMillis`, `time_Dur_fromNanos`,
  `time_Dur_add`, `time_Dur_sub`
- **Clock and instant:**
  `time_Clock_now`, `time_Clock_resolution`, `time_Clock_Inst_elapsed`, and
  `time_Clock_Inst_durSince`
- **Sleep:**
  `time_Clock_sleep` and its seconds, milliseconds, microseconds, and
  nanoseconds variants return `Sched_Cancelable$void`, making cancellation an
  explicit part of the clock contract

</details>

<details>
<summary><strong>I/O & Formatting, Filesystem</strong></summary>

#### `io` — Input/Output

- **Submodules:** `common`, `stream`, `Reader`, `Writer`, `Fixed`, `Buf`
- **Stream:**
  `io_stream_print`, `io_stream_println`, `io_stream_eprint`, `io_stream_eprintln`, `io_stream_nl`
- **Reader:**
  `io_Reader_read`, `io_Reader_readExact`, `io_Reader_readByte`, `io_Reader_skip`
- **Writer:**
  `io_Writer_write`, `io_Writer_writeBytes`, `io_Writer_writeByte`,
  `io_Writer_print`, `io_Writer_println`, `io_Writer_nl`
- **Fixed (in-memory fixed buffer):**
  `io_Fixed_reading`, `io_Fixed_writing`, `io_Fixed_written`, `io_Fixed_reset`,
  `io_Fixed_Reader_init`, `io_Fixed_reader`, `io_Fixed_Writer_init`, `io_Fixed_writer`
- **Buf (buffered Reader/Writer):**
  `io_Buf_Reader_init`, `io_Buf_Reader_fill`, `io_Buf_Reader_peekByte`, `io_Buf_Reader_readUntilByte`,
  `io_Buf_Reader_skipUntilByte`, `io_Buf_Reader_skip`, `io_Buf_reader`,
  `io_Buf_Writer_init`, `io_Buf_Writer_flush`, `io_Buf_writer`

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

#### `Ver` / `SemVer` — Version Values

- **`Ver`:**
  Compact `u32` ecosystem version with 8-bit major/minor/patch fields and a
  packed alpha/beta/rc/release label. `Ver_self`, `Ver_comp`, `Ver_clang`,
  `Ver_gcc`, and `Ver_gnu` expose current build/compiler versions; comparison
  follows the packed total order.
- **`SemVer`:**
  Semantic Versioning 2.0 value and precedence contract with `u64`
  major/minor/patch values plus prerelease and build identifier slices.
  `SemVer` comparison retains build metadata but excludes it from precedence.
  This is currently a value/comparison layer, not a text parser or formatter.

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

#### `net` — Networking

Networking utilities for cross-platform code.

#### `http` — HTTP *(planned)*

`http` is included by `dh.h`, but its public header currently contains only
the planned-module marker and exposes no client/server contract.

</details>

<!-- markdownlint-disable-next-line MD033 -->
<details>
<!-- markdownlint-disable-next-line MD033 -->
<summary><strong>Planned Modules</strong></summary>

#### Planned Module Bundles

These remaining names are direct `dh.h` bundles, but do not yet expose a
current public implementation contract. Their top-level headers contain only
a planned marker, or only include subheaders that do.

- **Foundation:** `crypt` (`tls`, `ssl`), `cmprs`
- **System:** `c`, `DynLib`
- **Text and data formats:** `regex`, `csv`, `ini`, `yaml`, `toml`, `json`
- **Text/binary codecs:** `base32`, `base64`
- **Binary and executable formats:**
  `ar`, `coff`, `pe`, `pdb`, `elf`, `dwarf`, `macho`, `pie`, `wasm`
- **Archives:** `zip`, `rar`, `tar`
- **Data and networking:** `db`, `ws`

</details>

<details>
<summary><strong>Testing & Entry Point</strong></summary>

#### `TEST` — Testing Framework

- **Macros:**
  `TEST_fn_`, `TEST_expect`, `TEST_expectMsg`
- **Usage:**
  Define tests with `TEST_fn_`, run with `dh-c test`

#### `start` / `main` — Entry Points

- `start.h` provides process startup and exit support.
- `dh-main.h` provides the error-result program entry surface and is included
  directly when needed.
- **Main form:**
  `fn_((main(S$S_const$u8 args))(E$void) $scope)`
  — Standard entry point with argument parsing and error handling

</details>

---

<!-- markdownlint-enable MD001 -->

## Meta System

The meta system has two related parts: the **meta type system** (type-erased generics)
and the **record-layout module** (record/type reflection).
Both use the same `u_` prefix and share `TypeInfo` from `core`/`type_info.h`.

---

**1. Meta type system** (`prl/u-meta.h`, included through `prl.h`)

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
    Meta → typed: `u_castP$((_T)(meta))`, `u_castV$((_T)(meta))`, `u_castS$((_T)(meta))`,
    `u_castA$((_N,_T)(meta))`, `u_castO$((_OT)(meta))`, `u_castE$((_ET)(meta))`.
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

**2. Record-layout module** (`dh/u-meta.h`)

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
  So the record-layout module lets you describe structs by their field types
  and then read/write fields through the same type-erased `u_*` representation
  that the meta type system uses.
- **N-replicated (SoA-style) access:**
  `u_fieldSli`, `u_fieldSliMut`, `u_fieldSlis`, `u_fieldSlisMut` expose a field across N records
  as `u_S_const$raw` / `u_S$raw`;
  `u_recordNPtr` / `u_recordNPtrMut` go from a field slice back to the record meta pointer.

**Relationship:**
The meta **type system** defines the generic representation
(pointer/slice/array/option/result + `TypeInfo`).
The record-layout **module** uses that representation for reflection:
it computes layout from field `TypeInfo`s
and gives you field and record access as `u_P$raw` / `u_S$raw`,
so generic code can walk records without knowing the concrete struct type.
Both rely on `TypeInfo` from `core`/`type_info.h`
(and compile-time `typeInfo$(_T)` from `builtin`/`core`).

---

## Platform Support

| Category         | Support Range                                                               |
| ---------------- | --------------------------------------------------------------------------- |
| **OS**           | Windows, Linux, Darwin/macOS, and WASI; maturity varies by target           |
| **Architecture** | x86, x86_64, AArch64, ARM, RISC-V, and WebAssembly detection               |
| **Clang**        | Primary and recommended compiler                                            |
| **GCC**          | GCC-compatible paths where selected features permit                        |
| **MSVC**         | Header detection only; not a supported `dh-c` compiler                     |

> **Note:** See [Prerequisites](#prerequisites) for host requirements. Use
> `dh-c doctor`, `dh-c toolchain all`, and `dh-c plan` to inspect effective
> support for a concrete host, target, and project.

---

## Code Samples

### Optional Values

```c
fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_(($s(items), $rf(0))(item, index)) {
        if (*item == value) return_some(intCast$((i32)(index)));
    } $end(for);
    return_none();
} $unscoped(fn);

$static fn_((example(void))(void) $scope) {
    var_(nums, A$$(5, i32)) = A_init({ 10, 20, 30, 40, 50 });
    let found = findValueIndex(30, A_ref$((S$i32)(nums)).as_const);

    if_some((found)(index)) {
        io_stream_println(u8_l("Found at: {:d}"), index);
    } else_none {
        io_stream_println(u8_l("Not found"));
    }

    let value_or_default = orelse_((found)(-1));
    let value_assumed = unwrap_(found);
    claim_assert(value_or_default == value_assumed);
} $unscoped(fn);
```

### Error Results & Defer

```c
errset_((my_math_E)(
    my_math_DivisionByZero,
    my_math_Overflow,
    my_math_Underflow
));

T_use_E$($set(my_math_E)(i32));
$attr($must_check)
$static fn_((safeDivide(i32 lhs, i32 rhs))(my_math_E$i32) $scope) {
    if (rhs == 0) return_err(E_cause$my_math_DivisionByZero());
    return_ok(lhs / rhs);
} $unscoped(fn);

$attr($must_check)
$static fn_((example(mem_Alctr gpa))(E$void) $guard) {
    // Allocate resources
    var buffer = try_(mem_Alctr_alloc$i32($trace gpa, 100));
    defer_(mem_Alctr_free$i32($trace gpa, buffer));

    // Only executed when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_l("Occurred error!: {:e}"), err));

    // Error propagation (try_) and handling (catch_)
    let divided = try_(safeDivide(10, 2));
    let divided_handled = catch_((safeDivide(10, 0))($ignore, 1));
    let_ignore = divided;
    let_ignore = divided_handled;

    return_ok({});
} $unguarded(fn);
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
    patt_((InputEvent_press_key)(on_pressed)) {
        debug_assert_true_fmt(
            -1 < on_pressed->key && on_pressed->key <= 255,
            "key is out of range"
        );
        break;
    } $end(patt);
    patt_((InputEvent_release_button)(on_released)) {
        debug_assert_true_fmt(
            -1 < on_released->button && on_released->button <= 5,
            "button is out of range"
        );
        break;
    } $end(patt);
    fallback_(claim_unreachable);
} $end(match);
} $unscoped(fn);
```

### Chaining - Filter, Map, Fold, Reduce

`fold_` can accumulate into scalar or container state. For example, an
allocator-backed `ArrList` can be initialized before the chain, appended to
inside the fold with `try_`, and released with `defer_`; `reduce_` instead
returns `O$(_T)` because an empty input has no initial accumulator.

```c
$static fn_((example(void))(void) $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let items = A_ref(data);

    let even_square_sum = chain$((i32)(items)(
        filter_((item)(*item % 2 == 0)),
        map$((i32)(item)(*item * *item)),
        fold_((0), (acc, item)(acc + *item))
    ));

    let minimum = chain$((O$u32)(items)(
        filter_((item)(*item > 0)),
        map$((u32)(item)(as$(u32)(*item))),
        reduce_((acc, item)(pri_min(acc, *item)))
    ));

    claim_assert(even_square_sum == 20);
    claim_assert(unwrap_(minimum) == 1);
} $unscoped(fn);
```

### Threads vs Stackless-Coroutines

Threads run typed closures. A closure can wrap a normal function or a coroutine
frame, so `thrd_spawn` can execute either shape through the same typed closure
contract. `thrd_join$i32` returns the common typed `Clsr$i32*` surface, so the
result is read from that returned closure instead of reopening the original
function-specific or coroutine-specific storage.

```c
$static fn_((timesTwo(i32 input))(i32)) {
    return input * 2;
};
fn_use_Clsr_((timesTwo)(i32)(i32));

$static co_fn_(sumAfterSuspend, (i32 lhs; i32 rhs), i32);
co_fn_frame_scope(
    sumAfterSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({ var_(idle, Void); })
);
co_fn_scope(sumAfterSuspend) {
    suspend_((idle)(Void_()));
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((sumAfterSuspend)(i32, i32)(i32));

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use_thrd_spawn$(i32);
T_use_thrd_join$(i32);

fn_((example(void))(E$void) $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let spawn_cfg = (thrd_SpawnCfg){
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };

    var function_clsr = clsr_((timesTwo)(21));
    let function_thread = try_(thrd_spawn$i32(spawn_cfg, function_clsr.as_base));
    let function_joined = thrd_join$i32(function_thread);

    var coroutine_clsr = clsr_((sumAfterSuspend)(19, 23));
    let coroutine_thread = try_(thrd_spawn$i32(spawn_cfg, coroutine_clsr.as_base));
    let coroutine_joined = thrd_join$i32(coroutine_thread);

    io_stream_println(u8_l("function: {:d}"), function_joined->ctx.ret);
    io_stream_println(u8_l("coroutine: {:d}"), coroutine_joined->ctx.ret);
    return_ok({});
} $unguarded(fn);
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
#include "dh-TEST-main.h"
#include "dh/TEST.h"

// Define functions to test
$static fn_((mathAdd(i32 a, i32 b))(i32)) { return a + b; };
$static fn_((mathMul(i32 a, i32 b))(i32)) { return a * b; };

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

    // TEST_expect returns an error value, so expected failures can be inspected
    let should_fail = expr_(TEST_E$void $guard)({
        ETrace_disable();
        defer_(ETrace_enable());
        $break_(TEST_expect(product == 30));
    }) $unguarded(expr);
    try_(TEST_expect(isErr(should_fail)));
} $unscoped(TEST_fn);
```

---

## Documentation

Public project documents:

| Document | Purpose |
| --- | --- |
| [`README.md`](./README.md) | Project overview and entry point. |
| [`BUILD.md`](./BUILD.md) | Public build guide for direct-source, workspace, project, dependency, and prebuilt flows. |
| [`dh-c/docs/dh-files.md`](./dh-c/docs/dh-files.md) | Canonical authored and generated `.dh` file contracts. |
| [`dh-c/docs/project-dh-contract.md`](./dh-c/docs/project-dh-contract.md) | Named project and target-root contract. |
| [`dh-c/docs/external-dependencies.md`](./dh-c/docs/external-dependencies.md) | Dependency providers, locks, fetch, and update behavior. |
| [`dh-c/docs/external-tools.md`](./dh-c/docs/external-tools.md) | Helper tool resolution and environment overrides. |
| [`dh-c/docs/artifact-manifest.md`](./dh-c/docs/artifact-manifest.md) | Generated library artifact inventory. |
| [`dh-c/docs/prebuilt-packages.md`](./dh-c/docs/prebuilt-packages.md) | Prebuilt package layout and consumption. |
| [`LICENSE`](./LICENSE) | MIT license. |

For API details, use the public headers under `dh/include/dh/` and the checked
examples and tests in this repository.

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

- **Author:** Gyeongtae Kim (dev-dasae)
- **Email:** [codingpelican@gmail.com](mailto:codingpelican@gmail.com)

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

Copyright © 2024-2026 Gyeongtae Kim.
