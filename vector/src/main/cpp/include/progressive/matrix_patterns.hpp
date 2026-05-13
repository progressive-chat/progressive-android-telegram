#ifndef PROGRESSIVE_MATRIX_PATTERNS_HPP
#define PROGRESSIVE_MATRIX_PATTERNS_HPP

#include <string>
#include <vector>

namespace progressive {

// Matrix Identifier grammar (per spec appendices).
// https://matrix.org/docs/spec/appendices#identifier-grammar

// Check if a string is a valid Matrix user ID: @user:server
bool isUserId(const std::string& input);

// Check if a string is a valid Matrix room ID: !xxx:server
bool isRoomId(const std::string& input);

// Check if a string is a valid Matrix room alias: #alias:server
bool isRoomAlias(const std::string& input);

// Check if a string is a valid Matrix event ID: $xxx
bool isEventId(const std::string& input);

// Check if a string is a valid Matrix group ID: +group:server
bool isGroupId(const std::string& input);

// Check if a URL is a Matrix.to permalink.
bool isMatrixToPermalink(const std::string& url);

// Check if a URL is an app permalink (https://domain/#/room/user/...).
bool isAppPermalink(const std::string& url);

// Parse a Matrix.to permalink and extract the type and identifier.
// e.g. "https://matrix.to/#/@user:server" → {"user", "@user:server"}
// e.g. "https://matrix.to/#/!room:server/$event" → {"room", "!room:server", "$event"}
struct PermalinkInfo {
    std::string type;        // "user", "room", "event"
    std::string roomId;
    std::string userId;
    std::string eventId;
    bool valid = false;
};
PermalinkInfo parseMatrixToPermalink(const std::string& url);

// Extract all Matrix IDs (user, room, alias, event) from text.
struct ExtractedIds {
    std::vector<std::string> userIds;
    std::vector<std::string> roomIds;
    std::vector<std::string> roomAliases;
    std::vector<std::string> eventIds;
};
ExtractedIds extractMatrixIds(const std::string& text);

// Check if a string is a valid matrix.to URL for a room.
bool isMxcUrl(const std::string& url);

// Check if a string is a valid phone number (E.164-ish).
bool isPhoneNumber(const std::string& input);

// Check if a string is a valid email address.
bool isValidEmail(const std::string& input);

} // namespace progressive

#endif // PROGRESSIVE_MATRIX_PATTERNS_HPP
