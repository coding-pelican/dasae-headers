#!/bin/sh
set -eu

repo_root=$(git rev-parse --show-toplevel)
hook_path=$(git rev-parse --git-path hooks/pre-push)
legacy_hook_path=$(git rev-parse --git-path hooks/pre-commit)
managed_marker="# dasae-headers verify-native pre-push"
legacy_managed_marker="# dasae-headers verify-native"
action=${1:-install}

if [ "$#" -gt 1 ]; then
    echo "usage: scripts/git-hooks.sh [install|uninstall|status|stop|log]" >&2
    exit 2
fi

resolve_hook_path() {
    case "$1" in
        /*|[A-Za-z]:/*)
            printf '%s\n' "$1"
            ;;
        *)
            printf '%s\n' "$repo_root/$1"
            ;;
    esac
}

hook_path=$(resolve_hook_path "$hook_path")
legacy_hook_path=$(resolve_hook_path "$legacy_hook_path")
state_dir=$(resolve_hook_path "$(git rev-parse --git-path dh-hooks)")
log_path="$state_dir/verify-native.log"
pid_path="$state_dir/verify-native.pid"

case "$action" in
    install)
        if [ -f "$hook_path" ] && ! grep -Fqx "$managed_marker" "$hook_path"; then
            echo "git-hooks: existing pre-push hook is not managed by this repository" >&2
            exit 1
        fi
        dh_c_path=$(command -v dh-c || true)
        if [ -x /usr/bin/bash ]; then
            bash_path=/usr/bin/bash
        else
            bash_path=$(command -v bash || true)
        fi
        if [ -z "$bash_path" ]; then
            echo "git-hooks: bash is required to install the pre-push hook" >&2
            exit 127
        fi

        mkdir -p "$(dirname "$hook_path")"
        {
            echo '#!/bin/sh'
            echo "$managed_marker"
            echo 'set -eu'
            echo 'repo_root=$(git rev-parse --show-toplevel)'
            if [ -n "$dh_c_path" ]; then
                echo "export DH_C='$dh_c_path'"
            fi
            echo "exec '$bash_path' \"\$repo_root/scripts/verify-native.sh\""
        } >"$hook_path"
        chmod +x "$hook_path"

        if [ -f "$legacy_hook_path" ] && grep -Fqx "$legacy_managed_marker" "$legacy_hook_path"; then
            rm -f "$legacy_hook_path"
        fi

        echo "git-hooks: installed $hook_path"
        ;;
    uninstall)
        removed=false
        if [ -f "$hook_path" ] && grep -Fqx "$managed_marker" "$hook_path"; then
            rm -f "$hook_path"
            removed=true
        fi
        if [ -f "$legacy_hook_path" ] && grep -Fqx "$legacy_managed_marker" "$legacy_hook_path"; then
            rm -f "$legacy_hook_path"
            removed=true
        fi
        if [ "$removed" = true ]; then
            echo "git-hooks: removed repository-managed hooks"
        else
            echo "git-hooks: no repository-managed hooks installed"
        fi
        ;;
    status)
        if [ ! -f "$pid_path" ]; then
            echo "git-hooks: verification is not running"
            exit 1
        fi
        pid=$(cat "$pid_path")
        if [ -n "$pid" ] && kill -0 "$pid" 2>/dev/null; then
            echo "git-hooks: verification is running with process $pid"
            exit 0
        fi
        rm -f "$pid_path"
        echo "git-hooks: verification is not running"
        exit 1
        ;;
    stop)
        if [ ! -f "$pid_path" ]; then
            echo "git-hooks: verification is not running"
            exit 1
        fi
        pid=$(cat "$pid_path")
        if [ -n "$pid" ] && kill -0 "$pid" 2>/dev/null; then
            case "$(uname -s)" in
                MINGW*|MSYS*|CYGWIN*)
                    win_pid=$(ps -p "$pid" -o winpid= 2>/dev/null | tr -d '[:space:]')
                    if [ -n "$win_pid" ]; then
                        taskkill.exe //PID "$win_pid" //T //F >/dev/null
                    else
                        kill -TERM "$pid"
                    fi
                    ;;
                *)
                    pkill -TERM -P "$pid" 2>/dev/null || true
                    kill -TERM "$pid"
                    ;;
            esac
            echo "git-hooks: stopping verification process $pid"
            exit 0
        fi
        rm -f "$pid_path"
        echo "git-hooks: verification is not running"
        exit 1
        ;;
    log)
        if [ ! -f "$log_path" ]; then
            echo "git-hooks: verification log does not exist" >&2
            exit 1
        fi
        cat "$log_path"
        ;;
    *)
        echo "usage: scripts/git-hooks.sh [install|uninstall|status|stop|log]" >&2
        exit 2
        ;;
esac
