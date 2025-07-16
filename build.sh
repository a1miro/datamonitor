#!/bin/bash

# Build script for DataMonitor - supports macOS, iOS device, and iOS simulator

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_usage() {
    echo "Usage: $0 [macos|ios-device|ios-simulator|clean] [Debug|Release]"
    echo ""
    echo "Platforms:"
    echo "  macos         - Build for macOS (universal binary)"
    echo "  ios-device    - Build for iOS device (arm64)"
    echo "  ios-simulator - Build for iOS simulator (x86_64 + arm64)"
    echo "  clean         - Clean all build directories"
    echo ""
    echo "Build types:"
    echo "  Debug         - Debug build (default)"
    echo "  Release       - Release build"
    echo ""
    echo "Examples:"
    echo "  $0 macos"
    echo "  $0 ios-device Release"
    echo "  $0 clean"
}

clean_builds() {
    echo -e "${YELLOW}Cleaning build directories...${NC}"
    rm -rf "${BUILD_DIR}"
    echo -e "${GREEN}Build directories cleaned.${NC}"
}

build_macos() {
    local build_type=${1:-Debug}
    local build_dir="${BUILD_DIR}/macos-${build_type,,}"
    
    echo -e "${YELLOW}Building for macOS (${build_type})...${NC}"
    
    cmake -B "${build_dir}" -S "${PROJECT_DIR}" \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    cmake --build "${build_dir}" --parallel 8
    
    echo -e "${GREEN}macOS build completed: ${build_dir}/datamonitor.app${NC}"
}

build_ios_device() {
    local build_type=${1:-Debug}
    local build_dir="${BUILD_DIR}/ios-device-${build_type,,}"
    
    echo -e "${YELLOW}Building for iOS device (${build_type})...${NC}"
    
    cmake -B "${build_dir}" -S "${PROJECT_DIR}" \
        -DCMAKE_SYSTEM_NAME=iOS \
        -DCMAKE_OSX_ARCHITECTURES=arm64 \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -G Xcode
    
    cmake --build "${build_dir}" --config "${build_type}" --parallel 8
    
    echo -e "${GREEN}iOS device build completed: ${build_dir}/${build_type}-iphoneos/datamonitor.app${NC}"
}

build_ios_simulator() {
    local build_type=${1:-Debug}
    local build_dir="${BUILD_DIR}/ios-simulator-${build_type,,}"
    
    echo -e "${YELLOW}Building for iOS simulator (${build_type})...${NC}"
    
    cmake -B "${build_dir}" -S "${PROJECT_DIR}" \
        -DCMAKE_SYSTEM_NAME=iOS \
        -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
        -DCMAKE_OSX_SYSROOT=iphonesimulator \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -G Xcode
    
    cmake --build "${build_dir}" --config "${build_type}" --parallel 8
    
    echo -e "${GREEN}iOS simulator build completed: ${build_dir}/${build_type}-iphonesimulator/datamonitor.app${NC}"
}

# Check if Qt6 is available
if ! command -v qmake &> /dev/null; then
    echo -e "${RED}Error: Qt6 not found. Please install Qt6 and ensure it's in your PATH.${NC}"
    exit 1
fi

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo -e "${RED}Error: This script requires macOS to build iOS applications.${NC}"
    exit 1
fi

# Parse command line arguments
PLATFORM=${1:-}
BUILD_TYPE=${2:-Debug}

case "${PLATFORM}" in
    "macos")
        build_macos "${BUILD_TYPE}"
        ;;
    "ios-device")
        build_ios_device "${BUILD_TYPE}"
        ;;
    "ios-simulator")
        build_ios_simulator "${BUILD_TYPE}"
        ;;
    "clean")
        clean_builds
        ;;
    "")
        echo -e "${RED}Error: No platform specified.${NC}"
        print_usage
        exit 1
        ;;
    *)
        echo -e "${RED}Error: Unknown platform '${PLATFORM}'.${NC}"
        print_usage
        exit 1
        ;;
esac
