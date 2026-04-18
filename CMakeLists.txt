cmake_minimum_required(VERSION 3.5)

project(hyperart VERSION 0.9.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core)

add_subdirectory(hyperart_core)
add_subdirectory(hyperart_qtwidgets)
add_subdirectory(hyperart_qtquick)
