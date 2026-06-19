# dansi reference documents

This file is a link-only reference index for restructuring `dansi` by protocol ownership.

Do not vendor upstream PDFs/HTML into the public repository unless the source explicitly grants redistribution rights and the matching license/notice is committed with the copy. Prefer links, access dates, implementation notes, and tables reconstructed from the specifications.

Access date: 2026-06-18

---

## Ownership rule

- Syntax/framing belongs to `dansi-core`.
- Semantic ownership belongs to the module that owns the protocol.
- Runtime/backend events belong to `daterm`, not `dansi`.

Examples:

- `OSC` framing -> `dansi-core`
- `OSC 8` hyperlink semantics -> `dansi-link`
- `OSC 133` / `OSC 633` shell integration semantics -> `dansi-shell`
- `OSC 1337` iTerm2 semantics -> `dansi-iterm`
- `DCS` framing -> `dansi-core`
- Sixel payload semantics -> `dansi-sixel`
- `CSI` framing -> `dansi-core`
- standard CSI control functions -> `dansi-core`
- DEC private CSI semantics -> `dansi-dec`
- xterm private CSI semantics -> `dansi-xterm`
- Kitty keyboard CSI-u semantics -> `dansi-kitty`

---

## `dansi-core`

Purpose:

- ANSI / ECMA-48 / ISO/IEC 6429 control sequence representation layer
- C0 / C1
- ESC / CSI / OSC / DCS / PM / APC / ST
- parameter / intermediate / final byte structure
- sequence encode/decode
- standard control functions

### Primary specification

#### ECMA-48: Control functions for coded character sets, 5th edition, June 1991

- Official page: <https://ecma-international.org/publications-and-standards/standards/ecma-48/>
- PDF: <https://ecma-international.org/wp-content/uploads/ECMA-48_5th_edition_june_1991.pdf>
- Use for:
  - core grammar and framing
  - C0/C1 tables
  - CSI parameter/intermediate/final byte grammar
  - OSC/DCS/PM/APC/ST control strings
  - standard control function catalog
- Notes:
  - This is the main implementation document for `dansi-core`.
  - Do not treat DEC private or xterm private meanings as core just because they use CSI/OSC/DCS framing.

### ISO counterpart

#### ISO/IEC 6429:1992 — Information technology — Control functions for coded character sets

- ISO Online Browsing Platform entry: <https://www.iso.org/obp/ui/en/>
- ANSI Webstore preview PDF: <https://webstore.ansi.org/preview-pages/ISO/preview_ISO%2BIEC%2B6429-1992.pdf>
- Use for:
  - confirming the ISO/IEC counterpart to ECMA-48
  - checking edition/introductory notes and terminology
- Notes:
  - The public ANSI preview is not the full standard.
  - For implementation, ECMA-48 5th edition is the accessible full-text primary reference.
  - Keep this entry to make the ANSI / ECMA-48 / ISO/IEC 6429 relationship explicit.

### ANSI counterpart

#### ANSI X3.64-1979 — Additional Controls for Use with American National Standard Code for Information Interchange

- NIST-hosted PDF including ANSI X3.64-1979 / FIPS PUB 86 material: <https://nvlpubs.nist.gov/nistpubs/Legacy/FIPS/fipspub86.pdf>
- FIPS PUB 86 1981 announcement/adoption PDF: <https://nvlpubs.nist.gov/nistpubs/Legacy/FIPS/fipspub86-1981.pdf>
- Use for:
  - ANSI X3.64 historical counterpart
  - differences and historical framing around ANSI X3.64 / ISO 6429 / ECMA-48
  - validating that `dansi-core` is not terminal-only; the scope includes character-imaging I/O devices
- Notes:
  - The PDF itself contains a copyright notice. Do not vendor it into the repo without permission.
  - Use as a historical/reference source; prefer ECMA-48 for the main implementation table unless a specific ANSI distinction is being tested.

### Compatibility / real-world cross-check

#### XTerm Control Sequences

- HTML: <https://invisible-island.net/xterm/ctlseqs/ctlseqs.html>
- PDF: <https://invisible-island.net/xterm/ctlseqs/ctlseqs.pdf>
- Use for:
  - parser recovery behavior
  - real-world terminal sequence compatibility
  - cross-checking whether a sequence is ECMA/core, DEC, or xterm-owned
- Notes:
  - `xterm` documents ECMA/ISO, DEC, and xterm-dependent sequences together.
  - Do not classify ownership by document location alone.

---

## `dansi-dec`

Purpose:

- DEC VT family
- VT52 / VT100 / VT220 / VT320 / VT420 / VT510 / VT520 / VT525
- DEC private sequences
- DEC charset / line drawing
- device attributes and DEC-specific reports
- DECSET / DECRST and related private modes

### VT100

#### VT100 User Guide

- HTML contents: <https://vt100.net/docs/vt100-ug/contents.html>
- Chapter 3 Programmer Information: <https://vt100.net/docs/vt100-ug/chapter3.html>
- PDF mirror: <https://geoffg.net/Downloads/Terminal/VT100_User_Guide.pdf>
- Use for:
  - VT100 control sequence behavior
  - keyboard/application keypad behavior
  - DEC private modes and reports in VT100 context
- Notes:
  - Standard ECMA/ANSI control functions used by VT100 remain `dansi-core` owned.
  - VT100-specific/private/variant behavior belongs to `dansi-dec`.

### VT220

#### VT220 Programmer Reference Manual

- HTML contents: <https://vt100.net/docs/vt220-rm/contents.html>
- HTML chapter on control sequences/private controls: <https://www.zx.net.nz/computers/dec/vt220/doc/vt220-rm/chapter4.html>
- Use for:
  - VT220 control functions
  - 7-bit/8-bit controls
  - character sets
  - DCS-related behavior
  - DEC private control sequence classification

### Later VT family

#### VT520/VT525 Video Terminal Programmer Information

- PDF: <https://web.mit.edu/dosathena/doc/www/ek-vt520-rm.pdf>
- Use for:
  - later VT-family compatibility
  - ANSI control function summary
  - keyboard processing and terminal-level behavior

#### VT330/VT340 Programmer Reference Manual, Volume 2: Graphics Programming

- HTML contents: <https://vt100.net/docs/vt3xx-gp/>
- PDF: <https://bitsavers.trailing-edge.com/pdf/dec/terminal/vt340/EK-VT3XX-GP-001_VT330_VT340_Graphics_Programming_Mar87.pdf>
- Use for:
  - DEC graphics programming context
  - Sixel reference when implementing `dansi-sixel`

---

## `dansi-xterm`

Purpose:

- xterm-dependent / xterm-compatible extension namespace
- private modes
- mouse tracking
- focus tracking
- bracketed paste
- modifyOtherKeys / formatOtherKeys
- synchronized output
- window ops
- xterm-compatible OSC meanings
- xterm-specific reports and capability queries

### Main reference

#### XTerm Control Sequences

- HTML: <https://invisible-island.net/xterm/ctlseqs/ctlseqs.html>
- PDF: <https://invisible-island.net/xterm/ctlseqs/ctlseqs.pdf>
- Use for:
  - mouse modes: X10, normal, button-event, any-event, SGR, UTF-8, urxvt, SGR-pixels
  - bracketed paste
  - focus tracking
  - modifyOtherKeys / formatOtherKeys
  - xterm private modes
  - window manipulation and reports
  - OSC title/palette/selection-related behavior
- Notes:
  - xterm supports many DEC/ECMA functions. Those are not automatically `dansi-xterm` owned.
  - Tag each implemented sequence with owner: `core`, `dec`, or `xterm`.

---

## `dansi-link`

Purpose:

- OSC 8 hyperlink protocol

### Main references

#### Hyperlinks in Terminal Emulators, Egmont Koblinger

- Gist: <https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda>
- Use for:
  - OSC 8 syntax
  - parameters
  - open/close hyperlink behavior
  - compatibility notes

#### Contour clickable links documentation

- URL: <https://contour-terminal.org/vt-extensions/clickable-links/>
- Use for:
  - independent implementation cross-check
  - concise OSC 8 examples

#### iTerm2 escape code documentation

- URL: <https://iterm2.com/documentation-escape-codes.html>
- Use for:
  - iTerm2 support and syntax cross-check

---

## `dansi-shell`

Purpose:

- shell integration OSC protocols
- OSC 133 / OSC 633 family
- optionally OSC 7 current working directory if owned here by project policy

### OSC 133

#### Contour OSC 133 shell integration

- URL: <https://contour-terminal.org/vt-extensions/osc-133-shell-integration/>
- Use for:
  - OSC 133 prompt/input/output zone semantics

#### WezTerm shell integration

- URL: <https://wezterm.org/shell-integration.html>
- Use for:
  - OSC 7
  - OSC 133
  - OSC 1337 interaction in shell integration context

### OSC 633

#### Visual Studio Code Terminal Shell Integration

- URL: <https://code.visualstudio.com/docs/terminal/shell-integration>
- Use for:
  - OSC 633 custom shell integration sequences
  - prompt start/end, pre-exec, command complete, command line metadata
- Notes:
  - `OSC 633` is VS Code-owned/custom. Treat as `dansi-shell`, not core.

---

## `dansi-iterm`

Purpose:

- iTerm2-specific protocols
- OSC 1337 family and other proprietary escape codes

### Main reference

#### iTerm2 Proprietary Escape Codes

- URL: <https://iterm2.com/documentation-escape-codes.html>
- Use for:
  - OSC 1337 image/file/metadata/user-var protocols
  - iTerm2-specific title/badge/mark behavior
- Notes:
  - iTerm2 explicitly frames these as non-standard/proprietary escape codes.
  - Do not put these semantics in `dansi-core`.

---

## `dansi-kitty`

Purpose:

- Kitty keyboard protocol
- Kitty graphics protocol

### Keyboard

#### Kitty keyboard protocol

- URL: <https://sw.kovidgoyal.net/kitty/keyboard-protocol/>
- Use for:
  - disambiguated keyboard input
  - modifier reporting
  - press/repeat/release event reporting
  - alternate layout keys
  - all-keys reporting
- Notes:
  - CSI-u framing may be parsed by `dansi-core`, but keyboard semantics belong to `dansi-kitty`.

### Graphics

#### Kitty graphics protocol

- URL: <https://sw.kovidgoyal.net/kitty/graphics-protocol/>
- Use for:
  - APC-style graphics protocol
  - image data transmission
  - placement/chunking
- Notes:
  - APC framing belongs to `dansi-core`; graphics semantics belong to `dansi-kitty`.

---

## `dansi-sixel`

Purpose:

- Sixel graphics protocol

### Main reference

#### VT330/VT340 Programmer Reference Manual, Volume 2: Graphics Programming

- HTML contents: <https://vt100.net/docs/vt3xx-gp/>
- Chapter 14 Sixel Graphics: <https://vt100.net/docs/vt3xx-gp/chapter14.html>
- PDF: <https://bitsavers.trailing-edge.com/pdf/dec/terminal/vt340/EK-VT3XX-GP-001_VT330_VT340_Graphics_Programming_Mar87.pdf>
- Use for:
  - Sixel payload grammar
  - color registers
  - raster attributes
  - DEC graphics behavior
- Notes:
  - DCS framing belongs to `dansi-core`; Sixel payload semantics belong to `dansi-sixel`.

---

## `daterm` runtime/backend references

Purpose:

- raw mode
- actual terminal/runtime I/O
- OS/backend input events
- resize events
- protocol enable/disable lifecycle
- event routing from selected protocol decoders

### Windows Console input

#### KEY_EVENT_RECORD

- URL: <https://learn.microsoft.com/en-us/windows/console/key-event-record-str>
- Use for:
  - native key down/up
  - repeat count
  - virtual key code / scan code
  - modifier state

#### INPUT_RECORD

- URL: <https://learn.microsoft.com/en-us/windows/console/input-record-str>
- Use for:
  - key/mouse/window-buffer-size event dispatch

### Terminal key-event fallback behavior

#### tcell EventKey documentation

- URL: <https://pkg.go.dev/github.com/gdamore/tcell/v2>
- Use for:
  - documenting that traditional terminal programs generally lack key release events
  - fallback model: press stream + synthetic repeat + no release visibility

### Comparative API reference

#### Crossterm keyboard enhancement flags

- URL: <https://docs.rs/crossterm/latest/crossterm/event/struct.PushKeyboardEnhancementFlags.html>
- URL: <https://docs.rs/crossterm/latest/crossterm/event/enum.KeyboardEnhancementFlags.html>
- Use for:
  - API design comparison for opt-in keyboard enhancement
  - press/repeat/release reporting when supported
