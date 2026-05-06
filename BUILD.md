# DH-C Build Tool

A lightweight, cross-platform build tool for C projects using the DH-C system.

## Features

- Cargo-like project management (workspace and project commands)
- Cross-platform support (Windows, macOS, Linux)
- Auto-detection of the DH-C system
- Support for multiple build configurations (dev, test, profile, release, performance, embedded, micro)
- Automatic include/source file discovery
- IDE integration through `.clangd` and VSCode tasks
- Option to build without the DH/DSL library path (`--no-dsl`)
- Configurable output suffix for build configurations
- Test mode with automatic cleanup

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

### Getting Help

Display help information:

```sh
dh-c --help
# or
dh-c -h
```

Display version information:

```sh
dh-c --version
```

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

Build without the DH/DSL library path:

```sh
dh-c build file.c --no-dsl
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

Show commands being executed:

```sh
dh-c build file.c --show-commands
```

Enable output suffix for build configurations:

```sh
dh-c build dev file.c --use-output-suffix
```

## VSCode Integration

The DH-C build tool includes a `tasks.json` template that provides VSCode integration with the following tasks:

- `dh>create workspace` - Create a new workspace
- `dh>create project` - Create a new project
- `dh>build current file` - Build the current file
- `dh>build project` - Build the entire project
- `dh>test current file` - Run tests for the current file
- `dh>test project` - Run tests for the entire project
- `dh>run current file` - Build and run the current file
- `dh>run project` - Build and run the entire project
- `dh>execute current file` - Run the existing executable for the current file without building
- `dh>execute project` - Run the existing executable for the project without building

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

Directory aliases are also supported at the project root:

- `include`, `includes`, `inc`
- `source`, `sources`, `src`
- `tests`, `test`
- `samples`, `sample`
- `examples`, `example`

Only one alias variant per category may exist at the same level.

## `project.dh`

`project.dh` is the reusable build-contract layer on top of the CLI.

It is not the only way to use `dh-c`.

- the CLI must remain able to perform builds directly from explicit paths and flags
- `project.dh` exists to structure, reuse, and automate those CLI-level decisions inside a project

It now covers:

- project-wide defaults such as `output=`, `build-runs-tests=`, `no-dsl=`, and PCH settings
- reusable self-project source roots through repeated `self-root=`
- named target roots through `[target-root <name>]` blocks

Detailed explicit-vs-implicit rules live in `dh-c/docs/project-dh-contract.md`.

Example-first CLI sentence rules and the proposed `exclude` contract live in
`dh-c/docs/cli-syntax-draft.md`.

### Project-wide defaults

`project.dh` can define project defaults such as:

```txt
output=my-project
build-runs-tests=true
no-dsl=false
```

### Reusable self roots

Repeated `self-root=` entries declare reusable self-project code that should be built once and reused internally:

```txt
self-root=src
self-root=pkg
self-root=internal
```

If no `self-root=` is declared, the resolved project `src` root is used implicitly.

### Target roots

Named target roots describe where buildable targets live and what artifact they produce:

```txt
[target-root cmd]
path=cmd
kind=executable
selection=dir
link-self=true

[target-root plugins]
path=plugins
kind=shared-lib
selection=dir
link-self=true
```

Defaults inside a target-root block:

- `kind=executable`
- `selection=path`
- `link-self=true`

### CLI-first behavior

These are first-class CLI features:

- `--sample`
- `--example`
- `--test`

They select the project's `samples`, `examples`, and `tests` target families directly.

When `project.dh` does not explicitly declare matching target roots, `dh-c` still resolves those families from the built-in project categories based on the resolved `samples`, `examples`, and `tests` directories.

`project.dh` can make those same behaviors explicit and reusable, but the flags themselves are not deprecated or secondary.

### Companion `.dh`

Companion `.dh` files can override `output=`, `build-runs-tests=`, and `no-dsl=` for the owning target.

Dependency blocks can additionally set `test=true` to run that dependency's tests during dependency traversal.

## Test Mode

When running in test mode with `dh-c test`, a special executable with the `-TEST` suffix is created. This test executable is automatically removed after the test is completed, keeping your build directory clean.

## Version

Current version: 0.1.0-alpha.0.1
