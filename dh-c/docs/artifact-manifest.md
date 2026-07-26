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
artifact=static|libs/foo.lib|7f3b2a3e19d79db1|none|native
artifact=static-lto|libs/foo.lto.lib|91ea76258e781f20|none|4511ca2f49b713f4
artifact=shared|libs/foo.dll|3b41852001136cf8|8dbce2e48d210f67|native
artifact=import|libs/foo.dll.lib|3b41852001136cf8|8dbce2e48d210f67|native
```

Required scalar keys:

- `target`
- `profile`

Each `artifact=` entry has five fields:

```text
<role>|<relative-path>|<abi-contract>|<producer-link-contract>|<toolchain-contract>
```

Supported roles are `static`, `static-lto`, `shared`, and `import`.

- `abi-contract` identifies the compiled C ABI/build-fact surface.
- `producer-link-contract` is `none` for archives and an exact producer-link
  fingerprint for shared libraries. It is provenance, not a requirement that a
  consumer repeat the producer's top-level link list.
- `toolchain-contract` is `native` for ordinary native objects. LTO archives
  store an exact compiler/version/target fingerprint because LTO object formats
  are toolchain-specific.

Unknown keys, duplicate scalar keys, duplicate artifact entries, malformed
roles, paths outside `libs/`, malformed hashes, and manifests without artifacts
are rejected. There is no `manifest-version`, format selector, or legacy
fallback. This is one dh-c-owned schema; an incompatible change updates dh-c and
the produced packages together.

The software version declared through `version-*` keys is unrelated. It
versions the program or library being built; `manifest.dh` records a concrete
prebuilt artifact set.

## Compile ABI versus producer link surface

The compile ABI contract and link contract deliberately answer different
questions.

The compile ABI contract includes the normalized target, profile, artifact
kind, C standard, target arch/tune/ABI, compile environment, effective
`COMP_HAS_*`/`COMP_NO_*` build facts, ABI-affecting flags, user defines, and LTO
or PIC state. These values can alter generated code, public layouts, inline
functions, or available declarations.

The producer link contract includes the compiler driver, resolved target,
startup/default-library injection policy, explicit `link=` inputs, link search
paths, raw link arguments, entry point, linker script, and other effective
linker controls. Changing any of these invalidates the local link cache and
changes shared-library provenance.

For example:

```text
--link-stdlib=off --link=msvcrt --link=user32
```

means that the compiler driver must not inject its normal startup/default
libraries, while `msvcrt` and `user32` are explicit link inputs. The exact list
is part of the producer link contract. It is not interpreted as "no runtime
exists", and a prebuilt consumer is not forced to repeat the same top-level
list merely because the producer used it internally.

If explicit libraries provide compile-time capabilities that differ from the
driver defaults, the producer states those facts explicitly, for example:

```text
--link-stdlib=off \
--link=msvcrt \
--link=user32 \
--define=COMP_HAS_LIBC \
--define=COMP_HAS_STDLIB
```

dh-c suppresses its automatic opposite fact when either member of a
`COMP_HAS_*`/`COMP_NO_*` pair is explicitly defined. The resulting fact is part
of the object-cache and prebuilt ABI contract.

A custom runtime may instead expose no libc at all. Its exact libraries and
link arguments still participate in the link contract. Any runtime identity
that is visible across the public ABI should additionally be represented by a
stable `target-abi` value and/or an explicit public define; dh-c does not guess
arbitrary runtime ABIs from library filenames.

## Compiler and runtime families

Native non-LTO C artifacts are not rejected merely because one side used Clang
and the other GCC. They are accepted when the normalized target and ABI
contract match. LTO artifacts require the exact toolchain contract.

GNU libc and musl toolchains normally produce different normalized target
triples (`*-linux-gnu` versus `*-linux-musl`), so their prebuilt directories and
manifest target contracts differ. Explicit `target-abi`, sysroot, and public
build defines provide further separation for custom or mixed runtime ABIs.

## Why the manifest is an inventory

`manifest.dh` describes concrete binary library artifacts. A source-free
`kind=lib` project is header-only and does not invent an empty binary manifest.

A profile directory can contain more than one legitimate library artifact:

- every `kind=lib` profile emits the native static archive and shared library;
- Windows shared builds also emit an import library;
- effective-LTO profiles additionally emit a `.lto` static archive;
- multiple explicitly named library targets may share one target/profile
  directory.

Therefore a single `artifact-kind`/`artifact` pair would be overwritten by the
last successful library build. The repeated inventory preserves all artifacts
that coexist in `libs/`. Each successful library artifact first writes its own
sidecar under the generated `.manifest/` state; `manifest.dh` is regenerated
from valid sidecars whose artifact files still exist. Executables created by
`test`, `sample`, or `example` builds never update it.

## Compatibility checks

A prebuilt artifact is usable only when:

- `manifest.dh` exists and satisfies the fixed schema;
- normalized target matches;
- profile matches;
- the exact selected path is listed with the expected role;
- the compiled ABI contract matches;
- LTO artifacts have an exact toolchain-contract match;
- on Windows, a selected shared library also has its listed and present import
  library with the same contracts.

The producer link fingerprint is retained for provenance and inventory
consistency. It is not compared with the consumer's final executable link
surface.

`prebuilt=auto` skips an incompatible package and falls back to source;
`prebuilt=required` fails; `prebuilt=off` never consumes it.

## Promotion and generic packaging

A prebuilt producer runs:

```sh
dh-c package <profile> --layout=prebuilt
```

The command promotes the generated prebuilt contract:

```text
build/<target>/<profile>/
    manifest.dh
    libs/
```

into:

```text
prebuilt/<target>/<profile>/
```

The default `--layout=install` remains the generic install/runtime staging
contract. It translates library artifacts into `lib/` and runtime shared
objects into `bin/`; therefore it does not copy the prebuilt-only
`manifest.dh`, whose paths intentionally refer to `libs/`.

There is no separate package file inventory yet. SHA-256 remains an explicit
roadmap item for a future verification, publishing, install-ownership, or
package-diff consumer.
