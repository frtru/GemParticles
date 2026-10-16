#!/bin/bash

set -e

echo "Running post-create setup for GemParticles DevContainer..."

# Ensure we're in the project root
cd /app

# Check for existing build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

# Install conan dependencies (deps in Release, GemParticles in Debug)
echo "Installing dependencies with Conan..."
scripts/bootstrap.sh Debug

# Configure CMake with debug symbols (the preset also exports compile_commands.json)
echo "Configuring CMake in Debug mode with debug symbols..."
cmake --preset conan-debug

# Create .ccls file for language server
echo "Creating .ccls configuration for the language server..."
cat > .ccls << EOF
%compile_commands.json
%c -std=c11
%cpp -std=c++17
-I/app/src
-I/app/src/utils
-I/app/build/_deps
-I/root/.conan2/p/b/*/p/include
EOF

# Link compile_commands.json to project root for language servers
if [ -f "build/Debug/compile_commands.json" ]; then
    ln -sf "build/Debug/compile_commands.json" "compile_commands.json"
fi

# Attempt to build the project
echo "Building the project..."
cmake --build --preset conan-debug -j$(nproc) || echo "Build completed with some warnings or errors"

echo "DevContainer setup complete." 