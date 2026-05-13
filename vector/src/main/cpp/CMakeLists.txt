cmake_minimum_required(VERSION 3.22.1)
project("progressive_native")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    progressive_native
    SHARED
    src/jumptodate.cpp
    src/json_parser.cpp
    src/relation.cpp
    src/exporter.cpp
    src/jni_bridge.cpp
)

target_include_directories(
    progressive_native
    PUBLIC
    include
)

find_library(log_lib log)

target_link_libraries(
    progressive_native
    ${log_lib}
)
