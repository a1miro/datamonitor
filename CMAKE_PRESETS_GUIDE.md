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

### iOS SDK Not Found Error
**Error:** `iphoneos is not an iOS SDK` or `xcrun: error: SDK "iphoneos" cannot be located`

**Root Cause:** This happens when `xcode-select` is pointing to Command Line Tools instead of full Xcode.

**Solution:**
1. Verify Xcode is installed:
   ```bash
   ls -la /Applications/ | grep -i xcode
   ```

2. Check current developer directory:
   ```bash
   xcode-select --print-path
   ```

3. If it shows `/Library/Developer/CommandLineTools`, switch to Xcode:
   ```bash
   sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
   ```

4. Verify iOS SDKs are now available:
   ```bash
   xcodebuild -showsdks
   xcrun --show-sdk-path --sdk iphoneos
   xcrun --show-sdk-path --sdk iphonesimulator
   ```

5. Accept Xcode license if prompted:
   ```bash
   sudo xcodebuild -license accept
   ```

**Alternative for CMake Presets:** If you only need macOS builds, you can temporarily disable iOS presets by adding conditions to `CMakePresets.json`.

### Qt6 Not Found for iOS
**Error:** `Could not find a package configuration file provided by "Qt6"` when building for iOS

**Root Cause:** CMake can't find Qt6 for iOS builds because the Qt6 installation path is not specified.

**Solution:**
1. Verify Qt6 with iOS support is installed:
   ```bash
   ls -la /Users/amironenko/Qt6/
   ls -la /Users/amironenko/Qt6/6.9.1/ios/
   ```

2. Check if Qt6Config.cmake exists for iOS:
   ```bash
   ls -la /Users/amironenko/Qt6/6.9.1/ios/lib/cmake/Qt6/Qt6Config.cmake
   ```

3. Update CMakePresets.json to include Qt6 path in iOS presets:
   ```json
   {
     "name": "ios-device-debug",
     "cacheVariables": {
       "CMAKE_PREFIX_PATH": "/Users/amironenko/Qt6/6.9.1/ios",
       // ...other variables
     }
   }
   ```

4. Alternatively, set environment variable:
   ```bash
   export CMAKE_PREFIX_PATH="/Users/amironenko/Qt6/6.9.1/ios"
   cmake --preset ios-device-debug
   ```

**Note:** Make sure you're using the same Qt6 version (6.9.1) for all platforms to avoid compatibility issues.

## iOS Device Deployment

### Deploy to iPhone/iPad

After building the iOS app, you need to deploy it to your physical device:

#### Method 1: Using Xcode (Recommended)
```bash
# Build for iOS device
cmake --preset ios-device-debug && cmake --build --preset ios-device-debug

# Open the Xcode project
open build/ios-device-debug/DataMonitor.xcodeproj
```

**In Xcode:**
1. Connect your iPhone via USB
2. Select your device from the device dropdown (next to the scheme selector)
3. Click the "Play" button or press `Cmd+R` to build and run
4. If prompted, trust the developer certificate on your iPhone:
   - Go to **Settings > General > VPN & Device Management**
   - Find your Apple ID under "Developer App"
   - Tap **Trust [Your Apple ID]**

#### Method 2: Using ios-deploy (Command Line)
```bash
# Install ios-deploy if not already installed
brew install ios-deploy

# Deploy to first connected device (default)
ios-deploy --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app

# List all connected devices to get device IDs
ios-deploy --detect

# Deploy to specific device by ID
ios-deploy --id [DEVICE_ID] --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app

# Deploy to specific device by name
ios-deploy --device-name "Andrei's iPhone" --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app
```

#### Method 3: Using Xcode Command Line Tools
```bash
# List connected devices with IDs
xcrun devicectl list devices

# Deploy to specific device using device ID
xcrun devicectl install app --device 136706E6-68C4-5654-B6E1-E2ABBEDF4821 build/ios-device-debug/Debug-iphoneos/datamonitor.app

# Deploy using device name (if unique)
xcrun devicectl install app --device "Andrei's iPhone" build/ios-device-debug/Debug-iphoneos/datamonitor.app
```

### Code Signing Requirements

For deployment to work, ensure proper code signing:

1. **Apple Developer Account**: You need an Apple ID (free tier works for personal devices)

2. **Automatic Signing** (Easiest):
   - In Xcode, select your target
   - Go to **Signing & Capabilities**
   - Enable **Automatically manage signing**
   - Select your team/Apple ID

3. **Manual Signing** (Advanced):
   - Create certificates and provisioning profiles in Apple Developer portal
   - Configure signing in Xcode manually

### Common Error: "A valid provisioning profile for this executable was not found" (0xe8008015)

This error occurs when the app isn't properly code signed for your device. Here's how to fix it:

#### Step 1: Open the App in Xcode
```bash
# After building, open the Xcode project
open build/ios-device-debug/DataMonitor.xcodeproj
```

#### Step 2: Configure Signing in Xcode
1. Select the **DataMonitor** target in the project navigator
2. Go to **Signing & Capabilities** tab
3. **Team**: Select your Apple ID team (if not shown, add your Apple ID in Xcode Preferences)
4. **Bundle Identifier**: Change to something unique like `com.yourname.datamonitor`
5. **Automatically manage signing**: Enable this checkbox
6. **Provisioning Profile**: Should show "Xcode Managed Profile" when automatic signing is enabled

#### Step 3: Add Your Apple ID to Xcode (if needed)
1. **Xcode > Preferences** (or **Settings** on newer Xcode)
2. **Accounts** tab
3. Click **+** and **Add Apple ID**
4. Sign in with your Apple ID
5. Click **Download Manual Profiles** if needed

#### Step 4: Register Your Device
1. Connect your iPhone/iPad via USB
2. In Xcode, go to **Window > Devices and Simulators**
3. Select your device
4. Click **Use for Development**
5. Follow the prompts to register the device

#### Step 5: Trust Developer on Device
1. On your iPhone/iPad: **Settings > General > VPN & Device Management**
2. Under "Developer App", find your Apple ID
3. Tap **Trust [Your Apple ID]**
4. Confirm by tapping **Trust**

#### Step 6: Build and Deploy from Xcode
1. In Xcode, select your connected device from the device dropdown
2. Click the **Play** button or press `Cmd+R`
3. The app should build, install, and launch on your device

#### Alternative: Fix Bundle Identifier in CMake
If you prefer to fix this at the CMake level:

1. Edit `CMakeLists.txt` and change the bundle identifier:
```cmake
set_target_properties(datamonitor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourname.datamonitor"  # Change this
    # ...other properties
)
```

2. Rebuild and try deploying again:
```bash
cmake --preset ios-device-debug
cmake --build --preset ios-device-debug
ios-deploy --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app
```

#### Quick Fix Checklist
- [ ] Device is connected via USB and trusted
- [ ] Apple ID is added to Xcode
- [ ] Device is registered for development
- [ ] Bundle identifier is unique (not com.example.*)
- [ ] Automatic signing is enabled in Xcode
- [ ] Developer certificate is trusted on device
- [ ] Using Xcode deployment instead of ios-deploy for first-time setup

#### Why This Happens
- **Default Bundle ID**: CMake generates a generic bundle identifier that conflicts with existing apps
- **Missing Provisioning**: Device isn't registered with your Apple ID
- **Untrusted Developer**: Your developer certificate isn't trusted on the device
- **Wrong Signing**: App is unsigned or signed with wrong certificate

#### Pro Tip
For the smoothest experience, always do the initial deployment through Xcode. Once the app is properly signed and installed, you can use `ios-deploy` for subsequent deployments.

### Debugging Deployment Issues

#### Check Device Connection Status
```bash
# List all connected devices
xcrun devicectl list devices

# Check if ios-deploy can see your device
ios-deploy --detect

# Get detailed device information
xcrun devicectl list devices --verbose

# Check Xcode's view of devices
open -a Xcode
# Then: Window > Devices and Simulators
```

#### Verify App Installation
```bash
# List installed apps on device (requires device ID)
xcrun devicectl list apps --device [DEVICE_ID]

# Check if your app is installed
xcrun devicectl list apps --device [DEVICE_ID] | grep -i datamonitor
```

#### Common Deployment Commands
```bash
# Deploy and launch with detailed logging
ios-deploy --debug --verbose --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app

# Deploy to specific device by ID
ios-deploy --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app --id [DEVICE_ID]

# Deploy to device by name
ios-deploy --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app --device-name "Your iPhone"

# Just install without launching
ios-deploy --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app --no-debug

# Uninstall the app first, then install
ios-deploy --uninstall_only --bundle_id com.yourname.datamonitor --device-name "Your iPhone"
ios-deploy --debug --bundle build/ios-device-debug/Debug-iphoneos/datamonitor.app --device-name "Your iPhone"
```

#### Using devicectl (iOS 17+)
```bash
# Install app using devicectl (newer method)
xcrun devicectl device install app --device [DEVICE_ID] build/ios-device-debug/Debug-iphoneos/datamonitor.app

# Launch app using devicectl
xcrun devicectl device process launch --device [DEVICE_ID] com.yourname.datamonitor
```

#### Device Trust Issues
If you see "device is not paired" or trust issues:

1. **Reset Trust**:
   - Disconnect device
   - On device: Settings > General > Reset > Reset Location & Privacy
   - Reconnect and re-trust computer

2. **Check Pairing**:
   ```bash
   # Verify device pairing
   xcrun devicectl list devices
   ```

3. **Re-pair Device**:
   - Disconnect and reconnect USB
   - When prompted on device, tap "Trust This Computer"
   - Enter device passcode

#### Build Troubleshooting
```bash
# Clean build completely
rm -rf build/ios-device-debug
cmake --preset ios-device-debug
cmake --build --preset ios-device-debug

# Check if app bundle is valid
ls -la build/ios-device-debug/Debug-iphoneos/datamonitor.app/
codesign -dv build/ios-device-debug/Debug-iphoneos/datamonitor.app/

# Verify app structure
find build/ios-device-debug/Debug-iphoneos/datamonitor.app/ -type f
```

#### Error Code Reference
- **0xe8008015**: Invalid provisioning profile / code signing issue
- **0xe8000022**: Device is locked or passcode required
- **0xe8000067**: App installation failed (often bundle ID conflict)
- **0xe800001c**: Device not found or not connected
- **0xe8008001**: Code signing identity not found

#### When All Else Fails
1. **Use Xcode for First Deployment**: Always recommended for new projects
2. **Check Apple Developer Status**: Ensure your Apple ID is valid
3. **Try Different USB Port/Cable**: Sometimes connection issues cause deployment failures
4. **Restart Devices**: Restart both Mac and iOS device
5. **Check iOS Version**: Ensure iOS version is compatible with Qt 6.9.1
6. **Verify Qt Installation**: Ensure iOS libraries are properly installed
