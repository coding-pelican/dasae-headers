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
dh-c deps status
dh-c deps fetch
dh-c deps update
dh-c deps build [profile] [build options]
dh-c deps install [profile] [build options]
```

`fetch` clones missing sources and records resolved commits in `lock.dh`. `update` fetches tags and refs, then checks out the requested revision or performs a fast-forward-only pull when no revision is declared.

## Provider execution

### CMake

`deps build` configures and builds an out-of-source tree. `deps install` additionally runs `cmake --install` with `CMAKE_INSTALL_PREFIX` set to the dependency package directory.

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

Fetched `provider=dh` projects continue to participate in the existing `dh-c deps` graph. Direct provider execution through `deps build/install` is intentionally not duplicated yet.

The lock file represents resolved source state; `project.dh` remains the requested-state contract.
