#!/bin/bash
# Cross-platform DH-C Installation Script
# Works on Windows (Git Bash/WSL/etc.) and Unix-like systems
#
# Make it executable
# chmod +x install-dh-c.sh
#
# Run it
# ./install-dh-c.sh
# or
# sh install-dh-c.sh

set -e # Exit on any error

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

echo -e "${CYAN}DH-C Installation Script${RESET}"
echo -e "${CYAN}-----------------------${RESET}"

# Detect OS and shell environment
IS_WINDOWS=false
IS_GITBASH=false
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    IS_WINDOWS=true
    if [[ "$(uname -s)" == "MINGW"* ]] || [[ "$MSYSTEM" == "MINGW"* ]]; then
        IS_GITBASH=true
    fi
    echo -e "${YELLOW}Detected Windows environment (Git Bash/MSYS2)${RESET}"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "${YELLOW}Detected Linux environment${RESET}"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo -e "${YELLOW}Detected macOS environment${RESET}"
else
    echo -e "${YELLOW}Detected Unix-like environment${RESET}"
fi

# Step 1: Check if clang is installed
echo -e "${YELLOW}Checking for clang...${RESET}"
if command -v clang &>/dev/null; then
    CLANG_VERSION=$(clang --version | head -n 1)
    echo -e "${GREEN}Found clang: $CLANG_VERSION${RESET}"
else
    echo -e "${RED}Error: Clang compiler not found. Please install clang first.${RESET}"
    if $IS_WINDOWS; then
        echo -e "${RED}Download from: https://releases.llvm.org/${RESET}"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo -e "${RED}On Ubuntu/Debian: sudo apt install clang${RESET}"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo -e "${RED}On macOS with Homebrew: brew install llvm${RESET}"
    fi
    exit 1
fi

# Step 2: Create installation directory
if $IS_WINDOWS; then
    # On Windows/Git Bash, use Windows-style paths
    INSTALL_DIR="$USERPROFILE/.dh-c"
    if $IS_GITBASH; then
        # Convert to Unix-style path for Git Bash
        INSTALL_DIR=$(cygpath -u "$INSTALL_DIR")
    fi
else
    INSTALL_DIR="$HOME/.dh-c"
fi

echo -e "${YELLOW}Creating installation directory: $INSTALL_DIR${RESET}"
mkdir -p "$INSTALL_DIR"

# Step 3: Compile dh-c
SOURCE_PATH="$(pwd)"
SOURCE_FILE="$SOURCE_PATH/dh-c.c"

if [ ! -f "$SOURCE_FILE" ]; then
    echo -e "${RED}Error: Could not find dh-c.c in the current directory.${RESET}"
    exit 1
fi

echo -e "${YELLOW}Compiling dh-c...${RESET}"
if $IS_WINDOWS; then
    OUTPUT_FILE="$INSTALL_DIR/dh-c.exe"
else
    OUTPUT_FILE="$INSTALL_DIR/dh-c"
fi

clang -std=c17 -Wall -Wextra -O3 -o "$OUTPUT_FILE" "$SOURCE_FILE"

if [ ! -f "$OUTPUT_FILE" ]; then
    echo -e "${RED}Error: Compilation failed.${RESET}"
    exit 1
fi

# Make the binary executable
chmod +x "$OUTPUT_FILE"
echo -e "${GREEN}DH-C compiled successfully!${RESET}"

# Step 4: Update PATH and environment variables
echo -e "${YELLOW}Setting up environment variables...${RESET}"

if $IS_WINDOWS; then
    # Convert paths to absolute Windows paths
    if $IS_GITBASH; then
        # Get absolute paths in Windows format
        WIN_INSTALL_DIR=$(cd "$INSTALL_DIR" && pwd -W)
        WIN_SOURCE_PATH=$(cd "$SOURCE_PATH/dh" && pwd -W)
    else
        # For other Windows environments, use cygpath with absolute path conversion
        WIN_INSTALL_DIR=$(cd "$INSTALL_DIR" && cygpath -w "$(pwd)")
        WIN_SOURCE_PATH=$(cd "$SOURCE_PATH/dh" && cygpath -w "$(pwd)")
    fi

    # Escape backslashes for the registry file
    WIN_INSTALL_DIR=${WIN_INSTALL_DIR//\\/\\\\}
    WIN_SOURCE_PATH=${WIN_SOURCE_PATH//\\/\\\\}

    REG_FILE="$INSTALL_DIR/dh_env.reg"

    echo "Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Environment]
\"PATH\"=\"${WIN_INSTALL_DIR};%PATH%\"
\"DH_HOME\"=\"${WIN_SOURCE_PATH}\"" >"$REG_FILE"

    # Create a batch file to apply the registry changes
    BAT_FILE="$INSTALL_DIR/apply_env.bat"
    echo "@echo off
echo Updating environment variables...
reg import \"%~dp0dh_env.reg\"
if errorlevel 1 (
    echo Failed to update environment variables!
    pause
    exit /b 1
)
echo Environment variables updated successfully!
echo DH_HOME set to: ${WIN_SOURCE_PATH}
echo Added to PATH: ${WIN_INSTALL_DIR}
pause" >"$BAT_FILE"

    echo -e "${YELLOW}Created Windows registry script and batch file.${RESET}"
    echo -e "${YELLOW}To complete installation, run the following as administrator:${RESET}"
    echo -e "${CYAN}$WIN_INSTALL_DIR\\apply_env.bat${RESET}"

    # Also update current session for Git Bash
    if $IS_GITBASH; then
        SHELL_CONFIG="$HOME/.bashrc"
        if ! grep -q "export PATH=.*$INSTALL_DIR" "$SHELL_CONFIG" 2>/dev/null; then
            echo -e "\n# Added by DH-C installer" >>"$SHELL_CONFIG"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >>"$SHELL_CONFIG"
            echo "export DH_HOME=\"$SOURCE_PATH/dh\"" >>"$SHELL_CONFIG"
        fi
    fi
else
    # Unix-like systems
    if [[ "$SHELL" == *"bash"* ]]; then
        SHELL_CONFIG="$HOME/.bashrc"
    elif [[ "$SHELL" == *"zsh"* ]]; then
        SHELL_CONFIG="$HOME/.zshrc"
    else
        echo -e "${YELLOW}Unsupported shell. Please add the following to your shell configuration file manually:${RESET}"
        echo -e "export PATH=\"$INSTALL_DIR:\$PATH\""
        echo -e "export DH_HOME=\"$SOURCE_PATH/dh\""
        SHELL_CONFIG=""
    fi

    if [ -n "$SHELL_CONFIG" ]; then
        if ! grep -q "export PATH=.*$INSTALL_DIR" "$SHELL_CONFIG" 2>/dev/null; then
            echo -e "\n# Added by DH-C installer" >>"$SHELL_CONFIG"
            echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >>"$SHELL_CONFIG"
            echo "export DH_HOME=\"$SOURCE_PATH/dh\"" >>"$SHELL_CONFIG"
        fi

        # For macOS, create a LaunchAgent for persistent environment variables
        if [[ "$OSTYPE" == "darwin"* ]]; then
            echo -e "${YELLOW}Setting up macOS LaunchAgent for persistent environment variables...${RESET}"
            LAUNCH_AGENT_DIR="$HOME/Library/LaunchAgents"
            mkdir -p "$LAUNCH_AGENT_DIR"
            PLIST_FILE="$LAUNCH_AGENT_DIR/com.dh-c.environment.plist"

            cat >"$PLIST_FILE" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
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
        <string>$SOURCE_PATH/dh</string>
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

            # Create a wrapper script to ensure environment variables are set
            WRAPPER_SCRIPT="$INSTALL_DIR/dh-c-wrapper.sh"
            cat >"$WRAPPER_SCRIPT" <<EOF
#!/bin/bash

# Set the environment variable explicitly
export DH_HOME="$SOURCE_PATH/dh"

# Run the actual binary with all arguments passed to this script
exec $INSTALL_DIR/dh-c "\$@"
EOF
            chmod +x "$WRAPPER_SCRIPT"

            # Add alias to shell config
            if ! grep -q "alias dh-c=" "$SHELL_CONFIG" 2>/dev/null; then
                echo -e "\n# Create an alias to ensure DH_HOME is set correctly" >>"$SHELL_CONFIG"
                echo "alias dh-c=\"$WRAPPER_SCRIPT\"" >>"$SHELL_CONFIG"
            fi

            # Load the LaunchAgent
            launchctl unload "$PLIST_FILE" 2>/dev/null || true
            launchctl load "$PLIST_FILE"
            echo -e "${GREEN}Created and loaded LaunchAgent for environment variables${RESET}"
            echo -e "${GREEN}Created wrapper script to ensure environment variables are set${RESET}"
        fi
    fi
fi

# Final instructions
echo -e "\n${CYAN}DH-C installation complete!${RESET}"
echo -e "${CYAN}You can now use dh-c from any directory like this:${RESET}"
echo -e "  dh-c --version"
echo -e "  dh-c workspace my-workspace"
echo -e "  dh-c project my-project"

if $IS_WINDOWS; then
    echo -e "\n${YELLOW}Important: To complete the Windows installation:${RESET}"
    echo -e "1. Run the batch file as administrator: ${CYAN}$WIN_INSTALL_DIR\\apply_env.bat${RESET}"
    echo -e "2. Log out and log back in, or restart your computer"
    if $IS_GITBASH; then
        echo -e "3. For Git Bash, also run: ${CYAN}source $SHELL_CONFIG${RESET}"
    fi
elif [ -n "$SHELL_CONFIG" ]; then
    echo -e "\n${YELLOW}To start using dh-c immediately, run:${RESET}"
    echo -e "  source $SHELL_CONFIG"

    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo -e "\n${GREEN}Environment variables have been set up with a LaunchAgent.${RESET}"
        echo -e "${GREEN}They will be available in all new terminal sessions.${RESET}"
    fi
fi
