# dh-c Build Guide

`dh-c` is the build tool for the `dasae-headers` C workspace.

It does two different jobs:

- build `dh-c` itself
- use `dh-c` to build C files, C projects, target-root families, tests, and dependency graphs

This guide only documents what is supported now.

`workspace` and `project` are reserved command names. They are intentionally not implemented yet and are not part of the supported flow in this document.

## What `dh-c` Is For

`dh-c` is not only a "project generator".

It is a build contract layer over plain C compilation:

- direct file builds still work
- project-local defaults can be declared in `project.dh`
- reusable self code and named target families can be declared in `project.dh`
- dependency projects can be built and copied into `lib/deps/`
- tests, sample/example families, and target-root families can all be addressed through one CLI

The practical split is:

- explicit CLI paths are the immediate build target
- `project.dh` supplies defaults, reusable structure, and dependency contracts

## Current Support

Supported commands:

- `build`
- `lib`
- `run`
- `test`
- `deps`
- `clean`
- `build-dsl`
- `test-dsl`
- `clean-dsl`
- `build-self`
- `clean-self`
- `--help`
- `--version`

Reserved and unsupported commands:

- `workspace`
- `project`

## Prerequisites

You need these tools in `PATH`:

- `clang`
- `make`

The default self-build flow expects the same warning/extension model used by this repository's Clang configuration.

## Build `dh-c`

From the repository root:

```sh
cd dh-c
make PROFILE=release
```

The built binary is written to:

```txt
dh-c/build/dh-c.exe     # Windows
dh-c/build/dh-c         # Unix-like shells
```

Useful self-build profiles:

- `PROFILE=dev`
- `PROFILE=test`
- `PROFILE=release`
- `PROFILE=optimize`
- `PROFILE=compact`
- `PROFILE=micro`

## Verify `dh-c`

From the repository root:

```sh
sh dh-c/tests/run-tests.sh
sh dh-c/tests/run-tests.sh --integration
sh dh-c/tests/run-tests.sh --address-sanitizer
sh dh-c/tests/run-tests.sh --integration --address-sanitizer
```

What these cover:

- unit coverage for `dal-c-ext` helpers and `dal-c` contracts
- CLI help/version/reserved-command behavior
- project build/run/test scenarios
- target-root contract scenarios
- dependency copy scenarios
- memory checks through AddressSanitizer

On Windows, run these from Git Bash or another `sh` environment rather than PowerShell.

## Quick Start

### 1. Build a Single File

```sh
dh-c build src/main.c
dh-c run src/main.c
```

This path works even outside a `project.dh` project.

### 2. Build the Default Project Target

```sh
dh-c build
dh-c run
dh-c test
dh-c clean
```

This requires running inside a directory where `dh-c` can detect the nearest ancestor `project.dh`.

### 3. Build a Target Family

```sh
dh-c build cmd/runner1
dh-c run cmd/runner1
dh-c build plugins/render
```

If the path falls under a declared `[target-root ...]`, the target-root contract decides:

- artifact kind
- allowed path selection mode
- whether self code is linked

### 4. Build Compatibility Families

```sh
dh-c build --sample
dh-c build --example
dh-c build --test
```

These are the built-in compatibility selectors for `samples/`, `examples/`, and `tests/`.

## Command Model

`dh-c` reads each command as:

```txt
dh-c <verb> [profile] [path] [options]
```

The tokens play different roles:

- verb: `build`, `run`, `test`, `clean`, `deps`
- profile: `dev`, `test`, `profile`, `stable`, `release`, `optimize`, `compact`, `micro`
- path: explicit file path, directory path, or target-root path
- modifier: `--lib`, `--shared`, `--self`, `--dsl`, `--recur`, `--debug`, `--verbose`
- selector: `--sample`, `--example`, `--test`

### Path Resolution Rules

`dh-c build`

- builds the default project output

`dh-c build file.c`

- builds that file directly

`dh-c run path/to/target`

- if the path matches a declared target root, the target-root contract applies
- otherwise `dh-c` treats the path as an explicit file or directory selection

`dh-c build .`

- `.` is a compatibility alias for `--all`
- it means "build all source files in the project `src` family"

## Profiles

The built-in profiles are:

- `dev`: `-g3 -Og`, assertions enabled
- `test`: `-g -O1`, assertions enabled
- `profile`: `-g -O2`, assertions enabled
- `stable`: `-g1 -O2`, assertions disabled, ThinLTO
- `release`: `-g1 -O3`, ThinLTO, exceptions/unwind tables disabled, safe ICF
- `optimize`: `-O3 -march=native -mtune=native`, Full LTO, assertions disabled
- `compact`: `-Os`, ThinLTO size-oriented build
- `micro`: `-Oz`, ThinLTO smallest-size profile

## Important Options

Shared build options:

- `--compiler=<name>`
- `--std=<std>`
- `--arch=<target>`
- `--target=<triple>`
- `--use-dsl`
- `--no-dsl`
- `--freestanding`
- `--hosted`
- `--has-libc`
- `--no-libc`
- `--has-default-libs`
- `--no-default-libs`
- `--has-start-files`
- `--no-start-files`
- `--std-lib`
- `--no-std-lib`
- `--crt`
- `--no-crt`
- `--sysroot=<path>`
- `--include=<path>` or `-I<path>`
- `--isystem=<path>`
- `--link=<lib>` or `-l<lib>`
- `--define=<macro>` or `-D<macro>`
- `--undef=<macro>` or `-U<macro>`
- `--entry=<symbol>`
- `--compiler-args="..."`
- `--comp-args="..."`
- `--args="..."`
- `--file=<path>`
- `--output=<path>` or `-o<path>`
- `--exclude=<path>`
- `--dh-file=<path>`
- `--loose-errors`
- `--show-commands`
- `--verbose`
- `--dh=<path>`
- `--prebuilt=<auto|off|required>`

Build-specific modifiers:

- `--lib`
- `--static`
- `--shared`
- `--sample`
- `--example`
- `--test`
- `--all`
- `--dsl`
- `--self`
- `--recur`


## Library Artifact Variants

Library kind and optimization profile remain separate contracts.

For a static-library build:

- profiles without effective LTO produce only the native archive
- profiles with effective LTO produce both the native archive and an LTO archive
- the native archive is always compiled with LTO disabled
- the LTO archive uses the effective profile mode (`thin`, `full`, or explicit `on`)

Artifact names:

| Platform | Native static | LTO static | Shared | Import library |
|---|---|---|---|---|
| Windows | `mylib.lib` | `mylib.lto.lib` | `mylib.dll` | `mylib.dll.lib` |
| Linux | `libmylib.a` | `libmylib.lto.a` | `libmylib.so` | not required |

A shared-library build performs its profile LTO while producing the DLL/SO; it does not expose a separate LTO artifact. A project with `kind=lib` produces the applicable static variants and the shared library.

Final linked targets select one static variant automatically:

- effective LTO enabled -> prefer the matching `.lto` archive
- effective LTO disabled -> use the native archive
- when no `.lto` peer exists -> fall back to the native archive

Run/test-specific modifiers:

- `--debug`
- `--exec-args="..."`

Clean-specific modifiers:

- `--cache`
- `--self`
- `--dsl`
- `--recur`

## `project.dh`

`project.dh` is the project contract file.

It currently supports three layers:

- project-wide defaults
- named target-root blocks
- dependency blocks

### Project-Wide Keys

Supported top-level keys:

```txt
output=<name>
build-runs-tests=<true|false>
no-dsl=<true|false>
pch=<auto|off|path>
pch-exclude=<header>
self-root=<path>
prebuilt=<auto|off|required>
```

Meaning:

- `output`: default artifact name when the CLI does not override it
- `build-runs-tests`: plain `dh-c build` also runs tests afterward
- `no-dsl`: disables automatic `dh` linking for that project scope
- `pch`: auto/off/explicit PCH header contract
- `pch-exclude`: files that must compile without the project PCH
- repeated `self-root`: reusable project-owned source roots
- `prebuilt`: default packaged-artifact policy for `dh` and dependency projects

### Named Target Roots

Target roots are declared like this:

```txt
[target-root cmd]
path=cmd
kind=executable
selection=dir
link-self=true
```

Supported target-root keys:

- `path`
- `kind`
- `selection`
- `link-self`

Meaning:

- `path`: root directory for that target family
- `kind`: `executable`, `static-lib`, or `shared-lib`
- `selection`: `path`, `file`, or `dir`
- `link-self`: whether the target links reusable self code

### Dependency Blocks

Dependency blocks use section names such as:

```txt
[B]
path=../B
profile=default
linking=static
no-dsl=true
test=true
prebuilt=auto
```

Supported dependency keys:

- `path`
- `profile`
- `linking`
- `no-dsl`
- `test`
- `prebuilt`

Meaning:

- `path`: dependency project root
- `profile`: dependency build profile or `default`
- `linking`: static/shared preference for that dependency
- `no-dsl`: dependency-local DSL suppression
- `test`: run that dependency's tests during dependency traversal
- `prebuilt`: dependency-local `auto`, `off`, or `required` policy; this may override the project-wide policy

## Directory Layout

A typical project layout is:

```txt
my-project/
  project.dh
  include/
  src/
  tests/
  samples/
  examples/
  lib/
    deps/
  build/
  prebuilt/
    stable/
      libs/
      deps/
  .cache/
```

Supported built-in directory aliases:

- include: `include`, `includes`, `inc`
- src: `src`, `source`, `sources`
- tests: `tests`, `test`
- samples: `samples`, `sample`
- examples: `examples`, `example`

Only one alias variant per category should exist at the same project level.

## Generated Directories

`dh-c` writes generated data to three main places:

`build/`

- final artifacts
- object files
- generated plan makefiles
- PCH outputs

`prebuilt/<profile>/`

- packaged SDK artifacts that are trusted as immutable inputs
- `libs/` contains the dependency's own native/LTO/shared artifacts
- optional `deps/` contains already-staged transitive dependency artifacts
- this directory is deliberately separate from `build/`, so stale local build caches cannot masquerade as SDK packages

`.cache/`

- generated unity sources
- generated test runner sources

`lib/deps/`

- copied dependency headers
- copied dependency libraries
- copied dependency PCH files
- copied transitive dependency artifacts

## Dependency Flow

`dh-c deps` first checks the selected packaged-artifact policy and then prepares the consumer project's `lib/deps/`.

Policy modes:

- `auto` (default): use `prebuilt/<profile>` when the required artifact exists; otherwise build from source
- `off`: ignore packaged artifacts and build from source
- `required`: use packaged artifacts and fail instead of falling back to source

A normal `dh-c test` may use prebuilt dependencies, which avoids rebuilding them in CI. When recursive dependency tests are explicitly requested, source is required because a packaged binary cannot execute the dependency's own test sources.

After selecting or building the dependency, `dh-c` copies its consumable outputs into the consumer project's `lib/deps/`.

What is copied:

- public headers
- built static/shared libraries
- PCH files when present
- transitive dependency artifacts

This is why `deps` is the command that prepares a project-local consumable dependency boundary rather than only invoking nested builds.

## Examples

Build the default project output:

```sh
dh-c build
```

Build and run a target-root executable:

```sh
dh-c run cmd/runner1
```

Build a shared plugin:

```sh
dh-c build plugins/render
```

Build a static library from an explicit file:

```sh
dh-c build --lib --static src/mylib.c
```

Build all project sources:

```sh
dh-c build --all
dh-c build .
```

Run dependency preparation:

```sh
dh-c deps --verbose
```

Run tests recursively:

```sh
dh-c test --recur
```

Build the DSL boundary explicitly:

```sh
dh-c build --dsl
dh-c build-dsl
```

Operate on the `dh-c` self boundary:

```sh
dh-c build --self
dh-c clean --self
dh-c build-self
dh-c clean-self
```

## Current Non-Goals

These names exist but are not supported commands yet:

- `dh-c workspace`
- `dh-c project`

Do not build workflows or documentation around them as if they were already implemented.

## Related Public Documents

- [`README.md`](./README.md)
- [`PROJECT_TREE.md`](./PROJECT_TREE.md)


## Help views

```sh
dh-c help          # concise command overview
dh-c help --list   # command names only
dh-c help --all    # complete reference
dh-c help build    # one command
```

## Target-scoped build layout

Build and packaged prebuilt artifacts are separated by normalized target identity:

```text
build/<target>/<profile>/
prebuilt/<target>/<profile>/
```

For an implicit host build, dh-c asks the active compiler for its target triple. `build/native` is created as a best-effort directory link to that host target directory. Failure to create the convenience link does not fail the build.

DH tests now return a non-zero process status when one or more test units fail, so CI does not need to parse `[FAIL]` or `Failed: N` output.

## Build introspection

The current build contract can be inspected without executing a build:

```sh
dh-c target show
dh-c plan dev
dh-c explain rebuild dev
dh-c doctor
```

`plan` runs the same target/source/configuration resolution as `build` and writes the real generated Makefile, but does not invoke Make. `explain rebuild` reports missing output and generated-plan staleness. `target show` prints the normalized target-scoped output directory. `doctor` checks the selected compiler, Make, archiver, DH installation, project detection, and target resolution.
