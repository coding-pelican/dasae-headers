# PowerShell script to install DH-C build tool

# Define colors
$CYAN = [ConsoleColor]::Cyan
$GREEN = [ConsoleColor]::Green
$YELLOW = [ConsoleColor]::Yellow
$RED = [ConsoleColor]::Red

Write-Host "DH-C Installation Script" -ForegroundColor $CYAN
Write-Host "-----------------------" -ForegroundColor $CYAN

# Detect OS
Write-Host "Detected Windows environment" -ForegroundColor $YELLOW

# Check if running with admin privileges
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "Warning: Not running with administrator privileges. Some operations may fail." -ForegroundColor $YELLOW
}

# Check for clang compiler
Write-Host "Checking for clang..." -ForegroundColor $YELLOW
if (-not (Get-Command "clang" -ErrorAction SilentlyContinue)) {
    Write-Host "Error: clang compiler not found. Please install clang first." -ForegroundColor $RED
    Write-Host "Download from: https://releases.llvm.org/" -ForegroundColor $RED
    exit 1
}
else {
    $clangVersion = (clang --version) | Select-Object -First 1
    Write-Host "Found clang: $clangVersion" -ForegroundColor $GREEN
}

# Create installation directory
$installDir = Join-Path $env:USERPROFILE ".dh-c"
Write-Host "Creating installation directory: $installDir" -ForegroundColor $YELLOW
if (-not (Test-Path $installDir)) {
    New-Item -ItemType Directory -Path $installDir | Out-Null
}

# Get the source path
$sourcePath = $PSScriptRoot
$sourceFile = Join-Path $sourcePath "dh-c.c"

if (-not (Test-Path $sourceFile)) {
    Write-Host "Error: Could not find dh-c.c in the current directory." -ForegroundColor $RED
    exit 1
}

# Compile dh-c
Write-Host "Compiling dh-c..." -ForegroundColor $YELLOW
$outputFile = Join-Path $installDir "dh-c.exe"

clang -std=c17 -Wall -Wextra -O3 -o "$outputFile" "$sourceFile"

if (-not (Test-Path $outputFile)) {
    Write-Host "Error: Compilation failed." -ForegroundColor $RED
    exit 1
}

Write-Host "DH-C compiled successfully!" -ForegroundColor $GREEN

# Update PATH and environment variables
Write-Host "Setting up environment variables..." -ForegroundColor $YELLOW

# Convert paths to Windows format
$winInstallDir = $installDir
$winSourcePath = Join-Path $sourcePath "dh"

# Create necessary directories
if (-not (Test-Path $winSourcePath)) {
    New-Item -ItemType Directory -Path $winSourcePath -Force | Out-Null
}

# Update user PATH environment variable
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if (-not $userPath.Contains($winInstallDir)) {
    [Environment]::SetEnvironmentVariable("Path", "$winInstallDir;$userPath", "User")
    Write-Host "Added $winInstallDir to user PATH" -ForegroundColor $GREEN
}

# Set DH_HOME environment variable
[Environment]::SetEnvironmentVariable("DH_HOME", $winSourcePath, "User")
Write-Host "Set DH_HOME environment variable to: $winSourcePath" -ForegroundColor $GREEN

# Create a wrapper batch file
$wrapperFile = Join-Path $installDir "dh-c-wrapper.bat"
@"
@echo off
set DH_HOME=$winSourcePath
"%~dp0dh-c.exe" %*
"@ | Out-File -FilePath $wrapperFile -Encoding ASCII

# Create Registry file for environment variables
$regFile = Join-Path $installDir "dh_env.reg"
$winInstallDirEscaped = $winInstallDir -replace '\\', '\\'
$winSourcePathEscaped = $winSourcePath -replace '\\', '\\'

@"
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\Environment]
"PATH"="$winInstallDirEscaped;%PATH%"
"DH_HOME"="$winSourcePathEscaped"
"@ | Out-File -FilePath $regFile -Encoding ASCII

# Create a batch file to apply the registry changes
$batFile = Join-Path $installDir "apply_env.bat"
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
echo DH_HOME set to: $winSourcePath
echo Added to PATH: $winInstallDir
pause
"@ | Out-File -FilePath $batFile -Encoding ASCII

# Final instructions
Write-Host "`nDH-C installation complete!" -ForegroundColor $CYAN
Write-Host "You can now use dh-c from any directory like this:" -ForegroundColor $CYAN
Write-Host "  dh-c --version"
Write-Host "  dh-c workspace my-workspace"
Write-Host "  dh-c project my-project"

Write-Host "`nImportant: To complete the Windows installation:" -ForegroundColor $YELLOW
Write-Host "1. Run the batch file as administrator: $batFile" -ForegroundColor $CYAN
Write-Host "2. Log out and log back in, or restart your computer" -ForegroundColor $CYAN
Write-Host "3. Alternatively, restart your terminal to use dh-c in the current session" -ForegroundColor $CYAN
