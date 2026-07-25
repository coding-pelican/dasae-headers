# External dependency contract

`dh-c` separates source acquisition, provider build trees, and staged package outputs.

```text
<project>/.dh-c/deps/
  src/<name>/
  build/<target>/<profile>/<name>/
  packages/<target>/<profile>/<name>/
  lock.dh
```

A dependency section may declare:

```ini
[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
```

Supported fields:

- `path=`: local source or package root.
- `source=`: Git URL or local Git repository.
- `revision=`: tag, commit, or branch resolved by fetch/update.
- `provider=dh|cmake|make|custom|prebuilt`.
- `build-command=` and `install-command=` for custom or overridden make workflows.

Commands:

```sh
dh-c status
dh-c fetch
dh-c update
dh-c deps [profile] [build options]
```

`fetch`, `update`, and `status` operate on the dependency graph declared by the nearest `project.dh`. `fetch` clones missing sources and records resolved commits in `lock.dh`. `update` fetches tags and refs, then re-resolves the requested revision or performs a fast-forward-only pull when no revision is declared.

## Provider execution

### CMake

During `dh-c deps`, CMake dependencies are configured and built out-of-source, then privately installed with `CMAKE_INSTALL_PREFIX` set to the dependency package directory.

Profile mapping:

- `dev`, `test` -> `Debug`
- `stable` -> `RelWithDebInfo`
- `release`, `optimize`, `fast` -> `Release`
- `compact`, `micro` -> `MinSizeRel`

### Make

The default build is `make -C <source>`. Installation is `make -C <source> install PREFIX=<package>`. `build-command=` or `install-command=` overrides the respective command.

### Custom

`build-command=` is required. `install-command=` is optional. Commands execute from the source directory with these environment variables:

```text
DH_DEP_SOURCE
DH_DEP_BUILD
DH_DEP_PACKAGE
DH_DEP_PROFILE
DH_DEP_TARGET
```

### Prebuilt

`path=` identifies an existing package root. Build/install validates that the directory exists and performs no compilation.

### dh

Fetched `provider=dh` projects continue to participate in the existing `dh-c deps` graph. The existing `dh-c deps` graph remains the single dependency-build entry point; provider-specific build/install verbs are not exposed as a second CLI hierarchy.

The lock file represents resolved source state; `project.dh` remains the requested-state contract.

## Runtime exports

External providers may declare package-relative runtime files in their `project.dh`
dependency block:

```ini
[SDL]
provider=cmake
source=https://github.com/libsdl-org/SDL.git
revision=...
runtime-file=bin/SDL3.dll
```

`runtime-file` may be repeated. During `dh-c package`, declared files are copied
from the dependency's private package root into the current project's `bin/`
directory. A missing declared runtime file is an error. When no runtime export is
declared, the conventional dependency `bin/` tree is staged for compatibility.
External providers do not participate in the recursive `provider=dh` source build
graph.
