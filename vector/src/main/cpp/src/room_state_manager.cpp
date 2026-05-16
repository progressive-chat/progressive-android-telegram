#include "progressive/room_state_manager.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ====== Enums ======

const char* historyVisibilityToString(RoomHistoryVisibility v) {
    switch (v) {
        case RoomHistoryVisibility::WORLD_READABLE: return "world_readable";
        case RoomHistoryVisibility::SHARED: return "shared";
        case RoomHistoryVisibility::INVITED: return "invited";
        case RoomHistoryVisibility::JOINED: return "joined";
    }
    return "shared";
}

RoomHistoryVisibility historyVisibilityFromString(const std::string& s) {
    if (s == "world_readable") return RoomHistoryVisibility::WORLD_READABLE;
    if (s == "shared") return RoomHistoryVisibility::SHARED;
    if (s == "invited") return RoomHistoryVisibility::INVITED;
    if (s == "joined") return RoomHistoryVisibility::JOINED;
    return RoomHistoryVisibility::SHARED;
}

const char* joinRuleToString(RoomJoinRule rule) {
    switch (rule) {
        case RoomJoinRule::PUBLIC: return "public";
        case RoomJoinRule::INVITE: return "invite";
        case RoomJoinRule::KNOCK: return "knock";
        case RoomJoinRule::PRIVATE: return "private";
    }
    return "invite";
}

RoomJoinRule joinRuleFromString(const std::string& s) {
    if (s == "public") return RoomJoinRule::PUBLIC;
    if (s == "invite") return RoomJoinRule::INVITE;
    if (s == "knock") return RoomJoinRule::KNOCK;
    if (s == "private") return RoomJoinRule::PRIVATE;
    return RoomJoinRule::INVITE;
}

// ====== Helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

// ====== History Visibility Functions ======
// Original: shouldShareHistory() = WORLD_READABLE || SHARED

bool shouldShareHistory(RoomHistoryVisibility visibility) {
    return visibility == RoomHistoryVisibility::WORLD_READABLE ||
           visibility == RoomHistoryVisibility::SHARED;
}

bool canSeeEvent(RoomHistoryVisibility visibility, MembershipState memberStateAtEventTime,
                  MembershipState memberCurrentState) {
    switch (visibility) {
        case RoomHistoryVisibility::WORLD_READABLE:
            // Anyone can see all events, even non-members
            return true;

        case RoomHistoryVisibility::SHARED:
            // Joined members see all events; non-members see nothing
            return memberCurrentState == MembershipState::JOIN;

        case RoomHistoryVisibility::INVITED:
            // Members see events from when they were invited onwards
            return memberCurrentState == MembershipState::JOIN ||
                   memberCurrentState == MembershipState::INVITE ||
                   (memberStateAtEventTime == MembershipState::INVITE);

        case RoomHistoryVisibility::JOINED:
            // Members see events from when they joined onwards
            return memberCurrentState == MembershipState::JOIN &&
                   memberStateAtEventTime == MembershipState::JOIN;
    }
    return false;
}

bool canNonMemberSeeEvents(RoomHistoryVisibility visibility) {
    return visibility == RoomHistoryVisibility::WORLD_READABLE;
}

std::string getVisibilityLabel(RoomHistoryVisibility visibility) {
    switch (visibility) {
        case RoomHistoryVisibility::WORLD_READABLE: return "Anyone";
        case RoomHistoryVisibility::SHARED: return "Members (since beginning)";
        case RoomHistoryVisibility::INVITED: return "Members (since invite)";
        case RoomHistoryVisibility::JOINED: return "Members (since join)";
    }
    return "Members";
}

std::string getVisibilityDescription(RoomHistoryVisibility visibility) {
    switch (visibility) {
        case RoomHistoryVisibility::WORLD_READABLE:
            return "Anyone can read the room history, even without joining.";
        case RoomHistoryVisibility::SHARED:
            return "All members can see the entire room history.";
        case RoomHistoryVisibility::INVITED:
            return "Members can see history from the point they were invited.";
        case RoomHistoryVisibility::JOINED:
            return "Members can only see history from the point they joined.";
    }
    return "";
}

// ====== Content Builders ======

std::string buildHistoryVisibilityContent(RoomHistoryVisibility visibility) {
    return R"({"history_visibility":")" + std::string(historyVisibilityToString(visibility)) + R"("})";
}

std::string buildJoinRulesContent(RoomJoinRule rule) {
    return R"({"join_rule":")" + std::string(joinRuleToString(rule)) + R"("})";
}

RoomHistoryVisibility parseHistoryVisibility(const std::string& contentJson) {
    auto vis = extractStr(contentJson, "history_visibility");
    return historyVisibilityFromString(vis);
}

RoomJoinRule parseJoinRules(const std::string& contentJson) {
    auto rule = extractStr(contentJson, "join_rule");
    return joinRuleFromString(rule);
}

// ====== RoomStateManager ======

RoomStateManager::RoomStateManager() {}

RoomStateSummary& RoomStateManager::getOrCreateState(const std::string& roomId) {
    auto it = rooms_.find(roomId);
    if (it == rooms_.end()) {
        RoomStateSummary s;
        s.roomId = roomId;
        rooms_[roomId] = s;
    }
    return rooms_[roomId];
}

void RoomStateManager::setHistoryVisibility(const std::string& roomId, RoomHistoryVisibility visibility) {
    auto& state = getOrCreateState(roomId);
    state.historyVisibility = visibility;
    state.isWorldReadable = (visibility == RoomHistoryVisibility::WORLD_READABLE);
    state.canShareHistory = shouldShareHistory(visibility);
}

void RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRule rule) {
    auto& state = getOrCreateState(roomId);
    state.joinRule = rule;
    state.isPublicRoom = (rule == RoomJoinRule::PUBLIC);
}

void RoomStateManager::setRoomName(const std::string& roomId, const std::string& name) {
    getOrCreateState(roomId).roomName = name;
}

void RoomStateManager::setEncrypted(const std::string& roomId, bool encrypted) {
    getOrCreateState(roomId).isEncrypted = encrypted;
}

void RoomStateManager::setMemberCount(const std::string& roomId, int count) {
    getOrCreateState(roomId).memberCount = count;
}

RoomStateSummary RoomStateManager::getRoomState(const std::string& roomId) const {
    auto it = rooms_.find(roomId);
    if (it != rooms_.end()) return it->second;
    RoomStateSummary s;
    s.roomId = roomId;
    return s;
}

bool RoomStateManager::canShareRoomHistory(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.canShareHistory;
}

bool RoomStateManager::isPublicRoom(const std::string& roomId) const {
    return getRoomState(roomId).isPublicRoom;
}

bool RoomStateManager::isWorldReadable(const std::string& roomId) const {
    return getRoomState(roomId).isWorldReadable;
}

bool RoomStateManager::isInviteOnly(const std::string& roomId) const {
    return getRoomState(roomId).joinRule == RoomJoinRule::INVITE;
}

bool RoomStateManager::areGuestsAllowed(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.isPublicRoom && state.isWorldReadable;
}

void RoomStateManager::clear() { rooms_.clear(); }

// ====== Serialization ======

std::string RoomStateManager::roomStateToJson(const RoomStateSummary& state) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"room_id":")" << esc(state.roomId)
       << R"(","name":")" << esc(state.roomName)
       << R"(","history_visibility":")" << historyVisibilityToString(state.historyVisibility)
       << R"(","visibility_label":")" << getVisibilityLabel(state.historyVisibility)
       << R"(","join_rule":")" << joinRuleToString(state.joinRule)
       << R"(,"is_public":)" << (state.isPublicRoom ? "true" : "false")
       << R"(,"is_world_readable":)" << (state.isWorldReadable ? "true" : "false")
       << R"(,"can_share_history":)" << (state.canShareHistory ? "true" : "false")
       << R"(,"is_invite_only":)" << (joinRuleToString(state.joinRule) == std::string("invite") ? "true" : "false")
       << R"(,"is_encrypted":)" << (state.isEncrypted ? "true" : "false")
       << R"(,"members":)" << state.memberCount
       << "}";
    return os.str();
}

} // namespace progressive
