#Requires -Version 5.0

<#
.SYNOPSIS
    DH-C build tool installation script.

.DESCRIPTION
    Installs the DH-C build tool by compiling the source code and setting up necessary environment variables.

.PARAMETER Force
    Forces installation even if the tool is already installed.

.PARAMETER SkipEnvSetup
    Skips the environment variable setup steps.

.EXAMPLE
    .\install-dh-c.ps1

.EXAMPLE
    .\install-dh-c.ps1 -Force
#>

[CmdletBinding()]
param (
    [switch]$Force,
    [switch]$SkipEnvSetup
)

# Define output colors
$Script:Colors = @{
    Info    = [ConsoleColor]::Cyan
    Success = [ConsoleColor]::Green
    Warning = [ConsoleColor]::Yellow
    Error   = [ConsoleColor]::Red
}

# Global variables
$Script:InstallDir = Join-Path $env:USERPROFILE ".dh-c"
$Script:SourcePath = $PSScriptRoot

function Write-ColorMessage {
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [string]$Message,

        [Parameter(Position = 1)]
        [ConsoleColor]$ForegroundColor = $Colors.Info
    )

    Write-Host $Message -ForegroundColor $ForegroundColor
}

function Test-AdminPrivileges {
    $isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

    if (-not $isAdmin) {
        Write-ColorMessage "Warning: Not running with administrator privileges. Some operations may fail." -ForegroundColor $Colors.Warning
    }

    return $isAdmin
}

function Test-ClangInstallation {
    Write-ColorMessage "Checking for clang..." -ForegroundColor $Colors.Info

    if (-not (Get-Command "clang" -ErrorAction SilentlyContinue)) {
        Write-ColorMessage "Error: clang compiler not found. Please install clang first." -ForegroundColor $Colors.Error
        Write-ColorMessage "Download from: https://releases.llvm.org/" -ForegroundColor $Colors.Error
        return $false
    }

    $clangVersion = (clang --version) | Select-Object -First 1
    Write-ColorMessage "Found clang: $clangVersion" -ForegroundColor $Colors.Success
    return $true
}

function Initialize-InstallDirectories {
    Write-ColorMessage "Creating installation directory: $InstallDir" -ForegroundColor $Colors.Info

    if (-not (Test-Path $InstallDir)) {
        New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null
    }

    $dhSourcePath = Join-Path $SourcePath "dh"
    if (-not (Test-Path $dhSourcePath)) {
        New-Item -ItemType Directory -Path $dhSourcePath -Force | Out-Null
    }

    return $true
}

function Invoke-Compilation {
    Write-ColorMessage "Compiling dh-c..." -ForegroundColor $Colors.Info

    $sourceFile = Join-Path $SourcePath "dh-c.c"
    $outputFile = Join-Path $InstallDir "dh-c.exe"

    if (-not (Test-Path $sourceFile)) {
        Write-ColorMessage "Error: Could not find dh-c.c in the current directory." -ForegroundColor $Colors.Error
        return $false
    }

    try {
        clang -std=c17 -Wall -Wextra -O3 -o "$outputFile" "$sourceFile"

        if (-not (Test-Path $outputFile)) {
            Write-ColorMessage "Error: Compilation failed." -ForegroundColor $Colors.Error
            return $false
        }

        Write-ColorMessage "DH-C compiled successfully!" -ForegroundColor $Colors.Success
        return $true
    }
    catch {
        Write-ColorMessage "Error during compilation: $_" -ForegroundColor $Colors.Error
        return $false
    }
}

function Set-EnvironmentVariables {
    if ($SkipEnvSetup) {
        Write-ColorMessage "Skipping environment variable setup as requested." -ForegroundColor $Colors.Warning
        return $true
    }

    Write-ColorMessage "Setting up environment variables..." -ForegroundColor $Colors.Info

    $dhHomePath = Join-Path $SourcePath "dh"

    try {
        # Update user PATH environment variable
        $userPath = [Environment]::GetEnvironmentVariable("Path", "User")
        if (-not $userPath.Contains($InstallDir)) {
            [Environment]::SetEnvironmentVariable("Path", "$InstallDir;$userPath", "User")
            Write-ColorMessage "Added $InstallDir to user PATH" -ForegroundColor $Colors.Success
        }

        # Set DH_HOME environment variable
        [Environment]::SetEnvironmentVariable("DH_HOME", $dhHomePath, "User")
        Write-ColorMessage "Set DH_HOME environment variable to: $dhHomePath" -ForegroundColor $Colors.Success

        return $true
    }
    catch {
        Write-ColorMessage "Error setting environment variables: $_" -ForegroundColor $Colors.Error
        return $false
    }
}

function New-WrapperScripts {
    Write-ColorMessage "Creating helper scripts..." -ForegroundColor $Colors.Info

    $dhHomePath = Join-Path $SourcePath "dh"

    # Create a wrapper batch file
    $wrapperFile = Join-Path $InstallDir "dh-c-wrapper.bat"
    @"
@echo off
set DH_HOME=$dhHomePath
"%~dp0dh-c.exe" %*
"@ | Out-File -FilePath $wrapperFile -Encoding ASCII

    # Create Registry file for environment variables
    $regFile = Join-Path $InstallDir "dh_env.reg"
    $installDirEscaped = $InstallDir -replace '\\', '\\'
    $dhHomePathEscaped = $dhHomePath -replace '\\', '\\'

    @"
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Environment]
"PATH"="$installDirEscaped;%PATH%"
"DH_HOME"="$dhHomePathEscaped"
"@ | Out-File -FilePath $regFile -Encoding ASCII

    # Create a batch file to apply the registry changes
    $batFile = Join-Path $InstallDir "apply_env.bat"
    @"
@echo off
echo Updating environment variables...
reg import "%~dp0dh_env.reg"
if errorlevel 1 (
    echo Failed to update environment variables!
    pause
    exit /b 1
)
echo Environment variables updated successfully!
echo DH_HOME set to: $dhHomePath
echo Added to PATH: $InstallDir
pause
"@ | Out-File -FilePath $batFile -Encoding ASCII

    return $true
}

function Show-Instructions {
    # Format batch file path for display
    $batFile = Join-Path $InstallDir "apply_env.bat" -Resolve -ErrorAction SilentlyContinue
    if (-not $batFile) {
        $batFile = "$InstallDir\apply_env.bat"
    }

    Write-ColorMessage "`nDH-C installation complete!" -ForegroundColor $Colors.Success
    Write-ColorMessage "You can now use dh-c from any directory like this:" -ForegroundColor $Colors.Info
    Write-ColorMessage "  dh-c --version"
    Write-ColorMessage "  dh-c workspace my-workspace"
    Write-ColorMessage "  dh-c project my-project"

    Write-ColorMessage "`nImportant: To complete the Windows installation:" -ForegroundColor $Colors.Warning
    Write-ColorMessage "1. Run the batch file as administrator: $batFile" -ForegroundColor $Colors.Info
    Write-ColorMessage "2. Log out and log back in, or restart your computer" -ForegroundColor $Colors.Info
    Write-ColorMessage "3. Alternatively, restart your terminal to use dh-c in the current session" -ForegroundColor $Colors.Info
}

function Install-Dh-C {
    Write-ColorMessage "DH-C Installation Script" -ForegroundColor $Colors.Info
    Write-ColorMessage "-----------------------" -ForegroundColor $Colors.Info

    Write-ColorMessage "Detected Windows environment" -ForegroundColor $Colors.Warning

    # Main installation flow
    $null = Test-AdminPrivileges

    if (-not (Test-ClangInstallation)) { return }

    if (-not (Initialize-InstallDirectories)) { return }

    if (-not (Invoke-Compilation)) { return }

    if (-not (Set-EnvironmentVariables)) {
        # Continue with installation even if env vars fail
        Write-ColorMessage "Continuing installation despite environment variable setup issues..." -ForegroundColor $Colors.Warning
    }

    if (-not (New-WrapperScripts)) {
        Write-ColorMessage "Warning: Failed to create some helper scripts." -ForegroundColor $Colors.Warning
    }

    Show-Instructions
}

# Start the installation process
Install-Dh-C
