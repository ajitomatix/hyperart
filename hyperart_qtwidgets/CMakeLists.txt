cmake_minimum_required(VERSION 3.5)

find_package(Qt6 6.8 REQUIRED COMPONENTS Widgets)

set(WIDGETS_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    poincareview.cpp poincareview.h
    canvaselements.cpp canvaselements.h
)

qt_add_executable(hyperart_qtwidgets
    MACOSX_BUNDLE
    MANUAL_FINALIZATION
    ${WIDGETS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/HyperArt.icns
)

target_link_libraries(hyperart_qtwidgets PRIVATE hyperart_core Qt6::Widgets)

set_target_properties(hyperart_qtwidgets PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.ajitomatix.hyperart.legacy
    MACOSX_BUNDLE_BUNDLE_NAME "Hyper Art Legacy"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "HyperArtLegacy"
    MACOSX_BUNDLE_ICON_FILE HyperArt.icns
    RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../icons/HyperArt.icns
)

install(TARGETS hyperart_qtwidgets
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(hyperart_qtwidgets)
