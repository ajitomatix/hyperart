cmake_minimum_required(VERSION 3.5)

project(hyperart_qtwidgets VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        defs.h
        defs.cpp
        datareader.h
        datareader.cpp
        diagram.h
        diagram.cpp
        pattern.h
        pattern.cpp
        permutation.h
        permutation.cpp
        element.h
        element.cpp
        transformation.h
        transformation.cpp
        matrix.h
        matrix.cpp
        layer.h
        layer.cpp
        pgonedge.h
        pgonedge.cpp
        parameters.h
        parameters.cpp
        regularpgon.h
        regularpgon.cpp
        irregularpgon.h
        irregularpgon.cpp
        poincareview.h
        poincareview.cpp
        canvaselements.h
        canvaselements.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hyperart_qtwidgets
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET hyperart_qtwidgets APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(hyperart_qtwidgets SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(hyperart_qtwidgets
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(hyperart_qtwidgets PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(hyperart_qtwidgets PRIVATE Qt${QT_VERSION_MAJOR}::Xml)

set_target_properties(hyperart_qtwidgets PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER hyperart.ajitomatix.com
    MACOSX_BUNDLE_BUNDLE_NAME "Hyper Art"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS hyperart_qtwidgets
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hyperart_qtwidgets)
endif()
