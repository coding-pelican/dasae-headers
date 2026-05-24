# daterm

`daterm` is a concrete terminal context layer for DH-C.

It is not an ANSI sequence library. `dansi` owns ANSI/xterm byte protocol
construction and parsing. `daterm` owns terminal objects, backend context,
terminal lifecycle policy, and the `io_Reader`/`io_Writer` contracts through
which application code uses `dansi`.

The intended shape is:

```txt
application
  |
  | calls dansi_*Write / dansi_*fetch through reader-writer contracts
  v
daterm_Term
  |
  | exposes io_Reader, io_Writer, poll/wait
  v
backend context
  |
  | pass-through, intercept, parse, or native dispatch
  v
OS terminal / virtual terminal / user implementation
```

## Boundary

`daterm` should not duplicate semantic drawing APIs that already exist as
`dansi` protocol primitives.

For example, application code should write terminal protocol like this:

```c
let out = daterm_Term_writer(term);

try_(dansi_screen_clearWrite(out));
try_(dansi_cursor_moveToWrite(1, 1, out));
try_(dansi_color_fg24bitWrite(255, 220, 80, out));
try_(io_Writer_writeBytes(out, u8_l("ready")));
try_(dansi_attr_resetWrite(out));
```

`daterm` provides the concrete writer. It does not need separate
`daterm_Term_clearScreen`, `daterm_Term_moveTo`, or `daterm_Term_setFg`
wrappers.

The same rule applies to queries. If a query can be expressed as a `dansi`
request/receive/parse/fetch operation over `io_Writer` and `io_Reader`, then a
backend can support it by intercepting that byte protocol. A separate
`Term_VTbl` query method is only justified when the contract cannot be modeled
through the reader/writer protocol.

## Package Shape

```txt
include/
  daterm.h              package umbrella
  daterm-runtime.h      backend-neutral runtime umbrella
  daterm-context.h      concrete context umbrella

include/daterm-runtime/
  base.h                shared value types
  Event.h               backend-neutral events
  Term.h                terminal object interface

include/daterm-context/
  ANSI.h                current OS terminal context
  ANSI/private.h        internal/test-only ANSI helpers
  Virt.h                disabled virtual-terminal draft
```

`Virt` is currently not an active public context. A virtual backend should be
reintroduced only after its role is narrowed: either a terminal implementation
that parses `dansi` byte streams, or a focused test backend.

## Runtime Types

`daterm_Term` is a lightweight object handle:

```c
typedef struct daterm_Term {
    P$raw ctx;
    const daterm_Term_VTbl* vtbl;
} daterm_Term;
```

Current runtime operations are:

- `daterm_Term_reader`
- `daterm_Term_writer`
- `daterm_Term_poll`
- `daterm_Term_wait`
- `daterm_Term_timedWait`
- screen/cursor query functions currently present in the interface

The query functions are transitional. The preferred direction is to let `dansi`
query primitives run through `daterm_Term_reader` and `daterm_Term_writer`, so a
backend can pass through to ANSI, answer from a native OS API, or answer from a
virtual state model without exposing a separate semantic query surface.

## ANSI Context

`daterm_ANSI` represents the current process terminal context.

It owns:

- input and output `fs_File` handles
- an input `io_Buf_Reader`
- report buffer storage
- raw mode state
- mouse tracking state
- output mode policy
- ESC timeout policy for ambiguous input

Configuration is explicit:

```c
var cfg = daterm_ANSI_Cfg_default(gpa);
cfg.input_file = io_getStdIn();
cfg.output_file = io_getStdOut();
cfg.output_mode = daterm_ANSI_OutputMode_processed;
cfg.esc_timeout = daterm_ANSI_esc_timeout_default;

var ansi = try_(daterm_ANSI_init(cfg));
defer_(daterm_ANSI_fini(&ansi));
```

The default configuration uses owned input/report buffers. Fixed caller-owned
buffers can be supplied through `daterm_ANSI_Cfg_input_buf_fixed` and
`daterm_ANSI_Cfg_report_buf_fixed` when allocation is not desired.

## Lifecycle Policy

Lifecycle policy should be explicit and conservative.

Current primitive responsibilities:

- `daterm_ANSI_enableRawMode`
- `daterm_ANSI_disableRawMode`
- `daterm_ANSI_enableMouseTracking`
- `daterm_ANSI_disableMouseTracking`
- `daterm_ANSI_fini`

`fini` must clean up state that the context owns. If raw mode or mouse tracking
was enabled through `daterm_ANSI`, it must be disabled through `daterm_ANSI`.

Broad "enter/leave" helpers are only useful as fixed sugar for a known common
combination. If they grow options, they become lifecycle policy APIs and should
be replaced by explicit primitive calls or a dedicated lifecycle configuration
type.

Alternate screen is the same kind of policy decision. `dansi` provides
`dansi_screen_enterAlternateWrite` and `dansi_screen_exitAlternateWrite`.
`daterm` should track alternate screen only if the `Term` abstraction itself
owns that state and can guarantee cleanup. Otherwise it should leave alternate
screen as an explicit `dansi` protocol operation chosen by the caller.

## Input And Events

`daterm_Term_poll` is non-blocking. Backends must not block a frame loop while
trying to parse input.

For ANSI streams this requires:

- checking buffered input before OS readiness
- reading only when the OS says input is ready
- preserving partial escape sequences
- distinguishing standalone ESC from a longer sequence with a configurable
  timeout

Core key parsing comes from `dansi_Event`. xterm mouse parsing comes from
`dansi_mouse_parseSGR`. `daterm_Event` wraps these into backend-neutral event
variants:

- `daterm_Event_key`
- `daterm_Event_mouse`
- `daterm_Event_resize`

Resize events are a backend concern. ANSI streams may not provide a native
resize event, while an OS backend may be able to detect one directly.

## Query Path

Preferred query flow:

```c
let out = daterm_Term_writer(term);
let in = daterm_Term_reader(term);

var_(buf, dansi_screen_SizeReportBuf) $undefined;
let size = try_(dansi_screen_fetchTextAreaSizeChars(out, in, A_ref$((S$u8)(buf))));
```

An OS-backed `daterm_ANSI` writer/reader may pass the request to the terminal and
read the report. A backend with a native answer may intercept the request bytes
and synthesize the expected response bytes. A virtual backend may parse the same
request bytes and answer from its in-memory terminal state.

That keeps application code protocol-oriented and keeps backend specialization
behind the `io_Reader`/`io_Writer` boundary.

## Output Newlines

`io_Writer_nl` is a logical LF byte, not terminal cursor movement. Terminal
layout code should use `dansi_cursor_moveNextLineWrite`,
`dansi_cursor_moveToWrite`, `dansi_line_clearWrite`, and related protocol
primitives when cursor position matters.

This matters in raw mode and processed-output mode because `\n` and terminal
"next line" are not the same contract.

## PTY Scope

PTY/ConPTY hosting is out of scope for `daterm`.

A future cross-platform pseudo-terminal abstraction should live in a separate
package such as `dapty`. That package would own child process spawning and
master-side PTY/ConPTY IO. `daterm` should remain focused on current terminal
objects and backend-neutral terminal contracts.

## Verification

Tests live under `tests/test-*.c`.

```sh
dh-c test
```

From a dependent package, use recursive tests when the dependent package should
also verify `dansi` and `daterm`:

```sh
dh-c test --recur
```

The active ANSI tests cover configuration defaults, buffered sequence polling,
split CSI handling, ESC timeout behavior, and platform-gated raw output mode
behavior.
