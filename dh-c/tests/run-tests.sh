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
        static_lib_pattern="*.lib"
        ;;
    *)
        exe_ext=""
        shared_pattern="*render*.so"
        lib_kind_shared_pattern="libcore.so"
        lib_kind_static_pattern="libcore.a"
        lib_kind_lto_static_pattern="libcore.lto.a"
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
    "$find_bin" "$root" -type d \( -name build -o -name .cache \) -prune -print | while IFS= read -r dir; do
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
assert_contains "$LAST_OUTPUT" "help --all" "Concise help did not point to full help"

invoke_external "0" "$repo_root" "$cli_exe" help --list
assert_contains "$LAST_OUTPUT" "build" "Help list did not contain build"
assert_contains "$LAST_OUTPUT" "test" "Help list did not contain test"

invoke_external "0" "$repo_root" "$cli_exe" help --all
assert_contains "$LAST_OUTPUT" "RESERVED COMMANDS:" "Help output did not describe reserved commands"
assert_contains "$LAST_OUTPUT" "COMMAND OPTION BOUNDARIES:" "Help output did not describe command option boundaries"
assert_contains "$LAST_OUTPUT" "PROJECT.DH KEYS:" "Help output did not describe project.dh keys"
assert_contains "$LAST_OUTPUT" "durable source assets" "Help output did not describe cleanup-owned generated paths"

invoke_external "0" "$repo_root" "$cli_exe" help build
assert_contains "$LAST_OUTPUT" "USAGE:" "Command help did not show usage"
assert_contains "$LAST_OUTPUT" "CONTRACT:" "Command help did not show contract"
assert_contains "$LAST_OUTPUT" "--output-ext" "Build help did not describe explicit output extension"

invoke_external "0" "$repo_root" "$cli_exe" help clean
assert_contains "$LAST_OUTPUT" "Do not store durable source assets" "Clean help did not describe cleanup-owned generated paths"

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

if [ "$integration" -eq 1 ]; then
    reset_temp_root

    plain_project=$(copy_scenario_project "dh-c/tests/fixture/plain-project")
    invoke_external "0" "$plain_project" "$cli_exe" build
    assert_contains "$LAST_OUTPUT" "Build successful!" "Plain project build did not succeed"
    assert_build_artifacts_exist "$plain_project" "plain-project$exe_ext"

    invoke_external "0" "$plain_project" "$cli_exe" run
    assert_contains "$LAST_OUTPUT" "plain-project" "Plain project run output was unexpected"

    invoke_external "0" "$plain_project" "$cli_exe" test
    assert_contains "$LAST_OUTPUT" "test-smoke" "Plain project test output was unexpected"
    assert_contains "$LAST_OUTPUT" "[TEST REPORT] status=PASS" "Test command did not emit a pass report"

    invoke_external "0" "$plain_project" "$cli_exe" clean
    if [ -e "$plain_project/build" ]; then
        printf 'Plain project clean did not remove build directory\n' >&2
        exit 1
    fi

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
    assert_contains "$LAST_OUTPUT" "Build successful!" "Selected example test did not succeed"

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

    invoke_external "0" "$lib_kind_project" "$cli_exe" build stable
    assert_contains "$LAST_OUTPUT" "Build successful!" "Stable project kind=lib build did not succeed"
    assert_build_artifacts_exist \
        "$lib_kind_project" \
        "$lib_kind_static_pattern" \
        "$lib_kind_lto_static_pattern" \
        "$lib_kind_shared_pattern"

    if ! "$find_bin" "$lib_kind_project/build" -type f -name manifest.dh | "$grep_bin" . >/dev/null 2>&1; then
        printf 'Build did not generate manifest.dh\n' >&2
        exit 1
    fi

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
