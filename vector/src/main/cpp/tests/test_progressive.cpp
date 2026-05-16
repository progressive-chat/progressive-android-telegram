// Progressive Chat C++ unit tests — critical path verification
#include "test_framework.hpp"
#include "progressive/crypto_algorithms.hpp"
#include "progressive/timeline_chunk.hpp"
#include "progressive/markdown.hpp"
#include "progressive/sync_models.hpp"
#include "progressive/matrix_patterns.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/room_state.hpp"
#include "progressive/olm_session.hpp"
#include "progressive/event_relations.hpp"
#include "progressive/push_rules.hpp"
#include "progressive/content_scanner.hpp"
#include "progressive/password_validator.hpp"
#include "progressive/notif_format.hpp"
#include "progressive/identity_utils.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/user_status.hpp"
#include "progressive/event_classifier.hpp"
#include "progressive/poll_utils.hpp"
#include "progressive/room_counter.hpp"
#include "progressive/report_utils.hpp"
#include "progressive/permalink.hpp"
#include "progressive/well_known.hpp"
#include "progressive/olm_session.hpp"
#include "progressive/sas_verification.hpp"
#include <cstring>

// ==== SHA-256 verification (E2EE foundation) ====
static void test_sha256_known_vector() {
    // SHA-256("abc") = ba7816bf... (RFC 6234)
    std::string input = "abc";
    auto hash = progressive::sha256(
        reinterpret_cast<const uint8_t*>(input.data()), input.size());
    ASSERT_EQ(hash.size(), 32u);
    // First byte: 0xBA
    ASSERT_EQ(hash[0], 0xBAu);
    ASSERT_EQ(hash[1], 0x78u);
    ASSERT_EQ(hash[31], 0x83u);
}

static void test_sha256_empty() {
    auto hash = progressive::sha256((const uint8_t*)"", 0);
    ASSERT_EQ(hash.size(), 32u);
    // SHA-256("") starts with e3b0c442...
    ASSERT_EQ(hash[0], 0xE3u);
    ASSERT_EQ(hash[1], 0xB0u);
}

// ==== Display-index arithmetic (timeline order) ====
static void test_compute_display_indices_simple() {
    auto indices = progressive::TimelineChunkManager::computeDisplayIndices(0, 100, 5);
    ASSERT_EQ(indices.size(), 5u);
    // Should be evenly distributed between 0 and 100
    ASSERT_GT(indices[0], 0);
    ASSERT_LT(indices[4], 100);
    // Should be monotonic
    for (size_t i = 1; i < indices.size(); i++)
        ASSERT_TRUE(indices[i] > indices[i-1]);
}

static void test_compute_display_indices_small_gap() {
    auto indices = progressive::TimelineChunkManager::computeDisplayIndices(5, 10, 8);
    ASSERT_EQ(indices.size(), 8u);
    // Gap too small — sequential: 6,7,8,9,10,11,12,13
    ASSERT_EQ(indices[0], 6);
    ASSERT_EQ(indices[7], 13);
}

static void test_compute_display_indices_zero() {
    auto indices = progressive::TimelineChunkManager::computeDisplayIndices(0, 0, 0);
    ASSERT_EQ(indices.size(), 0u);
}

// ==== Markdown rendering (every message display) ====
static void test_markdown_bold() {
    auto result = progressive::markdownToHtml("**bold**");
    ASSERT_TRUE(result.find("<strong>") != std::string::npos || 
               result.find("<b>") != std::string::npos);
}

static void test_markdown_italic() {
    auto result = progressive::markdownToHtml("*italic*");
    ASSERT_TRUE(result.find("<em>") != std::string::npos || 
               result.find("<i>") != std::string::npos);
}

static void test_markdown_plain_passthrough() {
    auto result = progressive::markdownToHtml("hello world");
    ASSERT_TRUE(result.find("hello world") != std::string::npos);
}

static void test_markdown_html_passthrough() {
    auto result = progressive::markdownToHtml("<b>already bold</b>");
    ASSERT_TRUE(result.find("<b>") != std::string::npos || 
               result.find("already bold") != std::string::npos);
}

// ==== TimelineChunkManager basic operations ====
static void test_timeline_add_event() {
    progressive::TimelineChunkManager mgr("!test:matrix.org");
    progressive::TimelineEventData ev;
    ev.eventId = "$ev1"; ev.roomId = "!test:matrix.org";
    ev.type = "m.room.message"; ev.senderId = "@alice:matrix.org";
    ev.contentJson = "{\"body\":\"hello\"}";
    ev.originServerTs = 1000; ev.displayIndex = 0;
    
    int di = mgr.addLiveEvent(ev);
    ASSERT_EQ(di, 0);
    ASSERT_EQ(mgr.totalEventCount(), 1);
    
    auto* found = mgr.getEvent("$ev1");
    ASSERT_TRUE(found != nullptr);
    ASSERT_STREQ(found->eventId, "$ev1");
}

static void test_timeline_duplicate() {
    progressive::TimelineChunkManager mgr("!test:matrix.org");
    progressive::TimelineEventData ev;
    ev.eventId = "$dup"; ev.roomId = "!test:matrix.org";
    ev.type = "m.room.message"; ev.contentJson = "{}";
    
    mgr.addLiveEvent(ev);
    int di2 = mgr.addLiveEvent(ev); // duplicate
    ASSERT_EQ(di2, -1); // should be rejected
    ASSERT_EQ(mgr.totalEventCount(), 1);
}

static void test_timeline_get_snapshot() {
    progressive::TimelineChunkManager mgr("!test:matrix.org");
    for (int i = 0; i < 5; i++) {
        progressive::TimelineEventData ev;
        ev.eventId = "$ev" + std::to_string(i);
        ev.roomId = "!test:matrix.org";
        ev.type = "m.room.message";
        ev.contentJson = "{}";
        mgr.addLiveEvent(ev);
    }
    ASSERT_EQ(mgr.totalEventCount(), 5);
    
    auto snap = mgr.getSnapshot(3, 1);
    ASSERT_EQ(snap.size(), 3u);
}

// ==== Sync response parser (next_batch extraction) ====
static void test_sync_parse_next_batch() {
    std::string json = R"({"next_batch":"s12345_67890","rooms":{"join":{},"invite":{},"leave":{}}})";
    auto resp = progressive::parseSyncResponse(json);
    ASSERT_STREQ(resp.nextBatch, "s12345_67890");
}

static void test_sync_parse_empty() {
    auto resp = progressive::parseSyncResponse("{}");
    ASSERT_STREQ(resp.nextBatch, "");
    ASSERT_EQ(resp.rooms.join.size(), 0u);
}

// ==== Matrix ID validation ====
static void test_is_valid_user_id() {
    ASSERT_TRUE(progressive::isUserId("@alice:matrix.org"));
    ASSERT_FALSE(progressive::isUserId("alice"));
    ASSERT_FALSE(progressive::isUserId(""));
}

static void test_is_valid_room_id() {
    ASSERT_TRUE(progressive::isRoomId("!abc123:matrix.org"));
    ASSERT_FALSE(progressive::isRoomId("#alias:matrix.org"));
}

static void test_is_valid_room_alias() {
    ASSERT_TRUE(progressive::isRoomAlias("#room:matrix.org"));
    ASSERT_FALSE(progressive::isRoomAlias("!abc:matrix.org"));
}

static void test_is_valid_event_id() {
    ASSERT_TRUE(progressive::isEventId("$abcdefghijklmnopqrstuvwxyz1234567890"));
    ASSERT_FALSE(progressive::isEventId("short"));
}

// ==== MIME type normalization ====
static void test_normalize_mime_type() {
    ASSERT_STREQ(progressive::normalizeMimeType("image/jpg"), "image/jpeg");
    ASSERT_STREQ(progressive::normalizeMimeType("image/jpeg"), "image/jpeg");
    ASSERT_STREQ(progressive::normalizeMimeType("text/plain"), "text/plain");
}

// ==== Room state parsing ====
static void test_parse_join_rules() {
    std::string json = R"({"join_rule":"public"})";
    auto rules = progressive::parseJoinRules(json);
    auto ruleStr = progressive::joinRuleToString(rules.rule);
    ASSERT_STREQ(ruleStr, "public");
}

static void test_parse_history_visibility() {
    std::string json = R"({"history_visibility":"shared"})";
    auto vis = progressive::parseHistoryVisibility(json);
    auto visStr = progressive::historyVisibilityToString(vis.visibility);
    ASSERT_STREQ(visStr, "shared");
}

// ==== Base58 encode/decode (recovery keys) ====
static void test_base58_roundtrip() {
    std::vector<uint8_t> data = {0x00, 0x01, 0x02, 0xFF};
    auto encoded = progressive::base58Encode(data);
    auto decoded = progressive::base58Decode(encoded);
    ASSERT_EQ(decoded.size(), data.size());
    for (size_t i = 0; i < data.size(); i++)
        ASSERT_EQ(decoded[i], data[i]);
}

// ==== Event relations ====
static void test_is_reply() {
    ASSERT_TRUE(progressive::isReply(R"({"m.in_reply_to":{"event_id":"$abc"}})"));
    ASSERT_FALSE(progressive::isReply(R"({"body":"hello"})"));
}

static void test_is_edit() {
    ASSERT_TRUE(progressive::isEdit(R"({"m.relates_to":{"rel_type":"m.replace","event_id":"$x"}})"));
    ASSERT_FALSE(progressive::isEdit(R"({"body":"hello"})"));
}

static void test_is_reaction() {
    ASSERT_TRUE(progressive::isReaction(R"({"m.relates_to":{"rel_type":"m.annotation","key":"👍"}})"));
    ASSERT_FALSE(progressive::isReaction(R"({"body":"hello"})"));
}

static void test_extract_thread_root() {
    std::string json = R"({"m.relates_to":{"rel_type":"m.thread","event_id":"$root"}})";
    auto root = progressive::extractThreadRoot(json);
    ASSERT_STREQ(root, "$root");
}

// ==== Push rules ====
static void test_is_known_push_rule_kind() {
    ASSERT_TRUE(progressive::isKnownPushRuleKind("override"));
    ASSERT_TRUE(progressive::isKnownPushRuleKind("content"));
    ASSERT_FALSE(progressive::isKnownPushRuleKind("invalid_kind"));
}

// ==== Content scanner ====
static void test_is_server_notice() {
    ASSERT_TRUE(progressive::isServerNotice(R"({"server_notice_type":"m.server_notice"})"));
    ASSERT_FALSE(progressive::isServerNotice(R"({"body":"hello"})"));
}

static void test_must_accept_tos() {
    ASSERT_TRUE(progressive::mustAcceptTos(R"({"errcode":"M_CONSENT_NOT_GIVEN"})"));
    ASSERT_FALSE(progressive::mustAcceptTos(R"({"errcode":"M_FORBIDDEN"})"));
}

// ==== Password validation ====
static void test_password_meets_minimum() {
    ASSERT_TRUE(progressive::meetsMinimumRequirements("Abcdefg1"));   // 8+ chars, upper, lower, digit
    ASSERT_FALSE(progressive::meetsMinimumRequirements("short"));     // too short
    ASSERT_FALSE(progressive::meetsMinimumRequirements("abcdefgh"));  // no upper/digit
}

static void test_password_count_char_classes() {
    ASSERT_EQ(progressive::countCharClasses("a"), 1);        // lower only
    ASSERT_EQ(progressive::countCharClasses("aA"), 2);       // lower + upper
    ASSERT_EQ(progressive::countCharClasses("aA1"), 3);      // lower + upper + digit
    ASSERT_EQ(progressive::countCharClasses("aA1!"), 4);     // all four classes
}

static void test_password_strength_label() {
    ASSERT_STREQ(progressive::getStrengthLabel(85), "Strong");
    ASSERT_STREQ(progressive::getStrengthLabel(65), "Good");
    ASSERT_STREQ(progressive::getStrengthLabel(45), "Fair");
    ASSERT_STREQ(progressive::getStrengthLabel(25), "Weak");
}

// ==== Notification formatting ====
static void test_format_badge_text() {
    ASSERT_STREQ(progressive::formatBadgeText(0), "");
    ASSERT_STREQ(progressive::formatBadgeText(5), "5");
    ASSERT_STREQ(progressive::formatBadgeText(100), "99+");
}

static void test_total_unread_count() {
    ASSERT_EQ(progressive::getTotalUnreadCount(3, 2), 5);
    ASSERT_EQ(progressive::getTotalUnreadCount(0, 0), 0);
}

// ==== Identity validation ====
static void test_is_email() {
    ASSERT_TRUE(progressive::isEmail("alice@matrix.org"));
    ASSERT_FALSE(progressive::isEmail("@alice:matrix.org"));
    ASSERT_FALSE(progressive::isEmail("notanemail"));
}

static void test_is_msisdn() {
    ASSERT_TRUE(progressive::isMsisdn("+1234567890"));
    ASSERT_FALSE(progressive::isMsisdn("notaphone"));
}

static void test_extract_alias_localpart() {
    ASSERT_STREQ(progressive::extractAliasLocalpart("#room:matrix.org"), "room");
    ASSERT_STREQ(progressive::extractAliasLocalpart("#my-room:example.com"), "my-room");
}

// ==== Content utilities ====
static void test_is_mxc_uri() {
    ASSERT_TRUE(progressive::isMxcUri("mxc://matrix.org/abc123"));
    ASSERT_FALSE(progressive::isMxcUri("https://matrix.org/abc123"));
}

static void test_extract_mxc_server_name() {
    ASSERT_STREQ(progressive::extractMxcServerName("mxc://matrix.org/abc123"), "matrix.org");
}

static void test_extract_mxc_media_id() {
    ASSERT_STREQ(progressive::extractMxcMediaId("mxc://server/abc123"), "abc123");
}

static void test_has_text_with_image() {
    ASSERT_TRUE(progressive::hasTextWithImage(R"({"msgtype":"m.image","body":"alt text"})"));
    ASSERT_FALSE(progressive::hasTextWithImage(R"({"msgtype":"m.text","body":"hello"})"));
}

// ==== User status ====
static void test_get_status_suggestions() {
    auto suggestions = progressive::getStatusSuggestions();
    ASSERT_GT(suggestions.size(), 0u);
}

static void test_build_user_status_json() {
    auto json = progressive::buildUserStatusJson("busy", "💼", 1000000);
    ASSERT_TRUE(json.find("\"busy\"") != std::string::npos);
    ASSERT_TRUE(json.find("\"💼\"") != std::string::npos);
}

// ==== Poll utilities ====
static void test_is_valid_poll_question() {
    ASSERT_TRUE(progressive::isValidPollQuestion("What is your favorite color?"));
    ASSERT_FALSE(progressive::isValidPollQuestion(""));
}

static void test_is_poll_ended() {
    ASSERT_FALSE(progressive::isPollEnded(0));  // never ends
    ASSERT_TRUE(progressive::isPollEnded(1));    // already ended (1ms epoch)
}

// ==== Event classifier ====
static void test_classify_event_message() {
    auto type = progressive::classifyEvent("m.room.message", "m.text");
    auto desc = progressive::getEventTypeDescription(type);
    ASSERT_TRUE(!desc.empty());
}

// ==== Edit history ====
static void test_get_edit_badge_text() {
    ASSERT_STREQ(progressive::getEditBadgeText(0), "");
    ASSERT_TRUE(progressive::getEditBadgeText(3).find("3") != std::string::npos);
}

// ==== Room counter ====
static void test_room_count_zero() {
    std::vector<progressive::RoomCountEntry> rooms;
    auto result = progressive::countRooms(rooms, 0, false, false);
    ASSERT_EQ(result.totalRooms, 0);
}

// ==== Report utilities ====
static void test_is_valid_report_reason() {
    ASSERT_TRUE(progressive::isValidReportReason("m.spam"));
    ASSERT_FALSE(progressive::isValidReportReason("invalid_reason"));
}

// ==== Permalink builder ====
static void test_build_event_permalink() {
    auto link = progressive::buildEventPermalink("!room:matrix.org", "$event123");
    ASSERT_TRUE(link.find("!room:matrix.org") != std::string::npos || link.find("$event123") != std::string::npos);
}

static void test_build_room_permalink() {
    auto link = progressive::buildRoomPermalink("!room:matrix.org");
    ASSERT_TRUE(link.find("!room:matrix.org") != std::string::npos);
}

// ==== Well-known discovery ====
static void test_needs_well_known_discovery() {
    ASSERT_TRUE(progressive::needsWellKnownDiscovery("https://matrix.org"));
    ASSERT_FALSE(progressive::needsWellKnownDiscovery("https://matrix-client.matrix.org"));
}

// ==== Olm account (identity keys) ====
static void test_olm_account_create() {
    auto account = progressive::createOlmAccount("@alice:matrix.org", "DEVICE1");
    ASSERT_TRUE(account.valid);
    ASSERT_TRUE(!account.identityKeysJson.empty());
    progressive::destroyOlmAccount(account);
}

static void test_olm_account_pickle_roundtrip() {
    auto account = progressive::createOlmAccount("@bob:matrix.org", "DEV2");
    ASSERT_TRUE(account.valid);
    auto pickled = progressive::pickleOlmAccount(account);
    ASSERT_TRUE(!pickled.empty());
    auto restored = progressive::unpickleOlmAccount(pickled, "@bob:matrix.org", "DEV2");
    ASSERT_TRUE(restored.valid);
    progressive::destroyOlmAccount(account);
    progressive::destroyOlmAccount(restored);
}

// ==== SAS verification ====
static void test_sas_create() {
    auto sas = progressive::sasCreate();
    ASSERT_TRUE(sas.valid);
    ASSERT_TRUE(!sas.ourPubkey.empty());
    progressive::sasDestroy(sas);
}

static void test_sas_emoji_table() {
    for (int i = 0; i < 64; i++) {
        auto emoji = progressive::sasEmojiForIndex(i);
        ASSERT_TRUE(emoji.emoji != nullptr);
        ASSERT_TRUE(emoji.description != nullptr);
    }
}

// ==== Device fingerprint ====
static void test_compute_device_fingerprint() {
    // Base64 of 32 zero bytes = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
    auto fp = progressive::computeDeviceFingerprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=");
    ASSERT_TRUE(!fp.empty());
    // Should have 8 words separated by spaces
    int spaces = 0;
    for (char c : fp) if (c == ' ') spaces++;
    ASSERT_TRUE(spaces >= 6); // 7+ spaces for 8 words
}

// ==== Run all tests ====
int main() {
    printf("=== Progressive Chat C++ Unit Tests ===\n");
    TEST_RUNNER(runner);
    
    printf("\n-- SHA-256 --\n");
    ADD_TEST(runner, test_sha256_known_vector);
    ADD_TEST(runner, test_sha256_empty);
    
    printf("\n-- Display Index --\n");
    ADD_TEST(runner, test_compute_display_indices_simple);
    ADD_TEST(runner, test_compute_display_indices_small_gap);
    ADD_TEST(runner, test_compute_display_indices_zero);
    
    printf("\n-- Markdown --\n");
    ADD_TEST(runner, test_markdown_bold);
    ADD_TEST(runner, test_markdown_italic);
    ADD_TEST(runner, test_markdown_plain_passthrough);
    ADD_TEST(runner, test_markdown_html_passthrough);
    
    printf("\n-- Timeline --\n");
    ADD_TEST(runner, test_timeline_add_event);
    ADD_TEST(runner, test_timeline_duplicate);
    ADD_TEST(runner, test_timeline_get_snapshot);
    
    printf("\n-- Sync Parser --\n");
    ADD_TEST(runner, test_sync_parse_next_batch);
    ADD_TEST(runner, test_sync_parse_empty);
    
    printf("\n-- Matrix IDs --\n");
    ADD_TEST(runner, test_is_valid_user_id);
    ADD_TEST(runner, test_is_valid_room_id);
    ADD_TEST(runner, test_is_valid_room_alias);
    ADD_TEST(runner, test_is_valid_event_id);
    
    printf("\n-- MIME & Room State --\n");
    ADD_TEST(runner, test_normalize_mime_type);
    ADD_TEST(runner, test_parse_join_rules);
    ADD_TEST(runner, test_parse_history_visibility);
    
    printf("\n-- Crypto --\n");
    ADD_TEST(runner, test_base58_roundtrip);
    
    printf("\n-- Event Relations --\n");
    ADD_TEST(runner, test_is_reply);
    ADD_TEST(runner, test_is_edit);
    ADD_TEST(runner, test_is_reaction);
    ADD_TEST(runner, test_extract_thread_root);
    
    printf("\n-- Push & Content --\n");
    ADD_TEST(runner, test_is_known_push_rule_kind);
    ADD_TEST(runner, test_is_server_notice);
    ADD_TEST(runner, test_must_accept_tos);
    
    printf("\n-- Password & Notifications --\n");
    ADD_TEST(runner, test_password_meets_minimum);
    ADD_TEST(runner, test_password_count_char_classes);
    ADD_TEST(runner, test_password_strength_label);
    ADD_TEST(runner, test_format_badge_text);
    ADD_TEST(runner, test_total_unread_count);
    
    printf("\n-- Identity & Content --\n");
    ADD_TEST(runner, test_is_email);
    ADD_TEST(runner, test_is_msisdn);
    ADD_TEST(runner, test_extract_alias_localpart);
    ADD_TEST(runner, test_is_mxc_uri);
    ADD_TEST(runner, test_extract_mxc_server_name);
    ADD_TEST(runner, test_extract_mxc_media_id);
    ADD_TEST(runner, test_has_text_with_image);
    
    printf("\n-- User Status --\n");
    ADD_TEST(runner, test_get_status_suggestions);
    ADD_TEST(runner, test_build_user_status_json);
    
    printf("\n-- Misc --\n");
    ADD_TEST(runner, test_is_valid_poll_question);
    ADD_TEST(runner, test_is_poll_ended);
    ADD_TEST(runner, test_classify_event_message);
    ADD_TEST(runner, test_get_edit_badge_text);
    ADD_TEST(runner, test_room_count_zero);
    
    printf("\n-- Report & Permalink --\n");
    ADD_TEST(runner, test_is_valid_report_reason);
    ADD_TEST(runner, test_build_event_permalink);
    ADD_TEST(runner, test_build_room_permalink);
    ADD_TEST(runner, test_needs_well_known_discovery);
    
    printf("\n-- Olm & SAS --\n");
    ADD_TEST(runner, test_olm_account_create);
    ADD_TEST(runner, test_olm_account_pickle_roundtrip);
    ADD_TEST(runner, test_sas_create);
    ADD_TEST(runner, test_sas_emoji_table);
    ADD_TEST(runner, test_compute_device_fingerprint);
    
    return runner.summary();
}
