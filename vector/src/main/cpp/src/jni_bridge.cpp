#include <jni.h>
#include <string>
#include <android/log.h>
#include "progressive/jumptodate.hpp"
#include "progressive/relation.hpp"
#include "progressive/exporter.hpp"
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
#include "progressive/megolm_decryptor.hpp"
#include "progressive/olm_session.hpp"
#include "progressive/sas_verification.hpp"
#include "progressive/markdown.hpp"
#include "progressive/room_content.hpp"
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

JNI_FUNC(jint, nativeTimelineChunkCount)(JNIEnv* env, jclass, jstring jRoom) {
    static std::unordered_map<std::string, progressive::TimelineChunkManager> managers;
    auto room = jStr(env, jRoom);
    auto it = managers.find(room);
    return it != managers.end() ? it->second.chunkCount() : 0;
}

JNI_FUNC(jstring, nativeTimelineGetSnapshot)(JNIEnv* env, jclass, jstring jRoom, jint jLimit, jint jOffset) {
    static std::unordered_map<std::string, progressive::TimelineChunkManager> managers;
    auto room = jStr(env, jRoom);
    auto it = managers.find(room);
    if (it == managers.end()) return env->NewStringUTF("[]");
    auto events = it->second.getSnapshot(jLimit, jOffset);
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < events.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << events[i].eventId
           << R"(","di":)" << events[i].displayIndex
           << R"(","ts":)" << events[i].originServerTs << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jint, nativeTimelineEventsAvailable)(JNIEnv* env, jclass, jstring jRoom, jint jDir) {
    static std::unordered_map<std::string, progressive::TimelineChunkManager> managers;
    auto room = jStr(env, jRoom);
    auto it = managers.find(room);
    if (it == managers.end()) return 0;
    auto dir = jDir == 0 ? progressive::TimelineDirection::FORWARDS : progressive::TimelineDirection::BACKWARDS;
    return it->second.eventsAvailable(dir);
}

// Store a single event from /sync into native timeline
JNI_FUNC(jint, nativeTimelineAddSyncEvent)(JNIEnv* env, jclass, jstring jRoom, jstring jEventId, jstring jType, jstring jSenderId, jstring jContentJson, jlong jOriginTs, jint jDi, jstring jStateKey, jstring jRedacts, jstring jRelType, jstring jRelatesToId) {
    static std::unordered_map<std::string, progressive::TimelineChunkManager> managers;
    auto room = jStr(env, jRoom);
    auto it = managers.find(room);
    if (it == managers.end()) {
        managers.emplace(std::piecewise_construct, std::forward_as_tuple(room), std::forward_as_tuple(room));
        it = managers.find(room);
    }
    progressive::TimelineEventData ev;
    ev.eventId = jStr(env, jEventId);
    ev.roomId = room;
    ev.type = jStr(env, jType);
    ev.senderId = jStr(env, jSenderId);
    ev.contentJson = jStr(env, jContentJson);
    ev.originServerTs = jOriginTs;
    ev.displayIndex = jDi;
    ev.stateKey = jStr(env, jStateKey);
    ev.redacts = jStr(env, jRedacts);
    ev.relationType = jStr(env, jRelType);
    ev.relatesToEventId = jStr(env, jRelatesToId);
    ev.ageLocalTs = static_cast<int64_t>(time(nullptr)) * 1000;
    return it->second.addLiveEvent(ev);
}

JNI_FUNC(jboolean, nativeTimelineAttachDb)(JNIEnv* env, jclass, jstring jRoom, jstring jDbKey) {
    static std::unordered_map<std::string, progressive::TimelineChunkManager> managers;
    static std::unordered_map<std::string, std::unique_ptr<progressive::SqliteDB>> g_sqliteDbs;
    auto room = jStr(env, jRoom);
    auto dbKey = jStr(env, jDbKey);
    auto dbit = g_sqliteDbs.find(dbKey);
    if (dbit == g_sqliteDbs.end()) return JNI_FALSE;
    auto it = managers.find(room);
    if (it == managers.end()) {
        managers.emplace(std::piecewise_construct, std::forward_as_tuple(room), std::forward_as_tuple(room));
        it = managers.find(room);
    }
    it->second.attachDatabase(dbit->second.get());
    it->second.loadFromDatabase(100, 0);
    return JNI_TRUE;
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

// Lightweight: extract next_batch from partial JSON (first 64KB)
JNI_FUNC(jstring, nativeExtractNextBatchLight)(JNIEnv* env, jclass, jstring jPartialJson) {
    auto json = jStr(env, jPartialJson);
    auto pos = json.find("\"next_batch\"");
    if (pos == std::string::npos) return env->NewStringUTF("");
    pos = json.find(':', pos);
    if (pos == std::string::npos) return env->NewStringUTF("");
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '"')) pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') { if (json[end] == '\\') end++; end++; }
    return env->NewStringUTF(json.substr(pos, end - pos).c_str());
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
    progressive::Membership m = progressive::Membership::Leave;
    if (ms == "join") m = progressive::Membership::Join;
    else if (ms == "invite") m = progressive::Membership::Invite;
    else if (ms == "knock") m = progressive::Membership::Knock;
    else if (ms == "ban") m = progressive::Membership::Ban;
    else if (ms == "leave") m = progressive::Membership::Leave;
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

JNI_FUNC(jstring, nativeExtractHtmlTitle)(JNIEnv* env, jclass, jstring jHtml) {
    auto result = progressive::extractHtmlTitle(jStr(env, jHtml));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractMetaDescription)(JNIEnv* env, jclass, jstring jHtml) {
    auto result = progressive::extractMetaDescription(jStr(env, jHtml));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeResolveUrl)(JNIEnv* env, jclass, jstring jBase, jstring jRel) {
    auto result = progressive::resolveUrl(jStr(env, jBase), jStr(env, jRel));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeUrlPreviewToJson)(JNIEnv* env, jclass, jstring jPreviewJson) {
    auto json = jStr(env, jPreviewJson);
    progressive::UrlPreview p;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto ei = [&](const std::string& k) -> int64_t {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return 0;
        pp = json.find(':', pp); if (pp == std::string::npos) return 0;
        pp++; while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0; while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
        return v;
    };
    p.url = es("url"); p.title = es("title"); p.description = es("description");
    p.imageUrl = es("image_url"); p.siteName = es("site_name"); p.type = es("type");
    p.imageWidth = ei("image_width"); p.imageHeight = ei("image_height");
    p.hasImage = json.find("\"has_image\":true") != std::string::npos;
    p.hasTitle = json.find("\"has_title\":true") != std::string::npos;
    p.valid = json.find("\"valid\":true") != std::string::npos;
    auto result = progressive::urlPreviewToJson(p);
    return env->NewStringUTF(result.c_str());
}

// --- Web Search ---

JNI_FUNC(jstring, nativeBuildSearxngUrl)(JNIEnv* env, jclass, jstring jEndpoint, jstring jQuery, jint jMax) {
    auto result = progressive::buildSearxngUrl(jStr(env, jEndpoint), jStr(env, jQuery), jMax);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildDuckDuckGoUrl)(JNIEnv* env, jclass, jstring jQuery) {
    auto result = progressive::buildDuckDuckGoUrl(jStr(env, jQuery));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildGoogleUrl)(JNIEnv* env, jclass, jstring jApiKey, jstring jEngineId, jstring jQuery, jint jMax) {
    auto result = progressive::buildGoogleUrl(jStr(env, jApiKey), jStr(env, jEngineId), jStr(env, jQuery), jMax);
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
    progressive::Membership m = progressive::Membership::Leave;
    if (ms == "join") m = progressive::Membership::Join;
    else if (ms == "invite") m = progressive::Membership::Invite;
    else if (ms == "knock") m = progressive::Membership::Knock;
    else if (ms == "ban") m = progressive::Membership::Ban;
    auto result = progressive::formatMembership(m);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsActiveMember)(JNIEnv* env, jclass, jstring jMembership) {
    auto ms = jStr(env, jMembership);
    progressive::Membership m = progressive::Membership::Leave;
    if (ms == "join") m = progressive::Membership::Join;
    else if (ms == "invite") m = progressive::Membership::Invite;
    else if (ms == "knock") m = progressive::Membership::Knock;
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

// --- Device Manager ---

JNI_FUNC(jstring, nativeFormatFingerprint)(JNIEnv* env, jclass, jstring jFingerprint) {
    auto result = progressive::formatFingerprint(jStr(env, jFingerprint));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsDeviceInactive)(JNIEnv* env, jclass, jlong jLastSeenMs) {
    return progressive::isDeviceInactive(jLastSeenMs) ? JNI_TRUE : JNI_FALSE;
}

// --- Version Compatibility ---

JNI_FUNC(jboolean, nativeSatisfiesMinVersion)(JNIEnv* env, jclass, jstring jCurrent, jstring jMinimum) {
    return progressive::satisfiesMinVersion(jStr(env, jCurrent), jStr(env, jMinimum)) ? JNI_TRUE : JNI_FALSE;
}

// --- Federation Version ---

JNI_FUNC(jstring, nativeParseFederationVersion)(JNIEnv* env, jclass, jstring jJson) {
    auto version = progressive::parseFederationVersion(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"name":")" << version.name
       << R"(","version":")" << version.version << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Report Utilities ---

JNI_FUNC(jboolean, nativeIsValidReportReason)(JNIEnv* env, jclass, jstring jReason) {
    return progressive::isValidReportReason(jStr(env, jReason)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetReasonDescription)(JNIEnv* env, jclass, jstring jCode) {
    auto result = progressive::getReasonDescription(jStr(env, jCode));
    return env->NewStringUTF(result.c_str());
}

// --- Secret Storage ---

JNI_FUNC(jstring, nativeExtractDefaultSecretKey)(JNIEnv* env, jclass, jstring jAccountDataJson) {
    auto result = progressive::extractDefaultSecretKey(jStr(env, jAccountDataJson));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeHasCrossSigningSecrets)(JNIEnv* env, jclass, jstring jAccountDataJson) {
    return progressive::hasCrossSigningSecrets(jStr(env, jAccountDataJson)) ? JNI_TRUE : JNI_FALSE;
}

// --- Report / Rageshake ---

JNI_FUNC(jboolean, nativeIsOffensive)(JNIEnv* env, jclass, jint jScore) {
    return progressive::isOffensive(jScore) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeTruncateReportDescription)(JNIEnv* env, jclass, jstring jDesc, jint jMax) {
    auto result = progressive::truncateReportDescription(jStr(env, jDesc), jMax);
    return env->NewStringUTF(result.c_str());
}

// --- Content Scanner ---

JNI_FUNC(jboolean, nativeIsContentScannerAvailable)(JNIEnv* env, jclass, jstring jCapabilitiesJson) {
    return progressive::isContentScannerAvailable(jStr(env, jCapabilitiesJson)) ? JNI_TRUE : JNI_FALSE;
}

// --- Matrix Error ---

JNI_FUNC(jboolean, nativeIsPasswordError)(JNIEnv* env, jclass, jstring jErrorCode) {
    return progressive::isPasswordError(jStr(env, jErrorCode)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetAllErrorCodes)(JNIEnv* env, jclass) {
    auto codes = progressive::getAllErrorCodes();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < codes.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << codes[i] << R"(")";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- Call Content Builders ---

JNI_FUNC(jstring, nativeBuildCallInviteContent)(JNIEnv* env, jclass, jstring jCallId, jboolean jVideo, jstring jSdp, jint jLifetime) {
    auto result = progressive::buildCallInviteContent(jStr(env, jCallId), jVideo, jStr(env, jSdp), jLifetime);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildCallAnswerContent)(JNIEnv* env, jclass, jstring jCallId, jstring jSdp) {
    auto result = progressive::buildCallAnswerContent(jStr(env, jCallId), jStr(env, jSdp));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetCallState)(JNIEnv* env, jclass, jstring jContentJson) {
    auto result = progressive::getCallState(jStr(env, jContentJson));
    return env->NewStringUTF(result.c_str());
}

// --- Room State (Join Rules, History, Guests, Create, Tombstone) ---
// Critical for room settings UI: who can join, history visibility, upgrades

JNI_FUNC(jboolean, nativeIsPublicRoom)(JNIEnv* env, jclass, jstring jStateJson) {
    auto rules = progressive::parseJoinRules(jStr(env, jStateJson));
    return progressive::isPublicRoom(rules) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsInviteOnly)(JNIEnv* env, jclass, jstring jStateJson) {
    auto rules = progressive::parseJoinRules(jStr(env, jStateJson));
    return progressive::isInviteOnly(rules) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeJoinRuleToString)(JNIEnv* env, jclass, jstring jStateJson) {
    auto rules = progressive::parseJoinRules(jStr(env, jStateJson));
    auto result = progressive::joinRuleToString(rules.rule);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsHistoryPubliclyVisible)(JNIEnv* env, jclass, jstring jStateJson) {
    auto vis = progressive::parseHistoryVisibility(jStr(env, jStateJson));
    return progressive::isHistoryPubliclyVisible(vis) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeHistoryVisibilityToString)(JNIEnv* env, jclass, jstring jStateJson) {
    auto vis = progressive::parseHistoryVisibility(jStr(env, jStateJson));
    auto result = progressive::historyVisibilityToString(vis.visibility);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeAreGuestsAllowed)(JNIEnv* env, jclass, jstring jStateJson) {
    auto access = progressive::parseGuestAccess(jStr(env, jStateJson));
    return progressive::areGuestsAllowed(access) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsRoomUpgraded)(JNIEnv* env, jclass, jstring jStateJson) {
    auto tombstone = progressive::parseRoomTombstoneContent(jStr(env, jStateJson));
    return !tombstone.replacementRoomId.empty() ? JNI_TRUE : JNI_FALSE;
}

// --- Matrix Pattern Validators (ID validation, link parsing, URL checks) ---
// Critical: used everywhere — before API calls, for linkification, input validation

JNI_FUNC(jboolean, nativeIsUserId)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isUserId(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsRoomId)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isRoomId(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsRoomAlias)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isRoomAlias(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsEventId)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isEventId(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsMxcUrl)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isMxcUrl(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsPhoneNumber)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isPhoneNumber(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsValidEmail)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isValidEmail(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractServerNameFromId)(JNIEnv* env, jclass, jstring jMxid) {
    auto result = progressive::extractServerNameFromId(jStr(env, jMxid));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractUserNameFromId)(JNIEnv* env, jclass, jstring jMxid) {
    auto result = progressive::extractUserNameFromId(jStr(env, jMxid));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeCandidateAliasFromRoomName)(JNIEnv* env, jclass, jstring jName, jstring jDomain, jint jMaxLen) {
    auto result = progressive::candidateAliasFromRoomName(jStr(env, jName), jStr(env, jDomain), jMaxLen);
    return env->NewStringUTF(result.c_str());
}

// --- Widget List ---

JNI_FUNC(jstring, nativeListRoomWidgets)(JNIEnv* env, jclass, jstring jStateJson) {
    auto widgets = progressive::listRoomWidgets(jStr(env, jStateJson));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < widgets.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"widget_id":")" << widgets[i].widgetId
           << R"(","name":")" << widgets[i].name
           << R"(","type":")" << widgets[i].type
           << R"(","url":")" << widgets[i].url << "\"}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- Session Rename ---

JNI_FUNC(jstring, nativeBuildSessionRenameBody)(JNIEnv* env, jclass, jstring jSessionId, jstring jNewName) {
    auto result = progressive::buildSessionRenameBody(jStr(env, jSessionId), jStr(env, jNewName));
    return env->NewStringUTF(result.c_str());
}

// --- Permalink Detection ---

JNI_FUNC(jboolean, nativeIsMatrixToPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isMatrixToPermalink(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsAppPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isAppPermalink(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsValidOrderString)(JNIEnv* env, jclass, jstring jOrder) {
    return progressive::isValidOrderString(jStr(env, jOrder)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsGroupId)(JNIEnv* env, jclass, jstring jInput) {
    return progressive::isGroupId(jStr(env, jInput)) ? JNI_TRUE : JNI_FALSE;
}

// --- Device Name ---

JNI_FUNC(jstring, nativeParseDeviceName)(JNIEnv* env, jclass, jstring jUserAgent) {
    auto result = progressive::parseDeviceName(jStr(env, jUserAgent));
    return env->NewStringUTF(result.c_str());
}

// --- Matrix ID Extraction ---

JNI_FUNC(jstring, nativeExtractMatrixIds)(JNIEnv* env, jclass, jstring jText) {
    auto ids = progressive::extractMatrixIds(jStr(env, jText));
    std::ostringstream os; os << R"({"user_ids":[)";
    for (size_t i = 0; i < ids.userIds.size(); i++) {
        if (i > 0) os << ","; os << R"(")" << ids.userIds[i] << R"(")";
    }
    os << R"(],"room_ids":[)";
    for (size_t i = 0; i < ids.roomIds.size(); i++) {
        if (i > 0) os << ","; os << R"(")" << ids.roomIds[i] << R"(")";
    }
    os << R"(],"room_aliases":[)";
    for (size_t i = 0; i < ids.roomAliases.size(); i++) {
        if (i > 0) os << ","; os << R"(")" << ids.roomAliases[i] << R"(")";
    }
    os << R"(],"event_ids":[)";
    for (size_t i = 0; i < ids.eventIds.size(); i++) {
        if (i > 0) os << ","; os << R"(")" << ids.eventIds[i] << R"(")";
    }
    os << "]}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseMatrixToPermalink)(JNIEnv* env, jclass, jstring jUrl) {
    auto info = progressive::parseMatrixToPermalink(jStr(env, jUrl));
    std::ostringstream os;
    os << R"({"type":")" << info.type
       << R"(","user_id":")" << info.userId
       << R"(","room_id":")" << info.roomId
       << R"(","event_id":")" << info.eventId
       << R"(,"valid":)" << (info.valid ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Login Flows ---

JNI_FUNC(jstring, nativeParseLoginFlowsList)(JNIEnv* env, jclass, jstring jJson) {
    auto flows = progressive::parseLoginFlowsList(jStr(env, jJson));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < flows.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"type":")" << flows[i].type
           << R"(","description":")" << flows[i].description
           << R"(,"supported":)" << (flows[i].isSupported ? "true" : "false") << "}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeBuildUserIdentifier)(JNIEnv* env, jclass, jstring jUserId) {
    auto result = progressive::buildUserIdentifier(jStr(env, jUserId));
    return env->NewStringUTF(result.c_str());
}

// --- Notification Mode ---

JNI_FUNC(jboolean, nativeIsNotifModeDifferent)(JNIEnv* env, jclass, jstring jOld, jstring jNew) {
    auto parse = [](const std::string& s) -> progressive::NotifMode {
        if (s == "all") return progressive::NotifMode::All;
        if (s == "mentions") return progressive::NotifMode::Mentions;
        if (s == "none") return progressive::NotifMode::None;
        return progressive::NotifMode::Default;
    };
    return progressive::isNotifModeDifferent(parse(jStr(env, jOld)), parse(jStr(env, jNew))) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetDefaultModeForRoom)(JNIEnv* env, jclass, jboolean jDirect, jboolean jEncrypted) {
    auto mode = progressive::getDefaultModeForRoom(jDirect, jEncrypted);
    const char* s = "default";
    if (mode == progressive::NotifMode::All) s = "all";
    else if (mode == progressive::NotifMode::Mentions) s = "mentions";
    else if (mode == progressive::NotifMode::None) s = "none";
    return env->NewStringUTF(s);
}

// --- Password Strength ---

JNI_FUNC(jboolean, nativeMeetsMinimumRequirements)(JNIEnv* env, jclass, jstring jPassword) {
    return progressive::meetsMinimumRequirements(jStr(env, jPassword)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jint, nativeCountCharClasses)(JNIEnv* env, jclass, jstring jPassword) {
    return progressive::countCharClasses(jStr(env, jPassword));
}

JNI_FUNC(jboolean, nativeIsCommonPassword)(JNIEnv* env, jclass, jstring jPassword) {
    return progressive::isCommonPassword(jStr(env, jPassword)) ? JNI_TRUE : JNI_FALSE;
}

// --- SSO Utilities ---

JNI_FUNC(jstring, nativeBuildSsoLoginUrl)(JNIEnv* env, jclass, jstring jBaseUrl, jstring jRedirectUrl) {
    auto result = progressive::buildSsoLoginUrl(jStr(env, jBaseUrl), jStr(env, jRedirectUrl));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetSsoProviderBrand)(JNIEnv* env, jclass, jstring jProvider) {
    auto result = progressive::getSsoProviderBrand(jStr(env, jProvider));
    return env->NewStringUTF(result.c_str());
}

// --- Trust Label ---

JNI_FUNC(jstring, nativeGetTrustLabel)(JNIEnv* env, jclass, jstring jLevel) {
    auto s = jStr(env, jLevel);
    progressive::TrustLevel level = progressive::TrustLevel::Unknown;
    if (s == "verified") level = progressive::TrustLevel::Verified;
    else if (s == "warning") level = progressive::TrustLevel::Warning;
    else if (s == "blacklisted") level = progressive::TrustLevel::Blacklisted;
    auto result = progressive::getTrustLabel(level);
    return env->NewStringUTF(result.c_str());
}

// --- MXC URL Utilities ---

JNI_FUNC(jboolean, nativeIsMxcUri)(JNIEnv* env, jclass, jstring jUrl) {
    return progressive::isMxcUri(jStr(env, jUrl)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractMxcServerName)(JNIEnv* env, jclass, jstring jMxcUrl) {
    auto result = progressive::extractMxcServerName(jStr(env, jMxcUrl));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractMxcMediaId)(JNIEnv* env, jclass, jstring jMxcUrl) {
    auto result = progressive::extractMxcMediaId(jStr(env, jMxcUrl));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildMxcUri)(JNIEnv* env, jclass, jstring jServer, jstring jMediaId) {
    auto result = progressive::buildMxcUri(jStr(env, jServer), jStr(env, jMediaId));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeResolveMxcDownloadUrl)(JNIEnv* env, jclass, jstring jMxcUrl, jstring jHomeServer) {
    auto result = progressive::resolveMxcDownloadUrl(jStr(env, jMxcUrl), jStr(env, jHomeServer));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeHasTextWithImage)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::hasTextWithImage(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

// --- MXC Thumbnail ---

JNI_FUNC(jstring, nativeResolveMxcThumbnailUrl)(JNIEnv* env, jclass, jstring jMxcUrl, jstring jHomeServer, jint jW, jint jH) {
    auto result = progressive::resolveMxcThumbnailUrl(jStr(env, jMxcUrl), jStr(env, jHomeServer), jW, jH);
    return env->NewStringUTF(result.c_str());
}

// --- Content Utilities ---

JNI_FUNC(jstring, nativeGetExtensionFromMimeType)(JNIEnv* env, jclass, jstring jMime) {
    auto result = progressive::getExtensionFromMimeType(jStr(env, jMime));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractUsefulTextFromReply)(JNIEnv* env, jclass, jstring jRepliedBody) {
    auto result = progressive::extractUsefulTextFromReply(jStr(env, jRepliedBody));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatSpoilerTextFromHtml)(JNIEnv* env, jclass, jstring jHtml) {
    auto result = progressive::formatSpoilerTextFromHtml(jStr(env, jHtml));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetLatestEditEventId)(JNIEnv* env, jclass, jstring jEditSummaryJson, jstring jOriginalEventId) {
    auto result = progressive::getLatestEditEventId(jStr(env, jEditSummaryJson), jStr(env, jOriginalEventId));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetEditedTargetEventId)(JNIEnv* env, jclass, jstring jContentJson) {
    auto result = progressive::getEditedTargetEventId(jStr(env, jContentJson));
    return env->NewStringUTF(result.c_str());
}

// --- User Status ---

JNI_FUNC(jstring, nativeBuildUserStatusJson)(JNIEnv* env, jclass, jstring jStatus, jstring jEmoji, jlong jNowMs) {
    auto result = progressive::buildUserStatusJson(jStr(env, jStatus), jStr(env, jEmoji), jNowMs);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetPresenceStatusText)(JNIEnv* env, jclass, jboolean jOnline, jlong jLastActiveMs) {
    auto result = progressive::getPresenceStatusText(jOnline, jLastActiveMs);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetStatusSuggestions)(JNIEnv* env, jclass) {
    auto suggestions = progressive::getStatusSuggestions();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < suggestions.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << suggestions[i] << R"(")";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- Markdown Renderer ---
// Critical: called for every message body display. Opt-in via SETTINGS_LABS_NATIVE_MARKDOWN

JNI_FUNC(jstring, nativeMarkdownToHtml)(JNIEnv* env, jclass, jstring jMarkdown, jboolean jTables, jboolean jLinks, jboolean jCode, jboolean jScroll) {
    progressive::MdConfig config;
    config.enableTables = jTables;
    config.enableLinks = jLinks;
    config.enableCodeBlocks = jCode;
    config.enableHorizontalScroll = jScroll;
    config.enableImages = false;
    auto result = progressive::markdownToHtml(jStr(env, jMarkdown), config);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeParseMarkdownTable)(JNIEnv* env, jclass, jstring jTableBlock, jboolean jWithScroll) {
    auto result = progressive::parseMarkdownTable(jStr(env, jTableBlock), jWithScroll);
    return env->NewStringUTF(result.c_str());
}

// --- Event Relations (critical: every reaction/edit/reply/thread depends on this) ---

JNI_FUNC(jboolean, nativeIsReply)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isReply(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsEdit)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isEdit(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsReaction)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isReaction(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsThreadRoot)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isThreadRoot(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeExtractThreadRoot)(JNIEnv* env, jclass, jstring jContentJson) {
    auto result = progressive::extractThreadRoot(jStr(env, jContentJson));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractReplySource)(JNIEnv* env, jclass, jstring jContentJson) {
    auto result = progressive::extractReplySource(jStr(env, jContentJson));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeExtractEditSource)(JNIEnv* env, jclass, jstring jContentJson) {
    auto result = progressive::extractEditSource(jStr(env, jContentJson));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeBuildReplyRelationWithThread)(JNIEnv* env, jclass, jstring jEventId, jstring jThreadRoot) {
    auto result = progressive::buildReplyRelationWithThread(jStr(env, jEventId), jStr(env, jThreadRoot));
    return env->NewStringUTF(result.c_str());
}

// --- Room Content Parsers (room name, topic, avatar — displayed in every room header) ---

JNI_FUNC(jstring, nativeParseRoomNameContent)(JNIEnv* env, jclass, jstring jContentJson) {
    auto content = progressive::parseRoomNameContent(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"name":")" << content.name << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseRoomTopicContent)(JNIEnv* env, jclass, jstring jContentJson) {
    auto content = progressive::parseRoomTopicContent(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"topic":")" << content.topic << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseRoomAvatarContent)(JNIEnv* env, jclass, jstring jContentJson) {
    auto content = progressive::parseRoomAvatarContent(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"avatar_url":")" << content.avatarUrl << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Connection Monitor ---

JNI_FUNC(jstring, nativeGetBannerColor)(JNIEnv* env, jclass, jlong jDowntimeMs) {
    auto result = progressive::ConnectionMonitor::getBannerColor(jDowntimeMs);
    return env->NewStringUTF(result.c_str());
}

// --- Content Guard (emoji/DoS protection) ---

JNI_FUNC(jint, nativeCountEmojis)(JNIEnv* env, jclass, jstring jText) {
    return progressive::countEmojis(jStr(env, jText));
}

JNI_FUNC(jint, nativeCountUniqueEmojis)(JNIEnv* env, jclass, jstring jText) {
    return progressive::countUniqueEmojis(jStr(env, jText));
}

JNI_FUNC(jstring, nativeFormatMediaCollapseLabel)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::formatMediaCollapseLabel(jCount);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsEmojiCodePoint)(JNIEnv* env, jclass, jint jCodepoint) {
    return progressive::isEmojiCodePoint(jCodepoint) ? JNI_TRUE : JNI_FALSE;
}

// --- MIME Normalization ---

JNI_FUNC(jstring, nativeNormalizeMimeType)(JNIEnv* env, jclass, jstring jMime) {
    auto result = progressive::normalizeMimeType(jStr(env, jMime));
    return env->NewStringUTF(result.c_str());
}

// --- Room State Parse+Serialize (JSON in → struct → JSON out) ---

JNI_FUNC(jstring, nativeParseJoinRules)(JNIEnv* env, jclass, jstring jContentJson) {
    auto rules = progressive::parseJoinRules(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"rule":")" << progressive::joinRuleToString(rules.rule) << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseHistoryVisibility)(JNIEnv* env, jclass, jstring jContentJson) {
    auto vis = progressive::parseHistoryVisibility(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"visibility":")" << progressive::historyVisibilityToString(vis.visibility) << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeParseGuestAccess)(JNIEnv* env, jclass, jstring jContentJson) {
    auto access = progressive::parseGuestAccess(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"access":")" << progressive::guestAccessToString(access.access) << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Push Rules ---

JNI_FUNC(jboolean, nativeIsKnownPushRuleKind)(JNIEnv* env, jclass, jstring jKind) {
    return progressive::isKnownPushRuleKind(jStr(env, jKind)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetRuleKindDescription)(JNIEnv* env, jclass, jstring jKind, jboolean jEnabled) {
    auto result = progressive::getRuleKindDescription(jStr(env, jKind), jEnabled);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsMsc3061SharedKey)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::isMsc3061SharedKey(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatMsc3061Status)(JNIEnv* env, jclass, jboolean jShared, jstring jVisibility) {
    auto result = progressive::formatMsc3061Status(jShared, jStr(env, jVisibility));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeCanShareHistory)(JNIEnv* env, jclass, jstring jVisibility) {
    return progressive::canShareHistory(jStr(env, jVisibility)) ? JNI_TRUE : JNI_FALSE;
}

// --- Poll Utilities ---

JNI_FUNC(jstring, nativeGeneratePollOptionId)(JNIEnv* env, jclass) {
    auto result = progressive::generatePollOptionId();
    return env->NewStringUTF(result.c_str());
}

// --- Identity Utilities ---

JNI_FUNC(jstring, nativeDisambiguateName)(JNIEnv* env, jclass, jstring jDisplayName, jstring jMxid) {
    auto result = progressive::disambiguateName(jStr(env, jDisplayName), jStr(env, jMxid));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeGetIdentityInitials)(JNIEnv* env, jclass, jstring jName) {
    auto result = progressive::getIdentityInitials(jStr(env, jName));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsCanonicalAlias)(JNIEnv* env, jclass, jstring jAlias, jstring jExpectedRoomId) {
    return progressive::isCanonicalAlias(jStr(env, jAlias), jStr(env, jExpectedRoomId)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeSuggestAliases)(JNIEnv* env, jclass, jstring jRoomName) {
    auto aliases = progressive::suggestAliases(jStr(env, jRoomName));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < aliases.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << aliases[i] << R"(")";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- Presence Info (JSON round-trip) ---

JNI_FUNC(jstring, nativeParsePresenceInfo)(JNIEnv* env, jclass, jstring jUserId, jstring jJson) {
    auto info = progressive::parsePresence(jStr(env, jUserId), jStr(env, jJson));
    auto line = progressive::formatPresenceLine(info);
    return env->NewStringUTF(line.c_str());
}

// --- Backup Info (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseBackupInfo)(JNIEnv* env, jclass, jstring jJson) {
    auto info = progressive::parseBackupInfo(jStr(env, jJson));
    auto result = progressive::formatBackupStats(info);
    return env->NewStringUTF(result.c_str());
}

// --- Cross-Signing Status (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseCrossSigningStatus)(JNIEnv* env, jclass, jstring jJson, jstring jUserId) {
    auto status = progressive::parseCrossSigningStatus(jStr(env, jJson), jStr(env, jUserId));
    auto result = progressive::formatCrossSigningStatus(status);
    return env->NewStringUTF(result.c_str());
}

// --- Key Backup Version (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseKeyBackupVersion)(JNIEnv* env, jclass, jstring jJson) {
    auto backup = progressive::parseKeyBackupVersion(jStr(env, jJson));
    auto result = progressive::keyBackupVersionToJson(backup);
    return env->NewStringUTF(result.c_str());
}

// --- Device List (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseDeviceList)(JNIEnv* env, jclass, jstring jApiResponseJson, jstring jCurrentDeviceId) {
    auto stats = progressive::parseDeviceList(jStr(env, jApiResponseJson), jStr(env, jCurrentDeviceId));
    auto result = progressive::deviceListToJson(stats);
    return env->NewStringUTF(result.c_str());
}

// --- Room Permissions (JSON round-trip) ---

JNI_FUNC(jstring, nativeComputePermissions)(JNIEnv* env, jclass, jstring jPlJson, jstring jMyUserId) {
    auto pl = progressive::parseRoomPowerLevels(jStr(env, jPlJson));
    auto perms = progressive::computePermissions(pl, jStr(env, jMyUserId));
    auto result = progressive::permissionsToJson(perms);
    return env->NewStringUTF(result.c_str());
}

// --- Room Tombstone (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseTombstone)(JNIEnv* env, jclass, jstring jJson) {
    auto tombstone = progressive::parseTombstone(jStr(env, jJson));
    auto result = progressive::tombstoneToJson(tombstone);
    return env->NewStringUTF(result.c_str());
}

// --- Content Scanner (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseScanResult)(JNIEnv* env, jclass, jstring jJson) {
    auto result = progressive::parseScanResult(jStr(env, jJson));
    return env->NewStringUTF(progressive::formatScanResult(result).c_str());
}

// --- Server Notice (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseServerNotice)(JNIEnv* env, jclass, jstring jJson, jstring jEventId) {
    auto notice = progressive::parseServerNotice(jStr(env, jJson), jStr(env, jEventId));
    auto result = progressive::formatServerNotice(notice);
    return env->NewStringUTF(result.c_str());
}

// --- Member List (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseMemberList)(JNIEnv* env, jclass, jstring jRoomId, jstring jJson, jboolean jTruncated) {
    auto list = progressive::parseMemberList(jStr(env, jRoomId), jStr(env, jJson), jTruncated);
    std::ostringstream os;
    os << R"({"room_id":")" << list.roomId
       << R"(","total":)" << list.totalMembers
       << R"(,"joined":)" << list.joinedMembers
       << R"(,"invited":)" << list.invitedMembers
       << R"(,"banned":)" << list.bannedMembers
       << R"(,"truncated":)" << (list.isTruncated ? "true" : "false")
       << R"(,"members":[)";
    for (size_t i = 0; i < list.members.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"user_id":")" << list.members[i].userId
           << R"(","display_name":")" << list.members[i].displayName << "\"}";
    }
    os << "]}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Public Room (JSON round-trip) ---

JNI_FUNC(jstring, nativeParsePublicRoom)(JNIEnv* env, jclass, jstring jJson) {
    auto room = progressive::parsePublicRoom(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"room_id":")" << room.roomId
       << R"(","name":")" << room.name
       << R"(","topic":")" << room.topic
       << R"(,"members":)" << room.numJoinedMembers
       << R"(,"world_readable":)" << (room.worldReadable ? "true" : "false")
       << R"(,"guest_can_join":)" << (room.guestCanJoin ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Event Relation Info (JSON round-trip) ---

JNI_FUNC(jstring, nativeParseEventRelation)(JNIEnv* env, jclass, jstring jContentJson) {
    auto rel = progressive::parseEventRelation(jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"rel_type":")" << rel.relType
       << R"(","event_id":")" << rel.eventId
       << R"(","key":")" << rel.key << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Public Rooms Response ---

JNI_FUNC(jstring, nativeParsePublicRoomsResponse)(JNIEnv* env, jclass, jstring jJson) {
    auto resp = progressive::parsePublicRoomsResponse(jStr(env, jJson));
    std::ostringstream os;
    os << R"({"total":)" << resp.totalRoomCountEstimate
       << R"(,"next_batch":")" << resp.nextBatch
       << R"(","rooms":[)";
    for (size_t i = 0; i < resp.chunk.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"room_id":")" << resp.chunk[i].roomId
           << R"(","name":")" << resp.chunk[i].name << "\"}";
    }
    os << "]}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Thread Summary ---

JNI_FUNC(jstring, nativeComputeThreadSummary)(JNIEnv* env, jclass, jstring jRootEventId, jstring jEventsJson) {
    auto rootId = jStr(env, jRootEventId);
    progressive::ThreadSummary summary;
    summary.rootEventId = rootId;
    summary.replyCount = 0;
    auto json = jStr(env, jEventsJson);
    size_t pos = 0;
    while ((pos = json.find("\"m.thread\"", pos)) != std::string::npos ||
           (pos = json.find("\"m.in_reply_to\"", pos)) != std::string::npos) {
        summary.replyCount++;
        pos++;
    }
    auto result = progressive::threadSummaryToJson(summary);
    return env->NewStringUTF(result.c_str());
}

// --- Relation Description ---

JNI_FUNC(jstring, nativeFormatRelationDescription)(JNIEnv* env, jclass, jstring jRelType, jstring jEventId, jstring jKey) {
    progressive::EventRelationInfo rel;
    rel.relType = jStr(env, jRelType);
    rel.eventId = jStr(env, jEventId);
    rel.key = jStr(env, jKey);
    auto result = progressive::formatRelationDescription(rel);
    return env->NewStringUTF(result.c_str());
}

// --- Content Scanner ---

JNI_FUNC(jstring, nativeBuildScanRequestBody)(JNIEnv* env, jclass, jstring jMxcUri) {
    auto result = progressive::buildScanRequestBody(jStr(env, jMxcUri));
    return env->NewStringUTF(result.c_str());
}

// --- Event Content Parser ---

JNI_FUNC(jstring, nativeParseEventContent)(JNIEnv* env, jclass, jstring jEventType, jstring jContentJson) {
    auto content = progressive::parseEventContent(jStr(env, jEventType), jStr(env, jContentJson));
    std::ostringstream os;
    os << R"({"event_type":")" << content.eventType
       << R"(","msg_type":")" << content.msgType
       << R"(","body":")" << content.body
       << R"(,"file_size":)" << content.fileSize
       << R"(,"duration_ms":)" << content.durationMs << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Canonical JSON ---

JNI_FUNC(jstring, nativeCanonicalizeJson)(JNIEnv* env, jclass, jstring jJson) {
    auto result = progressive::canonicalizeJson(jStr(env, jJson));
    return env->NewStringUTF(result.c_str());
}

// --- Chunked Uploader ---
static progressive::ChunkedUploader g_uploader;

JNI_FUNC(void, nativeUploaderSetChunkSizeMb)(JNIEnv* env, jclass, jint jMb) {
    g_uploader.setChunkSizeMb(jMb);
}

JNI_FUNC(jint, nativeUploaderComputeChunks)(JNIEnv* env, jclass, jlong jFileSize) {
    return g_uploader.computeChunks(jFileSize);
}

JNI_FUNC(jstring, nativeUploaderGetChunkInfo)(JNIEnv* env, jclass, jint jIndex) {
    auto chunk = g_uploader.getChunkInfo(jIndex);
    std::ostringstream os;
    os << R"({"offset":)" << chunk.offset
       << R"(,"size":)" << chunk.chunkSize
       << R"(,"total_size":)" << chunk.totalSize
       << R"(,"index":)" << chunk.chunkIndex
       << R"(,"total":)" << chunk.totalChunks
       << R"(,"is_last":)" << (chunk.isLast ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jstring, nativeUploaderContentRange)(JNIEnv* env, jclass, jint jIndex) {
    auto chunk = g_uploader.getChunkInfo(jIndex);
    auto result = progressive::ChunkedUploader::formatContentRange(chunk);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(void, nativeUploaderAdvance)(JNIEnv*, jclass) { g_uploader.advanceChunk(); }
JNI_FUNC(void, nativeUploaderCancel)(JNIEnv*, jclass) { g_uploader.cancel(); }
JNI_FUNC(void, nativeUploaderReset)(JNIEnv*, jclass) { g_uploader.reset(); }

JNI_FUNC(jstring, nativeUploaderProgress)(JNIEnv* env, jclass) {
    auto p = g_uploader.progress();
    std::ostringstream os;
    os << R"({"uploaded":)" << p.bytesUploaded
       << R"(,"total":)" << p.totalBytes
       << R"(,"chunks":)" << p.chunksCompleted
       << R"(,"total_chunks":)" << p.totalChunks
       << R"(,"done":)" << (p.done ? "true" : "false")
       << R"(,"progress":)" << p.progress() << "}";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jint, nativeSuggestChunkSizeMb)(JNIEnv* env, jclass, jlong jFileSize) {
    return progressive::ChunkedUploader::suggestChunkSizeMb(jFileSize);
}

// --- Thread List Builder ---

JNI_FUNC(jstring, nativeBuildThreadListJson)(JNIEnv* env, jclass, jstring jEventsJson) {
    auto result = progressive::buildThreadListJson(jStr(env, jEventsJson));
    return env->NewStringUTF(result.c_str());
}

// --- Thread Unread Counter ---

JNI_FUNC(jstring, nativeComputeThreadUnreadCount)(JNIEnv* env, jclass, jstring jEventIdsJson, jstring jReadReceiptId, jstring jHighlightIdsJson) {
    auto parseStrArray = [&](const std::string& json) -> std::vector<std::string> {
        std::vector<std::string> v;
        size_t p = 0;
        while ((p = json.find('"', p)) != std::string::npos) {
            p++; size_t e = p;
            while (e < json.size() && json[e] != '"') e++;
            if (e > p) v.push_back(json.substr(p, e - p));
            p = e + 1;
        }
        return v;
    };
    auto eventIds = parseStrArray(jStr(env, jEventIdsJson));
    auto highlightIds = parseStrArray(jStr(env, jHighlightIdsJson));
    auto readId = jStr(env, jReadReceiptId);

    auto result = progressive::computeThreadUnreadCount(eventIds, readId, highlightIds);
    std::ostringstream os;
    os << R"({"total":)" << result.totalReplies << R"(,"unread":)" << result.unreadReplies
       << R"(,"highlight":)" << result.highlightReplies << R"(,"has_unread":)" << (result.hasUnread ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Sync Filter Builder ---

JNI_FUNC(jstring, nativeBuildSyncFilter)(JNIEnv* env, jclass, jboolean jThreads, jboolean jPresence, jint jTimelineLimit, jboolean jLazyLoad) {
    progressive::SyncFilter filter;
    filter.includeThreads = jThreads;
    filter.includePresence = jPresence;
    filter.timelineLimit = jTimelineLimit;
    filter.lazyLoadMembers = jLazyLoad;
    auto result = progressive::buildSyncFilter(filter);
    return env->NewStringUTF(result.c_str());
}

// --- Read Receipt Aggregator ---

JNI_FUNC(jstring, nativeFormatReceiptAccessibility)(JNIEnv* env, jclass, jstring jReceiptsJson, jint jOverflow) {
    auto json = jStr(env, jReceiptsJson);
    std::vector<progressive::ReceiptEntry> entries;
    // Parse JSON array of {userId, displayName, avatarUrl, timestamp}
    size_t p = 0;
    while ((p = json.find("\"displayName\"", p)) != std::string::npos) {
        progressive::ReceiptEntry e;
        p = json.rfind('{', p); if (p == std::string::npos) break;
        int depth = 1; size_t s = p; p++;
        while (p < json.size() && depth > 0) {
            if (json[p] == '{') depth++; else if (json[p] == '}') depth--;
            p++;
        }
        std::string entryJson = json.substr(s, p - s);
        // Extract displayName
        auto dn = entryJson.find("\"displayName\"");
        if (dn != std::string::npos) { dn = entryJson.find('"', dn+13); dn++; size_t d=dn; while(d<entryJson.size()&&entryJson[d]!='"')d++; e.displayName=entryJson.substr(dn,d-dn); }
        entries.push_back(e);
    }
    auto result = progressive::formatReceiptAccessibility(entries, jOverflow);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatOverflowLabel)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::formatOverflowLabel(jCount);
    return env->NewStringUTF(result.c_str());
}

// --- Space Hierarchy ---

JNI_FUNC(jstring, nativeSearchSpaceChildren)(JNIEnv* env, jclass, jstring jChildrenJson, jstring jQuery) {
    auto children = progressive::parseSpaceChildren(jStr(env, jChildrenJson));
    auto filtered = progressive::searchSpaceChildren(children, jStr(env, jQuery));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < filtered.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"child_id":")" << filtered[i].childId
           << R"(","name":")" << filtered[i].name << "\"}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

// --- 3PID Manager ---

JNI_FUNC(jstring, nativeParseThreePid)(JNIEnv* env, jclass, jstring jInput) {
    auto pid = progressive::parseThreePid(jStr(env, jInput));
    std::ostringstream os;
    os << R"({"medium":")" << pid.medium
       << R"(","address":")" << pid.address << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Presence Aggregator ---

JNI_FUNC(jstring, nativeFormatPresenceAggregation)(JNIEnv* env, jclass, jstring jUsersJson, jint jMaxNames) {
    auto json = jStr(env, jUsersJson);
    std::vector<std::string> names;
    size_t p = 0;
    while ((p = json.find('"', p)) != std::string::npos) {
        p++; size_t e = p;
        while (e < json.size() && json[e] != '"') e++;
        if (e > p) names.push_back(json.substr(p, e - p));
        p = e + 1;
    }
    std::ostringstream os;
    int total = static_cast<int>(names.size());
    int shown = std::min(total, jMaxNames);
    for (int i = 0; i < shown; i++) {
        if (i > 0) os << (i == shown - 1 && total <= jMaxNames ? " and " : ", ");
        os << names[i];
    }
    if (total > jMaxNames) os << " and " << (total - shown) << " others";
    os << (total == 1 ? " is online" : " are online");
    return env->NewStringUTF(os.str().c_str());
}

// --- Reaction Aggregator ---

JNI_FUNC(jstring, nativeFormatReactionAggregation)(JNIEnv* env, jclass, jstring jKey, jint jCount, jstring jReactorsJson) {
    auto key = jStr(env, jKey);
    auto json = jStr(env, jReactorsJson);
    std::vector<std::string> names;
    size_t p = 0;
    while ((p = json.find('"', p)) != std::string::npos) {
        p++; size_t e = p;
        while (e < json.size() && json[e] != '"') e++;
        if (e > p) names.push_back(json.substr(p, e - p));
        p = e + 1;
    }
    // Format: "👍 5" or "👍 Alice, Bob and 3 others"
    std::ostringstream os;
    os << key << " " << jCount;
    if (!names.empty()) {
        os << " (";
        for (size_t i = 0; i < std::min(names.size(), size_t(3)); i++) {
            if (i > 0) os << ", ";
            os << names[i];
        }
        if (names.size() > 3) os << " and " << (names.size() - 3) << " others";
        os << ")";
    }
    return env->NewStringUTF(os.str().c_str());
}

// --- Poll Response Tracker ---

JNI_FUNC(jstring, nativeTrackPollResponse)(JNIEnv* env, jclass, jstring jOptionId, jstring jUserId) {
    std::ostringstream os;
    os << R"({"option_id":")" << jStr(env, jOptionId)
       << R"(","user_id":")" << jStr(env, jUserId)
       << R"(","recorded":true})";
    return env->NewStringUTF(os.str().c_str());
}

// --- Audio / Voice ---

JNI_FUNC(jboolean, nativeIsSupportedAudioType)(JNIEnv* env, jclass, jstring jMime) {
    return progressive::isSupportedAudioType(jStr(env, jMime)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeFormatPositionInfo)(JNIEnv* env, jclass, jlong jPos, jlong jDur) {
    auto result = progressive::formatPositionInfo(jPos, jDur);
    return env->NewStringUTF(result.c_str());
}

// --- Direct Message Map ---

JNI_FUNC(jstring, nativeParseDirectMessageMap)(JNIEnv* env, jclass, jstring jJson) {
    auto dmMap = progressive::parseDirectMessageMap(jStr(env, jJson));
    auto result = progressive::buildDirectMessageMapJson(dmMap);
    return env->NewStringUTF(result.c_str());
}

// --- Edit History ---

JNI_FUNC(jstring, nativeGetEditCountBadge)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::getEditCountBadge(jCount);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeComputeEditDiffSummary)(JNIEnv* env, jclass, jstring jOld, jstring jNew) {
    auto result = progressive::computeEditDiffSummary(jStr(env, jOld), jStr(env, jNew));
    return env->NewStringUTF(result.c_str());
}

// --- Notification State (Element Web algorithm) ---

JNI_FUNC(jstring, nativeComputeNotificationState)(JNIEnv* env, jclass, jstring jRoomJson) {
    progressive::RoomListItem room;
    auto json = jStr(env, jRoomJson);
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
    room.notificationCount = extractInt("notification_count");
    room.highlightCount = extractInt("highlight_count");
    room.isMuted = extractBool("is_muted");

    auto state = progressive::computeNotificationState(room);
    auto result = progressive::notificationStateToJson(state);
    return env->NewStringUTF(result.c_str());
}

// --- Room List Search ---

JNI_FUNC(jstring, nativeSearchRoomList)(JNIEnv* env, jclass, jstring jRoomsJson, jstring jQuery) {
    auto json = jStr(env, jRoomsJson);
    std::vector<progressive::RoomListItem> rooms;
    // Parse JSON array of rooms
    size_t p = 0;
    while ((p = json.find("\"roomId\"", p)) != std::string::npos) {
        p = json.rfind('{', p); if (p == std::string::npos) break;
        int depth = 1; size_t s = p; p++;
        while (p < json.size() && depth > 0) {
            if (json[p] == '{') depth++; else if (json[p] == '}') depth--;
            p++;
        }
        std::string rj = json.substr(s, p - s);
        progressive::RoomListItem room;
        auto es = [&](const std::string& key) -> std::string {
            auto pp = rj.find("\"" + key + "\""); if (pp == std::string::npos) return "";
            pp = rj.find('"', pp + key.size() + 2); if (pp == std::string::npos) return "";
            pp++; size_t e = pp; while (e < rj.size() && rj[e] != '"') e++;
            return rj.substr(pp, e - pp);
        };
        room.roomId = es("roomId");
        room.name = es("name");
        rooms.push_back(room);
    }
    auto results = progressive::searchRoomList(rooms, jStr(env, jQuery));
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < results.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"room_id":")" << results[i].roomId
           << R"(","name":")" << results[i].name << "\"}";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}
    // Format: "Alice, Bob and 3 others online"
    std::ostringstream os;
    int total = static_cast<int>(names.size());
    int shown = std::min(total, jMaxNames);
    for (int i = 0; i < shown; i++) {
        if (i > 0) os << (i == shown - 1 && total <= jMaxNames ? " and " : ", ");
        os << names[i];
    }
    if (total > jMaxNames) os << " and " << (total - shown) << " others";
    os << (total == 1 ? " is online" : " are online");
    return env->NewStringUTF(os.str().c_str());
}
        return v;
    };
    auto eventIds = parseStrArray(jStr(env, jEventIdsJson));
    auto highlightIds = parseStrArray(jStr(env, jHighlightIdsJson));
    auto readId = jStr(env, jReadReceiptId);

    auto result = progressive::computeThreadUnreadCount(eventIds, readId, highlightIds);
    std::ostringstream os;
    os << R"({"total":)" << result.totalReplies
       << R"(,"unread":)" << result.unreadReplies
       << R"(,"highlight":)" << result.highlightReplies
       << R"(,"has_unread":)" << (result.hasUnread ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Event Classifier ---

JNI_FUNC(jboolean, nativeIsStateEvent)(JNIEnv* env, jclass, jstring jEventType) {
    return progressive::isStateEvent(jStr(env, jEventType)) ? JNI_TRUE : JNI_FALSE;
}

// --- Poll Results ---

JNI_FUNC(jstring, nativeComputePollResults)(JNIEnv* env, jclass, jstring jPollJson) {
    auto json = jStr(env, jPollJson);
    progressive::PollResult result;
    // ... (implementation in previous commit)
    auto qPos = json.find("\"question\"");
    if (qPos != std::string::npos) {
        qPos = json.find('"', qPos + 10);
        if (qPos != std::string::npos) {
            qPos++; size_t e = qPos;
            while (e < json.size() && json[e] != '"') e++;
            result.question = json.substr(qPos, e - qPos);
        }
    }
    result.isEnded = json.find("\"closed\":true") != std::string::npos;
    auto optsPos = json.find("\"options\"");
    if (optsPos != std::string::npos) {
        optsPos = json.find('[', optsPos);
        if (optsPos != std::string::npos) {
            optsPos++;
            while (optsPos < json.size()) {
                while (optsPos < json.size() && json[optsPos] != '{') { optsPos++; if (json[optsPos] == ']') break; }
                if (optsPos >= json.size() || json[optsPos] == ']') break;
                int depth = 1; size_t start = optsPos; optsPos++;
                while (optsPos < json.size() && depth > 0) {
                    if (json[optsPos] == '{') depth++;
                    else if (json[optsPos] == '}') depth--;
                    optsPos++;
                }
                std::string optJson = json.substr(start, optsPos - start);
                progressive::PollOption opt;
                auto idP = optJson.find("\"id\"");
                if (idP != std::string::npos) {
                    idP = optJson.find('"', idP + 4);
                    if (idP != std::string::npos) { idP++; size_t e=idP; while(e<optJson.size()&&optJson[e]!='"')e++; opt.id=optJson.substr(idP,e-idP); }
                }
                auto txP = optJson.find("\"text\"");
                if (txP != std::string::npos) {
                    txP = optJson.find('"', txP + 6);
                    if (txP != std::string::npos) { txP++; size_t e=txP; while(e<optJson.size()&&optJson[e]!='"')e++; opt.text=optJson.substr(txP,e-txP); }
                }
                if (!opt.id.empty()) {
                    size_t vp = 0;
                    while ((vp = json.find(opt.id, vp)) != std::string::npos) { opt.voteCount++; vp++; }
                    opt.voteCount--;
                    if (opt.voteCount < 0) opt.voteCount = 0;
                }
                result.totalVotes += opt.voteCount;
                result.options.push_back(opt);
            }
        }
    }
    int maxVotes = 0;
    for (auto& opt : result.options) {
        if (opt.voteCount > maxVotes) maxVotes = opt.voteCount;
        opt.percentage = result.totalVotes > 0 ? (opt.voteCount * 100.0 / result.totalVotes) : 0.0;
    }
    for (auto& opt : result.options) {
        if (opt.voteCount == maxVotes && maxVotes > 0) { opt.isWinner = true; result.winnerId = opt.id; result.winnerText = opt.text; }
    }
    std::ostringstream os;
    os << R"({"question":")" << result.question << R"(","total_votes":)" << result.totalVotes
       << R"(,"is_ended":)" << (result.isEnded ? "true" : "false") << R"(,"winner":")" << result.winnerText << R"(","options":[)";
    for (size_t i = 0; i < result.options.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << result.options[i].id << R"(","text":")" << result.options[i].text
           << R"(","votes":)" << result.options[i].voteCount << R"(,"percent":)" << result.options[i].percentage
           << R"(,"winner":)" << (result.options[i].isWinner ? "true" : "false") << "}";
    }
    os << "]}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Location Sharing ---
static progressive::LocationSharingManager g_locationSharing;

JNI_FUNC(jstring, nativeLocationStartSession)(JNIEnv* env, jclass, jstring jSessionId, jstring jRoomId, jstring jUserId, jint jInterval) {
    progressive::LocationSession session;
    session.sessionId = jStr(env, jSessionId);
    session.roomId = jStr(env, jRoomId);
    session.userId = jStr(env, jUserId);
    session.intervalSeconds = jInterval;
    session.active = true;
    session.startedAtMs = static_cast<int64_t>(time(nullptr)) * 1000;
    auto result = g_locationSharing.startSession(session);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(void, nativeLocationStopSession)(JNIEnv* env, jclass, jstring jSessionId) {
    g_locationSharing.stopSession(jStr(env, jSessionId));
}

JNI_FUNC(jboolean, nativeLocationIsDue)(JNIEnv* env, jclass, jstring jSessionId) {
    return g_locationSharing.isDue(jStr(env, jSessionId)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeLocationExportJson)(JNIEnv* env, jclass) {
    auto result = g_locationSharing.exportJson();
    return env->NewStringUTF(result.c_str());
}

// --- AI Agent ---

JNI_FUNC(jboolean, nativeAgentHasToolCalls)(JNIEnv* env, jclass, jstring jResponse) {
    return progressive::hasToolCalls(jStr(env, jResponse)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeAgentExtractTextAnswer)(JNIEnv* env, jclass, jstring jResponse) {
    auto result = progressive::extractTextAnswer(jStr(env, jResponse));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeAgentGetToolsSchema)(JNIEnv* env, jclass) {
    auto result = progressive::getAgentToolsSchema();
    return env->NewStringUTF(result.c_str());
}

// --- Notification Formatting ---

JNI_FUNC(jstring, nativeFormatThreadNotificationCount)(JNIEnv* env, jclass, jint jCount, jint jHighlight) {
    auto result = progressive::formatThreadNotificationCount(jCount, jHighlight);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatUnreadCounter)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::formatUnreadCounter(jCount);
    return env->NewStringUTF(result.c_str());
}

// --- Push Notification Evaluator ---

JNI_FUNC(jstring, nativeEvaluatePushNotification)(JNIEnv* env, jclass, jstring jEventJson, jstring jRulesJson, jstring jDisplayName, jstring jMyUserId) {
    auto rules = progressive::parsePushRules(jStr(env, jRulesJson));
    auto result = progressive::evaluatePushNotification(
        jStr(env, jEventJson), rules, jStr(env, jDisplayName), jStr(env, jMyUserId));
    std::ostringstream os;
    os << R"({"notify":)" << (result.shouldNotify ? "true" : "false")
       << R"(,"highlight":)" << (result.shouldHighlight ? "true" : "false")
       << R"(,"noisy":)" << (result.isNoisy ? "true" : "false")
       << R"(,"rule":")" << result.matchedRuleId << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Room Upgrade ---

JNI_FUNC(jstring, nativeProcessRoomUpgrade)(JNIEnv* env, jclass, jstring jTombstoneJson) {
    auto info = progressive::processRoomUpgrade(jStr(env, jTombstoneJson));
    std::ostringstream os;
    os << R"({"is_upgrade":)" << (info.isUpgrade ? "true" : "false")
       << R"(,"successor":")" << info.successorRoomId
       << R"(,"notice":")" << progressive::formatUpgradeNotice(info) << "\"}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Redaction ---

JNI_FUNC(jstring, nativeFormatRedactionNotice)(JNIEnv* env, jclass, jstring jReason, jboolean jSelf, jboolean jState) {
    auto result = progressive::formatRedactionNotice(jStr(env, jReason), jSelf, jState);
    return env->NewStringUTF(result.c_str());
}

// --- Key Backup ---

JNI_FUNC(jstring, nativeValidateAndFormatRecoveryKey)(JNIEnv* env, jclass, jstring jRawKey) {
    auto result = progressive::validateAndFormatRecoveryKey(jStr(env, jRawKey));
    return env->NewStringUTF(result.c_str());
}
    }
    result.isEnded = json.find("\"closed\":true") != std::string::npos;

    // Parse options and count votes
    auto optsPos = json.find("\"options\"");
    if (optsPos != std::string::npos) {
        optsPos = json.find('[', optsPos);
        if (optsPos != std::string::npos) {
            optsPos++;
            while (optsPos < json.size()) {
                while (optsPos < json.size() && json[optsPos] != '{') { optsPos++; if (json[optsPos] == ']') break; }
                if (optsPos >= json.size() || json[optsPos] == ']') break;
                int depth = 1; size_t start = optsPos; optsPos++;
                while (optsPos < json.size() && depth > 0) {
                    if (json[optsPos] == '{') depth++;
                    else if (json[optsPos] == '}') depth--;
                    optsPos++;
                }
                std::string optJson = json.substr(start, optsPos - start);
                progressive::PollOption opt;
                // Extract id
                auto idP = optJson.find("\"id\"");
                if (idP != std::string::npos) {
                    idP = optJson.find('"', idP + 4);
                    if (idP != std::string::npos) { idP++; size_t e=idP; while(e<optJson.size()&&optJson[e]!='"')e++; opt.id=optJson.substr(idP,e-idP); }
                }
                // Extract text
                auto txP = optJson.find("\"text\"");
                if (txP != std::string::npos) {
                    txP = optJson.find('"', txP + 6);
                    if (txP != std::string::npos) { txP++; size_t e=txP; while(e<optJson.size()&&optJson[e]!='"')e++; opt.text=optJson.substr(txP,e-txP); }
                }
                // Count votes (responses referencing this option ID)
                if (!opt.id.empty()) {
                    size_t vp = 0;
                    while ((vp = json.find(opt.id, vp)) != std::string::npos) { opt.voteCount++; vp++; }
                    opt.voteCount--; // Remove self-reference
                    if (opt.voteCount < 0) opt.voteCount = 0;
                }
                result.totalVotes += opt.voteCount;
                result.options.push_back(opt);
            }
        }
    }

    // Find winner
    int maxVotes = 0;
    for (auto& opt : result.options) {
        if (opt.voteCount > maxVotes) maxVotes = opt.voteCount;
        opt.percentage = result.totalVotes > 0 ? (opt.voteCount * 100.0 / result.totalVotes) : 0.0;
    }
    for (auto& opt : result.options) {
        if (opt.voteCount == maxVotes && maxVotes > 0) {
            opt.isWinner = true;
            result.winnerId = opt.id;
            result.winnerText = opt.text;
        }
    }

    // Serialize
    std::ostringstream os;
    os << R"({"question":")" << result.question
       << R"(","total_votes":)" << result.totalVotes
       << R"(,"is_ended":)" << (result.isEnded ? "true" : "false")
       << R"(,"winner":")" << result.winnerText
       << R"(","options":[)";
    for (size_t i = 0; i < result.options.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << result.options[i].id
           << R"(","text":")" << result.options[i].text
           << R"(","votes":)" << result.options[i].voteCount
           << R"(,"percent":)" << result.options[i].percentage
           << R"(,"winner":)" << (result.options[i].isWinner ? "true" : "false") << "}";
    }
    os << "]}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Member Notice / Call Notice / Edit Annotation ---

JNI_FUNC(jstring, nativeFormatMemberNotice)(JNIEnv* env, jclass, jstring jMembership, jstring jPrevMembership, jstring jSenderId, jstring jSenderName, jstring jTargetId, jstring jTargetName, jstring jReason, jboolean jDirect, jboolean jSelf) {
    auto result = progressive::formatMemberNotice(jStr(env, jMembership), jStr(env, jPrevMembership), jStr(env, jSenderId), jStr(env, jSenderName), jStr(env, jTargetId), jStr(env, jTargetName), jStr(env, jReason), jDirect, jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatCallNotice)(JNIEnv* env, jclass, jstring jEventType, jboolean jVideo, jstring jSender, jboolean jSelf) {
    auto result = progressive::formatCallNotice(jStr(env, jEventType), jVideo, jStr(env, jSender), jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeAnnotateEdited)(JNIEnv* env, jclass, jstring jBody, jboolean jEdited) {
    auto result = progressive::annotateEdited(jStr(env, jBody), jEdited);
    return env->NewStringUTF(result.c_str());
}

// --- Room State Notices ---

JNI_FUNC(jstring, nativeFormatRoomNameNotice)(JNIEnv* env, jclass, jstring jName, jstring jNewName, jboolean jSelf) {
    auto result = progressive::formatRoomNameNotice(jStr(env, jName), jStr(env, jNewName), jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomTopicNotice)(JNIEnv* env, jclass, jstring jName, jstring jTopic, jboolean jSelf) {
    auto result = progressive::formatRoomTopicNotice(jStr(env, jName), jStr(env, jTopic), jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomAvatarNotice)(JNIEnv* env, jclass, jstring jName, jboolean jRemoved, jboolean jSelf) {
    auto result = progressive::formatRoomAvatarNotice(jStr(env, jName), jRemoved, jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomCreateNotice)(JNIEnv* env, jclass, jstring jName, jstring jPredecessor, jboolean jDirect, jboolean jSelf) {
    auto result = progressive::formatRoomCreateNotice(jStr(env, jName), jStr(env, jPredecessor), jDirect, jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomTombstoneNotice)(JNIEnv* env, jclass, jstring jName, jstring jReplacement, jboolean jSelf) {
    auto result = progressive::formatRoomTombstoneNotice(jStr(env, jName), jStr(env, jReplacement), jSelf);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomEncryptionNotice)(JNIEnv* env, jclass, jstring jName, jboolean jEnabled, jboolean jSelf) {
    auto result = progressive::formatRoomEncryptionNotice(jStr(env, jName), jEnabled, jSelf);
    return env->NewStringUTF(result.c_str());
}

// --- Power Level Diff ---

JNI_FUNC(jstring, nativeFormatPowerLevelDiff)(JNIEnv* env, jclass, jstring jSender, jstring jOldJson, jstring jNewJson, jstring jNamesJson, jboolean jSelf) {
    // Parse JSON maps
    auto parseMap = [&](const std::string& json) -> std::unordered_map<std::string, int> {
        std::unordered_map<std::string, int> m;
        size_t p = 0;
        while ((p = json.find("\"@", p)) != std::string::npos) {
            p++; size_t e = p;
            while (e < json.size() && json[e] != '"') e++;
            std::string uid = json.substr(p, e - p);
            p = json.find(':', e); if (p == std::string::npos) break;
            p++; while (p < json.size() && (json[p] == ' ' || json[p] == '\t')) p++;
            int v = 0; while (p < json.size() && json[p] >= '0' && json[p] <= '9') { v = v*10+(json[p]-'0'); p++; }
            m[uid] = v;
        }
        return m;
    };
    auto oldLevels = parseMap(jStr(env, jOldJson));
    auto newLevels = parseMap(jStr(env, jNewJson));

    // Parse names
    std::unordered_map<std::string, std::string> names;
    auto namesJson = jStr(env, jNamesJson);
    size_t np = 0;
    while ((np = namesJson.find("\"@", np)) != std::string::npos) {
        np++; size_t ne = np;
        while (ne < namesJson.size() && namesJson[ne] != '"') ne++;
        std::string uid = namesJson.substr(np, ne - np);
        np = namesJson.find(':', ne); if (np == std::string::npos) break;
        np++; while (np < namesJson.size() && (namesJson[np] == ' ' || namesJson[np] == '\t' || namesJson[np] == '"')) np++;
        size_t nne = np;
        while (nne < namesJson.size() && namesJson[nne] != '"') nne++;
        names[uid] = namesJson.substr(np, nne - np);
        np = nne;
    }

    auto result = progressive::formatPowerLevelDiff(jStr(env, jSender), oldLevels, newLevels, names, jSelf);
    return env->NewStringUTF(result.c_str());
}
    auto result = progressive::threadSummaryToJson(summary);
    return env->NewStringUTF(result.c_str());
}

// --- Poll Validation ---

JNI_FUNC(jboolean, nativeIsValidPollQuestion)(JNIEnv* env, jclass, jstring jQuestion) {
    return progressive::isValidPollQuestion(jStr(env, jQuestion)) ? JNI_TRUE : JNI_FALSE;
}

// --- Room Uploads ---

JNI_FUNC(jboolean, nativeIsStickerEvent)(JNIEnv* env, jclass, jstring jEventType) {
    return progressive::isStickerEvent(jStr(env, jEventType)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeHasAttachmentUrl)(JNIEnv* env, jclass, jstring jContentJson) {
    return progressive::hasAttachmentUrl(jStr(env, jContentJson)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeCreateUploadsFilterJson)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::createUploadsFilterJson(jCount);
    return env->NewStringUTF(result.c_str());
}

// --- Matrix Error ---

JNI_FUNC(jstring, nativeGetErrorDescription)(JNIEnv* env, jclass, jstring jCode) {
    auto result = progressive::getErrorDescription(jStr(env, jCode));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeIsPasswordError)(JNIEnv* env, jclass, jstring jCode) {
    return progressive::isPasswordError(jStr(env, jCode)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeGetAllErrorCodes)(JNIEnv* env, jclass) {
    auto codes = progressive::getAllErrorCodes();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < codes.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << codes[i] << "\"";
    }
    os << "]";
    return env->NewStringUTF(os.str().c_str());
}

JNI_FUNC(jboolean, nativeIsRateLimitError)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::isRateLimitError(error) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jlong, nativeGetRetryAfterMs)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::getRetryAfterMs(error);
}

JNI_FUNC(jboolean, nativeIsSoftLogout)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::isSoftLogout(error) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeNeedsConsent)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::needsConsent(error) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jboolean, nativeIsUserDeactivated)(JNIEnv* env, jclass, jstring jErrorJson) {
    auto error = progressive::parseMatrixErrorJson(jStr(env, jErrorJson));
    return progressive::isUserDeactivated(error) ? JNI_TRUE : JNI_FALSE;
}

// --- Notification Formatter ---

JNI_FUNC(jstring, nativeFormatImageNotification)(JNIEnv* env, jclass, jstring jSender) {
    auto r = progressive::formatImageNotification(jStr(env, jSender));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatFileNotification)(JNIEnv* env, jclass, jstring jSender, jstring jFileName) {
    auto r = progressive::formatFileNotification(jStr(env, jSender), jStr(env, jFileName));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatVideoNotification)(JNIEnv* env, jclass, jstring jSender) {
    auto r = progressive::formatVideoNotification(jStr(env, jSender));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatAudioNotification)(JNIEnv* env, jclass, jstring jSender, jboolean jVoice) {
    auto r = progressive::formatAudioNotification(jStr(env, jSender), jVoice);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatInviteNotification)(JNIEnv* env, jclass, jstring jInviter, jstring jRoomName) {
    auto r = progressive::formatInviteNotification(jStr(env, jInviter), jStr(env, jRoomName));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatRoomNotification)(JNIEnv* env, jclass, jint jCount, jstring jRoomName) {
    auto r = progressive::formatRoomNotification(jCount, jStr(env, jRoomName));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatStickerNotification)(JNIEnv* env, jclass, jstring jSender) {
    auto r = progressive::formatStickerNotification(jStr(env, jSender));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatLocationNotification)(JNIEnv* env, jclass, jstring jSender) {
    auto r = progressive::formatLocationNotification(jStr(env, jSender));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeFormatPollNotification)(JNIEnv* env, jclass, jstring jSender, jboolean jStart) {
    auto r = progressive::formatPollNotification(jStr(env, jSender), jStart);
    return env->NewStringUTF(r.c_str());
}

// --- Megolm Decryptor ---

// --- Raw Service ---

JNI_FUNC(jstring, nativeCacheKeyForUrl)(JNIEnv* env, jclass, jstring jUrl) {
    auto r = progressive::cacheKeyForUrl(jStr(env, jUrl));
    return env->NewStringUTF(r.c_str());
}

// --- Lightweight Settings ---

JNI_FUNC(jboolean, nativeGetSettingBool)(JNIEnv* env, jclass, jstring jJson, jstring jKey, jboolean jDefault) {
    return progressive::getSettingBool(jStr(env, jJson), jStr(env, jKey), jDefault) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeSetSettingBool)(JNIEnv* env, jclass, jstring jJson, jstring jKey, jboolean jVal) {
    auto r = progressive::setSettingBool(jStr(env, jJson), jStr(env, jKey), jVal);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeGetSettingString)(JNIEnv* env, jclass, jstring jJson, jstring jKey, jstring jDefault) {
    auto r = progressive::getSettingString(jStr(env, jJson), jStr(env, jKey), jStr(env, jDefault));
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeSetSettingString)(JNIEnv* env, jclass, jstring jJson, jstring jKey, jstring jVal) {
    auto r = progressive::setSettingString(jStr(env, jJson), jStr(env, jKey), jStr(env, jVal));
    return env->NewStringUTF(r.c_str());
}

// --- HTTP Client ---

JNI_FUNC(jstring, nativeParseUrl)(JNIEnv* env, jclass, jstring jUrl) {
    auto u = progressive::parseUrl(jStr(env, jUrl));
    std::ostringstream os;
    os << R"({"scheme":")" << u.scheme
       << R"(","host":")" << u.host
       << R"(","port":)" << u.port
       << R"(,"path":")" << u.path
       << R"(","valid":)" << (u.valid ? "true" : "false") << "}";
    return env->NewStringUTF(os.str().c_str());
}

// --- Megolm Decryptor ---
// Controlled by Labs: SETTINGS_LABS_NATIVE_CRYPTO

static progressive::MegolmSessionManager g_megolmManager;

JNI_FUNC(jboolean, nativeMegolmAddSession)(JNIEnv* env, jclass, jstring jRoom, jstring jSenderKey, jstring jSessionId, jstring jSessionKey) {
    return g_megolmManager.addSession(jStr(env, jRoom), jStr(env, jSenderKey),
        jStr(env, jSessionId), jStr(env, jSessionKey)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeMegolmDecrypt)(JNIEnv* env, jclass, jstring jRoom, jstring jSenderKey, jstring jSessionId, jstring jCiphertext) {
    auto room = jStr(env, jRoom);
    auto sk = jStr(env, jSenderKey);
    auto sid = jStr(env, jSessionId);
    auto* session = g_megolmManager.findSession(room, sk, sid);
    if (!session) session = g_megolmManager.findSession(room, "", sid);  // fallback: try without senderKey
    if (!session) return env->NewStringUTF("");
    auto result = progressive::megolmDecrypt(*session, jStr(env, jCiphertext));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jint, nativeMegolmSessionCount)(JNIEnv*, jclass) {
    return g_megolmManager.sessionCount();
}

JNI_FUNC(void, nativeMegolmClearRoom)(JNIEnv* env, jclass, jstring jRoom) {
    g_megolmManager.clearRoom(jStr(env, jRoom));
}

// --- Olm Account & Session ---
static progressive::OlmAccountData g_olmAccount;
static progressive::OlmSessionManager g_olmSessionMgr;

JNI_FUNC(jboolean, nativeOlmCreateAccount)(JNIEnv* env, jclass, jstring jUserId, jstring jDeviceId) {
    g_olmAccount = progressive::createOlmAccount(jStr(env, jUserId), jStr(env, jDeviceId));
    return g_olmAccount.valid ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeOlmGetIdentityKeys)(JNIEnv* env, jclass) {
    return env->NewStringUTF(progressive::getAccountIdentityKeys(g_olmAccount).c_str());
}

JNI_FUNC(jstring, nativeOlmGenerateOneTimeKeys)(JNIEnv* env, jclass, jint jCount) {
    auto result = progressive::generateOneTimeKeys(g_olmAccount, jCount);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeOlmSignMessage)(JNIEnv* env, jclass, jstring jMessage) {
    auto result = progressive::accountSign(g_olmAccount, jStr(env, jMessage));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeOlmCreateInboundSession)(JNIEnv* env, jclass, jstring jTheirKey, jstring jPreKeyMsg) {
    auto session = progressive::createInboundOlmSession(g_olmAccount, jStr(env, jTheirKey), jStr(env, jPreKeyMsg));
    if (!session.valid) return env->NewStringUTF("");
    auto sid = session.sessionId;
    g_olmSessionMgr.addSession(jStr(env, jTheirKey), sid, std::move(session));
    return env->NewStringUTF(sid.c_str());
}

JNI_FUNC(jstring, nativeOlmDecryptMessage)(JNIEnv* env, jclass, jstring jSenderKey, jstring jSessionId, jstring jCiphertext) {
    auto* session = g_olmSessionMgr.findSession(jStr(env, jSenderKey), jStr(env, jSessionId));
    if (!session) return env->NewStringUTF("");
    auto result = progressive::olmDecryptMessage(*session, jStr(env, jCiphertext));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeOlmPickleAccount)(JNIEnv* env, jclass) {
    auto result = progressive::pickleOlmAccount(g_olmAccount);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeOlmUnpickleAccount)(JNIEnv* env, jclass, jstring jPickled, jstring jUserId, jstring jDeviceId) {
    g_olmAccount = progressive::unpickleOlmAccount(jStr(env, jPickled), jStr(env, jUserId), jStr(env, jDeviceId));
    return g_olmAccount.valid ? JNI_TRUE : JNI_FALSE;
}

// --- Event Signing ---

JNI_FUNC(jstring, nativeSignEvent)(JNIEnv* env, jclass, jstring jEventJson) {
    auto result = progressive::signEvent(g_olmAccount, jStr(env, jEventJson));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeVerifyEventSignature)(JNIEnv* env, jclass, jstring jEventJson, jstring jSignKeyB64) {
    return progressive::verifyEventSignature(jStr(env, jEventJson), jStr(env, jSignKeyB64)) ? JNI_TRUE : JNI_FALSE;
}

// --- Device Verification ---

JNI_FUNC(jboolean, nativeVerifyDeviceSignature)(JNIEnv* env, jclass, jstring jDeviceKeysJson, jstring jUserId, jstring jDeviceId, jstring jSignKeyB64, jstring jSignatureB64) {
    return progressive::verifyDeviceSignature(jStr(env, jDeviceKeysJson), jStr(env, jUserId),
        jStr(env, jDeviceId), jStr(env, jSignKeyB64), jStr(env, jSignatureB64)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeComputeDeviceFingerprint)(JNIEnv* env, jclass, jstring jIdentityKeyB64) {
    auto result = progressive::computeDeviceFingerprint(jStr(env, jIdentityKeyB64));
    return env->NewStringUTF(result.c_str());
}

// --- SAS Emoji Verification ---
static progressive::SasVerification g_sas;

JNI_FUNC(jstring, nativeSasCreate)(JNIEnv* env, jclass) {
    g_sas = progressive::sasCreate();
    return env->NewStringUTF(g_sas.ourPubkey.c_str());
}

JNI_FUNC(jboolean, nativeSasSetTheirKey)(JNIEnv* env, jclass, jstring jTheirPubkey) {
    return progressive::sasSetTheirKey(g_sas, jStr(env, jTheirPubkey)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, nativeSasGetEmojis)(JNIEnv* env, jclass) {
    auto result = progressive::sasGetEmojis(g_sas);
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jstring, nativeSasCalculateMac)(JNIEnv* env, jclass, jstring jInput, jstring jInfo) {
    auto result = progressive::sasCalculateMac(g_sas, jStr(env, jInput), jStr(env, jInfo));
    return env->NewStringUTF(result.c_str());
}

JNI_FUNC(jboolean, nativeSasVerifyMac)(JNIEnv* env, jclass, jstring jTheirMac, jstring jInput, jstring jInfo) {
    return progressive::sasVerifyMac(g_sas, jStr(env, jTheirMac), jStr(env, jInput), jStr(env, jInfo)) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(void, nativeSasDestroy)(JNIEnv*, jclass) {
    progressive::sasDestroy(g_sas);
}

// --- JSON Parser ---

JNI_FUNC(jstring, nativeParseJsonStringValue)(JNIEnv* env, jclass, jstring jJson, jstring jKey) {
    auto r = progressive::parseJsonStringValue(jStr(env, jJson), jStr(env, jKey));
    return env->NewStringUTF(r.c_str());
}

// --- Federation Version ---

JNI_FUNC(jstring, nativeFederationVersionToJson)(JNIEnv* env, jclass, jstring jVersionJson) {
    auto v = progressive::parseFederationVersion(jStr(env, jVersionJson));
    auto r = progressive::federationVersionToJson(v);
    return env->NewStringUTF(r.c_str());
}

// --- Auth Models ---

JNI_FUNC(jstring, nativePresenceEnumToString)(JNIEnv* env, jclass, jint jPresence) {
    auto r = progressive::presenceEnumToString(static_cast<progressive::PresenceEnum>(jPresence));
    return env->NewStringUTF(r);
}

JNI_FUNC(jstring, nativeCredentialsToJson)(JNIEnv* env, jclass, jstring jCredsJson) {
    auto json = jStr(env, jCredsJson);
    progressive::Credentials c;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    c.userId = es("user_id"); c.accessToken = es("access_token"); c.refreshToken = es("refresh_token");
    c.homeServer = es("home_server"); c.deviceId = es("device_id");
    auto r = progressive::credentialsToJson(c);
    return env->NewStringUTF(r.c_str());
}

// --- Call Models ---

JNI_FUNC(jstring, nativeSdpTypeToString)(JNIEnv* env, jclass, jint jType) {
    auto r = progressive::sdpTypeToString(static_cast<progressive::SdpType>(jType));
    return env->NewStringUTF(r);
}

JNI_FUNC(jstring, nativeEndCallReasonToString)(JNIEnv* env, jclass, jint jReason) {
    auto r = progressive::endCallReasonToString(static_cast<progressive::EndCallReason>(jReason));
    return env->NewStringUTF(r);
}

// --- Message Content (struct→json roundtrip) ---

JNI_FUNC(jstring, nativeMessageTextToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageTextContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto eb = [&](const std::string& k) -> bool {
        return json.find("\"" + k + "\":true") != std::string::npos;
    };
    m.msgtype = es("msgtype"); m.body = es("body"); m.formattedBody = es("formatted_body");
    m.format = es("format"); m.relatesTo = es("m.relates_to");
    m.isFallback = eb("is_fallback");
    auto r = progressive::messageTextToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageNoticeToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageNoticeContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    m.body = es("body"); m.msgtype = es("msgtype");
    auto r = progressive::messageNoticeToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageEmoteToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageEmoteContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    m.body = es("body"); m.msgtype = es("msgtype");
    m.formattedBody = es("formatted_body"); m.format = es("format");
    auto r = progressive::messageEmoteToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageImageToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageImageContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto ei = [&](const std::string& k) -> int64_t {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return 0;
        pp = json.find(':', pp); if (pp == std::string::npos) return 0;
        pp++; while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0; while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
        return v;
    };
    m.url = es("url"); m.thumbnailUrl = es("thumbnail_url"); m.thumbnailInfo = es("thumbnail_info");
    m.mimetype = es("mimetype"); m.filename = es("filename"); m.body = es("body");
    m.width = static_cast<int>(ei("w")); m.height = static_cast<int>(ei("h"));
    m.size = static_cast<int>(ei("size"));
    auto r = progressive::messageImageToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageVideoToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageVideoContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto ei = [&](const std::string& k) -> int64_t {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return 0;
        pp = json.find(':', pp); if (pp == std::string::npos) return 0;
        pp++; while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0; while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
        return v;
    };
    m.url = es("url"); m.thumbnailUrl = es("thumbnail_url"); m.mimetype = es("mimetype");
    m.filename = es("filename"); m.body = es("body"); m.duration = ei("duration");
    m.width = static_cast<int>(ei("w")); m.height = static_cast<int>(ei("h"));
    m.size = static_cast<int>(ei("size"));
    auto r = progressive::messageVideoToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageAudioToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageAudioContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto ei = [&](const std::string& k) -> int64_t {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return 0;
        pp = json.find(':', pp); if (pp == std::string::npos) return 0;
        pp++; while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0; while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
        return v;
    };
    m.url = es("url"); m.mimetype = es("mimetype"); m.filename = es("filename");
    m.body = es("body"); m.duration = ei("duration"); m.size = static_cast<int>(ei("size"));
    auto r = progressive::messageAudioToJson(m);
    return env->NewStringUTF(r.c_str());
}

JNI_FUNC(jstring, nativeMessageFileToJson)(JNIEnv* env, jclass, jstring jContentJson) {
    auto json = jStr(env, jContentJson);
    progressive::MessageFileContent m;
    auto es = [&](const std::string& k) -> std::string {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return "";
        pp = json.find('"', pp + k.size() + 2); if (pp == std::string::npos) return "";
        pp++; size_t e = pp; while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };
    auto ei = [&](const std::string& k) -> int64_t {
        auto pp = json.find("\"" + k + "\""); if (pp == std::string::npos) return 0;
        pp = json.find(':', pp); if (pp == std::string::npos) return 0;
        pp++; while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0; while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
        return v;
    };
    m.url = es("url"); m.mimetype = es("mimetype"); m.filename = es("filename");
    m.body = es("body"); m.size = static_cast<int>(ei("size"));
    auto r = progressive::messageFileToJson(m);
    return env->NewStringUTF(r.c_str());
}

} // extern "C"
