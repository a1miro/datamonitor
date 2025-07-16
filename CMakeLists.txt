cmake_minimum_required(VERSION 3.16)

project(DataMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_standard_project_setup()

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
