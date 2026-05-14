#include "progressive/push_condition.hpp"
#include <sstream>
#include <cctype>

namespace progressive {

// ==== Glob to Regex (from Glob.kt:24-38) ====
// Original Kotlin:
//   fun String.simpleGlobToRegExp(): String = buildString {
//       forEach { char -> when (char) {
//           '*' -> append(".*")
//           '?' -> append(".")
//           '.' -> append("\\.")
//           '\\' -> append("\\\\")
//           else -> append(char)
//       }}
//   }

std::string globToRegex(const std::string& glob) {
    std::ostringstream regex;
    for (char c : glob) {
        switch (c) {
            case '*': regex << ".*"; break;
            case '?': regex << "."; break;
            case '.': regex << "\\."; break;
            case '\\': regex << "\\\\"; break;
            default: regex << c; break;
        }
    }
    return regex.str();
}

bool hasSpecialGlobChar(const std::string& pattern) {
    return pattern.find('*') != std::string::npos || pattern.find('?') != std::string::npos;
}

// ==== JSON Field Extraction (from EventMatchCondition.kt:86-104) ====
// Original Kotlin:
//   private fun extractField(jsonObject: Map<*, *>, fieldPath: String): String? {
//       val fieldParts = fieldPath.split(".")
//       var jsonElement: Map<*, *> = jsonObject
//       fieldParts.forEachIndexed { index, pathSegment ->
//           if (index == fieldParts.lastIndex) return jsonElement[pathSegment]?.toString()
//           else jsonElement = jsonElement[pathSegment] as? Map<*, *> ?: return null
//       }
//   }

std::string extractJsonField(const std::string& json, const std::string& fieldPath) {
    if (fieldPath.empty()) return "";

    // Split field path by "."
    std::vector<std::string> parts;
    {
        size_t start = 0;
        while (start < fieldPath.size()) {
            auto dot = fieldPath.find('.', start);
            if (dot == std::string::npos) {
                parts.push_back(fieldPath.substr(start));
                break;
            }
            parts.push_back(fieldPath.substr(start, dot - start));
            start = dot + 1;
        }
    }
    if (parts.empty()) return "";

    // Navigate JSON structure
    size_t searchStart = 0;
    for (size_t i = 0; i < parts.size(); ++i) {
        const auto& part = parts[i];
        bool isLast = (i == parts.size() - 1);

        // Find the key in current scope
        std::string keySearch = "\"" + part + "\":";
        auto keyPos = json.find(keySearch, searchStart);
        if (keyPos == std::string::npos) {
            keySearch = "\"" + part + "\": ";
            keyPos = json.find(keySearch, searchStart);
        }
        if (keyPos == std::string::npos) return "";

        size_t valueStart = keyPos + keySearch.size();

        if (isLast) {
            // Last part — extract string value
            while (valueStart < json.size() && json[valueStart] == ' ') valueStart++;
            if (valueStart < json.size() && json[valueStart] == '"') {
                // String value
                valueStart++;
                auto end = json.find('"', valueStart);
                if (end != std::string::npos) return json.substr(valueStart, end - valueStart);
            } else {
                // Numeric or boolean value — read until comma or brace
                size_t end = valueStart;
                while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']') end++;
                return json.substr(valueStart, end - valueStart);
            }
            return "";
        } else {
            // Navigate into nested object
            while (valueStart < json.size() && json[valueStart] != '{') valueStart++;
            if (valueStart >= json.size()) return "";
            searchStart = valueStart + 1;
        }
    }
    return "";
}

// ==== Simple Regex Match ====
// Implements basic regex matching without external library.
// Supports: . (any char), .* (any sequence), \\ (escaped chars), literal chars.
// Case-insensitive.

static bool regexMatchImpl(const std::string& text, size_t ti, const std::string& pattern, size_t pi) {
    if (pi >= pattern.size()) return ti >= text.size();

    // Handle .* — greedy match
    if (pi + 1 < pattern.size() && pattern[pi] == '.' && pattern[pi + 1] == '*') {
        // Try matching from current position forward
        for (size_t k = ti; k <= text.size(); ++k) {
            if (regexMatchImpl(text, k, pattern, pi + 2)) return true;
        }
        return false;
    }

    // Handle escape sequences
    if (pattern[pi] == '\\' && pi + 1 < pattern.size()) {
        if (ti < text.size() && std::tolower(static_cast<unsigned char>(text[ti])) ==
            std::tolower(static_cast<unsigned char>(pattern[pi + 1]))) {
            return regexMatchImpl(text, ti + 1, pattern, pi + 2);
        }
        return false;
    }

    // Handle . (any char)
    if (pattern[pi] == '.') {
        return ti < text.size() && regexMatchImpl(text, ti + 1, pattern, pi + 1);
    }

    // Literal match (case-insensitive)
    if (ti < text.size() && std::tolower(static_cast<unsigned char>(text[ti])) ==
        std::tolower(static_cast<unsigned char>(pattern[pi]))) {
        return regexMatchImpl(text, ti + 1, pattern, pi + 1);
    }

    return false;
}

bool simpleRegexMatch(const std::string& text, const std::string& regexPattern) {
    return regexMatchImpl(text, 0, regexPattern, 0);
}

bool simpleRegexContainsMatch(const std::string& text, const std::string& regexPattern) {
    for (size_t i = 0; i <= text.size(); ++i) {
        if (regexMatchImpl(text, i, regexPattern, 0)) return true;
    }
    return false;
}

// ==== Event Match Condition Evaluator (from EventMatchCondition.kt:41-84) ====
// Original Kotlin:
//   fun isSatisfied(event: Event): Boolean {
//       val value = extractField(rawJson, key) ?: return false
//       return if (key == "content.body") {
//           val modPattern = if (pattern.startsWith("*") && pattern.endsWith("*"))
//               pattern.removePrefix("*").removeSuffix("*").simpleGlobToRegExp()
//           else "(\\W|^)" + pattern.simpleGlobToRegExp() + "(\\W|$)"
//           regex.containsMatchIn(value)
//       } else {
//           regex.matches(value)
//       }
//   }

bool evaluateEventMatchCondition(
    const std::string& eventJson,
    const std::string& key,
    const std::string& pattern)
{
    std::string value = extractJsonField(eventJson, key);
    if (value.empty()) return false;

    // Convert glob pattern to regex
    std::string modPattern;

    // Original: special handling for content.body — word boundary matching
    if (key == "content.body") {
        if (!pattern.empty() && pattern.front() == '*' && pattern.back() == '*') {
            // Original: removePrefix("*").removeSuffix("*")
            // Leading/trailing stars don't affect containsMatchIn result
            modPattern = globToRegex(pattern.substr(1, pattern.size() - 2));
        } else {
            // Word boundary: (\\W|^)pattern(\\W|$)
            modPattern = "(\\W|^)" + globToRegex(pattern) + "(\\W|$)";
        }
        // Original: regex.containsMatchIn(value) — match anywhere in the value
        return simpleRegexContainsMatch(value, modPattern);
    } else {
        // Original: regex.matches(value) — match the ENTIRE value
        // For patterns without special glob chars, prepend and append .*
        if (!hasSpecialGlobChar(pattern)) {
            modPattern = ".*" + globToRegex(pattern) + ".*";
        } else {
            modPattern = globToRegex(pattern);
        }
        return simpleRegexMatch(value, modPattern);
    }
}

// ==== Generic Push Condition Evaluator ====

PushCondition evaluatePushCondition(
    const PushCondition& condition,
    const std::string& eventJson)
{
    PushCondition result = condition;

    // Original: when (kind) { "event_match" -> ... }
    if (condition.kind == "event_match") {
        result.isSatisfied = evaluateEventMatchCondition(eventJson, condition.key, condition.pattern);
    }
    // Other condition kinds (room_member_count, sender_notification_permission)
    // require server-side evaluation or room state — not evaluable from event alone.

    return result;
}

std::string pushConditionToJson(const PushCondition& condition) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"kind": ")" << esc(condition.kind) << R"(",)";
    json << R"("key": ")" << esc(condition.key) << R"(",)";
    json << R"("pattern": ")" << esc(condition.pattern) << R"(",)";
    json << R"("isSatisfied": )" << (condition.isSatisfied ? "true" : "false") << "}";
    return json.str();
}

} // namespace progressive
