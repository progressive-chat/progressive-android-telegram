// Auto-generated stubs from linker errors
#include "progressive/composer_manager.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/oidc_manager.hpp"
#include "progressive/media_viewer.hpp"
namespace progressive {

// SKIP: autoReplaceEmojis(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: buildMxcUri(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: buildQuotedBody(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
std::string ComposerManager::applyBold(const std::string&, int, int) { return {}; }
std::string ComposerManager::applyItalic(const std::string&, int, int) { return {}; }
ComposerManager::ComposerManager() {}
void ComposerManager::enterEditMode(const std::string&) {}
void ComposerManager::enterQuoteMode(const std::string&) {}
void ComposerManager::enterRegularMode(bool) {}
void ComposerManager::enterReplyMode(const std::string&) {}
void ComposerManager::setText(const std::string&) {}
std::string ComposerManager::stateToJson() const { return {}; }
std::string CrossSigningManager::buildCrossSigningInfo(const std::string&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&) { return {}; }
std::string CrossSigningManager::buildMasterKey(const std::string&, const std::string&) { return {}; }
std::string CrossSigningManager::buildSelfSigningKey(const std::string&, const std::string&) { return {}; }
std::string CrossSigningManager::buildUserSigningKey(const std::string&, const std::string&) { return {}; }
bool CrossSigningManager::canCrossSign() const { return {}; }
bool CrossSigningManager::checkSelfTrust() const { return {}; }
std::string CrossSigningManager::crossSigningInfoToJson(progressive::CSM_CrossSigningInfo const&) const { return {}; }
CrossSigningManager::CrossSigningManager() {}
void CrossSigningManager::importPrivateKeys(const std::string&, const std::string&, const std::string&) {}
bool CrossSigningManager::isInitialized() const { return {}; }
void CrossSigningManager::markMyMasterKeyAsTrusted() {}
std::string CrossSigningManager::trustResultToJson(progressive::UserTrustResult const&) const { return {}; }
std::string DeviceManager::buildDeleteRequest(progressive::DeviceDeletionRequest const&) const { return {}; }
std::string DeviceManager::buildRenameRequest(progressive::DeviceRenameRequest const&) const { return {}; }
std::string DeviceManager::cryptoDeviceToJson(progressive::CryptoDeviceInfo const&) const { return {}; }
DeviceManager::DeviceManager() {}
std::string DeviceManager::devicesToJson(std::__ndk1::vector<progressive::DeviceInfo, std::__ndk1::allocator<progressive::DeviceInfo> > const&) const { return {}; }
std::string DeviceManager::deviceToJson(progressive::DeviceInfo const&) const { return {}; }
void DeviceManager::formatFingerprint(const std::string&) const {}
void DeviceManager::formatLastSeen(long long) const {}
void DeviceManager::getTrustLabel(progressive::DeviceTrustLevel const&) const {}
void DeviceManager::isDeviceInactive(long long, int) const {}
void DeviceManager::parseCryptoDeviceInfo(const std::string&, const std::string&, const std::string&) {}
void DeviceManager::parseDeviceInfo(const std::string&, const std::string&) {}
void DeviceManager::parseDevicesList(const std::string&) {}
void DeviceManager::satisfiesMinVersion(const std::string&, const std::string&) const {}
// SKIP: ensureCorrectFormattedBodyInTextReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: extractMentionQuery(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int)
// SKIP: extractUsefulTextFromReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: formatSpoilerTextFromHtml(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getEditedTargetEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getExtensionFromMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: getLatestEditEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: hasTextWithImage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: normalizeMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
// SKIP: parseSpaceChild(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
void PollManager::buildPollEndContent(const std::string&, const std::string&, bool) {}
void PollManager::buildPollResponseContent(const std::string&, std::__ndk1::vector<std::string, std::__ndk1::allocator<std::string > > const&, bool) {}
void PollManager::buildPollStartContent(const std::string&, std::__ndk1::vector<std::string, std::__ndk1::allocator<std::string > > const&, progressive::PollKind, int, bool, std::string&) {}
void PollManager::formatPollEvent(progressive::PollResultFull const&) {}
void PollManager::getWinnerText(progressive::PollResultFull const&) const {}
void PollManager::isValidPollQuestion(const std::string&) {}
void PollManager::parsePollStartContent(const std::string&, bool) {}
PollManager::PollManager() {}
void PollManager::tallyVotes(progressive::PollContent const&, std::__ndk1::vector<progressive::PollVote, std::__ndk1::allocator<progressive::PollVote> > const&) {}
// SKIP: resolveMxcThumbnailUrl(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int, int, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&)
void RoomDirectoryManager::aliasResultToJson(progressive::AliasAvailabilityResult const&) const {}
void RoomDirectoryManager::buildPublicRoomsRequest(progressive::PublicRoomsParams const&) const {}
void RoomDirectoryManager::buildVisibilityRequest(progressive::RoomDirectoryVisibility) const {}
void RoomDirectoryManager::formatRoomPreview(progressive::PublicRoom const&) const {}
void RoomDirectoryManager::parseAliasAvailability(const std::string&, const std::string&) const {}
void RoomDirectoryManager::parsePublicRoomsResponse(const std::string&) const {}
void RoomDirectoryManager::parseVisibilityResponse(const std::string&) const {}
std::string RoomDirectoryManager::responseToJson(progressive::PublicRoomsResponse const&) const { return {}; }
RoomDirectoryManager::RoomDirectoryManager() {}
bool RoomStateManager::isInviteOnly(const std::string&) const { return {}; }
bool RoomStateManager::isPublicRoom(const std::string&) const { return {}; }
RoomStateManager::RoomStateManager() {}
void RoomStateManager::setHistoryVisibility(const std::string&, progressive::RSM_RoomHistoryVisibility) {}
void RoomStateManager::setJoinRule(const std::string&, progressive::RoomJoinRule) {}
std::string ServerNoticeManager::formatDowntime(long long) { return {}; }
void ServerNoticeManager::formatResourceLimitError(progressive::ServerNoticeInfo const&, progressive::ResourceLimitMode) {}
std::string ServerNoticeManager::getBannerColor(progressive::ServerNoticeInfo const&) { return {}; }
void ServerNoticeManager::getErrorCodeDescription(const std::string&) {}
bool ServerNoticeManager::isConsentError(const std::string&) { return {}; }
bool ServerNoticeManager::isRateLimitError(const std::string&) { return {}; }
bool ServerNoticeManager::isResourceLimitError(const std::string&) { return {}; }
void ServerNoticeManager::parseMatrixError(const std::string&) {}
ServerNoticeManager::ServerNoticeManager() {}
void ServerNoticeManager::serverNoticeToJson(progressive::ServerNoticeInfo const&) {}
void SpaceGraph::addChild(const std::string&, progressive::SpaceChildEntry const&) {}
void SpaceGraph::flatListToJson(std::__ndk1::vector<progressive::SpaceNode, std::__ndk1::allocator<progressive::SpaceNode> > const&) const {}
void SpaceGraph::getChildren(const std::string&) const {}
void SpaceGraph::getDepth(const std::string&) const {}
void SpaceGraph::getParents(const std::string&) const {}
void SpaceGraph::graphResultToJson(progressive::SpaceGraphResult const&) const {}
void SpaceGraph::isInSpace(const std::string&, const std::string&) const {}
void SpaceGraph::searchSpaceRooms(const std::string&, const std::string&) const {}
void SpaceGraph::setNodeMetadata(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, bool) {}
void SpaceGraph::setRoot(const std::string&, const std::string&, const std::string&, const std::string&) {}
SpaceGraph::SpaceGraph() {}
void SpaceGraph::spaceToTreeJson(const std::string&, int) const {}
void SpaceGraph::traverse(progressive::SpaceTraversalOptions const&) const {}
// SKIP: validateMessage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int)
// SKIP: visibilityToString(progressive::RoomDirectoryVisibility)

std::string autoReplaceEmojis(const std::string&) { return {}; }
std::string buildMxcUri(const std::string&, const std::string&) { return {}; }
std::string buildQuotedBody(const std::string&, const std::string&, const std::string&) { return {}; }
void ensureCorrectFormattedBodyInTextReply(const std::string&, const std::string&, const std::string&) {}
void extractMentionQuery(const std::string&, int) {}
void extractUsefulTextFromReply(const std::string&) {}
std::string formatSpoilerTextFromHtml(const std::string&) { return {}; }
void getEditedTargetEventId(const std::string&) {}
void getExtensionFromMimeType(const std::string&) {}
void getLatestEditEventId(const std::string&, const std::string&) {}
void hasTextWithImage(const std::string&) {}
void normalizeMimeType(const std::string&) {}
std::string parseSpaceChild(const std::string&, const std::string&) { return {}; }
void resolveMxcThumbnailUrl(const std::string&, const std::string&, int, int, const std::string&) {}
void validateMessage(const std::string&, int) {}
bool visibilityToString(progressive::RoomDirectoryVisibility) { return {}; }
} // namespace progressive