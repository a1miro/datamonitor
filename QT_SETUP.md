# Qt Environment Setup for DataMonitor

This project uses a single `QT_ROOT` environment variable to specify the Qt installation path, making the CMake presets portable across different development environments.

## Setup

Set the `QT_ROOT` environment variable to point to your Qt installation directory:

### Linux/macOS
```bash
export QT_ROOT="/path/to/Qt/6.x.x"
```

### Examples

**Standard Qt Installer locations:**
```bash
# Linux
export QT_ROOT="/opt/Qt/6.9.1"

# macOS
export QT_ROOT="/Users/username/Qt/6.9.1"

# Alternative locations
export QT_ROOT="$HOME/Qt/6.9.1"
```

**Homebrew on macOS:**
```bash
export QT_ROOT="/opt/homebrew/Cellar/qt@6/6.x.x"
```

## Expected Directory Structure

Your Qt installation should have this structure under `$QT_ROOT`:
```
$QT_ROOT/
├── gcc_64/          # Linux Qt binaries
├── macos/           # macOS Qt binaries
└── ios/             # iOS Qt binaries
```

## Persistent Setup

Add the export to your shell profile:

**~/.bashrc or ~/.zshrc:**
```bash
export QT_ROOT="/opt/Qt/6.9.1"  # Adjust path as needed
```

## VS Code Integration

Add to `.vscode/settings.json`:
```json
{
    "cmake.environment": {
        "QT_ROOT": "/opt/Qt/6.9.1"
    }
}
```

## Verification

Test your setup:
```bash
echo $QT_ROOT
ls $QT_ROOT/gcc_64    # Should show Qt Linux installation
ls $QT_ROOT/macos     # Should show Qt macOS installation (on macOS)
ls $QT_ROOT/ios       # Should show Qt iOS installation (on macOS)
```

## Alternative: User CMakePresets

Instead of environment variables, you can create `CMakeUserPresets.json` to override paths:

```json
{
    "version": 3,
    "configurePresets": [
        {
            "name": "linux-debug",
            "inherits": "linux-debug",
            "cacheVariables": {
                "CMAKE_PREFIX_PATH": "/custom/path/to/Qt/6.x.x/gcc_64",
                "Qt6_DIR": "/custom/path/to/Qt/6.x.x/gcc_64/lib/cmake/Qt6"
            }
        }
    ]
}
```

This approach allows per-user customization without modifying the main `CMakePresets.json` file.
