# dh-c Configuration Files

This document is the canonical guide for files a user may write for `dh-c`.
For command syntax, start with `dh-c --help`. For a focused version of this
contract, use:

```sh
dh-c help files
dh-c help project-dh
dh-c help dh-file
dh-c help precedence
dh-c help invocation-only
```

## 1. Choose the file by ownership scope

| File | Written by | Scope | Sections |
| --- | --- | --- | --- |
| `workspace.dh` | user | descendant projects in one workspace | no |
| `project.dh` | user | one named project | yes |
| `target.dh` | user | one resolved directory target | no |
| `<source>.dh` | user | one selected source and its build unit | primary projectless source only |
| file passed by `--dh-file` | user | one invocation or reusable policy overlay | no |
| `lock.dh` / `<source>.lock.dh` | `dh-c` | exact dependency resolution | generated |
| `manifest.dh` | `dh-c` | prebuilt artifact compatibility | generated |

Authored `.dh` files are strict. Unknown keys, malformed `key=value` lines,
and sections in a flat file are errors. `dh-c` does not silently ignore a
misspelled build contract.


## 2. Create a safe starting point

Use the scaffold commands when beginning a named project or workspace:

```sh
dh-c project app
dh-c workspace .
```

`dh-c project [path]` creates a minimal project without overwriting an existing
`project.dh`:

```text
app/
├── project.dh
├── src/main.c
├── include/
└── tests/
```

The generated project is immediately buildable with `dh-c build`.

`dh-c workspace [path]` creates `workspace.dh` as a strict flat defaults file
and cache/discovery boundary. It also refuses to overwrite an existing
configuration. Use `dh-c help project` and `dh-c help workspace` for command
examples, then `dh-c help project-dh` and `dh-c help workspace-dh` for the file
contracts.

## 3. Resolution order

Configuration is merged in this order:

1. built-in and profile defaults
2. `workspace.dh`
3. root `project.dh`
4. resolved target's `target.dh`
5. selected source companions, in source order
6. explicit `--dh-file` overlays, in command-line order
7. command-line options

Later scalar values replace earlier values. Repeatable values accumulate in
order:

- `include`
- `isystem`
- `define`
- `undef`
- `link`
- `link-dir`
- `comp-args`
- `link-args`

## 4. Flat `.dh` files

`workspace.dh`, `target.dh`, secondary `<source>.dh` companions, and files
supplied by `--dh-file` share the same strict flat grammar. The primary source
companion of a projectless build unit uses the same top-level properties but may
also own dependency sections.

```ini
# comments start with # or ;
output=my-tool
kind=executable
std=c17
include=include
define=MY_TOOL_BUILD
link=user32
```

Sections are not allowed in ordinary flat files. Target-root declarations belong
in root `project.dh`. Dependencies belong either in root `project.dh` or, for a
projectless unit, in the first selected source's companion.

### 4.1 Build defaults

```ini
output=<name>
kind=<executable|static-lib|shared-lib|lib>
build-runs-tests=<on|off>
```

`kind=lib` means the project library set, not a single static archive. It
produces native static and shared artifacts, plus the profile's LTO static
variant when enabled.

### 4.2 Toolchain and target

```ini
compiler=<command>
std=<c-standard>
arch=<target-triple>
target=<target-triple>
target-arch=<architecture-feature-set>
target-tune=<cpu>
target-abi=<abi-name>
sysroot=<path>
entry=<symbol>
```

`arch` and `target` are aliases.

### 4.3 Compile inputs

```ini
include=<path>
isystem=<path>
define=<macro-or-macro=value>
undef=<macro>
comp-args=<raw compiler arguments>
profile=<dev|fast|test|profile|stable|release|optimize|compact|micro>
```

Repeat these keys to append multiple values.

`fast` prioritizes compilation latency with assertions enabled, no debug
information, and no stack-frame size diagnostic. Other profiles retain the
default `-Wframe-larger-than=4096` diagnostic.

### 4.4 Compile environment and diagnostics

```ini
hosted=<on|off>
freestanding=<on|off>
loose-errors=<auto|never|warn|suppress>
macro-backtrace-limit=<short|unlimited|non-negative integer>
```

`freestanding=on` changes C compilation semantics. It does not implicitly
remove startup files, libc, or default libraries.

### 4.5 Link inputs and runtime policy

```ini
link=<library-name-or-supported-link-input>
link-dir=<path>
link-args=<raw linker arguments>
link-script=<path>
entry=<symbol>

link-dsl=<on|off>
link-libc=<on|off>
link-default-libs=<on|off>
link-start-files=<on|off>
link-compiler-rt=<auto|on|off>
link-stdlib=<on|off>
link-crt=<on|off>
link-mode=<auto|static|shared>
```

`link-stdlib=off` disables compiler-driver startup files and default libraries.
Explicit `link`, `link-dir`, and `link-args` inputs remain active, so a user may
supply `msvcrt`, `user32`, musl, a custom runtime, startup objects, or a fully
self-authored runtime.

When a manually linked runtime changes compile-time capability facts, declare
those facts explicitly with `define=COMP_HAS_*` or `define=COMP_NO_*`.

`link-crt` is the exact alias of `link-start-files`; it is not a separate
runtime-library selector.

### 4.6 Artifact and optimization policy

```ini
output-ext=<.extension>
objcopy=<tool>
objcopy-format=<format>
prebuilt=<auto|off|required>
lto=<auto|off|on|full|thin>

omit-frame-pointer=<auto|on|off>
function-sections=<auto|on|off>
data-sections=<auto|on|off>
gc-sections=<auto|on|off>
whole-archive=<auto|on|off>
unroll-loops=<auto|on|off>
unwind-tables=<auto|on|off>
async-unwind-tables=<auto|on|off>
exceptions=<auto|on|off>
strip=<auto|on|off>
icf=<auto|off|safe|all>
merge-all-constants=<auto|on|off>
stack-protector=<auto|on|off>
```

`output-ext` applies to a single output artifact. It is invalid for an automatic
`kind=lib` build because that build intentionally creates multiple artifact
kinds.

### 4.7 Version constants

```ini
version-namespace=my_lib
version-core=1.2.3
version-prefix=<alpha|beta|rc>
version-suffix=<number>
version-build=<identifier>
```

These describe the built software and generate compile-time version constants.
They are unrelated to `manifest.dh` schema compatibility.

### 4.8 Relative paths

The following relative property paths are resolved from the `.dh` file that
contains them:

- `include`
- `isystem`
- `link-dir`
- `sysroot`
- `link-script`

This keeps a companion or reusable overlay relocatable and independent of the
shell's current directory.

## 5. `workspace.dh`

`workspace.dh` is both a workspace boundary and a flat defaults file.

```text
workspace/
├── workspace.dh
├── app/project.dh
└── core/project.dh
```

Example:

```ini
compiler=clang
std=c17
include=shared/include
macro-backtrace-limit=short
```

The workspace owns the preferred shared build cache:

```text
workspace/.dh-c/cache/
```

Mutable dependency source checkouts remain project-owned under each project's
`.dh-c/deps/`, so two projects may resolve the same dependency name to different
revisions without collision.

`workspace.dh` is flat. It cannot declare dependencies or target roots. Mutable
dependency state belongs to each descendant project or projectless source unit,
not to one workspace-wide checkout namespace.

## 6. Root `project.dh`

`project.dh` is the complete contract for one named project.

```ini
output=my-app
kind=executable
std=c17
link-dsl=on
pch=auto

[target-root tools]
path=tools
kind=executable
selection=dir
link-project=on

[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
linking=shared
prebuilt=auto
runtime-file=bin/SDL3.dll
```

### 6.1 Top-level structural keys

In addition to all flat property keys, project scope supports:

```ini
self-root=<path>
exclude=<path>
pch=<auto|off|deps|header-path>
pch-exclude=<header>
```

### 6.2 Target-root sections

```ini
[target-root <name>]
path=<project-relative path>
kind=<executable|static-lib|shared-lib|lib|image|preprocessed|assembly>
selection=<path|file|dir>
link-project=<on|off>
exclude=<target-relative path>
```

### 6.3 Dependency sections

Every section that does not begin with `target-root ` is a dependency.

```ini
[dependency-name]
path=<local path>
source=<git URL>
archive=<archive URL or local path>
package-root=<materialized-source-relative package path>
revision=<branch|tag|commit>
provider=<dh|cmake|make|custom|prebuilt>
build-command=<command>
install-command=<command>
runtime-file=<package-relative path>
profile=<profile>
linking=<static|shared>
prebuilt=<auto|off|required>
link-dsl=<on|off>
test=<on|off>
```

`source=` and `archive=` are mutually exclusive. `revision=` applies only to Git
sources and is rejected beside `archive=`; archive SHA-256 values are generated
into `lock.dh`. `package-root=` is a relative path for `provider=prebuilt` when
the materialized source wraps the actual `include/`, `lib/`, and `bin/` package
root. It cannot escape the materialized source. `runtime-file` is repeatable.
Dependency sections also accept compile/link property keys when the dependency
requires a local contract override.

Use:

```sh
dh-c fetch
dh-c update
dh-c status
dh-c deps
dh-c graph
```

## 7. `target.dh`

A directory target may contain `target.dh`:

```text
examples/demo/
├── target.dh
├── include/
└── src/
```

```ini
output=demo-tool
kind=executable
include=include
define=DEMO_BUILD
```

It applies only when that directory is the resolved target. Build, test,
syntax, and compile-database operations validate the same file contract.

`target.dh` replaces the ambiguous historical use of a nested `project.dh` as a
flat target overlay. A nested project that is genuinely independent still uses
its own root `project.dh` and is discovered as a separate project.

## 8. Source companions and projectless dependency units

When sources are selected, `dh-c` loads same-stem companions:

```text
main.c  -> main.dh
util.c  -> util.dh
```

```sh
dh-c build main.c util.c
```

loads both companions in source order. This lets code and the build knowledge
required by that code remain separate but colocated.

When no `project.dh` owns the invocation, the **first selected source** is the
primary build-unit owner. Its companion may contain dependency sections:

```ini
# main.dh
link-dsl=off

[fmt]
source=https://example.invalid/fmt.git
revision=v1.0.0
provider=dh
```

```sh
dh-c update main.c
dh-c build main.c util.c
dh-c status main.c
```

The exact resolution is generated beside the owner as `main.lock.dh`. Secondary
companions such as `util.dh` and explicit `--dh-file` overlays remain strict flat
files; sections there are errors. In a durable multi-source unit, keep source
order stable or use a named `project.dh` when dependency ownership should not be
expressed by a primary source.

Use source companions for source-local compile/link requirements. Avoid placing
build-unit-wide output names in multiple companions unless their override order
is intentional.

## 9. Explicit overlays

Use `--dh-file` for named, reusable policy bundles:

```sh
dh-c build main.c \
  --dh-file=policies/freestanding.dh \
  --dh-file=platform/windows.dh
```

Do not confuse it with `--dh`:

```text
--dh-file=<path>   authored build overlay
--dh=<path>        DH installation override
```


## 10. Persistent contract versus invocation-only controls

Authored `.dh` files describe a reusable build contract. They intentionally do
not store one command invocation's scheduling, presentation, selection, or
maintenance behavior.

Keep these controls on the command line:

- `jobs`, `verbose`, `progress`, `commands`, and `elapsed-precision`
- executable arguments such as `exec-args`
- `dry-run`, recursion, and clean/prune selectors
- test/sample/example/source selection for one invocation
- analysis and emit requests such as maps, disassembly, IR, or temporary files

Persist compiler, linker, runtime, target, output, optimization, and version
requirements in `workspace.dh`, `project.dh`, `target.dh`, a source companion,
or an explicit `--dh-file` overlay.

When a selection is a stable named part of a project, declare a
`[target-root <name>]` in `project.dh` rather than encoding the selection in a
policy overlay.

```sh
dh-c help invocation-only
```

## 11. Generated files

### `lock.dh` and `<source>.lock.dh`

- `lock.dh` is generated beside root `project.dh`
- `<source>.lock.dh` is generated beside the primary projectless source
- both record exact resolved dependency commits or archive SHA-256 values
- both are read by `fetch`, build, and status operations
- both are intentionally updated only by `update`
- both should normally be committed to source control
- neither is removed by `clean --deps`

Projectless disposable dependency source/build/package state is isolated under a
workspace unit scope when `workspace.dh` is found, otherwise under the user/global
`dh-c` cache. The durable lock remains beside the source that owns it.

### `manifest.dh`

- generated for library/prebuilt target-profile output
- inventories all library artifacts, not the last executable built
- records ABI contract, producer-link provenance, and LTO toolchain identity
- is not replaced by test/sample/example executables
- rejects old or structurally invalid contracts rather than preserving
  unnecessary schema compatibility

## 12. Discoverability

```sh
dh-c --help
dh-c help --list
dh-c help files
dh-c help project-dh
dh-c help workspace-dh
dh-c help target-dh
dh-c help dh-file
dh-c help dependencies
dh-c help precedence
dh-c help lock-dh
dh-c help manifest-dh
dh-c help --all
```
