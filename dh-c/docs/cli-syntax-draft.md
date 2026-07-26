# `dh-c` CLI Syntax Draft

## Goal
The question is not whether a token is long, short, abbreviated, or symbolic.

The real question is:

1. what meaning does the user substitute in their head when they read the token?
2. is that substituted meaning stable across commands?
3. does the runtime behavior match that expectation?

Natural CLI syntax comes from stable substitution meaning, not from forcing
everything into long English words.

That means all of these can be good tokens if their substituted meaning is
stable:

- `recur`
- `.`
- `@`
- `dsl`
- `dh`

And all of these can be bad tokens if their meaning shifts by context.

## Core Principle

Every token in `dh-c` should belong to one of these roles:

- verb
- selector
- modifier
- path
- symbolic shorthand

The role determines how the token is read.

### Verb

A verb names the action.

Examples:

- `build`
- `run`
- `test`
- `clean`

When a user reads:

```sh
dh-c build
```

they should substitute:

```txt
build the default project target
```

### Selector

A selector narrows the target family without replacing the verb.

Examples:

- `--sample`
- `--example`
- `--test`

When a user reads:

```sh
dh-c build --sample foo.c
```

they should substitute:

```txt
build the sample target `foo.c`
```

This is why `sample` / `example` / `test` read more naturally as flags than as
positional nouns in `dh-c`.

The verb is already present. The selector refines the target family.

### Modifier

A modifier changes how the action is applied without changing what the main
action is.

Examples:

- `--recur`
- `--self`
- `--dsl`
- `--lib`
- `--static`
- `--shared`
- `--debug`
- `--verbose`
- `--show-commands`
- `--cache`
- `--link-dsl=off`
- `--exclude`

When a user reads:

```sh
dh-c clean --recur
```

they should substitute:

```txt
clean recursively
```

The fact that `recur` is abbreviated is not itself a problem.
The problem would be if `recur` meant different things in different command
families.

Some modifiers refine boundary or artifact kind rather than traversal:

- `build --self` -> build the self boundary
- `build --dsl` -> build and include the DSL boundary
- `build --lib` -> build this target as a library
- `build --lib --static` -> build this target as a static library
- `build --lib --shared` -> build this target as a shared library

### Path

A path is the concrete object of the action.

Examples:

- `src/main.c`
- `cmd/runner1`
- `tests/test-parser.c`
- `.hidden-src`

When a user reads:

```sh
dh-c run cmd/runner1
```

they should substitute:

```txt
build and run the target at `cmd/runner1`
```

### Symbolic shorthand

A symbol is acceptable if its substituted meaning is obvious and stable.

Examples:

- `.`
- future `@...` forms, if introduced

When a user reads:

```sh
dh-c build .
```

they should substitute something stable such as:

```txt
build the current default selection
```

If `.` means "all project sources" in one place, "current directory" in another,
and "current file" somewhere else, then the problem is not that `.` is a symbol.
The problem is unstable substitution meaning.

The same rule applies to any future `@` syntax.
If `@` is introduced, its grammar and substituted meaning must be singular and
stable.

## Canonical Command Reading

### Default project actions

```sh
dh-c build
dh-c run
dh-c test
dh-c clean
```

These should read as:

- build the default project target
- build and run the default project target
- build and run the default project tests
- clean the default project outputs

### Explicit path actions

```sh
dh-c build src/main.c
dh-c run cmd/runner1
dh-c test tests/test-parser.c
```

These should read as:

- build this file
- run this target path
- test this explicit test path

### Selector-based actions

```sh
dh-c build --sample sample-basic.c
dh-c run --example color-demo.c
dh-c build --test test-parser.c
dh-c test test-parser.c
```

These should read as:

- build the sample target `sample-basic.c`
- run the example target `color-demo.c`
- build the test-family target `test-parser.c`
- test the explicit test target `test-parser.c`

This is the correct form for category selectors.

It is more natural than trying to force category words into positional subject
slots such as:

```txt
dh-c test test test-parser.c
```

which is invalid as a language pattern for this CLI.

## Stability Rules

### 1. One token, one substituted meaning

A token should not change its core substituted meaning by verb unless the new
meaning is obviously the same semantic operation.

Good examples:

- `--recur` always means "apply this command recursively"
- `--dsl` means "include the DSL boundary in this command"

For `--dsl`, the runtime order may differ by command graph, but the substituted
meaning stays the same:

- `dh-c build --dsl` -> build the project and include the DSL boundary
- `dh-c test --dsl` -> test the project and include the DSL boundary
- `dh-c clean --dsl` -> clean the project and include the DSL boundary

If the dependency graph forces DSL work to happen first, that is a natural
execution consequence, not a semantic mismatch.

### 2. Selectors should stay selectors

`--sample`, `--example`, and `--test` should consistently behave as family
selectors, not as unrelated special modes.

### 3. Explicit paths should stay explicit

If the user explicitly names a file or directory, `dh-c` should treat that as
direct intent rather than replacing it with an unrelated auto-discovery result.

This must remain true even for hidden paths:

```sh
dh-c build .hidden/main.c
dh-c build .hidden-src
```

### 4. Shorthand may exist

Abbreviation is allowed.

Examples:

- `recur`
- `dsl`
- `dh`

The acceptance criterion is not spelling length.
The acceptance criterion is whether the user can reliably substitute the same
meaning every time they see the token.

## Target Semantic Roles

This section records the intended semantic roles, even when the current surface
still contains historical command forms that should be collapsed later.

### Verbs

- `build`
- `run`
- `test`
- `clean`
- `deps`
- `help`
- `version`

### Selector flags

- `--sample`
- `--example`
- `--test`
- `--all`

### Boundary modifiers

- `--self`
- `--dsl`
- `--recur`
- `--cache`
- `--link-dsl=off`

### Artifact modifiers

- `--lib`
- `--static`
- `--shared`

### Execution and output modifiers

- `--debug`
- `--verbose`
- `--show-commands`

### Low-level build modifiers

- `--compiler`
- `--std`
- `--arch`
- `--target`
- `--sysroot`
- `--include`
- `--isystem`
- `--link`
- `--define`
- `--undef`
- `--compiler-args`
- `--runtime-args`
- `--args`

### Path-like modifiers

- `--output`
- `--output-ext`
- `--exclude`
- `--dh`
- `--dh-file`

For build-like targets, `--output` is an output stem or directory. Target
extensions are generated from the selected target kind. For compilation database
commands, `--output` remains the compilation database file path.
For single-artifact build targets, `--output-ext=<.ext>` overrides the generated
extension without falling back to raw compiler/linker output naming.

## Specific Direction For Existing Tokens

### `recur`

`recur` is acceptable if it always means:

```txt
apply the current command recursively
```

No rename is needed purely because it is abbreviated.

### `.`

`.` is acceptable if it always means one stable thing in the grammar.

Preferred rule:

```txt
the current default selection in the current project context
```

It must not silently drift between unrelated meanings.

### `--sample`, `--example`, `--test`

These should remain selector flags.

Canonical examples should prefer:

```sh
dh-c build --sample sample-basic.c
dh-c run --example color-demo.c
dh-c build --test test-parser.c
```

### `--args`

`--args` is too context-sensitive to be a strong canonical surface.

Preferred canonical surface:

- `--compiler-args "..."`
- `--runtime-args "..."`

`--args` may remain as a compatibility alias if needed.

### `--dsl`

`--dsl` is acceptable as a token.

Its substituted meaning should be:

```txt
include the DSL boundary in the current command
```

That means:

- `build --dsl` expands the build boundary to include DSL work
- `test --dsl` expands the test boundary to include DSL work
- `clean --dsl` expands the clean boundary to include DSL work

The execution order may differ because the dependency graph differs.
That is expected and should not be described as a semantic inconsistency.

### `--self`

`--self` should be read as a boundary modifier:

```txt
apply the current command to the self boundary
```

That means:

- `build --self` -> build self
- `clean --self` -> clean self
- `test --self` -> test self, if that action is supported

The important point is that the base verb stays visible.
`self` should refine the command boundary rather than replace the verb.

### `--recur`

`--recur` should be read as a traversal modifier:

```txt
apply this command recursively across the relevant project or dependency graph
```

It combines naturally with `--dsl`:

- `build --recur --dsl`
- `test --recur --dsl`
- `clean --recur --dsl`

In all three cases the user expectation is:

- keep the same base verb
- traverse recursively
- include the DSL boundary

That semantic model is stable.

### `--lib`

`--lib` should be read as an artifact-kind modifier:

```txt
build this target as a library
```

That means the canonical reading is:

- `build --lib foo.c`
- `build --lib --static foo.c`
- `build --lib --shared foo.c`

The base action is still `build`.
`--lib` changes the output artifact kind, not the verb.

### `--static`, `--shared`, `--dynamic`

`--static` and `--shared` should be read as library-kind refinements under
`--lib`.

Their substituted meanings should be:

```txt
--static -> make the library static
--shared -> make the library shared
```

So the natural command family is:

- `build --lib --static ...`
- `build --lib --shared ...`

`dynamic` should not be the main canonical word on the CLI surface when what
the user means is "shared library".

If `--dynamic` exists, it should be treated as a compatibility alias for
`--shared`, not as the preferred semantic surface.

## Semantic Debt To Remove

These conflicts should be written down explicitly so they do not get forgotten
between chats:

- `build-self` and `clean-self` duplicate the cleaner `build --self` /
 `clean --self` boundary-modifier model
- `build-dsl` / `test-dsl` / `clean-dsl` duplicate the cleaner `--dsl`
 boundary-modifier model
- `lib` as a verb conflicts with `--lib` as the more natural artifact modifier
- `--lib=<name>` conflicts with the more important meaning of `--lib` as
 "build as library"; that naming surface should be removed or renamed
- `--dynamic` is weaker than `--shared` as a canonical user-facing token

## Help And Version Forms

`help` and `version` are first-class query actions.

All of these should be supported:

```sh
dh-c help
dh-c version
dh-c --help
dh-c --version
dh-c -h
dh-c -v
```

They should be treated as equivalent query surfaces, not as separate feature
tiers.

## Exclude Contract

## CLI

`--exclude <path>` is a modifier.

It should read as:

```txt
do this action, but exclude this path
```

Repeatable examples:

```sh
dh-c build --exclude src/generated
dh-c build cmd/runner1 --exclude cmd/runner1/tmp
dh-c test --exclude tests/slow
dh-c build --sample sample-basic.c --exclude samples/archive
```

Meaning:

- if the excluded path is a file, exclude that file
- if the excluded path is a directory, exclude the whole subtree
- CLI excludes are normalized before matching

### Conflict rule

If the user explicitly names a file target and also excludes that same file or
one of its ancestors, `dh-c` should reject the command early.

Example:

```sh
dh-c build src/generated/foo.c --exclude src/generated
```

Expected result:

- error: explicit input conflicts with exclude contract

## `project.dh`

Project-level exclusion is repeatable:

```txt
exclude=src/generated
exclude=src/legacy/old-api.c
exclude=tests/slow
```

Meaning:

- resolve relative to project root
- file exclude means exact file
- directory exclude means subtree

Target-root-local exclusion is also repeatable:

```txt
[target-root cmd]
path=cmd
kind=executable
selection=dir
exclude=common/archive
exclude=old
```

Meaning:

- inside `[target-root ...]`, `exclude=` is relative to that target root's `path=`
- it applies only while resolving that target family

### Exclude precedence

Recommended order:

1. explicit CLI conflict check
2. CLI `--exclude`
3. target-root `exclude=`
4. project-level `exclude=`
5. built-in automatic hidden-path skip

## Hidden Paths

Automatic recursive collection should skip path segments that start with `.`.

Examples:

- `.git`
- `.cache`
- `.archive`
- `.draft`

This automatic rule must not override explicit user intent.

These remain valid unless an explicit exclude contract rejects them:

```sh
dh-c build .hidden/main.c
dh-c build .hidden-src
```

## Help And Examples

Help should show examples according to token role.

Good examples:

```txt
dh-c build
dh-c build src/main.c
dh-c build --sample sample-basic.c
dh-c run --example color-demo.c
dh-c build --recur --dsl
dh-c clean --recur
dh-c build .
dh-c help
dh-c --help
dh-c -h
dh-c build --exclude src/generated
```

Bad examples are ones that force selector words into positional repetition or
hide the real substituted meaning of a token.

## Project state, cache, package, and install commands

The current project remains the implicit subject. Cache is not a command namespace.

Canonical forms:

```sh
dh-c status [profile]
dh-c clean [profile]
dh-c clean --cache [--older-than=<duration>] [--dry-run]
dh-c clean --deps [--unused] [--older-than=<duration>] [--dry-run] [--force]
dh-c explain rebuild [profile]
dh-c package [profile]
dh-c install [profile] --prefix=<path>
```

Non-canonical forms such as `dh-c cache status`, `dh-c cache clean`, and
`dh-c cache explain` are rejected. `status` reports dependency, build, cache,
and package state. `clean` owns generated state. `explain rebuild` explains
why the current build graph is stale.

Cleanup scopes are explicit:

- no scope: materialized build outputs, generated dependency exports, and the active build cache
- `--cache`: cache only; it must not delete materialized build outputs
- `--deps`: fetched source/provider-build/staged-package dependency state only
- `--unused`: further restrict dependency cleanup to names absent from the current `project.dh`
- `--older-than`: further restrict the selected cache or dependency scope by age
- `--dry-run`: report the exact removals without changing the filesystem
- `--force`: permit deletion of Git dependency checkouts containing genuine user changes; dh-c-generated untracked state is ignored

`clean --deps` never rewrites the sibling `lock.dh`. A resolved input contract is
not generated cache state.
