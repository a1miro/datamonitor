# iOS Toolchain for CMake
# This toolchain file configures CMake for iOS builds

set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_SYSTEM_VERSION 15.0)

# Set the deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0)

# Determine if building for simulator or device
if(NOT DEFINED IOS_PLATFORM)
    set(IOS_PLATFORM "OS")  # Default to device
endif()

if(IOS_PLATFORM STREQUAL "SIMULATOR" OR IOS_PLATFORM STREQUAL "SIMULATOR64")
    set(CMAKE_OSX_SYSROOT iphonesimulator)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(IOS_SIMULATOR TRUE)
else()
    set(CMAKE_OSX_SYSROOT iphoneos)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(IOS_SIMULATOR FALSE)
endif()

# Set the find root path mode
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set the install name tool
set(CMAKE_INSTALL_NAME_TOOL xcrun install_name_tool)

# Skip the platform compiler checks for cross compiling
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)

# Set minimum iOS version
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})

# Disable bitcode (deprecated in Xcode 14)
set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE NO)

# Set code signing settings (can be overridden)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "")

# Set device family (1=iPhone, 2=iPad, 1,2=Universal)
set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")

# Additional iOS-specific settings
set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "arm64")

if(IOS_SIMULATOR)
    set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "x86_64 arm64")
endif()

# Qt-specific iOS settings
set(CMAKE_XCODE_ATTRIBUTE_SKIP_INSTALL NO)
set(CMAKE_XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)")
