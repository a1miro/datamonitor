# DataMonitor - Cross-Platform Build Guide

This document explains how to build DataMonitor for macOS and iOS platforms.

## Prerequisites

### Required Software
- **Xcode** (latest version from Mac App Store)
- **Qt6** with iOS support (install from [qt.io](https://www.qt.io/download))
- **CMake** 3.16 or later
- **macOS** 11.0 or later

### Qt6 Installation
When installing Qt6, make sure to select:
- Qt 6.6+ (or latest LTS)
- **macOS** kit
- **iOS** kit
- Qt Creator (optional but recommended)
- CMake and Ninja build tools

### Verify Installation
```bash
# Check Xcode
xcodebuild -version

# Check Qt6
qmake --version

# Check CMake
cmake --version
```

## Build Methods

### Method 1: Using Build Script (Recommended)

The project includes a convenient build script that handles all platform configurations:

```bash
# Build for macOS (universal binary)
./build.sh macos

# Build for iOS device
./build.sh ios-device

# Build for iOS simulator
./build.sh ios-simulator

# Build release versions
./build.sh macos Release
./build.sh ios-device Release

# Clean all builds
./build.sh clean
```

### Method 2: Using VS Code Tasks

If you're using VS Code, you can use the predefined tasks:

1. Open Command Palette (`Cmd+Shift+P`)
2. Type "Tasks: Run Task"
3. Select from available tasks:
   - `Build macOS Universal`
   - `Build iOS Device`
   - `Build iOS Simulator`
   - `Build iOS Device Release`
   - `Build iOS Simulator Release`
   - `Clean All Builds`

### Method 3: Manual CMake Commands

#### macOS Build
```bash
# Configure
cmake -B build/macos -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"

# Build
cmake --build build/macos --parallel 8
```

#### iOS Device Build
```bash
# Configure
cmake -B build/ios-device -S . \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_BUILD_TYPE=Release \
    -G Xcode

# Build
cmake --build build/ios-device --config Release
```

#### iOS Simulator Build
```bash
# Configure
cmake -B build/ios-simulator -S . \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_OSX_SYSROOT=iphonesimulator \
    -DCMAKE_BUILD_TYPE=Release \
    -G Xcode

# Build
cmake --build build/ios-simulator --config Release
```

## Build Output Locations

After successful builds, you'll find the applications at:

- **macOS**: `build/macos-debug/datamonitor.app` or `build/macos-release/datamonitor.app`
- **iOS Device**: `build/ios-device-debug/Debug-iphoneos/datamonitor.app`
- **iOS Simulator**: `build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app`

## iOS Deployment

### For iOS Device Deployment
1. Open the Xcode project: `build/ios-device-debug/DataMonitor.xcodeproj`
2. Configure your development team in Xcode
3. Set up code signing certificates
4. Select your connected iOS device
5. Build and run from Xcode

### For iOS Simulator
1. Open iOS Simulator
2. Run: `xcrun simctl install booted build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app`
3. Launch the app from simulator

## Troubleshooting

### Common Issues

1. **Qt6 not found**
   ```bash
   # Add Qt6 to PATH
   export PATH="/usr/local/Qt/6.6.0/macos/bin:$PATH"
   ```

2. **iOS build fails**
   - Ensure Xcode command line tools are installed: `xcode-select --install`
   - Verify iOS SDK is available: `xcrun --show-sdk-path --sdk iphoneos`

3. **Code signing issues**
   - Open the Xcode project and configure your development team
   - Update `XCODE_ATTRIBUTE_DEVELOPMENT_TEAM` in CMakeLists.txt

4. **Architecture issues**
   - For M1 Macs, ensure you're using arm64 architecture
   - Check CMake generator: use `-G Xcode` for iOS builds

### Development Team Configuration

To set your development team for iOS builds, edit `CMakeLists.txt` and update:
```cmake
XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "YOUR_TEAM_ID_HERE"
```

Find your team ID in Xcode under Preferences > Accounts > Your Apple ID > Team Details.

## Project Structure

```
datamonitor/
├── CMakeLists.txt              # Main CMake configuration
├── Info.plist.in               # macOS bundle info template
├── build.sh                    # Cross-platform build script
├── cmake/
│   └── ios.toolchain.cmake     # iOS CMake toolchain
├── src/                        # C++ source files
├── qml/                        # QML UI files
└── .vscode/
    └── tasks.json              # VS Code build tasks
```

## Platform-Specific Features

### macOS
- Universal binary (Intel + Apple Silicon)
- Native macOS bundle with proper Info.plist
- High DPI support

### iOS
- Universal iOS app (iPhone + iPad)
- iOS 15.0+ deployment target
- Proper code signing configuration
- App Store deployment ready

## Next Steps

1. **For Development**: Use the build script or VS Code tasks for quick iteration
2. **For Distribution**: Use Xcode for final code signing and App Store submission
3. **For CI/CD**: Integrate the build script into your automated build pipeline
