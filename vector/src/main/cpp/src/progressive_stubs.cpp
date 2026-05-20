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
DeviceManager::DeviceManager() {}
SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) { return {}; }
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl, int width, int height, const std::string& method) { return ""; }
const char* visibilityToString(RoomDirectoryVisibility) { return ""; }
} // namespace progressive