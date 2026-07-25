# dh-c Prebuilt Package Contract

## Purpose

Prebuilt packages are immutable SDK inputs. They are not ordinary `build/` cache entries. `dh-c` can use them for `dh` itself and for any dependency declared in `project.dh`.

## Layout

A platform-specific SDK or dependency package uses:

```txt
<project>/
  include/
  prebuilt/
    dev/
      libs/
      deps/
    test/
      libs/
      deps/
    stable/
      libs/
      deps/
    release/
      libs/
      deps/
```

`libs/` contains the package's own artifacts. `deps/` is optional and contains transitive headers and link artifacts in the same relative layout that would otherwise be staged under `lib/deps/`.

Artifact names follow the normal library contract:

| Platform | Native static | LTO static | Shared | Import library |
|---|---|---|---|---|
| Windows | `mylib.lib` | `mylib.lto.lib` | `mylib.dll` | `mylib.dll.lib` |
| Linux | `libmylib.a` | `libmylib.lto.a` | `libmylib.so` | — |

Profiles without effective LTO require only the native static archive. Profiles with effective LTO package both native and `.lto` static archives so the final consumer link can choose correctly.

## Selection Modes

```txt
prebuilt=auto
prebuilt=off
prebuilt=required
```

- `auto` is the default. It uses a complete package when available and otherwise builds source.
- `off` always traverses and builds source.
- `required` rejects a missing package instead of silently building source.

The same values are accepted through `--prebuilt=<mode>`. A bare `--prebuilt` means `required`.

## Per-Dependency Selection

```txt
prebuilt=off

[fmt]
path=../fmt
linking=static
prebuilt=auto

[crypto]
path=../crypto
linking=shared
prebuilt=required
```

Here the project normally source-builds dependencies, `fmt` optionally consumes its package, and `crypto` must consume its shared package. Dependency-local policy is intentionally selectable independently from the project default.

## Tests And CI

A normal `dh-c test` may consume prebuilt dependencies. This lets CI rebuild only the current project and its test runner.

When dependency tests are explicitly requested with recursive testing or `test=true`, `dh-c` requires source for that dependency. A prebuilt library alone cannot represent or execute the dependency project's own tests.

## Package Boundaries

The current lookup assumes a platform-specific SDK archive: Windows and Linux packages should be distributed separately. LTO archives remain tied to the compatible compiler/LTO toolchain used by that SDK. Native archives and DLL/SO outputs are the compatibility fallback.

## Recommended SDK Profile Set

For general Windows/Linux SDK packages, the recommended supported set is:

- `fast`: developer iteration; native archive only
- `dev`: dependency debugging with assertions and full debug information; native archive only
- `test`: CI and project-test builds without rebuilding unchanged dependencies; native archive only
- `stable`: general optimized SDK use; native and ThinLTO static archives plus shared output
- `release`: final distribution builds; native and ThinLTO static archives plus shared output

`profile` is better shipped as an optional diagnostics package because profiling often requires a deliberate symbol/frame-pointer/tool setup. `optimize` is host-native and must be built for the final machine. `compact` and `micro` are final-size-policy builds whose value depends strongly on the complete link, so they remain source/local-build profiles in the general SDK.

There is currently no `compat` build profile. Portability should remain a target/toolchain/ABI package property rather than another optimization profile. The native non-LTO archive emitted by `stable` already provides the intended portable fallback within a matching platform SDK.

## Artifact manifest

New packages should include `manifest.dh` in each target/profile directory. dh-c validates
the normalized target, profile, and effective LTO state before using a packaged artifact.
See [artifact-manifest.md](artifact-manifest.md). Packages without a manifest remain accepted
as legacy packages during migration.
