#!/bin/bash
# Cross-platform DH-C Installation Script
# Works on Windows (Git Bash/WSL/etc.) and Unix-like systems
#
# Make it executable:
#   chmod +x install-dh-c.sh
# Run it:
#   ./install-dh-c.sh
# or
#   sh install-dh-c.sh

set -e # Exit on any error

# Script parameters
FORCE=false
SKIP_ENV_SETUP=false

# Define colors
if [ -t 1 ]; then # Check if stdout is a terminal
    CYAN="\033[1;36m"
    GREEN="\033[1;32m"
    YELLOW="\033[1;33m"
    RED="\033[1;31m"
    RESET="\033[0m"
else
    CYAN=""
    GREEN=""
    YELLOW=""
    RED=""
    RESET=""
fi

# Global variables
INSTALL_DIR=""
SOURCE_PATH="$(pwd)"
SOURCE_FILE=""
IS_WINDOWS=false
IS_GITBASH=false
SHELL_CONFIG=""
WIN_BAT_PATH=""

# Print colorized messages
print_message() {
    local message="$1"
    local color="${2:-$RESET}"
    printf "%b%s%b\n" "$color" "$message" "$RESET"
}

# Path management functions
unix_path() {
    local path="$1"
    if $IS_GITBASH; then
        cygpath -u "$path"
    else
        echo "$path"
    fi
}

windows_path() {
    local path="$1"
    if command -v cygpath &>/dev/null; then
        # Cygwin / MSYS2 / Git Bash all support cygpath
        cygpath -w "$path"
    else
        # Fallback: just swap slashes for backslashes
        echo "${path//\//\\}"
    fi
}

join_unix_path() {
    local base="${1%/}"
    local comp="${2#/}"
    echo "${base}/${comp}"
}

join_windows_path() {
    local base="${1%\\}"
    local comp="${2#\\}"
    echo "${base}\\${comp}"
}

# Parse command line arguments
parse_arguments() {
    while [ "$#" -gt 0 ]; do
        case "$1" in
        --force) FORCE=true ;;
        --skip-env) SKIP_ENV_SETUP=true ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            print_message "Unknown option: $1" "$RED"
            show_help
            exit 1
            ;;
        esac
        shift
    done
}

show_help() {
    print_message "DH-C Installation Script" "$CYAN"
    print_message "Usage: $0 [options]" "$CYAN"
    print_message "Options:" "$CYAN"
    print_message "  --force       Force installation even if already installed" "$CYAN"
    print_message "  --skip-env    Skip environment variable setup" "$CYAN"
    print_message "  --help        Show this help message" "$CYAN"
}

# Detect platform
detect_platform() {
    if [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* || "$OSTYPE" == "win32"* ]]; then
        IS_WINDOWS=true
        if [[ "$(uname -s)" == "MINGW"* ]] || [[ "$MSYSTEM" == "MINGW"* ]]; then
            IS_GITBASH=true
        fi
        print_message "Detected Windows environment (Git Bash/MSYS2)" "$YELLOW"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        print_message "Detected Linux environment" "$YELLOW"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        print_message "Detected macOS environment" "$YELLOW"
    else
        print_message "Detected Unix-like environment" "$YELLOW"
    fi

    # Set installation directory
    if $IS_WINDOWS; then
        local win_install_dir
        win_install_dir="$(join_windows_path "$USERPROFILE" ".dh-c")"
        INSTALL_DIR="$(unix_path "$win_install_dir")"
    else
        INSTALL_DIR="$(join_unix_path "$HOME" ".dh-c")"
    fi

    SOURCE_FILE="$(join_unix_path "$SOURCE_PATH" "dh-c.c")"
}

# Check if clang is installed
check_clang() {
    print_message "Checking for clang..." "$YELLOW"
    if command -v clang &>/dev/null; then
        local ver
        ver=$(clang --version | head -n1)
        print_message "Found clang: $ver" "$GREEN"
        return 0
    else
        print_message "Error: Clang compiler not found. Please install clang first." "$RED"
        if $IS_WINDOWS; then
            print_message "Download from: https://releases.llvm.org/" "$RED"
        elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
            print_message "On Ubuntu/Debian: sudo apt install clang" "$RED"
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            print_message "On macOS with Homebrew: brew install llvm" "$RED"
        fi
        return 1
    fi
}

# Create installation directory
setup_directories() {
    print_message "Creating installation directory: $INSTALL_DIR" "$YELLOW"
    mkdir -p "$INSTALL_DIR"

    local dh_dir
    dh_dir="$(join_unix_path "$SOURCE_PATH" "dh")"
    mkdir -p "$dh_dir"

    return 0
}

# Compile dh-c
compile_dh_c() {
    print_message "Compiling dh-c..." "$YELLOW"
    if [ ! -f "$SOURCE_FILE" ]; then
        print_message "Error: Could not find dh-c.c in the current directory." "$RED"
        return 1
    fi

    local output_file
    if $IS_WINDOWS; then
        output_file="$(join_unix_path "$INSTALL_DIR" "dh-c.exe")"
    else
        output_file="$(join_unix_path "$INSTALL_DIR" "dh-c")"
    fi

    clang -std=c17 -Wall -Wextra -O3 -o "$output_file" "$SOURCE_FILE"
    chmod +x "$output_file"

    if [ -f "$output_file" ]; then
        print_message "DH-C compiled successfully!" "$GREEN"
        return 0
    else
        print_message "Error: Compilation failed." "$RED"
        return 1
    fi
}

# Set up Windows-specific environment
setup_windows_env() {
    if $SKIP_ENV_SETUP; then
        print_message "Skipping environment variable setup as requested." "$YELLOW"
        return 0
    fi

    print_message "Setting up Windows environment variables..." "$YELLOW"

    local dh_source_unix
    dh_source_unix="$(join_unix_path "$SOURCE_PATH" "dh")"

    local win_install_dir win_source_path
    # always convert to a real Windows path
    win_install_dir="$(windows_path "$INSTALL_DIR")"
    win_source_path="$(windows_path "$dh_source_unix")"

    # Escape backslashes for display
    local display_install_dir="${win_install_dir//\\/\\\\}"
    local display_source_path="${win_source_path//\\/\\\\}"

    print_message "Windows install dir: $display_install_dir" "$YELLOW"
    print_message "Windows source path: $display_source_path" "$YELLOW"

    # Batch file path
    local win_bat_file
    if [[ "$win_install_dir" == *"\\" ]]; then
        win_bat_file="${win_install_dir}apply_env.bat"
    else
        win_bat_file="${win_install_dir}\\apply_env.bat"
    fi
    local display_bat="${win_bat_file//\\/\\\\}"
    print_message "Windows batch file path: $display_bat" "$YELLOW"
    WIN_BAT_PATH="$display_bat"

    # Create registry file
    local reg_file
    reg_file="$(join_unix_path "$INSTALL_DIR" "dh_env.reg")"
    cat >"$reg_file" <<EOF
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\\Environment]
"PATH"="${win_install_dir//\\/\\\\};%PATH%"
"DH_HOME"="${win_source_path//\\/\\\\}"
EOF

    # Create batch script
    local bat_file
    bat_file="$(join_unix_path "$INSTALL_DIR" "apply_env.bat")"
    cat >"$bat_file" <<EOF
@echo off
echo Updating environment variables...
reg import "%~dp0dh_env.reg"
if errorlevel 1 (
    echo Failed to update environment variables!
    pause
    exit /b 1
)
echo Environment variables updated successfully!
echo DH_HOME set to: ${win_source_path//\\/\\\\}
echo Added to PATH: ${win_install_dir//\\/\\\\}
pause
EOF

    print_message "Created Windows registry script and batch file." "$YELLOW"

    if $IS_GITBASH; then
        setup_gitbash_env
    fi

    return 0
}

# Set up Git Bash environment
setup_gitbash_env() {
    local shell_config="$HOME/.bashrc"
    if ! grep -q "export PATH=.*$INSTALL_DIR" "$shell_config" 2>/dev/null; then
        {
            echo ""
            echo "# Added by DH-C installer"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\""
            echo "export DH_HOME=\"$(join_unix_path "$SOURCE_PATH" "dh")\""
        } >>"$shell_config"
        print_message "Updated Git Bash environment in $shell_config" "$GREEN"
    fi
}

# Set up Unix environment
setup_unix_env() {
    if $SKIP_ENV_SETUP; then
        print_message "Skipping environment variable setup as requested." "$YELLOW"
        return 0
    fi

    print_message "Setting up Unix environment variables..." "$YELLOW"

    if [[ "$SHELL" == *"bash"* ]]; then
        SHELL_CONFIG="$HOME/.bashrc"
    elif [[ "$SHELL" == *"zsh"* ]]; then
        SHELL_CONFIG="$HOME/.zshrc"
    else
        print_message "Unsupported shell. Please add the following to your shell configuration file manually:" "$YELLOW"
        print_message "export PATH=\"$INSTALL_DIR:\$PATH\"" "$YELLOW"
        print_message "export DH_HOME=\"$(join_unix_path "$SOURCE_PATH" "dh")\"" "$YELLOW"
        return 0
    fi

    if ! grep -q "export PATH=.*$INSTALL_DIR" "$SHELL_CONFIG" 2>/dev/null; then
        {
            echo ""
            echo "# Added by DH-C installer"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\""
            echo "export DH_HOME=\"$(join_unix_path "$SOURCE_PATH" "dh")\""
        } >>"$SHELL_CONFIG"
        print_message "Updated shell environment in $SHELL_CONFIG" "$GREEN"
    fi

    if [[ "$OSTYPE" == "darwin"* ]]; then
        setup_macos_env
    fi

    return 0
}

# Set up macOS specific environment
setup_macos_env() {
    print_message "Setting up macOS LaunchAgent for persistent environment variables..." "$YELLOW"

    local launch_agent_dir
    launch_agent_dir="$(join_unix_path "$HOME" "Library/LaunchAgents")"
    mkdir -p "$launch_agent_dir"
    local plist_file
    plist_file="$(join_unix_path "$launch_agent_dir" "com.dh-c.environment.plist")"

    cat >"$plist_file" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
  "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.dh-c.environment</string>
    <key>ProgramArguments</key>
    <array>
        <string>/bin/sh</string>
        <string>-c</string>
        <string>exit 0</string>
    </array>
    <key>EnvironmentVariables</key>
    <dict>
        <key>DH_HOME</key>
        <string>$(join_unix_path "$SOURCE_PATH" "dh")</string>
        <key>PATH</key>
        <string>$INSTALL_DIR:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin</string>
    </dict>
    <key>RunAtLoad</key>
    <true/>
    <key>GlobalEnvironment</key>
    <true/>
</dict>
</plist>
EOF

    local wrapper_script
    wrapper_script="$(join_unix_path "$INSTALL_DIR" "dh-c-wrapper.sh")"
    cat >"$wrapper_script" <<EOF
#!/bin/bash
export DH_HOME="$(join_unix_path "$SOURCE_PATH" "dh")"
exec $INSTALL_DIR/dh-c "\$@"
EOF
    chmod +x "$wrapper_script"

    if ! grep -q "alias dh-c=" "$SHELL_CONFIG" 2>/dev/null; then
        {
            echo ""
            echo "# Alias to ensure DH_HOME is set correctly"
            echo "alias dh-c=\"$wrapper_script\""
        } >>"$SHELL_CONFIG"
    fi

    launchctl unload "$plist_file" 2>/dev/null || true
    launchctl load "$plist_file"

    print_message "Created and loaded LaunchAgent for environment variables" "$GREEN"
    print_message "Created wrapper script to ensure environment variables are set" "$GREEN"
}

# Show final instructions
show_instructions() {
    print_message ""
    print_message "DH-C installation complete!" "$CYAN"
    print_message "You can now use dh-c from any directory:" "$CYAN"
    print_message "  dh-c --version" "$CYAN"
    print_message "  dh-c workspace my-workspace" "$CYAN"
    print_message "  dh-c project my-project" "$CYAN"

    if $IS_WINDOWS; then
        print_message ""
        print_message "Important: To complete the Windows installation:" "$YELLOW"
        print_message "1. Run the batch file as administrator: $WIN_BAT_PATH" "$CYAN"
        print_message "2. Log out and log back in, or restart your computer" "$CYAN"
        if $IS_GITBASH; then
            print_message "3. For Git Bash, also run: source $HOME/.bashrc" "$CYAN"
        fi
    elif [ -n "$SHELL_CONFIG" ]; then
        print_message ""
        print_message "To start using dh-c immediately, run:" "$YELLOW"
        print_message "  source $SHELL_CONFIG" "$CYAN"
        if [[ "$OSTYPE" == "darwin"* ]]; then
            print_message ""
            print_message "Environment variables have been set up with a LaunchAgent." "$GREEN"
            print_message "They will be available in all new terminal sessions." "$GREEN"
        fi
    fi
}

# Main installation function
install_dh_c() {
    print_message "DH-C Installation Script" "$CYAN"
    print_message "-----------------------" "$CYAN"

    detect_platform

    if ! check_clang; then
        return 1
    fi

    if ! setup_directories; then
        return 1
    fi

    if ! compile_dh_c; then
        return 1
    fi

    if $IS_WINDOWS; then
        setup_windows_env || print_message "Warning: Failed to set up Windows environment. Some features may not work correctly." "$YELLOW"
    else
        setup_unix_env || print_message "Warning: Failed to set up Unix environment. Some features may not work correctly." "$YELLOW"
    fi

    show_instructions
    return 0
}

# Parse command line arguments and run
parse_arguments "$@"
install_dh_c
exit 0
