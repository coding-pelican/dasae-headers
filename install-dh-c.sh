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

# =============================================================================
# Global Variables and Configuration
# =============================================================================

# Script parameters
SKIP_ENV_SETUP=false

# Global variables
INSTALL_DIR=""
SOURCE_PATH="$(pwd)"
IS_WINDOWS=false
IS_UNIX_SHELL=false
SHELL_CONFIG=""
WIN_BAT_PATH=""

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

# =============================================================================
# Core Helper Functions
# =============================================================================

print_message() {
    local message="$1"
    local color="${2:-$RESET}"
    printf "%b%s%b\n" "$color" "$message" "$RESET"
}

# --- Path Management ---

unix_path() {
    local path="$1"
    if $IS_UNIX_SHELL && command -v cygpath &>/dev/null; then
        cygpath -u "$path"
    else
        echo "$path"
    fi
}

windows_path() {
    local path="$1"
    if command -v cygpath &>/dev/null; then
        cygpath -w "$path"
    else
        echo "${path//\//\\}"
    fi
}

join_path() {
    local base_path="${1%/}"
    local component="${2#/}"
    base_path="${base_path//\\//}"
    echo "${base_path}/${component}"
}

normalize_path_for_display() {
    echo "${1//\\//}"
}

# =============================================================================
# Command-Line Argument Parsing
# =============================================================================

show_help() {
    print_message "DH-C Installation Script" "$CYAN"
    print_message "Usage: $0 [options]" "$CYAN"
    print_message "Options:" "$CYAN"
    print_message "  --skip-env    Skip environment variable setup" "$CYAN"
    print_message "  --help        Show this help message" "$CYAN"
}

parse_arguments() {
    while [ "$#" -gt 0 ]; do
        case "$1" in
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

# =============================================================================
# Core Installation Logic
# =============================================================================

detect_platform() {
    if [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* || "$OSTYPE" == "win32"* ]]; then
        IS_WINDOWS=true
        if [[ "$(uname -s)" == "MINGW"* ]] || [[ "$MSYSTEM" == "MINGW"* ]]; then
            IS_UNIX_SHELL=true
        fi
        print_message "Detected Windows with Unix-like shell environment" "$YELLOW"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        print_message "Detected Linux environment" "$YELLOW"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        print_message "Detected macOS environment" "$YELLOW"
    else
        print_message "Detected Unix-like environment" "$YELLOW"
    fi

    # Set installation directory
    if $IS_WINDOWS; then
        local win_install_path
        win_install_path="$(windows_path "$USERPROFILE/.dh-c")"
        INSTALL_DIR="$(unix_path "$win_install_path")"
    else
        INSTALL_DIR="$HOME/.dh-c"
    fi
}

check_clang() {
    print_message "Checking for clang..." "$YELLOW"
    if command -v clang &>/dev/null; then
        local ver
        ver=$(clang --version | head -n1)
        print_message "Found clang: $ver" "$GREEN"
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

setup_directories() {
    print_message "Creating installation directory: $(normalize_path_for_display "$INSTALL_DIR")" "$YELLOW"
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$(join_path "$SOURCE_PATH" "dh")"
}

compile_dh_c() {
    print_message "Compiling dh-c..." "$YELLOW"
    local source_file="$(join_path "$SOURCE_PATH" "dh-c.c")"

    if [ ! -f "$source_file" ]; then
        print_message "Error: Could not find dh-c.c in the current directory." "$RED"
        return 1
    fi

    local output_file="$(join_path "$INSTALL_DIR" "dh-c")"
    if $IS_WINDOWS; then
        output_file="${output_file}.exe"
    fi

    clang -std=c17 -Wall -Wextra -O3 -flto -o "$output_file" "$source_file"
    chmod +x "$output_file"

    if [ -f "$output_file" ]; then
        print_message "DH-C compiled successfully!" "$GREEN"
    else
        print_message "Error: Compilation failed." "$RED"
        return 1
    fi
}

# =============================================================================
# Platform-Specific Setup
# =============================================================================

# -----------------------------------------------------------------------------
# Windows Setup
# -----------------------------------------------------------------------------

create_windows_files() {
    local install_path_win="$1"
    local source_path_win="$2"
    local install_dir="$3"

    # Create backup script
    local backup_file="$(join_path "$install_dir" "backup_env.bat")"
    cat >"$backup_file" <<EOF
@echo off
echo Creating backup of current environment variables...
echo This will save your current PATH and DH_HOME values.
echo.

REM Export current user environment variables to backup file
reg export "HKEY_CURRENT_USER\\Environment" "%~dp0env_backup.reg" /y
if errorlevel 1 (
    echo Failed to create backup!
    echo ERROR: Could not export registry keys.
    pause
    exit /b 1
)

echo Backup created successfully!
echo Location: %~dp0env_backup.reg
echo.
echo This backup contains your current environment variables and can be used
echo to restore your settings if needed using restore_env.bat
echo.
pause
EOF

    # Create batch script that uses reg add commands
    local batch_file="$(join_path "$install_dir" "apply_env.bat")"
    WIN_BAT_PATH="$(windows_path "$batch_file")"

    cat >"$batch_file" <<EOF
@echo off
setlocal enabledelayedexpansion
echo DH-C Environment Setup
echo ====================

REM Create backup first
call "%~dp0backup_env.bat"
if errorlevel 1 (
    echo Failed to create backup!
    pause
    exit /b 1
)

echo.
echo Setting up environment variables...

REM Set DH_HOME
reg add "HKEY_CURRENT_USER\\Environment" /v DH_HOME /t REG_SZ /d "${source_path_win//\\/\\\\}" /f
if !errorlevel! neq 0 (
    echo Failed to set DH_HOME!
    pause
    exit /b 1
)

REM Add to PATH
REM First, get the current user PATH
set "USER_PATH="
for /f "tokens=2,*" %%a in ('reg query "HKEY_CURRENT_USER\\Environment" /v PATH 2^>nul') do set "USER_PATH=%%b"

REM Check if our path is already in the user PATH
echo "!USER_PATH!" | findstr /i /c:"${install_path_win//\\/\\\\}" >nul
if %errorlevel% == 0 (
    echo DH-C path already in user PATH.
) else (
    if defined USER_PATH (
        set "NEW_PATH=${install_path_win//\\/\\\\};!USER_PATH!"
    ) else (
        set "NEW_PATH=${install_path_win//\\/\\\\}"
    )
    reg add "HKEY_CURRENT_USER\\Environment" /v PATH /t REG_EXPAND_SZ /d "!NEW_PATH!" /f
    if !errorlevel! neq 0 (
        echo Failed to update PATH!
        pause
        exit /b 1
    )
)

echo.
echo Environment variables updated successfully!
echo DH_HOME set to: ${source_path_win//\\/\\\\}
echo Added to PATH: ${install_path_win//\\/\\\\}
echo.
echo Please restart your command prompt or log out and back in for changes to take effect.
pause
EOF

    # Create restore script
    local restore_file="$(join_path "$install_dir" "restore_env.bat")"
    cat >"$restore_file" <<EOF
@echo off
echo Restoring environment variables from backup...
if not exist "%~dp0env_backup.reg" (
    echo Backup file not found: %~dp0env_backup.reg
    echo Cannot restore environment variables.
    pause
    exit /b 1
)

reg import "%~dp0env_backup.reg"
if errorlevel 1 (
    echo Failed to restore environment variables!
    pause
    exit /b 1
)

echo Environment variables restored successfully!
pause
EOF

    # Create registry repair script
    local repair_file="$(join_path "$install_dir" "repair_registry.bat")"
    cat >"$repair_file" <<EOF
@echo off
setlocal enabledelayedexpansion
echo DH-C Registry Repair Tool
echo =========================
echo.
echo This tool will repair common registry issues with DH-C environment variables.
echo.

REM Check if running as administrator
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Running as administrator - Good!
) else (
    echo WARNING: Not running as administrator. Some repairs may fail.
    echo Please run this script as administrator for best results.
    echo.
)

echo Checking current environment variables...
echo.

REM Display current values
echo Current DH_HOME: %DH_HOME%
echo Current PATH contains DH-C:
echo %PATH% | findstr /i "${install_path_win//\\/\\\\}" >nul && echo YES || echo NO
echo.

echo Creating backup before repair...
call "%~dp0backup_env.bat"
if errorlevel 1 (
    echo Failed to create backup! Aborting repair for safety.
    pause
    exit /b 1
)

echo Starting registry repair...
echo.

REM Get current system PATH for rebuilding
echo 1. Getting system PATH...
for /f "tokens=2*" %%A in ('reg query "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" /v PATH 2^>nul') do set "SYSTEM_PATH=%%B"

REM Get current user PATH to preserve non-DH-C entries
echo 2. Preserving existing user PATH entries...
for /f "tokens=2,*" %%a in ('reg query "HKEY_CURRENT_USER\\Environment" /v PATH 2^>nul') do set "USER_PATH=%%b"

REM Remove old DH-C entries from PATH if they exist
echo 3. Cleaning old DH-C entries...
if defined USER_PATH (
    set "CLEAN_PATH=!USER_PATH:${install_path_win//\\/\\\\};=!"
    set "CLEAN_PATH=!CLEAN_PATH:;${install_path_win//\\/\\\\}=!"
    set "CLEAN_PATH=!CLEAN_PATH:${install_path_win//\\/\\\\}=!"
) else (
    set "CLEAN_PATH=%SYSTEM_PATH%"
)

REM Set clean environment variables
echo 4. Setting clean DH-C environment variables...
reg add "HKEY_CURRENT_USER\\Environment" /v DH_HOME /t REG_SZ /d "${source_path_win//\\/\\\\}" /f
if !errorlevel! neq 0 (
    echo Failed to set DH_HOME!
    goto :error
)

reg add "HKEY_CURRENT_USER\\Environment" /v PATH /t REG_EXPAND_SZ /d "${install_path_win//\\/\\\\};!CLEAN_PATH!" /f
if !errorlevel! neq 0 (
    echo Failed to set PATH!
    goto :error
)

echo.
echo 5. Verifying registry entries...
reg query "HKEY_CURRENT_USER\\Environment" /v "DH_HOME" >nul 2>&1
if errorlevel 1 (
    echo ERROR: DH_HOME not found in registry!
    goto :error
)

reg query "HKEY_CURRENT_USER\\Environment" /v "PATH" >nul 2>&1
if errorlevel 1 (
    echo ERROR: PATH not found in registry!
    goto :error
)

echo.
echo Registry repair completed successfully!
echo.
echo Changes made:
echo - DH_HOME set to: ${source_path_win//\\/\\\\}
echo - PATH updated to include: ${install_path_win//\\/\\\\}
echo - Backup saved to: %~dp0env_backup.reg
echo.
echo Please restart your command prompt or log out and back in.
echo.
pause
exit /b 0

:error
echo.
echo Registry repair failed!
echo Your original settings have been backed up to: %~dp0env_backup.reg
echo You can restore them using restore_env.bat
echo.
echo Please try running this script as administrator.
echo If problems persist, you may need to manually set:
echo   DH_HOME = ${source_path_win//\\/\\\\}
echo   PATH = ${install_path_win//\\/\\\\};[existing PATH]
echo.
pause
exit /b 1
EOF

    # Create comprehensive diagnostic script
    local diag_file="$(join_path "$install_dir" "diagnose_env.bat")"
    cat >"$diag_file" <<EOF
@echo off
echo DH-C Environment Diagnostics
echo ============================
echo.

echo 1. Current Environment Variables:
echo    DH_HOME = %DH_HOME%
echo    PATH contains DH-C:
echo %PATH% | findstr /i "${install_path_win//\\/\\\\}" >nul && echo    YES || echo    NO
echo.

echo 2. Registry Entries:
echo    Checking HKEY_CURRENT_USER\\Environment...
reg query "HKEY_CURRENT_USER\\Environment" /v "DH_HOME" 2>nul || echo    DH_HOME: NOT FOUND
reg query "HKEY_CURRENT_USER\\Environment" /v "PATH" 2>nul || echo    PATH: NOT FOUND
echo.

echo 3. File System Check:
echo    DH-C executable:
if exist "${install_path_win//\\/\\\\}\\dh-c.exe" (
    echo    FOUND: ${install_path_win//\\/\\\\}\\dh-c.exe
) else (
    echo    NOT FOUND: ${install_path_win//\\/\\\\}\\dh-c.exe
)
echo    DH source directory:
if exist "${source_path_win//\\/\\\\}" (
    echo    FOUND: ${source_path_win//\\/\\\\}
) else (
    echo    NOT FOUND: ${source_path_win//\\/\\\\}
)
echo.

echo 4. Recommendations:
echo %PATH% | findstr /i "${install_path_win//\\/\\\\}" >nul || echo    - Run apply_env.bat to set environment variables
reg query "HKEY_CURRENT_USER\\Environment" /v "DH_HOME" >nul 2>&1 || echo    - Run repair_registry.bat to fix registry issues
if not exist "${install_path_win//\\/\\\\}\\dh-c.exe" echo    - Reinstall DH-C (dh-c.exe is missing)
echo.

pause
EOF

    print_message "Created Windows batch scripts with safety measures." "$YELLOW"
    print_message "IMPORTANT: A backup script has been created for your environment variables." "$YELLOW"
    print_message "Available tools:" "$YELLOW"
    print_message "  - Backup: $(normalize_path_for_display "$(windows_path "$backup_file")")" "$YELLOW"
    print_message "  - Apply: $(normalize_path_for_display "$WIN_BAT_PATH")" "$YELLOW"
    print_message "  - Diagnose: $(normalize_path_for_display "$(windows_path "$diag_file")")" "$YELLOW"
    print_message "  - Restore: $(normalize_path_for_display "$(windows_path "$restore_file")")" "$YELLOW"
    print_message "  - Repair: $(normalize_path_for_display "$(windows_path "$repair_file")")" "$YELLOW"
}

setup_windows_env() {
    if $SKIP_ENV_SETUP; then
        print_message "Skipping environment variable setup as requested." "$YELLOW"
        return 0
    fi

    print_message "Setting up Windows environment variables..." "$YELLOW"

    # Check if already set
    if [ -n "$DH_HOME" ] && echo "$PATH" | grep -qF "$INSTALL_DIR"; then
        print_message "Environment variables are already set correctly." "$GREEN"
        return 0
    fi

    local dh_source_path="$(join_path "$SOURCE_PATH" "dh")"
    local install_path_win="$(windows_path "$INSTALL_DIR")"
    local source_path_win="$(windows_path "$dh_source_path")"

    print_message "Windows install dir: $(normalize_path_for_display "$install_path_win")" "$YELLOW"
    print_message "Windows source path: $(normalize_path_for_display "$source_path_win")" "$YELLOW"

    create_windows_files "$install_path_win" "$source_path_win" "$INSTALL_DIR"
}

# -----------------------------------------------------------------------------
# Unix/Linux/macOS Setup
# -----------------------------------------------------------------------------

add_to_shell_config() {
    local config_file="$1"
    if ! grep -q "export PATH=.*$INSTALL_DIR" "$config_file" 2>/dev/null; then
        {
            echo ""
            echo "# Added by DH-C installer"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\""
            echo "export DH_HOME=\"$(join_path "$SOURCE_PATH" "dh")\""
        } >>"$config_file"
        print_message "Updated shell environment in $config_file" "$GREEN"
    fi
}

setup_macos_env() {
    print_message "Setting up macOS LaunchAgent for persistent environment variables..." "$YELLOW"

    local launch_agent_dir="$HOME/Library/LaunchAgents"
    mkdir -p "$launch_agent_dir"
    local plist_file="$(join_path "$launch_agent_dir" "com.dh-c.environment.plist")"

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
        <string>$(join_path "$SOURCE_PATH" "dh")</string>
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

    local wrapper_script="$(join_path "$INSTALL_DIR" "dh-c-wrapper.sh")"
    cat >"$wrapper_script" <<EOF
#!/bin/bash
export DH_HOME="$(join_path "$SOURCE_PATH" "dh")"
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
}

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
        print_message "export DH_HOME=\"$(join_path "$SOURCE_PATH" "dh")\"" "$YELLOW"
        return 0
    fi

    add_to_shell_config "$SHELL_CONFIG"

    if [[ "$OSTYPE" == "darwin"* ]]; then
        setup_macos_env
    fi
}

# =============================================================================
# Final Instructions
# =============================================================================

show_instructions() {
    print_message ""
    print_message "DH-C installation complete!" "$CYAN"
    print_message "You can now use dh-c from any directory:" "$CYAN"
    print_message "  dh-c --version" "$CYAN"
    print_message "  dh-c workspace my-workspace" "$CYAN"
    print_message "  dh-c project my-project" "$CYAN"

    if $IS_WINDOWS && ! $IS_UNIX_SHELL; then
        print_message ""
        print_message "To complete the Windows installation:" "$YELLOW"
        print_message "1. RECOMMENDED: Create a backup first by running:" "$CYAN"
        print_message "   $(normalize_path_for_display "$(windows_path "$(join_path "$INSTALL_DIR" "backup_env.bat")")")" "$CYAN"
        print_message ""
        print_message "2. Set up environment variables by running AS ADMINISTRATOR:" "$CYAN"
        print_message "   $(normalize_path_for_display "$WIN_BAT_PATH")" "$CYAN"
        print_message ""
        print_message "3. Restart your command prompt or log out and back in" "$CYAN"
        print_message ""
        print_message "4. Verify installation by running:" "$CYAN"
        print_message "   $(normalize_path_for_display "$(windows_path "$(join_path "$INSTALL_DIR" "diagnose_env.bat")")")" "$CYAN"
        print_message ""
        print_message "If you encounter issues:" "$YELLOW"
        print_message "• Use Diagnose tool to identify problems" "$CYAN"
        print_message "• Use Repair tool if registry issues are detected" "$CYAN"
        print_message "• Use Restore tool to revert to backup if needed" "$CYAN"
    elif [ -n "$SHELL_CONFIG" ] || $IS_UNIX_SHELL; then
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

# =============================================================================
# Main Execution
# =============================================================================

install_dh_c() {
    print_message "DH-C Installation Script" "$CYAN"
    print_message "-----------------------" "$CYAN"

    detect_platform
    check_clang || return 1
    setup_directories
    compile_dh_c || return 1

    # Choose setup method based on environment
    if $IS_WINDOWS && ! $IS_UNIX_SHELL; then
        setup_windows_env || print_message "Warning: Failed to set up Windows environment." "$YELLOW"
    else
        setup_unix_env || print_message "Warning: Failed to set up Unix environment." "$YELLOW"
    fi

    show_instructions
}

# --- Script Entry Point ---
parse_arguments "$@"
install_dh_c
exit 0
