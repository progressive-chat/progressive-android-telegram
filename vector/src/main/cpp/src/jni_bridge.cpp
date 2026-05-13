#include <jni.h>
#include <string>
#include <android/log.h>
#include "progressive/jumptodate.hpp"
#include "progressive/relation.hpp"
#include "progressive/exporter.hpp"
#include "progressive/eventcache.hpp"
#include "progressive/eventdb.hpp"
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
</style>\n";
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
 * Method: nativeCacheClear
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeCacheClear(
    JNIEnv* /* env */,
    jclass /* this */
) {
    g_eventCache.clear();
    LOGD("EventCache cleared");
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

// --- Event Database (SQLite-based replacement for slow Realm queries) ---
static progressive::EventDatabase g_eventDb;

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbOpen
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbOpen(
    JNIEnv* env,
    jclass /* this */,
    jstring jDbPath
) {
    if (!jDbPath) return JNI_FALSE;
    auto path = std::string(env->GetStringUTFChars(jDbPath, nullptr));
    env->ReleaseStringUTFChars(jDbPath, path.c_str());
    return g_eventDb.open(path) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbClose(JNIEnv*, jclass) {
    g_eventDb.close();
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbInsertEvent
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JIZ)V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbInsertEvent(
    JNIEnv* env, jclass,
    jstring jEventId, jstring jRoomId, jstring jSenderId, jstring jSenderName,
    jstring jTimestamp, jstring jBody, jstring jMsgType, jstring jEventType,
    jstring jRelationType, jstring jSourceEventId,
    jlong jOriginServerTs, jint jDisplayIndex, jboolean jSentByMe
) {
    DbEvent e;
    e.eventId       = jEventId ? std::string(env->GetStringUTFChars(jEventId, nullptr)) : "";
    e.roomId        = jRoomId ? std::string(env->GetStringUTFChars(jRoomId, nullptr)) : "";
    e.senderId      = jSenderId ? std::string(env->GetStringUTFChars(jSenderId, nullptr)) : "";
    e.senderName    = jSenderName ? std::string(env->GetStringUTFChars(jSenderName, nullptr)) : "";
    e.timestamp     = jTimestamp ? std::string(env->GetStringUTFChars(jTimestamp, nullptr)) : "";
    e.body          = jBody ? std::string(env->GetStringUTFChars(jBody, nullptr)) : "";
    e.msgType       = jMsgType ? std::string(env->GetStringUTFChars(jMsgType, nullptr)) : "";
    e.eventType     = jEventType ? std::string(env->GetStringUTFChars(jEventType, nullptr)) : "";
    e.relationType  = jRelationType ? std::string(env->GetStringUTFChars(jRelationType, nullptr)) : "";
    e.sourceEventId = jSourceEventId ? std::string(env->GetStringUTFChars(jSourceEventId, nullptr)) : "";
    e.originServerTs = jOriginServerTs;
    e.displayIndex  = jDisplayIndex;
    e.sentByMe      = jSentByMe;

    if (jEventId)     env->ReleaseStringUTFChars(jEventId, e.eventId.c_str());
    if (jRoomId)      env->ReleaseStringUTFChars(jRoomId, e.roomId.c_str());
    if (jSenderId)    env->ReleaseStringUTFChars(jSenderId, e.senderId.c_str());
    if (jSenderName)  env->ReleaseStringUTFChars(jSenderName, e.senderName.c_str());
    if (jTimestamp)   env->ReleaseStringUTFChars(jTimestamp, e.timestamp.c_str());
    if (jBody)        env->ReleaseStringUTFChars(jBody, e.body.c_str());
    if (jMsgType)     env->ReleaseStringUTFChars(jMsgType, e.msgType.c_str());
    if (jEventType)   env->ReleaseStringUTFChars(jEventType, e.eventType.c_str());
    if (jRelationType) env->ReleaseStringUTFChars(jRelationType, e.relationType.c_str());
    if (jSourceEventId) env->ReleaseStringUTFChars(jSourceEventId, e.sourceEventId.c_str());

    g_eventDb.insertEvent(e);
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbGetContext
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbGetContext(
    JNIEnv* env, jclass, jstring jEventId
) {
    if (!jEventId) return env->NewStringUTF(R"({"cached": false})");
    auto id = std::string(env->GetStringUTFChars(jEventId, nullptr));
    env->ReleaseStringUTFChars(jEventId, id.c_str());

    auto json = g_eventDb.getContextJson(id);
    return env->NewStringUTF(json.c_str());
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbClearRoom
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbClearRoom(
    JNIEnv* env, jclass, jstring jRoomId
) {
    if (!jRoomId) return;
    auto id = std::string(env->GetStringUTFChars(jRoomId, nullptr));
    env->ReleaseStringUTFChars(jRoomId, id.c_str());
    g_eventDb.clearRoom(id);
}

/*
 * Class: im.vector.app.features.jumptodate.ProgressiveNative
 * Method: nativeDbCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_im_vector_app_features_jumptodate_ProgressiveNative_nativeDbCount(JNIEnv*, jclass) {
    return g_eventDb.count();
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

} // extern "C"
