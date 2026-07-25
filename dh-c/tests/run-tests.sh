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
assert_contains "$LAST_OUTPUT" "BUILD AND EXECUTION:" "Help output did not group build commands"
assert_contains "$LAST_OUTPUT" "DEPENDENCIES AND DELIVERY:" "Help output did not group dependency commands"
assert_contains "$LAST_OUTPUT" "INSPECTION AND TOOLING:" "Help output did not group inspection commands"
assert_contains "$LAST_OUTPUT" "ALIASES:" "Help output did not separate compatibility aliases"
assert_contains "$LAST_OUTPUT" "help --all" "Concise help did not point to full help"

invoke_external "0" "$repo_root" "$cli_exe" help --list
assert_contains "$LAST_OUTPUT" "build" "Help list did not contain build"
assert_contains "$LAST_OUTPUT" "test" "Help list did not contain test"

invoke_external "0" "$repo_root" "$cli_exe" help --all
assert_contains "$LAST_OUTPUT" "RESERVED COMMANDS:" "Help output did not describe reserved commands"
assert_contains "$LAST_OUTPUT" "COMMAND OPTION BOUNDARIES:" "Help output did not describe command option boundaries"
assert_contains "$LAST_OUTPUT" "PROJECT.DH KEYS:" "Help output did not describe project.dh keys"
assert_contains "$LAST_OUTPUT" "LINK MODEL:" "Help output did not describe freestanding/link-model separation"
assert_contains "$LAST_OUTPUT" 'exact alias of `--link-start-files=off`' "Help output did not explain the CRT alias"
assert_contains "$LAST_OUTPUT" "durable source assets" "Help output did not describe cleanup-owned generated paths"

invoke_external "0" "$repo_root" "$cli_exe" help build
assert_contains "$LAST_OUTPUT" "USAGE:" "Command help did not show usage"
assert_contains "$LAST_OUTPUT" "CONTRACT:" "Command help did not show contract"
assert_contains "$LAST_OUTPUT" "--output-ext" "Build help did not describe explicit output extension"

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

invoke_external "1" "$repo_root" "$cli_exe" workspace demo
assert_contains "$LAST_OUTPUT" "Not implemented: workspace" "Reserved workspace command did not report current status"

invoke_external "1" "$repo_root" "$cli_exe" project demo
assert_contains "$LAST_OUTPUT" "Not implemented: project" "Reserved project command did not report current status"

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
assert_contains "$LAST_OUTPUT" "Preserved dirty dependency checkout" "dependency cleanup did not preserve a dirty checkout"
[ -d "$lock_project/.dh-c/deps/src/dep" ]
assert_true $? "dependency cleanup removed a dirty checkout without --force"
invoke_external "0" "$lock_project" "$cli_exe" clean --deps --older-than=0s --force
[ ! -e "$lock_project/.dh-c/deps/src/dep" ]
assert_true $? "forced dependency cleanup did not remove the dirty checkout"
rm -rf "$lock_contract_root"

provider_contract_root=$(mktemp -d "${TMPDIR:-/tmp}/dh-c-provider-contract.XXXXXX")
provider_bin="$provider_contract_root/bin"
provider_source="$provider_contract_root/source"
provider_sysroot="$provider_contract_root/sysroot"
mkdir -p "$provider_bin" "$provider_source" "$provider_sysroot"
provider_source_native=$(native_path "$provider_source")
provider_sysroot_native=$(native_path "$provider_sysroot")
provider_probe="$repo_root/dh-c/tests/provider-probe.c"
provider_cmake="$provider_bin/cmake$exe_ext"
provider_make="$provider_bin/make$exe_ext"
clang -std=gnu17 -Wall -Wextra -Werror -o "$provider_cmake" "$provider_probe"
cp "$provider_cmake" "$provider_make"
chmod +x "$provider_cmake" "$provider_make"

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
invoke_external "0" "$cmake_project" env PATH="$provider_bin:$PATH" DH_TEST_PROVIDER_LOG="$cmake_log_native" "$cli_exe" deps dev --target=aarch64-w64-windows-gnu --sysroot="$provider_sysroot_native" --compiler=clang-cross
cmake_text=$(cat "$cmake_log")
assert_contains "$cmake_text" "-DCMAKE_C_COMPILER=clang-cross" "CMake provider did not receive the effective compiler"
assert_contains "$cmake_text" "-DCMAKE_AR=llvm-ar" "CMake provider did not receive the archiver"
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
invoke_external "0" "$make_project" env PATH="$provider_bin:$PATH" DH_TEST_PROVIDER_LOG="$make_log_native" "$cli_exe" deps dev --target=aarch64-w64-windows-gnu --sysroot="$provider_sysroot_native" --compiler=clang-cross
make_text=$(cat "$make_log")
assert_contains "$make_text" "DH_DEP_TARGET=aarch64-w64-windows-gnu" "Make provider did not receive the target triple"
assert_contains "$make_text" "DH_DEP_CC=clang-cross" "Make provider did not receive the effective compiler"
assert_contains "$make_text" "DH_DEP_AR=llvm-ar" "Make provider did not receive the archiver"
assert_contains "$make_text" "DH_DEP_SYSROOT=$provider_sysroot_native" "Make provider did not receive the sysroot"
assert_contains "$make_text" "DH_DEP_CFLAGS=--target=aarch64-w64-windows-gnu --sysroot=$provider_sysroot_native" "Make provider did not receive target C flags"
rm -rf "$provider_contract_root"

if [ "$integration" -eq 1 ]; then
    reset_temp_root

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
