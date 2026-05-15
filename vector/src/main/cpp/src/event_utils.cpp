#include "progressive/event_utils.hpp"
#include <algorithm>
#include <sstream>

namespace progressive {

// ==== Event Summary ====

std::string formatEventSummary(
    const std::string& eventType,
    const std::string& msgType,
    const std::string& senderName,
    const std::string& body,
    const std::string& membership,
    const std::string& displayName,
    bool isRedacted,
    bool isEncrypted)
{
    if (isRedacted) return "Message removed";

    if (isEncrypted) return senderName.empty() ? "encrypted message" : senderName + ": encrypted message";

    // State events: membership changes
    if (eventType == "m.room.member") {
        std::string prefix = senderName.empty() ? "Someone" : senderName;
        if (membership == "join") {
            return prefix + " joined the room";
        } else if (membership == "invite") {
            return prefix + " was invited";
        } else if (membership == "leave" || membership == "ban") {
            return prefix + " left the room";
        } else if (!displayName.empty()) {
            return prefix + " changed their name to " + displayName;
        }
    }

    // Room state changes
    if (eventType == "m.room.name") {
        return senderName.empty() ? "Room name changed" : senderName + " changed the room name";
    }
    if (eventType == "m.room.topic") {
        return senderName.empty() ? "Topic changed" : senderName + " changed the topic";
    }
    if (eventType == "m.room.avatar") {
        return senderName.empty() ? "Avatar changed" : senderName + " changed the room avatar";
    }
    if (eventType == "m.room.create") {
        return "Room created";
    }
    if (eventType == "m.room.tombstone") {
        return "Room upgraded";
    }
    if (eventType == "m.room.join_rules") {
        return senderName.empty() ? "Join rules changed" : senderName + " changed join rules";
    }
    if (eventType == "m.room.encryption") {
        return senderName.empty() ? "Encryption enabled" : senderName + " enabled encryption";
    }
    if (eventType == "m.room.power_levels") {
        return senderName.empty() ? "Power levels changed" : senderName + " changed power levels";
    }

    // Messages
    if (eventType == "m.room.message" || eventType == "m.sticker") {
        // Format: "Sender: preview" or "Sender sent X"
        if (msgType == "m.text" || msgType == "m.notice") {
            std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
            return senderName.empty() ? preview : senderName + ": " + preview;
        } else if (msgType == "m.emote") {
            std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
            return senderName.empty() ? preview : "* " + senderName + " " + preview;
        } else if (msgType == "m.image") {
            return senderName.empty() ? "sent an image" : senderName + " sent an image";
        } else if (msgType == "m.video") {
            return senderName.empty() ? "sent a video" : senderName + " sent a video";
        } else if (msgType == "m.audio") {
            return senderName.empty() ? "sent an audio file" : senderName + " sent a voice message";
        } else if (msgType == "m.file") {
            return senderName.empty() ? "sent a file" : senderName + " sent a file";
        } else if (msgType == "m.location") {
            return senderName.empty() ? "shared their location" : senderName + " shared their location";
        }
    }

    // Sticker
    if (eventType == "m.sticker") {
        return senderName.empty() ? "sent a sticker" : senderName + " sent a sticker";
    }

    // Poll
    if (eventType == "m.poll.start" || eventType == "org.matrix.msc3381.poll.start") {
        return senderName.empty() ? "created a poll" : senderName + " created a poll";
    }
    if (eventType == "m.poll.end" || eventType == "org.matrix.msc3381.poll.end") {
        return senderName.empty() ? "ended a poll" : senderName + " ended a poll";
    }

    // Call
    if (eventType == "m.call.invite") {
        return senderName.empty() ? "started a call" : senderName + " started a call";
    }
    if (eventType == "m.call.hangup") {
        return "Call ended";
    }

    // Key verification
    if (eventType == "m.key.verification.request") {
        return senderName.empty() ? "verification request" : senderName + " wants to verify";
    }
    if (eventType == "m.key.verification.done") {
        return "Verification complete";
    }

    // Beacon / Live location
    if (eventType == "m.beacon_info") {
        return senderName.empty() ? "Live location" : senderName + " is sharing live location";
    }

    // Fallback: show the body if available
    if (!body.empty()) {
        std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
        return senderName.empty() ? preview : senderName + ": " + preview;
    }

    return senderName.empty() ? "sent a message" : senderName + " sent a message";
}

// ==== Read Marker ====

ReadMarkerPosition calculateReadMarker(
    const std::vector<std::string>& eventIds,
    const std::string& readEventId,
    const std::vector<std::string>& highlightIds)
{
    ReadMarkerPosition result;

    if (eventIds.empty()) return result;

    // Find the position of the read event
    int readIndex = -1;
    if (!readEventId.empty()) {
        for (int i = (int)eventIds.size() - 1; i >= 0; i--) {
            if (eventIds[i] == readEventId) {
                readIndex = i;
                break;
            }
        }
    }

    if (readIndex < 0) {
        // Nothing read — everything is unread
        result.eventIndex = -1;
        result.unreadCount = (int)eventIds.size();
        result.allRead = false;
    } else if (readIndex == (int)eventIds.size() - 1) {
        // Last event is read — everything read
        result.eventIndex = -1;
        result.unreadCount = 0;
        result.allRead = true;
    } else {
        result.eventIndex = readIndex;
        result.unreadCount = (int)eventIds.size() - readIndex - 1;
        result.allRead = false;
    }

    // Count highlights after the read marker
    for (const auto& id : highlightIds) {
        int idx = -1;
        for (int i = 0; i < (int)eventIds.size(); i++) {
            if (eventIds[i] == id) { idx = i; break; }
        }
        if (idx > readIndex) result.highlightCount++;
    }

    return result;
}

// ==== Typing Indicator ====

std::string formatTypingIndicator(
    const std::vector<std::string>& typingUserNames,
    int maxNamesShown)
{
    if (typingUserNames.empty()) return "";

    int count = (int)typingUserNames.size();

    if (count == 1) {
        return typingUserNames[0] + " is typing...";
    }

    if (count == 2) {
        return typingUserNames[0] + " and " + typingUserNames[1] + " are typing...";
    }

    int shown = std::min(count, maxNamesShown);
    std::string result;
    for (int i = 0; i < shown; i++) {
        if (i > 0) {
            result += (i == shown - 1 && count <= maxNamesShown) ? " and " : ", ";
        }
        result += typingUserNames[i];
    }

    int remaining = count - shown;
    if (remaining > 0) {
        result += " and " + std::to_string(remaining) + " other";
        if (remaining > 1) result += "s";
    }

    result += (count == 1) ? " is typing..." : " are typing...";
    return result;
}

// ==== Power Level Capabilities ====

PowerLevelCapabilities calculateCapabilities(
    int userPowerLevel,
    int eventsDefault,
    int stateDefault,
    int inviteLevel,
    int kickLevel,
    int banLevel,
    int redactLevel,
    int notifyRoomLevel,
    const std::unordered_map<std::string, int>& eventTypeLevels)
{
    PowerLevelCapabilities c;

    c.isOwner = userPowerLevel >= 100;
    c.isModerator = userPowerLevel >= 50;

    c.canSendMessages = userPowerLevel >= eventsDefault;
    c.canSendState = userPowerLevel >= stateDefault;

    c.canInvite = userPowerLevel >= inviteLevel;
    c.canKick = userPowerLevel >= kickLevel;
    c.canBan = userPowerLevel >= banLevel;
    c.canRedact = userPowerLevel >= redactLevel;
    c.canRedactOthers = userPowerLevel >= 50; // Moderator+

    c.canChangeRoomName = userPowerLevel >= stateDefault;
    c.canChangeRoomTopic = userPowerLevel >= stateDefault;
    c.canChangeRoomAvatar = userPowerLevel >= stateDefault;
    c.canChangeJoinRules = userPowerLevel >= stateDefault;
    c.canChangeHistoryVisibility = userPowerLevel >= stateDefault;
    c.canChangeGuestAccess = userPowerLevel >= stateDefault;

    c.canNotifyEveryone = userPowerLevel >= notifyRoomLevel;

    // Check specific event type overrides
    auto checkOverridden = [&](const std::string& type) -> bool {
        auto it = eventTypeLevels.find(type);
        if (it != eventTypeLevels.end()) return userPowerLevel >= it->second;
        return userPowerLevel >= eventsDefault;
    };

    c.canSendMessages = checkOverridden("m.room.message");
    c.canSendState = std::max(userPowerLevel >= stateDefault, userPowerLevel >= 50);

    return c;
}

} // namespace progressive
