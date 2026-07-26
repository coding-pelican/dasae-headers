# `project.dh` Contract

`project.dh` is the complete, user-authored contract for one named `dh-c`
project. It is not the only `.dh` file type. Start with
[`dh-files.md`](./dh-files.md) for workspace, target, source-companion, explicit
overlay, lock, and manifest ownership.

## Minimal project

```ini
output=hello
kind=executable
std=c17
```

```text
hello/
├── project.dh
└── src/main.c
```

```sh
dh-c build
dh-c run
dh-c test
```

## Project scope

Project scope accepts all strict flat `.dh` keys documented in
[`dh-files.md`](./dh-files.md), plus:

```ini
self-root=<path>
exclude=<path>
pch=<auto|off|deps|header-path>
pch-exclude=<header>
```

Project-relative property paths are resolved from the directory containing
`project.dh`.

## Target roots

```ini
[target-root tools]
path=tools
kind=executable
selection=dir
link-project=on
exclude=generated
```

Supported keys:

| Key | Meaning |
| --- | --- |
| `path` | project-relative target-family root |
| `kind` | `executable`, `static-lib`, `shared-lib`, `lib`, `image`, `preprocessed`, or `assembly` |
| `selection` | `path`, `file`, or `dir` |
| `link-project` | whether the target links the project library contract |
| `exclude` | target-relative exclusion; repeatable |

A resolved directory target may provide a flat `target.dh`. Do not use a nested
`project.dh` merely as a target overlay.

## Dependencies

Any section other than `[target-root <name>]` is a dependency:

```ini
[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
profile=release
linking=shared
prebuilt=auto
runtime-file=bin/SDL3.dll
```

Core keys:

```text
path
source
archive
revision
provider
build-command
install-command
runtime-file / runtime-files
profile
linking
test
```

`source=` and `archive=` are mutually exclusive. `revision=` selects a Git
branch, tag, or commit and is rejected beside `archive=`; archive content is
resolved to `sha256:<hex>` in the generated lock. Dependency sections also
accept compile/link property keys, including `include`, `define`, `link`,
`link-dir`, runtime-link toggles, target/ABI settings, and `prebuilt`.

`lock.dh` is generated beside `project.dh`. `fetch` preserves an existing exact
resolution; `update` intentionally resolves again and rewrites the lock.

## Project library behavior

```ini
kind=lib
output=my-lib
```

`kind=lib` means the complete library set. On Windows this normally includes:

```text
libs/my-lib.lib
libs/my-lib.dll
libs/my-lib.dll.lib
libs/my-lib.lto.lib   # when effective LTO is enabled
```

On Unix-like targets it normally includes native/LTO static archives and the
shared library for that target. `test`, sample, and example commands prepare the
same project library set before linking their executable; they do not replace
its `manifest.dh`.

The explicit command `dh-c lib` remains the compatibility alias for
`dh-c build --lib` and requests one library artifact according to its link mode.

## PCH

```ini
pch=auto
pch-exclude=large_optional_header.h
```

- `auto`: detected DH bundle where available
- `deps`: generated `lib/deps.h`
- `off`: disable PCH
- any other value: project-relative header path

## Precedence

```text
built-in/profile
-> workspace.dh
-> project.dh
-> target.dh
-> source companions
-> --dh-file overlays
-> CLI
```

Use `dh-c help precedence` for the concise contract.

## Validation

`project.dh` is strict:

- malformed section headers fail
- malformed non-`key=value` lines fail
- unknown keys fail in their exact scope
- duplicate target-root names or paths fail
- invalid target-root contracts fail

This is intentional. A build contract typo must not become an ignored option.
