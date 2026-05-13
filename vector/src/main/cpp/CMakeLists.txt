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
    src/content_filter.cpp
    src/network_stats.cpp
    src/masquerade.cpp
    src/user_mask.cpp
    src/chunked_upload.cpp
    src/chat_features.cpp
    src/invitation_hide.cpp
    src/thread_aggregator.cpp
    src/user_messages.cpp
    src/room_version.cpp
    src/chat_preview.cpp
    src/ram_monitor.cpp
    src/cache_manager.cpp
    src/message_aggregator.cpp
    src/room_info.cpp
    src/deleted_archive.cpp
    src/search_index.cpp
    src/module_loader.cpp
    src/notification.cpp
    src/room_mirror.cpp
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
