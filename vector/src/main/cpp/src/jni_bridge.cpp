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

extern "C" {

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeValidateAndBuild
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;
 *
 * Returns JSON string: {"url": "...", ...} or {"error": "..."}
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeValidateAndBuild(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeParseResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 *
 * Parses the HTTP response body and returns JSON with eventId or error.
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseResponse(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
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
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseRelation(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
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
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventHtml(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeFormatEventPlainText
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 *
 * Formats a single event as plain text line.
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEventPlainText(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
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
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildHtmlExport(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCachePut
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCachePut(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCacheGetContext
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheGetContext(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeCacheSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheSize(
    JNIEnv* /* env */,
    jclass /* this */
) {
    return static_cast<jint>(g_eventCache.size());
}

// --- Translation ---

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeBuildTranslateRequest
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildTranslateRequest(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeParseTranslateResponse
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseTranslateResponse(
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
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeComputeProxyConfig
 * Signature: (IILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeProxyConfig(
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
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilAddress(
    JNIEnv* env, jclass, jstring jAddr
) {
    if (!jAddr) return JNI_FALSE;
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    return progressive::isYggdrasilAddress(addr) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsYggdrasilDomain(
    JNIEnv* env, jclass, jstring jHost
) {
    if (!jHost) return JNI_FALSE;
    auto host = std::string(env->GetStringUTFChars(jHost, nullptr));
    env->ReleaseStringUTFChars(jHost, host.c_str());
    return progressive::isYggdrasilDomain(host) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildYggHomeserverUrl(
    JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls
) {
    if (!jAddr) return env->NewStringUTF("");
    auto addr = std::string(env->GetStringUTFChars(jAddr, nullptr));
    env->ReleaseStringUTFChars(jAddr, addr.c_str());
    auto url = progressive::buildYggHomeserverUrl(addr, jPort, jTls);
    return env->NewStringUTF(url.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeRewriteHomeserverUrl(
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

// --- Room Tombstone ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseTombstone(
    JNIEnv* env, jclass, jstring jContentJson
) {
    auto json = jContentJson ? std::string(env->GetStringUTFChars(jContentJson, nullptr)) : "{}";
    if (jContentJson) env->ReleaseStringUTFChars(jContentJson, json.c_str());
    auto tombstone = progressive::parseTombstone(json);
    auto result = progressive::tombstoneToJson(tombstone);
    return env->NewStringUTF(result.c_str());
}

// --- Sync Utils ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildSyncFilter(
    JNIEnv* env, jclass, jboolean jIncludeThreads, jboolean jIncludePresence, jint jTimelineLimit
) {
    SyncFilter filter;
    filter.includeThreads = jIncludeThreads;
    filter.includePresence = jIncludePresence;
    filter.timelineLimit = jTimelineLimit;
    auto s = progressive::buildSyncFilter(filter);
    return env->NewStringUTF(s.c_str());
}

// --- Event Display ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeClassifyEvent(
    JNIEnv* env, jclass, jstring jEventType, jstring jMsgType
) {
    auto et = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    auto mt = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    if (jEventType) env->ReleaseStringUTFChars(jEventType, et.c_str());
    if (jMsgType) env->ReleaseStringUTFChars(jMsgType, mt.c_str());
    return env->NewStringUTF(std::to_string(static_cast<int>(progressive::classifyEvent(et, mt))).c_str());
}

// --- Permalink ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildEventPermalink(
    JNIEnv* env, jclass, jstring jRoomId, jstring jEventId
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto eventId = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jEventId) env->ReleaseStringUTFChars(jEventId, eventId.c_str());
    auto s = progressive::buildEventPermalink(roomId, eventId);
    return env->NewStringUTF(s.c_str());
}

// --- Network Monitor ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetRecommendedMediaQuality(
    JNIEnv* env, jclass,
    jint jType, jboolean jConnected, jboolean jMetered,
    jint jSignal, jdouble jLatency, jdouble jLoss
) {
    auto quality = progressive::computeNetworkQuality(
        static_cast<NetworkType>(jType), jConnected, jMetered, false,
        jSignal, jLatency, jLoss
    );
    auto s = progressive::getRecommendedMediaQuality(quality);
    return env->NewStringUTF(s.c_str());
}

// --- Client Info ---

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCompareSemver(
    JNIEnv* env, jclass, jstring jA, jstring jB
) {
    auto a = jA ? std::string(env->GetStringUTFChars(jA, nullptr)) : "";
    auto b = jB ? std::string(env->GetStringUTFChars(jB, nullptr)) : "";
    if (jA) env->ReleaseStringUTFChars(jA, a.c_str());
    if (jB) env->ReleaseStringUTFChars(jB, b.c_str());
    return progressive::compareSemver(a, b);
}

// --- Keyshare ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildKeyRequestBody(
    JNIEnv* env, jclass,
    jstring jRoomId, jstring jSessionId, jstring jSenderKey, jstring jAlgorithm,
    jstring jRequestId, jstring jDeviceId
) {
    auto roomId   = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto sessionId = jSessionId ? std::string(env->GetStringUTFChars(jSessionId, nullptr)) : "";
    auto senderKey = jSenderKey ? std::string(env->GetStringUTFChars(jSenderKey, nullptr)) : "";
    auto algorithm = jAlgorithm ? std::string(env->GetStringUTFChars(jAlgorithm, nullptr)) : "";
    auto requestId = jRequestId ? std::string(env->GetStringUTFChars(jRequestId, nullptr)) : "";
    auto deviceId  = jDeviceId ? std::string(env->GetStringUTFChars(jDeviceId, nullptr)) : "";

    if (jRoomId)    env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jSessionId) env->ReleaseStringUTFChars(jSessionId, sessionId.c_str());
    if (jSenderKey) env->ReleaseStringUTFChars(jSenderKey, senderKey.c_str());
    if (jAlgorithm) env->ReleaseStringUTFChars(jAlgorithm, algorithm.c_str());
    if (jRequestId) env->ReleaseStringUTFChars(jRequestId, requestId.c_str());
    if (jDeviceId)  env->ReleaseStringUTFChars(jDeviceId, deviceId.c_str());

    auto s = progressive::buildKeyRequestBody(roomId, sessionId, senderKey, algorithm, requestId, deviceId);
    return env->NewStringUTF(s.c_str());
}

// --- Display Name ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserIdToDisplayName(
    JNIEnv* env, jclass, jstring jUserId
) {
    auto userId = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    if (jUserId) env->ReleaseStringUTFChars(jUserId, userId.c_str());
    auto s = progressive::userIdToDisplayName(userId);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserIdToColor(
    JNIEnv* env, jclass, jstring jUserId
) {
    auto userId = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    if (jUserId) env->ReleaseStringUTFChars(jUserId, userId.c_str());
    auto s = progressive::userIdToColor(userId);
    return env->NewStringUTF(s.c_str());
}

// --- Message Location ---

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEstimatePaginationRequests(
    JNIEnv*, jclass, jint jMissing, jint jPageSize
) {
    return progressive::estimatePaginationRequests(jMissing, jPageSize);
}

// --- Timeline Utils ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldAutoScroll(
    JNIEnv*, jclass, jboolean jIsOwnMsg
) {
    LiveTimelineState state;
    auto result = progressive::shouldAutoScroll(state, jIsOwnMsg);
    return result ? JNI_TRUE : JNI_FALSE;
}

// --- Cross Signing ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseCrossSigningStatus(
    JNIEnv* env, jclass, jstring jAccountDataJson, jstring jUserId
) {
    auto json = jAccountDataJson ? std::string(env->GetStringUTFChars(jAccountDataJson, nullptr)) : "";
    auto userId = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    if (jAccountDataJson) env->ReleaseStringUTFChars(jAccountDataJson, json.c_str());
    if (jUserId) env->ReleaseStringUTFChars(jUserId, userId.c_str());

    auto status = progressive::parseCrossSigningStatus(json, userId);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream out;
    out << R"({"isSetup": )" << (status.isSetup ? "true" : "false");
    out << R"(,"isVerified": )" << (status.isVerified ? "true" : "false");
    out << R"(,"needsBootstrap": )" << (status.needsBootstrap ? "true" : "false") << "}";
    return env->NewStringUTF(out.str().c_str());
}

// --- Edit History ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetEditBadgeText(
    JNIEnv* env, jclass, jint jEditCount
) {
    auto s = progressive::getEditBadgeText(jEditCount);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetEditCountBadge(
    JNIEnv* env, jclass, jint jEditCount
) {
    auto s = progressive::getEditCountBadge(jEditCount);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeEditDiffSummary(
    JNIEnv* env, jclass, jstring jOldBody, jstring jNewBody
) {
    auto oldBody = jOldBody ? std::string(env->GetStringUTFChars(jOldBody, nullptr)) : "";
    auto newBody = jNewBody ? std::string(env->GetStringUTFChars(jNewBody, nullptr)) : "";
    if (jOldBody) env->ReleaseStringUTFChars(jOldBody, oldBody.c_str());
    if (jNewBody) env->ReleaseStringUTFChars(jNewBody, newBody.c_str());
    auto s = progressive::computeEditDiffSummary(oldBody, newBody);
    return env->NewStringUTF(s.c_str());
}

// --- Read Marker / Unread Count ---
// Ported from: TimelineViewModel.kt (read marker index math)
//              ReadMarkers.kt (server-side management)
//              RoomSummary.kt (unread count display)

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeReadMarker(
    JNIEnv* env, jclass,
    jstring jLastReadEventId,
    jobjectArray jLoadedEventIds,
    jobjectArray jLoadedSenders,
    jbooleanArray jIsMention,
    jbooleanArray jIsHighlight,
    jstring jMyUserId
) {
    std::string lastReadEventId = jLastReadEventId ? env->GetStringUTFChars(jLastReadEventId, nullptr) : "";
    if (jLastReadEventId) env->ReleaseStringUTFChars(jLastReadEventId, lastReadEventId.c_str());

    // Convert event IDs array
    std::vector<std::string> eventIds;
    jsize idCount = jLoadedEventIds ? env->GetArrayLength(jLoadedEventIds) : 0;
    for (jsize i = 0; i < idCount; ++i) {
        auto jId = (jstring)env->GetObjectArrayElement(jLoadedEventIds, i);
        const char* c = env->GetStringUTFChars(jId, nullptr);
        eventIds.push_back(c);
        env->ReleaseStringUTFChars(jId, c);
        env->DeleteLocalRef(jId);
    }

    // Convert senders array
    std::vector<std::string> senders;
    jsize senderCount = jLoadedSenders ? env->GetArrayLength(jLoadedSenders) : 0;
    for (jsize i = 0; i < senderCount; ++i) {
        auto jSender = (jstring)env->GetObjectArrayElement(jLoadedSenders, i);
        const char* c = env->GetStringUTFChars(jSender, nullptr);
        senders.push_back(c);
        env->ReleaseStringUTFChars(jSender, c);
        env->DeleteLocalRef(jSender);
    }

    // Convert mention boolean array
    std::vector<bool> isMention;
    jsize mentionCount = jIsMention ? env->GetArrayLength(jIsMention) : 0;
    if (mentionCount > 0) {
        jboolean* raw = env->GetBooleanArrayElements(jIsMention, nullptr);
        for (jsize i = 0; i < mentionCount; ++i) isMention.push_back(raw[i]);
        env->ReleaseBooleanArrayElements(jIsMention, raw, JNI_ABORT);
    }

    // Convert highlight boolean array
    std::vector<bool> isHighlight;
    jsize hlCount = jIsHighlight ? env->GetArrayLength(jIsHighlight) : 0;
    if (hlCount > 0) {
        jboolean* raw = env->GetBooleanArrayElements(jIsHighlight, nullptr);
        for (jsize i = 0; i < hlCount; ++i) isHighlight.push_back(raw[i]);
        env->ReleaseBooleanArrayElements(jIsHighlight, raw, JNI_ABORT);
    }

    std::string myUserId = jMyUserId ? env->GetStringUTFChars(jMyUserId, nullptr) : "";
    if (jMyUserId) env->ReleaseStringUTFChars(jMyUserId, myUserId.c_str());

    auto state = progressive::computeReadMarker(lastReadEventId, eventIds, senders, isMention, isHighlight, myUserId);
    auto json = progressive::readMarkerToJson(state);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldShowJumpToUnread(
    JNIEnv* env, jclass, jstring jReadMarkerJson
) {
    std::string json = jReadMarkerJson ? env->GetStringUTFChars(jReadMarkerJson, nullptr) : "{}";
    if (jReadMarkerJson) env->ReleaseStringUTFChars(jReadMarkerJson, json.c_str());

    // Quick parse of unread fields from JSON
    bool hasUnread = json.find(R"("hasUnread": true)") != std::string::npos ||
                     json.find(R"("hasUnread":true)") != std::string::npos;
    bool hasIndex = json.find(R"("readMarkerIndex":)") != std::string::npos;
    return hasUnread && hasIndex;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatUnreadJumpLabel(
    JNIEnv* env, jclass, jstring jReadMarkerJson
) {
    std::string json = jReadMarkerJson ? env->GetStringUTFChars(jReadMarkerJson, nullptr) : "{}";
    if (jReadMarkerJson) env->ReleaseStringUTFChars(jReadMarkerJson, json.c_str());

    // Quick parse unreadCount from JSON
    int unreadCount = 0;
    auto pos = json.find(R"("unreadCount")");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] == ' ') pos++;
            while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
                unreadCount = unreadCount * 10 + (json[pos] - '0');
                pos++;
            }
        }
    }

    int unreadMentions = 0;
    auto mp = json.find(R"("unreadMentions")");
    if (mp != std::string::npos) {
        mp = json.find(':', mp);
        if (mp != std::string::npos) {
            mp++;
            while (mp < json.size() && json[mp] == ' ') mp++;
            while (mp < json.size() && json[mp] >= '0' && json[mp] <= '9') {
                unreadMentions = unreadMentions * 10 + (json[mp] - '0');
                mp++;
            }
        }
    }

    std::ostringstream out;
    out << unreadCount;
    if (unreadCount == 1) out << " new message";
    else out << " new messages";
    if (unreadMentions > 0) out << " (" << unreadMentions << " mentions)";
    auto label = out.str();
    return env->NewStringUTF(label.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeAdvanceReadMarker(
    JNIEnv* env, jclass, jstring jRoomId, jstring jLatestEventId
) {
    std::string roomId = jRoomId ? env->GetStringUTFChars(jRoomId, nullptr) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    std::string latestEventId = jLatestEventId ? env->GetStringUTFChars(jLatestEventId, nullptr) : "";
    if (jLatestEventId) env->ReleaseStringUTFChars(jLatestEventId, latestEventId.c_str());
    auto newMark = progressive::advanceReadMarker(roomId, latestEventId);
    return env->NewStringUTF(newMark.c_str());
}

// --- Slash Commands ---
// Ported from: SlashCommandParser.kt, SlashCommand.kt, Command.kt

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseSlashCommand(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());

    auto cmd = progressive::parseSlashCommand(text);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"isSlashCommand": )" << (cmd.isSlashCommand ? "true" : "false");
    json << R"(,"command": ")" << esc(cmd.command) << R"(")";
    json << R"(,"arguments": ")" << esc(cmd.arguments) << R"(")";
    json << R"(,"type": )" << static_cast<int>(cmd.type);
    json << R"(,"needsMatrixId": )" << (cmd.needsMatrixİd ? "true" : "false");
    json << R"(,"isMessage": )" << (progressive::isMessageCommand(cmd.type) ? "true" : "false");
    json << R"(,"isAdmin": )" << (progressive::isAdminCommand(cmd.type) ? "true" : "false") << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatSlashCommand(
    JNIEnv* env, jclass,
    jstring jCommand, jstring jArguments, jint jType, jstring jSenderDisplayName
) {
    auto arguments = jArguments ? std::string(env->GetStringUTFChars(jArguments, nullptr)) : "";
    auto sender = jSenderDisplayName ? std::string(env->GetStringUTFChars(jSenderDisplayName, nullptr)) : "";
    if (jArguments) env->ReleaseStringUTFChars(jArguments, arguments.c_str());
    if (jSenderDisplayName) env->ReleaseStringUTFChars(jSenderDisplayName, sender.c_str());

    progressive::SlashCommand cmd;
    cmd.type = static_cast<progressive::SlashCommandType>(jType);
    cmd.arguments = arguments;
    auto formatted = progressive::formatSlashCommand(cmd, sender);
    return env->NewStringUTF(formatted.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsKnownSlashCommand(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    return progressive::isKnownSlashCommand(text);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetAvailableCommands(
    JNIEnv* env, jclass
) {
    auto cmds = progressive::getAvailableCommands();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < cmds.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << cmds[i] << R"(")";
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

// --- Typing Monitor ---
// Ported from: TypingUsersTracker.kt, TypingHelper.kt, TypingView.kt

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUpdateTypingState(
    JNIEnv* env, jclass,
    jstring jRoomId,
    jobjectArray jTypingUserIds,
    jobjectArray jDisplayNames,
    jlong jNowMs
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());

    std::vector<std::string> userIds;
    jsize count = jTypingUserIds ? env->GetArrayLength(jTypingUserIds) : 0;
    for (jsize i = 0; i < count; ++i) {
        auto jId = (jstring)env->GetObjectArrayElement(jTypingUserIds, i);
        const char* c = env->GetStringUTFChars(jId, nullptr);
        userIds.push_back(c);
        env->ReleaseStringUTFChars(jId, c);
        env->DeleteLocalRef(jId);
    }

    std::vector<std::string> names;
    jsize nameCount = jDisplayNames ? env->GetArrayLength(jDisplayNames) : 0;
    for (jsize i = 0; i < nameCount; ++i) {
        auto jName = (jstring)env->GetObjectArrayElement(jDisplayNames, i);
        const char* c = env->GetStringUTFChars(jName, nullptr);
        names.push_back(c);
        env->ReleaseStringUTFChars(jName, c);
        env->DeleteLocalRef(jName);
    }

    auto state = progressive::updateTypingState(roomId, userIds, names, jNowMs);
    auto json = progressive::typingStateToJson(state);
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatTypingText(
    JNIEnv* env, jclass, jstring jTypingStateJson
) {
    // Quick-parse the typing state from JSON
    auto json = jTypingStateJson ? std::string(env->GetStringUTFChars(jTypingStateJson, nullptr)) : "{}";
    if (jTypingStateJson) env->ReleaseStringUTFChars(jTypingStateJson, json.c_str());

    // Parse typingUsers array
    progressive::TypingState state;
    state.hasAnyoneTyping = json.find(R"("hasAnyoneTyping": true)") != std::string::npos ||
                            json.find(R"("hasAnyoneTyping":true)") != std::string::npos;

    // Count typing users by counting "userId" occurrences
    size_t pos = 0;
    while ((pos = json.find("\"userId\"", pos)) != std::string::npos) {
        pos++;
        state.typingUsers.push_back({});
    }
    state.typingCount = static_cast<int>(state.typingUsers.size());

    auto text = progressive::formatTypingText(state, 3);
    return env->NewStringUTF(text.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsUserTyping(
    JNIEnv* env, jclass, jstring jTypingStateJson, jstring jUserId, jlong jNowMs
) {
    auto json = jTypingStateJson ? std::string(env->GetStringUTFChars(jTypingStateJson, nullptr)) : "{}";
    if (jTypingStateJson) env->ReleaseStringUTFChars(jTypingStateJson, json.c_str());
    auto userId = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    if (jUserId) env->ReleaseStringUTFChars(jUserId, userId.c_str());

    // Quick check: is the userId in the JSON?
    return json.find("\"userId\": \"" + userId + "\"") != std::string::npos;
}

// --- URL Preview / OpenGraph ---
// Ported from: UrlPreviewer.kt, EventHtmlRenderer.kt

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseUrlPreview(
    JNIEnv* env, jclass, jstring jHtml, jstring jBaseUrl
) {
    auto html = jHtml ? std::string(env->GetStringUTFChars(jHtml, nullptr)) : "";
    auto baseUrl = jBaseUrl ? std::string(env->GetStringUTFChars(jBaseUrl, nullptr)) : "";
    if (jHtml) env->ReleaseStringUTFChars(jHtml, html.c_str());
    if (jBaseUrl) env->ReleaseStringUTFChars(jBaseUrl, baseUrl.c_str());

    auto preview = progressive::parseUrlPreview(html, baseUrl);
    auto json = progressive::urlPreviewToJson(preview);
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractHtmlTitle(
    JNIEnv* env, jclass, jstring jHtml
) {
    auto html = jHtml ? std::string(env->GetStringUTFChars(jHtml, nullptr)) : "";
    if (jHtml) env->ReleaseStringUTFChars(jHtml, html.c_str());
    auto title = progressive::extractHtmlTitle(html);
    return env->NewStringUTF(title.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeResolveUrl(
    JNIEnv* env, jclass, jstring jBaseUrl, jstring jRelative
) {
    auto base = jBaseUrl ? std::string(env->GetStringUTFChars(jBaseUrl, nullptr)) : "";
    auto rel = jRelative ? std::string(env->GetStringUTFChars(jRelative, nullptr)) : "";
    if (jBaseUrl) env->ReleaseStringUTFChars(jBaseUrl, base.c_str());
    if (jRelative) env->ReleaseStringUTFChars(jRelative, rel.c_str());
    auto resolved = progressive::resolveUrl(base, rel);
    return env->NewStringUTF(resolved.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsImageUrl(
    JNIEnv* env, jclass, jstring jUrl
) {
    auto url = jUrl ? std::string(env->GetStringUTFChars(jUrl, nullptr)) : "";
    if (jUrl) env->ReleaseStringUTFChars(jUrl, url.c_str());
    return progressive::isImageUrl(url);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeStripHtmlTags(
    JNIEnv* env, jclass, jstring jHtml
) {
    auto html = jHtml ? std::string(env->GetStringUTFChars(jHtml, nullptr)) : "";
    if (jHtml) env->ReleaseStringUTFChars(jHtml, html.c_str());
    auto stripped = progressive::stripHtmlTags(html);
    return env->NewStringUTF(stripped.c_str());
}

// --- Matrix Error Parser ---
// Ported from: MatrixError.kt (218L)

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseMatrixError(
    JNIEnv* env, jclass, jstring jJson
) {
    auto json = jJson ? std::string(env->GetStringUTFChars(jJson, nullptr)) : "{}";
    if (jJson) env->ReleaseStringUTFChars(jJson, json.c_str());
    auto error = progressive::parseMatrixError(json);
    auto result = progressive::matrixErrorToJson(error);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetErrorDescription(
    JNIEnv* env, jclass, jstring jErrorCode
) {
    auto code = jErrorCode ? std::string(env->GetStringUTFChars(jErrorCode, nullptr)) : "";
    if (jErrorCode) env->ReleaseStringUTFChars(jErrorCode, code.c_str());
    auto desc = progressive::getErrorDescription(code);
    return env->NewStringUTF(desc.c_str());
}

// --- AI Agent Executor ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildAgentSystemPrompt(
    JNIEnv* env, jclass, jstring jSystemPrompt
) {
    progressive::AgentConfig config;
    if (jSystemPrompt) {
        config.systemPrompt = std::string(env->GetStringUTFChars(jSystemPrompt, nullptr));
        env->ReleaseStringUTFChars(jSystemPrompt, config.systemPrompt.c_str());
    }
    config.toolsDescription = progressive::getAgentToolsSchema();
    auto prompt = progressive::buildAgentSystemPrompt(config);
    return env->NewStringUTF(prompt.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeProcessAgentIteration(
    JNIEnv* env, jclass, jstring jStateJson, jstring jLlmResponse
) {
    // Quick minimal parse of agent state from JSON
    auto stateJson = jStateJson ? std::string(env->GetStringUTFChars(jStateJson, nullptr)) : "{}";
    auto llmResp = jLlmResponse ? std::string(env->GetStringUTFChars(jLlmResponse, nullptr)) : "";
    if (jStateJson) env->ReleaseStringUTFChars(jStateJson, stateJson.c_str());
    if (jLlmResponse) env->ReleaseStringUTFChars(jLlmResponse, llmResp.c_str());

    progressive::AgentState state;
    state.llmResponse = llmResp;

    auto updated = progressive::processAgentIteration(state, llmResp);
    auto result = progressive::agentStateToJson(updated);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseToolCalls(
    JNIEnv* env, jclass, jstring jLlmResponse
) {
    auto resp = jLlmResponse ? std::string(env->GetStringUTFChars(jLlmResponse, nullptr)) : "";
    if (jLlmResponse) env->ReleaseStringUTFChars(jLlmResponse, resp.c_str());

    auto calls = progressive::parseToolCalls(resp);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"hasToolCalls": )" << (!calls.empty() ? "true" : "false") << ",";
    json << R"("calls": [)";
    for (size_t i = 0; i < calls.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"name": ")" << esc(calls[i].toolName) << R"(")";
        json << R"(,"arguments": )" << calls[i].argumentsJson;
        json << R"(,"callId": ")" << esc(calls[i].callId) << R"("})";
    }
    json << "]}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractTextAnswer(
    JNIEnv* env, jclass, jstring jLlmResponse
) {
    auto resp = jLlmResponse ? std::string(env->GetStringUTFChars(jLlmResponse, nullptr)) : "";
    if (jLlmResponse) env->ReleaseStringUTFChars(jLlmResponse, resp.c_str());
    auto answer = progressive::extractTextAnswer(resp);
    return env->NewStringUTF(answer.c_str());
}

// --- Push Condition Evaluator ---
// Ported from: EventMatchCondition.kt (105L), Glob.kt (39L)

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEvaluatePushCondition(
    JNIEnv* env, jclass, jstring jEventJson, jstring jKey, jstring jPattern
) {
    auto json = jEventJson ? std::string(env->GetStringUTFChars(jEventJson, nullptr)) : "{}";
    auto key = jKey ? std::string(env->GetStringUTFChars(jKey, nullptr)) : "";
    auto pattern = jPattern ? std::string(env->GetStringUTFChars(jPattern, nullptr)) : "";
    if (jEventJson) env->ReleaseStringUTFChars(jEventJson, json.c_str());
    if (jKey) env->ReleaseStringUTFChars(jKey, key.c_str());
    if (jPattern) env->ReleaseStringUTFChars(jPattern, pattern.c_str());
    return progressive::evaluateEventMatchCondition(json, key, pattern);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractJsonField(
    JNIEnv* env, jclass, jstring jJson, jstring jFieldPath
) {
    auto json = jJson ? std::string(env->GetStringUTFChars(jJson, nullptr)) : "{}";
    auto path = jFieldPath ? std::string(env->GetStringUTFChars(jFieldPath, nullptr)) : "";
    if (jJson) env->ReleaseStringUTFChars(jJson, json.c_str());
    if (jFieldPath) env->ReleaseStringUTFChars(jFieldPath, path.c_str());
    auto value = progressive::extractJsonField(json, path);
    return env->NewStringUTF(value.c_str());
}

// --- Thread Metadata ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeThreadMeta(
    JNIEnv* env, jclass,
    jstring jRootContent,
    jobjectArray jReplySenders,
    jobjectArray jReplyBodies,
    jlongArray jReplyTimestamps
) {
    auto rootContent = jRootContent ? std::string(env->GetStringUTFChars(jRootContent, nullptr)) : "{}";
    if (jRootContent) env->ReleaseStringUTFChars(jRootContent, rootContent.c_str());

    std::vector<std::string> senders, bodies;
    std::vector<int64_t> timestamps;

    jsize n = jReplySenders ? env->GetArrayLength(jReplySenders) : 0;
    for (jsize i = 0; i < n; ++i) {
        auto js = (jstring)env->GetObjectArrayElement(jReplySenders, i);
        if (js) { const char* c = env->GetStringUTFChars(js, nullptr); senders.push_back(c); env->ReleaseStringUTFChars(js, c); env->DeleteLocalRef(js); }
        auto jb = (jstring)env->GetObjectArrayElement(jReplyBodies, i);
        if (jb) { const char* c = env->GetStringUTFChars(jb, nullptr); bodies.push_back(c); env->ReleaseStringUTFChars(jb, c); env->DeleteLocalRef(jb); }
    }

    if (jReplyTimestamps) {
        jsize tn = env->GetArrayLength(jReplyTimestamps);
        jlong* raw = env->GetLongArrayElements(jReplyTimestamps, nullptr);
        for (jsize i = 0; i < tn; ++i) timestamps.push_back(raw[i]);
        env->ReleaseLongArrayElements(jReplyTimestamps, raw, JNI_ABORT);
    }

    auto meta = progressive::computeThreadMeta(rootContent, senders, bodies, timestamps);
    auto result = progressive::threadMetaToJson(meta);
    return env->NewStringUTF(result.c_str());
}

// --- Pending Message Editing ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCanEditPendingMessage(
    JNIEnv* env, jclass, jstring jLocalId, jint jState
) {
    progressive::PendingMessage msg;
    msg.localId = jLocalId ? std::string(env->GetStringUTFChars(jLocalId, nullptr)) : "";
    if (jLocalId) env->ReleaseStringUTFChars(jLocalId, msg.localId.c_str());
    msg.state = static_cast<progressive::MessageSendState>(jState);
    bool can = progressive::canEditPendingMessage(msg);
    return env->NewStringUTF(can ? "true" : "false");
}

// --- Sender Notification Filter ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldNotifyForSender(
    JNIEnv* env, jclass, jstring jSettingsJson, jstring jSenderId, jboolean jHasMention, jboolean jHasHighlight
) {
    auto settingsJson = jSettingsJson ? std::string(env->GetStringUTFChars(jSettingsJson, nullptr)) : "{}";
    auto sender = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
    if (jSettingsJson) env->ReleaseStringUTFChars(jSettingsJson, settingsJson.c_str());
    if (jSenderId) env->ReleaseStringUTFChars(jSenderId, sender.c_str());

    auto settings = progressive::parseSenderNotifSettings(settingsJson);
    return progressive::shouldNotifyForSender(settings, sender, jHasMention, jHasHighlight);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeToggleMuteSender(
    JNIEnv* env, jclass, jstring jSettingsJson, jstring jSenderId, jboolean jMute
) {
    auto json = jSettingsJson ? std::string(env->GetStringUTFChars(jSettingsJson, nullptr)) : "{}";
    auto sender = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
    if (jSettingsJson) env->ReleaseStringUTFChars(jSettingsJson, json.c_str());
    if (jSenderId) env->ReleaseStringUTFChars(jSenderId, sender.c_str());

    auto settings = progressive::parseSenderNotifSettings(json);
    if (jMute) progressive::muteSender(settings, sender);
    else progressive::unmuteSender(settings, sender);
    auto result = progressive::senderNotifSettingsToJson(settings);
    return env->NewStringUTF(result.c_str());
}

// --- String Order / Fractional Indexing ---
// Ported from: StringOrderUtils.kt (87L), SpaceOrderUtils.kt (105L)

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeStringMidPoints(
    JNIEnv* env, jclass, jstring jLeft, jstring jRight, jint jCount
) {
    auto left = jLeft ? std::string(env->GetStringUTFChars(jLeft, nullptr)) : "";
    auto right = jRight ? std::string(env->GetStringUTFChars(jRight, nullptr)) : "";
    if (jLeft) env->ReleaseStringUTFChars(jLeft, left.c_str());
    if (jRight) env->ReleaseStringUTFChars(jRight, right.c_str());

    auto mids = progressive::stringMidPoints(left, right, jCount);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < mids.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(mids[i]) << R"(")";
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeStringAverage(
    JNIEnv* env, jclass, jstring jLeft, jstring jRight
) {
    auto left = jLeft ? std::string(env->GetStringUTFChars(jLeft, nullptr)) : "";
    auto right = jRight ? std::string(env->GetStringUTFChars(jRight, nullptr)) : "";
    if (jLeft) env->ReleaseStringUTFChars(jLeft, left.c_str());
    if (jRight) env->ReleaseStringUTFChars(jRight, right.c_str());
    auto avg = progressive::stringAverage(left, right);
    return env->NewStringUTF(avg.c_str());
}

// --- Event Type Classifier ---
// Ported from: EventType.kt (146L), MessageType.kt (52L)

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeRouteEventForProcessing(
    JNIEnv* env, jclass, jstring jEventType, jstring jMsgType
) {
    auto et = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    auto mt = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    if (jEventType) env->ReleaseStringUTFChars(jEventType, et.c_str());
    if (jMsgType) env->ReleaseStringUTFChars(jMsgType, mt.c_str());
    auto route = progressive::routeEventForProcessing(et, mt);
    return env->NewStringUTF(route.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsCallEvent(
    JNIEnv* env, jclass, jstring jEventType
) {
    auto et = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    if (jEventType) env->ReleaseStringUTFChars(jEventType, et.c_str());
    return progressive::isCallEvent(et);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsVerificationEvent(
    JNIEnv* env, jclass, jstring jEventType
) {
    auto et = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    if (jEventType) env->ReleaseStringUTFChars(jEventType, et.c_str());
    return progressive::isVerificationEvent(et);
}

} // extern "C"
