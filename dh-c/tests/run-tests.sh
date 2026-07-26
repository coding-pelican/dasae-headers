#!/bin/sh
set -eu

integration=0
address_sanitizer=0

for arg in "$@"; do
    case "$arg" in
        --integration) integration=1 ;;
        --address-sanitizer) address_sanitizer=1 ;;
        *)
            printf 'Unknown option: %s\n' "$arg" >&2
            exit 1
            ;;
    esac
done

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
include_dir="$repo_root/dh-c/include"
internal_dir="$repo_root/dh-c/src"
scratch_root="$repo_root/dh-c/tests/.scratch"
bin_root="$scratch_root/bin"
temp_root="$scratch_root/integration"
find_bin=${FIND_BIN:-find}
grep_bin=${GREP_BIN:-grep}

if [ -x /usr/bin/find ]; then
    find_bin=/usr/bin/find
fi
if [ -x /usr/bin/grep ]; then
    grep_bin=/usr/bin/grep
fi

case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        exe_ext=".exe"
        shared_pattern="*render*.dll"
        lib_kind_shared_pattern="core.dll"
        lib_kind_static_pattern="core.lib"
        lib_kind_lto_static_pattern="core.lto.lib"
        lib_kind_import_pattern="core.dll.lib"
        static_lib_pattern="*.lib"
        ;;
    *)
        exe_ext=""
        shared_pattern="*render*.so"
        lib_kind_shared_pattern="libcore.so"
        lib_kind_static_pattern="libcore.a"
        lib_kind_lto_static_pattern="libcore.lto.a"
        lib_kind_import_pattern=""
        static_lib_pattern="*.a"
        ;;
esac

unit_exe="$bin_root/dh-c-unit$exe_ext"
cli_exe="$bin_root/dh-c-cli-test$exe_ext"

common_sources='
'"${repo_root}"'/dh-c/src/dal-c/Cmd.c
'"${repo_root}"'/dh-c/src/dal-c/Project.c
'"${repo_root}"'/dh-c/src/dal-c/archive.c
'"${repo_root}"'/dh-c/src/dal-c/build.c
'"${repo_root}"'/dh-c/src/dal-c-ext/str.c
'"${repo_root}"'/dh-c/src/dal-c-ext/path.c
'"${repo_root}"'/dh-c/src/dal-c-ext/dir.c
'"${repo_root}"'/dh-c/src/dal-c-ext/file.c
'"${repo_root}"'/dh-c/src/dal-c-ext/env.c
'"${repo_root}"'/dh-c/src/dal-c-ext/proc.c
'"${repo_root}"'/dh-c/src/dal-c-ext/ArrStr.c
'

LAST_OUTPUT=""

assert_true() {
    if [ "$1" -ne 0 ]; then
        printf '%s\n' "$2" >&2
        exit 1
    fi
}

assert_contains() {
    text=$1
    needle=$2
    message=$3
    case "$text" in
        *"$needle"*) ;;
        *)
            printf '%s\n' "$message" >&2
            exit 1
            ;;
    esac
}

assert_occurrences() {
    text=$1
    needle=$2
    expected=$3
    message=$4
    count=$( (printf '%s\n' "$text" | "$grep_bin" -F -o "$needle" || true) | wc -l | tr -d ' ')
    if [ "$count" != "$expected" ]; then
        printf '%s: expected %s occurrence(s), found %s\n' "$message" "$expected" "$count" >&2
        exit 1
    fi
}

native_path() {
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) cygpath -w "$1" ;;
        *) printf '%s\n' "$1" ;;
    esac
}

invoke_external() {
    expected_codes=$1
    working_directory=$2
    shift 2

    output_file=$(mktemp)
    old_pwd=$(pwd)
    cd "$working_directory"
    set +e
    "$@" >"$output_file" 2>&1
    exit_code=$?
    set -e
    cd "$old_pwd"

    LAST_OUTPUT=$(cat "$output_file")
    rm -f "$output_file"

    case " $expected_codes " in
        *" $exit_code "*) return 0 ;;
        *)
            printf 'Command failed with exit code %s:\n' "$exit_code" >&2
            printf '%s\n' "$*" >&2
            printf '%s\n' "$LAST_OUTPUT" >&2
            exit 1
            ;;
    esac
}

build_binary() {
    output_path=$1
    extra_source=$2

    mkdir -p "$(dirname "$output_path")"

    set -- clang \
        -std=gnu17 \
        -Wall \
        -Wextra \
        -Werror \
        -g \
        -O0 \
        "-I$include_dir" \
        "-I$internal_dir" \

    if [ "$address_sanitizer" -eq 1 ]; then
        set -- "$@" -O1 -fno-omit-frame-pointer -fsanitize=address
    fi

    set -- "$@" -o "$output_path" "$extra_source"
    for source in $common_sources; do
        set -- "$@" "$source"
    done
    invoke_external "0" "$repo_root" "$@"
}

assert_build_artifacts_exist() {
    project_root=$1
    shift
    build_dir="$project_root/build"
    [ -d "$build_dir" ]
    assert_true $? "Expected build directory at $build_dir"

    for pattern in "$@"; do
        if ! "$find_bin" "$build_dir" -type f -name "$pattern" | "$grep_bin" . >/dev/null 2>&1; then
            printf "Expected artifact matching '%s' under %s\n" "$pattern" "$build_dir" >&2
            exit 1
        fi
    done
}

assert_build_artifacts_absent() {
    project_root=$1
    shift
    build_dir="$project_root/build"

    for pattern in "$@"; do
        if "$find_bin" "$build_dir" -type f -name "$pattern" | "$grep_bin" . >/dev/null 2>&1; then
            printf "Unexpected artifact matching '%s' under %s\n" "$pattern" "$build_dir" >&2
            exit 1
        fi
    done
}

remove_recur() {
    path=$1
    i=0
    while [ "$i" -lt 20 ]; do
        rm -rf "$path" 2>/dev/null || true
        if [ ! -e "$path" ]; then
            return 0
        fi
        i=$((i + 1))
        sleep 0.1
    done
    rm -rf "$path" 2>/dev/null || true
    [ ! -e "$path" ]
}

remove_generated_dirs() {
    root=$1
    "$find_bin" "$root" -type d \( -name build -o -name .cache -o -name .dh-c \) -prune -print | while IFS= read -r dir; do
        remove_recur "$dir" || true
    done
}

reset_temp_root() {
    remove_recur "$temp_root"
    mkdir -p "$temp_root"
}

cleanup_temp_root() {
    remove_recur "$scratch_root" || true
}

trap cleanup_temp_root EXIT

copy_scenario_project() {
    relative_source=$1
    source="$repo_root/$relative_source"
    name=$(basename "$source")
    destination="$temp_root/$name"
    cp -R "$source" "$destination"
    remove_generated_dirs "$destination"
    printf '%s\n' "$destination"
}

build_binary "$unit_exe" "$repo_root/dh-c/tests/test-dal-c.c"
build_binary "$cli_exe" "$repo_root/dh-c/src/dal-c.c"

invoke_external "0" "$repo_root" "$unit_exe"

invoke_external "0" "$repo_root" "$cli_exe" --version
assert_contains "$LAST_OUTPUT" "dh-c version" "Version output did not contain tool version"
assert_contains "$LAST_OUTPUT" "dasae-headers path:" "Version output did not contain detected dasae-headers path"

invoke_external "0" "$repo_root" "$cli_exe" --help
assert_contains "$LAST_OUTPUT" "COMMANDS:" "Help output did not list commands"
assert_contains "$LAST_OUTPUT" "SETUP" "Help output did not expose setup commands"
assert_contains "$LAST_OUTPUT" "EVERYDAY" "Help output did not prioritize everyday commands"
assert_contains "$LAST_OUTPUT" "DEPENDENCIES & DELIVERY" "Help output did not group dependency commands"
assert_contains "$LAST_OUTPUT" "INSPECT & DEBUG" "Help output did not group inspection commands"
assert_contains "$LAST_OUTPUT" "ALIASES:" "Help output did not separate compatibility aliases"
assert_contains "$LAST_OUTPUT" "help --all" "Concise help did not point to full help"

invoke_external "0" "$repo_root" "$cli_exe" help --list
assert_contains "$LAST_OUTPUT" "setup:         workspace project" "Help list did not contain setup commands"
assert_contains "$LAST_OUTPUT" "build" "Help list did not contain build"
assert_contains "$LAST_OUTPUT" "test" "Help list did not contain test"

invoke_external "0" "$repo_root" "$cli_exe" help --all
assert_contains "$LAST_OUTPUT" "COMMAND OPTION BOUNDARIES:" "Help output did not describe command option boundaries"
assert_contains "$LAST_OUTPUT" "CONFIGURATION TOPICS:" "Help output did not include configuration topics"
assert_contains "$LAST_OUTPUT" "target.dh" "Help output did not describe target.dh"
assert_contains "$LAST_OUTPUT" "LINK MODEL:" "Help output did not describe freestanding/link-model separation"
assert_contains "$LAST_OUTPUT" 'exact alias of `--link-start-files=off`' "Help output did not explain the CRT alias"
assert_contains "$LAST_OUTPUT" "durable source assets" "Help output did not describe cleanup-owned generated paths"

invoke_external "0" "$repo_root" "$cli_exe" help tools
assert_contains "$LAST_OUTPUT" "DH_C_AR" "Tool help did not expose archiver injection"
assert_contains "$LAST_OUTPUT" "DH_C_SHELL" "Tool help did not expose provider shell injection"

invoke_external "0" "$repo_root" "$cli_exe" help files
assert_contains "$LAST_OUTPUT" "workspace.dh" "File help did not describe workspace.dh"
assert_contains "$LAST_OUTPUT" "project.dh" "File help did not describe project.dh"
assert_contains "$LAST_OUTPUT" "target.dh" "File help did not describe target.dh"
assert_contains "$LAST_OUTPUT" "<source>.dh" "File help did not describe source companions"

invoke_external "0" "$repo_root" "$cli_exe" help dh-file
assert_contains "$LAST_OUTPUT" "comp-args" "dh-file help did not describe persistent compiler args"
assert_contains "$LAST_OUTPUT" "link-script" "dh-file help did not describe linker scripts"
assert_contains "$LAST_OUTPUT" "output-ext" "dh-file help did not describe output extension"

invoke_external "0" "$repo_root" "$cli_exe" help precedence
assert_contains "$LAST_OUTPUT" "workspace.dh" "Precedence help did not start at workspace configuration"
assert_contains "$LAST_OUTPUT" "command-line options" "Precedence help did not end at command line"

invoke_external "0" "$repo_root" "$cli_exe" help project
assert_contains "$LAST_OUTPUT" "Create a minimal buildable project.dh project" "Project scaffold help was not available"
assert_contains "$LAST_OUTPUT" "never overwritten" "Project scaffold help did not describe overwrite safety"

invoke_external "0" "$repo_root" "$cli_exe" help workspace
assert_contains "$LAST_OUTPUT" "Create a workspace.dh boundary" "Workspace scaffold help was not available"

invoke_external "0" "$repo_root" "$cli_exe" help invocation-only
assert_contains "$LAST_OUTPUT" "jobs" "Invocation-only help did not describe scheduling controls"
assert_contains "$LAST_OUTPUT" "analysis/emit" "Invocation-only help did not describe one-off artifact controls"

invoke_external "0" "$repo_root" "$cli_exe" help build
assert_contains "$LAST_OUTPUT" "USAGE:" "Command help did not show usage"
assert_contains "$LAST_OUTPUT" "CONTRACT:" "Command help did not show contract"
assert_contains "$LAST_OUTPUT" "--output-ext" "Build help did not describe explicit output extension"

invoke_external "0" "$repo_root" "$cli_exe" help package
assert_contains "$LAST_OUTPUT" "--layout=install|prebuilt" "Package help did not expose prebuilt promotion"
assert_contains "$LAST_OUTPUT" "prebuilt/<target>/<profile>" "Package help did not describe prebuilt output"

invoke_external "0" "$repo_root" "$cli_exe" help clean
assert_contains "$LAST_OUTPUT" "Do not store durable source assets" "Clean help did not describe cleanup-owned generated paths"
assert_contains "$LAST_OUTPUT" "--cache" "Clean help did not describe cache scope"
assert_contains "$LAST_OUTPUT" "--deps" "Clean help did not describe dependency scope"
assert_contains "$LAST_OUTPUT" "--unused" "Clean help did not describe unused dependency cleanup"
assert_contains "$LAST_OUTPUT" "--older-than" "Clean help did not describe age-based cleanup"
assert_contains "$LAST_OUTPUT" "--dry-run" "Clean help did not describe preview mode"

invoke_external "0" "$repo_root" "$cli_exe" syntax --help
assert_contains "$LAST_OUTPUT" "never links" "Syntax help did not describe non-linking contract"
assert_contains "$LAST_OUTPUT" "not accepted" "Syntax help did not describe unavailable options"

invoke_external "1" "$repo_root" "$cli_exe" help no-such-command
assert_contains "$LAST_OUTPUT" "Unknown help topic" "Unknown help topic did not fail clearly"

scaffold_root=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-scaffold.XXXXXX")
workspace_root="$scaffold_root/workspace"
project_root="$workspace_root/app"

invoke_external "0" "$repo_root" "$cli_exe" workspace "$workspace_root"
assert_contains "$LAST_OUTPUT" "Created workspace:" "Workspace scaffold did not report success"
if [ ! -f "$workspace_root/workspace.dh" ]; then
    printf 'Workspace scaffold did not create workspace.dh\n' >&2
    exit 1
fi

invoke_external "0" "$repo_root" "$cli_exe" project "$project_root"
assert_contains "$LAST_OUTPUT" "Created project:" "Project scaffold did not report success"
if [ ! -f "$project_root/project.dh" ] || [ ! -f "$project_root/src/main.c" ]; then
    printf 'Project scaffold did not create the expected project files\n' >&2
    exit 1
fi

invoke_external "0" "$project_root" "$cli_exe" build
assert_contains "$LAST_OUTPUT" "Build successful" "Generated project did not build successfully"

invoke_external "1" "$repo_root" "$cli_exe" project "$project_root"
assert_contains "$LAST_OUTPUT" "Refusing to overwrite" "Project scaffold did not protect existing configuration"
rm -rf "$scaffold_root"

no_project_clean=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-no-project-clean.XXXXXX")
mkdir -p "$no_project_clean/build/dev"
printf 'stale\n' >"$no_project_clean/build/dev/stale.txt"
invoke_external "0" "$no_project_clean" "$cli_exe" clean dev
if [ -e "$no_project_clean/build/dev" ]; then
    printf 'No-project clean did not remove build/dev directory\n' >&2
    exit 1
fi
rm -rf "$no_project_clean"

# Dependency resolution contract: fetch preserves lock.dh, update deliberately
# re-resolves it, and status rejects an unlocked checkout.
lock_contract_root=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-lock-contract.XXXXXX")
lock_origin="$lock_contract_root/origin"
lock_project="$lock_contract_root/project"
mkdir -p "$lock_origin" "$lock_project"
lock_origin_native=$(native_path "$lock_origin")
git -C "$lock_origin" init -q -b main
git -C "$lock_origin" config user.email dh-c-test@example.invalid
git -C "$lock_origin" config user.name dh-c-test
printf 'one\n' >"$lock_origin/value.txt"
git -C "$lock_origin" add value.txt
git -C "$lock_origin" commit -q -m one
lock_commit_one=$(git -C "$lock_origin" rev-parse HEAD)
cat >"$lock_project/project.dh" <<EOF
[dep]
source=$lock_origin_native
revision=main
provider=cmake
EOF

invoke_external "0" "$lock_project" "$cli_exe" fetch
[ -f "$lock_project/lock.dh" ]
assert_true $? "fetch did not create project-level lock.dh"
[ -f "$lock_project/.dh-c/deps/usage/dep.stamp" ]
assert_true $? "fetch did not update the dependency last-use stamp"
invoke_external "0" "$lock_project" "$cli_exe" status
assert_contains "$LAST_OUTPUT" "[READY]" "status did not accept the locked checkout"

printf 'two\n' >"$lock_origin/value.txt"
git -C "$lock_origin" commit -qam two
lock_commit_two=$(git -C "$lock_origin" rev-parse HEAD)
invoke_external "0" "$lock_project" "$cli_exe" fetch
lock_checkout=$(git -C "$lock_project/.dh-c/deps/src/dep" rev-parse HEAD)
[ "$lock_checkout" = "$lock_commit_one" ]
assert_true $? "fetch replaced an existing locked dependency resolution"

invoke_external "0" "$lock_project" "$cli_exe" update
lock_checkout=$(git -C "$lock_project/.dh-c/deps/src/dep" rev-parse HEAD)
[ "$lock_checkout" = "$lock_commit_two" ]
assert_true $? "update did not re-resolve the requested dependency revision"
lock_recorded=$(awk -F= '/^revision=/{print $2}' "$lock_project/lock.dh")
[ "$lock_recorded" = "$lock_commit_two" ]
assert_true $? "update did not rewrite lock.dh with the resolved commit"

rm -f "$lock_project/lock.dh"
invoke_external "1" "$lock_project" "$cli_exe" status
assert_contains "$LAST_OUTPUT" "[UNLOCKED]" "status did not report a missing dependency lock"

printf 'local change\n' >>"$lock_project/.dh-c/deps/src/dep/value.txt"
invoke_external "1" "$lock_project" "$cli_exe" clean --deps --older-than=0s
assert_contains "$LAST_OUTPUT" "Preserved dependency checkout" "dependency cleanup did not preserve a dirty checkout"
[ -d "$lock_project/.dh-c/deps/src/dep" ]
assert_true $? "dependency cleanup removed a dirty checkout without --force"
invoke_external "0" "$lock_project" "$cli_exe" clean --deps --older-than=0s --force
[ ! -e "$lock_project/.dh-c/deps/src/dep" ]
assert_true $? "forced dependency cleanup did not remove the dirty checkout"
rm -rf "$lock_contract_root"

# Archive dependency contract: fetch locks exact archive bytes, fetch preserves the
# lock, update deliberately accepts new bytes, and provider=prebuilt materializes
# the extracted package through the normal private package/staging path.
archive_contract_root=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-archive-contract.XXXXXX")
archive_payload_root="$archive_contract_root/payload"
archive_package="$archive_payload_root/dep-package"
archive_package_root="$archive_package/targets/x86_64-w64-windows-gnu"
archive_project="$archive_contract_root/project"
archive_file="$archive_contract_root/dep.tar.gz"
mkdir -p "$archive_package_root/include" "$archive_package_root/lib" "$archive_project"
printf '#define ARCHIVE_VALUE 1\n' >"$archive_package_root/include/archive_dep.h"
printf 'archive-import-one\n' >"$archive_package_root/lib/libarchive_dep.dll.a"
printf 'archive-static-one\n' >"$archive_package_root/lib/libarchive_dep.a"
printf 'archive-unselected\n' >"$archive_package_root/lib/libunselected.a"
tar -czf "$archive_file" -C "$archive_payload_root" dep-package
archive_file_native=$(native_path "$archive_file")

archive_invalid_project="$archive_contract_root/invalid-project"
mkdir -p "$archive_invalid_project"
cat >"$archive_invalid_project/project.dh" <<EOF
[dep]
source=https://example.invalid/dep.git
archive=$archive_file_native
provider=prebuilt
EOF
invoke_external "1" "$archive_invalid_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "cannot declare both source= and archive=" "dependency parser accepted both source= and archive="

cat >"$archive_invalid_project/project.dh" <<EOF
[dep]
archive=$archive_file_native
revision=v1.0.0
provider=prebuilt
EOF
invoke_external "1" "$archive_invalid_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "cannot declare revision= with archive=" "dependency parser accepted revision= beside archive="

cat >"$archive_invalid_project/project.dh" <<EOF
[dep]
archive=$archive_file_native
package-root=../outside
provider=prebuilt
EOF
invoke_external "1" "$archive_invalid_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "package-root must stay within" "dependency parser accepted an escaping package-root"

cat >"$archive_project/project.dh" <<EOF
[dep]
archive=$archive_file_native
package-root=targets/x86_64-w64-windows-gnu
provider=prebuilt
linking=shared
link=archive_dep
EOF

invoke_external "0" "$archive_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "[FETCH] dep" "archive fetch did not download/materialize the dependency"
assert_contains "$(cat "$archive_project/lock.dh")" "archive=$archive_file_native" "archive lock did not preserve the authored archive location"
archive_revision_one=$(awk -F= '/^revision=/{print $2}' "$archive_project/lock.dh")
assert_contains "$archive_revision_one" "sha256:" "archive lock did not record a SHA-256 resolution"
[ -f "$archive_project/.dh-c/deps/src/dep/targets/x86_64-w64-windows-gnu/include/archive_dep.h" ]
assert_true $? "archive fetch did not unwrap the archive before applying package-root"
invoke_external "0" "$archive_project" "$cli_exe" status
assert_contains "$LAST_OUTPUT" "[READY]" "archive status did not accept the locked materialization"
invoke_external "0" "$archive_project" "$cli_exe" deps dev
[ -f "$archive_project/lib/deps/archive_dep.h" ]
assert_true $? "prebuilt archive header was not staged for the consumer"
[ -f "$archive_project/lib/deps/libarchive_dep.dll.a" ]
assert_true $? "selected prebuilt archive import library was not staged for the consumer"
[ ! -f "$archive_project/lib/deps/libarchive_dep.a" ]
assert_true $? "prebuilt archive staged a static variant for linking=shared"
[ ! -f "$archive_project/lib/deps/libunselected.a" ]
assert_true $? "prebuilt archive staged a library absent from link="

printf '#define ARCHIVE_VALUE 2\n' >"$archive_package_root/include/archive_dep.h"
printf 'archive-import-two\n' >"$archive_package_root/lib/libarchive_dep.dll.a"
rm -f "$archive_file"
tar -czf "$archive_file" -C "$archive_payload_root" dep-package
invoke_external "0" "$archive_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "[HAVE]" "archive fetch did not preserve an existing locked materialization"
archive_revision_preserved=$(awk -F= '/^revision=/{print $2}' "$archive_project/lock.dh")
[ "$archive_revision_preserved" = "$archive_revision_one" ]
assert_true $? "archive fetch rewrote an existing lock"
rm -rf "$archive_project/.dh-c/deps/src/dep"
invoke_external "1" "$archive_project" "$cli_exe" fetch
assert_contains "$LAST_OUTPUT" "differs from lock.dh" "archive fetch accepted changed bytes for an existing lock"
invoke_external "0" "$archive_project" "$cli_exe" update
archive_revision_two=$(awk -F= '/^revision=/{print $2}' "$archive_project/lock.dh")
[ "$archive_revision_two" != "$archive_revision_one" ]
assert_true $? "archive update did not re-resolve changed archive bytes"
invoke_external "0" "$archive_project" "$cli_exe" status
assert_contains "$LAST_OUTPUT" "[READY]" "archive status did not accept the updated materialization"
rm -rf "$archive_contract_root"

provider_contract_root=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-provider-contract.XXXXXX")
provider_bin="$provider_contract_root/bin"
provider_source="$provider_contract_root/source"
provider_sysroot="$provider_contract_root/sysroot"
mkdir -p "$provider_bin" "$provider_source" "$provider_sysroot"
provider_source_native=$(native_path "$provider_source")
provider_sysroot_native=$(native_path "$provider_sysroot")
provider_probe="$repo_root/dh-c/tests/provider-probe.c"
tool_probe_source="$repo_root/dh-c/tests/tool-probe.c"
provider_cmake="$provider_bin/cmake$exe_ext"
provider_make="$provider_bin/make$exe_ext"
tool_probe="$provider_bin/tool-probe$exe_ext"
clang -std=gnu17 -Wall -Wextra -Werror -o "$provider_cmake" "$provider_probe"
clang -std=gnu17 -Wall -Wextra -Werror -o "$tool_probe" "$tool_probe_source"
cp "$provider_cmake" "$provider_make"
chmod +x "$provider_cmake" "$provider_make" "$tool_probe"

cmake_project="$provider_contract_root/cmake-project"
mkdir -p "$cmake_project"
cat >"$cmake_project/project.dh" <<EOF
output=provider-contract
link-dsl=off

[dep]
path=$provider_source_native
provider=cmake
EOF
cmake_log="$provider_contract_root/cmake.log"
cmake_log_native=$(native_path "$cmake_log")
invoke_external "0" "$cmake_project" env DH_C_CMAKE="$provider_cmake" DH_C_AR=llvm-ar-22 DH_TEST_PROVIDER_LOG="$cmake_log_native" "$cli_exe" deps dev --target=aarch64-w64-windows-gnu --sysroot="$provider_sysroot_native" --compiler=clang-cross
cmake_text=$(cat "$cmake_log")
assert_contains "$cmake_text" "-DCMAKE_C_COMPILER=clang-cross" "CMake provider did not receive the effective compiler"
assert_contains "$cmake_text" "-DCMAKE_AR=llvm-ar-22" "CMake provider did not receive the injected archiver"
assert_contains "$cmake_text" "-DCMAKE_C_COMPILER_TARGET=aarch64-w64-windows-gnu" "CMake provider did not receive the target triple"
assert_contains "$cmake_text" "-DCMAKE_SYSROOT=$provider_sysroot_native" "CMake provider did not receive the sysroot"

make_project="$provider_contract_root/make-project"
mkdir -p "$make_project"
cat >"$make_project/project.dh" <<EOF
output=provider-contract
link-dsl=off

[dep]
path=$provider_source_native
provider=make
EOF
make_log="$provider_contract_root/make.log"
make_log_native=$(native_path "$make_log")
invoke_external "0" "$make_project" env DH_C_MAKE="$provider_make" DH_C_AR=llvm-ar-22 DH_TEST_PROVIDER_LOG="$make_log_native" "$cli_exe" deps dev --target=aarch64-w64-windows-gnu --sysroot="$provider_sysroot_native" --compiler=clang-cross
make_text=$(cat "$make_log")
assert_contains "$make_text" "DH_DEP_TARGET=aarch64-w64-windows-gnu" "Make provider did not receive the target triple"
assert_contains "$make_text" "DH_DEP_CC=clang-cross" "Make provider did not receive the effective compiler"
assert_contains "$make_text" "DH_DEP_AR=llvm-ar-22" "Make provider did not receive the injected archiver"
assert_contains "$make_text" "DH_DEP_SYSROOT=$provider_sysroot_native" "Make provider did not receive the sysroot"
assert_contains "$make_text" "DH_DEP_CFLAGS=--target=aarch64-w64-windows-gnu --sysroot=$provider_sysroot_native" "Make provider did not receive target C flags"

target_probe_project="$provider_contract_root/target-probe-project"
mkdir -p "$target_probe_project/src"
cat >"$target_probe_project/project.dh" <<'EOF'
output=target-probe
kind=static-lib
link-dsl=off
EOF
cat >"$target_probe_project/src/value.c" <<'EOF'
int target_probe_value(void);
int target_probe_value(void) { return 1; }
EOF
target_probe_log="$provider_contract_root/target-probe.log"
target_probe_log_native=$(native_path "$target_probe_log")
invoke_external "0" "$target_probe_project" env DH_TEST_TOOL_LOG="$target_probe_log_native" DH_TEST_TOOL_TARGET=x86_64-unknown-linux-gnu "$cli_exe" plan dev --compiler="$tool_probe"
target_probe_count=$("$grep_bin" -F -c 'arg=--print-target-triple' "$target_probe_log" || true)
[ "$target_probe_count" = "1" ]
assert_true $? "Target resolution launched the compiler more than once in one command"

archive_rsp_project="$provider_contract_root/long-workspace-path-with-hyphens/archive-response-project"
mkdir -p "$archive_rsp_project/src"
cat >"$archive_rsp_project/project.dh" <<'EOF'
output=archive-response
kind=static-lib
link-dsl=off
EOF
archive_source_count=24
i=1
while [ "$i" -le "$archive_source_count" ]; do
    number=$(printf '%03d' "$i")
    cat >"$archive_rsp_project/src/object-file-with-a-long-name-$number.c" <<EOF
int archive_response_symbol_$number(void);
int archive_response_symbol_$number(void) { return $i; }
EOF
    i=$((i + 1))
done
archive_probe_log="$provider_contract_root/archive-probe.log"
archive_probe_log_native=$(native_path "$archive_probe_log")
invoke_external "0" "$archive_rsp_project" env DH_C_AR="$tool_probe" DH_TEST_TOOL_LOG="$archive_probe_log_native" DH_TEST_TOOL_TOUCH_ARG=2 "$cli_exe" build dev
archive_probe_text=$(cat "$archive_probe_log")
assert_contains "$archive_probe_text" "argc=3" "Archiver received expanded object paths instead of one response file"
assert_contains "$archive_probe_text" "arg=@" "Archiver did not receive a response-file argument"
archive_rsp=$("$find_bin" "$archive_rsp_project/build" -type f -name '*.rsp' | head -n 1)
[ -n "$archive_rsp" ]
assert_true $? "Static archive response file was not generated"
archive_rsp_lines=$(wc -l <"$archive_rsp" | tr -d ' ')
[ "$archive_rsp_lines" = "$archive_source_count" ]
assert_true $? "Static archive response file did not contain every object"

rm -rf "$provider_contract_root"

if [ "$integration" -eq 1 ]; then
    reset_temp_root

    adhoc_workspace="$temp_root/adhoc-workspace"
    adhoc_unit="$adhoc_workspace/local-unit"
    mkdir -p "$adhoc_unit/vendor"
    cat >"$adhoc_workspace/workspace.dh" <<'EOF'
std=c17
EOF
    cat >"$adhoc_unit/vendor/dep.h" <<'EOF'
#ifndef DEP_H
#define DEP_H
#define DEP_VALUE 0
#endif
EOF
    cat >"$adhoc_unit/main.c" <<'EOF'
#include <dep.h>
int main(void) { return DEP_VALUE; }
EOF
    cat >"$adhoc_unit/main.dh" <<'EOF'
link-dsl=off

[dep]
path=vendor/dep.h
provider=dh
EOF
    invoke_external "0" "$adhoc_unit" "$cli_exe" deps main.c
    assert_contains "$LAST_OUTPUT" "dep" "Projectless deps command did not process the primary companion dependency"
    invoke_external "0" "$adhoc_unit" "$cli_exe" graph main.c
    assert_contains "$LAST_OUTPUT" "BUILD UNIT main" "Projectless graph did not identify the primary source unit"
    assert_contains "$LAST_OUTPUT" "- dep [provider=dh" "Projectless graph did not include the source-owned dependency"
    invoke_external "0" "$adhoc_unit" "$cli_exe" graph main.c --format=dot
    assert_contains "$LAST_OUTPUT" "digraph dh_c" "Projectless DOT graph did not render"
    invoke_external "0" "$adhoc_unit" "$cli_exe" target show main.c
    adhoc_build_native=$(native_path "$adhoc_unit/build")
    assert_contains "$LAST_OUTPUT" "$adhoc_build_native" "Projectless target show did not report the source-unit build directory"
    invoke_external "0" "$adhoc_unit" "$cli_exe" doctor main.c
    assert_contains "$LAST_OUTPUT" "build-unit:" "Projectless doctor did not identify the source unit"
    invoke_external "0" "$adhoc_unit" "$cli_exe" build main.c
    assert_contains "$LAST_OUTPUT" "Build successful!" "Projectless header-only dependency build did not succeed"
    [ ! -e "$adhoc_unit/.dh-c" ]
    assert_true $? "Projectless dependency build leaked generated state beside the source"
    if ! "$find_bin" "$adhoc_workspace/.dh-c/units" -type f -path '*/exports/deps/dep.h' | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Projectless dependency header was not staged in the workspace unit state\n' >&2
        exit 1
    fi
    if ! "$find_bin" "$adhoc_unit/build" -type f -name "main$exe_ext" | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Projectless dependency build did not materialize the executable beside the source unit\n' >&2
        exit 1
    fi

    cat >"$adhoc_unit/util.c" <<'EOF'
int util_value(void) { return 0; }
EOF
    cat >"$adhoc_unit/util.dh" <<'EOF'
[illegal-secondary-dependency]
path=vendor/dep.h
EOF
    invoke_external "1" "$adhoc_unit" "$cli_exe" build main.c util.c
    assert_contains "$LAST_OUTPUT" "sections are not allowed in source companion .dh" "Secondary source companion accepted a dependency section"
    rm -f "$adhoc_unit/util.c" "$adhoc_unit/util.dh"

    adhoc_remote="$temp_root/adhoc-remote"
    adhoc_remote_unit="$adhoc_workspace/remote-unit"
    mkdir -p "$adhoc_remote/include" "$adhoc_remote/src" "$adhoc_remote_unit"
    invoke_external "0" "$adhoc_remote" git init
    invoke_external "0" "$adhoc_remote" git config user.email dh-c-tests@example.invalid
    invoke_external "0" "$adhoc_remote" git config user.name dh-c-tests
    cat >"$adhoc_remote/project.dh" <<'EOF'
output=remote
kind=static-lib
link-dsl=off
EOF
    cat >"$adhoc_remote/include/remote.h" <<'EOF'
#ifndef REMOTE_H
#define REMOTE_H
#define REMOTE_VALUE 0
int remote_value(void);
#endif
EOF
    cat >"$adhoc_remote/src/remote.c" <<'EOF'
#include <remote.h>
int remote_value(void) { return REMOTE_VALUE; }
EOF
    invoke_external "0" "$adhoc_remote" git add project.dh include/remote.h src/remote.c
    invoke_external "0" "$adhoc_remote" git commit -m initial
    invoke_external "0" "$adhoc_remote" git symbolic-ref --short HEAD
    adhoc_branch=$(printf '%s\n' "$LAST_OUTPUT" | tail -n 1)
    adhoc_remote_native=$(native_path "$adhoc_remote")
    cat >"$adhoc_remote_unit/main.c" <<'EOF'
#include <remote.h>
int main(void) { return REMOTE_VALUE; }
EOF
    cat >"$adhoc_remote_unit/main.dh" <<EOF
link-dsl=off

[remote]
source=$adhoc_remote_native
revision=$adhoc_branch
provider=dh
EOF
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" update main.c
    [ -f "$adhoc_remote_unit/main.lock.dh" ]
    assert_true $? "Projectless dependency update did not create main.lock.dh"
    first_lock=$(cat "$adhoc_remote_unit/main.lock.dh")
    assert_contains "$first_lock" "[remote]" "Projectless lock did not contain the dependency section"
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" status main.c
    assert_contains "$LAST_OUTPUT" "READY" "Projectless dependency status did not report the locked checkout as ready"
    assert_contains "$LAST_OUTPUT" "main.lock.dh" "Projectless dependency status did not show its source-owned lock"

    cat >"$adhoc_remote/include/remote.h" <<'EOF'
#ifndef REMOTE_H
#define REMOTE_H
#define REMOTE_VALUE 0
#define REMOTE_GENERATION 2
int remote_value(void);
#endif
EOF
    invoke_external "0" "$adhoc_remote" git add include/remote.h
    invoke_external "0" "$adhoc_remote" git commit -m second
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" fetch main.c
    [ "$(cat "$adhoc_remote_unit/main.lock.dh")" = "$first_lock" ]
    assert_true $? "Projectless fetch changed an existing exact resolution"
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" update main.c
    [ "$(cat "$adhoc_remote_unit/main.lock.dh")" != "$first_lock" ]
    assert_true $? "Projectless update did not refresh the source-owned lock"
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" build main.c
    assert_contains "$LAST_OUTPUT" "Build successful!" "Projectless locked Git dependency build did not succeed"
    invoke_external "0" "$adhoc_remote_unit" "$cli_exe" clean main.c --deps --older-than=0s
    if "$find_bin" "$adhoc_workspace/.dh-c/units" -type d -path '*/deps/src/remote' | "$grep_bin" . >/dev/null 2>&1; then
        printf '%s\n' "Projectless dependency cleanup preserved generated provider state as a dirty checkout" >&2
        exit 1
    fi
    [ -f "$adhoc_remote_unit/main.lock.dh" ]
    assert_true $? "Projectless dependency cleanup removed the durable source lock"

    explain_project=$(copy_scenario_project "dh-c/tests/fixture/plain-project")
    invoke_external "0" "$explain_project" "$cli_exe" explain rebuild --link-dsl=off
    assert_contains "$LAST_OUTPUT" 'Finished `explain rebuild`' "Explain reported the underlying build action instead of explain rebuild"
    [ ! -e "$explain_project/build" ]
    assert_true $? "explain rebuild materialized the build directory"
    [ ! -e "$explain_project/.dh-c" ]
    assert_true $? "explain rebuild materialized project state"

    plain_project=$(copy_scenario_project "dh-c/tests/fixture/plain-project")
    invoke_external "0" "$plain_project" "$cli_exe" target show
    [ ! -e "$plain_project/build" ]
    assert_true $? "target show materialized build/native"
    invoke_external "0" "$plain_project" "$cli_exe" build
    assert_contains "$LAST_OUTPUT" "Build successful!" "Plain project build did not succeed"
    assert_build_artifacts_exist "$plain_project" "plain-project$exe_ext"
    [ -d "$plain_project/build/native" ]
    assert_true $? "Host build did not create build/native"
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) ;;
        *)
            [ -L "$plain_project/build/native" ]
            assert_true $? "Host build/native is not a symbolic link"
            native_target=$(readlink "$plain_project/build/native")
            [ -d "$plain_project/build/$native_target" ]
            assert_true $? "Host build/native does not point to the normalized target directory"
            rm "$plain_project/build/native"
            ln -s stale-target "$plain_project/build/native"
            invoke_external "0" "$plain_project" "$cli_exe" build
            [ "$(readlink "$plain_project/build/native")" != "stale-target" ]
            assert_true $? "Host build did not refresh a stale build/native link"
            ;;
    esac

    invoke_external "0" "$plain_project" "$cli_exe" run
    assert_contains "$LAST_OUTPUT" "plain-project" "Plain project run output was unexpected"

    invoke_external "0" "$plain_project" "$cli_exe" test
    assert_contains "$LAST_OUTPUT" "test-smoke" "Plain project test output was unexpected"
    assert_contains "$LAST_OUTPUT" "[TEST]" "Test command did not emit a test report"
    assert_contains "$LAST_OUTPUT" "status: PASS" "Test command did not emit a pass status"

    failing_test_project=$(copy_scenario_project "dh-c/tests/fixture/failing-test-project")
    invoke_external "7" "$failing_test_project" "$cli_exe" test
    assert_contains "$LAST_OUTPUT" "[TEST]" "Failing test did not emit a test report"
    assert_contains "$LAST_OUTPUT" "status: FAIL" "Failing test did not report failure"
    assert_contains "$LAST_OUTPUT" "exit: 7" "Failing test did not preserve the child exit code"

    invoke_external "0" "$plain_project" "$cli_exe" clean
    if [ -e "$plain_project/build" ]; then
        printf 'Plain project clean did not remove build directory\n' >&2
        exit 1
    fi

    link_contract_root="$temp_root/link-contracts"
    mkdir -p "$link_contract_root/freestanding/src" "$link_contract_root/custom-entry/src"
    cat >"$link_contract_root/freestanding/project.dh" <<'EOF'
output=freestanding-hosted
link-dsl=off
EOF
    cat >"$link_contract_root/freestanding/src/main.c" <<'EOF'
int main(void) { return 0; }
EOF
    invoke_external "0" "$link_contract_root/freestanding" "$cli_exe" build --freestanding
    assert_contains "$LAST_OUTPUT" "Build successful!" "Freestanding compilation with the default link model did not build"

    cat >"$link_contract_root/custom-entry/project.dh" <<'EOF'
output=custom-entry
link-dsl=off
EOF
    cat >"$link_contract_root/custom-entry/src/entry.c" <<'EOF'
#if defined(_MSC_VER)
__declspec(noreturn) void dh_entry(void);
__declspec(noreturn) void dh_entry(void) { for (;;) {} }
#else
__attribute__((noreturn)) void dh_entry(void);
__attribute__((noreturn)) void dh_entry(void) { for (;;) {} }
#endif
EOF

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-start-files=off --entry=dh_entry
    assert_contains "$LAST_OUTPUT" "Build successful!" "No-start-files executable contract did not build"
    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-crt=off --entry=dh_entry
    assert_contains "$LAST_OUTPUT" "Build successful!" "No-CRT alias contract did not build"
    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-stdlib=off --entry=dh_entry
    assert_contains "$LAST_OUTPUT" "Build successful!" "No-stdlib executable contract did not build"
    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-default-libs=off --link-start-files=off --entry=dh_entry
    assert_contains "$LAST_OUTPUT" "Build successful!" "Independent default-library/start-file contract did not build"
    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean

    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*)
            invoke_external "1" "$link_contract_root/custom-entry" "$cli_exe" build --link-libc=off --link-start-files=off --entry=dh_entry
            assert_contains "$LAST_OUTPUT" "cannot be represented" "Windows no-libc contract did not fail explicitly"
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-libc=off --link-default-libs=off --link-start-files=off --entry=dh_entry
            assert_contains "$LAST_OUTPUT" "Build successful!" "Windows no-libc/no-default-libs contract did not build"
            ;;
        *)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" build --link-libc=off --link-start-files=off --entry=dh_entry
            assert_contains "$LAST_OUTPUT" "Build successful!" "No-libc executable contract did not build"
            ;;
    esac

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" lib --shared --link-default-libs=off --link-start-files=off --entry=dh_entry
            ;;
        *)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" lib --shared --link-default-libs=off
            ;;
    esac
    assert_contains "$LAST_OUTPUT" "Build successful!" "No-default-libraries shared-library contract did not build"

    invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" lib --shared --link-stdlib=off --entry=dh_entry
            ;;
        *)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" lib --shared --link-stdlib=off
            ;;
    esac
    assert_contains "$LAST_OUTPUT" "Build successful!" "No-stdlib shared-library contract did not build"

    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*) ;;
        *)
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" clean
            invoke_external "0" "$link_contract_root/custom-entry" "$cli_exe" lib --shared --link-libc=off
            assert_contains "$LAST_OUTPUT" "Build successful!" "No-libc shared-library contract did not build"
            ;;
    esac

    target_root_contract=$(copy_scenario_project "dh-c/lab/target-root-contract")
    invoke_external "0" "$target_root_contract" "$cli_exe" build cmd/runner1 --link-dsl=off
    assert_contains "$LAST_OUTPUT" "Build successful!" "Target-root executable build did not succeed"
    assert_build_artifacts_exist "$target_root_contract" "runner1$exe_ext"

    invoke_external "0" "$target_root_contract" "$cli_exe" run cmd/runner1 --link-dsl=off

    invoke_external "0" "$target_root_contract" "$cli_exe" build plugins/render --link-dsl=off
    assert_contains "$LAST_OUTPUT" "Build successful!" "Target-root plugin build did not succeed"
    assert_build_artifacts_exist "$target_root_contract" "$shared_pattern"

    invoke_external "0" "$target_root_contract" "$cli_exe" clean

    target_root_compat=$(copy_scenario_project "dh-c/lab/target-root-compat")
    invoke_external "0" "$target_root_compat" "$cli_exe" build --sample --link-dsl=off
    assert_contains "$LAST_OUTPUT" "Build successful!" "Sample target build did not succeed"

    invoke_external "0" "$target_root_compat" "$cli_exe" build --example --link-dsl=off
    assert_contains "$LAST_OUTPUT" "Build successful!" "Example target build did not succeed"

    invoke_external "0" "$target_root_compat" "$cli_exe" test --example example-usage.c --link-dsl=off
    assert_contains "$LAST_OUTPUT" "status: PASS" "Selected example test did not succeed"

    invoke_external "0" "$target_root_compat" "$cli_exe" clean

    recursive_dsl_project=$(copy_scenario_project "dh-c/tests/fixture/recursive-dsl-project")
    invoke_external "0" "$recursive_dsl_project" "$cli_exe" test --recur
    assert_contains "$LAST_OUTPUT" "fixture: recursive dsl reaches current project" "Recursive test did not include current project tests"
    assert_occurrences "$LAST_OUTPUT" "fixture-dep: recursive dsl includes dependency once" "1" "Recursive test ran dependency tests outside the aggregate"

    lib_kind_project=$(copy_scenario_project "dh-c/tests/fixture/lib-kind-project")
    invoke_external "0" "$lib_kind_project" "$cli_exe" build
    assert_contains "$LAST_OUTPUT" "Build successful!" "Project kind=lib build did not succeed"
    assert_build_artifacts_exist "$lib_kind_project" "$lib_kind_static_pattern" "$lib_kind_shared_pattern"
    assert_build_artifacts_absent "$lib_kind_project" "$lib_kind_lto_static_pattern"
    if [ -n "$lib_kind_import_pattern" ]; then
        assert_build_artifacts_exist "$lib_kind_project" "$lib_kind_import_pattern"
    fi
    dev_manifest=$("$find_bin" "$lib_kind_project/build" -path '*/dev/manifest.dh' -type f | head -n 1)
    [ -n "$dev_manifest" ]
    assert_true $? "Development kind=lib build did not generate manifest.dh"
    assert_contains "$(cat "$dev_manifest")" "artifact=static|" "Manifest did not record the native static library"
    assert_contains "$(cat "$dev_manifest")" "artifact=shared|" "Manifest did not record the shared library"
    if [ -n "$lib_kind_import_pattern" ]; then
        assert_contains "$(cat "$dev_manifest")" "artifact=import|" "Manifest did not record the Windows import library"
    fi
    dev_manifest_before=$(cat "$dev_manifest")
    invoke_external "0" "$lib_kind_project" "$cli_exe" test
    assert_contains "$LAST_OUTPUT" "[TEST]" "kind=lib test did not emit the structured report"
    assert_contains "$LAST_OUTPUT" "status: PASS" "kind=lib test did not pass"
    assert_occurrences "$LAST_OUTPUT" "Build successful!" "0" "kind=lib test repeated generic build-success banners"
    assert_contains "$LAST_OUTPUT" "[TIMING]" "default test output omitted the timing summary"
    assert_contains "$LAST_OUTPUT" "project library:" "default test output omitted project-library timing"
    assert_contains "$LAST_OUTPUT" "executable:" "default test output omitted executable timing"
    assert_contains "$LAST_OUTPUT" "execution:" "default test output omitted execution timing"
    assert_build_artifacts_exist "$lib_kind_project" "$lib_kind_static_pattern" "$lib_kind_shared_pattern"
    [ "$(cat "$dev_manifest")" = "$dev_manifest_before" ]
    assert_true $? "Test executable overwrote the prebuilt library manifest"

    invoke_external "0" "$lib_kind_project" "$cli_exe" test --verbose
    assert_contains "$LAST_OUTPUT" "[TIMING]" "verbose test output omitted the timing summary"
    assert_contains "$LAST_OUTPUT" "project library:" "verbose test output omitted project-library timing"
    assert_contains "$LAST_OUTPUT" "executable:" "verbose test output omitted executable timing"
    assert_contains "$LAST_OUTPUT" "execution:" "verbose test output omitted execution timing"
    [ "$(cat "$dev_manifest")" = "$dev_manifest_before" ]
    assert_true $? "Verbose test execution changed the prebuilt library manifest"

    invoke_external "0" "$lib_kind_project" "$cli_exe" test --sample sample-core.c
    assert_contains "$LAST_OUTPUT" "status: PASS" "kind=lib sample consumer did not pass"
    [ "$(cat "$dev_manifest")" = "$dev_manifest_before" ]
    assert_true $? "Sample executable overwrote the prebuilt library manifest"

    invoke_external "0" "$lib_kind_project" "$cli_exe" test --example example-core.c
    assert_contains "$LAST_OUTPUT" "status: PASS" "kind=lib example consumer did not pass"
    [ "$(cat "$dev_manifest")" = "$dev_manifest_before" ]
    assert_true $? "Example executable overwrote the prebuilt library manifest"

    invoke_external "0" "$lib_kind_project" "$cli_exe" package
    if ! "$find_bin" "$lib_kind_project/package" -type f -name "$lib_kind_static_pattern" | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Library package did not stage the native static archive\n' >&2
        exit 1
    fi
    if ! "$find_bin" "$lib_kind_project/package" -type f -name "$lib_kind_shared_pattern" | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Library package did not stage the shared library\n' >&2
        exit 1
    fi
    if [ -n "$lib_kind_import_pattern" ] && ! "$find_bin" "$lib_kind_project/package" -type f -name "$lib_kind_import_pattern" | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Library package did not stage the Windows import library\n' >&2
        exit 1
    fi
    if "$find_bin" "$lib_kind_project/package" -type f -name manifest.dh | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Generic package incorrectly copied the prebuilt-only manifest.dh\n' >&2
        exit 1
    fi

    invoke_external "0" "$lib_kind_project" "$cli_exe" package --layout=prebuilt
    prebuilt_manifest=$("$find_bin" "$lib_kind_project/prebuilt" -path '*/dev/manifest.dh' -type f | head -n 1)
    [ -n "$prebuilt_manifest" ]
    assert_true $? "Prebuilt package did not promote manifest.dh"
    prebuilt_profile=$(dirname "$prebuilt_manifest")
    [ -d "$prebuilt_profile/libs" ]
    assert_true $? "Prebuilt package did not promote libs/"
    if "$find_bin" "$prebuilt_profile" -type d -name obj | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Prebuilt package included producer object state\n' >&2
        exit 1
    fi
    if "$find_bin" "$prebuilt_profile" -type f -name '*.rsp' | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Prebuilt package included producer response files\n' >&2
        exit 1
    fi

    invoke_external "0" "$lib_kind_project" "$cli_exe" build stable
    assert_contains "$LAST_OUTPUT" "Build successful!" "Stable project kind=lib build did not succeed"
    assert_build_artifacts_exist \
        "$lib_kind_project" \
        "$lib_kind_static_pattern" \
        "$lib_kind_lto_static_pattern" \
        "$lib_kind_shared_pattern"
    stable_manifest=$("$find_bin" "$lib_kind_project/build" -path '*/stable/manifest.dh' -type f | head -n 1)
    [ -n "$stable_manifest" ]
    assert_true $? "Stable kind=lib build did not generate manifest.dh"
    assert_contains "$(cat "$stable_manifest")" "artifact=static-lto|" "Stable manifest did not record the LTO static library"

    deps_graph_root=$(copy_scenario_project "dh-c/tests/fixture/deps-graph")
    deps_graph_project="$deps_graph_root/C"

    invoke_external "0" "$deps_graph_project" "$cli_exe" graph
    assert_contains "$LAST_OUTPUT" "PROJECT C" "Graph output did not identify the root project"
    assert_contains "$LAST_OUTPUT" "- B [provider=dh" "Graph output did not contain the direct dependency"
    assert_contains "$LAST_OUTPUT" "- A [provider=dh" "Graph output did not contain the transitive dependency"

    invoke_external "0" "$deps_graph_project" "$cli_exe" graph --format=dot
    assert_contains "$LAST_OUTPUT" "digraph dh_c" "DOT graph output did not contain a graph declaration"
    assert_contains "$LAST_OUTPUT" "[label=\"B" "DOT graph output did not contain the direct dependency"
    deps_graph_dir="$deps_graph_project/lib/deps"
    rm -rf "$deps_graph_dir"

    invoke_external "0" "$deps_graph_project" "$cli_exe" deps --verbose
    [ -d "$deps_graph_dir" ]
    assert_true $? "Dependency build did not create lib/deps"
    [ -f "$deps_graph_dir/A.h" ]
    assert_true $? "Dependency build did not copy transitive header"
    [ -f "$deps_graph_dir/B.h" ]
    assert_true $? "Dependency build did not copy direct dependency header"
    if ! "$find_bin" "$deps_graph_dir" -type f -name "$static_lib_pattern" | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Dependency build did not copy expected static libraries\n' >&2
        exit 1
    fi
    [ -f "$deps_graph_dir/A.h.pch" ]
    assert_true $? "Dependency build did not copy transitive PCH"
    [ -f "$deps_graph_dir/B.h.pch" ]
    assert_true $? "Dependency build did not copy direct dependency PCH"
fi

printf 'All dh-c tests passed.\n'
