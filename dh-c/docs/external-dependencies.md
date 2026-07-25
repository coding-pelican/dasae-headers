# External dependency contract

`dh-c` separates immutable source acquisition from build and package outputs.

```text
<project>/.dh-c/deps/
  src/<name>/                 fetched source checkout
  build/<target>/<profile>/   provider build trees (reserved contract)
  packages/<target>/<profile>/ staged/installable provider outputs (reserved contract)
  lock.dh                     resolved Git revisions
```

A dependency section may declare:

```ini
[SDL]
source=https://github.com/libsdl-org/SDL.git
revision=release-3.2.0
provider=cmake
```

Supported contract fields:

- `path=`: existing local source tree. It remains the direct local dependency form.
- `source=`: external Git source URL or local Git repository path.
- `revision=`: tag, commit, or branch to resolve. `fetch` and `update` check it out detached.
- `provider=dh|cmake|make|custom|prebuilt`: build-provider ownership. Source management accepts every provider; provider build execution is implemented incrementally.
- `build-command=` and `install-command=`: reserved custom-provider commands.

Commands:

```sh
dh-c deps status
dh-c deps fetch
dh-c deps update
```

`fetch` clones missing sources and resolves declared revisions. `update` fetches tags and refs, then either resolves the declared revision or performs a fast-forward-only pull when no revision is declared. Successful fetch/update writes `.dh-c/deps/lock.dh` with the resolved commit for every external dependency.

A dependency with `source=` and no `path=` is exposed to the existing dependency graph through `.dh-c/deps/src/<name>`. Therefore a fetched `provider=dh` project can immediately enter the existing `dh-c deps` source build path.

The lock file records resolved state; `project.dh` remains the requested-state contract. Moving revisions should be used deliberately. Immutable commit IDs or release tags are recommended for reproducible builds.
