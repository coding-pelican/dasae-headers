# dh-c Build Contract Map

## Normalized Intent
`dh-c` now lowers command payloads through `dal_c_CommandIntent`.

Owner:
- `dh-c/src/dal-c/Cmd.c`
- `dh-c/src/dal-c/internal.h`

Normalized fields:
- action
- target file
- output path override
- run args
- sample/example/test context
- build-all
- recursive
- debug
- dsl-first
- cache-only
- library linking

Cross-module rule:
- `Cmd.c` owns command-surface interpretation.
- `build.c` consumes normalized intent and must not reinterpret raw payload unions.

## Artifact And Output Contract
Owner:
- `dh-c/src/dal-c/build.c`

Rules:
- profile root remains `build/<profile>/`
- executable defaults are context-scoped:
  - `build/<profile>/samples/...`
  - `build/<profile>/examples/...`
  - `build/<profile>/tests/...`
  - `build/<profile>/targets/...`
- static and shared libraries stay at `build/<profile>/`
- explicit output overrides still win

Why:
- build/run now share the same output-path policy
- sample/example/test artifacts no longer collide with main executable names

## Makefile And Stale-Detection Contract
Owner:
- `dh-c/src/dal-c/build.c`

Rules:
- Makefiles are plan-scoped under `build/<profile>/.plans/<context>/`
- object files are shared under `build/<profile>/obj/`
- generated makefiles are rewritten only when content changes
- generated unity/test-runner sources are rewritten only when content changes
- object paths are keyed by compile-contract hash, not by the active plan file
- plan files do not own object freshness; source dependencies do

Compile-contract hash inputs:
- profile
- compiler and compile options
- include and define sets
- `compiler_args`
- PCH usage
- test-mode usage
- third-party warning mode

Why:
- switching between `sample`, `example`, `test`, `run`, or dependency builds no longer invalidates unrelated objects
- repeated `run` reaches `make: Nothing to be done for 'all'.`

## Common Source Reuse Contract
Owner:
- `dh-c/src/dal-c/Cmd.c`
- `dh-c/src/dal-c/build.c`

Rules:
- sample/example/test targets do not directly append project `src/` into the target plan
- non-`dh` projects first build a project static library from `src/`
- sample/example/test targets then link that library plus only their context-specific sources
- `dh` self-project sample/test paths link `dh.lib` directly and skip a second local-project-lib path

Why:
- context targets rebuild only their own sources
- project core code is reused through the cached library artifact

## PCH Policy
Owner:
- `dh-c/src/dal-c/Project.c`
- `dh-c/src/dal-c/build.c`
- project `project.dh`

Rules:
- `pch=auto` means detect by project convention
- `pch=off` disables PCH
- `pch=<path>` pins an explicit header
- `pch-exclude=<header>` declares headers that force no-PCH translation units when included by a source
- `dh/project.dh` declares:
  - `pch=dh.h`
  - `pch-exclude=dh-main.h`
  - `pch-exclude=dh-TEST-main.h`

Why:
- PCH exclusion is now project policy, not hardcoded source-name heuristics
- `dh` self-build uses the same PCH framework as other projects

## libdh And Self-Build Contract
Owner:
- `dh-c/src/dal-c/build.c`

Rules:
- `dal_c__ensureLibDH()` detects the real `dh` project instead of constructing an ad hoc temporary contract
- self-build uses the same plan-scoped makefile generation
- self-build uses the same object cache and PCH policy as normal builds

## Command Surface Matrix
Owner:
- `dh-c/include/dal-c.h`
- `dh-c/src/dal-c/Cmd.c`

Current intended surfaces:
- `build`: executable build, direct source build, aggregate source build, sample/example/test contexts, `--dsl`, `--recur`
- `run`: same artifact policy as `build`, then execute/debug
- `test`: test runner or standalone test build, then execute/debug
- `build-dsl`: build `dh` only
- `test-dsl`: build/run `dh/tests` directly
- `clean`: build/cache cleanup with optional `--dsl` and `--recur`
- `clean-dsl`: DSL-only cleanup, `--cache` only

Audited fix:
- `clean-dsl` help no longer advertises `clean`-only options

## Removed Contradictions
- one profile-wide Makefile being overwritten by multiple command contexts
- object freshness depending on Makefile timestamp churn
- hardcoded `dh-main.h` / `dh-TEST-main.h` no-PCH exceptions in core build logic
- `Cmd.c` and `build.c` each owning their own cross-module command interpretation
- sample/example/test plans recompiling project `src/` on every context build
