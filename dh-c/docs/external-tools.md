# External tool path contract

`dh-c` separates portable build intent from machine-local executable locations.
Compiler and image-production tools remain authored build properties; helper
programs used to execute that contract are injected through environment
variables.

## Precedence

For helper tools, resolution follows this order:

1. the `DH_C_*` override;
2. the conventional environment variable shown below, when applicable;
3. dh-c's default executable name.

The value is an executable path or name, not a shell command with additional
arguments. CI images that install version-suffixed LLVM tools should set the
corresponding override instead of creating an alias:

```sh
DH_C_AR=llvm-ar-22 dh-c build release
```

## Tool matrix

| Purpose | dh-c override | Conventional fallback | Default |
| --- | --- | --- | --- |
| generated build executor | `DH_C_MAKE` | `MAKE` | `make` |
| static archive producer | `DH_C_AR` | `AR` | `llvm-ar` |
| debugger | `DH_C_DEBUGGER` | `DEBUGGER` | `lldb` |
| CMake provider | `DH_C_CMAKE` | `CMAKE` | `cmake` |
| Git dependency transport | `DH_C_GIT` | `GIT` | `git` |
| primary archive downloader | `DH_C_CURL` | `CURL` | `curl` |
| fallback archive downloader | `DH_C_WGET` | `WGET` | `wget` |
| primary archive extractor | `DH_C_TAR` | `TAR` | `tar` |
| ZIP fallback extractor | `DH_C_UNZIP` | `UNZIP` | `unzip` |
| tidy command | `DH_C_CLANG_TIDY` | `CLANG_TIDY` | `clang-tidy` |
| format command | `DH_C_CLANG_FORMAT` | `CLANG_FORMAT` | `clang-format` |
| disassembly output | `DH_C_LLVM_OBJDUMP` | `LLVM_OBJDUMP` | `llvm-objdump` |
| DWARF inspection output | `DH_C_LLVM_DWARFDUMP` | `LLVM_DWARFDUMP` | `llvm-dwarfdump` |
| PDB inspection output | `DH_C_LLVM_PDBUTIL` | `LLVM_PDBUTIL` | `llvm-pdbutil` |
| Unix custom-provider shell | `DH_C_SHELL` | — | `/bin/sh` |
| Windows custom-provider command interpreter | `DH_C_CMD` | `COMSPEC` | `cmd.exe` |

The Windows directory-junction fallback uses the same `DH_C_CMD`/`COMSPEC`
contract.

Generated Makefiles also expose `RM`, `MV`, and `PRINTF` as ordinary overridable
Make variables. These are recipe utilities supplied by the selected Make/shell
environment rather than separately launched dh-c subprocesses.

## Authored build tools

These executable selections are intentionally not duplicated as `DH_C_*`
helper variables:

- `compiler=` in `.dh` configuration or `--compiler=` on the command line;
- `objcopy=`/`objcopy-format=` in `.dh` configuration or their CLI forms.

The compiler affects target detection, ABI, runtime discovery, and artifact
compatibility. `objcopy` affects the produced image format. They are therefore
part of the authored build contract rather than an incidental host-machine
helper choice.

## Archive command length

Static-library plans write one quoted object path per line to a generated
response file and invoke the archiver as:

```text
<archiver> rcs <target> @<response-file>
```

The archiver command therefore has constant argument count regardless of source
count or workspace path length. Changing the object list rewrites the response
file and invalidates the archive target through its Make dependency.

## Target detection startup

When no explicit `--target`/`target=` is present, dh-c asks the selected compiler
for its default target triple. The result is cached per compiler, command
working directory, and executable search path for the bounded lifetime of the
dh-c process. One command therefore launches at most one
`--print-target-triple` probe for a given compiler instead of repeating the same
probe while resolving dependency, library, and test plans.

Use `dh-c help tools` for the compact CLI reference and `dh-c doctor` to inspect
the effective core/provider tools needed by the detected project.
