#include <jni.h>
#include <string>
#include <android/log.h>
#include "progressive/jumptodate.hpp"

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

} // extern "C"
