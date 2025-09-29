---
title: Quick Start Guide
description: Getting started with dasae-headers for safer C programming
created: 2025-04-01
updated: 2025-04-13
version: v0.1.1
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

### Getting the Source

Clone the repository to your local machine:

```sh
git clone https://github.com/coding-pelican/dasae-headers.git
cd dasae-headers
```

### Installing the dh-c Build Tool

The dh-c build tool simplifies project management, building, and testing of dasae-headers projects.

#### Windows

1. Open a PowerShell prompt as Administrator
2. Navigate to the dasae-headers directory
3. Run the installation script:

```ps1
.\install-dh-c.ps1
```

4. Restart your terminal or PowerShell session

#### Linux/macOS

1. Open a terminal
2. Navigate to the dasae-headers directory
3. Make the installation script executable and run it:

```sh
chmod +x install-dh-c.sh
./install-dh-c.sh
```

4. Restart your terminal session or source your profile

## Project Setup

### Creating a New Project

Create a new project using the dh-c tool:

```sh
dh-c project my-project
cd my-project
```

This command creates a standard project structure with all necessary configuration files.

### Directory Structure

The dh-c tool creates the following directory structure:

```txt
my-project/
├── include/          # Project-specific headers
├── src/              # Source files
│   └── main.c        # Main entry point
├── lib/              # Third-party libraries
├── build/            # Build output (created during build)
├── .clangd           # Language server configuration
├── .clang-format     # Code formatter configuration
└── .vscode/          # VS Code configuration
    └── tasks.json    # Build tasks
```

## First Program

Let's create a simple "Hello, world!" program:

1. Open `src/main.c` and replace its contents with:

```c
#include "dh/main.h"
#include "dh/Str.h"

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $scope) {
    // Create a string literal
    let hello = Str_l("Hello, dasae-headers!");

    // Print to console with newline
    Str_println(hello);

    // Return success
    return_ok({});
} $unscoped;
```

2. Build and run the program:

```sh
dh-c build dev    # Build in development mode
dh-c run dev      # Run the program
```

### Key Features in This Example

1. `fn_` - Function declaration
2. `Sli$Sli_const$u8` - Slice of constant strings (command-line arguments)
3. `Err$void` - Error result type with void payload
7. `$scope` - Function with extended scope for return payload
4. `let` - Type inference for variable declaration
5. `Str_l` - String literal creation
6. `return_ok` - Successful return with payload
8. `$unscoped` - End of function's extended scope

## Key Concepts

### Variable Declaration

dasae-headers provides both traditional C-style and modern type inference approaches for variable declaration:

```c
// Basic form (immutable)
const i32 count = 10;
// Type inference (immutable)
let count = 10;  // const i32 inferred
// Type specified (immutable)
let_(count, i32) = 10; // const i32

// Basic form (mutable)
i32 number = 42;
// Type inference (mutable)
var number = 42;  // i32 inferred
// Type specified (mutable)
var_(number, i32) = 42; // i32
```

### Memory Safety with defer

Automatic resource cleanup using `defer`:

```c
fn_(readFile(Sli_const$u8 path), Err$Str $guard) {
    let_(file, FILE*) = fopen(path.ptr, "r");
    if (file == null) {
        return_err(fileError("Could not open file"));
    }

    // This will execute when the function exits
    defer_($ignore = fclose(file));

    // Process file...

    return_ok(fileContents);
} $unguarded;
```

### Error Handling

Explicit error handling with the `try_` pattern:

```c
fn_(processData(void), Err$void $scope) {
    // Call function that may fail and propagate error
    let result = try_(getData());

    // Process result...

    return_ok({});
} $unscoped;
```

### Optional Types

Safe handling of nullable values:

```c
fn_(findUser(i32 id), Opt$User $scope) {
    if (id <= 0) {
        return_none();  // No user found
    }

    User user = getUserById(id);
    return_some(user);  // User found
} $unscoped;

// Usage
if_some(findUser(42), user) {
    // User exists, process user
} else_none {
    // Handle not found case
}
```

## Building and Running

The dh-c tool provides a simple and consistent way to build and run your projects.

### Building

Build your project with one of the predefined configurations:

```sh
dh-c build dev         # Development mode (debug, no optimization)
dh-c build test        # Test mode
dh-c build release     # Release mode (optimized)
dh-c build performance # Maximum performance
dh-c build embedded    # Size optimization
dh-c build micro       # Extreme size optimization
```

### Running

Run your application:

```sh
dh-c run dev           # Build and run in development mode
```

Pass arguments to your program:

```sh
dh-c run dev --args="arg1 arg2"
```

### Testing

Run tests:

```sh
dh-c test              # Run all tests
```

### Additional Options

Show executed commands:

```sh
dh-c build dev --show-commands
```

Enable output suffix for build configurations:

```sh
dh-c build dev --use-output-suffix
```

Get help:

```sh
dh-c --help
```

## Debugging

dasae-headers includes built-in debug features:

### Assertions

```c
debug_assert_true(condition, "Error message");
debug_assert_fmt(0 < count, "Invalid count: %d", count);
```

## IDE Integration

The dh-c tool creates VSCode configuration files for your project:

### VSCode Tasks

Press `Ctrl+Shift+B`(default shortcut) to access build tasks:
- `dh>build project` - Build the project
- `dh>run project` - Build and run the project
- `dh>test project` - Run tests
- `dh>execute project` - Run without rebuilding

## Next Steps

Now that you've set up a basic project, you can explore:

- **[Core Concepts](./concepts/index.md)** - Fundamental principles like memory safety, error handling, and type system
- **[API Reference](./api/index.md)** - Comprehensive documentation for all library modules
- **[Examples](./examples/index.md)** - Practical code samples demonstrating library features
- **[Tutorials](./tutorials/index.md)** - Step-by-step guides for learning specific techniques

Happy coding with dasae-headers!
