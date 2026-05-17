#include "progressive/room_state.hpp"
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room History Visibility Manager
//
// Faithful port from Element Android original sources:
//   RoomHistoryVisibility.kt — WORLD_READABLE, SHARED, INVITED, JOINED
//     shouldShareHistory() → WORLD_READABLE | SHARED
//   RoomJoinRules.kt — PUBLIC, INVITE, KNOCK, etc.
//
// Matrix spec: m.room.history_visibility state event
// Controls who can see what in the room based on membership.
//
// Covers:
//   1. History visibility types (world_readable, shared, invited, joined)
//   2. Who can see events based on membership state
//   3. Share history check (for MSC3061 key sharing)
//   4. Join rule integration
//   5. Build state event content
// ================================================================

// ---- Room History Visibility ----
// Original: RoomHistoryVisibility.kt (WORLD_READABLE, SHARED, INVITED, JOINED)


const char* historyVisibilityToString(RoomHistoryVisibility v);
RoomHistoryVisibility historyVisibilityFromString(const std::string& s);

// ---- Room Join Rules ----


const char* joinRuleToString(RoomJoinRules rule);
RoomJoinRules joinRuleFromString(const std::string& s);

// ---- Membership State (for visibility checks) ----


// ---- Room State Summary ----


// ---- History Visibility Check ----
// Original: shouldShareHistory() = WORLD_READABLE || SHARED

// Check if history can be shared with new members.
// Per MSC3061: only WORLD_READABLE and SHARED allow sharing.
bool shouldShareHistory(RoomHistoryVisibility visibility);

// Check if a member can see an event based on history_visibility.
// Takes: history visibility, member's state when event was sent, member's current state.
bool canSeeEvent(RoomHistoryVisibility visibility, Membership memberStateAtEventTime,
                  Membership memberCurrentState);

// Check if a non-member can see events.
bool canNonMemberSeeEvents(RoomHistoryVisibility visibility);

// Get the effective visibility for a given membership.
std::string getVisibilityLabel(RoomHistoryVisibility visibility);

// Get a human-readable description of what the visibility means.
std::string getVisibilityDescription(RoomHistoryVisibility visibility);

// ---- Room State Builder ----
// Original: Build m.room.history_visibility state event content

// Build history_visibility state event content.
std::string buildHistoryVisibilityContent(RoomHistoryVisibility visibility);

// Build join_rules state event content.
std::string buildJoinRulesContent(RoomJoinRules rule);

// Parse history visibility from state event content.
RoomHistoryVisibility parseHistoryVisibility(const std::string& contentJson);

// Parse join rules from state event content.
RoomJoinRules parseJoinRules(const std::string& contentJson);

// ---- Room State Manager ----

class RoomStateManager {
public:
    RoomStateManager();

    // ====== Room State ======

    // Set room state from events.
    void setHistoryVisibility(const std::string& roomId, RoomHistoryVisibility visibility);
    void setJoinRule(const std::string& roomId, RoomJoinRules rule);
    void setRoomName(const std::string& roomId, const std::string& name);
    void setEncrypted(const std::string& roomId, bool encrypted);
    void setMemberCount(const std::string& roomId, int count);

    // Get room state summary.
    RoomStateSummary getRoomState(const std::string& roomId) const;

    // ====== Visibility Checks ======

    // Check if room history can be shared (for MSC3061 key sharing).
    bool canShareRoomHistory(const std::string& roomId) const;

    // Check if a room is publicly viewable.
    bool isPublicRoom(const std::string& roomId) const;

    // Check if a room is world-readable.
    bool isWorldReadable(const std::string& roomId) const;

    // Check if a room is invite-only.
    bool isInviteOnly(const std::string& roomId) const;

    // Check if guests are allowed.
    bool areGuestsAllowed(const std::string& roomId) const;

    // ====== Serialization ======

    // Export room state as JSON.
    std::string roomStateToJson(const RoomStateSummary& state) const;

    // Clear room state.
    void clear();

private:
    std::unordered_map<std::string, RoomStateSummary> rooms_; // roomId → state
    RoomStateSummary& getOrCreateState(const std::string& roomId);
};

} // namespace progressive
