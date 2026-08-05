#!/usr/bin/env bash
set -euo pipefail

repo_root=$(git rev-parse --show-toplevel)
dh_root="$repo_root/dh"
dh_c_root="$repo_root/dh-c"
state_dir=$(git rev-parse --git-path dh-hooks)
find_bin=find

if [ -x /usr/bin/find ]; then
    find_bin=/usr/bin/find
fi

case "$state_dir" in
    /*|[A-Za-z]:/*)
        ;;
    *)
        state_dir="$repo_root/$state_dir"
        ;;
esac

mkdir -p "$state_dir"
log_path="$state_dir/verify-native.log"
pid_path="$state_dir/verify-native.pid"

if [ -f "$pid_path" ]; then
    active_pid=$(cat "$pid_path")
    if [[ "$active_pid" =~ ^[0-9]+$ ]] && kill -0 "$active_pid" 2>/dev/null; then
        echo "verify-native: verification is already running with process $active_pid" >&2
        exit 3
    fi
    rm -f "$pid_path"
fi

printf '%s\n' "$BASHPID" >"$pid_path"
child_pid=
cleanup() {
    rm -f "$pid_path"
}
stop_child() {
    if [ -z "$child_pid" ] || ! kill -0 "$child_pid" 2>/dev/null; then
        return
    fi
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*)
            win_pid=$(ps -p "$child_pid" -o winpid= 2>/dev/null | tr -d '[:space:]')
            if [ -n "$win_pid" ]; then
                taskkill.exe //PID "$win_pid" //T //F >/dev/null 2>&1 || true
            else
                kill -TERM "$child_pid" 2>/dev/null || true
            fi
            ;;
        *)
            pkill -TERM -P "$child_pid" 2>/dev/null || true
            kill -TERM "$child_pid" 2>/dev/null || true
            ;;
    esac
}
cancel() {
    trap '' INT TERM HUP
    stop_child
    exit 130
}
trap cleanup EXIT
trap cancel INT TERM HUP
exec > >(tee "$log_path") 2>&1

run_in() {
    working_dir=$1
    shift
    echo
    echo "verify-native: $working_dir: $*"
    (
        cd "$working_dir"
        "$@"
    ) &
    child_pid=$!
    if wait "$child_pid"; then
        child_pid=
    else
        status=$?
        child_pid=
        return "$status"
    fi
}

case $(uname -s) in
    MINGW*|MSYS*|CYGWIN*)
        cross_target=x86_64-linux-gnu
        exe_ext=.exe
        ;;
    Linux*)
        cross_target=x86_64-w64-windows-gnu
        exe_ext=
        ;;
    *)
        echo "verify-native: supported native hosts are Windows and Linux" >&2
        exit 2
        ;;
esac

run_in "$repo_root" sh dh-c/tests/run-tests.sh --integration

for profile in test release; do
    run_in "$dh_c_root" make "PROFILE=$profile"
    run_in "$repo_root" "$dh_c_root/build/$profile/dh-c$exe_ext" --version
done

dh_c="$dh_c_root/build/release/dh-c$exe_ext"

run_project() {
    project_root=$1
    shift
    run_in "$project_root" "$dh_c" "$@" --dh="$dh_root"
}

has_c_sources() {
    source_root=$1
    [ -d "$source_root" ] && "$find_bin" "$source_root" -type f -name '*.c' -print -quit | grep -q .
}

has_headers() {
    include_root=$1
    [ -d "$include_root" ] && "$find_bin" "$include_root" -type f -name '*.h' -print -quit | grep -q .
}

syntax_headers() {
    project_root=$1
    profile=$2
    target_args=$3

    while IFS= read -r header; do
        if [ "$target_args" = cross ]; then
            run_project "$project_root" syntax "$profile" \
                --target="$cross_target" \
                --freestanding \
                --link-libc=off \
                --link-default-libs=off \
                --file="$header"
        else
            run_project "$project_root" syntax "$profile" --file="$header"
        fi
    done < <("$find_bin" "$project_root/include" -type f -name '*.h' -print)
}

verify_project() {
    project_root=$1
    project_name=$2

    echo
    echo "verify-native: verifying $project_name"

    for profile in test release; do
        if has_c_sources "$project_root/tests"; then
            if [ "$profile" = test ]; then
                run_project "$project_root" test --test
            else
                run_project "$project_root" test "$profile" --test
            fi
        else
            echo "verify-native: [SKIP] $project_name tests: no C sources"
        fi

        if has_c_sources "$project_root/samples"; then
            run_project "$project_root" build "$profile" --sample
        else
            echo "verify-native: [SKIP] $project_name samples: no C sources"
        fi

        if has_c_sources "$project_root/examples"; then
            run_project "$project_root" build "$profile" --example
        else
            echo "verify-native: [SKIP] $project_name examples: no C sources"
        fi

        if has_c_sources "$project_root/src"; then
            run_project "$project_root" syntax "$profile" \
                --target="$cross_target" \
                --freestanding \
                --link-libc=off \
                --link-default-libs=off \
                --all
        elif has_headers "$project_root/include"; then
            syntax_headers "$project_root" "$profile" cross
        else
            echo "verify-native: [SKIP] $project_name sources: no C sources or public headers"
        fi

        if has_c_sources "$project_root/tests"; then
            run_project "$project_root" syntax "$profile" \
                --target="$cross_target" \
                --freestanding \
                --link-libc=off \
                --link-default-libs=off \
                --test
        fi
        if has_c_sources "$project_root/samples"; then
            run_project "$project_root" syntax "$profile" \
                --target="$cross_target" \
                --freestanding \
                --link-libc=off \
                --link-default-libs=off \
                --sample
        fi
        if has_c_sources "$project_root/examples"; then
            run_project "$project_root" syntax "$profile" \
                --target="$cross_target" \
                --freestanding \
                --link-libc=off \
                --link-default-libs=off \
                --example
        fi
    done
}

verify_source() {
    source_path=$1
    source_name=$2
    source_root=$(dirname "$source_path")

    echo
    echo "verify-native: verifying $source_name"
    for profile in test release; do
        run_project "$source_root" build "$profile" "$source_path"
        run_project "$source_root" syntax "$profile" "$source_path" \
            --target="$cross_target" \
            --freestanding \
            --link-libc=off \
            --link-default-libs=off
    done
}

verify_header_project() {
    project_root=$1
    project_name=$2

    echo
    echo "verify-native: verifying $project_name public headers"
    for profile in test release; do
        syntax_headers "$project_root" "$profile" native
    done
}

verify_project "$dh_root" dh
verify_project "$repo_root/dh-examples/dacolor" dacolor
verify_project "$repo_root/dh-examples/dansi" dansi
verify_project "$repo_root/dh-examples/daterm" daterm
verify_project "$repo_root/dh-examples/Grid" Grid
verify_source "$repo_root/dh-examples/extra/coding-pelican.c" coding-pelican.c
verify_header_project "$repo_root/dh-extras/sys/api/windows/wnd" dh-extras/wnd
verify_header_project "$repo_root/dh-extras/sys/call/linux/x11" dh-extras/x11

echo
echo "verify-native: repository native and cross-target verification passed"
