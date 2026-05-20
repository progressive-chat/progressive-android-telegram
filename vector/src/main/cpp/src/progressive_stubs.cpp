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
    DeviceManager::DeviceManager() {}
    DevicesListResponse DeviceManager::parseDevicesList(const std::string& json) { return {}; }
    DeviceInfo DeviceManager::parseDeviceInfo(const std::string& deviceId, const std::string& json) { return {}; }
    CryptoDeviceInfo DeviceManager::parseCryptoDeviceInfo(const std::string& deviceId, const std::string& userId, const std::string& json) { return {}; }
    std::string DeviceManager::buildRenameRequest(const DeviceRenameRequest& req) const { return {}; }
    std::string DeviceManager::buildDeleteRequest(const DeviceDeletionRequest& req) const { return {}; }
    std::string DeviceManager::buildBatchDeleteRequest(const std::vector<DeviceDeletionRequest>& requests) const { return {}; }
    bool DeviceManager::requiresUia(const std::string& deleteResponseJson) const { return {}; }
    std::string DeviceManager::formatTrustLevel(const DeviceTrustLevel& level) const { return {}; }
    std::string DeviceManager::getTrustLabel(const DeviceTrustLevel& level) const { return {}; }
    std::string DeviceManager::formatFingerprint(const std::string& rawKey) const { return {}; }
    std::string DeviceManager::formatShortKey(const std::string& rawKey) const { return {}; }
    bool DeviceManager::isDeviceInactive(int64_t lastSeenTs, int inactivityDays) const { return {}; }
    std::string DeviceManager::formatLastSeen(int64_t lastSeenTs) const { return {}; }
    bool DeviceManager::satisfiesMinVersion(const std::string& clientVersion, const std::string& minRequired) const { return {}; }
    void DeviceManager::sortDevices(std::vector<DeviceInfo>& devices, DeviceSortMode mode) const {}
    void DeviceManager::sortCryptoDevices(std::vector<CryptoDeviceInfo>& devices, DeviceSortMode mode) const {}
    std::string DeviceManager::deviceToJson(const DeviceInfo& device) const { return {}; }
    std::string DeviceManager::cryptoDeviceToJson(const CryptoDeviceInfo& device) const { return {}; }
    std::string DeviceManager::devicesToJson(const std::vector<DeviceInfo>& devices) const { return {}; }
    std::string DeviceManager::cryptoDevicesToJson(const std::vector<CryptoDeviceInfo>& devices) const { return {}; }
    std::string DeviceManager::trustLevelToJson(const DeviceTrustLevel& level) const { return {}; }
    std::string DeviceManager::extractStr(const std::string& json, const std::string& key) { return {}; }
    int64_t DeviceManager::extractInt(const std::string& json, const std::string& key) { return {}; }
    bool DeviceManager::extractBool(const std::string& json, const std::string& key) { return {}; }



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
SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) { return {}; }
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl, int width, int height, const std::string& method) { return ""; }
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