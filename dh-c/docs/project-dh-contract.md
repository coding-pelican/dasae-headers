# `project.dh` Contract

## Goal
`project.dh` declares two different things:

- project-wide build defaults and reusable self code
- named target roots that describe where buildable targets live

The important distinction is:

- self roots describe code that belongs to the project itself and is reused internally
- target roots describe entry targets or artifact-producing targets such as `cmd`, `examples`, or `plugins`

## Explicit Rules

### Project-wide keys

Supported project-level keys:

```txt
output=<name>
build-runs-tests=<true|false>
no-dsl=<true|false>
pch=<auto|off|path>
pch-exclude=<header>
self-root=<path>
```

Meaning:

- `output=` sets the default output name when the CLI does not provide one
- `build-runs-tests=` makes plain `dh-c build` run tests afterward
- `no-dsl=` disables automatic `dh` linking for that project or target
- `pch=` and `pch-exclude=` control the PCH contract
- repeated `self-root=` entries declare reusable self-project source roots

### Target-root blocks

Target roots are declared with:

```txt
[target-root <name>]
path=<path>
kind=<executable|static-lib|shared-lib>
selection=<path|file|dir>
link-self=<true|false>
```

Meaning:

- `<name>` is a semantic label, not an artifact kind
- `path=` is the root directory for that target family
- `kind=` chooses the produced artifact type
- `selection=` controls whether the CLI target under that root may be a file, a directory, or either
- `link-self=` controls whether the target links the cached self-project reusable unit

## Implicit Rules

These defaults currently apply when the contract does not say otherwise.

### Automatic source collection filters

When `dh-c` collects sources automatically from project roots or built-in target
families, it skips path segments that start with `.`.

That means hidden directories or files such as `.git`, `.cache`, `.archive`,
or `.local.c` are ignored during automatic recursive collection.

This automatic filter does not override explicit CLI intent:

- explicit file inputs are still accepted
- explicit directory inputs are still traversed as requested
- explicit target-root paths are still traversed as requested

### Self roots

If no `self-root=` is declared, `dh-c` uses the resolved project `src` root as the only self root.

That means this:

```txt
# no self-root entries
```

is treated like:

```txt
self-root=src
```

after directory-alias resolution.

### Built-in target roots

If you do not declare target roots for `samples`, `examples`, or `tests`, `dh-c` still creates built-in roots for those categories so these compatibility shorthands keep working:

- `--sample`
- `--example`
- `--test`

Those built-in roots inherit the resolved project category directory names, including supported aliases such as `example/` and `sample/`.

### Target-root block defaults

Inside a `[target-root ...]` block:

- `kind=` defaults to `executable`
- `selection=` defaults to `path`
- `link-self=` defaults to `true`

Only `path=` is effectively mandatory for a meaningful target root.

### Output naming defaults

When the CLI does not provide `--output` and no companion `.dh` overrides it:

- plain project build defaults to project name
- file target defaults to file basename without extension
- directory target defaults to directory basename

Examples:

- `build cmd/runner1` -> output name `runner1`
- `build plugins/render` with `kind=shared-lib` -> output name `render`
- `build src/main.c` -> output name `main`

## Resolution Rules

### Plain build without target root

These keep their direct meaning:

- `dh-c build`
- `dh-c build foo.c`
- `dh-c run foo.c`
- `dh-c test`

They operate on plain project sources or explicit source files, not on declared target-root contracts.

### Build through target roots

When the given path falls under a declared target root, the target-root contract applies.

Example:

```txt
self-root=src
self-root=pkg

[target-root cmd]
path=cmd
kind=executable
selection=dir
link-self=true
```

Then:

```sh
dh-c build cmd/runner1
dh-c run cmd/runner1
```

means:

- compile only sources under `cmd/runner1`
- reuse cached self sources from `src` and `pkg`
- link the self unit because `link-self=true`

### Compatibility shorthands

These are still accepted:

```sh
dh-c build --example example-color.c
dh-c run --sample sample-basic.c
dh-c build --test test-parser.c
```

They are compatibility shorthands for the built-in `examples`, `samples`, and `tests` target roots.

For new project-local architecture, prefer explicit `[target-root ...]` blocks over relying on those flags alone.

## Conflict Rules

`dh-c` rejects these early:

- duplicate target-root names
- duplicate target-root paths
- invalid `kind=`
- invalid `selection=`
- multiple alias directories for the same built-in category at the same project level

## Examples

### Folder-based executable targets

```txt
self-root=src
self-root=internal

[target-root cmd]
path=cmd
kind=executable
selection=dir
link-self=true
```

### Shared-library plugin targets

```txt
self-root=src

[target-root plugins]
path=plugins
kind=shared-lib
selection=dir
link-self=true
```

### Minimal project using only implicit defaults

```txt
output=my-project
```

This still implies:

- self root: resolved `src`
- built-in compatibility roots: resolved `samples`, `examples`, `tests`

## Companion `.dh` Files

Companion `.dh` files still apply to the owning build target and can override:

- `output=`
- `build-runs-tests=`
- `no-dsl=`

Dependency blocks in `project.dh` may additionally set:

```txt
test=true
```

to run that dependency's tests during dependency traversal.
