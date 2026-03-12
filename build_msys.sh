#!/bin/bash
# Build script for UKF Highway with GUI using MSYS
# This script builds the project with PCL visualization support

set -e

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "========================================"
echo "  Building UKF Highway with GUI (MSYS)"
echo "========================================"
echo ""

# Remove old build directory if exists
if [ -d "build" ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi

# Create fresh build directory
mkdir -p build
cd build

echo "Running CMake..."
# Configure with CMake - try to find PCL
cmake .. -G "Unix Makefiles" \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_BUILD_TYPE=Release

echo ""
echo "Building project..."
# Compile the project
make -j$(nproc)

echo ""
echo "========================================"
echo "  Build Complete!"
echo "========================================"
echo ""

# Check if GUI executable was built
if [ -f "ukf_highway_gui" ]; then
    echo "GUI executable built successfully!"
    echo ""
    echo "To run the GUI version, execute:"
    echo "  ./build/ukf_highway_gui"
    echo ""
    read -p "Would you like to run the GUI now? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        ./ukf_highway_gui
    fi
else
    echo "Note: GUI executable was not built (PCL may not be available)"
    echo ""
    if [ -f "ukf_highway" ]; then
        echo "Running headless version instead..."
        echo "To run manually: ./build/ukf_highway"
        read -p "Would you like to run the headless version? (y/n) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            ./ukf_highway
        fi
    fi
fi

