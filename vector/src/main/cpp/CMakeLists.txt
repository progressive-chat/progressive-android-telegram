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
    src/input_tools.cpp
    src/llm.cpp
    src/read_receipts.cpp
    src/room_analytics.cpp
    src/chat_tools.cpp
    src/lang_detect.cpp
    src/avatar_history.cpp
    src/event_link.cpp
    src/lightweight_call.cpp
    src/scheduled_edit.cpp
    src/svg_draw.cpp
    src/profile_swiper.cpp
    src/rainbow.cpp
    src/text_formats.cpp
    src/url_tools.cpp
    src/notif_priority.cpp
    src/matrix_patterns.cpp
    src/desync_detector.cpp
    src/latency_stats.cpp
    src/string_utils.cpp
    src/location_sharing.cpp
    src/color_utils.cpp
    src/e2ee_utils.cpp
    src/thumbnail.cpp
    src/waveform.cpp
    src/session_timeout.cpp
    src/password_validator.cpp
    src/spellcheck.cpp
    src/draft_manager.cpp
    src/link_preview.cpp
    src/hash_utils.cpp
    src/room_stats.cpp
    src/mention_parser.cpp
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
