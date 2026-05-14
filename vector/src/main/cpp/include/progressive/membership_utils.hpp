#ifndef PROGRESSIVE_MEMBERSHIP_UTILS_HPP
#define PROGRESSIVE_MEMBERSHIP_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room Membership ----

enum class Membership { None, Join, Invite, Leave, Ban, Knock, Unknown };

struct MemberInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    Membership membership = Membership::Unknown;
    int powerLevel = 0;
    std::string reason;        // ban reason or invite reason
    int64_t timestampMs = 0;
    bool isFromCache = false;
};

struct MembershipChange {
    std::string userId;
    std::string displayName;
    Membership oldMembership = Membership::Unknown;
    Membership newMembership = Membership::Unknown;
    std::string senderId;       // who made the change
    int64_t timestampMs = 0;
};

// Parse membership from a state event.
MemberInfo parseMemberInfo(const std::string& stateContentJson, const std::string& userId);

// Parse membership string to enum.
Membership parseMembership(const std::string& membershipStr);

// Format membership as human-readable text.
std::string formatMembership(Membership membership);

// Check if membership is a positive state (join/invite/knock).
bool isActiveMember(Membership membership);

// Check if membership allows reading room messages.
bool canReadMessages(Membership membership);

// Detect membership changes between two state events.
MembershipChange detectMembershipChange(const MemberInfo& oldInfo, const MemberInfo& newInfo);

// Format a membership change as human-readable text.
std::string formatMembershipChange(const MembershipChange& change);

// ---- Member List ----

struct MemberListInfo {
    std::string roomId;
    std::vector<MemberInfo> members;
    int totalMembers = 0;
    int joinedMembers = 0;
    int invitedMembers = 0;
    int bannedMembers = 0;
    bool isTruncated = false;    // server truncated the list
};

// Parse member list from Matrix API response.
MemberListInfo parseMemberList(const std::string& roomId, const std::string& apiResponseJson, bool isTruncated);

// Filter members by membership type.
std::vector<MemberInfo> filterByMembership(const std::vector<MemberInfo>& members, Membership type);

// Filter members by display name (search).
std::vector<MemberInfo> searchMembers(const std::vector<MemberInfo>& members, const std::string& query);

// Sort members (name, power level, join date).
void sortMembers(std::vector<MemberInfo>& members, const std::string& sortBy);

// Format member list stats as JSON.
std::string memberListToJson(const MemberListInfo& list);

} // namespace progressive

#endif // PROGRESSIVE_MEMBERSHIP_UTILS_HPP
