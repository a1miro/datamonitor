cmake_minimum_required(VERSION 3.16)

project(DataMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IOS TRUE)
    set(APPLE_MOBILE TRUE)
endif()

# Set deployment targets
if(APPLE)
    if(IOS)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0" CACHE STRING "iOS deployment target")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "macOS deployment target")
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_standard_project_setup()

# Set Qt policies to suppress warnings
qt_policy(SET QTP0001 NEW)  # Use ':/qt/qml/' as default resource prefix
qt_policy(SET QTP0004 NEW)  # Require qmldir files for QML modules

qt_add_executable(datamonitor
    src/main.cpp
    src/datasource.cpp
    src/datasource.h
    src/sinewave.cpp
    src/sinewave.h
    src/squarewave.cpp
    src/squarewave.h
    src/trianglewave.cpp
    src/trianglewave.h
    src/plotrenderer.cpp
    src/plotrenderer.h
    src/plotnode.cpp
    src/plotnode.h
)

qt_add_qml_module(datamonitor
    URI DataMonitor
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/PlotView.qml
    SOURCES
        src/datasource.cpp
        src/datasource.h
        src/sinewave.cpp
        src/sinewave.h
        src/squarewave.cpp
        src/squarewave.h
        src/trianglewave.cpp
        src/trianglewave.h
        src/plotrenderer.cpp
        src/plotrenderer.h
        src/plotnode.cpp
        src/plotnode.h
)

target_link_libraries(datamonitor PRIVATE Qt6::Core Qt6::Quick)

# Enable debug symbols
set_target_properties(datamonitor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.datamonitor
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# iOS-specific configurations
if(IOS)
    set_target_properties(datamonitor PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.example.datamonitor"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"  # iPhone and iPad
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET}
        XCODE_ATTRIBUTE_ENABLE_BITCODE NO
        XCODE_ATTRIBUTE_SKIP_INSTALL NO
        XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
        XCODE_ATTRIBUTE_SUPPORTS_MACCATALYST NO
    )

    # Set iOS architectures
    if(CMAKE_OSX_SYSROOT MATCHES "iphonesimulator")
        set_target_properties(datamonitor PROPERTIES
            OSX_ARCHITECTURES "x86_64;arm64"
        )
    else()
        set_target_properties(datamonitor PROPERTIES
            OSX_ARCHITECTURES "arm64"
        )
    endif()
endif()

# macOS-specific configurations
if(APPLE AND NOT IOS)
    # Only set custom Info.plist if the file exists
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
        set_target_properties(datamonitor PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
            OSX_ARCHITECTURES "x86_64;arm64"  # Universal binary
        )
    else()
        set_target_properties(datamonitor PROPERTIES
            OSX_ARCHITECTURES "x86_64;arm64"  # Universal binary
        )
    endif()
endif()
