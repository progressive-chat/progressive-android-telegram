// Auto-generated stubs with exact return types from headers
#include "progressive/content_utils.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"

namespace progressive {

std::string RoomDirectoryManager::buildPublicRoomsRequest(const PublicRoomsParams& params) const { return {}; }
PublicRoomsResponse RoomDirectoryManager::parsePublicRoomsResponse(const std::string& json) const { return {}; }
void RoomDirectoryManager::accumulateResults(PublicRoomsResponse& existing, const PublicRoomsResponse& nextPage) const {}
std::string RoomDirectoryManager::buildVisibilityRequest(RoomDirectoryVisibility visibility) const { return {}; }
RoomDirectoryVisibility RoomDirectoryManager::parseVisibilityResponse(const std::string& json) const { return {}; }
std::string RoomDirectoryManager::buildAliasCheckRequest(const std::string& aliasLocalPart) const { return {}; }
AliasAvailabilityResult RoomDirectoryManager::parseAliasAvailability(const std::string& json, const std::string& aliasLocalPart) const { return {}; }
std::string RoomDirectoryManager::formatRoomPreview(const PublicRoom& room) const { return {}; }
std::string RoomDirectoryManager::buildRoomJoinUrl(const std::string& roomId, const std::string& viaServer) const { return {}; }
std::string RoomDirectoryManager::buildRoomAvatarUrl(const std::string& avatarUrl, const std::string& homeServer) const { return {}; }
void RoomDirectoryManager::sortRoomsByPopularity(std::vector<PublicRoom>& rooms) const {}
void RoomDirectoryManager::sortRoomsByName(std::vector<PublicRoom>& rooms) const {}
std::string RoomDirectoryManager::roomToJson(const PublicRoom& room) const { return {}; }
std::string RoomDirectoryManager::roomsToJson(const std::vector<PublicRoom>& rooms) const { return {}; }
std::string RoomDirectoryManager::responseToJson(const PublicRoomsResponse& resp) const { return {}; }
std::string RoomDirectoryManager::aliasResultToJson(const AliasAvailabilityResult& result) const { return {}; }
std::string RoomDirectoryManager::extractStr(const std::string& json, const std::string& key) { return {}; }
int64_t RoomDirectoryManager::extractInt(const std::string& json, const std::string& key) { return {}; }
bool RoomDirectoryManager::extractBool(const std::string& json, const std::string& key) { return {}; }
void RoomStateManager::setHistoryVisibility(const std::string& roomId, RSM_RoomHistoryVisibility visibility) {}
void RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRule rule) {}
void RoomStateManager::setRoomName(const std::string& roomId, const std::string& name) {}
void RoomStateManager::setEncrypted(const std::string& roomId, bool encrypted) {}
void RoomStateManager::setMemberCount(const std::string& roomId, int count) {}
RoomStateSummary RoomStateManager::getRoomState(const std::string& roomId) const { return {}; }
bool RoomStateManager::canShareRoomHistory(const std::string& roomId) const { return {}; }
bool RoomStateManager::isPublicRoom(const std::string& roomId) const { return {}; }
bool RoomStateManager::isWorldReadable(const std::string& roomId) const { return {}; }
bool RoomStateManager::isInviteOnly(const std::string& roomId) const { return {}; }
bool RoomStateManager::areGuestsAllowed(const std::string& roomId) const { return {}; }
std::string RoomStateManager::roomStateToJson(const RoomStateSummary& state) const { return {}; }
void RoomStateManager::clear() {}


// Free functions from content_utils
std::string buildMxcUri(const std::string& server, const std::string& mediaId) { return ""; }
std::string ensureCorrectFormattedBodyInTextReply(const std::string& a, const std::string& b, const std::string& c) { return a; }
std::string extractUsefulTextFromReply(const std::string& body) { return body; }
std::string formatSpoilerTextFromHtml(const std::string& html) { return html; }
std::string getEditedTargetEventId(const std::string& json) { return ""; }
std::string getExtensionFromMimeType(const std::string& mime) { return ""; }
std::string getLatestEditEventId(const std::string& json, const std::string& eventId) { return ""; }
bool hasTextWithImage(const std::string& json) { return false; }
std::string normalizeMimeType(const std::string& mime) { return mime; }

// Constructors for missing modules
RoomDirectoryManager::RoomDirectoryManager() {}
RoomStateManager::RoomStateManager() {}
SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) { return {}; }
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl, int width, int height, const std::string& method) { return ""; }
const char* visibilityToString(RoomDirectoryVisibility) { return ""; }
// SpaceGraph stubs
SpaceGraph::SpaceGraph() {}
void SpaceGraph::setRoot(const std::string& spaceId, const std::string& name, const std::string& topic, const std::string& avatarUrl) {}
void SpaceGraph::addChild(const std::string& parentId, const SpaceChildEntry& child) {}
void SpaceGraph::setNodeMetadata(const std::string& roomId, const std::string& name, const std::string& topic, const std::string& avatarUrl, const std::string& joinRule, bool isJoined) {}
void SpaceGraph::addParent(const std::string& roomId, const SpaceParentEntry& parent) {}
void SpaceGraph::setOrder(const std::string& parentId, const std::string& childId, const std::string& order) {}
SpaceGraphResult SpaceGraph::traverse(const SpaceTraversalOptions& options) const { return {}; }
int SpaceGraph::getDepth(const std::string& roomId) const { return {}; }
bool SpaceGraph::isInSpace(const std::string& spaceId, const std::string& roomId) const { return {}; }
int SpaceGraph::deepestDepth() const { return {}; }
std::string SpaceGraph::spaceToTreeJson(const std::string& spaceId, int maxDepth) const { return {}; }
std::string SpaceGraph::flatListToJson(const std::vector<SpaceNode>& nodes) const { return {}; }
std::string SpaceGraph::graphResultToJson(const SpaceGraphResult& result) const { return {}; }
void SpaceGraph::clear() {}
void SpaceGraph::traverseBFS(const SpaceTraversalOptions& options, SpaceGraphResult& result) const {}
void SpaceGraph::traverseDFS(const std::string& nodeId, int depth, const SpaceTraversalOptions& options, SpaceGraphResult& result, std::unordered_set<std::string>& visited) {}
std::string SpaceGraph::nodeToJson(const std::string& nodeId, int depthLeft, std::unordered_set<std::string>& visited) const { return {}; }
std::vector<SpaceNode> SpaceGraph::getChildren(const std::string& spaceId) const { return {}; }
std::vector<std::string> SpaceGraph::getParents(const std::string& roomId) const { return {}; }
std::vector<SpaceNode> SpaceGraph::searchSpaceRooms(const std::string& spaceId, const std::string& query) const { return {}; }

} 