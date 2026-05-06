# dh-c Hardening Work

## Phase 1. Baseline and Plan

### Activity 1.1. Inspect current behavior

#### Step 1.1.1. Map codebase and risks

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P1-A1-S1-T1 | Step 1.1.1 | none | `dh-c/{include,src,tests,docs}` | current-state inventory | supported commands, module sizes, testing gaps, and obvious risks identified | done |
| P1-A1-S1-T2 | Step 1.1.1 | P1-A1-S1-T1 | current implementation | execution strategy | refactor, test, memory, and docs order defined | done |

## Phase 2. Code Hardening

### Activity 2.1. Reduce duplication and brittle hardcoded logic

#### Step 2.1.1. Extract repeated metadata and helpers

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P2-A1-S1-T1 | Step 2.1.1 | P1-A1-S1-T2 | `dal-c.h`, `Cmd.c`, `Project.c`, `build.c` | smaller, clearer command metadata and helper layer | repeated mapping/help/array logic removed or centralized without inflating codebase | done |
| P2-A1-S1-T2 | Step 2.1.1 | P2-A1-S1-T1 | `dal-c-ext/*` | hardened utility implementations | edge-case bugs and unsafe memory handling removed | done |

### Activity 2.2. Align user-facing command surface with reality

#### Step 2.2.1. Resolve scaffold command gap

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P2-A2-S1-T1 | Step 2.2.1 | P1-A1-S1-T2 | current `workspace`/`project` behavior | implemented behavior or explicit contract reduction | help/docs/tests match actual supported commands | done |

## Phase 3. Verification

### Activity 3.1. Exhaustive automated tests

#### Step 3.1.1. Build unit and scenario suites

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P3-A1-S1-T1 | Step 3.1.1 | P2-A1-S1-T1, P2-A1-S1-T2, P2-A2-S1-T1 | full `dh-c` feature surface | unit test suite | each utility/helper contract has direct automated coverage | done |
| P3-A1-S1-T2 | Step 3.1.1 | P3-A1-S1-T1 | CLI commands and lab fixtures | scenario/regression suite | build/run/test/help flows and discovered bugs are encoded as tests | done |

### Activity 3.2. Memory integrity

#### Step 3.2.1. Validate dynamic behavior

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P3-A2-S1-T1 | Step 3.2.1 | P3-A1-S1-T1, P3-A1-S1-T2 | compiled tests and CLI flows | sanitizer-backed verification | invalid access and leaks are either absent or fixed with regression coverage | done |

## Phase 4. User Documentation

### Activity 4.1. Rebuild help and build guide

#### Step 4.1.1. Publish accurate, complete guidance

| task_id | parent_step | depends_on | input | deliverable | acceptance_criteria | status |
| --- | --- | --- | --- | --- | --- | --- |
| P4-A1-S1-T1 | Step 4.1.1 | P3-A2-S1-T1 | final supported behavior | improved `dh-c --help` | command help covers every supported path, option, and constraint accurately | done |
| P4-A1-S1-T2 | Step 4.1.1 | P4-A1-S1-T1 | final supported behavior and preferred doc style | rewritten `BUILD.md` | guide is complete, user-friendly, and consistent with actual runtime behavior | done |
