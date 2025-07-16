#!/bin/bash

# Script to check and fix iOS development setup

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}iOS Development Setup Checker${NC}"
echo "======================================"

# Check 1: Xcode Installation
echo -e "${YELLOW}1. Checking Xcode installation...${NC}"
if [ -d "/Applications/Xcode.app" ]; then
    echo -e "${GREEN}✓ Xcode is installed${NC}"
    XCODE_VERSION=$(defaults read /Applications/Xcode.app/Contents/Info CFBundleShortVersionString 2>/dev/null || echo "Unknown")
    echo "  Version: $XCODE_VERSION"
else
    echo -e "${RED}✗ Xcode is not installed${NC}"
    echo -e "${YELLOW}Please install Xcode from the Mac App Store${NC}"
    exit 1
fi

# Check 2: Developer Directory
echo -e "${YELLOW}2. Checking developer directory...${NC}"
CURRENT_DEV_DIR=$(xcode-select --print-path)
echo "  Current: $CURRENT_DEV_DIR"

if [ "$CURRENT_DEV_DIR" = "/Applications/Xcode.app/Contents/Developer" ]; then
    echo -e "${GREEN}✓ Developer directory is correctly set${NC}"
else
    echo -e "${RED}✗ Developer directory points to Command Line Tools${NC}"
    echo -e "${YELLOW}To fix this, run:${NC}"
    echo "  sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer"
    
    # Ask if user wants to fix it now
    read -p "Do you want to fix this now? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${YELLOW}Switching developer directory...${NC}"
        sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
        echo -e "${GREEN}✓ Developer directory updated${NC}"
    else
        echo -e "${YELLOW}Skipping fix. iOS builds will not work until this is resolved.${NC}"
        exit 1
    fi
fi

# Check 3: Xcode License
echo -e "${YELLOW}3. Checking Xcode license...${NC}"
if xcodebuild -checkFirstLaunchStatus &>/dev/null; then
    echo -e "${GREEN}✓ Xcode license accepted${NC}"
else
    echo -e "${RED}✗ Xcode license not accepted${NC}"
    echo -e "${YELLOW}To fix this, run:${NC}"
    echo "  sudo xcodebuild -license accept"
    
    read -p "Do you want to accept the license now? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${YELLOW}Accepting Xcode license...${NC}"
        sudo xcodebuild -license accept
        echo -e "${GREEN}✓ Xcode license accepted${NC}"
    else
        echo -e "${YELLOW}Skipping license acceptance. iOS builds may not work.${NC}"
    fi
fi

# Check 4: iOS SDKs
echo -e "${YELLOW}4. Checking iOS SDKs...${NC}"
if command -v xcodebuild &> /dev/null; then
    echo "Available SDKs:"
    xcodebuild -showsdks | grep -E "(iOS|iPhone)" || echo "  No iOS SDKs found"
    
    # Check specific SDKs
    if xcrun --show-sdk-path --sdk iphoneos &>/dev/null; then
        IPHONEOS_PATH=$(xcrun --show-sdk-path --sdk iphoneos)
        echo -e "${GREEN}✓ iOS Device SDK: $IPHONEOS_PATH${NC}"
    else
        echo -e "${RED}✗ iOS Device SDK not found${NC}"
    fi
    
    if xcrun --show-sdk-path --sdk iphonesimulator &>/dev/null; then
        IPHONESIM_PATH=$(xcrun --show-sdk-path --sdk iphonesimulator)
        echo -e "${GREEN}✓ iOS Simulator SDK: $IPHONESIM_PATH${NC}"
    else
        echo -e "${RED}✗ iOS Simulator SDK not found${NC}"
    fi
else
    echo -e "${RED}✗ xcodebuild not available${NC}"
fi

# Check 5: CMake Version
echo -e "${YELLOW}5. Checking CMake version...${NC}"
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    echo "  CMake version: $CMAKE_VERSION"
    
    # Check if version is >= 3.19
    if printf '%s\n' "3.19" "$CMAKE_VERSION" | sort -V | head -n1 | grep -q "3.19"; then
        echo -e "${GREEN}✓ CMake version supports presets${NC}"
    else
        echo -e "${RED}✗ CMake version too old for presets (need >= 3.19)${NC}"
        echo -e "${YELLOW}Consider updating CMake: brew upgrade cmake${NC}"
    fi
else
    echo -e "${RED}✗ CMake not found${NC}"
fi

echo ""
echo -e "${BLUE}Setup Summary${NC}"
echo "=============="

# Final recommendations
if xcrun --show-sdk-path --sdk iphoneos &>/dev/null && xcrun --show-sdk-path --sdk iphonesimulator &>/dev/null; then
    echo -e "${GREEN}✓ iOS development environment is ready!${NC}"
    echo ""
    echo "You can now use iOS presets:"
    echo "  cmake --preset ios-device-debug"
    echo "  cmake --preset ios-simulator-debug"
else
    echo -e "${YELLOW}⚠ iOS development environment needs attention${NC}"
    echo ""
    echo "For macOS-only development, use:"
    echo "  cp CMakePresets-macOS-only.json CMakePresets.json"
    echo "  cmake --preset macos-debug"
fi

echo ""
echo "To test your setup:"
echo "  ./test_app.sh"
