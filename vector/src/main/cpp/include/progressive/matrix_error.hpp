#ifndef PROGRESSIVE_MATRIX_ERROR_HPP
#define PROGRESSIVE_MATRIX_ERROR_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace progressive {

// ---- Matrix Error Codes ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.failure.MatrixError.kt (218 lines)
//
// This is the standard Matrix error response format from the spec:
//   https://matrix.org/docs/spec/client_server/latest#api-standards
//
// Every Matrix API error response has:
//   {"errcode": "M_FORBIDDEN", "error": "You are not allowed to..."}
//
// Some errors include additional fields for specific situations.

// --- Error Code Constants (from MatrixError.kt companion object) ---
namespace ErrorCode {
    constexpr const char* M_FORBIDDEN = "M_FORBIDDEN";
    constexpr const char* M_UNKNOWN = "M_UNKNOWN";
    constexpr const char* M_UNKNOWN_TOKEN = "M_UNKNOWN_TOKEN";
    constexpr const char* M_MISSING_TOKEN = "M_MISSING_TOKEN";
    constexpr const char* M_BAD_JSON = "M_BAD_JSON";
    constexpr const char* M_NOT_JSON = "M_NOT_JSON";
    constexpr const char* M_NOT_FOUND = "M_NOT_FOUND";
    constexpr const char* M_LIMIT_EXCEEDED = "M_LIMIT_EXCEEDED";
    constexpr const char* M_USER_IN_USE = "M_USER_IN_USE";
    constexpr const char* M_ROOM_IN_USE = "M_ROOM_IN_USE";
    constexpr const char* M_BAD_PAGINATION = "M_BAD_PAGINATION";
    constexpr const char* M_UNAUTHORIZED = "M_UNAUTHORIZED";
    constexpr const char* M_OLD_VERSION = "M_OLD_VERSION";
    constexpr const char* M_UNRECOGNIZED = "M_UNRECOGNIZED";
    constexpr const char* M_LOGIN_EMAIL_URL_NOT_YET = "M_LOGIN_EMAIL_URL_NOT_YET";
    constexpr const char* M_THREEPID_AUTH_FAILED = "M_THREEPID_AUTH_FAILED";
    constexpr const char* M_THREEPID_NOT_FOUND = "M_THREEPID_NOT_FOUND";
    constexpr const char* M_THREEPID_IN_USE = "M_THREEPID_IN_USE";
    constexpr const char* M_SERVER_NOT_TRUSTED = "M_SERVER_NOT_TRUSTED";
    constexpr const char* M_TOO_LARGE = "M_TOO_LARGE";
    constexpr const char* M_CONSENT_NOT_GIVEN = "M_CONSENT_NOT_GIVEN";
    constexpr const char* M_RESOURCE_LIMIT_EXCEEDED = "M_RESOURCE_LIMIT_EXCEEDED";
    constexpr const char* M_USER_DEACTIVATED = "M_USER_DEACTIVATED";
    constexpr const char* M_INVALID_USERNAME = "M_INVALID_USERNAME";
    constexpr const char* M_INVALID_ROOM_STATE = "M_INVALID_ROOM_STATE";
    constexpr const char* M_THREEPID_DENIED = "M_THREEPID_DENIED";
    constexpr const char* M_UNSUPPORTED_ROOM_VERSION = "M_UNSUPPORTED_ROOM_VERSION";
    constexpr const char* M_INCOMPATIBLE_ROOM_VERSION = "M_INCOMPATIBLE_ROOM_VERSION";
    constexpr const char* M_BAD_STATE = "M_BAD_STATE";
    constexpr const char* M_GUEST_ACCESS_FORBIDDEN = "M_GUEST_ACCESS_FORBIDDEN";
    constexpr const char* M_CAPTCHA_NEEDED = "M_CAPTCHA_NEEDED";
    constexpr const char* M_CAPTCHA_INVALID = "M_CAPTCHA_INVALID";
    constexpr const char* M_MISSING_PARAM = "M_MISSING_PARAM";
    constexpr const char* M_INVALID_PARAM = "M_INVALID_PARAM";
    constexpr const char* M_EXCLUSIVE = "M_EXCLUSIVE";
    constexpr const char* M_CANNOT_LEAVE_SERVER_NOTICE_ROOM = "M_CANNOT_LEAVE_SERVER_NOTICE_ROOM";
    constexpr const char* M_WRONG_ROOM_KEYS_VERSION = "M_WRONG_ROOM_KEYS_VERSION";
    constexpr const char* M_WEAK_PASSWORD = "M_WEAK_PASSWORD";
    constexpr const char* M_PASSWORD_TOO_SHORT = "M_PASSWORD_TOO_SHORT";
    constexpr const char* M_PASSWORD_NO_DIGIT = "M_PASSWORD_NO_DIGIT";
    constexpr const char* M_PASSWORD_NO_UPPERCASE = "M_PASSWORD_NO_UPPERCASE";
    constexpr const char* M_PASSWORD_NO_LOWERCASE = "M_PASSWORD_NO_LOWERCASE";
    constexpr const char* M_PASSWORD_NO_SYMBOL = "M_PASSWORD_NO_SYMBOL";
    constexpr const char* M_PASSWORD_IN_DICTIONARY = "M_PASSWORD_IN_DICTIONARY";
    constexpr const char* M_TERMS_NOT_SIGNED = "M_TERMS_NOT_SIGNED";
    constexpr const char* M_INVALID_PEPPER = "M_INVALID_PEPPER";
    constexpr const char* ORG_MATRIX_EXPIRED_ACCOUNT = "ORG_MATRIX_EXPIRED_ACCOUNT";
    constexpr const char* LIMIT_TYPE_MAU = "monthly_active_user";
}

// --- Parsed Matrix Error ---
struct MatrixError {
    std::string code;              // M_FORBIDDEN, M_UNKNOWN, etc.
    std::string message;           // human-readable error message

    // Optional fields for specific error types
    std::optional<std::string> consentUri;       // M_CONSENT_NOT_GIVEN
    std::optional<std::string> limitType;        // M_RESOURCE_LIMIT_EXCEEDED
    std::optional<std::string> adminUri;         // M_RESOURCE_LIMIT_EXCEEDED
    std::optional<int64_t> retryAfterMs;         // M_LIMIT_EXCEEDED
    std::optional<bool> isSoftLogout;            // M_UNKNOWN_TOKEN
    std::optional<std::string> newLookupPepper;  // M_INVALID_PEPPER
    std::optional<std::string> session;          // UIA
    bool valid = false;                          // has errcode + error
};

// Parse a Matrix error JSON response.
MatrixError parseMatrixErrorJson(const std::string& json);

// Get a human-readable description for a Matrix error code.
std::string getErrorDescription(const std::string& errorCode);

// Check if a rate limit error (M_LIMIT_EXCEEDED).
bool isRateLimitError(const MatrixError& error);

// Get the retry delay from a rate limit error (ms).
int64_t getRetryAfterMs(const MatrixError& error);

// Check if the error is a soft logout (M_UNKNOWN_TOKEN + soft_logout=true).
bool isSoftLogout(const MatrixError& error);

// Check if consent is needed (M_CONSENT_NOT_GIVEN).
bool needsConsent(const MatrixError& error);

// Check if the error indicates user deactivation.
bool isUserDeactivated(const MatrixError& error);

// Check if the error is a password validation error.
bool isPasswordError(const std::string& errorCode);

// Get a list of all known Matrix error codes.
std::vector<std::string> getAllErrorCodes();

// Format error as JSON for Kotlin UI.
std::string matrixErrorToJson(const MatrixError& error);

} // namespace progressive

#endif // PROGRESSIVE_MATRIX_ERROR_HPP
