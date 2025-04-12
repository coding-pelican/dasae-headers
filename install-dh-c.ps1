# PowerShell script to install DH-C build tool

# Check if running with admin privileges
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "Warning: Not running with administrator privileges. Some operations may fail." -ForegroundColor Yellow
}

# Check for clang compiler
if (-not (Get-Command "clang" -ErrorAction SilentlyContinue)) {
    Write-Host "Error: clang compiler not found. Please install clang first." -ForegroundColor Red
    exit 1
}

# Get the DH library path
$dhPath = $PSScriptRoot
$binPath = Join-Path $dhPath "bin"

# Create bin directory if it doesn't exist
if (-not (Test-Path $binPath)) {
    New-Item -ItemType Directory -Path $binPath | Out-Null
    Write-Host "Created bin directory: $binPath"
}

# Compile dh-c
$sourcePath = Join-Path $dhPath "dh-c.c"
$outputPath = Join-Path $binPath "dh-c.exe"

Write-Host "Compiling dh-c build tool..."
clang -std=c17 -Wall -Wextra -O3 -o "$outputPath" "$sourcePath" -static

if (-not $?) {
    Write-Host "Error: Failed to compile dh-c" -ForegroundColor Red
    exit 1
}

# Update PATH environment variable
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if (-not $userPath.Contains($binPath)) {
    [Environment]::SetEnvironmentVariable("Path", "$userPath;$binPath", "User")
    Write-Host "Added $binPath to user PATH"
}

# Set DH_HOME environment variable
[Environment]::SetEnvironmentVariable("DH_HOME", $dhPath, "User")
Write-Host "Set DH_HOME environment variable to: $dhPath"

Write-Host "`nDH-C build tool installed successfully!" -ForegroundColor Green
Write-Host "Please restart your terminal for the changes to take effect."
Write-Host "`nUsage examples:"
Write-Host "  dh-c --version"
Write-Host "  dh-c workspace myworkspace"
Write-Host "  dh-c project myproject"
