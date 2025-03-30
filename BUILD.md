
# DH-C Build Tool

A simple build tool for DH-C projects that handles compilation, testing, and running of code. The tool supports both project and single-file compilation while integrating with the DH.

## Features

- Detects clang toolchain (falls back to gcc if clang is not available)
- Supports build, test, and run commands
- Handles debug/release/optimize modes
- Supports both project and single-file compilation
- Automatically finds and compiles source files
- Automatically adds include paths
- Auto-detects and includes DH library

## Project Structure

For project builds, the tool expects a standard DH-C project structure:

```
(root) <project-name>/
├── libs/                 # Optional dependencies
│   └── link.dhc          # Optional list of libraries to link
├── include/              # Public headers
│   ├── <project-name>.h
│   └── <project-name>/   # Project-specific headers
│       ├── common.h
│       └── ...
└── src/                  # Source files
    ├── common.c
    └── ...
```

For single file compilation, simply specify the .c file in the command.

## Building the Tool

Use clang to build the tool directly:

```bash
clang -std=c17 -Wall -Wextra -funsigned-char -fblocks -o dh-c dh-c.c
```

Or with gcc:

```bash
gcc -std=c17 -Wall -Wextra -funsigned-char -fblocks -o dh-c dh-c.c
```

On Windows you might need to add `-lws2_32` for socket libraries, and on Unix-like systems you might need `-lpthread` for threading.

## Usage

```
dh-c [build|test|run] [debug|release|optimize] [file.c] [options]
```

### Commands

- `build` - Build the project or file
- `test` - Build and run tests
- `run` - Build and run the project or file

### Build Modes

- `debug` - Debug build with no optimization
- `release` - Release build with -O2 optimization
- `optimize` - Optimized build (default: -O3)

### Options

- `--compiler=<clang|gcc>` - Specify compiler (default: clang)
- `--std=<c99|c11|c17>` - Specify C standard (default: c17)
- `--opt=<O0|O1|O2|O3|Os>` - Specify optimization level for optimize mode
- `--args="args"` - Arguments to pass when running
- `--dh=<path>` - Path to DH library (auto-detected by default)

## Examples

### Project Builds

```bash
# Build project in debug mode
dh-c build debug

# Build and run tests in release mode
dh-c test release

# Build and run in optimize mode with custom optimization level
dh-c run optimize --opt=O2
```

### Single File Compilation

```bash
# Build a single file in debug mode
dh-c build debug sample.c

# Build and run a single file
dh-c run sample.c

# Build and test a single file with custom arguments
dh-c test sample.c --args="--verbose"
```

### Specifying DH Library Location

If the tool can't automatically find the DH library, you can specify its location:

```bash
dh-c build sample.c --dh=/path/to/dh
```

## Output Files

For projects, the build tool creates executables in the `build` directory:
- `<project-name>` for release builds
- `<project-name>-debug` for debug builds
- `<project-name>-opt` for optimized builds

For single files, the executables are created in the same directory as the source file:
- `<file-name>` for release builds
- `<file-name>-debug` for debug builds
- `<file-name>-opt` for optimized builds
