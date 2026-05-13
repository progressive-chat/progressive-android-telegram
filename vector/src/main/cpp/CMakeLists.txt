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
    src/eventcache.cpp
    src/eventdb.cpp
    src/translate.cpp
    src/proxy.cpp
    src/yggdrasil.cpp
    src/markdown.cpp
    src/account_export.cpp
    src/audio_engine.cpp
    src/media_filter.cpp
    src/jni_bridge.cpp
)

target_include_directories(
    progressive_native
    PUBLIC
    include
)

find_library(log_lib log)

# SQLite3 - available in Android NDK sysroot
find_library(sqlite3_lib sqlite3)

target_link_libraries(
    progressive_native
    ${log_lib}
    ${sqlite3_lib}
)
