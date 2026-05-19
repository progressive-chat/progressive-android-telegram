#include "progressive/content_utils.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"

namespace progressive {

std::string CrossSigningManager::buildCrossSigningInfo(std::string const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&) const { return {}; }
std::string CrossSigningManager::buildMasterKey(std::string const&, std::string const&) const { return {}; }
std::string CrossSigningManager::buildSelfSigningKey(std::string const&, std::string const&) const { return {}; }
std::string CrossSigningManager::buildUserSigningKey(std::string const&, std::string const&) const { return {}; }
bool CrossSigningManager::canCrossSign() const { return {}; }
void CrossSigningManager::checkSelfTrust() const {}
void CrossSigningManager::crossSigningInfoToJson(progressive::CSM_CrossSigningInfo const&) const const {}
CrossSigningManager::CrossSigningManager() {}
void CrossSigningManager::importPrivateKeys(std::string const&, std::string const&, std::string const&) const {}
bool CrossSigningManager::isInitialized() const { return {}; }
void CrossSigningManager::markMyMasterKeyAsTrusted() {}
void CrossSigningManager::trustResultToJson(progressive::UserTrustResult const&) const const {}
std::string DeviceManager::buildDeleteRequest(progressive::DeviceDeletionRequest const&) const const { return {}; }
std::string DeviceManager::buildRenameRequest(progressive::DeviceRenameRequest const&) const const { return {}; }
void DeviceManager::cryptoDeviceToJson(progressive::CryptoDeviceInfo const&) const const {}
DeviceManager::DeviceManager() {}
void DeviceManager::devicesToJson(std::__ndk1::vector<progressive::DeviceInfo, std::__ndk1::allocator<progressive::DeviceInfo> > const&) const const {}
void DeviceManager::deviceToJson(progressive::DeviceInfo const&) const const {}
std::string DeviceManager::formatFingerprint(std::string const&) const const { return {}; }
std::string DeviceManager::formatLastSeen(long long) const { return {}; }
std::string DeviceManager::getTrustLabel(progressive::DeviceTrustLevel const&) const const { return {}; }
bool DeviceManager::isDeviceInactive(long long, int) const { return {}; }
std::string DeviceManager::parseCryptoDeviceInfo(std::string const&, std::string const&, std::string const&) const { return {}; }
std::string DeviceManager::parseDeviceInfo(std::string const&, std::string const&) const { return {}; }
std::string DeviceManager::parseDevicesList(std::string const&) const { return {}; }
void DeviceManager::satisfiesMinVersion(std::string const&, std::string const&) const const {}
std::string PollManager::buildPollEndContent(std::string const&, std::string const&, bool) const { return {}; }
std::string PollManager::formatPollEvent(progressive::PollResultFull const&) const { return {}; }
std::string PollManager::getWinnerText(progressive::PollResultFull const&) const const { return {}; }
bool PollManager::isValidPollQuestion(std::string const&) const { return {}; }
std::string PollManager::parsePollStartContent(std::string const&, bool) const { return {}; }
PollManager::PollManager() {}
void PollManager::tallyVotes(progressive::PollContent const&, std::__ndk1::vector<progressive::PollVote, std::__ndk1::allocator<progressive::PollVote> > const&) const {}
void RoomDirectoryManager::aliasResultToJson(progressive::AliasAvailabilityResult const&) const const {}
std::string RoomDirectoryManager::buildPublicRoomsRequest(progressive::PublicRoomsParams const&) const const { return {}; }
std::string RoomDirectoryManager::buildVisibilityRequest(progressive::RoomDirectoryVisibility) const { return {}; }
std::string RoomDirectoryManager::formatRoomPreview(progressive::PublicRoom const&) const const { return {}; }
std::string RoomDirectoryManager::parseAliasAvailability(std::string const&, std::string const&) const const { return {}; }
std::string RoomDirectoryManager::parsePublicRoomsResponse(std::string const&) const const { return {}; }
std::string RoomDirectoryManager::parseVisibilityResponse(std::string const&) const const { return {}; }
void RoomDirectoryManager::responseToJson(progressive::PublicRoomsResponse const&) const const {}
RoomDirectoryManager::RoomDirectoryManager() {}
bool RoomStateManager::isInviteOnly(std::string const&) const const { return {}; }
bool RoomStateManager::isPublicRoom(std::string const&) const const { return {}; }
RoomStateManager::RoomStateManager() {}
void RoomStateManager::setHistoryVisibility(std::string const&, progressive::RSM_RoomHistoryVisibility) const {}
void RoomStateManager::setJoinRule(std::string const&, progressive::RoomJoinRule) const {}
std::string ServerNoticeManager::formatDowntime(long long) { return {}; }
std::string ServerNoticeManager::formatResourceLimitError(progressive::ServerNoticeInfo const&, progressive::ResourceLimitMode) const { return {}; }
std::string ServerNoticeManager::getBannerColor(progressive::ServerNoticeInfo const&) const { return {}; }
std::string ServerNoticeManager::getErrorCodeDescription(std::string const&) const { return {}; }
bool ServerNoticeManager::isConsentError(std::string const&) const { return {}; }
bool ServerNoticeManager::isRateLimitError(std::string const&) const { return {}; }
bool ServerNoticeManager::isResourceLimitError(std::string const&) const { return {}; }
std::string ServerNoticeManager::parseMatrixError(std::string const&) const { return {}; }
ServerNoticeManager::ServerNoticeManager() {}
void ServerNoticeManager::serverNoticeToJson(progressive::ServerNoticeInfo const&) const {}
void SpaceGraph::addChild(std::string const&, progressive::SpaceChildEntry const&) const {}
void SpaceGraph::flatListToJson(std::__ndk1::vector<progressive::SpaceNode, std::__ndk1::allocator<progressive::SpaceNode> > const&) const const {}
std::string SpaceGraph::getChildren(std::string const&) const const { return {}; }
std::string SpaceGraph::getDepth(std::string const&) const const { return {}; }
std::string SpaceGraph::getParents(std::string const&) const const { return {}; }
void SpaceGraph::graphResultToJson(progressive::SpaceGraphResult const&) const const {}
bool SpaceGraph::isInSpace(std::string const&, std::string const&) const const { return {}; }
void SpaceGraph::searchSpaceRooms(std::string const&, std::string const&) const const {}
void SpaceGraph::setNodeMetadata(std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, bool) const {}
void SpaceGraph::setRoot(std::string const&, std::string const&, std::string const&, std::string const&) const {}
SpaceGraph::SpaceGraph() {}
void SpaceGraph::spaceToTreeJson(std::string const&, int) const const {}
void SpaceGraph::traverse(progressive::SpaceTraversalOptions const&) const const {}

} // namespace progressive