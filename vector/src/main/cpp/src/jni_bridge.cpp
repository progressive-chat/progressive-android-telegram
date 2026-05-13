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

// --- Markdown ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMarkdownToHtml(
    JNIEnv* env, jclass, jstring jMarkdown, jboolean jEnableTables
) {
    if (!jMarkdown) return env->NewStringUTF("");
    auto md = std::string(env->GetStringUTFChars(jMarkdown, nullptr));
    env->ReleaseStringUTFChars(jMarkdown, md.c_str());

    MdConfig config;
    config.enableTables = jEnableTables;
    auto html = progressive::markdownToHtml(md, config);
    return env->NewStringUTF(html.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseMarkdownTable(
    JNIEnv* env, jclass, jstring jTableBlock, jboolean jWithScroll
) {
    if (!jTableBlock) return env->NewStringUTF("");
    auto block = std::string(env->GetStringUTFChars(jTableBlock, nullptr));
    env->ReleaseStringUTFChars(jTableBlock, block.c_str());

    auto html = progressive::parseMarkdownTable(block, jWithScroll);
    return env->NewStringUTF(html.c_str());
}

// --- Account Export ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEncryptAccount(
    JNIEnv* env, jclass,
    jstring jUserId, jstring jToken, jstring jRefreshToken,
    jstring jHomeServer, jstring jDeviceId, jstring jDeviceName,
    jstring jDisplayName, jstring jAvatarUrl,
    jboolean jIncludeCache, jstring jPassphrase
) {
    AccountData data;
    data.userId       = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    data.accessToken  = jToken ? std::string(env->GetStringUTFChars(jToken, nullptr)) : "";
    data.refreshToken = jRefreshToken ? std::string(env->GetStringUTFChars(jRefreshToken, nullptr)) : "";
    data.homeServerUrl = jHomeServer ? std::string(env->GetStringUTFChars(jHomeServer, nullptr)) : "";
    data.deviceId     = jDeviceId ? std::string(env->GetStringUTFChars(jDeviceId, nullptr)) : "";
    data.deviceName   = jDeviceName ? std::string(env->GetStringUTFChars(jDeviceName, nullptr)) : "";
    data.displayName  = jDisplayName ? std::string(env->GetStringUTFChars(jDisplayName, nullptr)) : "";
    data.avatarUrl    = jAvatarUrl ? std::string(env->GetStringUTFChars(jAvatarUrl, nullptr)) : "";
    data.includeCache = jIncludeCache;

    auto pass = jPassphrase ? std::string(env->GetStringUTFChars(jPassphrase, nullptr)) : "";

    // Release
    if (jUserId) env->ReleaseStringUTFChars(jUserId, data.userId.c_str());
    if (jToken) env->ReleaseStringUTFChars(jToken, data.accessToken.c_str());
    if (jRefreshToken) env->ReleaseStringUTFChars(jRefreshToken, data.refreshToken.c_str());
    if (jHomeServer) env->ReleaseStringUTFChars(jHomeServer, data.homeServerUrl.c_str());
    if (jDeviceId) env->ReleaseStringUTFChars(jDeviceId, data.deviceId.c_str());
    if (jDeviceName) env->ReleaseStringUTFChars(jDeviceName, data.deviceName.c_str());
    if (jDisplayName) env->ReleaseStringUTFChars(jDisplayName, data.displayName.c_str());
    if (jAvatarUrl) env->ReleaseStringUTFChars(jAvatarUrl, data.avatarUrl.c_str());
    if (jPassphrase) env->ReleaseStringUTFChars(jPassphrase, pass.c_str());

    auto result = progressive::encryptAccountData(data, pass);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDecryptAccount(
    JNIEnv* env, jclass, jstring jEncrypted, jstring jPassphrase
) {
    if (!jEncrypted || !jPassphrase) return env->NewStringUTF("");
    auto enc = std::string(env->GetStringUTFChars(jEncrypted, nullptr));
    auto pass = std::string(env->GetStringUTFChars(jPassphrase, nullptr));
    env->ReleaseStringUTFChars(jEncrypted, enc.c_str());
    env->ReleaseStringUTFChars(jPassphrase, pass.c_str());

    auto data = progressive::decryptAccountData(enc, pass);
    if (data.userId.empty()) return env->NewStringUTF(R"({"error": "Decryption failed"})");

    auto json = progressive::accountToJson(data);
    return env->NewStringUTF(json.c_str());
}

// --- Audio ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatDuration(
    JNIEnv* env, jclass, jlong jMs
) {
    auto s = progressive::formatDuration(jMs);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatPositionInfo(
    JNIEnv* env, jclass, jlong jPos, jlong jDur
) {
    auto s = progressive::formatPositionInfo(jPos, jDur);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jfloat JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeProgress(
    JNIEnv*, jclass, jlong jPos, jlong jDur
) {
    return progressive::computeProgress(jPos, jDur);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsSupportedAudio(
    JNIEnv* env, jclass, jstring jMime
) {
    if (!jMime) return JNI_FALSE;
    auto mime = std::string(env->GetStringUTFChars(jMime, nullptr));
    env->ReleaseStringUTFChars(jMime, mime.c_str());
    return progressive::isSupportedAudioType(mime) ? JNI_TRUE : JNI_FALSE;
}

// --- Media Filter ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetFileExtension(
    JNIEnv* env, jclass, jstring jFileName, jstring jMimeType
) {
    auto fn = jFileName ? std::string(env->GetStringUTFChars(jFileName, nullptr)) : "";
    auto mt = jMimeType ? std::string(env->GetStringUTFChars(jMimeType, nullptr)) : "";
    if (jFileName) env->ReleaseStringUTFChars(jFileName, fn.c_str());
    if (jMimeType) env->ReleaseStringUTFChars(jMimeType, mt.c_str());
    auto ext = progressive::getFileExtension(fn, mt);
    return env->NewStringUTF(ext.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidMxcUri(
    JNIEnv* env, jclass, jstring jUri
) {
    if (!jUri) return JNI_FALSE;
    auto uri = std::string(env->GetStringUTFChars(jUri, nullptr));
    env->ReleaseStringUTFChars(jUri, uri.c_str());
    return progressive::isValidMxcUri(uri) ? JNI_TRUE : JNI_FALSE;
}

// --- Content Filter ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeKeywordFilterLoad(
    JNIEnv* env, jclass, jstring jRaw
) {
    if (!jRaw) return;
    auto raw = std::string(env->GetStringUTFChars(jRaw, nullptr));
    env->ReleaseStringUTFChars(jRaw, raw.c_str());
    g_keywordFilter.loadKeywords(raw);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeKeywordFilterCheck(
    JNIEnv* env, jclass, jstring jText
) {
    if (!jText) return env->NewStringUTF("");
    auto text = std::string(env->GetStringUTFChars(jText, nullptr));
    env->ReleaseStringUTFChars(jText, text.c_str());
    auto match = g_keywordFilter.check(text);
    return env->NewStringUTF(match.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeKeywordFilterExport(
    JNIEnv* env, jclass
) {
    auto s = g_keywordFilter.exportKeywords();
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeKeywordFilterCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_keywordFilter.count());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeKeywordFilterClear(
    JNIEnv*, jclass
) {
    g_keywordFilter.clear();
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldBlockImage(
    JNIEnv* env, jclass,
    jboolean jBlockAll, jboolean jAllowAvatars, jboolean jAllowStickers, jboolean jAllowEmoji,
    jstring jMxcUrl, jstring jImageType
) {
    ImagePolicy policy;
    policy.blockAllRemote = jBlockAll;
    policy.allowAvatars   = jAllowAvatars;
    policy.allowStickers  = jAllowStickers;
    policy.allowEmoji     = jAllowEmoji;

    auto url  = jMxcUrl ? std::string(env->GetStringUTFChars(jMxcUrl, nullptr)) : "";
    auto type = jImageType ? std::string(env->GetStringUTFChars(jImageType, nullptr)) : "";
    if (jMxcUrl) env->ReleaseStringUTFChars(jMxcUrl, url.c_str());
    if (jImageType) env->ReleaseStringUTFChars(jImageType, type.c_str());

    return policy.shouldBlock(url, type) ? JNI_TRUE : JNI_FALSE;
}

// --- Network Stats ---

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNetStatsStart(
    JNIEnv* env, jclass, jstring jUrl, jstring jMethod
) {
    auto url = jUrl ? std::string(env->GetStringUTFChars(jUrl, nullptr)) : "";
    auto method = jMethod ? std::string(env->GetStringUTFChars(jMethod, nullptr)) : "";
    if (jUrl) env->ReleaseStringUTFChars(jUrl, url.c_str());
    if (jMethod) env->ReleaseStringUTFChars(jMethod, method.c_str());
    return g_netStats.startRequest(url, method);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNetStatsEnd(
    JNIEnv* env, jclass,
    jint jRequestId, jint jStatusCode, jlong jBytesSent, jlong jBytesReceived,
    jstring jError
) {
    auto err = jError ? std::string(env->GetStringUTFChars(jError, nullptr)) : "";
    if (jError) env->ReleaseStringUTFChars(jError, err.c_str());
    g_netStats.endRequest(jRequestId, jStatusCode, jBytesSent, jBytesReceived, err);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNetStatsToJson(
    JNIEnv* env, jclass
) {
    auto json = g_netStats.statsToJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNetStatsToText(
    JNIEnv* env, jclass
) {
    auto text = g_netStats.statsToText();
    return env->NewStringUTF(text.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNetStatsClear(
    JNIEnv*, jclass
) {
    g_netStats.clear();
}

// --- Masquerade ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidMasqueradeName(
    JNIEnv* env, jclass, jstring jName
) {
    if (!jName) return JNI_FALSE;
    auto name = std::string(env->GetStringUTFChars(jName, nullptr));
    env->ReleaseStringUTFChars(jName, name.c_str());
    return progressive::isValidMasqueradeName(name) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetSuggestedMasqueradeNames(
    JNIEnv* env, jclass
) {
    auto names = progressive::getSuggestedNames();
    return env->NewStringUTF(names.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidIconAlias(
    JNIEnv* env, jclass, jstring jAlias
) {
    if (!jAlias) return JNI_FALSE;
    auto alias = std::string(env->GetStringUTFChars(jAlias, nullptr));
    env->ReleaseStringUTFChars(jAlias, alias.c_str());
    return progressive::isValidIconAlias(alias) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildMasqueradeAlias(
    JNIEnv* env, jclass, jstring jBase, jstring jIcon
) {
    auto base = jBase ? std::string(env->GetStringUTFChars(jBase, nullptr)) : "";
    auto icon = jIcon ? std::string(env->GetStringUTFChars(jIcon, nullptr)) : "";
    if (jBase) env->ReleaseStringUTFChars(jBase, base.c_str());
    if (jIcon) env->ReleaseStringUTFChars(jIcon, icon.c_str());
    auto alias = progressive::buildMasqueradeAlias(base, icon);
    return env->NewStringUTF(alias.c_str());
}

// --- User Mask ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskSet(
    JNIEnv* env, jclass,
    jstring jMxid, jstring jDisplayName, jstring jAvatarUrl, jstring jOverrideMxid
) {
    UserMask mask;
    mask.originalMxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    mask.displayName  = jDisplayName ? std::string(env->GetStringUTFChars(jDisplayName, nullptr)) : "";
    mask.avatarUrl    = jAvatarUrl ? std::string(env->GetStringUTFChars(jAvatarUrl, nullptr)) : "";
    mask.overrideMxid = jOverrideMxid ? std::string(env->GetStringUTFChars(jOverrideMxid, nullptr)) : "";

    if (jMxid) env->ReleaseStringUTFChars(jMxid, mask.originalMxid.c_str());
    if (jDisplayName) env->ReleaseStringUTFChars(jDisplayName, mask.displayName.c_str());
    if (jAvatarUrl) env->ReleaseStringUTFChars(jAvatarUrl, mask.avatarUrl.c_str());
    if (jOverrideMxid) env->ReleaseStringUTFChars(jOverrideMxid, mask.overrideMxid.c_str());

    g_userMasks.setMask(mask);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskRemove(
    JNIEnv* env, jclass, jstring jMxid
) {
    if (!jMxid) return;
    auto mxid = std::string(env->GetStringUTFChars(jMxid, nullptr));
    env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    g_userMasks.removeMask(mxid);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskResolveName(
    JNIEnv* env, jclass, jstring jMxid, jstring jOriginalName
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    auto orig = jOriginalName ? std::string(env->GetStringUTFChars(jOriginalName, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    if (jOriginalName) env->ReleaseStringUTFChars(jOriginalName, orig.c_str());

    auto resolved = progressive::resolveDisplayName(mxid, orig, g_userMasks);
    return env->NewStringUTF(resolved.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskResolveAvatar(
    JNIEnv* env, jclass, jstring jMxid, jstring jOriginalUrl
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    auto orig = jOriginalUrl ? std::string(env->GetStringUTFChars(jOriginalUrl, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    if (jOriginalUrl) env->ReleaseStringUTFChars(jOriginalUrl, orig.c_str());

    auto resolved = progressive::resolveAvatarUrl(mxid, orig, g_userMasks);
    return env->NewStringUTF(resolved.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_userMasks.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskImportJson(
    JNIEnv* env, jclass, jstring jJson
) {
    if (!jJson) return;
    auto json = std::string(env->GetStringUTFChars(jJson, nullptr));
    env->ReleaseStringUTFChars(jJson, json.c_str());
    g_userMasks.importJson(json);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidMxid(
    JNIEnv* env, jclass, jstring jMxid
) {
    if (!jMxid) return JNI_FALSE;
    auto mxid = std::string(env->GetStringUTFChars(jMxid, nullptr));
    env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    return progressive::isValidMxid(mxid) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskClear(
    JNIEnv*, jclass
) {
    g_userMasks.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserMaskCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_userMasks.count());
}

// --- Chunked Upload ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderSetChunkSizeMb(
    JNIEnv*, jclass, jint jMb
) {
    g_uploader.setChunkSizeMb(jMb);
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderComputeChunks(
    JNIEnv*, jclass, jlong jFileSize
) {
    return g_uploader.computeChunks(jFileSize);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderGetChunkJson(
    JNIEnv* env, jclass, jint jIndex
) {
    auto chunk = g_uploader.getChunkInfo(jIndex);
    auto json = progressive::ChunkedUploader::chunkToJson(chunk);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderAdvance(
    JNIEnv*, jclass
) {
    g_uploader.advanceChunk();
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderCancel(
    JNIEnv*, jclass
) {
    g_uploader.cancel();
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderFail(
    JNIEnv* env, jclass, jstring jError
) {
    auto err = jError ? std::string(env->GetStringUTFChars(jError, nullptr)) : "";
    if (jError) env->ReleaseStringUTFChars(jError, err.c_str());
    g_uploader.fail(err);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderReset(
    JNIEnv*, jclass
) {
    g_uploader.reset();
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderProgressJson(
    JNIEnv* env, jclass
) {
    auto prog = g_uploader.progress();
    // Return progress as JSON
    std::ostringstream json;
    json << "{";
    json << R"("bytesUploaded": )" << prog.bytesUploaded << ",";
    json << R"("totalBytes": )" << prog.totalBytes << ",";
    json << R"("chunksCompleted": )" << prog.chunksCompleted << ",";
    json << R"("totalChunks": )" << prog.totalChunks << ",";
    json << R"("progress": )" << prog.progress() << ",";
    json << R"("done": )" << (prog.done ? "true" : "false") << ",";
    json << R"("cancelled": )" << (prog.cancelled ? "true" : "false") << ",";
    json << R"("failed": )" << (prog.failed ? "true" : "false");
    if (!prog.error.empty()) json << R"(,"error": ")" << prog.error << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUploaderContentRange(
    JNIEnv* env, jclass, jint jIndex
) {
    auto chunk = g_uploader.getChunkInfo(jIndex);
    auto range = progressive::ChunkedUploader::formatContentRange(chunk);
    return env->NewStringUTF(range.c_str());
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSuggestChunkSizeMb(
    JNIEnv*, jclass, jlong jFileSize
) {
    return progressive::ChunkedUploader::suggestChunkSizeMb(jFileSize);
}

// --- Chat Features (Timezone + EXIF) ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetCommonTimezones(
    JNIEnv* env, jclass
) {
    auto zones = progressive::getCommonTimezones();
    // Return as JSON array
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < zones.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"id": ")" << zones[i].id << R"(",)";
        json << R"("name": ")" << zones[i].displayName << R"(",)";
        json << R"("offset": )" << zones[i].utcOffsetMinutes << "}";
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatTimestampInTimezone(
    JNIEnv* env, jclass, jlong jUtcMs, jstring jTzId
) {
    auto tzId = jTzId ? std::string(env->GetStringUTFChars(jTzId, nullptr)) : "UTC";
    if (jTzId) env->ReleaseStringUTFChars(jTzId, tzId.c_str());

    auto formatted = progressive::formatTimestampInTimezone(jUtcMs, tzId);
    return env->NewStringUTF(formatted.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidTimezoneId(
    JNIEnv* env, jclass, jstring jTzId
) {
    auto tzId = jTzId ? std::string(env->GetStringUTFChars(jTzId, nullptr)) : "";
    if (jTzId) env->ReleaseStringUTFChars(jTzId, tzId.c_str());
    return progressive::isValidTimezoneId(tzId) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFileHasMetadata(
    JNIEnv* env, jclass, jstring jMimeType
) {
    auto mime = jMimeType ? std::string(env->GetStringUTFChars(jMimeType, nullptr)) : "";
    if (jMimeType) env->ReleaseStringUTFChars(jMimeType, mime.c_str());
    return progressive::fileHasMetadata(mime) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetStrippableMimeTypes(
    JNIEnv* env, jclass
) {
    auto s = progressive::getStrippableMimeTypes();
    return env->NewStringUTF(s.c_str());
}

// --- Invitation Hide ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteHide(
    JNIEnv* env, jclass,
    jstring jRoomId, jstring jRoomName, jstring jInviterName, jstring jInviterMxid
) {
    HiddenInvitation inv;
    inv.roomId      = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    inv.roomName    = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    inv.inviterName = jInviterName ? std::string(env->GetStringUTFChars(jInviterName, nullptr)) : "";
    inv.inviterMxid = jInviterMxid ? std::string(env->GetStringUTFChars(jInviterMxid, nullptr)) : "";

    if (jRoomId)     env->ReleaseStringUTFChars(jRoomId, inv.roomId.c_str());
    if (jRoomName)   env->ReleaseStringUTFChars(jRoomName, inv.roomName.c_str());
    if (jInviterName) env->ReleaseStringUTFChars(jInviterName, inv.inviterName.c_str());
    if (jInviterMxid) env->ReleaseStringUTFChars(jInviterMxid, inv.inviterMxid.c_str());

    g_inviteHide.hide(inv);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteUnhide(
    JNIEnv* env, jclass, jstring jRoomId
) {
    if (!jRoomId) return;
    auto id = std::string(env->GetStringUTFChars(jRoomId, nullptr));
    env->ReleaseStringUTFChars(jRoomId, id.c_str());
    g_inviteHide.unhide(id);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteIsHidden(
    JNIEnv* env, jclass, jstring jRoomId
) {
    if (!jRoomId) return JNI_FALSE;
    auto id = std::string(env->GetStringUTFChars(jRoomId, nullptr));
    env->ReleaseStringUTFChars(jRoomId, id.c_str());
    return g_inviteHide.isHidden(id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_inviteHide.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteImportJson(
    JNIEnv* env, jclass, jstring jJson
) {
    if (!jJson) return;
    auto json = std::string(env->GetStringUTFChars(jJson, nullptr));
    env->ReleaseStringUTFChars(jJson, json.c_str());
    g_inviteHide.importJson(json);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteClear(
    JNIEnv*, jclass
) {
    g_inviteHide.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeInviteCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_inviteHide.count());
}

// --- Thread Aggregator ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeThreadAdd(
    JNIEnv* env, jclass,
    jstring jThreadId, jstring jRoomId, jstring jRoomName,
    jstring jAccountId, jstring jAccountIndex,
    jstring jLastMsg, jstring jLastSender,
    jlong jLastTs, jint jReplyCount, jboolean jUnread
) {
    ThreadInfo t;
    t.threadId     = jThreadId ? std::string(env->GetStringUTFChars(jThreadId, nullptr)) : "";
    t.roomId       = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    t.roomName     = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    t.accountId    = jAccountId ? std::string(env->GetStringUTFChars(jAccountId, nullptr)) : "";
    t.accountIndex = jAccountIndex ? std::string(env->GetStringUTFChars(jAccountIndex, nullptr)) : "";
    t.lastMessage  = jLastMsg ? std::string(env->GetStringUTFChars(jLastMsg, nullptr)) : "";
    t.lastSender   = jLastSender ? std::string(env->GetStringUTFChars(jLastSender, nullptr)) : "";
    t.lastTimestamp = jLastTs;
    t.replyCount   = jReplyCount;
    t.unread       = jUnread;

    if (jThreadId)   env->ReleaseStringUTFChars(jThreadId, t.threadId.c_str());
    if (jRoomId)     env->ReleaseStringUTFChars(jRoomId, t.roomId.c_str());
    if (jRoomName)   env->ReleaseStringUTFChars(jRoomName, t.roomName.c_str());
    if (jAccountId)  env->ReleaseStringUTFChars(jAccountId, t.accountId.c_str());
    if (jAccountIndex) env->ReleaseStringUTFChars(jAccountIndex, t.accountIndex.c_str());
    if (jLastMsg)    env->ReleaseStringUTFChars(jLastMsg, t.lastMessage.c_str());
    if (jLastSender) env->ReleaseStringUTFChars(jLastSender, t.lastSender.c_str());

    g_threadAgg.addThread(t);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeThreadGetAllJson(
    JNIEnv* env, jclass
) {
    auto json = g_threadAgg.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeThreadClear(
    JNIEnv*, jclass
) {
    g_threadAgg.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeThreadCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_threadAgg.count());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeThreadRemoveRoom(
    JNIEnv* env, jclass, jstring jRoomId
) {
    if (!jRoomId) return;
    auto id = std::string(env->GetStringUTFChars(jRoomId, nullptr));
    env->ReleaseStringUTFChars(jRoomId, id.c_str());
    g_threadAgg.removeRoom(id);
}

// --- User Messages ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatUserMessagePreview(
    JNIEnv* env, jclass,
    jstring jRoomName, jstring jBody, jstring jMsgType, jint jMaxLen
) {
    UserMessage msg;
    msg.roomName = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    msg.body     = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    msg.msgType  = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";

    if (jRoomName) env->ReleaseStringUTFChars(jRoomName, msg.roomName.c_str());
    if (jBody)     env->ReleaseStringUTFChars(jBody, msg.body.c_str());
    if (jMsgType)  env->ReleaseStringUTFChars(jMsgType, msg.msgType.c_str());

    auto preview = progressive::formatUserMessagePreview(msg, jMaxLen);
    return env->NewStringUTF(preview.c_str());
}

// --- Room Version ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetRoomVersionsJson(
    JNIEnv* env, jclass
) {
    auto json = progressive::roomVersionsToJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidRoomVersion(
    JNIEnv* env, jclass, jstring jVersion
) {
    auto v = jVersion ? std::string(env->GetStringUTFChars(jVersion, nullptr)) : "";
    if (jVersion) env->ReleaseStringUTFChars(jVersion, v.c_str());
    return progressive::isValidRoomVersion(v) ? JNI_TRUE : JNI_FALSE;
}

// --- Chat Preview ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatShortTime(
    JNIEnv* env, jclass, jlong jEpochMs
) {
    auto s = progressive::formatShortTime(jEpochMs);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeTruncateMessage(
    JNIEnv* env, jclass, jstring jBody, jint jMaxLen
) {
    auto body = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jBody) env->ReleaseStringUTFChars(jBody, body.c_str());
    auto s = progressive::truncateMessage(body, jMaxLen);
    return env->NewStringUTF(s.c_str());
}

// --- RAM Monitor ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetMemoryInfo(
    JNIEnv* env, jclass
) {
    auto info = progressive::getMemoryInfo();
    auto json = progressive::memoryInfoToJson(info);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatMemoryLabel(
    JNIEnv* env, jclass, jlong jRssKb
) {
    auto label = progressive::formatMemoryLabel(jRssKb);
    return env->NewStringUTF(label.c_str());
}

// --- Cache Manager ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheTrack(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jRoomName,
    jlong jTimestamp, jlong jSizeBytes, jstring jMsgType, jstring jBody
) {
    auto eventId  = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    auto roomId   = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto roomName = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    auto msgType  = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    auto body     = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";

    if (jEventId)  env->ReleaseStringUTFChars(jEventId, eventId.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jRoomName) env->ReleaseStringUTFChars(jRoomName, roomName.c_str());
    if (jMsgType)  env->ReleaseStringUTFChars(jMsgType, msgType.c_str());
    if (jBody)     env->ReleaseStringUTFChars(jBody, body.c_str());

    g_cacheMgr.track(eventId, roomId, roomName, jTimestamp, jSizeBytes, msgType, body);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheStatsJson(
    JNIEnv* env, jclass
) {
    auto json = g_cacheMgr.statsToJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheGetByRoom(
    JNIEnv* env, jclass, jstring jRoomId
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    auto entries = g_cacheMgr.getByRoom(roomId);
    auto json = progressive::CacheManager::entriesToJson(entries);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheGetOlderThan(
    JNIEnv* env, jclass, jlong jBeforeTs
) {
    auto entries = g_cacheMgr.getOlderThan(jBeforeTs);
    auto json = progressive::CacheManager::entriesToJson(entries);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheClear(
    JNIEnv*, jclass
) {
    g_cacheMgr.clear();
}

// --- Message Aggregator ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgAggAdd(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jRoomName,
    jstring jAccountId, jstring jAccountIndex,
    jstring jSenderName, jstring jBody, jstring jMsgType,
    jlong jTs
) {
    AggregatedMessage m;
    m.eventId      = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    m.roomId       = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    m.roomName     = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    m.accountId    = jAccountId ? std::string(env->GetStringUTFChars(jAccountId, nullptr)) : "";
    m.accountIndex = jAccountIndex ? std::string(env->GetStringUTFChars(jAccountIndex, nullptr)) : "";
    m.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    m.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    m.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    m.originServerTs = jTs;

    if (jEventId)  env->ReleaseStringUTFChars(jEventId, m.eventId.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, m.roomId.c_str());
    if (jRoomName) env->ReleaseStringUTFChars(jRoomName, m.roomName.c_str());
    if (jAccountId) env->ReleaseStringUTFChars(jAccountId, m.accountId.c_str());
    if (jAccountIndex) env->ReleaseStringUTFChars(jAccountIndex, m.accountIndex.c_str());
    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, m.senderName.c_str());
    if (jBody)     env->ReleaseStringUTFChars(jBody, m.body.c_str());
    if (jMsgType)  env->ReleaseStringUTFChars(jMsgType, m.msgType.c_str());

    g_msgAgg.addMessage(m);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgAggGetAllJson(
    JNIEnv* env, jclass
) {
    auto json = g_msgAgg.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgAggClear(
    JNIEnv*, jclass
) {
    g_msgAgg.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgAggCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_msgAgg.count());
}

// --- Room Info ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatCreationDate(
    JNIEnv* env, jclass, jlong jEpochMs
) {
    auto s = progressive::formatCreationDate(jEpochMs);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsLikelyFullHistory(
    JNIEnv*, jclass, jint jCached, jint jEstimated
) {
    return progressive::isLikelyFullHistory(jCached, jEstimated) ? JNI_TRUE : JNI_FALSE;
}

// --- Deleted Archive ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDeletedArchiveAdd(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jRoomName,
    jstring jSenderName, jstring jBody, jstring jMsgType,
    jstring jTimestamp, jlong jOriginTs, jstring jDeletedBy
) {
    DeletedEvent e;
    e.eventId       = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    e.roomId        = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    e.roomName      = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    e.senderName    = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    e.body          = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    e.msgType       = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    e.timestamp     = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    e.originServerTs = jOriginTs;
    e.deletedBy     = jDeletedBy ? std::string(env->GetStringUTFChars(jDeletedBy, nullptr)) : "";

    if (jEventId)  env->ReleaseStringUTFChars(jEventId, e.eventId.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, e.roomId.c_str());
    if (jRoomName) env->ReleaseStringUTFChars(jRoomName, e.roomName.c_str());
    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, e.senderName.c_str());
    if (jBody)     env->ReleaseStringUTFChars(jBody, e.body.c_str());
    if (jMsgType)  env->ReleaseStringUTFChars(jMsgType, e.msgType.c_str());
    if (jTimestamp) env->ReleaseStringUTFChars(jTimestamp, e.timestamp.c_str());
    if (jDeletedBy) env->ReleaseStringUTFChars(jDeletedBy, e.deletedBy.c_str());

    g_deletedArchive.archive(e);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDeletedArchiveExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_deletedArchive.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDeletedArchiveClear(
    JNIEnv*, jclass
) {
    g_deletedArchive.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDeletedArchiveCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_deletedArchive.count());
}

// --- Search Index ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSearchIndexMessage(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jRoomName,
    jstring jSenderName, jstring jBody, jlong jTimestamp, jboolean jEncrypted
) {
    auto eventId    = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    auto roomId     = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto roomName   = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    auto senderName = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    auto body       = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";

    if (jEventId)  env->ReleaseStringUTFChars(jEventId, eventId.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jRoomName) env->ReleaseStringUTFChars(jRoomName, roomName.c_str());
    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, senderName.c_str());
    if (jBody)     env->ReleaseStringUTFChars(jBody, body.c_str());

    g_searchIndex.indexMessage(eventId, roomId, roomName, senderName, body, jTimestamp, jEncrypted);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSearchQuery(
    JNIEnv* env, jclass, jstring jQuery, jint jLimit
) {
    auto query = jQuery ? std::string(env->GetStringUTFChars(jQuery, nullptr)) : "";
    if (jQuery) env->ReleaseStringUTFChars(jQuery, query.c_str());

    auto result = g_searchIndex.search(query, jLimit);

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("query": ")" << esc(result.query) << R"(",)";
    json << R"("totalHits": )" << result.totalHits << ",";
    json << R"("roomsSearched": )" << result.roomsSearched << ",";
    json << R"("searchTimeMs": )" << result.searchTimeMs << ",";
    json << R"("hits": )" << progressive::SearchIndex::hitsToJson(result.hits);
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSearchClear(
    JNIEnv*, jclass
) {
    g_searchIndex.clear();
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSearchIndexedCount(
    JNIEnv*, jclass
) {
    return static_cast<jint>(g_searchIndex.indexedCount());
}

// --- Module Loader ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeModuleScanDir(
    JNIEnv* env, jclass, jstring jDir
) {
    auto dir = jDir ? std::string(env->GetStringUTFChars(jDir, nullptr)) : "";
    if (jDir) env->ReleaseStringUTFChars(jDir, dir.c_str());
    g_moduleLoader.scanDirectory(dir);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeModuleEnable(
    JNIEnv* env, jclass, jstring jName
) {
    auto name = jName ? std::string(env->GetStringUTFChars(jName, nullptr)) : "";
    if (jName) env->ReleaseStringUTFChars(jName, name.c_str());
    g_moduleLoader.enable(name);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeModuleIsEnabled(
    JNIEnv* env, jclass, jstring jName
) {
    auto name = jName ? std::string(env->GetStringUTFChars(jName, nullptr)) : "";
    if (jName) env->ReleaseStringUTFChars(jName, name.c_str());
    return g_moduleLoader.isEnabled(name) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeModuleListJson(
    JNIEnv* env, jclass
) {
    auto json = g_moduleLoader.listModulesJson();
    return env->NewStringUTF(json.c_str());
}

// --- Notification Keywords ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNotifKeywordAdd(
    JNIEnv* env, jclass, jstring jKeyword, jboolean jCaseSensitive
) {
    auto kw = jKeyword ? std::string(env->GetStringUTFChars(jKeyword, nullptr)) : "";
    if (jKeyword) env->ReleaseStringUTFChars(jKeyword, kw.c_str());
    g_notifKeywords.addKeyword(kw, jCaseSensitive);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNotifKeywordCheck(
    JNIEnv* env, jclass, jstring jBody
) {
    auto body = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jBody) env->ReleaseStringUTFChars(jBody, body.c_str());
    auto match = g_notifKeywords.check(body);
    return env->NewStringUTF(match.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNotifKeywordExport(
    JNIEnv* env, jclass
) {
    auto json = g_notifKeywords.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNotifKeywordImport(
    JNIEnv* env, jclass, jstring jJson
) {
    if (!jJson) return;
    auto json = std::string(env->GetStringUTFChars(jJson, nullptr));
    env->ReleaseStringUTFChars(jJson, json.c_str());
    g_notifKeywords.importJson(json);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeNotifKeywordClear(
    JNIEnv*, jclass
) {
    g_notifKeywords.clear();
}

// --- Reaction Preview ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatReactionPreview(
    JNIEnv* env, jclass,
    jstring jReactorName, jstring jEmoji, jstring jSourceBody, jstring jSourceSender
) {
    ReactionPreview rp;
    rp.reactorName      = jReactorName ? std::string(env->GetStringUTFChars(jReactorName, nullptr)) : "";
    rp.reactionEmoji    = jEmoji ? std::string(env->GetStringUTFChars(jEmoji, nullptr)) : "";
    rp.sourceBody       = jSourceBody ? std::string(env->GetStringUTFChars(jSourceBody, nullptr)) : "";
    rp.sourceSenderName = jSourceSender ? std::string(env->GetStringUTFChars(jSourceSender, nullptr)) : "";

    if (jReactorName)  env->ReleaseStringUTFChars(jReactorName, rp.reactorName.c_str());
    if (jEmoji)        env->ReleaseStringUTFChars(jEmoji, rp.reactionEmoji.c_str());
    if (jSourceBody)   env->ReleaseStringUTFChars(jSourceBody, rp.sourceBody.c_str());
    if (jSourceSender) env->ReleaseStringUTFChars(jSourceSender, rp.sourceSenderName.c_str());

    auto s = progressive::formatReactionPreview(rp);
    return env->NewStringUTF(s.c_str());
}

// --- Room Mirror ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorAdd(
    JNIEnv* env, jclass,
    jstring jSrcRoomId, jstring jSrcRoomName,
    jstring jMirRoomId, jstring jMirRoomName,
    jboolean jEnabled, jboolean jUseDolls
) {
    MirrorConfig cfg;
    cfg.sourceRoomId   = jSrcRoomId ? std::string(env->GetStringUTFChars(jSrcRoomId, nullptr)) : "";
    cfg.sourceRoomName = jSrcRoomName ? std::string(env->GetStringUTFChars(jSrcRoomName, nullptr)) : "";
    cfg.mirrorRoomId   = jMirRoomId ? std::string(env->GetStringUTFChars(jMirRoomId, nullptr)) : "";
    cfg.mirrorRoomName = jMirRoomName ? std::string(env->GetStringUTFChars(jMirRoomName, nullptr)) : "";
    cfg.enabled  = jEnabled;
    cfg.useDolls = jUseDolls;

    if (jSrcRoomId)   env->ReleaseStringUTFChars(jSrcRoomId, cfg.sourceRoomId.c_str());
    if (jSrcRoomName) env->ReleaseStringUTFChars(jSrcRoomName, cfg.sourceRoomName.c_str());
    if (jMirRoomId)   env->ReleaseStringUTFChars(jMirRoomId, cfg.mirrorRoomId.c_str());
    if (jMirRoomName) env->ReleaseStringUTFChars(jMirRoomName, cfg.mirrorRoomName.c_str());

    g_mirrorMgr.addMirror(cfg);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorRemove(
    JNIEnv* env, jclass, jstring jSrcRoomId
) {
    if (!jSrcRoomId) return;
    auto id = std::string(env->GetStringUTFChars(jSrcRoomId, nullptr));
    env->ReleaseStringUTFChars(jSrcRoomId, id.c_str());
    g_mirrorMgr.removeMirror(id);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorIsActive(
    JNIEnv* env, jclass, jstring jSrcRoomId
) {
    if (!jSrcRoomId) return JNI_FALSE;
    auto id = std::string(env->GetStringUTFChars(jSrcRoomId, nullptr));
    env->ReleaseStringUTFChars(jSrcRoomId, id.c_str());
    return g_mirrorMgr.isMirroring(id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorFormatMessage(
    JNIEnv* env, jclass,
    jstring jSenderName, jstring jSenderMxid, jstring jRoomName,
    jstring jBody, jstring jMsgType, jlong jTs
) {
    MirrorMessage msg;
    msg.senderName   = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    msg.senderMxid   = jSenderMxid ? std::string(env->GetStringUTFChars(jSenderMxid, nullptr)) : "";
    msg.sourceRoomName = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    msg.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    msg.msgType      = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    msg.timestamp    = jTs;

    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, msg.senderName.c_str());
    if (jSenderMxid) env->ReleaseStringUTFChars(jSenderMxid, msg.senderMxid.c_str());
    if (jRoomName)   env->ReleaseStringUTFChars(jRoomName, msg.sourceRoomName.c_str());
    if (jBody)       env->ReleaseStringUTFChars(jBody, msg.body.c_str());
    if (jMsgType)    env->ReleaseStringUTFChars(jMsgType, msg.msgType.c_str());

    auto s = progressive::RoomMirrorManager::formatMirrorMessage(msg);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorGenerateDollMxid(
    JNIEnv* env, jclass,
    jstring jOriginalMxid, jstring jTargetServer
) {
    auto mxid  = jOriginalMxid ? std::string(env->GetStringUTFChars(jOriginalMxid, nullptr)) : "";
    auto server = jTargetServer ? std::string(env->GetStringUTFChars(jTargetServer, nullptr)) : "";
    if (jOriginalMxid) env->ReleaseStringUTFChars(jOriginalMxid, mxid.c_str());
    if (jTargetServer) env->ReleaseStringUTFChars(jTargetServer, server.c_str());

    auto doll = progressive::RoomMirrorManager::generateDollMxid(mxid, server);
    return env->NewStringUTF(doll.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorIsValidDoll(
    JNIEnv* env, jclass, jstring jMxid
) {
    if (!jMxid) return JNI_FALSE;
    auto mxid = std::string(env->GetStringUTFChars(jMxid, nullptr));
    env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    return progressive::RoomMirrorManager::isValidDollMxid(mxid) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMirrorExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_mirrorMgr.exportJson();
    return env->NewStringUTF(json.c_str());
}

// --- Input Tools (Symbol Bar + Replacement Engine) ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSymbolAdd(
    JNIEnv* env, jclass, jstring jSymbol, jstring jLabel
) {
    auto sym = jSymbol ? std::string(env->GetStringUTFChars(jSymbol, nullptr)) : "";
    auto lbl = jLabel ? std::string(env->GetStringUTFChars(jLabel, nullptr)) : "";
    if (jSymbol) env->ReleaseStringUTFChars(jSymbol, sym.c_str());
    if (jLabel)  env->ReleaseStringUTFChars(jLabel, lbl.c_str());
    g_symbolBar.addSymbol(sym, lbl);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSymbolExport(
    JNIEnv* env, jclass
) {
    auto json = g_symbolBar.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSymbolImport(
    JNIEnv* env, jclass, jstring jJson
) {
    if (!jJson) return;
    auto json = std::string(env->GetStringUTFChars(jJson, nullptr));
    env->ReleaseStringUTFChars(jJson, json.c_str());
    g_symbolBar.importJson(json);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeReplacementAddRule(
    JNIEnv* env, jclass, jstring jPattern, jstring jReplacement, jboolean jExactMatch
) {
    auto pattern = jPattern ? std::string(env->GetStringUTFChars(jPattern, nullptr)) : "";
    auto replacement = jReplacement ? std::string(env->GetStringUTFChars(jReplacement, nullptr)) : "";
    if (jPattern) env->ReleaseStringUTFChars(jPattern, pattern.c_str());
    if (jReplacement) env->ReleaseStringUTFChars(jReplacement, replacement.c_str());
    g_replacementEngine.addRule(pattern, replacement, jExactMatch);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeReplacementApply(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto result = g_replacementEngine.apply(text);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeReplacementExport(
    JNIEnv* env, jclass
) {
    auto json = g_replacementEngine.exportJson();
    return env->NewStringUTF(json.c_str());
}

// --- LLM ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildLlmRequest(
    JNIEnv* env, jclass,
    jstring jPrompt, jint jProvider,
    jstring jEndpoint, jstring jToken, jstring jModel,
    jstring jSystemPrompt, jfloat jTemp, jint jMaxTokens
) {
    LlmConfig cfg;
    cfg.provider     = static_cast<LlmProvider>(jProvider);
    cfg.apiEndpoint  = jEndpoint ? std::string(env->GetStringUTFChars(jEndpoint, nullptr)) : "";
    cfg.apiToken     = jToken ? std::string(env->GetStringUTFChars(jToken, nullptr)) : "";
    cfg.model        = jModel ? std::string(env->GetStringUTFChars(jModel, nullptr)) : "gpt-4o-mini";
    cfg.systemPrompt = jSystemPrompt ? std::string(env->GetStringUTFChars(jSystemPrompt, nullptr)) : "";
    cfg.temperature  = jTemp;
    cfg.maxTokens    = jMaxTokens;

    auto prompt = jPrompt ? std::string(env->GetStringUTFChars(jPrompt, nullptr)) : "";

    if (jEndpoint) env->ReleaseStringUTFChars(jEndpoint, cfg.apiEndpoint.c_str());
    if (jToken)    env->ReleaseStringUTFChars(jToken, cfg.apiToken.c_str());
    if (jModel)    env->ReleaseStringUTFChars(jModel, cfg.model.c_str());
    if (jSystemPrompt) env->ReleaseStringUTFChars(jSystemPrompt, cfg.systemPrompt.c_str());
    if (jPrompt)   env->ReleaseStringUTFChars(jPrompt, prompt.c_str());

    auto body = progressive::buildLlmRequestBody(cfg, prompt);
    return env->NewStringUTF(body.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildLlmHeaders(
    JNIEnv* env, jclass, jint jProvider, jstring jToken
) {
    LlmConfig cfg;
    cfg.provider = static_cast<LlmProvider>(jProvider);
    cfg.apiToken = jToken ? std::string(env->GetStringUTFChars(jToken, nullptr)) : "";
    if (jToken) env->ReleaseStringUTFChars(jToken, cfg.apiToken.c_str());

    auto headers = progressive::buildLlmHeaders(cfg);
    return env->NewStringUTF(headers.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseLlmResponse(
    JNIEnv* env, jclass, jstring jBody, jint jStatusCode, jint jProvider
) {
    auto body = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jBody) env->ReleaseStringUTFChars(jBody, body.c_str());

    auto provider = static_cast<LlmProvider>(jProvider);
    auto result = progressive::parseLlmResponse(body, jStatusCode, provider);

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("success": )" << (result.success ? "true" : "false") << ",";
    json << R"("text": ")" << esc(result.text) << R"(",)";
    json << R"("errorMessage": ")" << esc(result.errorMessage) << R"(",)";
    json << R"("statusCode": )" << result.statusCode;
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatLlmBroadcast(
    JNIEnv* env, jclass, jstring jPrompt, jstring jResponse
) {
    auto prompt = jPrompt ? std::string(env->GetStringUTFChars(jPrompt, nullptr)) : "";
    auto response = jResponse ? std::string(env->GetStringUTFChars(jResponse, nullptr)) : "";
    if (jPrompt) env->ReleaseStringUTFChars(jPrompt, prompt.c_str());
    if (jResponse) env->ReleaseStringUTFChars(jResponse, response.c_str());

    auto s = progressive::formatLlmBroadcastMessage(prompt, response);
    return env->NewStringUTF(s.c_str());
}

// --- Duplicate Names ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatUserDisplayName(
    JNIEnv* env, jclass,
    jstring jDisplayName, jstring jMxid, jboolean jShowMxid
) {
    auto name = jDisplayName ? std::string(env->GetStringUTFChars(jDisplayName, nullptr)) : "";
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    if (jDisplayName) env->ReleaseStringUTFChars(jDisplayName, name.c_str());
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());

    auto s = progressive::formatUserDisplayName(name, mxid, jShowMxid);
    return env->NewStringUTF(s.c_str());
}

// --- User MXID Visibility ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMxidVisibilityHide(
    JNIEnv* env, jclass, jstring jMxid
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    g_mxidVisibility.hideMxid(mxid);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMxidVisibilityShow(
    JNIEnv* env, jclass, jstring jMxid
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    g_mxidVisibility.showMxid(mxid);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMxidVisibilityIsVisible(
    JNIEnv* env, jclass, jstring jMxid
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    return g_mxidVisibility.isVisible(mxid) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMxidVisibilityExport(
    JNIEnv* env, jclass
) {
    auto json = g_mxidVisibility.exportJson();
    return env->NewStringUTF(json.c_str());
}

// --- Read Receipts ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeReceiptDisplay(
    JNIEnv* env, jclass,
    jstring jReceiptsJson, jint jMaxVisible
) {
    auto json = jReceiptsJson ? std::string(env->GetStringUTFChars(jReceiptsJson, nullptr)) : "[]";
    if (jReceiptsJson) env->ReleaseStringUTFChars(jReceiptsJson, json.c_str());

    // Parse JSON array of receipts
    std::vector<ReceiptEntry> receipts;
    if (json != "[]") {
        size_t pos = 0;
        while (true) {
            pos = json.find("userId", pos);
            if (pos == std::string::npos) break;

            // Find the containing object
            auto objStart = json.rfind('{', pos);
            if (objStart == std::string::npos) break;

            int depth = 0;
            auto objEnd = objStart;
            while (objEnd < json.size()) {
                if (json[objEnd] == '{') ++depth;
                else if (json[objEnd] == '}') --depth;
                if (depth == 0) break;
                ++objEnd;
            }
            if (objEnd >= json.size()) break;

            std::string obj = json.substr(objStart, objEnd - objStart + 1);

            ReceiptEntry entry;

            auto uidSearch = std::string("\"userId\": \"");
            auto uidPos = obj.find(uidSearch);
            if (uidPos != std::string::npos) {
                uidPos += uidSearch.size();
                auto uidEnd = obj.find('"', uidPos);
                if (uidEnd != std::string::npos)
                    entry.userId = obj.substr(uidPos, uidEnd - uidPos);
            }

            auto dnSearch = std::string("\"displayName\": \"");
            auto dnPos = obj.find(dnSearch);
            if (dnPos != std::string::npos) {
                dnPos += dnSearch.size();
                auto dnEnd = obj.find('"', dnPos);
                if (dnEnd != std::string::npos)
                    entry.displayName = obj.substr(dnPos, dnEnd - dnPos);
            }

            auto avSearch = std::string("\"avatarUrl\": \"");
            auto avPos = obj.find(avSearch);
            if (avPos != std::string::npos) {
                avPos += avSearch.size();
                auto avEnd = obj.find('"', avPos);
                if (avEnd != std::string::npos)
                    entry.avatarUrl = obj.substr(avPos, avEnd - avPos);
            }

            auto tsSearch = std::string("\"timestamp\": ");
            auto tsPos = obj.find(tsSearch);
            if (tsPos != std::string::npos) {
                tsPos += tsSearch.size();
                while (tsPos < obj.size() && (obj[tsPos] == ' ' || obj[tsPos] == '\t')) ++tsPos;
                auto tsEnd = tsPos;
                while (tsEnd < obj.size() && obj[tsEnd] >= '0' && obj[tsEnd] <= '9') ++tsEnd;
                if (tsEnd > tsPos) {
                    entry.timestamp = std::stoll(obj.substr(tsPos, tsEnd - tsPos));
                }
            }

            if (!entry.userId.empty()) receipts.push_back(entry);
            pos = objEnd + 1;
        }
    }

    auto display = progressive::computeReceiptDisplay(receipts, jMaxVisible);
    auto result = progressive::receiptDisplayToJson(display);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatOverflowLabel(
    JNIEnv* env, jclass, jint jCount
) {
    auto s = progressive::formatOverflowLabel(jCount);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatReceiptAccessibility(
    JNIEnv* env, jclass, jstring jVisibleJson, jint jOverflow
) {
    // Parse visible entries JSON
    auto json = jVisibleJson ? std::string(env->GetStringUTFChars(jVisibleJson, nullptr)) : "[]";
    if (jVisibleJson) env->ReleaseStringUTFChars(jVisibleJson, json.c_str());

    std::vector<ReceiptEntry> entries;
    // Simplified: assume array of {"displayName": "..."}
    size_t pos = 0;
    while (true) {
        pos = json.find("displayName", pos);
        if (pos == std::string::npos) break;
        auto qStart = json.find('"', pos + 14);
        if (qStart == std::string::npos) break;
        ++qStart;
        auto qEnd = json.find('"', qStart);
        if (qEnd == std::string::npos) break;
        entries.push_back({.displayName = json.substr(qStart, qEnd - qStart)});
        pos = qEnd;
    }

    auto s = progressive::formatReceiptAccessibility(entries, jOverflow);
    return env->NewStringUTF(s.c_str());
}

// --- Room Analytics ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractServerName(
    JNIEnv* env, jclass, jstring jMxid
) {
    auto mxid = jMxid ? std::string(env->GetStringUTFChars(jMxid, nullptr)) : "";
    if (jMxid) env->ReleaseStringUTFChars(jMxid, mxid.c_str());
    auto s = progressive::extractServerName(mxid);
    return env->NewStringUTF(s.c_str());
}

// --- User Hide ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserHideFor(
    JNIEnv* env, jclass, jstring jUserId, jstring jDisplayName, jint jMinutes
) {
    auto uid = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    auto dn  = jDisplayName ? std::string(env->GetStringUTFChars(jDisplayName, nullptr)) : "";
    if (jUserId) env->ReleaseStringUTFChars(jUserId, uid.c_str());
    if (jDisplayName) env->ReleaseStringUTFChars(jDisplayName, dn.c_str());
    g_userHide.hideFor(uid, dn, jMinutes);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserHideIsHidden(
    JNIEnv* env, jclass, jstring jUserId
) {
    auto uid = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    if (jUserId) env->ReleaseStringUTFChars(jUserId, uid.c_str());
    return g_userHide.isHidden(uid) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUserHideGetActive(
    JNIEnv* env, jclass
) {
    g_userHide.cleanExpired();
    auto json = g_userHide.exportJson();
    return env->NewStringUTF(json.c_str());
}

// --- Message Queue ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueueEnqueue(
    JNIEnv* env, jclass,
    jstring jMsgId, jstring jRoomId, jstring jBody, jstring jFormattedBody,
    jint jOrder, jint jMaxRetries
) {
    QueuedMessage msg;
    msg.msgId        = jMsgId ? std::string(env->GetStringUTFChars(jMsgId, nullptr)) : "";
    msg.roomId       = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    msg.body         = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    msg.formattedBody = jFormattedBody ? std::string(env->GetStringUTFChars(jFormattedBody, nullptr)) : "";
    msg.order        = jOrder;
    msg.maxRetries   = jMaxRetries > 0 ? jMaxRetries : 5;

    if (jMsgId) env->ReleaseStringUTFChars(jMsgId, msg.msgId.c_str());
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, msg.roomId.c_str());
    if (jBody) env->ReleaseStringUTFChars(jBody, msg.body.c_str());
    if (jFormattedBody) env->ReleaseStringUTFChars(jFormattedBody, msg.formattedBody.c_str());

    g_msgQueue.enqueue(msg);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueueSetOrder(
    JNIEnv* env, jclass, jstring jMsgId, jint jOrder
) {
    auto id = jMsgId ? std::string(env->GetStringUTFChars(jMsgId, nullptr)) : "";
    if (jMsgId) env->ReleaseStringUTFChars(jMsgId, id.c_str());
    g_msgQueue.setOrder(id, jOrder);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueueMarkFailed(
    JNIEnv* env, jclass, jstring jMsgId, jstring jError
) {
    auto id = jMsgId ? std::string(env->GetStringUTFChars(jMsgId, nullptr)) : "";
    auto err = jError ? std::string(env->GetStringUTFChars(jError, nullptr)) : "";
    if (jMsgId) env->ReleaseStringUTFChars(jMsgId, id.c_str());
    if (jError) env->ReleaseStringUTFChars(jError, err.c_str());
    g_msgQueue.markFailed(id, err);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueueMarkSent(
    JNIEnv* env, jclass, jstring jMsgId
) {
    auto id = jMsgId ? std::string(env->GetStringUTFChars(jMsgId, nullptr)) : "";
    if (jMsgId) env->ReleaseStringUTFChars(jMsgId, id.c_str());
    g_msgQueue.markSent(id);
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueuePendingCount(
    JNIEnv*, jclass
) {
    return g_msgQueue.pendingCount();
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMsgQueueExport(
    JNIEnv* env, jclass
) {
    auto json = g_msgQueue.exportJson();
    return env->NewStringUTF(json.c_str());
}

// --- Image Crop ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidCrop(
    JNIEnv*, jclass, jint jImgW, jint jImgH, jint jX, jint jY, jint jW, jint jH
) {
    return progressive::isValidCrop(jImgW, jImgH, jX, jY, jW, jH) ? JNI_TRUE : JNI_FALSE;
}

// --- Auto-Scroll ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeScrollPlan(
    JNIEnv* env, jclass, jboolean jSmooth, jint jDurationMin, jint jTotalLines, jint jLineHeightPx
) {
    AutoScrollConfig cfg;
    cfg.smoothScroll = jSmooth;
    cfg.durationMinutes = jDurationMin;
    auto plan = progressive::computeScrollPlan(cfg, jTotalLines, jLineHeightPx);

    std::ostringstream json;
    json << "{";
    json << R"("totalLines": )" << plan.totalLines << ",";
    json << R"("linesPerMinute": )" << plan.linesPerMinute << ",";
    json << R"("scrollPxPerTick": )" << plan.scrollPxPerTick << ",";
    json << R"("estimatedFullScrollMin": )" << plan.estimatedFullScrollMin;
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

// --- Language Detection ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDetectLanguage(
    JNIEnv* env, jclass, jstring jText, jint jMethod
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());

    auto method = static_cast<DetectionMethod>(jMethod);
    auto result = progressive::detectLanguage(text, method);

    std::ostringstream json;
    json << R"({"langCode": ")" << result.langCode << R"(")";
    json << R"(,"langName": ")" << result.langName << R"(")";
    json << R"(,"confidence": )" << result.confidence;
    json << R"(,"label": ")" << progressive::getLanguageLabel(result.langCode) << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetLanguageLabel(
    JNIEnv* env, jclass, jstring jCode
) {
    auto code = jCode ? std::string(env->GetStringUTFChars(jCode, nullptr)) : "";
    if (jCode) env->ReleaseStringUTFChars(jCode, code.c_str());
    auto label = progressive::getLanguageLabel(code);
    return env->NewStringUTF(label.c_str());
}

// --- Language Hide ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLangHideAdd(
    JNIEnv* env, jclass,
    jstring jLangCode, jstring jRoomId, jstring jUserId,
    jboolean jSpecificUser, jint jMinutes
) {
    auto langCode = jLangCode ? std::string(env->GetStringUTFChars(jLangCode, nullptr)) : "";
    auto roomId   = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto userId   = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";

    if (jLangCode) env->ReleaseStringUTFChars(jLangCode, langCode.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jUserId)   env->ReleaseStringUTFChars(jUserId, userId.c_str());

    g_langHide.hideLanguage(langCode, roomId, userId, jSpecificUser, jMinutes);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLangHideIsHidden(
    JNIEnv* env, jclass,
    jstring jLangCode, jstring jRoomId, jstring jUserId
) {
    auto langCode = jLangCode ? std::string(env->GetStringUTFChars(jLangCode, nullptr)) : "";
    auto roomId   = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto userId   = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";

    if (jLangCode) env->ReleaseStringUTFChars(jLangCode, langCode.c_str());
    if (jRoomId)   env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    if (jUserId)   env->ReleaseStringUTFChars(jUserId, userId.c_str());

    return g_langHide.isHidden(langCode, roomId, userId) ? JNI_TRUE : JNI_FALSE;
}

// --- Chat Push Down ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeChatPushDown(
    JNIEnv* env, jclass, jstring jRoomId, jint jMinutes
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    g_chatPushDown.pushDown(roomId, jMinutes);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeChatIsPushedDown(
    JNIEnv* env, jclass, jstring jRoomId
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    return g_chatPushDown.isPushedDown(roomId) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeChatPushDownRestore(
    JNIEnv* env, jclass, jstring jRoomId
) {
    auto roomId = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, roomId.c_str());
    g_chatPushDown.restore(roomId);
}

// --- Emoji Blacklist ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEmojiBlacklistAdd(
    JNIEnv* env, jclass, jstring jEmoji
) {
    auto emoji = jEmoji ? std::string(env->GetStringUTFChars(jEmoji, nullptr)) : "";
    if (jEmoji) env->ReleaseStringUTFChars(jEmoji, emoji.c_str());
    g_emojiBlacklist.add(emoji);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEmojiBlacklistRemove(
    JNIEnv* env, jclass, jstring jEmoji
) {
    auto emoji = jEmoji ? std::string(env->GetStringUTFChars(jEmoji, nullptr)) : "";
    if (jEmoji) env->ReleaseStringUTFChars(jEmoji, emoji.c_str());
    g_emojiBlacklist.remove(emoji);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEmojiBlacklistIsBlocked(
    JNIEnv* env, jclass, jstring jEmoji
) {
    auto emoji = jEmoji ? std::string(env->GetStringUTFChars(jEmoji, nullptr)) : "";
    if (jEmoji) env->ReleaseStringUTFChars(jEmoji, emoji.c_str());
    return g_emojiBlacklist.isBlocked(emoji) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEmojiBlacklistExport(
    JNIEnv* env, jclass
) {
    auto json = g_emojiBlacklist.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEmojiBlacklistImport(
    JNIEnv* env, jclass, jstring jJson
) {
    if (!jJson) return;
    auto json = std::string(env->GetStringUTFChars(jJson, nullptr));
    env->ReleaseStringUTFChars(jJson, json.c_str());
    g_emojiBlacklist.importJson(json);
}

// --- Avatar History ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeAvatarAddChange(
    JNIEnv* env, jclass, jstring jMxcUrl, jstring jEventId, jlong jTimestamp
) {
    auto mxc = jMxcUrl ? std::string(env->GetStringUTFChars(jMxcUrl, nullptr)) : "";
    auto eid = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    if (jMxcUrl)  env->ReleaseStringUTFChars(jMxcUrl, mxc.c_str());
    if (jEventId) env->ReleaseStringUTFChars(jEventId, eid.c_str());
    g_avatarHistory.addChange(mxc, eid, jTimestamp);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeAvatarExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_avatarHistory.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeAvatarClear(
    JNIEnv*, jclass
) {
    g_avatarHistory.clear();
}

// --- Jump to Date with Time ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseJumpToDate(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());

    auto target = progressive::parseJumpToDate(input);

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << R"({"valid": )" << (target.valid ? "true" : "false");
    json << R"(,"timestampMs": )" << target.timestampMs;
    json << R"(,"hasTime": )" << (target.hasTime ? "true" : "false");
    if (!target.error.empty()) json << R"(,"error": ")" << esc(target.error) << R"(")";
    json << R"(,"year": )" << target.year;
    json << R"(,"month": )" << target.month;
    json << R"(,"day": )" << target.day;
    json << R"(,"hour": )" << target.hour;
    json << R"(,"minute": )" << target.minute;
    json << R"(,"formatted": ")" << progressive::formatJumpTarget(target) << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

// --- Room Matching ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeMatchRooms(
    JNIEnv* env, jclass, jstring jQuery, jstring jRoomsJson
) {
    auto query = jQuery ? std::string(env->GetStringUTFChars(jQuery, nullptr)) : "";
    auto rjson = jRoomsJson ? std::string(env->GetStringUTFChars(jRoomsJson, nullptr)) : "[]";
    if (jQuery)  env->ReleaseStringUTFChars(jQuery, query.c_str());
    if (jRoomsJson) env->ReleaseStringUTFChars(jRoomsJson, rjson.c_str());

    // Parse rooms JSON array
    std::vector<RoomMatch> rooms;
    if (rjson != "[]") {
        size_t pos = 0;
        while (true) {
            pos = rjson.find("\"roomId\"", pos);
            if (pos == std::string::npos) break;
            auto objStart = rjson.rfind('{', pos);
            if (objStart == std::string::npos) break;
            int depth = 0;
            auto objEnd = objStart;
            while (objEnd < rjson.size()) {
                if (rjson[objEnd] == '{') ++depth;
                else if (rjson[objEnd] == '}') --depth;
                if (depth == 0) break;
                ++objEnd;
            }
            if (objEnd >= rjson.size()) break;

            std::string obj = rjson.substr(objStart, objEnd - objStart + 1);
            RoomMatch rm;

            auto ridSearch = std::string("\"roomId\": \"");
            auto ridPos = obj.find(ridSearch);
            if (ridPos != std::string::npos) {
                ridPos += ridSearch.size();
                auto ridEnd = obj.find('"', ridPos);
                if (ridEnd != std::string::npos) rm.roomId = obj.substr(ridPos, ridEnd - ridPos);
            }

            auto rnSearch = std::string("\"roomName\": \"");
            auto rnPos = obj.find(rnSearch);
            if (rnPos != std::string::npos) {
                rnPos += rnSearch.size();
                auto rnEnd = obj.find('"', rnPos);
                if (rnEnd != std::string::npos) rm.roomName = obj.substr(rnPos, rnEnd - rnPos);
            }

            auto caSearch = std::string("\"canonicalAlias\": \"");
            auto caPos = obj.find(caSearch);
            if (caPos != std::string::npos) {
                caPos += caSearch.size();
                auto caEnd = obj.find('"', caPos);
                if (caEnd != std::string::npos) rm.canonicalAlias = obj.substr(caPos, caEnd - caPos);
            }

            if (!rm.roomId.empty()) rooms.push_back(rm);
            pos = objEnd + 1;
        }
    }

    auto matches = progressive::matchRooms(query, rooms);

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < matches.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"roomId": ")" << esc(matches[i].roomId) << R"(")";
        json << R"(,"roomName": ")" << esc(matches[i].roomName) << R"(")";
        json << R"(,"alias": ")" << esc(matches[i].canonicalAlias) << R"(")";
        json << R"(,"score": )" << matches[i].score << "}";
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsRoomId(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::isRoomId(input) ? JNI_TRUE : JNI_FALSE;
}


// --- Event Links ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractEventLinks(
    JNIEnv* env, jclass, jstring jBody
) {
    auto body = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jBody) env->ReleaseStringUTFChars(jBody, body.c_str());

    auto links = progressive::extractEventLinks(body);

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < links.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"eventId": ")" << esc(links[i].eventId) << R"(")";
        json << R"(,"startPos": )" << links[i].startPos;
        json << R"(,"endPos": )" << links[i].endPos;
        json << R"(,"originalText": ")" << esc(links[i].originalText) << R"(")";
        json << "}";
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatResolvedEvent(
    JNIEnv* env, jclass, jstring jSender, jstring jBody
) {
    auto sender = jSender ? std::string(env->GetStringUTFChars(jSender, nullptr)) : "";
    auto body   = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jSender) env->ReleaseStringUTFChars(jSender, sender.c_str());
    if (jBody)   env->ReleaseStringUTFChars(jBody, body.c_str());

    auto s = progressive::formatResolvedEventText(sender, body);
    return env->NewStringUTF(s.c_str());
}


// --- Timestamps with Seconds ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatTimestamp(
    JNIEnv* env, jclass, jlong jEpochMs, jboolean jIncludeSeconds
) {
    auto s = progressive::formatTimestamp(jEpochMs, jIncludeSeconds);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatFullTimestamp(
    JNIEnv* env, jclass, jlong jEpochMs
) {
    auto s = progressive::formatFullTimestamp(jEpochMs);
    return env->NewStringUTF(s.c_str());
}

// --- Lightweight Call ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLightCallEnter(
    JNIEnv* env, jclass
) {
    auto plan = g_lightCall.enterCallMode();
    auto json = progressive::LightweightCallManager::planToJson(plan);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLightCallExit(
    JNIEnv* env, jclass
) {
    auto plan = g_lightCall.exitCallMode();
    auto json = progressive::LightweightCallManager::planToJson(plan);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLightCallAssessMemory(
    JNIEnv* env, jclass
) {
    auto state = g_lightCall.assessMemory();
    auto json = progressive::LightweightCallManager::memoryStateToJson(state);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldUseLightweightMode(
    JNIEnv* env, jclass
) {
    auto state = progressive::LightweightCallManager{}.assessMemory();
    return progressive::LightweightCallManager::shouldUseLightweightMode(state) ? JNI_TRUE : JNI_FALSE;
}

// --- Scheduled Edits ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditSchedule(
    JNIEnv* env, jclass,
    jstring jRoomId, jstring jTargetEventId, jstring jNewContent,
    jstring jContentUrl, jstring jFormattedContent, jstring jFormattedUrl,
    jlong jScheduledAtMs, jboolean jRecurring
) {
    ScheduledEdit edit;
    edit.roomId        = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    edit.targetEventId = jTargetEventId ? std::string(env->GetStringUTFChars(jTargetEventId, nullptr)) : "";
    edit.newContent    = jNewContent ? std::string(env->GetStringUTFChars(jNewContent, nullptr)) : "";
    edit.contentUrl    = jContentUrl ? std::string(env->GetStringUTFChars(jContentUrl, nullptr)) : "";
    edit.formattedContent = jFormattedContent ? std::string(env->GetStringUTFChars(jFormattedContent, nullptr)) : "";
    edit.formattedUrl  = jFormattedUrl ? std::string(env->GetStringUTFChars(jFormattedUrl, nullptr)) : "";
    edit.scheduledAtMs = jScheduledAtMs;
    edit.isRecurring   = jRecurring;

    if (jRoomId)        env->ReleaseStringUTFChars(jRoomId, edit.roomId.c_str());
    if (jTargetEventId) env->ReleaseStringUTFChars(jTargetEventId, edit.targetEventId.c_str());
    if (jNewContent)    env->ReleaseStringUTFChars(jNewContent, edit.newContent.c_str());
    if (jContentUrl)    env->ReleaseStringUTFChars(jContentUrl, edit.contentUrl.c_str());
    if (jFormattedContent) env->ReleaseStringUTFChars(jFormattedContent, edit.formattedContent.c_str());
    if (jFormattedUrl)  env->ReleaseStringUTFChars(jFormattedUrl, edit.formattedUrl.c_str());

    auto id = g_schedEdits.schedule(edit);
    return env->NewStringUTF(id.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditCancel(
    JNIEnv* env, jclass, jstring jEditId
) {
    auto id = jEditId ? std::string(env->GetStringUTFChars(jEditId, nullptr)) : "";
    if (jEditId) env->ReleaseStringUTFChars(jEditId, id.c_str());
    g_schedEdits.cancel(id);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditGetDue(
    JNIEnv* env, jclass
) {
    auto due = g_schedEdits.getDueEdits();
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < due.size(); ++i) {
        if (i > 0) json << ",";
        json << progressive::ScheduledEditQueue::editToJson(due[i]);
    }
    json << "]";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditMarkApplied(
    JNIEnv* env, jclass, jstring jEditId
) {
    auto id = jEditId ? std::string(env->GetStringUTFChars(jEditId, nullptr)) : "";
    if (jEditId) env->ReleaseStringUTFChars(jEditId, id.c_str());
    g_schedEdits.markApplied(id);
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditMarkFailed(
    JNIEnv* env, jclass, jstring jEditId, jstring jError
) {
    auto id = jEditId ? std::string(env->GetStringUTFChars(jEditId, nullptr)) : "";
    auto err = jError ? std::string(env->GetStringUTFChars(jError, nullptr)) : "";
    if (jEditId) env->ReleaseStringUTFChars(jEditId, id.c_str());
    if (jError) env->ReleaseStringUTFChars(jError, err.c_str());
    g_schedEdits.markFailed(id, err);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditExport(
    JNIEnv* env, jclass
) {
    auto json = g_schedEdits.exportJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSchedEditStats(
    JNIEnv* env, jclass
) {
    auto stats = g_schedEdits.getStats();
    std::ostringstream json;
    json << "{";
    json << R"("totalEdits": )" << stats.totalEdits << ",";
    json << R"("pendingEdits": )" << stats.pendingEdits << ",";
    json << R"("appliedEdits": )" << stats.appliedEdits << ",";
    json << R"("failedEdits": )" << stats.failedEdits << ",";
    json << R"("nextEditAtMs": )" << stats.nextEditAtMs;
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

// --- SVG Renderer ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseSvg(
    JNIEnv* env, jclass, jstring jSvgData
) {
    auto svg = jSvgData ? std::string(env->GetStringUTFChars(jSvgData, nullptr)) : "";
    if (jSvgData) env->ReleaseStringUTFChars(jSvgData, svg.c_str());
    auto doc = progressive::parseSvg(svg);
    auto json = progressive::renderSvgToDrawCommands(doc);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidSvg(
    JNIEnv* env, jclass, jstring jData
) {
    auto data = jData ? std::string(env->GetStringUTFChars(jData, nullptr)) : "";
    if (jData) env->ReleaseStringUTFChars(jData, data.c_str());
    return progressive::isValidSvg(data) ? JNI_TRUE : JNI_FALSE;
}

// --- Drawing Canvas ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawMoveTo(
    JNIEnv*, jclass, jdouble jX, jdouble jY
) { g_drawCanvas.moveTo(jX, jY); }

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawLineTo(
    JNIEnv*, jclass, jdouble jX, jdouble jY
) { g_drawCanvas.lineTo(jX, jY); }

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawSetColor(
    JNIEnv*, jclass, jint jArgb
) { g_drawCanvas.setColor(jArgb); }

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawSetWidth(
    JNIEnv*, jclass, jdouble jW
) { g_drawCanvas.setWidth(jW); }

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawExportJson(
    JNIEnv* env, jclass
) {
    auto json = g_drawCanvas.exportCommandsJson();
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawToSvgPath(
    JNIEnv* env, jclass
) {
    auto d = g_drawCanvas.toSvgPath();
    return env->NewStringUTF(d.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDrawClear(
    JNIEnv*, jclass
) { g_drawCanvas.clear(); }

// --- Profile Swiper ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeProfileSwiperSetProfiles(
    JNIEnv* env, jclass, jstring jProfilesJson
) {
    auto json = jProfilesJson ? std::string(env->GetStringUTFChars(jProfilesJson, nullptr)) : "[]";
    if (jProfilesJson) env->ReleaseStringUTFChars(jProfilesJson, json.c_str());

    std::vector<ProfileEntry> profiles;
    size_t pos = 0;
    while (true) {
        pos = json.find("\"userId\"", pos);
        if (pos == std::string::npos) break;
        auto objStart = json.rfind('{', pos);
        if (objStart == std::string::npos) break;
        int depth = 0; auto objEnd = objStart;
        while (objEnd < json.size()) { if (json[objEnd] == '{') ++depth; else if (json[objEnd] == '}') --depth; if (depth == 0) break; ++objEnd; }
        if (objEnd >= json.size()) break;

        std::string obj = json.substr(objStart, objEnd - objStart + 1);
        ProfileEntry pe;
        auto extract = [&](const std::string& key) -> std::string {
            auto s = obj.find('"' + key + "\": \""); if (s == std::string::npos) return {};
            s += key.size() + 4; auto e = obj.find('"', s); if (e != std::string::npos) return obj.substr(s, e - s);
            return {};
        };
        pe.userId = extract("userId");
        pe.displayName = extract("displayName");
        pe.avatarUrl = extract("avatarUrl");
        pe.serverName = extract("serverName");
        auto mc = obj.find("\"messageCount\": "); if (mc != std::string::npos) { mc += 16; auto me = mc; while (me < obj.size() && obj[me] >= '0' && obj[me] <= '9') ++me; pe.messageCount = me > mc ? std::stoi(obj.substr(mc, me - mc)) : 0; }
        if (!pe.userId.empty()) profiles.push_back(pe);
        pos = objEnd + 1;
    }
    g_profileSwiper.setProfiles(profiles);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeProfileSwiperNext(
    JNIEnv* env, jclass
) {
    auto state = g_profileSwiper.swipeNext();
    auto json = progressive::ProfileSwiper::stateToJson(state);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeProfileSwiperPrev(
    JNIEnv* env, jclass
) {
    auto state = g_profileSwiper.swipePrev();
    auto json = progressive::ProfileSwiper::stateToJson(state);
    return env->NewStringUTF(json.c_str());
}

// --- Rainbow Generator ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGenerateRainbow(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto rainbow = progressive::generateRainbow(text);
    return env->NewStringUTF(rainbow.c_str());
}

// --- Text Formats ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatSpoiler(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatSpoiler(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatEmote(
    JNIEnv* env, jclass, jstring jSender, jstring jText
) {
    auto sender = jSender ? std::string(env->GetStringUTFChars(jSender, nullptr)) : "";
    auto text   = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jSender) env->ReleaseStringUTFChars(jSender, sender.c_str());
    if (jText)   env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatEmote(sender, text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatShrug(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatShrug(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatLenny(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatLenny(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatTableFlip(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatTableFlip(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatPlain(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::formatPlain(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsEmojiOnly(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    return progressive::isEmojiOnly(text) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeTruncateText(
    JNIEnv* env, jclass, jstring jText, jint jMaxLen
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::truncateText(text, jMaxLen);
    return env->NewStringUTF(s.c_str());
}

// --- URL Tools ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseUrl(
    JNIEnv* env, jclass, jstring jUrl
) {
    auto url = jUrl ? std::string(env->GetStringUTFChars(jUrl, nullptr)) : "";
    if (jUrl) env->ReleaseStringUTFChars(jUrl, url.c_str());

    auto p = progressive::parseUrl(url);
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (p.valid ? "true" : "false");
    json << R"(,"protocol": ")" << esc(p.protocol) << R"(")";
    json << R"(,"host": ")" << esc(p.host) << R"(")";
    json << R"(,"port": ")" << esc(p.port) << R"(")";
    json << R"(,"path": ")" << esc(p.path) << R"(")";
    json << R"(,"query": ")" << esc(p.query) << R"(")";
    json << R"(,"fragment": ")" << esc(p.fragment) << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeExtractFirstUrl(
    JNIEnv* env, jclass, jstring jText
) {
    auto text = jText ? std::string(env->GetStringUTFChars(jText, nullptr)) : "";
    if (jText) env->ReleaseStringUTFChars(jText, text.c_str());
    auto s = progressive::extractFirstUrl(text);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUrlEncode(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    auto s = progressive::urlEncode(input);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeUrlDecode(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    auto s = progressive::urlDecode(input);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildMatrixToUrl(
    JNIEnv* env, jclass, jstring jRoomId
) {
    auto id = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, id.c_str());
    auto s = progressive::buildMatrixToUrl(id);
    return env->NewStringUTF(s.c_str());
}

// --- Notification Priority ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeNotifPriority(
    JNIEnv* env, jclass,
    jboolean jIsDM, jboolean jIsMention, jboolean jIsRoomMention,
    jboolean jIsKeyword, jboolean jIsCall, jboolean jIsBackground,
    jboolean jDnd, jboolean jFavorite
) {
    NotifContent content;
    content.isDirectMessage = jIsDM;
    content.isMention = jIsMention;
    content.isRoomMention = jIsRoomMention;
    content.isKeywordMatch = jIsKeyword;
    content.isCallInvite = jIsCall;

    auto prio = progressive::computeNotifPriority(content, jIsBackground, jDnd, jFavorite);

    std::ostringstream json;
    json << "{";
    json << R"("level": )" << static_cast<int>(prio.level) << ",";
    json << R"("vibrate": )" << (prio.shouldVibrate ? "true" : "false") << ",";
    json << R"("wakeScreen": )" << (prio.shouldWakeScreen ? "true" : "false") << ",";
    json << R"("ledColor": )" << prio.ledColor << ",";
    json << R"("soundUri": ")" << prio.soundUri << R"(",)";
    json << R"("category": ")" << prio.category << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatNotifTitle(
    JNIEnv* env, jclass,
    jstring jRoomName, jstring jSenderName, jboolean jIsDM
) {
    NotifContent c;
    c.roomName   = jRoomName ? std::string(env->GetStringUTFChars(jRoomName, nullptr)) : "";
    c.senderName = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    c.isDirectMessage = jIsDM;
    if (jRoomName)   env->ReleaseStringUTFChars(jRoomName, c.roomName.c_str());
    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, c.senderName.c_str());

    auto s = progressive::formatNotifTitle(c);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatNotifBody(
    JNIEnv* env, jclass,
    jstring jBody, jstring jSenderName, jboolean jIsDM, jboolean jShowSender
) {
    NotifContent c;
    c.body       = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    c.senderName = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    c.isDirectMessage = jIsDM;
    if (jBody)       env->ReleaseStringUTFChars(jBody, c.body.c_str());
    if (jSenderName) env->ReleaseStringUTFChars(jSenderName, c.senderName.c_str());

    auto s = progressive::formatNotifBody(c, jShowSender);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsRoomMention(
    JNIEnv* env, jclass, jstring jBody
) {
    auto body = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    if (jBody) env->ReleaseStringUTFChars(jBody, body.c_str());
    return progressive::isRoomMention(body) ? JNI_TRUE : JNI_FALSE;
}

// --- Matrix Patterns ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsUserId(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::isUserId(input) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsRoomAlias(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::isRoomAlias(input) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsEventId(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::isEventId(input) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseMatrixToPermalink(
    JNIEnv* env, jclass, jstring jUrl
) {
    auto url = jUrl ? std::string(env->GetStringUTFChars(jUrl, nullptr)) : "";
    if (jUrl) env->ReleaseStringUTFChars(jUrl, url.c_str());

    auto info = progressive::parseMatrixToPermalink(url);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (info.valid ? "true" : "false");
    json << R"(,"type": ")" << esc(info.type) << R"(")";
    json << R"(,"roomId": ")" << esc(info.roomId) << R"(")";
    json << R"(,"userId": ")" << esc(info.userId) << R"(")";
    json << R"(,"eventId": ")" << esc(info.eventId) << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidEmail(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::isValidEmail(input) ? JNI_TRUE : JNI_FALSE;
}

// --- Desync Detector ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDesyncTrackEvent(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jServerName, jlong jTimestamp
) {
    auto eid = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    auto srv = jServerName ? std::string(env->GetStringUTFChars(jServerName, nullptr)) : "";
    if (jEventId)    env->ReleaseStringUTFChars(jEventId, eid.c_str());
    if (jServerName) env->ReleaseStringUTFChars(jServerName, srv.c_str());
    g_desyncDetector.trackEvent(eid, srv, jTimestamp);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDesyncCheck(
    JNIEnv* env, jclass, jstring jRoomId, jstring jCurrentServer
) {
    auto rid = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    auto cs  = jCurrentServer ? std::string(env->GetStringUTFChars(jCurrentServer, nullptr)) : "";
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, rid.c_str());
    if (jCurrentServer) env->ReleaseStringUTFChars(jCurrentServer, cs.c_str());

    auto report = g_desyncDetector.checkDesync(rid, cs);
    auto json = progressive::DesyncDetector::reportToJson(report);
    return env->NewStringUTF(json.c_str());
}

// --- Latency Tracker ---

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLatencyRecord(
    JNIEnv* env, jclass,
    jdouble jLatencyMs, jstring jServer, jstring jEndpoint, jboolean jSuccess
) {
    auto server = jServer ? std::string(env->GetStringUTFChars(jServer, nullptr)) : "";
    auto endpoint = jEndpoint ? std::string(env->GetStringUTFChars(jEndpoint, nullptr)) : "";
    if (jServer) env->ReleaseStringUTFChars(jServer, server.c_str());
    if (jEndpoint) env->ReleaseStringUTFChars(jEndpoint, endpoint.c_str());
    g_latencyTracker.record(jLatencyMs, server, endpoint, jSuccess);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLatencyStats(
    JNIEnv* env, jclass
) {
    auto stats = g_latencyTracker.computeStats();
    auto json = progressive::LatencyTracker::statsToJson(stats);
    return env->NewStringUTF(json.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLatencyStatsText(
    JNIEnv* env, jclass
) {
    auto stats = g_latencyTracker.computeStats();
    auto text = progressive::LatencyTracker::statsToText(stats);
    return env->NewStringUTF(text.c_str());
}

// --- String Utils ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSanitizeRoomName(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    auto s = progressive::sanitizeRoomName(input);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeWordCount(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    return progressive::wordCount(input);
}

// --- Location Sharing ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationStartSession(
    JNIEnv* env, jclass,
    jstring jRoomId, jstring jUserId, jint jIntervalSec, jboolean jAutoStop, jint jAutoStopMin
) {
    LocationSession session;
    session.roomId          = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    session.userId          = jUserId ? std::string(env->GetStringUTFChars(jUserId, nullptr)) : "";
    session.intervalSeconds = jIntervalSec;
    session.autoStop        = jAutoStop;
    session.autoStopMinutes = jAutoStopMin;
    if (jRoomId) env->ReleaseStringUTFChars(jRoomId, session.roomId.c_str());
    if (jUserId) env->ReleaseStringUTFChars(jUserId, session.userId.c_str());

    auto id = g_locationSharing.startSession(session);
    return env->NewStringUTF(id.c_str());
}

JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationStopSession(
    JNIEnv* env, jclass, jstring jSessionId
) {
    auto id = jSessionId ? std::string(env->GetStringUTFChars(jSessionId, nullptr)) : "";
    if (jSessionId) env->ReleaseStringUTFChars(jSessionId, id.c_str());
    g_locationSharing.stopSession(id);
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationIsDue(
    JNIEnv* env, jclass, jstring jSessionId
) {
    auto id = jSessionId ? std::string(env->GetStringUTFChars(jSessionId, nullptr)) : "";
    if (jSessionId) env->ReleaseStringUTFChars(jSessionId, id.c_str());
    return g_locationSharing.isDue(id) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationFormatMessage(
    JNIEnv* env, jclass,
    jdouble jLat, jdouble jLon, jdouble jAcc, jstring jLabel
) {
    GeoCoord coord;
    coord.latitude  = jLat;
    coord.longitude = jLon;
    coord.accuracy  = jAcc;
    auto label = jLabel ? std::string(env->GetStringUTFChars(jLabel, nullptr)) : "";
    if (jLabel) env->ReleaseStringUTFChars(jLabel, label.c_str());
    auto s = LocationSharingManager::formatLocationMessage(coord, label);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationFormatGeoJson(
    JNIEnv* env, jclass,
    jdouble jLat, jdouble jLon, jdouble jAcc
) {
    GeoCoord coord;
    coord.latitude  = jLat;
    coord.longitude = jLon;
    coord.accuracy  = jAcc;
    auto s = LocationSharingManager::formatGeoJson(coord);
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jdouble JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeLocationDistance(
    JNIEnv*, jclass,
    jdouble jLat1, jdouble jLon1, jdouble jLat2, jdouble jLon2
) {
    GeoCoord a{jLat1, jLon1}, b{jLat2, jLon2};
    return LocationSharingManager::distanceMeters(a, b);
}

// --- Color Utils ---

JNIEXPORT jdouble JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeContrastRatio(
    JNIEnv*, jclass, jint jFgR, jint jFgG, jint jFgB, jint jBgR, jint jBgG, jint jBgB
) {
    RgbaColor fg{jFgR, jFgG, jFgB, 255, true};
    RgbaColor bg{jBgR, jBgG, jBgB, 255, true};
    return progressive::contrastRatio(fg, bg);
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeParseColor(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    auto color = progressive::parseColor(input);
    std::ostringstream json;
    json << R"({"valid": )" << (color.valid ? "true" : "false");
    json << R"(,"r": )" << color.r << R"(,"g": )" << color.g << R"(,"b": )" << color.b;
    json << R"(,"hex": ")" << color.toHex() << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

// --- E2EE Utils ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetTrustLabel(
    JNIEnv* env, jclass, jint jLevel
) {
    auto s = progressive::getTrustLabel(static_cast<TrustLevel>(jLevel));
    return env->NewStringUTF(s.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGetTrustBadge(
    JNIEnv* env, jclass, jint jLevel
) {
    auto s = progressive::getTrustBadge(static_cast<TrustLevel>(jLevel));
    return env->NewStringUTF(s.c_str());
}

// --- Thumbnail ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeThumbnail(
    JNIEnv*, jclass, jint jSrcW, jint jSrcH, jint jMaxW, jint jMaxH, jboolean jUpscale, jint jQuality
) {
    ThumbnailParams p{jSrcW, jSrcH, jMaxW, jMaxH, jUpscale, jQuality};
    auto result = progressive::computeThumbnail(p);
    std::ostringstream json;
    json << R"({"targetW": )" << result.targetW;
    json << R"(,"targetH": )" << result.targetH;
    json << R"(,"scale": )" << result.scale;
    json << R"(,"estimatedBytes": )" << result.estimatedBytes << "}";
    return env->NewStringUTF(json.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeBuildThumbnailUrl(
    JNIEnv* env, jclass, jstring jMxc, jint jW, jint jH, jstring jMethod, jboolean jAnim
) {
    auto mxc = jMxc ? std::string(env->GetStringUTFChars(jMxc, nullptr)) : "";
    auto method = jMethod ? std::string(env->GetStringUTFChars(jMethod, nullptr)) : "scale";
    if (jMxc) env->ReleaseStringUTFChars(jMxc, mxc.c_str());
    if (jMethod) env->ReleaseStringUTFChars(jMethod, method.c_str());
    auto s = progressive::buildThumbnailUrl(mxc, jW, jH, method, jAnim);
    return env->NewStringUTF(s.c_str());
}

// --- Waveform ---

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSuggestBarCount(
    JNIEnv*, jclass, jlong jDurationMs
) {
    return progressive::suggestBarCount(jDurationMs);
}

JNIEXPORT jdouble JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeComputeRmsVolume(
    JNIEnv*, jclass, jintArray jSamples
) {
    // This would need to read jintArray samples — placeholder
    return 0.0;
}

// --- Session Timeout ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeShouldLock(
    JNIEnv*, jclass,
    jint jLockMethod, jint jIdleTimeoutMin, jint jMaxSessionMin,
    jint jMaxFailedPin, jboolean jLockOnBg,
    jlong jLastActivityMs, jlong jSessionStartMs,
    jint jFailedAttempts, jboolean jIsLocked, jboolean jIsBackground
) {
    SessionPolicy policy;
    policy.lockMethod = static_cast<LockMethod>(jLockMethod);
    policy.idleTimeoutMinutes = jIdleTimeoutMin;
    policy.maxSessionMinutes = jMaxSessionMin;
    policy.maxFailedPinAttempts = jMaxFailedPin;
    policy.lockOnBackground = jLockOnBg;

    SessionState state;
    state.lastActivityMs = jLastActivityMs;
    state.sessionStartMs = jSessionStartMs;
    state.failedAttempts = jFailedAttempts;
    state.isLocked = jIsLocked;
    state.isBackground = jIsBackground;

    return progressive::shouldLock(policy, state) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsValidPin(
    JNIEnv* env, jclass, jstring jPin, jint jMinLen, jint jMaxLen
) {
    auto pin = jPin ? std::string(env->GetStringUTFChars(jPin, nullptr)) : "";
    if (jPin) env->ReleaseStringUTFChars(jPin, pin.c_str());
    return progressive::isValidPin(pin, jMinLen, jMaxLen) ? JNI_TRUE : JNI_FALSE;
}

// --- Password Validator ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeValidatePassword(
    JNIEnv* env, jclass, jstring jPassword
) {
    auto pass = jPassword ? std::string(env->GetStringUTFChars(jPassword, nullptr)) : "";
    if (jPassword) env->ReleaseStringUTFChars(jPassword, pass.c_str());

    auto result = progressive::validatePassword(pass);
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (result.valid ? "true" : "false");
    json << R"(,"strength": )" << result.strength;
    json << R"(,"label": ")" << result.strengthLabel << R"(")";
    json << R"(,"feedback": ")" << esc(result.feedback) << R"(")";
    json << R"(,"crackTime": ")" << progressive::formatCrackTime(progressive::estimateCrackTimeSeconds(pass)) << R"(")";
    json << "}";
    return env->NewStringUTF(json.str().c_str());
}

// --- Spellcheck ---

JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeEditDistance(
    JNIEnv* env, jclass, jstring jA, jstring jB
) {
    auto a = jA ? std::string(env->GetStringUTFChars(jA, nullptr)) : "";
    auto b = jB ? std::string(env->GetStringUTFChars(jB, nullptr)) : "";
    if (jA) env->ReleaseStringUTFChars(jA, a.c_str());
    if (jB) env->ReleaseStringUTFChars(jB, b.c_str());
    return progressive::SpellChecker::editDistance(a, b);
}

// --- Typing Indicator ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeFormatTypingText(
    JNIEnv* env, jclass, jstring jNamesJson
) {
    auto json = jNamesJson ? std::string(env->GetStringUTFChars(jNamesJson, nullptr)) : "[]";
    if (jNamesJson) env->ReleaseStringUTFChars(jNamesJson, json.c_str());

    std::vector<std::string> names;
    size_t pos = 0;
    while (true) {
        pos = json.find('"', pos);
        if (pos == std::string::npos) break;
        ++pos;
        auto end = json.find('"', pos);
        if (end == std::string::npos) break;
        names.push_back(json.substr(pos, end - pos));
        pos = end + 1;
    }

    auto s = progressive::formatTypingText(names);
    return env->NewStringUTF(s.c_str());
}

// --- Link Preview ---

JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeIsImageUrl(
    JNIEnv* env, jclass, jstring jUrl
) {
    auto url = jUrl ? std::string(env->GetStringUTFChars(jUrl, nullptr)) : "";
    if (jUrl) env->ReleaseStringUTFChars(jUrl, url.c_str());
    return progressive::isImageUrl(url) ? JNI_TRUE : JNI_FALSE;
}

// --- Hash Utils ---

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeSha256Hex(
    JNIEnv* env, jclass, jstring jInput
) {
    auto input = jInput ? std::string(env->GetStringUTFChars(jInput, nullptr)) : "";
    if (jInput) env->ReleaseStringUTFChars(jInput, input.c_str());
    auto hash = progressive::sha256Hex(input);
    return env->NewStringUTF(hash.c_str());
}

JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeGenerateToken(
    JNIEnv* env, jclass, jint jBytes
) {
    auto token = progressive::generateToken(jBytes > 0 ? jBytes : 32);
    return env->NewStringUTF(token.c_str());
}

} // extern "C"
