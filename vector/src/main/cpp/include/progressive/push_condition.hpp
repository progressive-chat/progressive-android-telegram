#ifndef PROGRESSIVE_PUSH_CONDITION_HPP
#define PROGRESSIVE_PUSH_CONDITION_HPP

#include <string>

namespace progressive {

// ---- Push Rule Condition Evaluator ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.pushrules.EventMatchCondition.kt (105 lines)
//   org.matrix.android.sdk.internal.util.Glob.kt (39 lines)
//
// Evaluates whether a Matrix event matches a push notification rule condition.
// Conditions use glob-style patterns matched against dot-separated event fields.

// Convert a glob pattern to a regular expression.
// Original Kotlin (Glob.kt:simpleGlobToRegExp):
//   * → .*   ? → .   . → \\.   \\ → \\\\
std::string globToRegex(const std::string& glob);

// Check if a glob pattern contains special characters (* or ?).
// Original: contains("*") || contains("?")
bool hasSpecialGlobChar(const std::string& pattern);

// Evaluate a push rule condition against an event.
// Original Kotlin (EventMatchCondition.kt:isSatisfied):
//   - Extracts field via dot-separated key (e.g., "content.body")
//   - For "content.body": matches any word-boundary substring
//   - For other fields: matches the entire value
//   - Matching is case-insensitive
//
// @param eventJson  The full event JSON
// @param key        Dot-separated field path, e.g. "content.body"
// @param pattern    Glob-style pattern, e.g. "*hello*"
// @return true if the condition matches
bool evaluateEventMatchCondition(
    const std::string& eventJson,
    const std::string& key,
    const std::string& pattern
);

// Extract a value from a JSON object using a dot-separated path.
// "content.body" → event["content"]["body"]
// Original Kotlin (EventMatchCondition.kt:extractField)
std::string extractJsonField(const std::string& json, const std::string& fieldPath);

// Check if a string matches a regex pattern (case-insensitive).
// Simple implementation without external regex library.
bool simpleRegexMatch(const std::string& text, const std::string& regexPattern);

// Check if a regex pattern matches ANYWHERE in the text (contains match).
bool simpleRegexContainsMatch(const std::string& text, const std::string& regexPattern);

// Structure representing a push rule condition.
struct PushCondition {
    std::string kind;        // "event_match", "room_member_count", "sender_notification_permission"
    std::string key;         // for event_match: "content.body", "sender", "room_id", "type"
    std::string pattern;     // glob pattern
    bool isSatisfied = false;
};

// Evaluate a generic push condition against event JSON.
// Currently supports: event_match kind.
// Returns updated condition with isSatisfied set.
PushCondition evaluatePushCondition(
    const PushCondition& condition,
    const std::string& eventJson
);

// Format push condition as JSON for Kotlin UI.
std::string pushConditionToJson(const PushCondition& condition);

} // namespace progressive

#endif // PROGRESSIVE_PUSH_CONDITION_HPP
