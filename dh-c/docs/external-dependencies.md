# External dependency contract

`dh-c` separates requested dependency state, resolved dependency state, mutable
source/provider state, and staged package output.

```text
<project>/
  project.dh
  lock.dh
  .dh-c/
    deps/
      src/<name>/
      build/<target>/<profile>/<name>/
      packages/<target>/<profile>/<name>/
      usage/<name>.stamp
```

`project.dh` and `lock.dh` deliberately live at the same scope level:

- `project.dh` contains the requested source, revision, and provider.
- `lock.dh` contains the exact resolved source commit.
- `.dh-c/` contains disposable mutable state and must not own the persistent lock.

A dependency section may declare:

```ini
[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
```

Supported fields:

- `path=`: local source, a single header, or a package root.
- `source=`: Git URL or local Git repository.
- `revision=`: tag, commit, or branch resolved by fetch/update.
- `provider=dh|cmake|make|custom|prebuilt`.
- `build-command=` and `install-command=` for custom or overridden make workflows.

Header-only dependencies remain first-class inputs. A dependency may be a single
header or a directory with public headers and no compilation sources; `dh-c deps`
stages those headers without inventing a binary artifact.

Commands:

```sh
dh-c status
dh-c fetch
dh-c update
dh-c deps [profile] [build options]
```

## Resolution policy

`fetch`, `update`, and `status` operate on dependencies declared by the nearest
`project.dh`.

- `fetch` reuses an existing compatible `lock.dh` entry and checks out its exact
  commit. It resolves and records a new entry only when no entry exists.
- `update` deliberately re-resolves the requested revision and rewrites
  `lock.dh`.
- `status` compares each checkout with `lock.dh` and reports `READY`, `DRIFT`, or
  `UNLOCKED`.
- source provider build/install requires the checkout to match `lock.dh`.
- changing a dependency source or provider invalidates the old entry; use
  `dh-c update` to replace the resolution.

The dependency lock is persistent source state and should normally be committed.
The `.dh-c/` tree is generated state and should be ignored.

Successful `fetch`, `update`, dependency build, and dependency install operations
update `.dh-c/deps/usage/<name>.stamp`. The stamp is disposable generated state;
it exists only so age-based maintenance reflects actual use rather than the age of
a checkout's internal Git files. Older layouts without a stamp fall back to the
newest modification time found in the dependency source/build/package state.

## Generated dependency maintenance

Dependency cleanup remains part of the canonical `clean` command:

```sh
# Preview dependency state that is no longer declared by project.dh.
dh-c clean --deps --unused --dry-run

# Remove undeclared dependency source/build/package state.
dh-c clean --deps --unused

# Remove dependency state not used in the last 90 days.
dh-c clean --deps --older-than=90d

# Apply both filters: only undeclared entries that are also old.
dh-c clean --deps --unused --older-than=90d
```

`--unused` compares generated dependency names with the current `project.dh`.
`--older-than` accepts an integer followed by `s`, `m`, `h`, `d`, or `w`; an
omitted suffix means seconds. When both filters are supplied, both conditions
must match.

Dirty Git checkouts are preserved and make the cleanup command fail visibly.
Use `--force` only when those local changes are intentionally disposable. The
cleanup never rewrites `lock.dh`: generated state and resolved project input are
separate ownership domains. Run `dh-c update` later when the dependency contract
itself should change.

## Workspace and cache scope

A discovered `workspace.dh` is a project-discovery boundary and the preferred
shared build-cache scope:

```text
<workspace>/.dh-c/cache/
```

Projects in that workspace can reuse build/PCH/source-list cache entries. Their
mutable dependency checkouts remain under each owning project:

```text
<project>/.dh-c/deps/
```

This prevents two projects with the same dependency name but different locked
revisions from sharing one mutable checkout. Cross-project/global reuse of
immutable dependency contents can later be content-addressed; it must not be
implemented by aliasing mutable source trees.

When no workspace is found, project builds use their project cache. Ad-hoc builds
without either project or workspace use the user/global dh-c cache.

## Provider execution

### CMake

During `dh-c deps`, CMake dependencies are configured and built out-of-source,
then privately installed with `CMAKE_INSTALL_PREFIX` set to the dependency
package directory.

Profile mapping:

- `dev`, `test` -> `Debug`
- `stable` -> `RelWithDebInfo`
- `release`, `optimize`, `fast` -> `Release`
- `compact`, `micro` -> `MinSizeRel`

### Make

The default build is `make -C <source>`. Installation is
`make -C <source> install PREFIX=<package>`. `build-command=` or
`install-command=` overrides the respective command.

### Custom

`build-command=` is required. `install-command=` is optional. Commands execute
from the source directory with these environment variables:

```text
DH_DEP_SOURCE
DH_DEP_BUILD
DH_DEP_PACKAGE
DH_DEP_PROFILE
DH_DEP_TARGET
```

### Prebuilt

`path=` identifies an existing package root. Build/install validates that the
directory exists and performs no compilation.

### dh

Fetched `provider=dh` projects continue to participate in the existing `dh-c
deps` graph. Provider-specific build/install verbs are not exposed as a second
CLI hierarchy.

## Runtime exports

External providers may declare package-relative runtime files in their
`project.dh` dependency block:

```ini
[SDL]
provider=cmake
source=https://github.com/libsdl-org/SDL.git
revision=...
runtime-file=bin/SDL3.dll
```

`runtime-file` may be repeated. During `dh-c package`, declared files are copied
from the dependency's private package root into the current project's `bin/`
directory. A missing declared runtime file is an error. When no runtime export
is declared, the conventional dependency `bin/` tree is staged. External
providers do not participate in the recursive `provider=dh` source build graph.
