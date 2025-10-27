#!/bin/bash

# Five Nights at Freddy's - Quick Start Script
# This script checks dependencies and builds the game

set -e

echo "╔════════════════════════════════════════════╗"
echo "║  FNAF - Zig Edition Build Script          ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Check for Zig
if ! command -v zig &> /dev/null; then
    echo "❌ Zig compiler not found!"
    echo ""
    echo "Install Zig from: https://ziglang.org/download/"
    echo ""
    echo "Quick install (Linux):"
    echo "  wget https://ziglang.org/download/0.13.0/zig-linux-x86_64-0.13.0.tar.xz"
    echo "  tar -xf zig-linux-x86_64-0.13.0.tar.xz"
    echo "  export PATH=\$PATH:\$PWD/zig-linux-x86_64-0.13.0"
    exit 1
fi

echo "✓ Zig compiler found: $(zig version)"

# Check for SDL2
if ! pkg-config --exists sdl2; then
    echo "❌ SDL2 not found!"
    echo ""
    echo "Install SDL2:"
    echo "  Ubuntu/Debian: sudo apt install libsdl2-dev libsdl2-ttf-dev"
    echo "  Fedora:        sudo dnf install SDL2-devel SDL2_ttf-devel"
    echo "  Arch:          sudo pacman -S sdl2 sdl2_ttf"
    echo "  macOS:         brew install sdl2 sdl2_ttf"
    exit 1
fi

echo "✓ SDL2 found: $(pkg-config --modversion sdl2)"

# Check for SDL2_ttf
if ! pkg-config --exists SDL2_ttf; then
    echo "⚠️  SDL2_ttf not found, text rendering may fail"
    echo "   Install: libsdl2-ttf-dev (Debian) or SDL2_ttf-devel (Fedora)"
fi

# Build
echo ""
echo "Building game..."
zig build -Doptimize=ReleaseFast

echo ""
echo "╔════════════════════════════════════════════╗"
echo "║  Build Complete!                           ║"
echo "╚════════════════════════════════════════════╝"
echo ""
echo "Run the game:"
echo "  ./zig-out/bin/fnaf          # Start Night 1"
echo "  ./zig-out/bin/fnaf 5        # Start Night 5"
echo ""
echo "Or use:"
echo "  zig build run               # Build and run"
echo "  zig build run -- 3          # Build and run Night 3"
echo ""
