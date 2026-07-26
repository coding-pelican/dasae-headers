# dh-c Build Guide

`dh-c` is a direct-source and project build tool for dasae-headers. Its usage
model combines direct compiler-style input with optional workspace/project
contracts:

```sh
dh-c build main.c
dh-c build main.c util.c
dh-c build
dh-c test
dh-c package release
```

The command line and selected sources always remain first-class build input.
`project.dh` is optional for direct builds.

## Start here

Create a minimal named project or a workspace boundary without hand-writing the
first file:

```sh
dh-c project app
dh-c workspace .
```

Both commands refuse to overwrite an existing `project.dh` or `workspace.dh`.
The generated project is immediately buildable.

```sh
dh-c --help
dh-c help files
dh-c help build
dh-c help project-dh
dh-c help dh-file
dh-c help precedence
dh-c help invocation-only
```

The canonical file contract is
[`dh-c/docs/dh-files.md`](./dh-c/docs/dh-files.md).

## 1. Direct builds

```sh
dh-c build main.c
dh-c build main.c util.c
dh-c run main.c
dh-c test test-main.c
```

For each selected source, `dh-c` automatically loads a same-stem companion:

```text
main.c -> main.dh
util.c -> util.dh
```

Without a named `project.dh`, the first selected source is the build-unit owner.
Its companion may declare dependencies and `dh-c update main.c` generates
`main.lock.dh` beside it. Secondary companions remain flat overlays.

An explicit reusable overlay may be added with:

```sh
dh-c build main.c --dh-file=windows-runtime.dh
```

`--dh-file` is not `--dh`; `--dh` overrides the DH installation path.

## 2. Named projects

```text
my-project/
├── project.dh
├── src/
├── include/
├── tests/
├── samples/
└── examples/
```

Minimal `project.dh`:

```ini
output=my-project
kind=executable
std=c17
```

```sh
dh-c build
dh-c run
dh-c test
```

`dh-c` detects the nearest ancestor `project.dh`, stopping at a discovered
`workspace.dh` boundary.

## 3. Configuration files

| File | Role |
| --- | --- |
| `workspace.dh` | shared flat defaults and workspace cache/discovery boundary |
| `project.dh` | complete project, target-root, and dependency contract |
| `target.dh` | one resolved directory target's flat defaults |
| `<source>.dh` | one selected source's companion; the primary projectless companion may also own dependencies |
| `--dh-file` input | explicit reusable flat overlay |
| `lock.dh` / `<source>.lock.dh` | generated exact dependency resolution |
| `manifest.dh` | generated prebuilt compatibility contract |

Merge order:

```text
built-in/profile
-> workspace.dh
-> project.dh
-> target.dh
-> selected source companions
-> explicit --dh-file overlays
-> CLI
```

Authored `.dh` files are strict. Unknown keys and illegal sections fail.

## Configuration versus invocation controls

The `.dh` files store reusable compiler, linker, runtime, target, output,
optimization, dependency, and version contracts. Scheduling and one-off command
selection stay on the command line: jobs, progress/verbosity, run arguments,
dry-run/clean scopes, sample/test selection, and analysis/emit requests.

```sh
dh-c help invocation-only
```

## 4. Everyday commands

### Build

```sh
dh-c build [profile] [path] [options]
```

Common forms:

```sh
dh-c build
dh-c build release
dh-c build main.c util.c
dh-c build --example demo
dh-c build --lib --shared
dh-c build --image firmware.c --link-script=layout.ld
```

### Run

```sh
dh-c run [profile] [path] [options]
```

### Test

```sh
dh-c test [profile] [path] [options]
```

Test output separates result and timing:

```text
[TEST]
  status: PASS
  exit: 0
  executable: ...
  elapsed: 0.012s
[TIMING]
  setup: 1.20s
    project library: 0.80s
    executable: 0.38s
  execution: 0.01s
Finished `test` in 1.21s
```

### Clean and prune

```sh
dh-c clean
dh-c clean --cache --older-than=30d --dry-run
dh-c clean --deps --unused --dry-run
dh-c clean --deps --older-than=90d
dh-c clean --deps --unused --force
```

Git dependency checkouts with user changes are preserved unless `--force` is explicit.
Untracked `build/`, `.dh-c/`, package staging, and dependency-export state created by dh-c
do not make an otherwise clean checkout look user-modified. `lock.dh` is never removed
by dependency cleanup.

## 5. Inspection commands

```sh
dh-c plan [profile] [path]
dh-c explain rebuild [profile] [path]
dh-c target show
dh-c doctor
dh-c toolchain all
dh-c compile-db
dh-c syntax
dh-c tidy
dh-c format
```

`plan` and `explain rebuild` are read-only. They do not build dependencies,
write caches/contracts, acquire project build-state paths, or create
`build/native`.

## 6. Profiles

| Profile | Intended use |
| --- | --- |
| `dev` | debug iteration; default |
| `fast` | fastest compile path |
| `test` | test-focused optimization/debug balance |
| `profile` | profiling build |
| `stable` | stable optimized build with ThinLTO policy |
| `release` | release optimization and reduced unwind/exception policy |
| `optimize` | maximum native optimization |
| `compact` | size optimization |
| `micro` | extreme size optimization |

Use `dh-c help profiles` or `dh-c help --all` for the current exact flags.

## 7. Link and runtime model

These controls are independent build facts:

```ini
freestanding=on
link-libc=off
link-default-libs=off
link-start-files=off
link-compiler-rt=auto
link-stdlib=off
link-crt=off
```

`freestanding` changes compilation semantics only.

`link-stdlib=off` disables compiler-driver startup files and default libraries,
but explicit inputs remain:

```ini
link-stdlib=off
link=msvcrt
link=user32
define=COMP_HAS_LIBC
define=COMP_HAS_STDLIB
```

This supports compiler/platform runtimes, glibc or musl target contracts,
custom runtimes, and fully user-authored startup/runtime code. Cache keys record
the effective compile and link command contracts. Prebuilt manifests compare
public ABI facts rather than requiring every consumer to repeat a producer's
private top-level link list. LTO artifacts additionally require a compatible
compiler toolchain contract.

## 8. Library and prebuilt behavior

A project with:

```ini
kind=lib
output=core
```

builds the full library set in every profile. Effective LTO determines whether
an additional LTO static archive is present; shared/static production itself is
not restricted to stable/release.

The profile directory contains one generated `manifest.dh` that inventories the
library artifacts. Test/sample/example executables do not overwrite it.

Host builds also provide:

```text
build/native -> build/<normalized-host-target>
```

as a symbolic link or Windows junction where supported. Explicit cross-target
builds do not move the native alias.

See:

- [`dh-c/docs/artifact-manifest.md`](./dh-c/docs/artifact-manifest.md)
- [`dh-c/docs/prebuilt-packages.md`](./dh-c/docs/prebuilt-packages.md)

## 9. Dependencies

Declare dependencies in root `project.dh`, or in the primary source companion of
a projectless build unit:

```ini
[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
linking=shared
prebuilt=auto
runtime-file=bin/SDL3.dll
```

Named-project commands:

```sh
dh-c fetch
dh-c update
dh-c status
dh-c deps
```

Projectless source-unit commands:

```sh
dh-c update main.c
dh-c fetch main.c
dh-c status main.c
dh-c deps main.c
dh-c graph main.c
dh-c build main.c util.c
```

`lock.dh` lives beside `project.dh`; `<source>.lock.dh` lives beside the primary
projectless source. `fetch` preserves an existing resolution and `update`
intentionally changes it. `graph` supports both scopes; `package` and `install`
remain named-project operations.

Provider cross-target contracts include compiler, archiver, target, sysroot,
and provider-specific toolchain variables where applicable.

See [`dh-c/docs/external-dependencies.md`](./dh-c/docs/external-dependencies.md).

## 10. Generated state and source control

Normally tracked:

```text
workspace.dh
project.dh
target.dh
*.dh source companions
lock.dh
*.lock.dh projectless source locks
source and public assets
```

Normally ignored:

```text
build/
.dh-c/
prebuilt/   # unless deliberately vendored as source input
dist/       # generated release output
```

`manifest.dh` belongs inside generated build/prebuilt packages, not as a
hand-authored root project file.

## 11. Complete references

- [`dh-c/docs/dh-files.md`](./dh-c/docs/dh-files.md)
- [`dh-c/docs/project-dh-contract.md`](./dh-c/docs/project-dh-contract.md)
- [`dh-c/docs/external-dependencies.md`](./dh-c/docs/external-dependencies.md)
- [`dh-c/docs/artifact-manifest.md`](./dh-c/docs/artifact-manifest.md)
- [`dh-c/docs/prebuilt-packages.md`](./dh-c/docs/prebuilt-packages.md)
- `dh-c help --all`
