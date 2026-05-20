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
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl, int width, int height, const std::string& method) { return ""; }
// SpaceGraph stubs

} 