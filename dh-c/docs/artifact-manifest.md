# dh-c prebuilt manifest

`manifest.dh` is the compatibility contract for a concrete prebuilt target/profile
artifact directory. It is generated with a local build so that the same directory
can be promoted to `prebuilt/` or copied into a package without reconstructing
its build properties from `project.dh`.

`project.dh` describes requested build policy. `manifest.dh` describes the
concrete artifact that was produced.

## Location

Local builds:

```text
build/<normalized-target>/<profile>/manifest.dh
```

Packaged prebuilts:

```text
prebuilt/<normalized-target>/<profile>/manifest.dh
```

## Fixed schema

```ini
target=x86_64-w64-windows-gnu
profile=stable
artifact-kind=static-lib
artifact=foo.lto.lib
lto=on
compiler=clang
```

All six keys are required. Unknown or duplicate keys are rejected. There is no
`manifest-version`, `format`, or legacy fallback. This is one dh-c-owned schema,
not a registry of independently selectable manifest formats.

When the contract must change incompatibly, dh-c changes the schema and the
packages together. Old packages are not kept readable merely to preserve
backward compatibility.

The project/library version declared through `version-*` keys is unrelated. It
versions the software being built; the manifest describes one concrete prebuilt
artifact.

## Prebuilt compatibility

A prebuilt directory is usable only when `manifest.dh` exists and follows the
fixed schema. dh-c currently validates the requested:

- normalized target
- profile
- effective LTO state

The remaining required fields identify the artifact and compiler that produced
it and are reserved for stricter compatibility checks as the prebuilt contract
expands.

An incompatible package is skipped in `prebuilt=auto` mode and rejected in
`prebuilt=required` mode. `prebuilt=off` never consumes it.

## Packaging and installation

`dh-c package <profile>` builds the current project, stages external providers,
and creates:

```text
package/<normalized-target>/<profile>/
```

The package contains the concrete top-level artifacts and the existing
`manifest.dh`; compiler intermediates and cache state are not copied.
Conventional project `assets/` and `resources/` trees are staged, and files
installed under dependency package `bin/` directories are copied to the package
`bin/` directory.

There is no separate `package-manifest.dh` contract at present. A future package
verification/install inventory must first have a real consumer such as
`package verify`, install/uninstall ownership, publishing, or package diffing.

### Package layout

Concrete project artifacts are staged by role:

- `.exe` and `.dll` files go to `bin/`.
- `.lib`, `.a`, `.so`, and `.dylib` files go to `lib/`.
- `manifest.dh` remains at the package root.
- public project headers are copied to `include/`.
- extensionless native executables remain at the package root.

On Windows this keeps a shared library's runtime DLL separate from its import
library (`foo.dll` in `bin/`, `foo.dll.lib` in `lib/`). Dependency runtime
exports stage only explicitly named runtime files, so import/static libraries
are not accidentally copied into the runtime directory.

## Integrity roadmap

The present manifest is a compatibility contract, not a cryptographic integrity
record. SHA-256 remains an explicit later milestone for a package inventory once
verification or publishing consumes it. It is not part of the current change.
