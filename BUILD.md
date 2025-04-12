# DH-C Build Tool

A lightweight, cross-platform build tool for C projects using the DH-C system.

## Features

- Cargo-like project management (workspace and project commands)
- Cross-platform support (Windows, macOS, Linux)
- Auto-detection of the DH-C system
- Support for multiple build configurations (dev, test, profile, release, performance, embedded, micro)
- Automatic include/source file discovery
- IDE integration through `.clangd` and VSCode tasks
- Option to build without the DH library (`--no-libdh`)

## Installation

### Windows

1. Clone or download this repository
2. Open a PowerShell prompt as Administrator
3. Navigate to the directory containing the `dh-c.c` file
4. Run the installation script:

   ```ps1
   .\install-dh-c.ps1
   ```

5. Restart your terminal or PowerShell session

### Linux/macOS

1. Clone or download this repository
2. Open a terminal
3. Navigate to the directory containing the `dh-c.c` file
4. Make the installation script executable and run it:

   ```sh
   chmod +x install-dh-c.sh
   ./install-dh-c.sh
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

Build with a specific configuration:

```sh
dh-c build dev file.c
```

Build without the DH-C library:

```sh
dh-c build file.c --no-libdh
```

Build an entire project:

```sh
dh-c build
```

Build and run a file:

```sh
dh-c run file.c
```

Build and run tests:

```sh
dh-c test test-file.c
```

Run tests for an entire project:

```sh
dh-c test
```

### Build Configurations

DH-C supports several predefined build configurations:

- `dev` - Extended debug info, no optimization, assertions enabled
- `test` - Standard debug info, basic optimization, assertions enabled
- `profile` - Standard debug info, balanced optimization, assertions enabled
- `release` - Minimal debug info, balanced optimization, assertions disabled
- `performance` - No debug info, aggressive optimization, assertions disabled
- `embedded` - No debug info, size optimization, assertions disabled
- `micro` - No debug info, extreme size optimization, assertions disabled

### Additional Options

Specify compiler:

```sh
dh-c build file.c --compiler=gcc
```

Use a specific C standard:

```sh
dh-c build file.c --std=c11
```

Pass arguments to the program:

```sh
dh-c run file.c --args="arg1 arg2"
```

## VSCode Integration

The DH-C build tool includes a `tasks.json` template that provides VSCode integration with the following tasks:

- `dh>create workspace` - Create a new workspace
- `dh>create project` - Create a new project
- `dh>build current file` - Build the current file (default build task)
- `dh>build project` - Build the entire project
- `dh>test current file` - Run tests for the current file
- `dh>test project` - Run tests for the entire project
- `dh>run current file` - Build and run the current file (default test task)
- `dh>run project` - Build and run the entire project

## Environment Variables

- `DH_HOME`: Points to the DH library root directory. Set automatically by the installation scripts, but can be manually configured if needed.

## Project Structure

A typical DH-C project follows this structure:

```txt
my-project/
  ├── include/my-project (public headers)
  ├── src/my-project     (implementation files)
  ├── lib/               (third-party libraries)
  ├── .clangd            (language server config)
  ├── .clang-format      (code formatter config)
  └── .vscode/           (IDE config)
```
