#!/bin/bash

# Script to update ImGui backend implementation files

set -e

echo "==== ImGui Backend Updater ===="
echo

# Get the ImGui version from conanfile.txt (now need to go up 3 levels to find it)
IMGUI_VERSION=$(grep -oP 'imgui/\K[^@]*' ../../../conanfile.txt)

# Remove any trailing characters that might be causing issues
IMGUI_VERSION=$(echo "$IMGUI_VERSION" | tr -d '\r\n\t ')

if [ -z "$IMGUI_VERSION" ]; then
    echo "ERROR: Could not find ImGui version in conanfile.txt"
    exit 1
fi

echo "Detected ImGui version: $IMGUI_VERSION"
echo

# Create a temporary directory
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Target directory is now the current directory
TARGET_DIR="."
mkdir -p "$TARGET_DIR"

echo "Cloning ImGui repository..."
# Format the tag version
TAG_VERSION="v$IMGUI_VERSION"
echo "Using tag: $TAG_VERSION"

# Try with a direct clone
git clone --depth 1 --branch "$TAG_VERSION" https://github.com/ocornut/imgui.git "$TEMP_DIR/imgui"

if [ ! -d "$TEMP_DIR/imgui/.git" ]; then
    echo "ERROR: Failed to clone ImGui repository with tag $TAG_VERSION"
    echo "Trying to clone the repository without a specific tag..."
    git clone --depth 1 https://github.com/ocornut/imgui.git "$TEMP_DIR/imgui"
    
    if [ ! -d "$TEMP_DIR/imgui/.git" ]; then
        echo "ERROR: Failed to clone ImGui repository"
        exit 1
    fi
    
    # List available tags
    echo "Available tags:"
    git -C "$TEMP_DIR/imgui" tag | grep "$IMGUI_VERSION"
    
    # Try to checkout the correct version
    git -C "$TEMP_DIR/imgui" checkout "$TAG_VERSION" || 
    git -C "$TEMP_DIR/imgui" checkout "$TAG_VERSION^{}" ||
    git -C "$TEMP_DIR/imgui" checkout "tags/$TAG_VERSION" ||
    git -C "$TEMP_DIR/imgui" checkout "tags/$TAG_VERSION^{}" ||
    echo "WARNING: Could not checkout specific version $TAG_VERSION"
fi

echo "Successfully cloned ImGui repository"
echo

echo "Copying core files..."
# Core files
cp "$TEMP_DIR/imgui/imgui.h" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/imconfig.h" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/imgui_internal.h" "$TARGET_DIR/"

echo "Copying backend implementation files..."
# Backend files
cp "$TEMP_DIR/imgui/backends/imgui_impl_glfw.cpp" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/backends/imgui_impl_glfw.h" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/backends/imgui_impl_opengl3.cpp" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/backends/imgui_impl_opengl3.h" "$TARGET_DIR/"
cp "$TEMP_DIR/imgui/backends/imgui_impl_opengl3_loader.h" "$TARGET_DIR/"

echo "Copying files to match project's naming convention..."
# Rename files to match the project's naming convention
cp "$TARGET_DIR/imgui_impl_glfw.cpp" "$TARGET_DIR/imgui_glfw.cpp"
cp "$TARGET_DIR/imgui_impl_glfw.h" "$TARGET_DIR/imgui_glfw.h"

# Display summary
echo
echo "SUMMARY:"
echo "--------"
echo "ImGui version: $IMGUI_VERSION (tag: $TAG_VERSION)"
echo "Files updated in: $TARGET_DIR"
ls -la "$TARGET_DIR" | grep -v "extra_fonts" | grep -v "^d"
echo

echo "ImGui backend files have been successfully updated to version $IMGUI_VERSION!"
 