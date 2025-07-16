# DataMonitor Cross-Platform Build Examples

## Quick Start Examples

### 1. Build for macOS (Debug)
```bash
./build.sh macos
```
Output: `build/macos-debug/datamonitor.app`

### 2. Build for iOS Device (Release)
```bash
./build.sh ios-device Release
```
Output: `build/ios-device-release/Release-iphoneos/datamonitor.app`

### 3. Build for iOS Simulator
```bash
./build.sh ios-simulator
```
Output: `build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app`

### 4. Build All Platforms
```bash
./build.sh macos Debug
./build.sh macos Release
./build.sh ios-device Debug
./build.sh ios-device Release
./build.sh ios-simulator Debug
./build.sh ios-simulator Release
```

### 5. Clean Everything
```bash
./build.sh clean
```

## Running the Built Applications

### macOS
```bash
# Run directly
open build/macos-debug/datamonitor.app

# Or from command line
build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor
```

### iOS Simulator
```bash
# Install to simulator
xcrun simctl install booted build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app

# Launch
xcrun simctl launch booted com.example.datamonitor
```

### iOS Device
For device deployment, open the generated Xcode project:
```bash
open build/ios-device-debug/DataMonitor.xcodeproj
```

Then configure code signing and deploy from Xcode.

## Development Workflow

### For Active Development (macOS)
```bash
# Quick build and run
./build.sh macos && open build/macos-debug/datamonitor.app
```

### For iOS Testing
```bash
# Build for simulator and install
./build.sh ios-simulator
xcrun simctl install booted build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app
xcrun simctl launch booted com.example.datamonitor
```

### For Release Builds
```bash
# Build release versions for distribution
./build.sh macos Release
./build.sh ios-device Release
```

## VS Code Integration

Use Cmd+Shift+P and search for "Tasks: Run Task" then select:
- `Build macOS Universal` - Build universal macOS app
- `Build iOS Device` - Build for iOS device
- `Build iOS Simulator` - Build for iOS simulator
- `Clean All Builds` - Clean all build directories

## Continuous Integration Example

```bash
#!/bin/bash
# CI script example

set -e

echo "Building DataMonitor for all platforms..."

# Clean previous builds
./build.sh clean

# Build macOS
echo "Building macOS..."
./build.sh macos Release

# Build iOS
echo "Building iOS device..."
./build.sh ios-device Release

echo "Building iOS simulator..."
./build.sh ios-simulator Release

echo "All builds completed successfully!"

# Archive builds (example)
tar -czf datamonitor-macos.tar.gz -C build/macos-release datamonitor.app
tar -czf datamonitor-ios-device.tar.gz -C build/ios-device-release/Release-iphoneos datamonitor.app
tar -czf datamonitor-ios-simulator.tar.gz -C build/ios-simulator-release/Release-iphonesimulator datamonitor.app

echo "Archives created."
```
