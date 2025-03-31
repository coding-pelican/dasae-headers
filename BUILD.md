# DH-C Build Tool

A lightweight, cross-platform build tool for C projects using the DH library system.

## Features

- Cargo-like project management (workspace and project commands)
- Cross-platform support (Windows, macOS, Linux)
- Auto-detection of the DH library
- Support for debug, release, and optimized builds
- Automatic include/source file discovery
- IDE integration through `.clangd` and VSCode tasks

## Installation

### Windows

1. Clone or download this repository
2. Open a PowerShell prompt as Administrator
3. Navigate to the directory containing the `dh-c.c` file
4. Run the installation script:

   ```powershell
   .\install-dhc.ps1
   ```

5. Restart your terminal or PowerShell session

### Linux/macOS

1. Clone or download this repository
2. Open a terminal
3. Navigate to the directory containing the `dh-c.c` file
4. Make the installation script executable and run it:

   ```bash
   chmod +x install-dhc.sh
   ./install-dhc.sh
   ```

5. Follow the on-screen instructions to complete the installation

## Usage

### Project Management

Create a new workspace:

```sh
dh-c workspace my-workspace
```

Create a new project:

```sh
dh-c project my-project
```

Initialize the current directory as a workspace or project:

```sh
dh-c workspace .
dh-c project .
```

### Building and Running

Build a single file:

```sh
dh-c build file.c
```

Build a file with debug symbols:

```sh
dh-c build debug file.c
```

Build and run a file:

```sh
dh-c run file.c
```

Build and run tests:

```sh
dh-c test test-file.c
```

Build an entire project:

```sh
dh-c build
```

### Additional Options

Specify compiler:

```sh
dh-c build file.c --compiler=gcc
```

Use a specific C standard:

```sh
dh-c build file.c --std=c11
```

Set optimization level:

```sh
dh-c build optimize file.c --opt=O2
```

Pass arguments to the program:

```sh
dh-c run file.c --args="arg1 arg2"
```

## Environment Variables

- `DH_HOME`: Points to the DH library root directory. Set automatically by the installation scripts, but can be manually configured if needed.

## Project Structure

A typical DH-C project follows this structure:

```txt
my-project/
  ├── include/my-project (public headers)
  ├── src/my-project     (implementation files)
  ├── test/              (test files)
  ├── lib/               (third-party libraries)
  ├── .clangd            (language server config)
  └── .vscode/           (IDE config)
```

## Credits

DH-C was created as a lightweight build and project management system for C programming, inspired by Cargo from the Rust ecosystem.
