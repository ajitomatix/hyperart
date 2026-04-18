cmake_minimum_required(VERSION 3.5)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Gui)

qt_add_executable(hyperart_qtquick
    MACOSX_BUNDLE
    MANUAL_FINALIZATION
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/HyperArt.icns
)

qt_add_qml_module(hyperart_qtquick
    URI hyperart.main
    VERSION 1.0
    QML_FILES Main.qml
)

target_link_libraries(hyperart_qtquick PRIVATE hyperart_core Qt6::Quick Qt6::Gui)

set_target_properties(hyperart_qtquick PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.ajitomatix.hyperart
    MACOSX_BUNDLE_BUNDLE_NAME "Hyper Art"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "HyperArt"
    MACOSX_BUNDLE_ICON_FILE HyperArt.icns
    RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../icons/HyperArt.icns
)

qt_finalize_executable(hyperart_qtquick)
