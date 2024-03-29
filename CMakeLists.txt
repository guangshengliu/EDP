cmake_minimum_required(VERSION 3.5)

project(EDP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        experiment_table.h
        experiment_table.cpp
        lib/lib.h
        lib/special_math.h
        orthogonal/orthogonal_dialog.h
        orthogonal/orthogonal_dialog.cpp
        orthogonal/orthogonal_table.h
        orthogonal/orthogonal_widget.h
        orthogonal/orthogonal_widget.cpp
        orthogonal/orthogonal_result.h
        orthogonal/orthogonal_result.cpp
        uniform/uniform_dialog.h
        uniform/uniform_dialog.cpp
        uniform/uniform_table.h
        uniform/uniform_widget.h
        uniform/uniform_widget.cpp
        uniform/uniform_result.h
        uniform/uniform_result.cpp
        lib/matrix.h
        lib/regression_analysis.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EDP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EDP APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EDP SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EDP
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(EDP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(EDP PRIVATE Qt6::Charts)

set_target_properties(EDP PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS EDP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EDP)
endif()
