#include "progressive/composer_manager.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/content_utils.hpp"
namespace progressive {
// autoReplaceEmojis(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// buildMxcUri(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// buildQuotedBody(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
std::string ComposerManager::applyBold(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int, int) { return {}; }
std::string ComposerManager::applyItalic(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int, int) { return {}; }
ComposerManager::~ComposerManager() {}
ComposerManager::~ComposerManager() {}
ComposerManager::~ComposerManager() {}
ComposerManager::~ComposerManager() {}
ComposerManager::~ComposerManager() {}
ComposerManager::~ComposerManager() {}
std::string ComposerManager::stateToJson() const { return {}; }
std::string CrossSigningManager::buildCrossSigningInfo(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&, progressive::CSM_CrossSigningKey const&) { return {}; }
std::string CrossSigningManager::buildMasterKey(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
std::string CrossSigningManager::buildSelfSigningKey(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
std::string CrossSigningManager::buildUserSigningKey(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
bool CrossSigningManager::canCrossSign() const { return {}; }
bool CrossSigningManager::checkSelfTrust() const { return {}; }
std::string CrossSigningManager::crossSigningInfoToJson(progressive::CSM_CrossSigningInfo const&) const { return {}; }
CrossSigningManager::~CrossSigningManager() {}
CrossSigningManager::~CrossSigningManager() {}
bool CrossSigningManager::isInitialized() const { return {}; }
bool CrossSigningManager::markMyMasterKeyAsTrusted() { return {}; }
std::string CrossSigningManager::trustResultToJson(progressive::UserTrustResult const&) const { return {}; }
std::string DeviceManager::buildDeleteRequest(progressive::DeviceDeletionRequest const&) const { return {}; }
std::string DeviceManager::buildRenameRequest(progressive::DeviceRenameRequest const&) const { return {}; }
std::string DeviceManager::cryptoDeviceToJson(progressive::CryptoDeviceInfo const&) const { return {}; }
DeviceManager::~DeviceManager() {}
std::string DeviceManager::devicesToJson(std::__ndk1::vector<progressive::DeviceInfo, std::__ndk1::allocator<progressive::DeviceInfo> > const&) const { return {}; }
std::string DeviceManager::deviceToJson(progressive::DeviceInfo const&) const { return {}; }
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
DeviceManager::~DeviceManager() {}
// ensureCorrectFormattedBodyInTextReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// extractMentionQuery(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int) — skipped (no args)
// extractUsefulTextFromReply(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// formatSpoilerTextFromHtml(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// getEditedTargetEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// getExtensionFromMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// getLatestEditEventId(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// hasTextWithImage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// normalizeMimeType(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
// parseSpaceChild(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
PollManager::~PollManager() {}
// resolveMxcThumbnailUrl(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int, int, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) — skipped (no args)
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
std::string RoomDirectoryManager::responseToJson(progressive::PublicRoomsResponse const&) const { return {}; }
RoomDirectoryManager::~RoomDirectoryManager() {}
bool RoomStateManager::isInviteOnly(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
bool RoomStateManager::isPublicRoom(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
RoomStateManager::~RoomStateManager() {}
RoomStateManager::~RoomStateManager() {}
RoomStateManager::~RoomStateManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
bool ServerNoticeManager::isConsentError(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
bool ServerNoticeManager::isRateLimitError(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
bool ServerNoticeManager::isResourceLimitError(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
ServerNoticeManager::~ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
void SpaceGraph::addChild(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, progressive::SpaceChildEntry const&) { return {}; }
void SpaceGraph::flatListToJson(std::__ndk1::vector<progressive::SpaceNode, std::__ndk1::allocator<progressive::SpaceNode> > const&) const { return {}; }
void SpaceGraph::getChildren(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
void SpaceGraph::getDepth(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
void SpaceGraph::getParents(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
void SpaceGraph::graphResultToJson(progressive::SpaceGraphResult const&) const { return {}; }
void SpaceGraph::isInSpace(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
void SpaceGraph::searchSpaceRooms(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) const { return {}; }
void SpaceGraph::setNodeMetadata(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, bool) { return {}; }
void SpaceGraph::setRoot(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&) { return {}; }
void SpaceGraph::SpaceGraph() { return {}; }
void SpaceGraph::spaceToTreeJson(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int) const { return {}; }
void SpaceGraph::traverse(progressive::SpaceTraversalOptions const&) const { return {}; }
// validateMessage(std::__ndk1::basic_string<char, std::__ndk1::char_traits<char>, std::__ndk1::allocator<char> > const&, int) — skipped (no args)
// visibilityToString(progressive::RoomDirectoryVisibility) — skipped (no args)
} // namespace progressive