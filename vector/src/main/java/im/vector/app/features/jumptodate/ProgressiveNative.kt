package im.vector.app.features.jumptodate

import org.json.JSONObject
import timber.log.Timber

/**
 * JNI bridge to the progressive_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object ProgressiveNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load progressive_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    /**
     * Validates the date string (YYYY-MM-DD) and builds the MSC3030 URL.
     * Gates on the feature flag from C++ side.
     *
     * @param roomId Target room ID
     * @param dateString Date in YYYY-MM-DD format
     * @param serverUrl Base URL of the homeserver
     * @param accessToken User's access token
     * @param isEnabled Whether jumptodate labs setting is enabled
     *
     * @return JSON string with url, accessToken, timestamp — or {"error":"..."} on failure
     */
    @JvmStatic
    external fun nativeValidateAndBuild(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): String

    /**
     * Parses the UTC timestamp_to_event response from the homeserver.
     *
     * @param responseBody HTTP response body
     * @param httpStatus HTTP status code
     *
     * @return JSON string with eventId — or error info on failure
     */
    @JvmStatic
    external fun nativeParseResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    // --- Pure Kotlin fallback implementations ---

    fun validateAndBuildFallback(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): JSONObject {
        val result = JSONObject()

        if (!isEnabled) {
            return result.put("error", "/jumptodate is disabled. Enable it in Settings → Labs.")
        }

        // Validate date format
        val dateRegex = Regex("^(\\d{4})-(\\d{2})-(\\d{2})$")
        val match = dateRegex.matchEntire(dateString)
            ?: return result.put("error", "Invalid date format. Use YYYY-MM-DD.")

        val year = match.groupValues[1].toInt()
        val month = match.groupValues[2].toInt()
        val day = match.groupValues[3].toInt()

        if (month < 1 || month > 12) return result.put("error", "Invalid month. Must be 01-12.")
        if (day < 1 || day > 31) return result.put("error", "Invalid day. Must be 01-31.")

        // Days per month validation
        val daysInMonth = intArrayOf(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
        val isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
        val maxDay = if (month == 2 && isLeapYear) 29 else daysInMonth[month - 1]

        if (day > maxDay) return result.put("error", "Invalid day for given month.")

        // Compute Unix timestamp (UTC, milliseconds) using Java's Calendar
        val cal = java.util.GregorianCalendar(java.util.TimeZone.getTimeZone("UTC"))
        cal.set(year, month - 1, day, 0, 0, 0)
        cal.set(java.util.Calendar.MILLISECOND, 0)
        val timestamp = cal.timeInMillis

        // Build MSC3030 URL
        val url = "$serverUrl/_matrix/client/unstable/org.matrix.msc3030/rooms/$roomId/timestamp_to_event?ts=$timestamp&dir=f"

        result.put("url", url)
        result.put("accessToken", accessToken)
        result.put("timestamp", timestamp)

        return result
    }

    fun parseResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()

        if (responseBody == null) {
            return result.put("error", "Empty response from server.")
                    .put("statusCode", httpStatus)
        }

        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val errcode = json.optString("errcode", "")
                val error = json.optString("error", "")
                val msg = buildString {
                    append("Server returned $httpStatus")
                    if (errcode.isNotEmpty()) append(" ($errcode)")
                    if (error.isNotEmpty()) append(": $error")
                }
                result.put("error", msg)
                result.put("statusCode", httpStatus)
                return result
            } catch (e: Exception) {
                return result.put("error", "Server returned $httpStatus")
                        .put("statusCode", httpStatus)
            }
        }

        try {
            val json = JSONObject(responseBody)
            val eventId = json.optString("event_id", "")

            if (eventId.isEmpty()) {
                return result.put("error", "Response missing event_id field.")
                        .put("statusCode", httpStatus)
            }

            if (eventId == "~") {
                return result.put("error", "No events found after the given date.")
                        .put("statusCode", httpStatus)
            }

            result.put("eventId", eventId)
            result.put("statusCode", httpStatus)
            return result
        } catch (e: Exception) {
            return result.put("error", "Failed to parse server response.")
                    .put("statusCode", httpStatus)
        }
    }
}
