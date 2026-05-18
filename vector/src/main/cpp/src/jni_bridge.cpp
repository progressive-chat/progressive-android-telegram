#include <jni.h>
#include <string>
#include <android/log.h>
#include "progressive/jumptodate.hpp"
#include "progressive/relation.hpp"
#include "progressive/exporter.hpp"
#include "progressive/olm_session.hpp"
#include "progressive/bidirectional_utils.hpp"
#include "progressive/eventcache.hpp"
#include "progressive/translate.hpp"
#include "progressive/proxy.hpp"
#include "progressive/yggdrasil.hpp"
#include "progressive/markdown.hpp"
#include "progressive/account_export.hpp"
#include "progressive/audio_engine.hpp"
#include "progressive/media_filter.hpp"
#include "progressive/content_filter.hpp"
#include "progressive/network_stats.hpp"
#include "progressive/masquerade.hpp"
#include "progressive/user_mask.hpp"
#include "progressive/chunked_upload.hpp"
#include "progressive/chat_features.hpp"
#include "progressive/invitation_hide.hpp"
#include "progressive/thread_aggregator.hpp"
#include "progressive/user_messages.hpp"
#include "progressive/room_version.hpp"
#include "progressive/chat_preview.hpp"
#include "progressive/ram_monitor.hpp"
#include "progressive/cache_manager.hpp"
#include "progressive/message_aggregator.hpp"
#include "progressive/room_info.hpp"
#include "progressive/deleted_archive.hpp"
#include "progressive/search_index.hpp"
#include "progressive/module_loader.hpp"
#include "progressive/notification.hpp"
#include "progressive/room_mirror.hpp"
#include "progressive/input_tools.hpp"
#include "progressive/llm.hpp"
#include "progressive/read_receipts.hpp"
#include "progressive/room_analytics.hpp"
#include "progressive/chat_tools.hpp"
#include "progressive/lang_detect.hpp"
#include "progressive/avatar_history.hpp"
#include "progressive/event_link.hpp"
#include "progressive/lightweight_call.hpp"
#include "progressive/scheduled_edit.hpp"
#include "progressive/svg_draw.hpp"
#include "progressive/profile_swiper.hpp"
#include "progressive/rainbow.hpp"
#include "progressive/text_formats.hpp"
#include "progressive/url_tools.hpp"
#include "progressive/notif_priority.hpp"
#include "progressive/matrix_patterns.hpp"
#include "progressive/desync_detector.hpp"
#include "progressive/latency_stats.hpp"
#include "progressive/string_utils.hpp"
#include "progressive/location_sharing.hpp"
#include "progressive/color_utils.hpp"
#include "progressive/e2ee_utils.hpp"
#include "progressive/thumbnail.hpp"
#include "progressive/waveform.hpp"
#include "progressive/session_timeout.hpp"
#include "progressive/password_validator.hpp"
#include "progressive/spellcheck.hpp"
#include "progressive/draft_manager.hpp"
#include "progressive/link_preview.hpp"
#include "progressive/hash_utils.hpp"
#include "progressive/room_stats.hpp"
#include "progressive/mention_parser.hpp"
#include "progressive/poll_utils.hpp"
#include "progressive/reaction_utils.hpp"
#include "progressive/file_validator.hpp"
#include "progressive/date_utils.hpp"
#include "progressive/message_queue.hpp"
#include "progressive/pinned_events.hpp"
#include "progressive/server_capabilities.hpp"
#include "progressive/username_validator.hpp"
#include "progressive/emoji_analyzer.hpp"
#include "progressive/identity_utils.hpp"
#include "progressive/notif_analyzer.hpp"
#include "progressive/sync_analyzer.hpp"
#include "progressive/user_rating.hpp"
#include "progressive/event_timeline.hpp"
#include "progressive/room_directory.hpp"
#include "progressive/widget_utils.hpp"
#include "progressive/sso_utils.hpp"
#include "progressive/backup_utils.hpp"
#include "progressive/device_manager.hpp"
#include "progressive/presence_utils.hpp"
#include "progressive/room_permissions.hpp"
#include "progressive/room_summary.hpp"
#include "progressive/membership_utils.hpp"
#include "progressive/event_validator.hpp"
#include "progressive/room_encryption.hpp"
#include "progressive/login_utils.hpp"
#include "progressive/connection_monitor.hpp"
#include "progressive/push_rules.hpp"
#include "progressive/space_utils.hpp"
#include "progressive/event_relations.hpp"
#include "progressive/relation_builder.hpp"
#include "progressive/e2ee_decoration.hpp"
#include "progressive/room_list.hpp"
#include "progressive/media_utils.hpp"
#include "progressive/notif_settings.hpp"
#include "progressive/invite_utils.hpp"
#include "progressive/session_manager.hpp"
#include "progressive/auth_utils.hpp"
#include "progressive/content_scanner.hpp"
#include "progressive/event_encryption.hpp"
#include "progressive/report_utils.hpp"
#include "progressive/webrtc_utils.hpp"
#include "progressive/message_retry.hpp"
#include "progressive/sync_utils.hpp"
#include "progressive/event_display.hpp"
#include "progressive/permalink.hpp"
#include "progressive/network_monitor.hpp"
#include "progressive/client_info.hpp"
#include "progressive/keyshare.hpp"
#include "progressive/displayname_utils.hpp"
#include "progressive/message_location.hpp"
#include "progressive/timeline_utils.hpp"
#include "progressive/timeline_chunk.hpp"
#include "progressive/sliding_sync.hpp"
#include "progressive/oidc_auth.hpp"
#include "progressive/unified_push.hpp"
#include "progressive/sqlite_wrapper.hpp"
#include "progressive/eventdb.hpp"
#include "progressive/room_filter.hpp"
#include "progressive/thumbnail.hpp"
#include "progressive/sync_models.hpp"
#include "progressive/event_models.hpp"
#include "progressive/matrix_api.hpp"
#include "progressive/common_utils.hpp"
#include "progressive/media_utils_extra.hpp"
#include "progressive/account_data_utils.hpp"
#include "progressive/room_tombstone.hpp"
#include "progressive/openid_token.hpp"
#include "progressive/event_utils.hpp"
#include "progressive/content_builder.hpp"
#include "progressive/displayname_utils.hpp"
#include "progressive/permalink.hpp"
#include "progressive/media_utils.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/room_encryption.hpp"
#include "progressive/event_display.hpp"
#include "progressive/date_utils.hpp"
#include "progressive/presence_utils.hpp"
#include "progressive/device_naming.hpp"
#include "progressive/account_utils.hpp"
#include "progressive/login_utils.hpp"
#include "progressive/poll_utils.hpp"
#include "progressive/membership_utils.hpp"
#include "progressive/invite_utils.hpp"
#include "progressive/event_validator.hpp"
#include "progressive/widget_utils.hpp"
#include "progressive/cross_signing.hpp"
#include "progressive/edit_history.hpp"
#include "progressive/read_marker.hpp"
#include "progressive/slash_command.hpp"
#include "progressive/typing_monitor.hpp"
#include "progressive/url_preview.hpp"
#include "progressive/power_levels.hpp"
#include "progressive/well_known.hpp"
#include "progressive/room_sort.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/room_state.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/device_naming.hpp"
#include "progressive/sync_filter.hpp"
#include "progressive/room_name.hpp"
#include "progressive/notif_format.hpp"
#include "progressive/matrix_error.hpp"
#include "progressive/agent_executor.hpp"
#include "progressive/push_condition.hpp"
#include "progressive/sender_notif_filter.hpp"
#include "progressive/string_order.hpp"
#include "progressive/event_classifier.hpp"
#include "progressive/content_guard.hpp"
#include "progressive/user_status.hpp"
#include "progressive/verification_utils.hpp"
#include "progressive/account_utils.hpp"
#include <sstream>
#include <chrono>

using namespace progressive;

// --- Singleton keyword filter ---
static progressive::KeywordFilter g_keywordFilter;

// --- Singleton network stats collector ---
static progressive::NetworkStatsCollector g_netStats;

// --- Singleton user mask registry ---
static progressive::UserMaskRegistry g_userMasks;

// --- Singleton chunked uploader ---
static progressive::ChunkedUploader g_uploader;

// --- Singleton invitation hide list ---
static progressive::InvitationHideList g_inviteHide;

// --- Singleton thread aggregator ---
static progressive::ThreadAggregator g_threadAgg;

// --- Singleton cache manager ---
static progressive::CacheManager g_cacheMgr;

// --- Singleton message aggregator ---
static progressive::MessageAggregator g_msgAgg;

// --- Singleton deleted archive ---
static progressive::DeletedMessageArchive g_deletedArchive;

// --- Singleton search index ---
static progressive::SearchIndex g_searchIndex;

// --- Singleton module loader ---
static progressive::ModuleLoader g_moduleLoader;

// --- Singleton notification keywords ---
static progressive::NotificationKeywords g_notifKeywords;

// --- Singleton room mirror manager ---
static progressive::RoomMirrorManager g_mirrorMgr;

// --- Singleton symbol bar ---
static progressive::SymbolBar g_symbolBar;

// --- Singleton replacement engine ---
static progressive::ReplacementEngine g_replacementEngine;

// --- Singleton user MXID visibility ---
static progressive::UserMxidVisibility g_mxidVisibility;

// --- Singleton user hide manager ---
static progressive::UserHideManager g_userHide;

// --- Singleton message queue ---
static progressive::MessageQueue g_msgQueue;

// --- Singleton language hide manager ---
static progressive::LanguageHideManager g_langHide;

// --- Singleton chat push down ---
static progressive::ChatPushDownManager g_chatPushDown;

// --- Singleton emoji blacklist ---
static progressive::EmojiBlacklist g_emojiBlacklist;

// --- Singleton avatar history ---
static progressive::AvatarHistory g_avatarHistory;

// --- Singleton lightweight call manager ---
static progressive::LightweightCallManager g_lightCall;

// --- Singleton scheduled edit queue ---
static progressive::ScheduledEditQueue g_schedEdits;

// --- Singleton drawing canvas ---
static progressive::DrawingCanvas g_drawCanvas;

// --- Singleton profile swiper ---
static progressive::ProfileSwiper g_profileSwiper;

// --- Singleton connection monitor ---
static progressive::ConnectionMonitor g_connectionMonitor;

// --- Singleton desync detector ---
static progressive::DesyncDetector g_desyncDetector;

// --- Singleton latency tracker ---
static progressive::LatencyTracker g_latencyTracker;

// --- Singleton location sharing manager ---
static progressive::LocationSharingManager g_locationSharing;

#define LOG_TAG "ProgressiveNative"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define JNI_FUNC(ret, name) \
    JNIEXPORT ret JNICALL Java_chat_progressive_app_native_ProgressiveNative_##name

static std::string jStr(JNIEnv* env, jstring js) {
    if (!js) return "";
    const char* chars = env->GetStringUTFChars(js, nullptr);
    std::string result(chars);
    env->ReleaseStringUTFChars(js, chars);
    return result;
}

extern "C" {

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeValidateAndBuild
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
 *
 * Returns JSON string: {"url": "...", ...} or {"error": "..."}
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeValidateAndBuild(
    JNIEnv* env,
    jclass /* this */,
    jstring jRoomId,
    jstring jDateString,
    jstring jServerUrl,
    jstring jAccessToken,
    jboolean jIsEnabled
) {
    // Feature flag check — C++ gates the feature
    if (!jIsEnabled) {
        const char* err = R"({"error":"/jumptodate is disabled. Enable it in Settings → Labs."})";
        return env->NewStringUTF(err);
    }
    if (!jRoomId || !jDateString || !jServerUrl || !jAccessToken) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "All parameters must be non-null");
        return nullptr;
    }

    // Convert Java strings to C++ strings
    const char* roomIdCh     = env->GetStringUTFChars(jRoomId, nullptr);
    const char* dateCh       = env->GetStringUTFChars(jDateString, nullptr);
    const char* serverUrlCh  = env->GetStringUTFChars(jServerUrl, nullptr);
    const char* accessTokenCh = env->GetStringUTFChars(jAccessToken, nullptr);

    progressive::JumpToDateRequest request;
    request.roomId        = std::string(roomIdCh);
    request.dateString    = std::string(dateCh);
    request.serverBaseUrl = std::string(serverUrlCh);
    request.accessToken   = std::string(accessTokenCh);

    // Release Java strings
    env->ReleaseStringUTFChars(jRoomId, roomIdCh);
    env->ReleaseStringUTFChars(jDateString, dateCh);
    env->ReleaseStringUTFChars(jServerUrl, serverUrlCh);
    env->ReleaseStringUTFChars(jAccessToken, accessTokenCh);

    LOGD("nativeJumpToDate called: roomId=%s date=%s", request.roomId.c_str(), request.dateString.c_str());

    // Validate date and compute timestamp
    if (!progressive::validateAndComputeTimestamp(request)) {
        std::string errorJson = R"({"error":")" + request.errorMessage + R"("})";
        return env->NewStringUTF(errorJson.c_str());
    }

    // Build the MSC3030 URL
    std::string url = progressive::buildMsc3030Url(request);
    LOGD("MSC3030 URL: %s", url.c_str());

    // Return the URL and timestamp to Kotlin — HTTP call happens in Kotlin layer
    std::string resultJson =
        R"({"url":")" + url +
        R"(","accessToken":")" + request.accessToken +
        R"(","timestamp":)" + std::to_string(request.originServerTs) +
        R"(})";
    return env->NewStringUTF(resultJson.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeParseResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 *
 * Parses the HTTP response body and returns JSON with eventId or error.
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeParseResponse(
    JNIEnv* env,
    jclass /* this */,
    jstring jResponseBody,
    jint httpStatus
) {
    if (!jResponseBody) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "Response body must be non-null");
        return nullptr;
    }

    const char* bodyCh = env->GetStringUTFChars(jResponseBody, nullptr);
    std::string body(bodyCh);
    env->ReleaseStringUTFChars(jResponseBody, bodyCh);

    LOGD("nativeParseResponse: status=%d body_len=%zu", httpStatus, body.size());

    auto result = progressive::parseTimestampToEventResponse(body, httpStatus);

    if (result.success) {
        std::string json = R"({"eventId":")" + result.eventId + R"("})";
        return env->NewStringUTF(json.c_str());
    } else {
        std::string json =
            R"({"error":")" + result.errorMessage +
            R"(","statusCode":)" + std::to_string(result.statusCode) + R"(})";
        return env->NewStringUTF(json.c_str());
    }
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeParseRelation
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 *
 * Parses event JSON to find the source event this event relates to.
 * @param eventJson  Full JSON of the Matrix event
 * @param allowedTypes  Comma-separated list of allowed relation types (e.g. "m.annotation,m.reference")
 *                      or empty string for all types.
 *
 * Returns JSON: {"sourceEventId": "$xyz", "relationType": "m.annotation"} or {"isRelation": false}
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeParseRelation(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventJson,
    jstring jAllowedTypes
) {
    if (!jEventJson) {
        jclass exClass = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exClass, "Event JSON must be non-null");
        return nullptr;
    }

    const char* jsonCh = env->GetStringUTFChars(jEventJson, nullptr);
    std::string json(jsonCh);
    env->ReleaseStringUTFChars(jEventJson, jsonCh);

    LOGD("nativeParseRelation: eventJson_len=%zu", json.size());

    auto relation = progressive::parseRelation(json);

    if (!relation.isRelation) {
        return env->NewStringUTF(R"({"isRelation": false})");
    }

    // Check if this relation type is allowed
    bool allowed = true;
    if (jAllowedTypes) {
        const char* allowedCh = env->GetStringUTFChars(jAllowedTypes, nullptr);
        std::string allowedStr(allowedCh);
        env->ReleaseStringUTFChars(jAllowedTypes, allowedCh);

        if (!allowedStr.empty()) {
            allowed = progressive::isJumpableRelationType(relation.relationType) &&
                      allowedStr.find(relation.relationType) != std::string::npos;
        }
    } else {
        allowed = progressive::isJumpableRelationType(relation.relationType);
    }

    if (!allowed) {
        return env->NewStringUTF(R"({"isRelation": false})");
    }

    std::string resultJson =
        R"({"isRelation": true, "sourceEventId": ")" + relation.sourceEventId +
        R"(", "relationType": ")" + relation.relationType + R"("})";
    return env->NewStringUTF(resultJson.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeFormatEventHtml
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
 *
 * Formats a single event as HTML.
 * @param senderName  Display name of the sender
 * @param timestamp   ISO 8601 timestamp string
 * @param body        Plain text body
 * @param msgType     Message type (m.text, m.image, etc.)
 * @param fileName    Attachment filename if applicable
 * @param mediaSize   Attachment size in bytes as string (empty if none)
 * @param relationType Relation type string (m.annotation, m.reference, etc.)
 * @param isContinuation Whether this event is from the same sender as previous
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeFormatEventHtml(
    JNIEnv* env,
    jclass /* this */,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jFileName,
    jstring jMediaSize,
    jstring jRelationType,
    jboolean jIsContinuation
) {
    ExportEvent event;
    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";

    if (jMediaSize) {
        auto sizeStr = std::string(env->GetStringUTFChars(jMediaSize, nullptr));
        if (!sizeStr.empty()) event.mediaSize = std::stoll(sizeStr);
        env->ReleaseStringUTFChars(jMediaSize, sizeStr.c_str());
    }

    // Release all strings
    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());

    auto html = progressive::formatEventHtml(event, jIsContinuation);
    return env->NewStringUTF(html.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeFormatEventPlainText
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 *
 * Formats a single event as plain text line.
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeFormatEventPlainText(
    JNIEnv* env,
    jclass /* this */,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jFileName,
    jstring jRelationType
) {
    ExportEvent event;
    event.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp    = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.fileName     = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";

    if (jSenderName)   env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)    env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)         env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)      env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jFileName)     env->ReleaseStringUTFChars(jFileName, event.fileName.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());

    auto text = progressive::formatEventPlainText(event);
    return env->NewStringUTF(text.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeBuildHtmlExport
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/String;
 *
 * Builds a complete HTML export document from event HTML strings.
 * @param roomName    Room display name
 * @param roomTopic   Room topic
 * @param exportDate  ISO date of export
 * @param eventHtmls  Array of pre-rendered event HTML strings
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeBuildHtmlExport(
    JNIEnv* env,
    jclass /* this */,
    jstring jRoomName,
    jstring jRoomTopic,
    jstring jExportDate,
    jobjectArray jEventHtmls
) {
    auto roomName   = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    auto roomTopic  = jRoomTopic ? std::string(env->GetStringUTFChars(jRoomTopic, nullptr)) : "";
    auto exportDate = jExportDate ? std::string(env->GetStringUTFChars(jExportDate, nullptr)) : "";

    if (jRoomName)  env->ReleaseStringUTFChars(jRoomName, roomName.c_str());
    if (jRoomTopic) env->ReleaseStringUTFChars(jRoomTopic, roomTopic.c_str());
    if (jExportDate) env->ReleaseStringUTFChars(jExportDate, exportDate.c_str());

    // Build HTML document — events are already formatted, wrap them
    std::ostringstream html;
    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<title>" << escapeHtml(roomName) << " — Chat Export</title>\n";
    html << R"(<style>
body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; margin: 0; padding: 16px; background: #f5f5f5; }
.mx_ExportHeader { background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
.mx_ExportHeader h1 { margin: 0 0 8px; font-size: 1.5em; }
.mx_ExportHeader p { margin: 4px 0; color: #666; font-size: 0.9em; }
.mx_EventTile { background: #fff; border-radius: 8px; padding: 12px 16px; margin-bottom: 8px; box-shadow: 0 1px 2px rgba(0,0,0,0.05); }
.mx_EventTile_continuation { margin-top: -4px; border-radius: 0 0 8px 8px; }
.mx_EventTile_info { margin-bottom: 6px; }
.mx_EventTile_sender { font-weight: 600; color: #333; margin-right: 8px; }
.mx_MessageTimestamp { color: #999; font-size: 0.85em; }
.mx_EventTile_body { color: #222; line-height: 1.5; }
.mx_EventTile_attachment { background: #f0f0f0; border-radius: 4px; padding: 8px; margin-bottom: 8px; }
.mx_Attachment_name { font-weight: 500; }
.mx_Attachment_size { color: #999; margin-left: 8px; }
.mx_EventTile_reaction { font-style: italic; color: #666; }
.mx_EventTile_content { white-space: pre-wrap; word-wrap: break-word; }
hr { border: none; border-top: 1px solid #e0e0e0; margin: 16px 0; }
</style>)" << "\n";
    html << "</head>\n<body>\n";
    html << "<div class=\"mx_ExportHeader\">\n";
    html << "  <h1>" << escapeHtml(roomName) << "</h1>\n";
    if (!roomTopic.empty()) html << "  <p>" << escapeHtml(roomTopic) << "</p>\n";
    html << "  <p>Exported: " << exportDate << "</p>\n";

    jsize count = env->GetArrayLength(jEventHtmls);
    html << "  <p>Total messages: " << count << "</p>\n";
    html << "</div>\n";

    for (jsize i = 0; i < count; ++i) {
        auto jHtml = (jstring)env->GetObjectArrayElement(jEventHtmls, i);
        if (jHtml) {
            auto htmlStr = std::string(env->GetStringUTFChars(jHtml, nullptr));
            html << htmlStr;
            env->ReleaseStringUTFChars(jHtml, htmlStr.c_str());
        }
    }

    html << "<hr>\n<p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>\n";
    html << "</body>\n</html>";

    return env->NewStringUTF(html.str().c_str());
}

// --- Event Cache (global singleton for Stage 2 acceleration) ---
static progressive::EventCache g_eventCache;

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeCachePut
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeCachePut(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventId,
    jstring jSenderId,
    jstring jSenderName,
    jstring jTimestamp,
    jstring jBody,
    jstring jMsgType,
    jstring jEventType,
    jstring jRelationType,
    jstring jSourceEventId,
    jboolean jSentByMe
) {
    CachedEvent event;
    event.eventId     = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    event.senderId    = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
    event.senderName  = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    event.timestamp   = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    event.body        = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    event.msgType     = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    event.eventType   = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    event.relationType = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
    event.sourceEventId = jSourceEventId ? std::string(env->GetStringUTFChars(jSourceEventId, nullptr)) : "";
    event.sentByMe    = jSentByMe;

    // Release strings
    if (jEventId)     env->ReleaseStringUTFChars(jEventId, event.eventId.c_str());
    if (jSenderId)    env->ReleaseStringUTFChars(jSenderId, event.senderId.c_str());
    if (jSenderName)  env->ReleaseStringUTFChars(jSenderName, event.senderName.c_str());
    if (jTimestamp)   env->ReleaseStringUTFChars(jTimestamp, event.timestamp.c_str());
    if (jBody)        env->ReleaseStringUTFChars(jBody, event.body.c_str());
    if (jMsgType)     env->ReleaseStringUTFChars(jMsgType, event.msgType.c_str());
    if (jEventType)   env->ReleaseStringUTFChars(jEventType, event.eventType.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, event.relationType.c_str());
    if (jSourceEventId) env->ReleaseStringUTFChars(jSourceEventId, event.sourceEventId.c_str());

    g_eventCache.put(event);
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeCacheGetContext
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeCacheGetContext(
    JNIEnv* env,
    jclass /* this */,
    jstring jEventId
) {
    if (!jEventId) {
        return env->NewStringUTF(R"({"cached": false})");
    }

    auto eventId = std::string(env->GetStringUTFChars(jEventId, nullptr));
    env->ReleaseStringUTFChars(jEventId, eventId.c_str());

    auto json = g_eventCache.getContextData(eventId);
    return env->NewStringUTF(json.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeCacheSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeCacheSize(
    JNIEnv* /* env */,
    jclass /* this */
) {
    return static_cast<jint>(g_eventCache.size());
}

// --- Translation ---

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeBuildTranslateRequest
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeBuildTranslateRequest(
    JNIEnv* env, jclass,
    jstring jText, jstring jSourceLang, jstring jTargetLang,
    jstring jApiEndpoint, jstring jApiToken, jstring jModel
) {
    TranslateRequest req;
    req.text           = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    req.sourceLanguage = jSourceLang ? std::string(env->GetStringUTFChars(jSourceLang, nullptr)) : "";
    req.targetLanguage = jTargetLang ? std::string(env->GetStringUTFChars(jTargetLang, nullptr)) : "";
    req.apiEndpoint    = jApiEndpoint ? std::string(env->GetStringUTFChars(jApiEndpoint, nullptr)) : "";
    req.apiToken       = jApiToken ? std::string(env->GetStringUTFChars(jApiToken, nullptr)) : "";
    req.model          = jModel ? std::string(env->GetStringUTFChars(jModel, nullptr)) : "gpt-4o-mini";

    if (jText)        env->ReleaseStringUTFChars(jText, req.text.c_str());
    if (jSourceLang)  env->ReleaseStringUTFChars(jSourceLang, req.sourceLanguage.c_str());
    if (jTargetLang)  env->ReleaseStringUTFChars(jTargetLang, req.targetLanguage.c_str());
    if (jApiEndpoint) env->ReleaseStringUTFChars(jApiEndpoint, req.apiEndpoint.c_str());
    if (jApiToken)    env->ReleaseStringUTFChars(jApiToken, req.apiToken.c_str());
    if (jModel)       env->ReleaseStringUTFChars(jModel, req.model.c_str());

    auto body = progressive::buildTranslateRequestBody(req);
    return env->NewStringUTF(body.c_str());
}

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeParseTranslateResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeParseTranslateResponse(
    JNIEnv* env, jclass,
    jstring jResponseBody, jint jHttpStatus
) {
    if (!jResponseBody) {
        return env->NewStringUTF(R"({"success": false, "error": "Empty response"})");
    }
    auto body = std::string(env->GetStringUTFChars(jResponseBody, nullptr));
    env->ReleaseStringUTFChars(jResponseBody, body.c_str());

    auto result = progressive::parseTranslateResponse(body, jHttpStatus);

    if (result.success) {
        std::string json = R"({"success": true, "translatedText": ")" + result.translatedText + R"("})";
        return env->NewStringUTF(json.c_str());
    } else {
        std::string json = R"({"success": false, "error": ")" + result.errorMessage + R"(", "statusCode": )"
            + std::to_string(result.statusCode) + "}";
        return env->NewStringUTF(json.c_str());
    }
}

// --- Proxy Configuration ---

/*
 * Class: chat.progressive.app.native.ProgressiveNative
 * Method: nativeComputeProxyConfig
 * Signature: (IILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeComputeProxyConfig(
    JNIEnv* env, jclass,
    jint jConnType, jint jProxyType,
    jstring jHost, jint jPort,
    jstring jUsername, jstring jPassword
) {
    auto connType = static_cast<ConnectionType>(jConnType);
    auto proxyType = static_cast<ProxyType>(jProxyType);

    auto host     = jHost ? std::string(env->GetStringUTFChars(jHost, nullptr)) : "";
    auto username = jUsername ? std::string(env->GetStringUTFChars(jUsername, nullptr)) : "";
    auto password = jPassword ? std::string(env->GetStringUTFChars(jPassword, nullptr)) : "";

    if (jHost)     env->ReleaseStringUTFChars(jHost, host.c_str());
    if (jUsername) env->ReleaseStringUTFChars(jUsername, username.c_str());
    if (jPassword) env->ReleaseStringUTFChars(jPassword, password.c_str());

    auto config = progressive::computeProxyConfig(
        connType, proxyType, host, jPort, username, password
    );

    auto json = config.toJson();
    return env->NewStringUTF(json.c_str());
}

// --- Yggdrasil ---

JNIEXPORT jboolean JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeIsYggdrasilAddress(
    JNIEnv* env, jclass, jstring jAddr
) {
    if (!jAddr) return JNI_FALSE;
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    return progressive::isYggdrasilAddress(addr) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeIsYggdrasilDomain(
    JNIEnv* env, jclass, jstring jHost
) {
    if (!jHost) return JNI_FALSE;
    auto host = std::string(env->GetStringUTFChars(jHost, nullptr));
    env->ReleaseStringUTFChars(jHost, host.c_str());
    return progressive::isYggdrasilDomain(host) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeBuildYggHomeserverUrl(
    JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls
) {
    if (!jAddr) return env->NewStringUTF("");
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    auto url = progressive::buildYggHomeserverUrl(addr, jPort, jTls);
    return env->NewStringUTF(url.c_str());
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeRewriteHomeserverUrl(
    JNIEnv* env, jclass, jstring jOriginalUrl, jstring jYggAddr
) {
    if (!jOriginalUrl || !jYggAddr) return env->NewStringUTF("");
    auto original = std::string(env->GetStringUTFChars(jOriginalUrl, nullptr));
    auto ygg = std::string(env->GetStringUTFChars(jYggAddr, nullptr));
    env->ReleaseStringUTFChars(jOriginalUrl, original.c_str());
    env->ReleaseStringUTFChars(jYggAddr, ygg.c_str());
    auto result = progressive::rewriteHomeserverUrl(original, ygg);
    return env->NewStringUTF(result.c_str());
}

// --- Read Marker Time Label ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeFormatJumpToUnreadLabel(
    JNIEnv* env, jclass, jstring jReadMarkerJson, jlong jNowMs
) {
    auto json = jReadMarkerJson ? std::string(env->GetStringUTFChars(jReadMarkerJson, nullptr)) : "{}";
    if (jReadMarkerJson) env->ReleaseStringUTFChars(jReadMarkerJson, json.c_str());

    progressive::ReadMarkerState state;
    // Quick parse from JSON
    if (json.find("\"hasUnread\": true") != std::string::npos ||
        json.find("\"hasUnread\":true") != std::string::npos) state.hasUnread = true;
    // Extract timestamp
    auto tsPos = json.find("\"firstUnreadTimestampMs\"");
    if (tsPos != std::string::npos) {
        auto colon = json.find(':', tsPos);
        if (colon != std::string::npos) {
            colon++;
            while (colon < json.size() && (json[colon] == ' ' || json[colon] == '\t')) colon++;
            while (colon < json.size() && json[colon] >= '0' && json[colon] <= '9') {
                state.firstUnreadTimestampMs = state.firstUnreadTimestampMs * 10 + (json[colon] - '0');
                colon++;
            }
        }
    }

    auto label = progressive::formatJumpToUnreadLabel(state, jNowMs);
    return env->NewStringUTF(label.c_str());
}

// ==== Progressive Chat v0.2 JNI Bridges ====

// --- Event Summary ---
JNI_FUNC(jstring, nativeFormatEventSummary)(JNIEnv* env, jclass,
    jstring jType, jstring jMsgType, jstring jSender, jstring jBody,
    jstring jMembership, jstring jDisplayName, jboolean jRedacted, jboolean jEncrypted)
{
    auto result = progressive::formatEventSummary(
        jStr(env, jType), jStr(env, jMsgType), jStr(env, jSender), jStr(env, jBody),
        jStr(env, jMembership), jStr(env, jDisplayName), jRedacted, jEncrypted);
    return env->NewStringUTF(result.c_str());
}

// --- Typing Indicator ---
JNI_FUNC(jstring, nativeFormatTypingIndicator)(JNIEnv* env, jclass, jstring jNamesJson, jint jMax) {
    auto names = progressive::parseJsonStringArray(jStr(env, jNamesJson));
    auto result = progressive::formatTypingIndicator(names, jMax);
    return env->NewStringUTF(result.c_str());
}

// --- Calculate Capabilities ---
JNI_FUNC(jstring, nativeCalculateCapabilities)(JNIEnv* env, jclass, jint u, jint e, jint s,
    jint i, jint k, jint b, jint r, jint n)
{
    auto caps = progressive::calculateCapabilities(u, e, s, i, k, b, r, n);
    std::ostringstream os;
    os << R"({"isOwner":)" << caps.isOwner
       << R"(,"isModerator":)" << caps.isModerator
       << R"(,"canSendMessages":)" << caps.canSendMessages
       << R"(,"canInvite":)" << caps.canInvite
       << R"(,"canKick":)" << caps.canKick
       << R"(,"canBan":)" << caps.canBan
       << R"(,"canRedact":)" << caps.canRedact << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Content Builder ---
JNI_FUNC(jstring, nativeBuildTextContent)(JNIEnv* env, jclass, jstring jType, jstring jBody, jstring jFb) {
    auto result = progressive::buildTextContent(jStr(env, jType), jStr(env, jBody), jStr(env, jFb));
    return env->NewStringUTF(result.c_str());
}
JNI_FUNC(jstring, nativeBuildImageContent)(JNIEnv* env, jclass, jstring jBody, jstring jUrl,
    jint jW, jint jH, jlong jSz, jstring jMime) {
    auto result = progressive::buildImageContent(jStr(env, jBody), jStr(env, jUrl), jW, jH, jSz, jStr(env, jMime));
    return env->NewStringUTF(result.c_str());
}
JNI_FUNC(jstring, nativeBuildFileContent)(JNIEnv* env, jclass, jstring jBody, jstring jUrl,
    jstring jFn, jlong jSz, jstring jMime) {
    auto result = progressive::buildFileContent(jStr(env, jBody), jStr(env, jUrl), jStr(env, jFn), jSz, jStr(env, jMime));
    return env->NewStringUTF(result.c_str());
}

// --- Media ---
JNI_FUNC(jstring, nativeFormatFileSize)(JNIEnv* env, jclass, jlong jBytes) {
    return env->NewStringUTF(progressive::formatFileSize(jBytes).c_str());
}
JNI_FUNC(jstring, nativeFormatDuration)(JNIEnv* env, jclass, jlong jMs) {
    return env->NewStringUTF(progressive::formatDuration(jMs).c_str());
}
JNI_FUNC(jstring, nativeSanitizeFilename)(JNIEnv* env, jclass, jstring jName, jint jLen) {
    return env->NewStringUTF(progressive::sanitizeFilename(jStr(env, jName), jLen).c_str());
}

// --- Account Data ---
JNI_FUNC(jstring, nativeParseDirectMessages)(JNIEnv* env, jclass, jstring jJson) {
    auto dm = progressive::parseDirectMessageMap(jStr(env, jJson));
    return env->NewStringUTF(progressive::buildDirectMessageMapJson(dm).c_str());
}
JNI_FUNC(jstring, nativeParseIgnoredUsers)(JNIEnv* env, jclass, jstring jJson) {
    auto list = progressive::parseIgnoredUsers(jStr(env, jJson));
    std::ostringstream os; os << "["; for (size_t i = 0; i < list.size(); i++) {
        if (i>0) os<<","; os<<"\""<<list[i]<<"\""; } os<<"]";
    return env->NewStringUTF(os.str().c_str());
}
JNI_FUNC(jstring, nativeAddBreadcrumb)(JNIEnv* env, jclass, jstring jJson, jstring jRoom) {
    auto result = progressive::addBreadcrumb(jStr(env, jJson), jStr(env, jRoom));
    return env->NewStringUTF(result.c_str());
}
JNI_FUNC(jboolean, nativeIsValidUserId)(JNIEnv*, jclass, jstring jId) {
    return progressive::isValidUserId(jStr(nullptr, jId));
}
JNI_FUNC(jstring, nativeServerNameFromMxid)(JNIEnv* env, jclass, jstring jMxid) {
    return env->NewStringUTF(progressive::serverNameFromMxid(jStr(env, jMxid)).c_str());
}

// --- Relation Builder ---
JNI_FUNC(jstring, nativeBuildReplyRelation)(JNIEnv* env, jclass, jstring jId) {
    return env->NewStringUTF(progressive::buildReplyRelation(jStr(env, jId)).c_str());
}
JNI_FUNC(jstring, nativeBuildThreadRelation)(JNIEnv* env, jclass, jstring jR, jstring jL, jboolean jFb) {
    return env->NewStringUTF(progressive::buildThreadRelation(jStr(env, jR), jStr(env, jL), jFb).c_str());
}
JNI_FUNC(jstring, nativeBuildEditRelation)(JNIEnv* env, jclass, jstring jId) {
    return env->NewStringUTF(progressive::buildReplaceRelation(jStr(env, jId)).c_str());
}
JNI_FUNC(jstring, nativeBuildReactionRelation)(JNIEnv* env, jclass, jstring jId, jstring jKey) {
    return env->NewStringUTF(progressive::buildReactionRelation(jStr(env, jId), jStr(env, jKey)).c_str());
}
JNI_FUNC(jstring, nativeWrapWithRelation)(JNIEnv* env, jclass, jstring jContent, jstring jRel) {
    return env->NewStringUTF(progressive::wrapWithRelation(jStr(env, jContent), jStr(env, jRel)).c_str());
}

// --- Timeline Chunk (native pagination) ---
JNI_FUNC(jint, nativeTimelineAddEvents)(JNIEnv* env, jclass, jstring jRoom, jstring jEvents, jstring jPrev, jstring jNext, jint jDir) {
    static std::unordered_map<std::string, std::unique_ptr<progressive::TimelineChunkManager>> managers;
    auto room = jStr(env, jRoom);
    if (managers.find(room) == managers.end()) managers[room] = std::make_unique<progressive::TimelineChunkManager>(room);
    auto& mgr = *managers[room];
    // Parse events JSON array
    auto eventsJson = jStr(env, jEvents);
    std::vector<progressive::TimelineEventData> events;
    // Simplified: each event is a JSON object with event_id, type, sender, etc.
    // Full implementation would parse the array
    auto dir = jDir == 0 ? progressive::TimelineDirection::FORWARDS : progressive::TimelineDirection::BACKWARDS;
    return mgr.addChunk("chunk_" + std::to_string(time(nullptr)), events, jStr(env, jPrev), jStr(env, jNext), dir);
}

JNI_FUNC(jstring, nativeTimelineGetEvents)(JNIEnv* env, jclass, jstring jRoom) {
    static std::unordered_map<std::string, std::unique_ptr<progressive::TimelineChunkManager>> managers;
    auto room = jStr(env, jRoom);
    if (managers.find(room) == managers.end()) managers[room] = std::make_unique<progressive::TimelineChunkManager>(room);
    auto& mgr = *managers[room];
    auto events = mgr.getEventsInOrder();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < events.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << events[i].eventId << R"(","type":")" << events[i].type << R"(","di":)" << events[i].displayIndex << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeTimelineGetEvent)(JNIEnv* env, jclass, jstring jId) {
    return env->NewStringUTF("{}");
}

JNI_FUNC(void, nativeTimelineClear)(JNIEnv* env, jclass, jstring jRoom) {
    static std::unordered_map<std::string, std::unique_ptr<progressive::TimelineChunkManager>> managers;
    auto room = jStr(env, jRoom);
    auto it = managers.find(room);
    if (it != managers.end()) it->second->clear();
}

JNI_FUNC(jstring, nativeTimelineGetReplies)(JNIEnv* env, jclass, jstring jId) {
    return env->NewStringUTF("[]");
}

JNI_FUNC(jstring, nativeTimelineGetLatestEdit)(JNIEnv* env, jclass, jstring jId) {
    return env->NewStringUTF(jStr(env, jId).c_str());
}

JNI_FUNC(jstring, nativeTimelineGetThreadEvents)(JNIEnv* env, jclass, jstring jRoot) {
    return env->NewStringUTF("[]");
}

// --- Room Filter ---
JNI_FUNC(jstring, nativeFilterRooms)(JNIEnv* env, jclass, jstring jRooms, jint jCat, jstring jQuery) {
    // Stub: returns input as-is. Full impl parses JSON array, filters, returns.
    return jRooms;
}

// --- Sliding Sync ---
JNI_FUNC(jstring, nativeSlidingSync)(JNIEnv* env, jclass, jstring jReq, jstring jHs, jstring jTok) {
    progressive::SlidingSyncRequest req;
    req.pos = ""; req.timeout = 30000;
    auto resp = progressive::slidingSync(req, jStr(env, jHs), jStr(env, jTok));
    return resp.success ? env->NewStringUTF(resp.pos.c_str()) : env->NewStringUTF("");
}

// --- OIDC ---
JNI_FUNC(jstring, nativeDiscoverOidc)(JNIEnv* env, jclass, jstring jHs) {
    auto d = progressive::discoverOidc(jStr(env, jHs));
    std::ostringstream os;
    os << R"({"supportsOidc":)" << d.supportsOidc
       << R"(,"supportsPassword":)" << d.supportsPassword
       << R"(,"issuer":")" << d.issuer << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeBuildOAuthUrl)(JNIEnv* env, jclass, jstring jCid, jstring jRedir, jstring jState, jstring jPkce, jstring jPrompt) {
    progressive::OidcDiscovery disc;
    disc.authorizationEndpoint = jStr(env, jRedir);
    auto url = progressive::buildOAuthAuthorizationUrl(disc, jStr(env, jCid), jStr(env, jRedir), jStr(env, jState), jStr(env, jPkce), jStr(env, jPrompt));
    return env->NewStringUTF(url.c_str());
}

JNI_FUNC(jstring, nativeExchangeOidcCode)(JNIEnv* env, jclass, jstring jEp, jstring jCid, jstring jRedir, jstring jCode, jstring jVerifier) {
    auto tok = progressive::exchangeOidcCode(jStr(env, jEp), jStr(env, jCid), jStr(env, jRedir), jStr(env, jCode), jStr(env, jVerifier));
    std::ostringstream os;
    os << R"({"success":)" << tok.success << R"(,"accessToken":")" << tok.accessToken << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseOAuthCallback)(JNIEnv* env, jclass, jstring jUrl, jstring jRedir) {
    auto cb = progressive::parseOAuthCallback(jStr(env, jUrl), jStr(env, jRedir));
    std::ostringstream os;
    os << R"({"action":")" << (cb.action == progressive::OAuthAction::SUCCESS ? "success" : cb.action == progressive::OAuthAction::GO_BACK ? "go_back" : "none") << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeGenerateOAuthState)(JNIEnv* env, jclass) {
    return env->NewStringUTF(progressive::generateOAuthState().c_str());
}

JNI_FUNC(jstring, nativeGeneratePkce)(JNIEnv* env, jclass) {
    auto pkce = progressive::generatePkce();
    std::ostringstream os;
    os << R"({"codeVerifier":")" << pkce.codeVerifier << R"(","codeChallenge":")" << pkce.codeChallenge << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- UnifiedPush ---
JNI_FUNC(jstring, nativeParseUnifiedPushMessage)(JNIEnv* env, jclass, jstring jJson) {
    auto msg = progressive::parseUnifiedPushMessage(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"eventId":")" << msg.eventId << R"(","roomId":")" << msg.roomId
       << R"(","valid":)" << msg.valid << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Thumbnail Calc ---
JNI_FUNC(jstring, nativeCalculateThumbnailSize)(JNIEnv* env, jclass, jint jOw, jint jOh, jint jMw, jint jMh) {
    progressive::ThumbnailParams params;
    params.sourceW = jOw; params.sourceH = jOh;
    params.maxW = jMw; params.maxH = jMh;
    auto result = progressive::computeThumbnail(params);
    std::ostringstream os;
    os << R"({"width":)" << result.targetW << R"(,"height":)" << result.targetH << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Native SQLite Database (SqliteDB - replaces Realm for timeline storage) ---
// Controlled by Labs: SETTINGS_LABS_NATIVE_DB

static progressive::EventDatabase g_eventDb;
static std::unordered_map<std::string, std::unique_ptr<progressive::SqliteDB>> g_sqliteDbs;

// EventDatabase JNI (existing Kotlin signatures)
JNI_FUNC(jboolean, nativeDbOpen)(JNIEnv* env, jclass, jstring jPath) {
    return g_eventDb.open(jStr(env, jPath)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(void, nativeDbClose)(JNIEnv*, jclass) {
    g_eventDb.close();
}

JNI_FUNC(void, nativeDbInsertEvent)(JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jSenderId, jstring jSenderName,
    jstring jTimestamp, jstring jBody, jstring jMsgType, jstring jEventType,
    jstring jRelType, jstring jSourceId,
    jlong jOriginTs, jint jDi, jboolean jSentByMe) {
    progressive::DbEvent e;
    e.eventId = jStr(env, jEventId);
    e.roomId = jStr(env, jRoomId);
    e.senderId = jStr(env, jSenderId);
    e.senderName = jStr(env, jSenderName);
    e.timestamp = jStr(env, jTimestamp);
    e.body = jStr(env, jBody);
    e.msgType = jStr(env, jMsgType);
    e.eventType = jStr(env, jEventType);
    e.relationType = jStr(env, jRelType);
    e.sourceEventId = jStr(env, jSourceId);
    e.originServerTs = jOriginTs;
    e.displayIndex = jDi;
    e.sentByMe = jSentByMe;
    e.isEncrypted = false;
    g_eventDb.insertEvent(e);
}

JNI_FUNC(jstring, nativeDbGetContext)(JNIEnv* env, jclass, jstring jEventId) {
    return env->NewStringUTF(g_eventDb.getContextJson(jStr(env, jEventId)).c_str());
}

JNI_FUNC(void, nativeDbClearRoom)(JNIEnv* env, jclass, jstring jRoomId) {
    g_eventDb.clearRoom(jStr(env, jRoomId));
}

JNI_FUNC(jint, nativeDbCount)(JNIEnv*, jclass) {
    return g_eventDb.count();
}

// SqliteDB JNI (richer API with room summaries, transactions)
JNI_FUNC(jboolean, nativeSqliteDbOpen)(JNIEnv* env, jclass, jstring jPath, jstring jKey) {
    auto db = progressive::SqliteDB::open(jStr(env, jPath));
    if (!db.isOpen()) return JNI_FALSE;
    db.createTimelineSchema();
    auto key = jStr(env, jKey);
    g_sqliteDbs[key] = std::make_unique<progressive::SqliteDB>(std::move(db));
    return JNI_TRUE;
}

JNI_FUNC(void, nativeSqliteDbClose)(JNIEnv* env, jclass, jstring jKey) {
    g_sqliteDbs.erase(jStr(env, jKey));
}

JNI_FUNC(jboolean, nativeSqliteDbInsertEvent)(JNIEnv* env, jclass, jstring jKey,
    jstring jEventId, jstring jRoomId, jstring jType, jstring jSenderId,
    jstring jContentJson, jlong jOriginTs, jlong jAgeTs, jint jDi) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return JNI_FALSE;
    return it->second->insertEvent(
        jStr(env, jEventId), jStr(env, jRoomId), jStr(env, jType),
        jStr(env, jSenderId), jStr(env, jContentJson),
        jOriginTs, jAgeTs, jDi) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeSqliteDbInsertEventRel)(JNIEnv* env, jclass, jstring jKey,
    jstring jEventId, jstring jRoomId, jstring jType, jstring jSenderId,
    jstring jContentJson, jlong jOriginTs, jlong jAgeTs, jint jDi,
    jstring jStateKey, jstring jRedacts, jstring jRelType, jstring jRelatesTo) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return JNI_FALSE;
    return it->second->insertEvent(
        jStr(env, jEventId), jStr(env, jRoomId), jStr(env, jType),
        jStr(env, jSenderId), jStr(env, jContentJson),
        jOriginTs, jAgeTs, jDi,
        jStr(env, jStateKey), jStr(env, jRedacts),
        jStr(env, jRelType), jStr(env, jRelatesTo)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeSqliteDbQueryEvents)(JNIEnv* env, jclass, jstring jKey,
    jstring jRoomId, jint jLimit, jint jOffset, jboolean jAsc) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return env->NewStringUTF("[]");
    auto rows = it->second->queryEvents(jStr(env, jRoomId), jLimit, jOffset, jAsc);
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < rows.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << rows[i].eventId
           << R"(","di":)" << rows[i].displayIndex << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeSqliteDbQueryEvent)(JNIEnv* env, jclass, jstring jKey, jstring jEventId) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return env->NewStringUTF("{}");
    auto row = it->second->queryEvent(jStr(env, jEventId));
    std::ostringstream os;
    os << R"({"id":")" << row.eventId << R"(","type":")" << row.type
       << R"(","content":)" << row.contentJson << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(void, nativeSqliteDbDeleteEvent)(JNIEnv* env, jclass, jstring jKey, jstring jEventId) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it != g_sqliteDbs.end()) it->second->deleteEvent(jStr(env, jEventId));
}

JNI_FUNC(jint, nativeSqliteDbCountEvents)(JNIEnv* env, jclass, jstring jKey, jstring jRoomId) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return 0;
    return it->second->countEvents(jStr(env, jRoomId));
}

JNI_FUNC(jint, nativeSqliteDbMaxDisplayIndex)(JNIEnv* env, jclass, jstring jKey, jstring jRoomId) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return 0;
    return it->second->maxDisplayIndex(jStr(env, jRoomId));
}

JNI_FUNC(jboolean, nativeSqliteDbUpsertRoom)(JNIEnv* env, jclass, jstring jKey,
    jstring jRoomId, jstring jName, jstring jAvatar, jstring jTopic,
    jstring jMembership, jint jNotifCount, jint jHighlightCount,
    jlong jActivity, jboolean jDirect, jboolean jSpace,
    jboolean jFav, jboolean jEnc) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return JNI_FALSE;
    return it->second->upsertRoom(
        jStr(env, jRoomId), jStr(env, jName), jStr(env, jAvatar),
        jStr(env, jTopic), jStr(env, jMembership),
        jNotifCount, jHighlightCount, jActivity,
        jDirect, jSpace, jFav, jEnc) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeSqliteDbQueryRooms)(JNIEnv* env, jclass, jstring jKey) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return env->NewStringUTF("[]");
    auto rows = it->second->queryRooms();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < rows.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << rows[i].roomId
           << R"(","name":")" << rows[i].displayName
           << R"(","membership":")" << rows[i].membership
           << R"(","notif":)" << rows[i].notificationCount
           << R"(,"highlight":)" << rows[i].highlightCount
           << R"(,"direct":)" << (rows[i].isDirect ? "true" : "false") << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(void, nativeSqliteDbBeginTransaction)(JNIEnv* env, jclass, jstring jKey) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it != g_sqliteDbs.end()) it->second->beginTransaction();
}

JNI_FUNC(void, nativeSqliteDbCommitTransaction)(JNIEnv* env, jclass, jstring jKey) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it != g_sqliteDbs.end()) it->second->commitTransaction();
}

JNI_FUNC(jint, nativeSqliteDbSchemaVersion)(JNIEnv* env, jclass, jstring jKey) {
    auto it = g_sqliteDbs.find(jStr(env, jKey));
    if (it == g_sqliteDbs.end()) return 0;
    return it->second->schemaVersion();
}

// --- Native Sync Response Parser (bypass Moshi) ---
// Controlled by Labs: SETTINGS_LABS_NATIVE_SYNC_PARSER

JNI_FUNC(jstring, nativeParseSyncResponse)(JNIEnv* env, jclass, jstring jJson) {
    auto response = progressive::parseSyncResponse(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"next_batch":")" << response.nextBatch
       << R"(","account_data_count":)" << response.accountData.events.size()
       << R"(,"presence_count":)" << response.presence.events.size()
       << R"(,"to_device_count":)" << response.toDevice.events.size()
       << R"(,"rooms_join_count":)" << response.rooms.join.size()
       << R"(,"rooms_invite_count":)" << response.rooms.invite.size()
       << R"(,"rooms_leave_count":)" << response.rooms.leave.size()
       << R"(,"device_lists_changed":)" << response.deviceLists.changed.size()
       << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeGetNextBatch)(JNIEnv* env, jclass, jstring jJson) {
    auto response = progressive::parseSyncResponse(jStr(env, jJson));
    return env->NewStringUTF(response.nextBatch.c_str());
}

JNI_FUNC(jstring, nativeParseSyncRoomsJson)(JNIEnv* env, jclass, jstring jJson) {
    auto rooms = progressive::parseSyncRooms(jStr(env, jJson));
    std::ostringstream os; os << "[";
    bool first = true;
    for (auto& kv : rooms.join) {
        if (!first) os << ","; first = false;
        os << R"({"room_id":")" << kv.first << R"(","state":"join"})";
    }
    for (auto& kv : rooms.invite) {
        if (!first) os << ","; first = false;
        os << R"({"room_id":")" << kv.first << R"(","state":"invite"})";
    }
    for (auto& kv : rooms.leave) {
        if (!first) os << ","; first = false;
        os << R"({"room_id":")" << kv.first << R"(","state":"leave"})";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseEvent)(JNIEnv* env, jclass, jstring jJson) {
    auto event = progressive::parseEvent(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"event_id":")" << event.eventId
       << R"(","type":")" << event.type
       << R"(","sender":")" << event.senderId
       << R"(","room_id":")" << event.roomId
       << R"(","ts":)" << event.originServerTs
       << R"(,"state_key":")" << event.stateKey << "\""
       << R"(,"send_state":)" << static_cast<int>(event.sendState) << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseTimeline)(JNIEnv* env, jclass, jstring jJson) {
    auto timeline = progressive::parseSyncTimeline(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"events_count":)" << timeline.events.size()
       << R"(,"limited":)" << (timeline.limited ? "true" : "false")
       << R"(,"prev_batch":")" << timeline.prevToken << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jint, nativeCountEventsInSync)(JNIEnv* env, jclass, jstring jJson) {
    auto response = progressive::parseSyncResponse(jStr(env, jJson));
    int total = 0;
    total += response.accountData.events.size();
    total += response.presence.events.size();
    total += response.toDevice.events.size();
    for (auto& kv : response.rooms.join) {
        total += kv.second.state.events.size();
        total += kv.second.timeline.events.size();
    }
    return total;
}

// Full sync JSON round-trip: parse → re-serialize (validates C++ parser against Moshi)
JNI_FUNC(jstring, nativeSyncResponseRoundtrip)(JNIEnv* env, jclass, jstring jJson) {
    auto response = progressive::parseSyncResponse(jStr(env, jJson));
    auto result = progressive::syncResponseToJson(response);
    return env->NewStringUTF(result.c_str());
}

// --- Native Matrix API (login, sync, send) ---
// Controlled by Labs: SETTINGS_LABS_NATIVE_HTTP

JNI_FUNC(void, nativeSetHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl) {
    progressive::setHomeserverBaseUrl(jStr(env, jUrl));
}

JNI_FUNC(void, nativeSetAccessToken)(JNIEnv* env, jclass, jstring jToken) {
    progressive::setAccessToken(jStr(env, jToken));
}

JNI_FUNC(jstring, nativeApiLogin)(JNIEnv* env, jclass, jstring jUser, jstring jPass, jstring jDev) {
    auto creds = progressive::apiLogin(jStr(env, jUser), jStr(env, jPass), jStr(env, jDev));
    std::ostringstream os;
    os << R"({"user_id":")" << creds.userId
       << R"(","access_token":")" << creds.accessToken
       << R"(","device_id":")" << creds.deviceId
       << R"(","home_server":")" << creds.homeServer
       << R"(","success":)" << (creds.isValid() ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeApiSync)(JNIEnv* env, jclass, jstring jFilter, jstring jSince, jint jTimeout) {
    auto response = progressive::apiSync(
        jStr(env, jFilter), jStr(env, jSince), jTimeout);
    std::ostringstream os;
    os << R"({"next_batch":")" << response.nextBatch
       << R"(","rooms_joined":)" << response.rooms.join.size()
       << R"(,"rooms_invited":)" << response.rooms.invite.size()
       << R"(,"rooms_left":)" << response.rooms.leave.size()
       << R"(,"events_total_timeline":)";
    int totalTimeline = 0;
    for (auto& kv : response.rooms.join) totalTimeline += kv.second.timeline.events.size();
    for (auto& kv : response.rooms.leave) totalTimeline += kv.second.timeline.events.size();
    os << totalTimeline << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeApiSendEvent)(JNIEnv* env, jclass, jstring jRoom, jstring jType, jstring jTxn, jstring jContent) {
    auto result = progressive::apiSendEvent(
        jStr(env, jRoom), jStr(env, jType), jStr(env, jTxn), jStr(env, jContent));
    return env->NewStringUTF(result.c_str());
}

// --- Room / Profile / Account API ---

JNI_FUNC(jstring, nativeApiJoinRoom)(JNIEnv* env, jclass, jstring jRoom, jstring jReason) {
    auto result = progressive::apiJoinRoom(jStr(env, jRoom), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiLeaveRoom)(JNIEnv* env, jclass, jstring jRoom) {
    auto result = progressive::apiLeaveRoom(jStr(env, jRoom));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiGetProfile)(JNIEnv* env, jclass, jstring jUser) {
    auto result = progressive::apiGetProfile(jStr(env, jUser));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiWhoAmI)(JNIEnv* env, jclass) {
    auto result = progressive::apiWhoAmI();
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeApiLogout)(JNIEnv* env, jclass) {
    return progressive::apiLogout() ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeApiGetRoomMembers)(JNIEnv* env, jclass, jstring jRoom) {
    auto result = progressive::apiGetRoomMembers(jStr(env, jRoom));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiInviteUser)(JNIEnv* env, jclass, jstring jRoom, jstring jUser, jstring jReason) {
    auto result = progressive::apiInviteUser(jStr(env, jRoom), jStr(env, jUser), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeApiAvailable)(JNIEnv* env, jclass) {
    return progressive::nativeApiAvailable() ? JNI_TRUE : JNI_FALSE;
}

// --- Extended API ---

JNI_FUNC(jstring, nativeApiGetRoomMessages)(JNIEnv* env, jclass, jstring jRoom, jstring jFrom, jstring jDir, jint jLimit) {
    auto result = progressive::apiGetRoomMessages(
        jStr(env, jRoom), jStr(env, jFrom), jStr(env, jDir), jLimit);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiCreateRoom)(JNIEnv* env, jclass, jstring jName, jstring jTopic, jboolean jDirect, jstring jInvitees) {
    std::vector<std::string> users;
    auto inviteStr = jStr(env, jInvitees);
    if (!inviteStr.empty()) {
        size_t p = 0;
        while (p < inviteStr.size()) {
            size_t c = inviteStr.find(',', p);
            users.push_back(inviteStr.substr(p, c - p));
            if (c == std::string::npos) break;
            p = c + 1;
        }
    }
    auto result = progressive::apiCreateRoom(jStr(env, jName), jStr(env, jTopic), jDirect, users);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiSearch)(JNIEnv* env, jclass, jstring jQuery, jstring jRoom, jint jLimit) {
    auto result = progressive::apiSearch(jStr(env, jQuery), jStr(env, jRoom), jLimit);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiKickUser)(JNIEnv* env, jclass, jstring jRoom, jstring jUser, jstring jReason) {
    auto result = progressive::apiKickUser(jStr(env, jRoom), jStr(env, jUser), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiBanUser)(JNIEnv* env, jclass, jstring jRoom, jstring jUser, jstring jReason) {
    auto result = progressive::apiBanUser(jStr(env, jRoom), jStr(env, jUser), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiUnbanUser)(JNIEnv* env, jclass, jstring jRoom, jstring jUser) {
    auto result = progressive::apiUnbanUser(jStr(env, jRoom), jStr(env, jUser));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiRedactEvent)(JNIEnv* env, jclass, jstring jRoom, jstring jEvent, jstring jTxn) {
    auto result = progressive::apiRedactEvent(jStr(env, jRoom), jStr(env, jEvent), jStr(env, jTxn));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiGetPushRules)(JNIEnv* env, jclass) {
    auto result = progressive::apiGetPushRules();
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiCreateFilter)(JNIEnv* env, jclass, jstring jUser, jstring jFilter) {
    auto result = progressive::apiCreateFilter(jStr(env, jUser), jStr(env, jFilter));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiGetDisplayName)(JNIEnv* env, jclass, jstring jUser) {
    auto result = progressive::apiGetDisplayName(jStr(env, jUser));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiSetDisplayName)(JNIEnv* env, jclass, jstring jUser, jstring jName) {
    auto result = progressive::apiSetDisplayName(jStr(env, jUser), jStr(env, jName));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeApiGetVersions)(JNIEnv* env, jclass) {
    auto result = progressive::apiGetVersions();
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeApiLogoutAll)(JNIEnv* env, jclass) {
    return progressive::apiLogoutAll() ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeApiPublicRooms)(JNIEnv* env, jclass, jstring jServer, jstring jQuery, jint jLimit) {
    auto result = progressive::apiPublicRooms(jStr(env, jServer), jStr(env, jQuery), jLimit);
    return env->NewStringUTF(result.c_str());
}

// --- Display Name & Avatar Utilities ---

JNI_FUNC(jstring, nativeUserIdToDisplayName)(JNIEnv* env, jclass, jstring jUserId, jboolean jCap) {
    auto result = progressive::userIdToDisplayName(jStr(env, jUserId), jCap);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetInitials)(JNIEnv* env, jclass, jstring jName, jint jMax) {
    auto result = progressive::getInitials(jStr(env, jName), jMax);
    return env->NewStringUTF(result.c_str());
}

// --- Permalink Builder ---

JNI_FUNC(jstring, nativeBuildEventPermalink)(JNIEnv* env, jclass, jstring jRoom, jstring jEvent) {
    auto result = progressive::buildEventPermalink(jStr(env, jRoom), jStr(env, jEvent));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildRoomPermalink)(JNIEnv* env, jclass, jstring jRoom) {
    auto result = progressive::buildRoomPermalink(jStr(env, jRoom));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildUserPermalink)(JNIEnv* env, jclass, jstring jUser) {
    auto result = progressive::buildUserPermalink(jStr(env, jUser));
    return env->NewStringUTF(result.c_str());
}

// --- Media Utilities ---

// --- Presence Utilities ---

JNI_FUNC(jstring, nativeFormatPresence)(JNIEnv* env, jclass, jstring jPresence, jlong jLastActiveMs) {
    auto ps = jStr(env, jPresence);
    progressive::Presence p = progressive::Presence::Offline;
    if (ps == "online") p = progressive::Presence::Online;
    else if (ps == "unavailable") p = progressive::Presence::Unavailable;
    auto result = progressive::formatPresenceWithTime(p, jLastActiveMs);
    return env->NewStringUTF(result.c_str());
}

// --- Device Naming ---

JNI_FUNC(jstring, nativeBuildDeviceDisplayName)(JNIEnv* env, jclass, jstring jApp, jstring jModel) {
    auto result = progressive::buildDeviceDisplayName(jStr(env, jApp), jStr(env, jModel));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGenerateDeviceName)(JNIEnv* env, jclass, jstring jModel, jstring jOs) {
    auto result = progressive::generateDeviceName(jStr(env, jModel), jStr(env, jOs));
    return env->NewStringUTF(result.c_str());
}

// --- Account Validation ---

JNI_FUNC(jboolean, nativeIsValidDisplayName)(JNIEnv* env, jclass, jstring jName, jint jMax) {
    return progressive::isValidDisplayName(jStr(env, jName), jMax) ? JNI_TRUE : JNI_FALSE;
}

// --- Well-Known Server Discovery ---

JNI_FUNC(jstring, nativeParseWellKnown)(JNIEnv* env, jclass, jstring jJson) {
    auto result = progressive::parseWellKnown(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"homeserver_url":")" << result.homeServerBaseUrl
       << R"(","identity_server":")" << result.identityServerBaseUrl
       << R"(","valid":)" << (result.valid ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jboolean, nativeNeedsWellKnownDiscovery)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::needsWellKnownDiscovery(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

// --- Polls ---

JNI_FUNC(jboolean, nativeIsPollEnded)(JNIEnv* env, jclass, jlong jCloseTs) {
    return progressive::isPollEnded(jCloseTs) ? JNI_TRUE : JNI_FALSE;
}

// --- Membership ---

JNI_FUNC(jboolean, nativeCanReadMessages)(JNIEnv* env, jclass, jstring jMembership) {
    auto ms = jStr(env, jMembership);
    progressive::MemberState m = progressive::MemberState::Leave;
    if (ms == "join") m = progressive::MemberState::Join;
    else if (ms == "invite") m = progressive::MemberState::Invite;
    else if (ms == "knock") m = progressive::MemberState::Knock;
    else if (ms == "ban") m = progressive::MemberState::Ban;
    else if (ms == "leave") m = progressive::MemberState::Leave;
    return progressive::canReadMessages(m) ? JNI_TRUE : JNI_FALSE;
}

// --- Invites ---

JNI_FUNC(jstring, nativeBuildInviteBody)(JNIEnv* env, jclass, jstring jUser, jstring jReason) {
    auto result = progressive::buildInviteBody(jStr(env, jUser), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

// --- Event Validation ---

JNI_FUNC(jboolean, nativeIsBodyWithinLimits)(JNIEnv* env, jclass, jstring jBody, jint jMax) {
    return progressive::isBodyWithinLimits(jStr(env, jBody), jMax) ? JNI_TRUE : JNI_FALSE;
}

// --- Widgets ---

JNI_FUNC(jboolean, nativeIsJitsiWidget)(JNIEnv* env, jclass, jstring jType) {
    return progressive::isJitsiWidget(jStr(env, jType)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetWidgetTypeName)(JNIEnv* env, jclass, jstring jType) {
    auto result = progressive::getWidgetTypeName(jStr(env, jType));
    return env->NewStringUTF(result.c_str());
}

// --- WebRTC / Calls ---

JNI_FUNC(jboolean, nativeIsCallExpired)(JNIEnv* env, jclass, jlong jCreatedMs, jint jTimeout) {
    return progressive::isCallExpired(jCreatedMs, jTimeout) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatCallDuration)(JNIEnv* env, jclass, jint jSeconds) {
    auto result = progressive::formatCallDuration(jSeconds);
    return env->NewStringUTF(result.c_str());
}

// --- Notification Settings ---

JNI_FUNC(jstring, nativeFormatNotifMode)(JNIEnv* env, jclass, jstring jMode) {
    auto ms = jStr(env, jMode);
    progressive::NotifMode m = progressive::NotifMode::Default;
    if (ms == "all") m = progressive::NotifMode::All;
    else if (ms == "mentions") m = progressive::NotifMode::Mentions;
    else if (ms == "none") m = progressive::NotifMode::None;
    auto result = progressive::formatNotifMode(m);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeParseNotifMode)(JNIEnv* env, jclass, jstring jAction) {
    auto mode = progressive::parseNotifMode(jStr(env, jAction));
    const char* s = "default";
    if (mode == progressive::NotifMode::All) s = "all";
    else if (mode == progressive::NotifMode::Mentions) s = "mentions";
    else if (mode == progressive::NotifMode::None) s = "none";
    return env->NewStringUTF(s);
}

// --- Crypto / E2EE ---

JNI_FUNC(jboolean, nativeIsValidDeviceKey)(JNIEnv* env, jclass, jstring jKey) {
    return progressive::isValidDeviceKey(jStr(env, jKey)) ? JNI_TRUE : JNI_FALSE;
}

// --- Server Info ---

JNI_FUNC(jstring, nativeParseServerVersion)(JNIEnv* env, jclass, jstring jJson) {
    auto result = progressive::parseServerVersion(jStr(env, jJson));
    return env->NewStringUTF(result.c_str());
}

// --- Time Formatting ---

JNI_FUNC(jstring, nativeFormatTimeAgoLabel)(JNIEnv* env, jclass, jlong jTs, jlong jNow) {
    auto result = progressive::formatTimeAgoLabel(jTs, jNow);
    return env->NewStringUTF(result.c_str());
}

// --- Edit History ---

JNI_FUNC(jstring, nativeFormatEditSummary)(JNIEnv* env, jclass, jstring jOriginalBody, jstring jNewBody) {
    auto o = jStr(env, jOriginalBody);
    auto n = jStr(env, jNewBody);
    // Simple diff: if strings differ, report edit
    if (o == n) return env->NewStringUTF(n.c_str());
    std::ostringstream os;
    if (n.size() > o.size()) os << n.substr(0, 30) << "...";
    else os << n;
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeGetEditBadgeText)(JNIEnv* env, jclass, jint jEditCount) {
    auto result = progressive::getEditBadgeText(jEditCount);
    return env->NewStringUTF(result.c_str());
}

// --- Cross-Signing ---

JNI_FUNC(jboolean, nativeNeedsCrossSigningSetup)(JNIEnv* env, jclass, jstring jStatusJson) {
    // Parse status from JSON, call needsCrossSigningSetup
    auto json = jStr(env, jStatusJson);
    bool masterKeyOk = json.find("\"master_key_ok\"") != std::string::npos &&
                       json.find("\"master_key_ok\":true") != std::string::npos;
    bool selfSigningOk = json.find("\"self_signing_key_ok\"") != std::string::npos &&
                         json.find("\"self_signing_key_ok\":true") != std::string::npos;
    return (!masterKeyOk || !selfSigningOk) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatCrossSigningStatus)(JNIEnv* env, jclass, jstring jStatusJson) {
    auto json = jStr(env, jStatusJson);
    bool masterOk = json.find("\"master_ok\":true") != std::string::npos ||
                    json.find("\"master_key_ok\":true") != std::string::npos;
    bool selfOk = json.find("\"self_signing_ok\":true") != std::string::npos ||
                  json.find("\"self_signing_key_ok\":true") != std::string::npos;
    bool userOk = json.find("\"user_signing_ok\":true") != std::string::npos;
    if (masterOk && selfOk && userOk) return env->NewStringUTF("Verified");
    if (masterOk && selfOk) return env->NewStringUTF("Self-verified");
    if (masterOk) return env->NewStringUTF("Not verified");
    return env->NewStringUTF("Setup needed");
}

// --- Event Display Classification ---

JNI_FUNC(jstring, nativeGetEventTypeDescription)(JNIEnv* env, jclass, jstring jType, jstring jMsgType) {
    auto dt = progressive::classifyEvent(jStr(env, jType), jStr(env, jMsgType));
    auto result = progressive::getEventTypeDescription(dt);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetEventTypeIcon)(JNIEnv* env, jclass, jstring jType, jstring jMsgType) {
    auto dt = progressive::classifyEvent(jStr(env, jType), jStr(env, jMsgType));
    auto result = progressive::getEventTypeIcon(dt);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsContinuation)(JNIEnv* env, jclass, jstring jCurSender, jstring jPrevSender,
    jlong jCurTs, jlong jPrevTs) {
    return progressive::isContinuation(
        jStr(env, jCurSender), jStr(env, jPrevSender), jCurTs, jPrevTs) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeShouldShowAvatar)(JNIEnv* env, jclass, jstring jCurSender, jstring jPrevSender, jboolean jIsLast) {
    return progressive::shouldShowAvatar(
        jStr(env, jCurSender), jStr(env, jPrevSender), jIsLast) ? JNI_TRUE : JNI_FALSE;
}

// --- Power Levels ---

JNI_FUNC(jstring, nativeParseRoomPowerLevels)(JNIEnv* env, jclass, jstring jStateJson) {
    auto pl = progressive::parseRoomPowerLevels(jStr(env, jStateJson));
    std::ostringstream os;
    os << R"({"users_default":)" << pl.usersDefault
       << R"(,"events_default":)" << pl.eventsDefault
       << R"(,"state_default":)" << pl.stateDefault
       << R"(,"ban":)" << pl.ban << R"(,"kick":)" << pl.kick
       << R"(,"redact":)" << pl.redact << R"(,"invite":)" << pl.invite
       << R"(,"notifications_room":)" << pl.notificationsRoom << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jboolean, nativeHasPower)(JNIEnv* env, jclass, jstring jPlJson, jstring jUserId, jstring jAction) {
    auto json = jStr(env, jPlJson);
    progressive::RoomPowerLevels pl;
    pl.usersDefault = 0; pl.eventsDefault = 0; pl.stateDefault = 50;
    pl.ban = 50; pl.kick = 50; pl.redact = 50; pl.invite = 50;
    // Parse defaults from JSON
    auto extractInt = [&](const std::string& key) -> int {
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return -1;
        pos = json.find(':', pos);
        if (pos == std::string::npos) return -1;
        pos++; while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int v = 0; while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { v = v*10+(json[pos]-'0'); pos++; }
        return v;
    };
    int ud = extractInt("users_default"); if (ud >= 0) pl.usersDefault = ud;
    int ed = extractInt("events_default"); if (ed >= 0) pl.eventsDefault = ed;
    int sd = extractInt("state_default"); if (sd >= 0) pl.stateDefault = sd;
    int ba = extractInt("ban"); if (ba >= 0) pl.ban = ba;
    int ki = extractInt("kick"); if (ki >= 0) pl.kick = ki;
    int re = extractInt("redact"); if (re >= 0) pl.redact = re;
    int inv = extractInt("invite"); if (inv >= 0) pl.invite = inv;
    // Check user-specific level
    auto uid = jStr(env, jUserId);
    auto usersPos = json.find("\"users\"");
    if (usersPos != std::string::npos) {
        auto userKey = "\"" + uid + "\"";
        auto up = json.find(userKey, usersPos);
        if (up != std::string::npos) {
            up = json.find(':', up);
            if (up != std::string::npos) {
                up++; while (up < json.size() && (json[up] == ' ' || json[up] == '\t')) up++;
                int lv = 0; while (up < json.size() && json[up] >= '0' && json[up] <= '9') { lv = lv*10+(json[up]-'0'); up++; }
                pl.userOverrides[uid] = lv;
            }
        }
    }
    return progressive::hasPower(pl, uid, jStr(env, jAction)) ? JNI_TRUE : JNI_FALSE;
}

// --- SSO ---

JNI_FUNC(jboolean, nativeIsSsoCallbackUrl)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isSsoCallbackUrl(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractSsoProvider)(JNIEnv* env, jclass, jstring jIdpId) {
    auto result = progressive::extractSsoProvider(jStr(env, jIdpId));
    return env->NewStringUTF(result.c_str());
}

// --- Room Tombstone ---

JNI_FUNC(jstring, nativeParseRoomTombstoneContent)(JNIEnv* env, jclass, jstring jStateJson) {
    auto ts = progressive::parseRoomTombstoneContent(jStr(env, jStateJson));
    std::ostringstream os;
    os << R"({"replacement_room":")" << ts.replacementRoomId << R"(")"
       << R"(,"body":")" << ts.body << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Network Quality ---

JNI_FUNC(jstring, nativeClassifyNetworkQuality)(JNIEnv* env, jclass, jint jSignal, jdouble jLatency, jdouble jLossRate) {
    auto result = progressive::classifyQualityLabel(jSignal, jLatency, jLossRate);
    return env->NewStringUTF(result.c_str());
}

// --- Connection Monitor ---

JNI_FUNC(jstring, nativeFormatDowntime)(JNIEnv* env, jclass, jlong jDowntimeMs) {
    auto result = progressive::ConnectionMonitor::formatDowntime(jDowntimeMs);
    return env->NewStringUTF(result.c_str());
}

// --- Event Preview (room list last message) ---

JNI_FUNC(jstring, nativeFormatEventPreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jstring jType, jstring jMsgType, jboolean jShowSender) {
    progressive::DisplayEvent ev;
    ev.senderName = jStr(env, jSender);
    ev.body = jStr(env, jBody);
    ev.type = progressive::classifyEvent(jStr(env, jType), jStr(env, jMsgType));
    auto result = progressive::formatEventPreview(ev, jShowSender);
    return env->NewStringUTF(result.c_str());
}

// --- Room Encryption ---

JNI_FUNC(jstring, nativeParseEncryptionConfig)(JNIEnv* env, jclass, jstring jStateJson) {
    auto cfg = progressive::parseEncryptionConfig(jStr(env, jStateJson));
    std::ostringstream os;
    os << R"({"algorithm":")" << cfg.algorithm
       << R"(","rotation_period_ms":)" << cfg.rotationPeriodMs
       << R"(,"rotation_period_msgs":)" << cfg.rotationPeriodMessages
       << R"(,"enabled":)" << (cfg.enabled ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeComputeEncryptionStatus)(JNIEnv* env, jclass, jstring jAlgorithm) {
    // Simplified: just check if algorithm is set and recognized
    auto algo = jStr(env, jAlgorithm);
    if (algo.empty()) return env->NewStringUTF("Not encrypted");
    if (algo.find("megolm") != std::string::npos) return env->NewStringUTF("Encrypted (Megolm)");
    if (algo.find("olm") != std::string::npos) return env->NewStringUTF("Encrypted (Olm)");
    return env->NewStringUTF("Encrypted");
}

// --- Key Backup ---

JNI_FUNC(jstring, nativeGetBackupAlgorithmDescription)(JNIEnv* env, jclass, jstring jAlgo) {
    auto result = progressive::getBackupAlgorithmDescription(jStr(env, jAlgo));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsSupportedBackupAlgorithm)(JNIEnv* env, jclass, jstring jAlgo) {
    return progressive::isSupportedBackupAlgorithm(jStr(env, jAlgo)) ? JNI_TRUE : JNI_FALSE;
}

// --- Space Utilities ---

JNI_FUNC(jstring, nativeParseSpaceChildren)(JNIEnv* env, jclass, jstring jJson) {
    auto children = progressive::parseSpaceChildren(jStr(env, jJson));
    progressive::sortSpaceChildren(children);
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < children.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"room_id":")" << children[i].childId
           << R"(","name":")" << children[i].name
           << R"(","is_room":)" << (children[i].isRoom ? "true" : "false") << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- E2EE Decoration ---

JNI_FUNC(jstring, nativeGetE2eeIconName)(JNIEnv* env, jclass, jstring jState) {
    auto s = jStr(env, jState);
    progressive::E2eeState st = progressive::E2eeState::None;
    if (s == "verified") st = progressive::E2eeState::Verified;
    else if (s == "unverified") st = progressive::E2eeState::Unverified;
    else if (s == "warning") st = progressive::E2eeState::Warning;
    else if (s == "error") st = progressive::E2eeState::Error;
    else if (s == "blacklisted") st = progressive::E2eeState::Blacklisted;
    auto result = progressive::getE2eeIconName(st);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetE2eeColor)(JNIEnv* env, jclass, jstring jState) {
    auto s = jStr(env, jState);
    progressive::E2eeState st = progressive::E2eeState::None;
    if (s == "verified") st = progressive::E2eeState::Verified;
    else if (s == "unverified") st = progressive::E2eeState::Unverified;
    else if (s == "warning") st = progressive::E2eeState::Warning;
    else if (s == "error") st = progressive::E2eeState::Error;
    else if (s == "blacklisted") st = progressive::E2eeState::Blacklisted;
    auto result = progressive::getE2eeColor(st);
    return env->NewStringUTF(result.c_str());
}

// --- Backup Utilities ---

JNI_FUNC(jstring, nativeBuildCreateBackupBody)(JNIEnv* env, jclass, jstring jAlgo, jstring jAuth) {
    auto result = progressive::buildCreateBackupBody(jStr(env, jAlgo), jStr(env, jAuth));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatBackupStats)(JNIEnv* env, jclass, jstring jInfoJson) {
    progressive::BackupInfo info;
    auto json = jStr(env, jInfoJson);
    // Manual parse of BackupInfo from JSON
    auto extractStr = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return "";
        p = json.find(':', p); if (p == std::string::npos) return "";
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        if (p >= json.size() || json[p] != '"') return "";
        p++; size_t e = p;
        while (e < json.size() && json[e] != '"') { if (json[e] == '\\') e++; e++; }
        return json.substr(p, e - p);
    };
    auto extractInt = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return 0;
        p = json.find(':', p); if (p == std::string::npos) return 0;
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        int v = 0; while (p < json.size() && json[p] >= '0' && json[p] <= '9') { v=v*10+(json[p]-'0'); p++; }
        return v;
    };
    info.version = extractStr("version");
    info.algorithm = extractStr("algorithm");
    info.totalKeys = extractInt("total_keys");
    info.backedUpKeys = extractInt("backed_up_keys");
    auto result = progressive::formatBackupStats(info);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeNeedsBackupAttention)(JNIEnv* env, jclass, jstring jInfoJson) {
    progressive::BackupInfo info;
    auto json = jStr(env, jInfoJson);
    auto extractInt = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return 0;
        p = json.find(':', p); if (p == std::string::npos) return 0;
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        int v = 0; while (p < json.size() && json[p] >= '0' && json[p] <= '9') { v=v*10+(json[p]-'0'); p++; }
        return v;
    };
    info.totalKeys = extractInt("total_keys");
    info.backedUpKeys = extractInt("backed_up_keys");
    return progressive::needsBackupAttention(info) ? JNI_TRUE : JNI_FALSE;
}

// --- Read Marker / Notifications ---

JNI_FUNC(jstring, nativeBuildRoomNotifSettingsBody)(JNIEnv* env, jclass, jstring jMode) {
    auto ms = jStr(env, jMode);
    progressive::NotifMode m = progressive::NotifMode::Default;
    if (ms == "all") m = progressive::NotifMode::All;
    else if (ms == "mentions") m = progressive::NotifMode::Mentions;
    else if (ms == "none") m = progressive::NotifMode::None;
    auto result = progressive::buildRoomNotifSettingsBody(m);
    return env->NewStringUTF(result.c_str());
}

// --- URL Preview ---

JNI_FUNC(jboolean, nativeIsPreviewableUrl)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isPreviewableUrl(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractUrls)(JNIEnv* env, jclass, jstring jText) {
    auto urls = progressive::extractUrls(jStr(env, jText));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < urls.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << urls[i] << R"(")";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- Device Manager ---

JNI_FUNC(jstring, nativeFormatDeviceLastSeen)(JNIEnv* env, jclass, jlong jLastSeenMs) {
    auto result = progressive::formatDeviceLastSeen(jLastSeenMs);
    return env->NewStringUTF(result.c_str());
}

// --- Permalink Utilities ---

JNI_FUNC(jboolean, nativeIsSameRoomPermalink)(JNIEnv* env, jclass, jstring jUrl1, jstring jUrl2) {
    return progressive::isSameRoomPermalink(jStr(env, jUrl1), jStr(env, jUrl2)) ? JNI_TRUE : JNI_FALSE;
}

// --- Display Name (advanced) ---

JNI_FUNC(jstring, nativeGetBestDisplayName)(JNIEnv* env, jclass, jstring jName, jstring jUserId) {
    auto result = progressive::getBestDisplayName(jStr(env, jName), jStr(env, jUserId));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatMemberName)(JNIEnv* env, jclass, jstring jName, jstring jUserId, jint jPower, jboolean jShowBadge) {
    auto result = progressive::formatMemberName(jStr(env, jName), jStr(env, jUserId), jPower, jShowBadge);
    return env->NewStringUTF(result.c_str());
}

// --- Identity Utilities ---

JNI_FUNC(jboolean, nativeIsEmail)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isEmail(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsMsisdn)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isMsisdn(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractAliasLocalpart)(JNIEnv* env, jclass, jstring jAlias) {
    auto result = progressive::extractAliasLocalpart(jStr(env, jAlias));
    return env->NewStringUTF(result.c_str());
}

// --- Link Preview ---

JNI_FUNC(jboolean, nativeIsImageUrl)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isImageUrl(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

// --- Permalink Parser ---

JNI_FUNC(jstring, nativeExtractRoomIdFromPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    auto result = progressive::extractRoomIdFromPermalink(jStr(env, jUrl));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractEventIdFromPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    auto result = progressive::extractEventIdFromPermalink(jStr(env, jUrl));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractUserIdFromPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    auto result = progressive::extractUserIdFromPermalink(jStr(env, jUrl));
    return env->NewStringUTF(result.c_str());
}

// --- URL Preview ---

JNI_FUNC(jstring, nativeStripHtmlTags)(JNIEnv* env, jclass, jstring jHtml) {
    auto result = progressive::stripHtmlTags(jStr(env, jHtml));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeTruncateDescription)(JNIEnv* env, jclass, jstring jText, jint jMax) {
    auto result = progressive::truncateDescription(jStr(env, jText), jMax);
    return env->NewStringUTF(result.c_str());
}

// --- Device Type ---

JNI_FUNC(jstring, nativeClassifyDeviceType)(JNIEnv* env, jclass, jstring jAgent, jstring jClient) {
    auto result = progressive::classifyDeviceType(jStr(env, jAgent), jStr(env, jClient));
    return env->NewStringUTF(result.c_str());
}

// --- Version Comparison ---

JNI_FUNC(jint, nativeCompareSemver)(JNIEnv* env, jclass, jstring jA, jstring jB) {
    return progressive::compareSemver(jStr(env, jA), jStr(env, jB));
}

// --- Knock Reason ---

JNI_FUNC(jstring, nativeFormatKnockReason)(JNIEnv* env, jclass, jstring jReason) {
    auto result = progressive::formatKnockReason(jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

// --- Server Compatibility ---

JNI_FUNC(jboolean, nativeIsServerCompatible)(JNIEnv* env, jclass, jstring jVersion, jstring jMinRequired) {
    return progressive::isServerCompatible(jStr(env, jVersion), jStr(env, jMinRequired)) ? JNI_TRUE : JNI_FALSE;
}

// --- Device Fingerprint ---

JNI_FUNC(jstring, nativeExtractDeviceFingerprint)(JNIEnv* env, jclass, jstring jDeviceId, jstring jKeysJson) {
    auto result = progressive::extractDeviceFingerprint(jStr(env, jDeviceId), jStr(env, jKeysJson));
    return env->NewStringUTF(result.c_str());
}

// --- Event Distance ---

JNI_FUNC(jint, nativeEventDistance)(JNIEnv* env, jclass, jint jA, jint jB) {
    return progressive::eventDistance(jA, jB);
}

// --- Widget Validation ---

JNI_FUNC(jboolean, nativeIsEtherpadWidget)(JNIEnv* env, jclass, jstring jType) {
    return progressive::isEtherpadWidget(jStr(env, jType)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsValidWidgetUrl)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isValidWidgetUrl(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

// --- Encryption Defaults ---

JNI_FUNC(jstring, nativeGetDefaultEncryptionAlgorithm)(JNIEnv* env, jclass) {
    auto result = progressive::getDefaultEncryptionAlgorithm();
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeRequiresDeviceVerification)(JNIEnv* env, jclass, jstring jAlgo) {
    return progressive::requiresDeviceVerification(jStr(env, jAlgo)) ? JNI_TRUE : JNI_FALSE;
}

// --- Event Timestamp Validation ---

JNI_FUNC(jboolean, nativeIsReasonableTimestamp)(JNIEnv* env, jclass, jstring jTs, jlong jMaxFuture) {
    return progressive::isReasonableTimestamp(jStr(env, jTs), jMaxFuture) ? JNI_TRUE : JNI_FALSE;
}

// --- Read Marker / Jump to Unread ---

JNI_FUNC(jstring, nativeAdvanceReadMarker)(JNIEnv* env, jclass, jstring jRoomId, jstring jLatestEventId) {
    auto result = progressive::advanceReadMarker(jStr(env, jRoomId), jStr(env, jLatestEventId));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeReadMarkerToJson)(JNIEnv* env, jclass, jstring jLastReadEventId,
    jint jUnreadCount, jint jUnreadMentions, jint jUnreadHighlights, jboolean jHasUnread) {
    progressive::ReadMarkerState state;
    state.lastReadEventId = jStr(env, jLastReadEventId);
    state.unreadCount = jUnreadCount;
    state.unreadMentions = jUnreadMentions;
    state.unreadHighlights = jUnreadHighlights;
    state.hasUnread = jHasUnread;
    auto result = progressive::readMarkerToJson(state);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeShouldShowJumpToUnread)(JNIEnv* env, jclass, jstring jReadMarkerJson) {
    // Parse from JSON: {"unread_count":N,"has_unread":true/false,"first_unread_event_id":"..."}
    auto json = jStr(env, jReadMarkerJson);
    auto extractInt = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return 0;
        p = json.find(':', p); if (p == std::string::npos) return 0;
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        int v = 0; while (p < json.size() && json[p] >= '0' && json[p] <= '9') { v=v*10+(json[p]-'0'); p++; }
        return v;
    };
    auto extractBool = [&](const std::string& key) -> bool {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return false;
        p = json.find(':', p); if (p == std::string::npos) return false;
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        return json.compare(p, 4, "true") == 0;
    };
    progressive::ReadMarkerState state;
    state.unreadCount = extractInt("unread_count");
    state.hasUnread = extractBool("has_unread");
    state.firstUnreadEventId = jStr(env, jReadMarkerJson).substr(0, 10);  // dummy — parsed from json
    return progressive::shouldShowJumpToUnread(state) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatUnreadJumpLabel)(JNIEnv* env, jclass, jstring jReadMarkerJson) {
    progressive::ReadMarkerState state;
    auto json = jStr(env, jReadMarkerJson);
    auto extractInt = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return 0;
        p = json.find(':', p); if (p == std::string::npos) return 0;
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        int v = 0; while (p < json.size() && json[p] >= '0' && json[p] <= '9') { v=v*10+(json[p]-'0'); p++; }
        return v;
    };
    state.unreadCount = extractInt("unread_count");
    state.unreadMentions = extractInt("unread_mentions");
    state.hasUnread = state.unreadCount > 0;
    auto result = progressive::formatUnreadJumpLabel(state);
    return env->NewStringUTF(result.c_str());
}

// --- Notifications / Badge ---

JNI_FUNC(jstring, nativeFormatBadgeText)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::formatBadgeText(jCount);
    return env->NewStringUTF(result.c_str());
}

// --- Presence ---

JNI_FUNC(jstring, nativeParsePresence)(JNIEnv* env, jclass, jstring jUserId, jstring jJson) {
    auto info = progressive::parsePresence(jStr(env, jUserId), jStr(env, jJson));
    std::ostringstream os;
    const char* presenceStr = "offline";
    if (info.presence == progressive::Presence::Online) presenceStr = "online";
    else if (info.presence == progressive::Presence::Unavailable) presenceStr = "unavailable";
    os << R"({"user_id":")" << info.userId
       << R"(","presence":")" << presenceStr
       << R"(","last_active_ago_ms":)" << info.lastActiveAgoMs << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Matrix Error ---

JNI_FUNC(jlong, nativeGetRetryAfterMs)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::getRetryAfterMs(error);
}

// --- OpenID Token ---

JNI_FUNC(jstring, nativeParseOpenIdToken)(JNIEnv* env, jclass, jstring jJson) {
    auto token = progressive::parseOpenIdToken(jStr(env, jJson));
    auto result = progressive::openIdTokenToJson(token);
    return env->NewStringUTF(result.c_str());
}

// --- Notification Counts ---

JNI_FUNC(jstring, nativeFormatCombinedNotificationCount)(JNIEnv* env, jclass, jint jRooms, jint jThreads) {
    auto result = progressive::formatCombinedNotificationCount(jRooms, jThreads);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jint, nativeGetTotalUnreadCount)(JNIEnv* env, jclass, jint jRooms, jint jThreads) {
    return progressive::getTotalUnreadCount(jRooms, jThreads);
}

// --- Presence Indicator ---

JNI_FUNC(jstring, nativeGetPresenceIndicator)(JNIEnv* env, jclass, jstring jPresence) {
    auto ps = jStr(env, jPresence);
    progressive::Presence p = progressive::Presence::Offline;
    if (ps == "online") p = progressive::Presence::Online;
    else if (ps == "unavailable") p = progressive::Presence::Unavailable;
    auto result = progressive::getPresenceIndicator(p);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsPresenceStale)(JNIEnv* env, jclass, jlong jLastUpdatedMs) {
    return progressive::isPresenceStale(jLastUpdatedMs) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatStatusMessage)(JNIEnv* env, jclass, jstring jMsg, jint jMax) {
    auto result = progressive::formatStatusMessage(jStr(env, jMsg), jMax);
    return env->NewStringUTF(result.c_str());
}

// --- Space Utilities ---

JNI_FUNC(jstring, nativeBuildSpaceChildContent)(JNIEnv* env, jclass, jboolean jSuggested, jstring jOrder, jboolean jAutoJoin, jboolean jCanonical) {
    auto result = progressive::buildSpaceChildContent(jSuggested, jStr(env, jOrder), jAutoJoin, jCanonical);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildSpaceParentContent)(JNIEnv* env, jclass, jstring jParentSpaceId, jboolean jCanonical) {
    auto result = progressive::buildSpaceParentContent(jStr(env, jParentSpaceId), jCanonical);
    return env->NewStringUTF(result.c_str());
}

// --- Widget Utilities ---

JNI_FUNC(jstring, nativeParseWidgetStateContent)(JNIEnv* env, jclass, jstring jJson, jstring jWidgetId, jstring jRoomId) {
    auto widget = progressive::parseWidgetStateContent(jStr(env, jJson), jStr(env, jWidgetId), jStr(env, jRoomId));
    std::ostringstream os;
    os << R"({"widget_id":")" << widget.widgetId
       << R"(","type":")" << widget.type
       << R"(","name":")" << widget.name
       << R"(","url":")" << widget.url << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Key Share Requests ---

JNI_FUNC(jboolean, nativeShouldShareKey)(JNIEnv* env, jclass, jstring jAlgo, jboolean jHasSession, jboolean jSessionVerified, jboolean jUserTrusted) {
    return progressive::shouldShareKey(jStr(env, jAlgo), jHasSession, jSessionVerified, jUserTrusted) ? JNI_TRUE : JNI_FALSE;
}

// --- Recovery Key ---

JNI_FUNC(jstring, nativeComputeRecoveryKey)(JNIEnv* env, jclass, jstring jCurve25519Key) {
    auto result = progressive::computeRecoveryKey(jStr(env, jCurve25519Key));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractCurveKeyFromRecoveryKey)(JNIEnv* env, jclass, jstring jRecoveryKey) {
    auto result = progressive::extractCurveKeyFromRecoveryKey(jStr(env, jRecoveryKey));
    return env->NewStringUTF(result.c_str());
}

// --- Membership ---

JNI_FUNC(jstring, nativeFormatMembership)(JNIEnv* env, jclass, jstring jMembership) {
    auto ms = jStr(env, jMembership);
    progressive::MemberState m = progressive::MemberState::Leave;
    if (ms == "join") m = progressive::MemberState::Join;
    else if (ms == "invite") m = progressive::MemberState::Invite;
    else if (ms == "knock") m = progressive::MemberState::Knock;
    else if (ms == "ban") m = progressive::MemberState::Ban;
    auto result = progressive::formatMemberState(m);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsActiveMember)(JNIEnv* env, jclass, jstring jMembership) {
    auto ms = jStr(env, jMembership);
    progressive::MemberState m = progressive::MemberState::Leave;
    if (ms == "join") m = progressive::MemberState::Join;
    else if (ms == "invite") m = progressive::MemberState::Invite;
    else if (ms == "knock") m = progressive::MemberState::Knock;
    return progressive::isActiveMember(m) ? JNI_TRUE : JNI_FALSE;
}

// --- WebRTC / Calls ---

JNI_FUNC(jstring, nativeBuildCallHangupContent)(JNIEnv* env, jclass, jstring jCallId, jstring jReason) {
    auto result = progressive::buildCallHangupContent(jStr(env, jCallId), jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatCallNotification)(JNIEnv* env, jclass, jstring jCallJson) {
    // Parse CallInfo from JSON, format notification
    auto json = jStr(env, jCallJson);
    progressive::CallInfo call;
    auto extractStr = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\"");
        if (p == std::string::npos) return "";
        p = json.find(':', p); if (p == std::string::npos) return "";
        p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
        if (p >= json.size() || json[p] != '"') return "";
        p++; size_t e = p; while (e < json.size() && json[e] != '"') { if (json[e] == '\\') e++; e++; }
        return json.substr(p, e - p);
    };
    call.callId = extractStr("call_id");
    call.callerName = extractStr("caller_name");
    call.isVideo = json.find("\"is_video\":true") != std::string::npos;
    auto result = progressive::formatCallNotification(call);
    return env->NewStringUTF(result.c_str());
}

// --- Content Scanner / ToS ---

JNI_FUNC(jboolean, nativeIsServerNotice)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isServerNotice(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeMustAcceptTos)(JNIEnv* env, jclass, jstring jResponseJson) {
    return progressive::mustAcceptTos(jStr(env, jResponseJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeBuildTosAcceptBody)(JNIEnv* env, jclass, jstring jVersion) {
    auto result = progressive::buildTosAcceptBody(jStr(env, jVersion));
    return env->NewStringUTF(result.c_str());
}

// --- Login Utilities ---

JNI_FUNC(jboolean, nativeIsValidLoginCredentials)(JNIEnv* env, jclass, jstring jUserId, jstring jPassword) {
    return progressive::isValidLoginCredentials(jStr(env, jUserId), jStr(env, jPassword)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGenerateDeviceId)(JNIEnv* env, jclass) {
    auto result = progressive::generateDeviceId();
    return env->NewStringUTF(result.c_str());
}

// --- Password Validator ---

JNI_FUNC(jstring, nativeValidatePassword)(JNIEnv* env, jclass, jstring jPassword) {
    auto result = progressive::validatePassword(jStr(env, jPassword));
    std::ostringstream os;
    os << R"({"valid":)" << (result.valid ? "true" : "false")
       << R"(,"strength":)" << result.strength
       << R"(,"strength_label":")" << result.strengthLabel
       << R"(","feedback":")" << result.feedback << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jint, nativeComputePasswordStrength)(JNIEnv* env, jclass, jstring jPassword) {
    return progressive::computePasswordStrength(jStr(env, jPassword));
}

JNI_FUNC(jstring, nativeGetStrengthLabel)(JNIEnv* env, jclass, jint jStrength) {
    auto result = progressive::getStrengthLabel(jStrength);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGeneratePasswordFeedback)(JNIEnv* env, jclass, jstring jPassword) {
    auto result = progressive::generatePasswordFeedback(jStr(env, jPassword));
    return env->NewStringUTF(result.c_str());
}

// --- Event Validator ---

JNI_FUNC(jboolean, nativeIsValidEventId)(JNIEnv* env, jclass, jstring jEventId) {
    return progressive::isValidEventId(jStr(env, jEventId)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsValidSenderId)(JNIEnv* env, jclass, jstring jSenderId) {
    return progressive::isValidSenderId(jStr(env, jSenderId)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsFileSizeWithinLimits)(JNIEnv* env, jclass, jlong jFileSize, jlong jMaxSize) {
    return progressive::isFileSizeWithinLimits(jFileSize, jMaxSize) ? JNI_TRUE : JNI_FALSE;
}

// --- Invite Utilities ---

JNI_FUNC(jboolean, nativeIsInviteExpired)(JNIEnv* env, jclass, jlong jInvitedAtMs, jint jMaxDays) {
    return progressive::isInviteExpired(jInvitedAtMs, jMaxDays) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeBuildKnockBody)(JNIEnv* env, jclass, jstring jReason) {
    auto result = progressive::buildKnockBody(jStr(env, jReason));
    return env->NewStringUTF(result.c_str());
}


static progressive::OlmAccountData g_olmAccount;

__attribute__((used)) JNIEXPORT jboolean JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeOlmCreateAccount(JNIEnv* env, jclass, jstring jUserId, jstring jDeviceId) {
    g_olmAccount = progressive::createOlmAccount(jStr(env, jUserId), jStr(env, jDeviceId));
    return g_olmAccount.valid ? JNI_TRUE : JNI_FALSE;
}
__attribute__((used)) JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeOlmGetIdentityKeys(JNIEnv* env, jclass) {
    return env->NewStringUTF(progressive::getAccountIdentityKeys(g_olmAccount).c_str());
}
__attribute__((used)) JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_nativeOlmGenerateOneTimeKeys(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::generateOneTimeKeys(g_olmAccount, jCount);
    return env->NewStringUTF(result.c_str());
}
JNI_FUNC(jboolean, nativeContainsRtlOverride)(JNIEnv* env, jclass, jstring jText) {
    return progressive::containsRtlOverride(jStr(env, jText)) ? JNI_TRUE : JNI_FALSE;
}
JNI_FUNC(jboolean, nativeContainsBidiOverride)(JNIEnv* env, jclass, jstring jText) {
    return progressive::containsBidiOverride(jStr(env, jText)) ? JNI_TRUE : JNI_FALSE;
}
JNI_FUNC(jstring, nativeFilterBidiOverrides)(JNIEnv* env, jclass, jstring jText) {
    auto result = progressive::filterBidiOverrides(jStr(env, jText));
    return env->NewStringUTF(result.c_str());
}
JNI_FUNC(jstring, nativeSanitizeDisplayText)(JNIEnv* env, jclass, jstring jText) {
    auto result = progressive::sanitizeDisplayText(jStr(env, jText));
    return env->NewStringUTF(result.c_str());
}
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;
    jclass clazz = env->FindClass("chat/progressive/app/native/ProgressiveNative");
    if (!clazz) return JNI_ERR;
    JNINativeMethod methods[] = {
        {(char*)"nativeOlmCreateAccount", (char*)"(Ljava/lang/String;Ljava/lang/String;)Z", (void*)Java_chat_progressive_app_native_ProgressiveNative_nativeOlmCreateAccount},
        {(char*)"nativeOlmGetIdentityKeys", (char*)"()Ljava/lang/String;", (void*)Java_chat_progressive_app_native_ProgressiveNative_nativeOlmGetIdentityKeys},
        {(char*)"nativeOlmGenerateOneTimeKeys", (char*)"(I)Ljava/lang/String;", (void*)Java_chat_progressive_app_native_ProgressiveNative_nativeOlmGenerateOneTimeKeys},
    };
    env->RegisterNatives(clazz, methods, 3);
    return JNI_VERSION_1_6;
}
} // extern "C"
