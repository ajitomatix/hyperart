cmake_minimum_required(VERSION 3.5)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Xml Gui)

set(CORE_SOURCES
    defs.h defs.cpp
    matrix.h matrix.cpp
    transformation.h transformation.cpp
    element.h element.cpp
    pgonedge.h pgonedge.cpp
    irregularpgon.h irregularpgon.cpp
    regularpgon.h regularpgon.cpp
    permutation.h permutation.cpp
    pattern.h pattern.cpp
    parameters.h parameters.cpp
    layer.h layer.cpp
    datareader.h datareader.cpp
    diagram.h diagram.cpp
)

add_library(hyperart_core STATIC ${CORE_SOURCES})

target_link_libraries(hyperart_core PUBLIC Qt6::Core Qt6::Xml Qt6::Gui)

# Expose the include path so depending targets don't need changes
target_include_directories(hyperart_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
