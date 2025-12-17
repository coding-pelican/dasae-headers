#!/usr/bin/env bash
#
# `dasae-headers` Installation Script
#   - GitHub: <https://github.com/coding-pelican/dasae-headers>
#
# This script helps set up `dasae-headers` development environment:
#   - Detects OS and environment
#   - Checks/installs toolchain (Clang/GCC)
#   - Compiles and installs `dh-c` build tool
#   - Registers `DH_HOME` environment variable
#
# Usage: ./install.sh [OPTIONS]
#   --help          Show help message
#   --local         Local-only installation (no system changes)
#   --global        Global installation (may require sudo)
#   --skip-toolchain    Skip toolchain setup
#   --skip-dhc          Skip `dh-c` installation
#   --skip-env          Skip `DH_HOME` registration
#   --cc=PATH           Use specific compiler
#   --dh-home=PATH      Set `DH_HOME` to specific path
#   --prefix=PATH       Installation prefix for `dh-c`
#   --non-interactive   Use defaults without prompts
#   --dry-run           Show actions without executing
#   --print-env         Print environment commands for eval
#   --verbose           Show detailed output
#

set -euo pipefail

# ==============================================================================
# CONFIGURATION & CONSTANTS
# ==============================================================================

readonly SCRIPT_VERSION="0.1.0"
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Version requirements (from README specifications)
readonly CLANG_RECOMMENDED="19.1.0"
readonly CLANG_SUPPORTED="16.0.0"
readonly CLANG_MINIMUM="9.0.0"

readonly GCC_RECOMMENDED="15.1.0"
readonly GCC_SUPPORTED="13.1.0"
readonly GCC_MINIMUM=""  # TBU - not defined yet

# dh-c compilation settings
readonly DHC_SOURCE="dh-c.c"
readonly DHC_BINARY="dh-c"
# Note: -D_GNU_SOURCE enables POSIX functions like realpath, strdup, popen
readonly DHC_COMPILE_FLAGS="-std=c17 -D_GNU_SOURCE -Wall -Wextra -O3"

# Detected values (set during runtime)
declare OS_TYPE=""
declare OS_VARIANT=""
declare OS_VERSION=""
declare PACKAGE_MANAGER=""
declare SHELL_TYPE=""
declare SHELL_CONFIG_FILE=""
declare MSYS_ENVIRONMENT=""
declare TERMINAL_TYPE=""

declare DETECTED_CC=""
declare DETECTED_CC_PATH=""
declare DETECTED_CC_VERSION=""
declare DETECTED_CC_TIER=""

declare EXISTING_DH_HOME=""
declare EXISTING_DHC_PATH=""

# User selections (set during interactive phase)
declare INSTALL_SCOPE=""  # local|global
declare TOOLCHAIN_ACTION=""  # keep|install-local|install-global|update-local|update-global|skip
declare DHC_ACTION=""  # install-local|install-global|update-local|update-global|skip
declare ENV_ACTION=""  # register-local|register-global|update|skip

declare TARGET_CC=""
declare TARGET_DH_HOME=""
declare TARGET_DHC_PREFIX=""

# Command-line options
declare OPT_HELP=false
declare OPT_LOCAL=false
declare OPT_GLOBAL=false
declare OPT_SKIP_TOOLCHAIN=false
declare OPT_SKIP_DHC=false
declare OPT_SKIP_ENV=false
declare OPT_CC=""
declare OPT_DH_HOME=""
declare OPT_PREFIX=""
declare OPT_NON_INTERACTIVE=false
declare OPT_DRY_RUN=false
declare OPT_PRINT_ENV=false
declare OPT_VERBOSE=false

# ==============================================================================
# UTILITY FUNCTIONS
# ==============================================================================

# Terminal colors (auto-detect support)
setup_colors() {
    if [[ -t 1 ]] && [[ -z "${NO_COLOR:-}" ]]; then
        readonly C_RESET='\033[0m'
        readonly C_RED='\033[0;31m'
        readonly C_GREEN='\033[0;32m'
        readonly C_YELLOW='\033[0;33m'
        readonly C_BLUE='\033[0;34m'
        readonly C_MAGENTA='\033[0;35m'
        readonly C_CYAN='\033[0;36m'
        readonly C_WHITE='\033[0;37m'
        readonly C_BOLD='\033[1m'
        readonly C_DIM='\033[2m'
    else
        readonly C_RESET=''
        readonly C_RED=''
        readonly C_GREEN=''
        readonly C_YELLOW=''
        readonly C_BLUE=''
        readonly C_MAGENTA=''
        readonly C_CYAN=''
        readonly C_WHITE=''
        readonly C_BOLD=''
        readonly C_DIM=''
    fi
}

log_info() {
    echo -e "${C_BLUE}[INFO]${C_RESET} $*"
}

log_success() {
    echo -e "${C_GREEN}[OK]${C_RESET} $*"
}

log_warn() {
    echo -e "${C_YELLOW}[WARN]${C_RESET} $*" >&2
}

log_error() {
    echo -e "${C_RED}[ERROR]${C_RESET} $*" >&2
}

log_verbose() {
    if [[ "$OPT_VERBOSE" == true ]]; then
        echo -e "${C_DIM}[DEBUG]${C_RESET} $*"
    fi
}

log_dry_run() {
    echo -e "${C_MAGENTA}[DRY-RUN]${C_RESET} Would execute: $*"
}

# Print a header/section divider
print_header() {
    local title="$1"
    local width=60
    local padding=$(( (width - ${#title} - 2) / 2 ))
    echo ""
    echo -e "${C_CYAN}$(printf '=%.0s' $(seq 1 $width))${C_RESET}"
    echo -e "${C_CYAN}$(printf ' %.0s' $(seq 1 $padding)) ${C_BOLD}$title${C_RESET}${C_CYAN} $(printf ' %.0s' $(seq 1 $padding))${C_RESET}"
    echo -e "${C_CYAN}$(printf '=%.0s' $(seq 1 $width))${C_RESET}"
}

# Check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Compare two semantic versions: returns 0 if $1 >= $2, 1 otherwise
version_gte() {
    local v1="$1"
    local v2="$2"

    # Handle empty versions
    if [[ -z "$v1" ]]; then
        return 1
    fi
    if [[ -z "$v2" ]]; then
        return 0
    fi

    # Use sort -V if available (GNU coreutils)
    if printf '%s\n%s' "$v2" "$v1" | sort -V -C 2>/dev/null; then
        return 0
    fi

    # Fallback: manual comparison
    local IFS='.'
    local -a parts1=($v1)
    local -a parts2=($v2)

    local i
    for ((i = 0; i < ${#parts2[@]}; i++)); do
        local p1="${parts1[$i]:-0}"
        local p2="${parts2[$i]:-0}"

        # Remove non-numeric suffixes for comparison
        p1="${p1%%[!0-9]*}"
        p2="${p2%%[!0-9]*}"

        if ((p1 > p2)); then
            return 0
        elif ((p1 < p2)); then
            return 1
        fi
    done

    return 0
}

# Prompt user for yes/no confirmation
# Usage: prompt_yes_no "Question?" [default: y|n]
prompt_yes_no() {
    local prompt="$1"
    local default="${2:-y}"

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        [[ "$default" == "y" ]]
        return
    fi

    local yn_hint
    if [[ "$default" == "y" ]]; then
        yn_hint="[Y/n]"
    else
        yn_hint="[y/N]"
    fi

    local response
    while true; do
        echo -en "${C_YELLOW}$prompt${C_RESET} $yn_hint "
        read -r response
        response="${response:-$default}"
        response="${response,,}"  # lowercase

        case "$response" in
            y|yes) return 0 ;;
            n|no) return 1 ;;
            *) echo "Please answer yes or no." ;;
        esac
    done
}

# Prompt user to select from a menu
# Usage: prompt_select "Title" option1 option2 option3 ...
# Returns: Selected index (1-based) in $REPLY
prompt_select() {
    local title="$1"
    shift
    local -a options=("$@")

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        REPLY=1
        return 0
    fi

    echo ""
    echo -e "${C_BOLD}$title${C_RESET}"
    echo ""

    local i
    for ((i = 0; i < ${#options[@]}; i++)); do
        echo -e "  ${C_CYAN}[$((i + 1))]${C_RESET} ${options[$i]}"
    done
    echo ""

    local selection
    while true; do
        echo -en "${C_YELLOW}Select option [1-${#options[@]}]:${C_RESET} "
        read -r selection

        if [[ "$selection" =~ ^[0-9]+$ ]] && ((selection >= 1 && selection <= ${#options[@]})); then
            REPLY=$selection
            return 0
        else
            echo "Invalid selection. Please enter a number between 1 and ${#options[@]}."
        fi
    done
}

# Prompt user for a path input
# Usage: prompt_path "Prompt message" [default_value]
prompt_path() {
    local prompt="$1"
    local default="${2:-}"

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        REPLY="$default"
        return 0
    fi

    local hint=""
    if [[ -n "$default" ]]; then
        hint=" [${default}]"
    fi

    echo -en "${C_YELLOW}$prompt${C_RESET}${hint}: "
    read -r REPLY
    REPLY="${REPLY:-$default}"

    # Expand ~ to $HOME
    REPLY="${REPLY/#\~/$HOME}"
}

# Create a backup of a file before modifying
backup_file() {
    local file="$1"

    if [[ -f "$file" ]]; then
        local backup="${file}.backup.$(date +%Y%m%d_%H%M%S)"
        if [[ "$OPT_DRY_RUN" == true ]]; then
            log_dry_run "cp '$file' '$backup'"
        else
            cp "$file" "$backup"
            log_verbose "Created backup: $backup"
        fi
        echo "$backup"
    fi
}

# Execute a command (respects dry-run mode)
run_cmd() {
    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_dry_run "$*"
        return 0
    else
        log_verbose "Executing: $*"
        "$@"
    fi
}

# Execute a command with sudo if needed (respects dry-run mode)
run_sudo() {
    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_dry_run "sudo $*"
        return 0
    else
        log_verbose "Executing with sudo: $*"
        sudo "$@"
    fi
}

# ==============================================================================
# OS/ENVIRONMENT DETECTION
# ==============================================================================

detect_os() {
    print_header "Detecting Operating System"

    local uname_s
    uname_s="$(uname -s)"

    case "$uname_s" in
        Linux*)
            OS_TYPE="linux"
            detect_linux_variant
            ;;
        Darwin*)
            OS_TYPE="macos"
            detect_macos_variant
            ;;
        CYGWIN*|MINGW*|MSYS*)
            OS_TYPE="windows"
            detect_windows_variant
            ;;
        *)
            log_error "Unsupported operating system: $uname_s"
            exit 1
            ;;
    esac

    detect_shell
    detect_terminal

    log_success "Operating System: ${C_BOLD}$OS_TYPE${C_RESET}"
    [[ -n "$OS_VARIANT" ]] && log_info "  Variant: $OS_VARIANT"
    [[ -n "$OS_VERSION" ]] && log_info "  Version: $OS_VERSION"
    [[ -n "$PACKAGE_MANAGER" ]] && log_info "  Package Manager: $PACKAGE_MANAGER"
    [[ -n "$MSYS_ENVIRONMENT" ]] && log_info "  MSYS Environment: $MSYS_ENVIRONMENT"
    log_info "  Shell: $SHELL_TYPE"
    log_info "  Config File: $SHELL_CONFIG_FILE"
    [[ -n "$TERMINAL_TYPE" ]] && log_info "  Terminal: $TERMINAL_TYPE"
}

detect_linux_variant() {
    # Try to read os-release file
    if [[ -f /etc/os-release ]]; then
        # shellcheck source=/dev/null
        source /etc/os-release
        OS_VARIANT="${ID:-unknown}"
        OS_VERSION="${VERSION_ID:-}"
    elif [[ -f /etc/lsb-release ]]; then
        # shellcheck source=/dev/null
        source /etc/lsb-release
        OS_VARIANT="${DISTRIB_ID:-unknown}"
        OS_VERSION="${DISTRIB_RELEASE:-}"
    else
        OS_VARIANT="unknown"
    fi

    # Detect package manager
    if command_exists apt-get; then
        PACKAGE_MANAGER="apt"
    elif command_exists dnf; then
        PACKAGE_MANAGER="dnf"
    elif command_exists yum; then
        PACKAGE_MANAGER="yum"
    elif command_exists pacman; then
        PACKAGE_MANAGER="pacman"
    elif command_exists zypper; then
        PACKAGE_MANAGER="zypper"
    elif command_exists apk; then
        PACKAGE_MANAGER="apk"
    elif command_exists emerge; then
        PACKAGE_MANAGER="portage"
    else
        PACKAGE_MANAGER="unknown"
    fi
}

detect_macos_variant() {
    OS_VARIANT="macos"
    OS_VERSION="$(sw_vers -productVersion 2>/dev/null || echo "unknown")"

    # Check for Homebrew
    if command_exists brew; then
        PACKAGE_MANAGER="brew"
    elif [[ -x /opt/homebrew/bin/brew ]]; then
        PACKAGE_MANAGER="brew"
        # Add Homebrew to PATH for this session
        eval "$(/opt/homebrew/bin/brew shellenv)"
    elif [[ -x /usr/local/bin/brew ]]; then
        PACKAGE_MANAGER="brew"
    else
        PACKAGE_MANAGER="none"
        log_warn "Homebrew not found. Some features may be limited."
    fi
}

detect_windows_variant() {
    OS_VARIANT="windows"

    # Detect MSYS2 environment
    if [[ -n "${MSYSTEM:-}" ]]; then
        MSYS_ENVIRONMENT="$MSYSTEM"
        case "$MSYSTEM" in
            UCRT64)
                OS_VARIANT="msys2-ucrt64"
                PACKAGE_MANAGER="pacman"
                ;;
            CLANG64)
                OS_VARIANT="msys2-clang64"
                PACKAGE_MANAGER="pacman"
                ;;
            CLANG32)
                OS_VARIANT="msys2-clang32"
                PACKAGE_MANAGER="pacman"
                ;;
            MINGW64)
                OS_VARIANT="msys2-mingw64"
                PACKAGE_MANAGER="pacman"
                ;;
            MINGW32)
                OS_VARIANT="msys2-mingw32"
                PACKAGE_MANAGER="pacman"
                ;;
            MSYS)
                OS_VARIANT="msys2-msys"
                PACKAGE_MANAGER="pacman"
                ;;
            *)
                OS_VARIANT="msys2-$MSYSTEM"
                PACKAGE_MANAGER="pacman"
                ;;
        esac
    elif [[ "$(uname -s)" == CYGWIN* ]]; then
        OS_VARIANT="cygwin"
        PACKAGE_MANAGER="apt-cyg"
    elif [[ "$(uname -s)" == MINGW* ]]; then
        OS_VARIANT="mingw"
        PACKAGE_MANAGER="unknown"
    else
        OS_VARIANT="windows-unknown"
        PACKAGE_MANAGER="unknown"
    fi

    # Try to get Windows version
    if command_exists cmd.exe; then
        OS_VERSION="$(cmd.exe /c ver 2>/dev/null | tr -d '\r' | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1 || echo "unknown")"
    fi
}

detect_shell() {
    # Determine current shell
    local current_shell="${SHELL:-/bin/sh}"
    current_shell="$(basename "$current_shell")"

    case "$current_shell" in
        bash)
            SHELL_TYPE="bash"
            if [[ "$OS_TYPE" == "macos" ]]; then
                # macOS: prefer .bash_profile for login shells
                if [[ -f "$HOME/.bash_profile" ]]; then
                    SHELL_CONFIG_FILE="$HOME/.bash_profile"
                else
                    SHELL_CONFIG_FILE="$HOME/.bashrc"
                fi
            else
                SHELL_CONFIG_FILE="$HOME/.bashrc"
            fi
            ;;
        zsh)
            SHELL_TYPE="zsh"
            SHELL_CONFIG_FILE="$HOME/.zshrc"
            ;;
        fish)
            SHELL_TYPE="fish"
            SHELL_CONFIG_FILE="$HOME/.config/fish/config.fish"
            ;;
        ksh)
            SHELL_TYPE="ksh"
            SHELL_CONFIG_FILE="$HOME/.kshrc"
            ;;
        *)
            SHELL_TYPE="$current_shell"
            SHELL_CONFIG_FILE="$HOME/.profile"
            ;;
    esac

    # For MSYS2, also consider Windows-specific config
    if [[ "$OS_TYPE" == "windows" ]] && [[ -n "$MSYS_ENVIRONMENT" ]]; then
        # MSYS2 uses .bashrc typically
        if [[ ! -f "$SHELL_CONFIG_FILE" ]] && [[ -f "$HOME/.bashrc" ]]; then
            SHELL_CONFIG_FILE="$HOME/.bashrc"
        fi
    fi
}

detect_terminal() {
    TERMINAL_TYPE=""

    # Check for specific terminal emulators
    if [[ -n "${WT_SESSION:-}" ]]; then
        TERMINAL_TYPE="windows-terminal"
    elif [[ -n "${TERM_PROGRAM:-}" ]]; then
        case "$TERM_PROGRAM" in
            iTerm.app) TERMINAL_TYPE="iterm2" ;;
            Apple_Terminal) TERMINAL_TYPE="apple-terminal" ;;
            vscode) TERMINAL_TYPE="vscode" ;;
            *) TERMINAL_TYPE="$TERM_PROGRAM" ;;
        esac
    elif [[ -n "${COLORTERM:-}" ]]; then
        TERMINAL_TYPE="$COLORTERM"
    elif [[ -n "${TERM:-}" ]]; then
        TERMINAL_TYPE="$TERM"
    fi
}

# ==============================================================================
# TOOLCHAIN DETECTION
# ==============================================================================

detect_toolchain() {
    print_header "Detecting Toolchain"

    # Check for existing DH_HOME
    if [[ -n "${DH_HOME:-}" ]]; then
        EXISTING_DH_HOME="$DH_HOME"
        log_info "Existing DH_HOME: $EXISTING_DH_HOME"
    fi

    # Check for existing dh-c
    if command_exists dh-c; then
        EXISTING_DHC_PATH="$(command -v dh-c)"
        log_info "Existing dh-c: $EXISTING_DHC_PATH"
    elif [[ -x "$SCRIPT_DIR/bin/dh-c" ]]; then
        EXISTING_DHC_PATH="$SCRIPT_DIR/bin/dh-c"
        log_info "Existing dh-c (local): $EXISTING_DHC_PATH"
    fi

    # Detect compilers
    detect_clang
    detect_gcc

    # Choose best available compiler
    choose_compiler
}

detect_clang() {
    local clang_cmd=""
    local clang_path=""
    local clang_version=""

    # Priority order for Clang detection
    local -a clang_candidates=()

    # 1. If CC is set to clang, use it
    if [[ -n "${CC:-}" ]] && [[ "$(basename "$CC")" == clang* ]]; then
        clang_candidates+=("$CC")
    fi

    # 2. Check for user-specified compiler
    if [[ -n "$OPT_CC" ]] && [[ "$(basename "$OPT_CC")" == clang* ]]; then
        clang_candidates+=("$OPT_CC")
    fi

    # 3. Check versioned clang binaries (prefer newer)
    local v
    for v in 19 18 17 16 15 14 13 12 11 10 9; do
        if command_exists "clang-$v"; then
            clang_candidates+=("clang-$v")
        fi
    done

    # 4. Check plain clang
    if command_exists clang; then
        clang_candidates+=("clang")
    fi

    # 5. macOS: Check Homebrew LLVM
    if [[ "$OS_TYPE" == "macos" ]]; then
        if [[ -x "/opt/homebrew/opt/llvm/bin/clang" ]]; then
            clang_candidates+=("/opt/homebrew/opt/llvm/bin/clang")
        fi
        if [[ -x "/usr/local/opt/llvm/bin/clang" ]]; then
            clang_candidates+=("/usr/local/opt/llvm/bin/clang")
        fi
    fi

    # 6. MSYS2: Check MSYS2-specific paths
    if [[ "$OS_TYPE" == "windows" ]] && [[ -n "$MSYS_ENVIRONMENT" ]]; then
        case "$MSYS_ENVIRONMENT" in
            CLANG64)
                [[ -x "/clang64/bin/clang" ]] && clang_candidates+=("/clang64/bin/clang")
                ;;
            UCRT64)
                [[ -x "/ucrt64/bin/clang" ]] && clang_candidates+=("/ucrt64/bin/clang")
                ;;
            MINGW64)
                [[ -x "/mingw64/bin/clang" ]] && clang_candidates+=("/mingw64/bin/clang")
                ;;
        esac
    fi

    # Try each candidate
    for candidate in "${clang_candidates[@]}"; do
        if [[ -x "$candidate" ]] || command_exists "$candidate"; then
            local version_output
            version_output="$("$candidate" --version 2>/dev/null | head -1)" || continue

            # Extract version number (handles various formats)
            # "clang version 16.0.0" or "Apple clang version 15.0.0"
            local ver
            # Use portable grep -oE (extended regex) - works on both GNU and BSD grep
            ver="$(echo "$version_output" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)" || true

            if [[ -n "$ver" ]]; then
                clang_cmd="$candidate"
                clang_version="$ver"

                # Get full path
                if [[ "$candidate" == /* ]]; then
                    clang_path="$candidate"
                else
                    clang_path="$(command -v "$candidate" 2>/dev/null || echo "$candidate")"
                fi

                break
            fi
        fi
    done

    if [[ -n "$clang_version" ]]; then
        log_success "Found Clang: ${C_BOLD}$clang_version${C_RESET} ($clang_path)"

        # Determine tier
        local tier
        tier=$(get_version_tier "$clang_version" "$CLANG_RECOMMENDED" "$CLANG_SUPPORTED" "$CLANG_MINIMUM")

        case "$tier" in
            recommended)
                log_success "  Status: ${C_GREEN}RECOMMENDED${C_RESET}"
                ;;
            supported)
                log_info "  Status: ${C_YELLOW}SUPPORTED${C_RESET} (Recommended: $CLANG_RECOMMENDED+)"
                ;;
            tbu)
                log_warn "  Status: ${C_YELLOW}TBU${C_RESET} - Below supported version"
                ;;
            unsupported)
                log_error "  Status: ${C_RED}UNSUPPORTED${C_RESET} - Below minimum version"
                ;;
        esac

        # Store for later use
        DETECTED_CLANG_PATH="$clang_path"
        DETECTED_CLANG_VERSION="$clang_version"
        DETECTED_CLANG_TIER="$tier"
    else
        log_info "Clang: Not found"
        DETECTED_CLANG_PATH=""
        DETECTED_CLANG_VERSION=""
        DETECTED_CLANG_TIER="not_found"
    fi
}

detect_gcc() {
    local gcc_cmd=""
    local gcc_path=""
    local gcc_version=""

    # Priority order for GCC detection
    local -a gcc_candidates=()

    # 1. If CC is set to gcc, use it
    if [[ -n "${CC:-}" ]] && [[ "$(basename "$CC")" == gcc* ]]; then
        gcc_candidates+=("$CC")
    fi

    # 2. Check for user-specified compiler
    if [[ -n "$OPT_CC" ]] && [[ "$(basename "$OPT_CC")" == gcc* ]]; then
        gcc_candidates+=("$OPT_CC")
    fi

    # 3. Check versioned gcc binaries (prefer newer)
    local v
    for v in 15 14 13 12 11 10 9; do
        if command_exists "gcc-$v"; then
            gcc_candidates+=("gcc-$v")
        fi
    done

    # 4. Check plain gcc
    if command_exists gcc; then
        gcc_candidates+=("gcc")
    fi

    # 5. MSYS2: Check MSYS2-specific paths
    if [[ "$OS_TYPE" == "windows" ]] && [[ -n "$MSYS_ENVIRONMENT" ]]; then
        case "$MSYS_ENVIRONMENT" in
            UCRT64)
                [[ -x "/ucrt64/bin/gcc" ]] && gcc_candidates+=("/ucrt64/bin/gcc")
                ;;
            MINGW64)
                [[ -x "/mingw64/bin/gcc" ]] && gcc_candidates+=("/mingw64/bin/gcc")
                ;;
            MINGW32)
                [[ -x "/mingw32/bin/gcc" ]] && gcc_candidates+=("/mingw32/bin/gcc")
                ;;
        esac
    fi

    # Try each candidate
    for candidate in "${gcc_candidates[@]}"; do
        if [[ -x "$candidate" ]] || command_exists "$candidate"; then
            local version_output
            version_output="$("$candidate" --version 2>/dev/null | head -1)" || continue

            # Extract version number
            # "gcc (GCC) 13.2.0" or "gcc (Ubuntu 13.2.0-4ubuntu3) 13.2.0"
            local ver
            # Use portable grep -oE (extended regex) - get last version number (the actual GCC version)
            ver="$(echo "$version_output" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | tail -1)" || true

            if [[ -n "$ver" ]]; then
                gcc_cmd="$candidate"
                gcc_version="$ver"

                # Get full path
                if [[ "$candidate" == /* ]]; then
                    gcc_path="$candidate"
                else
                    gcc_path="$(command -v "$candidate" 2>/dev/null || echo "$candidate")"
                fi

                break
            fi
        fi
    done

    if [[ -n "$gcc_version" ]]; then
        log_success "Found GCC: ${C_BOLD}$gcc_version${C_RESET} ($gcc_path)"

        # Determine tier
        local tier
        tier=$(get_version_tier "$gcc_version" "$GCC_RECOMMENDED" "$GCC_SUPPORTED" "$GCC_MINIMUM")

        case "$tier" in
            recommended)
                log_success "  Status: ${C_GREEN}RECOMMENDED${C_RESET}"
                ;;
            supported)
                log_info "  Status: ${C_YELLOW}SUPPORTED${C_RESET} (Recommended: $GCC_RECOMMENDED+)"
                ;;
            tbu)
                log_warn "  Status: ${C_YELLOW}TBU${C_RESET} - Below supported version"
                ;;
            unsupported)
                log_error "  Status: ${C_RED}UNSUPPORTED${C_RESET} - Below minimum version"
                ;;
        esac

        # Store for later use
        DETECTED_GCC_PATH="$gcc_path"
        DETECTED_GCC_VERSION="$gcc_version"
        DETECTED_GCC_TIER="$tier"
    else
        log_info "GCC: Not found"
        DETECTED_GCC_PATH=""
        DETECTED_GCC_VERSION=""
        DETECTED_GCC_TIER="not_found"
    fi
}

# Get version tier: recommended|supported|tbu|unsupported
get_version_tier() {
    local version="$1"
    local recommended="$2"
    local supported="$3"
    local minimum="$4"

    if [[ -z "$version" ]]; then
        echo "not_found"
        return
    fi

    if version_gte "$version" "$recommended"; then
        echo "recommended"
    elif version_gte "$version" "$supported"; then
        echo "supported"
    elif [[ -n "$minimum" ]] && version_gte "$version" "$minimum"; then
        echo "tbu"
    else
        echo "unsupported"
    fi
}

# Choose the best available compiler
choose_compiler() {
    # Priority: user-specified > clang (recommended) > gcc (recommended) > clang (supported) > gcc (supported)

    if [[ -n "$OPT_CC" ]]; then
        # User specified a compiler
        if [[ -x "$OPT_CC" ]] || command_exists "$OPT_CC"; then
            DETECTED_CC="$OPT_CC"
            if [[ "$OPT_CC" == /* ]]; then
                DETECTED_CC_PATH="$OPT_CC"
            else
                DETECTED_CC_PATH="$(command -v "$OPT_CC" 2>/dev/null || echo "$OPT_CC")"
            fi
            # Get version
            local ver_out
            ver_out="$("$OPT_CC" --version 2>/dev/null | head -1)"
            DETECTED_CC_VERSION="$(echo "$ver_out" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)"

            if [[ "$(basename "$OPT_CC")" == clang* ]]; then
                DETECTED_CC_TIER="$DETECTED_CLANG_TIER"
            else
                DETECTED_CC_TIER="$DETECTED_GCC_TIER"
            fi

            log_info "Using specified compiler: $DETECTED_CC_PATH"
            return
        else
            log_error "Specified compiler not found: $OPT_CC"
            exit 1
        fi
    fi

    # Auto-select best compiler
    # Prefer Clang if at recommended level
    if [[ "${DETECTED_CLANG_TIER:-}" == "recommended" ]]; then
        DETECTED_CC="clang"
        DETECTED_CC_PATH="${DETECTED_CLANG_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_CLANG_VERSION:-}"
        DETECTED_CC_TIER="recommended"
        log_info "Selected compiler: Clang $DETECTED_CC_VERSION (recommended)"
        return
    fi

    # Then GCC if at recommended level
    if [[ "${DETECTED_GCC_TIER:-}" == "recommended" ]]; then
        DETECTED_CC="gcc"
        DETECTED_CC_PATH="${DETECTED_GCC_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_GCC_VERSION:-}"
        DETECTED_CC_TIER="recommended"
        log_info "Selected compiler: GCC $DETECTED_CC_VERSION (recommended)"
        return
    fi

    # Then Clang if at supported level
    if [[ "${DETECTED_CLANG_TIER:-}" == "supported" ]]; then
        DETECTED_CC="clang"
        DETECTED_CC_PATH="${DETECTED_CLANG_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_CLANG_VERSION:-}"
        DETECTED_CC_TIER="supported"
        log_info "Selected compiler: Clang $DETECTED_CC_VERSION (supported)"
        return
    fi

    # Then GCC if at supported level
    if [[ "${DETECTED_GCC_TIER:-}" == "supported" ]]; then
        DETECTED_CC="gcc"
        DETECTED_CC_PATH="${DETECTED_GCC_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_GCC_VERSION:-}"
        DETECTED_CC_TIER="supported"
        log_info "Selected compiler: GCC $DETECTED_CC_VERSION (supported)"
        return
    fi

    # TBU level (warn but allow)
    if [[ "${DETECTED_CLANG_TIER:-}" == "tbu" ]]; then
        DETECTED_CC="clang"
        DETECTED_CC_PATH="${DETECTED_CLANG_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_CLANG_VERSION:-}"
        DETECTED_CC_TIER="tbu"
        log_warn "Selected compiler: Clang $DETECTED_CC_VERSION (TBU - below recommended)"
        return
    fi

    if [[ "${DETECTED_GCC_TIER:-}" == "tbu" ]]; then
        DETECTED_CC="gcc"
        DETECTED_CC_PATH="${DETECTED_GCC_PATH:-}"
        DETECTED_CC_VERSION="${DETECTED_GCC_VERSION:-}"
        DETECTED_CC_TIER="tbu"
        log_warn "Selected compiler: GCC $DETECTED_CC_VERSION (TBU - below recommended)"
        return
    fi

    # No suitable compiler found
    DETECTED_CC=""
    DETECTED_CC_PATH=""
    DETECTED_CC_VERSION=""
    DETECTED_CC_TIER="not_found"
    log_warn "No suitable compiler found"
}

# ==============================================================================
# INTERACTIVE SELECTION
# ==============================================================================

interactive_selection() {
    print_header "Installation Options"

    # Toolchain selection
    select_toolchain_action

    # dh-c selection
    select_dhc_action

    # DH_HOME selection
    select_env_action

    # Show summary and confirm
    show_selection_summary
}

select_toolchain_action() {
    if [[ "$OPT_SKIP_TOOLCHAIN" == true ]]; then
        TOOLCHAIN_ACTION="skip"
        log_info "Toolchain setup: Skipped (--skip-toolchain)"
        return
    fi

    echo ""
    echo -e "${C_BOLD}Toolchain Configuration${C_RESET}"
    echo ""

    # Build options based on current state
    local -a options=()
    local -a actions=()

    if [[ -n "$DETECTED_CC_PATH" ]]; then
        local status_label
        case "$DETECTED_CC_TIER" in
            recommended) status_label="${C_GREEN}RECOMMENDED${C_RESET}" ;;
            supported) status_label="${C_YELLOW}SUPPORTED${C_RESET}" ;;
            tbu) status_label="${C_YELLOW}TBU${C_RESET}" ;;
            *) status_label="${C_RED}UNKNOWN${C_RESET}" ;;
        esac

        options+=("Keep current ($DETECTED_CC $DETECTED_CC_VERSION) [$status_label]")
        actions+=("keep")
    fi

    # Install/update options
    if [[ "$DETECTED_CC_TIER" != "recommended" ]]; then
        options+=("Install Clang $CLANG_RECOMMENDED+ locally (in $SCRIPT_DIR/toolchain/)")
        actions+=("install-local")

        options+=("Install Clang $CLANG_RECOMMENDED+ globally (via package manager)")
        actions+=("install-global")
    fi

    if [[ -n "$DETECTED_CC_PATH" ]] && [[ "$DETECTED_CC_TIER" != "recommended" ]]; then
        options+=("Update existing compiler")
        actions+=("update")
    fi

    options+=("Skip toolchain setup (I'll configure it myself)")
    actions+=("skip")

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        # Non-interactive: keep current if available and at least supported, otherwise try global install
        if [[ -n "$DETECTED_CC_PATH" ]] && [[ "$DETECTED_CC_TIER" == "recommended" || "$DETECTED_CC_TIER" == "supported" ]]; then
            TOOLCHAIN_ACTION="keep"
        elif [[ -n "$DETECTED_CC_PATH" ]]; then
            TOOLCHAIN_ACTION="keep"  # Keep what we have in non-interactive
        else
            TOOLCHAIN_ACTION="install-global"
        fi
        log_info "Toolchain action: $TOOLCHAIN_ACTION (non-interactive)"
        return
    fi

    prompt_select "Select toolchain option:" "${options[@]}"
    TOOLCHAIN_ACTION="${actions[$((REPLY - 1))]}"

    # If unsupported, require action
    if [[ "$DETECTED_CC_TIER" == "unsupported" || "$DETECTED_CC_TIER" == "not_found" ]] && [[ "$TOOLCHAIN_ACTION" == "keep" ]]; then
        log_error "Current compiler is unsupported. Please select an installation option."
        echo ""
        echo "Minimum requirements:"
        echo "  Clang: $CLANG_SUPPORTED+ (Recommended: $CLANG_RECOMMENDED+)"
        echo "  GCC: $GCC_SUPPORTED+ (Recommended: $GCC_RECOMMENDED+)"
        echo ""
        select_toolchain_action  # Retry
    fi
}

select_dhc_action() {
    if [[ "$OPT_SKIP_DHC" == true ]]; then
        DHC_ACTION="skip"
        log_info "dh-c installation: Skipped (--skip-dhc)"
        return
    fi

    echo ""
    echo -e "${C_BOLD}dh-c Build Tool${C_RESET}"
    echo ""

    local -a options=()
    local -a actions=()

    if [[ -n "$EXISTING_DHC_PATH" ]]; then
        options+=("Update existing dh-c ($EXISTING_DHC_PATH)")
        actions+=("update")
    fi

    options+=("Install locally (in $SCRIPT_DIR/bin/)")
    actions+=("install-local")

    options+=("Install globally (/usr/local/bin/ or equivalent)")
    actions+=("install-global")

    options+=("Skip dh-c installation (I'll compile it myself)")
    actions+=("skip")

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        if [[ -n "$EXISTING_DHC_PATH" ]]; then
            DHC_ACTION="update"
        else
            DHC_ACTION="install-local"
        fi
        log_info "dh-c action: $DHC_ACTION (non-interactive)"
        return
    fi

    prompt_select "Select dh-c installation option:" "${options[@]}"
    DHC_ACTION="${actions[$((REPLY - 1))]}"

    # Get custom prefix if needed
    if [[ "$DHC_ACTION" == "install-global" ]]; then
        if [[ -n "$OPT_PREFIX" ]]; then
            TARGET_DHC_PREFIX="$OPT_PREFIX"
        else
            TARGET_DHC_PREFIX="/usr/local"
        fi
    elif [[ "$DHC_ACTION" == "install-local" ]]; then
        TARGET_DHC_PREFIX="$SCRIPT_DIR"
    elif [[ "$DHC_ACTION" == "update" ]] && [[ -n "$EXISTING_DHC_PATH" ]]; then
        TARGET_DHC_PREFIX="$(dirname "$(dirname "$EXISTING_DHC_PATH")")"
    fi
}

select_env_action() {
    if [[ "$OPT_SKIP_ENV" == true ]]; then
        ENV_ACTION="skip"
        log_info "DH_HOME registration: Skipped (--skip-env)"
        return
    fi

    echo ""
    echo -e "${C_BOLD}DH_HOME Environment Variable${C_RESET}"
    echo ""

    local -a options=()
    local -a actions=()

    if [[ -n "$EXISTING_DH_HOME" ]]; then
        options+=("Update existing DH_HOME ($EXISTING_DH_HOME -> $SCRIPT_DIR)")
        actions+=("update")
    fi

    options+=("Register locally (create .dh-env file in $SCRIPT_DIR)")
    actions+=("register-local")

    options+=("Register globally (add to $SHELL_CONFIG_FILE)")
    actions+=("register-global")

    options+=("Skip registration (I'll set DH_HOME myself)")
    actions+=("skip")

    if [[ "$OPT_NON_INTERACTIVE" == true ]]; then
        if [[ -n "$EXISTING_DH_HOME" ]]; then
            ENV_ACTION="update"
        elif [[ "$OPT_GLOBAL" == true ]]; then
            ENV_ACTION="register-global"
        else
            ENV_ACTION="register-local"
        fi
        log_info "DH_HOME action: $ENV_ACTION (non-interactive)"
        return
    fi

    prompt_select "Select DH_HOME registration option:" "${options[@]}"
    ENV_ACTION="${actions[$((REPLY - 1))]}"

    # Determine target DH_HOME path
    if [[ -n "$OPT_DH_HOME" ]]; then
        TARGET_DH_HOME="$OPT_DH_HOME"
    else
        TARGET_DH_HOME="$SCRIPT_DIR/dh"
    fi
}

show_selection_summary() {
    print_header "Installation Summary"

    echo ""
    echo "The following actions will be performed:"
    echo ""

    # Toolchain
    echo -n "  Toolchain: "
    case "$TOOLCHAIN_ACTION" in
        keep)
            echo -e "${C_GREEN}Keep current${C_RESET} ($DETECTED_CC $DETECTED_CC_VERSION)"
            TARGET_CC="$DETECTED_CC_PATH"
            ;;
        install-local)
            echo -e "${C_YELLOW}Install locally${C_RESET} (Clang $CLANG_RECOMMENDED+ to $SCRIPT_DIR/toolchain/)"
            ;;
        install-global)
            echo -e "${C_YELLOW}Install globally${C_RESET} (Clang $CLANG_RECOMMENDED+ via $PACKAGE_MANAGER)"
            ;;
        update)
            echo -e "${C_YELLOW}Update${C_RESET} existing compiler"
            ;;
        skip)
            echo -e "${C_DIM}Skip${C_RESET}"
            TARGET_CC="$DETECTED_CC_PATH"
            ;;
    esac

    # dh-c
    echo -n "  dh-c: "
    case "$DHC_ACTION" in
        install-local)
            echo -e "${C_YELLOW}Install locally${C_RESET} ($SCRIPT_DIR/bin/dh-c)"
            ;;
        install-global)
            echo -e "${C_YELLOW}Install globally${C_RESET} ($TARGET_DHC_PREFIX/bin/dh-c)"
            ;;
        update)
            echo -e "${C_YELLOW}Update${C_RESET} ($EXISTING_DHC_PATH)"
            ;;
        skip)
            echo -e "${C_DIM}Skip${C_RESET}"
            ;;
    esac

    # DH_HOME
    echo -n "  DH_HOME: "
    case "$ENV_ACTION" in
        register-local)
            echo -e "${C_YELLOW}Register locally${C_RESET} (create $SCRIPT_DIR/.dh-env)"
            ;;
        register-global)
            echo -e "${C_YELLOW}Register globally${C_RESET} (add to $SHELL_CONFIG_FILE)"
            ;;
        update)
            echo -e "${C_YELLOW}Update${C_RESET} (change $EXISTING_DH_HOME -> $TARGET_DH_HOME/dh)"
            ;;
        skip)
            echo -e "${C_DIM}Skip${C_RESET}"
            ;;
    esac

    echo ""

    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_info "Dry-run mode: No changes will be made"
        return 0
    fi

    if ! prompt_yes_no "Proceed with installation?" "y"; then
        log_info "Installation cancelled by user"
        exit 0
    fi
}

# ==============================================================================
# TOOLCHAIN INSTALLATION
# ==============================================================================

install_toolchain() {
    if [[ "$TOOLCHAIN_ACTION" == "keep" ]] || [[ "$TOOLCHAIN_ACTION" == "skip" ]]; then
        return 0
    fi

    print_header "Installing Toolchain"

    case "$TOOLCHAIN_ACTION" in
        install-local)
            install_toolchain_local
            ;;
        install-global)
            install_toolchain_global
            ;;
        update)
            update_toolchain
            ;;
    esac
}

install_toolchain_local() {
    log_info "Installing Clang locally to $SCRIPT_DIR/toolchain/"

    local toolchain_dir="$SCRIPT_DIR/toolchain"
    run_cmd mkdir -p "$toolchain_dir"

    case "$OS_TYPE" in
        linux)
            install_clang_local_linux "$toolchain_dir"
            ;;
        macos)
            install_clang_local_macos "$toolchain_dir"
            ;;
        windows)
            install_clang_local_windows "$toolchain_dir"
            ;;
    esac
}

install_clang_local_linux() {
    local toolchain_dir="$1"

    log_info "Downloading pre-built Clang for Linux..."

    # Determine architecture
    local arch
    arch="$(uname -m)"
    case "$arch" in
        x86_64) arch="x86_64" ;;
        aarch64) arch="aarch64" ;;
        *)
            log_error "Unsupported architecture for local install: $arch"
            log_info "Please use global installation instead."
            return 1
            ;;
    esac

    # LLVM releases URL pattern
    local llvm_version="19.1.0"
    local llvm_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-${llvm_version}/clang+llvm-${llvm_version}-${arch}-linux-gnu-ubuntu-22.04.tar.xz"

    log_info "Download URL: $llvm_url"
    log_warn "Local toolchain installation requires downloading ~500MB."

    if ! prompt_yes_no "Continue with download?" "y"; then
        TOOLCHAIN_ACTION="skip"
        return 0
    fi

    local archive_file="$toolchain_dir/llvm.tar.xz"

    # Download
    if command_exists curl; then
        run_cmd curl -L -o "$archive_file" "$llvm_url"
    elif command_exists wget; then
        run_cmd wget -O "$archive_file" "$llvm_url"
    else
        log_error "Neither curl nor wget found. Cannot download toolchain."
        return 1
    fi

    # Extract
    log_info "Extracting..."
    run_cmd tar -xf "$archive_file" -C "$toolchain_dir" --strip-components=1

    # Cleanup
    run_cmd rm -f "$archive_file"

    # Update detected compiler
    TARGET_CC="$toolchain_dir/bin/clang"
    DETECTED_CC_PATH="$TARGET_CC"
    DETECTED_CC_VERSION="$llvm_version"

    log_success "Clang $llvm_version installed to $toolchain_dir"
}

install_clang_local_macos() {
    local toolchain_dir="$1"

    log_warn "Local toolchain installation on macOS is complex."
    log_info "Recommendation: Use Homebrew for global installation instead."

    if prompt_yes_no "Switch to global installation via Homebrew?" "y"; then
        TOOLCHAIN_ACTION="install-global"
        install_toolchain_global
        return
    fi

    # If user insists on local, try to download
    log_info "Attempting local installation..."

    local arch
    arch="$(uname -m)"
    local llvm_version="19.1.0"
    local llvm_url

    case "$arch" in
        arm64)
            llvm_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-${llvm_version}/clang+llvm-${llvm_version}-arm64-apple-darwin24.1.tar.xz"
            ;;
        x86_64)
            llvm_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-${llvm_version}/clang+llvm-${llvm_version}-x86_64-apple-darwin23.tar.xz"
            ;;
        *)
            log_error "Unsupported architecture: $arch"
            return 1
            ;;
    esac

    local archive_file="$toolchain_dir/llvm.tar.xz"

    if command_exists curl; then
        run_cmd curl -L -o "$archive_file" "$llvm_url"
    else
        log_error "curl not found. Cannot download toolchain."
        return 1
    fi

    log_info "Extracting..."
    run_cmd tar -xf "$archive_file" -C "$toolchain_dir" --strip-components=1
    run_cmd rm -f "$archive_file"

    TARGET_CC="$toolchain_dir/bin/clang"
    DETECTED_CC_PATH="$TARGET_CC"
    DETECTED_CC_VERSION="$llvm_version"

    log_success "Clang $llvm_version installed to $toolchain_dir"
}

install_clang_local_windows() {
    local toolchain_dir="$1"

    if [[ -n "$MSYS_ENVIRONMENT" ]]; then
        log_warn "On MSYS2, it's recommended to use pacman for global installation."

        if prompt_yes_no "Switch to global installation via pacman?" "y"; then
            TOOLCHAIN_ACTION="install-global"
            install_toolchain_global
            return
        fi
    fi

    log_error "Local toolchain installation on Windows is not yet supported."
    log_info "Please use global installation or install manually."
    TOOLCHAIN_ACTION="skip"
}

install_toolchain_global() {
    log_info "Installing Clang globally via package manager..."

    case "$OS_TYPE" in
        linux)
            install_clang_global_linux
            ;;
        macos)
            install_clang_global_macos
            ;;
        windows)
            install_clang_global_windows
            ;;
    esac
}

install_clang_global_linux() {
    case "$PACKAGE_MANAGER" in
        apt)
            log_info "Adding LLVM repository for Ubuntu/Debian..."

            # Check if we need sudo
            if [[ $EUID -ne 0 ]]; then
                log_info "Root privileges required for global installation."
            fi

            # Add LLVM repository
            if ! command_exists wget; then
                run_sudo apt-get update
                run_sudo apt-get install -y wget
            fi

            # Install LLVM's automatic installation script
            log_info "Running LLVM installation script..."
            if [[ "$OPT_DRY_RUN" == true ]]; then
                log_dry_run "wget -O - https://apt.llvm.org/llvm.sh | sudo bash -s 19"
            else
                wget -O - https://apt.llvm.org/llvm.sh | sudo bash -s 19
            fi

            TARGET_CC="clang-19"
            ;;
        dnf|yum)
            run_sudo "$PACKAGE_MANAGER" install -y clang
            TARGET_CC="clang"
            ;;
        pacman)
            run_sudo pacman -S --noconfirm clang
            TARGET_CC="clang"
            ;;
        zypper)
            run_sudo zypper install -y clang
            TARGET_CC="clang"
            ;;
        apk)
            run_sudo apk add clang
            TARGET_CC="clang"
            ;;
        *)
            log_error "Unsupported package manager: $PACKAGE_MANAGER"
            log_info "Please install Clang manually and re-run this script."
            return 1
            ;;
    esac

    # Verify installation
    if command_exists "$TARGET_CC"; then
        DETECTED_CC_PATH="$(command -v "$TARGET_CC")"
        local ver_out
        ver_out="$("$TARGET_CC" --version | head -1)"
        DETECTED_CC_VERSION="$(echo "$ver_out" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)"
        log_success "Clang $DETECTED_CC_VERSION installed successfully"
    else
        log_error "Clang installation may have failed. Please check manually."
        return 1
    fi
}

install_clang_global_macos() {
    if [[ "$PACKAGE_MANAGER" != "brew" ]]; then
        log_error "Homebrew is required for global Clang installation on macOS."
        log_info "Install Homebrew from: https://brew.sh"
        return 1
    fi

    log_info "Installing LLVM via Homebrew..."
    run_cmd brew install llvm

    # Homebrew LLVM is keg-only, need to get the path
    local llvm_prefix
    llvm_prefix="$(brew --prefix llvm)"

    TARGET_CC="$llvm_prefix/bin/clang"
    DETECTED_CC_PATH="$TARGET_CC"

    if [[ -x "$TARGET_CC" ]]; then
        local ver_out
        ver_out="$("$TARGET_CC" --version | head -1)"
        DETECTED_CC_VERSION="$(echo "$ver_out" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)"
        log_success "Clang $DETECTED_CC_VERSION installed successfully"

        log_info "Note: Homebrew LLVM is keg-only. To use it, either:"
        log_info "  1. Use full path: $TARGET_CC"
        log_info "  2. Add to PATH: export PATH=\"$llvm_prefix/bin:\$PATH\""
    else
        log_error "LLVM installation may have failed. Please check manually."
        return 1
    fi
}

install_clang_global_windows() {
    if [[ -z "$MSYS_ENVIRONMENT" ]]; then
        log_error "Global installation requires MSYS2 environment."
        return 1
    fi

    log_info "Installing Clang via pacman..."

    local package=""
    case "$MSYS_ENVIRONMENT" in
        CLANG64)
            package="mingw-w64-clang-x86_64-clang"
            ;;
        UCRT64)
            package="mingw-w64-ucrt-x86_64-clang"
            ;;
        MINGW64)
            package="mingw-w64-x86_64-clang"
            ;;
        MINGW32)
            package="mingw-w64-i686-clang"
            ;;
        MSYS)
            package="clang"
            ;;
        *)
            log_warn "Unknown MSYS2 environment: $MSYS_ENVIRONMENT"
            package="clang"
            ;;
    esac

    run_cmd pacman -S --noconfirm "$package"

    TARGET_CC="clang"
    if command_exists clang; then
        DETECTED_CC_PATH="$(command -v clang)"
        local ver_out
        ver_out="$(clang --version | head -1)"
        DETECTED_CC_VERSION="$(echo "$ver_out" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)"
        log_success "Clang $DETECTED_CC_VERSION installed successfully"
    fi
}

update_toolchain() {
    log_info "Updating toolchain..."

    case "$OS_TYPE" in
        linux)
            case "$PACKAGE_MANAGER" in
                apt)
                    run_sudo apt-get update
                    run_sudo apt-get upgrade -y clang clang-19 2>/dev/null || run_sudo apt-get upgrade -y clang
                    ;;
                dnf|yum)
                    run_sudo "$PACKAGE_MANAGER" update -y clang
                    ;;
                pacman)
                    run_sudo pacman -Syu --noconfirm clang
                    ;;
                *)
                    log_warn "Cannot update automatically. Please update manually."
                    ;;
            esac
            ;;
        macos)
            if [[ "$PACKAGE_MANAGER" == "brew" ]]; then
                run_cmd brew upgrade llvm
            fi
            ;;
        windows)
            if [[ -n "$MSYS_ENVIRONMENT" ]]; then
                run_cmd pacman -Syu --noconfirm
            fi
            ;;
    esac

    # Re-detect after update
    detect_clang
    TARGET_CC="$DETECTED_CLANG_PATH"
}

# ==============================================================================
# DH-C BUILD AND INSTALLATION
# ==============================================================================

install_dhc() {
    if [[ "$DHC_ACTION" == "skip" ]]; then
        return 0
    fi

    print_header "Building and Installing dh-c"

    # Determine compiler to use
    local compiler="${TARGET_CC:-$DETECTED_CC_PATH}"

    if [[ -z "$compiler" ]] || ! [[ -x "$compiler" || $(command -v "$compiler" 2>/dev/null) ]]; then
        # Fallback to any available compiler
        if command_exists clang; then
            compiler="clang"
        elif command_exists gcc; then
            compiler="gcc"
        else
            log_error "No suitable compiler found for building dh-c"
            return 1
        fi
    fi

    log_info "Using compiler: $compiler"

    # Check source file exists
    local source_file="$SCRIPT_DIR/$DHC_SOURCE"
    if [[ ! -f "$source_file" ]]; then
        log_error "dh-c source file not found: $source_file"
        return 1
    fi

    # Determine output path
    local output_dir
    local output_file

    case "$DHC_ACTION" in
        install-local)
            output_dir="$SCRIPT_DIR/bin"
            ;;
        install-global)
            output_dir="$TARGET_DHC_PREFIX/bin"
            ;;
        update)
            if [[ -n "$EXISTING_DHC_PATH" ]]; then
                output_dir="$(dirname "$EXISTING_DHC_PATH")"
            else
                output_dir="$SCRIPT_DIR/bin"
            fi
            ;;
    esac

    output_file="$output_dir/$DHC_BINARY"

    # Handle Windows executable extension
    if [[ "$OS_TYPE" == "windows" ]]; then
        output_file="${output_file}.exe"
    fi

    # Create output directory
    if [[ "$DHC_ACTION" == "install-global" ]] && [[ ! -d "$output_dir" ]]; then
        run_sudo mkdir -p "$output_dir"
    else
        run_cmd mkdir -p "$output_dir"
    fi

    # Build flags
    local -a build_flags=()
    IFS=' ' read -ra build_flags <<< "$DHC_COMPILE_FLAGS"

    # Add LTO if available and using clang
    if [[ "$(basename "$compiler")" == clang* ]]; then
        build_flags+=("-flto")
    fi

    # Add static linking where appropriate (avoid on Linux due to glibc limitations)
    case "$OS_TYPE" in
        windows)
            # Static linking on Windows/MSYS2
            if [[ -n "$MSYS_ENVIRONMENT" ]]; then
                build_flags+=("-static")
            fi
            ;;
    esac

    # Compile
    log_info "Compiling dh-c..."
    log_verbose "Command: $compiler ${build_flags[*]} -o $output_file $source_file"

    if [[ "$DHC_ACTION" == "install-global" ]]; then
        # Need to compile to temp location first, then move with sudo
        local temp_output="/tmp/dh-c-build-$$"
        [[ "$OS_TYPE" == "windows" ]] && temp_output="${temp_output}.exe"

        if [[ "$OPT_DRY_RUN" == true ]]; then
            log_dry_run "$compiler ${build_flags[*]} -o $temp_output $source_file"
            log_dry_run "sudo mv $temp_output $output_file"
        else
            "$compiler" "${build_flags[@]}" -o "$temp_output" "$source_file"
            run_sudo mv "$temp_output" "$output_file"
            run_sudo chmod +x "$output_file"
        fi
    else
        if [[ "$OPT_DRY_RUN" == true ]]; then
            log_dry_run "$compiler ${build_flags[*]} -o $output_file $source_file"
        else
            "$compiler" "${build_flags[@]}" -o "$output_file" "$source_file"
            chmod +x "$output_file"
        fi
    fi

    # Verify
    if [[ "$OPT_DRY_RUN" != true ]] && [[ -x "$output_file" ]]; then
        local version_info
        version_info="$("$output_file" --version 2>/dev/null | head -1 || echo "unknown")"
        log_success "dh-c installed: $output_file"
        log_info "  Version: $version_info"
    elif [[ "$OPT_DRY_RUN" == true ]]; then
        log_success "dh-c would be installed to: $output_file"
    else
        log_error "dh-c compilation may have failed"
        return 1
    fi
}

# ==============================================================================
# DH_HOME REGISTRATION
# ==============================================================================

register_dh_home() {
    if [[ "$ENV_ACTION" == "skip" ]]; then
        return 0
    fi

    print_header "Registering DH_HOME"

    local dh_home_value="${TARGET_DH_HOME:-$SCRIPT_DIR/dh}"

    case "$ENV_ACTION" in
        register-local)
            register_dh_home_local "$dh_home_value"
            ;;
        register-global)
            register_dh_home_global "$dh_home_value"
            ;;
        update)
            update_dh_home "$dh_home_value"
            ;;
    esac
}

register_dh_home_local() {
    local dh_home_value="$1"
    local env_file="$SCRIPT_DIR/.dh-env"

    log_info "Creating local environment file: $env_file"

    local content=""
    content+="# dasae-headers environment configuration\n"
    content+="# Source this file to set up the environment:\n"
    content+="#   source $env_file\n"
    content+="\n"
    content+="export DH_HOME=\"$dh_home_value\"\n"

    # Add dh-c to PATH if installed locally
    if [[ "$DHC_ACTION" == "install-local" ]] || [[ -x "$SCRIPT_DIR/bin/dh-c" ]]; then
        content+="export PATH=\"$SCRIPT_DIR/bin:\$PATH\"\n"
    fi

    # Add local toolchain to PATH if installed
    if [[ -d "$SCRIPT_DIR/toolchain/bin" ]]; then
        content+="export PATH=\"$SCRIPT_DIR/toolchain/bin:\$PATH\"\n"
    fi

    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_dry_run "Creating $env_file with content:"
        echo -e "$content"
    else
        echo -e "$content" > "$env_file"
        log_success "Environment file created: $env_file"
        echo ""
        log_info "To use dasae-headers, run:"
        echo -e "  ${C_CYAN}source $env_file${C_RESET}"
        echo ""
        log_info "Or add to your shell config:"
        echo -e "  ${C_CYAN}echo 'source $env_file' >> $SHELL_CONFIG_FILE${C_RESET}"
    fi
}

register_dh_home_global() {
    local dh_home_value="$1"

    log_info "Registering DH_HOME in: $SHELL_CONFIG_FILE"

    # Backup existing config
    if [[ -f "$SHELL_CONFIG_FILE" ]]; then
        backup_file "$SHELL_CONFIG_FILE"
    fi

    # Check if DH_HOME is already set in the file
    local marker="# dasae-headers"
    local export_line="export DH_HOME=\"$dh_home_value\""

    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_dry_run "Adding to $SHELL_CONFIG_FILE:"
        echo "  $export_line"
    else
        # Create config file if it doesn't exist
        if [[ ! -f "$SHELL_CONFIG_FILE" ]]; then
            touch "$SHELL_CONFIG_FILE"
        fi

        # Check if already registered
        if grep -q "^export DH_HOME=" "$SHELL_CONFIG_FILE" 2>/dev/null; then
            # Update existing line
            if [[ "$OS_TYPE" == "macos" ]]; then
                # macOS sed requires different syntax
                sed -i '' "s|^export DH_HOME=.*|$export_line|" "$SHELL_CONFIG_FILE"
            else
                sed -i "s|^export DH_HOME=.*|$export_line|" "$SHELL_CONFIG_FILE"
            fi
            log_info "Updated existing DH_HOME in $SHELL_CONFIG_FILE"
        else
            # Append new lines
            {
                echo ""
                echo "$marker"
                echo "$export_line"
            } >> "$SHELL_CONFIG_FILE"
            log_info "Added DH_HOME to $SHELL_CONFIG_FILE"
        fi

        # Also add PATH for dh-c if installed globally
        if [[ "$DHC_ACTION" == "install-global" ]]; then
            local path_line="export PATH=\"$TARGET_DHC_PREFIX/bin:\$PATH\""
            if ! grep -q "$TARGET_DHC_PREFIX/bin" "$SHELL_CONFIG_FILE" 2>/dev/null; then
                echo "$path_line" >> "$SHELL_CONFIG_FILE"
            fi
        fi

        log_success "DH_HOME registered globally"
        echo ""
        log_info "To apply changes, run:"
        echo -e "  ${C_CYAN}source $SHELL_CONFIG_FILE${C_RESET}"
        echo ""
        log_info "Or open a new terminal session."
    fi
}

update_dh_home() {
    local dh_home_value="$1"

    log_info "Updating DH_HOME from $EXISTING_DH_HOME to $dh_home_value"

    # Update in shell config file
    if [[ -f "$SHELL_CONFIG_FILE" ]]; then
        backup_file "$SHELL_CONFIG_FILE"

        if grep -q "^export DH_HOME=" "$SHELL_CONFIG_FILE" 2>/dev/null; then
            local export_line="export DH_HOME=\"$dh_home_value\""

            if [[ "$OPT_DRY_RUN" == true ]]; then
                log_dry_run "Updating DH_HOME in $SHELL_CONFIG_FILE"
            else
                if [[ "$OS_TYPE" == "macos" ]]; then
                    sed -i '' "s|^export DH_HOME=.*|$export_line|" "$SHELL_CONFIG_FILE"
                else
                    sed -i "s|^export DH_HOME=.*|$export_line|" "$SHELL_CONFIG_FILE"
                fi
                log_success "DH_HOME updated in $SHELL_CONFIG_FILE"
            fi
        else
            # Not found in config, register globally
            register_dh_home_global "$dh_home_value"
        fi
    else
        # Config file doesn't exist, register globally
        register_dh_home_global "$dh_home_value"
    fi
}

# ==============================================================================
# PRINT ENV (for eval)
# ==============================================================================

print_env_commands() {
    local dh_home="${TARGET_DH_HOME:-$SCRIPT_DIR/dh}"

    echo "export DH_HOME=\"$dh_home\""

    if [[ -d "$SCRIPT_DIR/bin" ]]; then
        echo "export PATH=\"$SCRIPT_DIR/bin:\$PATH\""
    fi

    if [[ -d "$SCRIPT_DIR/toolchain/bin" ]]; then
        echo "export PATH=\"$SCRIPT_DIR/toolchain/bin:\$PATH\""
    fi

    if [[ -n "$TARGET_CC" ]]; then
        echo "export CC=\"$TARGET_CC\""
    fi
}

# ==============================================================================
# HELP AND USAGE
# ==============================================================================

show_help() {
    cat << EOF
`dasae-headers` Installation Script v$SCRIPT_VERSION

Usage: $SCRIPT_NAME [OPTIONS]

Options:
  --help              Show this help message

  Installation Scope:
  --local             Local-only installation (no system-wide changes)
  --global            Global installation (may require sudo)

  Skip Components:
  --skip-toolchain    Skip toolchain detection/installation
  --skip-dhc          Skip `dh-c` build tool compilation
  --skip-env          Skip `DH_HOME` registration

  Direct Specification:
  --cc=PATH           Use specific compiler
  --dh-home=PATH      Set `DH_HOME` to specific path
  --prefix=PATH       Installation prefix for `dh-c` binary

  Behavior:
  --non-interactive   Use defaults without prompts
  --dry-run           Show actions without executing
  --print-env         Print environment commands (for eval)
  --verbose           Show detailed output

Examples:
  # Interactive installation
  ./install.sh

  # Local installation with defaults
  ./install.sh --local --non-interactive

  # Global installation
  ./install.sh --global

  # Use specific compiler
  ./install.sh --cc=/usr/bin/clang-19

  # Print environment for eval
  eval \$(./install.sh --print-env)

  # Dry run to see what would happen
  ./install.sh --dry-run

Compiler Requirements:
  Clang: ${CLANG_RECOMMENDED}+ (Recommended) / ${CLANG_SUPPORTED}+ (Supported) / ${CLANG_MINIMUM}+ (Minimum)
  GCC:   ${GCC_RECOMMENDED}+ (Recommended) / ${GCC_SUPPORTED}+ (Supported)

For more information, visit:
  GitHub: <https://github.com/coding-pelican/dasae-headers>

EOF
}

# ==============================================================================
# ARGUMENT PARSING
# ==============================================================================

parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --help|-h)
                OPT_HELP=true
                ;;
            --local)
                OPT_LOCAL=true
                ;;
            --global)
                OPT_GLOBAL=true
                ;;
            --skip-toolchain)
                OPT_SKIP_TOOLCHAIN=true
                ;;
            --skip-dhc)
                OPT_SKIP_DHC=true
                ;;
            --skip-env)
                OPT_SKIP_ENV=true
                ;;
            --cc=*)
                OPT_CC="${1#*=}"
                ;;
            --dh-home=*)
                OPT_DH_HOME="${1#*=}"
                # Expand ~ to $HOME
                OPT_DH_HOME="${OPT_DH_HOME/#\~/$HOME}"
                ;;
            --prefix=*)
                OPT_PREFIX="${1#*=}"
                OPT_PREFIX="${OPT_PREFIX/#\~/$HOME}"
                ;;
            --non-interactive)
                OPT_NON_INTERACTIVE=true
                ;;
            --dry-run)
                OPT_DRY_RUN=true
                ;;
            --print-env)
                OPT_PRINT_ENV=true
                ;;
            --verbose|-v)
                OPT_VERBOSE=true
                ;;
            *)
                log_error "Unknown option: $1"
                echo "Use --help for usage information."
                exit 1
                ;;
        esac
        shift
    done

    # Validate conflicting options
    if [[ "$OPT_LOCAL" == true ]] && [[ "$OPT_GLOBAL" == true ]]; then
        log_error "Cannot specify both --local and --global"
        exit 1
    fi
}

# ==============================================================================
# MAIN ENTRY POINT
# ==============================================================================

main() {
    # Initialize colors
    setup_colors

    # Parse command-line arguments
    parse_arguments "$@"

    # Show help if requested
    if [[ "$OPT_HELP" == true ]]; then
        show_help
        exit 0
    fi

    # Print env mode
    if [[ "$OPT_PRINT_ENV" == true ]]; then
        print_env_commands
        exit 0
    fi

    # Header
    echo ""
    echo -e "${C_BOLD}${C_CYAN}`dasae-headers` Installation Script${C_RESET} v$SCRIPT_VERSION"
    echo -e "${C_DIM}  - GitHub: <https://github.com/coding-pelican/dasae-headers>${C_RESET}"
    echo ""

    if [[ "$OPT_DRY_RUN" == true ]]; then
        log_warn "Running in dry-run mode. No changes will be made."
    fi

    # Detection phase
    detect_os
    detect_toolchain

    # Check if we have a usable compiler or can install one
    if [[ "$DETECTED_CC_TIER" == "unsupported" ]] && [[ "$OPT_SKIP_TOOLCHAIN" == true ]]; then
        log_error "No supported compiler found and toolchain installation is skipped."
        echo ""
        echo "Minimum requirements:"
        echo "  Clang: $CLANG_SUPPORTED+ (Recommended: $CLANG_RECOMMENDED+)"
        echo "  GCC: $GCC_SUPPORTED+ (Recommended: $GCC_RECOMMENDED+)"
        exit 1
    fi

    # Interactive selection
    interactive_selection

    # Execute installation steps
    install_toolchain
    install_dhc
    register_dh_home

    # Final summary
    print_header "Installation Complete"

    echo ""
    log_success "`dasae-headers` has been set up successfully!"
    echo ""

    # Post-installation instructions
    case "$ENV_ACTION" in
        register-local)
            echo "To start using `dasae-headers`:"
            echo -e "  ${C_CYAN}source $SCRIPT_DIR/.dh-env${C_RESET}"
            ;;
        register-global)
            echo "To apply environment changes:"
            echo -e "  ${C_CYAN}source $SHELL_CONFIG_FILE${C_RESET}"
            echo "Or open a new terminal session."
            ;;
        skip)
            echo "Remember to set `DH_HOME` before using `dasae-headers`:"
            echo -e "  ${C_CYAN}export DH_HOME=\"$SCRIPT_DIR/dh\"${C_RESET}"
            ;;
    esac

    echo ""
    echo "Quick start:"
    echo -e "  ${C_CYAN}dh-c project myproject${C_RESET}    # Create a new project"
    echo -e "  ${C_CYAN}cd myproject${C_RESET}"
    echo -e "  ${C_CYAN}dh-c run dev${C_RESET}              # Build and run"
    echo ""
}

# Run main function
main "$@"
