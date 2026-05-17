#include "progressive/room_state_manager.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ====== Enums ======





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






// ====== Content Builders ======





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

void RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRules rule) {
    auto& state = getOrCreateState(roomId);
    state.joinRule = rule;
    state.isPublicRoom = (rule == RoomJoinRules::PUBLIC);
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
    return getRoomState(roomId).joinRule == RoomJoinRules::INVITE;
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
