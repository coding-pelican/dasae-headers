# dh-c prebuilt manifest

`manifest.dh` is the fixed compatibility and artifact-inventory contract for one
prebuilt target/profile directory. A successful library build writes it beside
`libs/`, so that the whole profile directory can be promoted to `prebuilt/`
without reconstructing concrete artifacts from `project.dh`.

`project.dh` describes requested policy. `manifest.dh` describes the library
artifacts that actually exist in one concrete target/profile directory.

## Location

Local library builds:

```text
build/<normalized-target>/<profile>/manifest.dh
build/<normalized-target>/<profile>/libs/
```

Packaged prebuilts:

```text
prebuilt/<normalized-target>/<profile>/manifest.dh
prebuilt/<normalized-target>/<profile>/libs/
```

A real implicit-host build also exposes the normalized target directory through
`build/native`. The alias is not a separate artifact location and is not created
by read-only inspection commands.

## Fixed schema

```ini
target=x86_64-w64-windows-gnu
profile=stable
compiler=clang
artifact=static|libs/foo.lib
artifact=static-lto|libs/foo.lto.lib
artifact=shared|libs/foo.dll
artifact=import|libs/foo.dll.lib
```

Required scalar keys:

- `target`
- `profile`
- `compiler`

`artifact=` is repeated once for each library artifact in `libs/`. Supported
roles are `static`, `static-lto`, `shared`, and `import`.

Unknown keys, duplicate scalar keys, duplicate artifact entries, malformed
roles, paths outside `libs/`, and manifests without artifacts are rejected.
There is no `manifest-version`, format selector, or legacy fallback. This is one
dh-c-owned schema. An incompatible schema change changes dh-c and the produced
prebuilt packages together; old forms are not retained merely for compatibility.

The software version declared through `version-*` keys is unrelated. It
versions the program or library being built; `manifest.dh` records a concrete
prebuilt artifact set.

## Why the manifest is an inventory

A profile directory can contain more than one legitimate library artifact:

- every `kind=lib` profile emits the native static archive and shared library;
- Windows shared builds also emit an import library;
- effective-LTO profiles additionally emit a `.lto` static archive;
- multiple explicitly named library targets may share one target/profile
  directory.

Therefore a single `artifact-kind`/`artifact` pair would be overwritten by the
last successful library build. The repeated inventory preserves all artifacts
that coexist in `libs/`. Executables created by `test`, `sample`, or `example`
builds never update this manifest.

## Compatibility checks

A prebuilt artifact is usable only when:

- `manifest.dh` exists and satisfies the fixed schema;
- normalized target matches;
- profile matches;
- compiler contract matches;
- the exact selected path is listed with the expected role;
- on Windows, a selected shared library also has its listed and present import
  library.

The static role itself records whether the consumer requested native or LTO
static linkage. `prebuilt=auto` skips an incompatible package and falls back to
source; `prebuilt=required` fails; `prebuilt=off` never consumes it.

## Promotion and generic packaging

A prebuilt producer promotes or copies the complete build profile directory:

```text
build/<target>/<profile>/
    manifest.dh
    libs/
```

into:

```text
prebuilt/<target>/<profile>/
```

`dh-c package` is a generic install/runtime staging command. It translates
library artifacts into `lib/` and runtime shared objects into `bin/`; therefore
it does not copy the prebuilt-only `manifest.dh`, whose paths intentionally
refer to `libs/`.

There is no separate package file inventory yet. SHA-256 remains an explicit
roadmap item for a future verification, publishing, install-ownership, or
package-diff consumer.
