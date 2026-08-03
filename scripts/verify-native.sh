#!/usr/bin/env bash
set -euo pipefail

repo_root=$(git rev-parse --show-toplevel)
dh_root="$repo_root/dh"
profile=${DH_VERIFY_PROFILE:-dev}
state_dir=$(git rev-parse --git-path dh-hooks)

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

if [ -n "${DH_C:-}" ]; then
    dh_c=$DH_C
elif command -v dh-c >/dev/null 2>&1; then
    dh_c=$(command -v dh-c)
elif [ -x "$HOME/.dh-c/dh-c" ]; then
    dh_c="$HOME/.dh-c/dh-c"
elif [ -x "$HOME/.dh-c/dh-c.exe" ]; then
    dh_c="$HOME/.dh-c/dh-c.exe"
else
    echo "verify-native: dh-c is not available on PATH" >&2
    exit 127
fi

run_dh() {
    echo
    echo "verify-native: $dh_c $*"
    (
        cd "$dh_root"
        "$dh_c" "$@"
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
        ;;
    Linux*)
        cross_target=x86_64-w64-windows-gnu
        ;;
    *)
        echo "verify-native: supported native hosts are Windows and Linux" >&2
        exit 2
        ;;
esac

run_dh test "$profile" --test
run_dh syntax "$profile" \
    --target="$cross_target" \
    --freestanding \
    --link-libc=off \
    --link-default-libs=off \
    --all

echo
echo "verify-native: native tests and $cross_target syntax passed"
