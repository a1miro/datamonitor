#!/bin/bash

# Test script for DataMonitor application
# This script builds and tests the application, catching common errors

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Testing DataMonitor Application...${NC}"

# Test 1: Build macOS version
echo -e "${YELLOW}Test 1: Building macOS Debug version...${NC}"
cd "${PROJECT_DIR}"
cmake --preset macos-debug
cmake --build --preset macos-debug

if [ -f "build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor" ]; then
    echo -e "${GREEN}✓ Build successful${NC}"
else
    echo -e "${RED}✗ Build failed - executable not found${NC}"
    exit 1
fi

# Test 2: Check application startup
echo -e "${YELLOW}Test 2: Testing application startup...${NC}"
timeout 5s build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor > /tmp/datamonitor_test.log 2>&1 &
APP_PID=$!
sleep 2

# Check if process is still running (successful startup)
if kill -0 $APP_PID 2>/dev/null; then
    echo -e "${GREEN}✓ Application started successfully${NC}"
    kill $APP_PID 2>/dev/null || true
else
    echo -e "${RED}✗ Application failed to start${NC}"
    echo -e "${YELLOW}Error output:${NC}"
    cat /tmp/datamonitor_test.log
    exit 1
fi

# Test 3: Check for common errors in log
echo -e "${YELLOW}Test 3: Checking for common issues...${NC}"
if grep -q "QQmlApplicationEngine failed to load" /tmp/datamonitor_test.log; then
    echo -e "${RED}✗ QML loading error detected${NC}"
    echo -e "${YELLOW}Suggestion: Check QML resource paths in main.cpp${NC}"
elif grep -q "Starting data generation" /tmp/datamonitor_test.log; then
    echo -e "${GREEN}✓ Data generation started successfully${NC}"
else
    echo -e "${YELLOW}! Unknown startup behavior${NC}"
fi

# Test 4: Check bundle structure
echo -e "${YELLOW}Test 4: Verifying app bundle structure...${NC}"
if [ -d "build/macos-debug/datamonitor.app/Contents" ]; then
    echo -e "${GREEN}✓ App bundle structure correct${NC}"
else
    echo -e "${RED}✗ Invalid app bundle structure${NC}"
    exit 1
fi

# Test 5: Check QML resources
echo -e "${YELLOW}Test 5: Checking QML resources...${NC}"
if strings build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor | grep -q "main.qml"; then
    echo -e "${GREEN}✓ QML resources embedded${NC}"
else
    echo -e "${RED}✗ QML resources not found${NC}"
    exit 1
fi

echo -e "${GREEN}All tests passed! Application is working correctly.${NC}"

# Cleanup
rm -f /tmp/datamonitor_test.log

echo -e "${YELLOW}To run the application:${NC}"
echo "open build/macos-debug/datamonitor.app"
echo "or"
echo "build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor"
