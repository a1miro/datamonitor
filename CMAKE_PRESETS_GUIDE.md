# CMake Presets Quick Reference

## Available Presets

```bash
# List all available presets
cmake --list-presets
```

**Configure Presets:**
- `macos-debug` - macOS Debug (Universal Binary)
- `macos-release` - macOS Release (Universal Binary)
- `ios-device-debug` - iOS Device Debug (arm64)
- `ios-device-release` - iOS Device Release (arm64)
- `ios-simulator-debug` - iOS Simulator Debug (x86_64 + arm64)
- `ios-simulator-release` - iOS Simulator Release (x86_64 + arm64)

## Quick Start Commands

### macOS Development
```bash
# Configure and build debug
cmake --preset macos-debug
cmake --build --preset macos-debug

# Run the app
open build/macos-debug/datamonitor.app

# One-liner for quick iteration
cmake --preset macos-debug && cmake --build --preset macos-debug && open build/macos-debug/datamonitor.app
```

### iOS Device
```bash
# Configure and build debug
cmake --preset ios-device-debug
cmake --build --preset ios-device-debug

# Open in Xcode for deployment
open build/ios-device-debug/DataMonitor.xcodeproj
```

### iOS Simulator
```bash
# Configure and build debug
cmake --preset ios-simulator-debug
cmake --build --preset ios-simulator-debug

# Install to simulator
xcrun simctl install booted build/ios-simulator-debug/Debug-iphonesimulator/datamonitor.app
```

### Release Builds
```bash
# macOS Release
cmake --preset macos-release && cmake --build --preset macos-release

# iOS Device Release
cmake --preset ios-device-release && cmake --build --preset ios-device-release

# iOS Simulator Release
cmake --preset ios-simulator-release && cmake --build --preset ios-simulator-release
```

## VS Code Integration

### Using Command Palette
1. Press `Cmd+Shift+P`
2. Type "Tasks: Run Task"
3. Select desired task:
   - `Build macOS Debug` (default)
   - `Build macOS Release`
   - `Build iOS Device Debug`
   - `Build iOS Simulator Debug`
   - etc.

### Using CMake Tools Extension
1. Install "CMake Tools" extension
2. Status bar will show current preset
3. Click to change preset
4. Use build/debug buttons in status bar

## Build Output Locations

```
build/
├── macos-debug/datamonitor.app
├── macos-release/datamonitor.app
├── ios-device-debug/Debug-iphoneos/datamonitor.app
├── ios-device-release/Release-iphoneos/datamonitor.app
├── ios-simulator-debug/Debug-iphonesimulator/datamonitor.app
└── ios-simulator-release/Release-iphonesimulator/datamonitor.app
```

## Useful Aliases

Add these to your shell profile for faster access:

```bash
# ~/.bashrc or ~/.zshrc
alias qb-mac="cmake --preset macos-debug && cmake --build --preset macos-debug"
alias qr-mac="qb-mac && open build/macos-debug/datamonitor.app"
alias qb-ios="cmake --preset ios-device-debug && cmake --build --preset ios-device-debug"
alias qb-sim="cmake --preset ios-simulator-debug && cmake --build --preset ios-simulator-debug"
alias qclean="rm -rf build"
```

## CI/CD Usage

```yaml
# GitHub Actions example
- name: Build macOS
  run: |
    cmake --preset macos-release
    cmake --build --preset macos-release

- name: Build iOS
  run: |
    cmake --preset ios-device-release
    cmake --build --preset ios-device-release
```

## Troubleshooting

### Preset Not Found
```bash
# Ensure you're in the project root
cd /path/to/datamonitor

# Check CMake version (need 3.19+)
cmake --version

# Verify CMakePresets.json exists
ls -la CMakePresets.json
```

### Build Errors
```bash
# Clean and rebuild
rm -rf build
cmake --preset macos-debug
cmake --build --preset macos-debug
```

### iOS Code Signing
```bash
# Open Xcode project to configure signing
open build/ios-device-debug/DataMonitor.xcodeproj
```

### Runtime Errors

#### QML Resource Loading Error
**Error:** `QQmlApplicationEngine failed to load component qrc:/DataMonitor/qml/main.qml: No such file or directory`

**Solution:** This is due to Qt6's new QML resource system. The path in main.cpp should be:
```cpp
const QUrl url(QStringLiteral("qrc:/qt/qml/DataMonitor/qml/main.qml"));
```

#### Common Runtime Warnings (Can be ignored)
**Warning:** `QML QQuickRectangle: The current style does not support customization...`
- This is a style customization warning, application will work fine

**Warning:** `Line widths other than 1 are not supported by the graphics API`
- OpenGL limitation warning, plotting will still work

#### Application Won't Start on macOS
```bash
# Check if the app bundle was created
ls -la build/macos-debug/datamonitor.app

# Run from command line to see errors
build/macos-debug/datamonitor.app/Contents/MacOS/datamonitor

# Check Qt installation
qmake --version
```

#### QML Import Errors
If you see QML import errors, ensure all Qt modules are properly linked:
```cmake
# In CMakeLists.txt, make sure you have:
find_package(Qt6 REQUIRED COMPONENTS Core Quick)
target_link_libraries(datamonitor PRIVATE Qt6::Core Qt6::Quick)
```
