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
#include "progressive/membership_utils.hpp"
#include "progressive/url_tools.hpp"
#include "progressive/sso_utils.hpp"
#include "progressive/connection_monitor.hpp"
#include "progressive/date_utils.hpp"
#include "progressive/matrix_error.hpp"
#include "progressive/content_guard.hpp"
#include "progressive/invite_utils.hpp"
#include "progressive/file_validator.hpp"
#include "progressive/event_utils.hpp"
#include "progressive/megolm_decryptor.hpp"
#include "progressive/canonical_json.hpp"
#include "progressive/event_encryption.hpp"
#include "progressive/chunked_upload.hpp"
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
    auto fp = progressive::computeDeviceFingerprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=");
    ASSERT_TRUE(!fp.empty());
    int spaces = 0;
    for (char c : fp) if (c == ' ') spaces++;
    ASSERT_TRUE(spaces >= 6);
}

// ==== Membership formatting ====
static void test_format_membership_join() {
    auto result = progressive::formatMembership(progressive::Membership::Join);
    ASSERT_TRUE(!result.empty());
}

static void test_is_active_member() {
    ASSERT_TRUE(progressive::isActiveMember(progressive::Membership::Join));
    ASSERT_FALSE(progressive::isActiveMember(progressive::Membership::Leave));
    ASSERT_FALSE(progressive::isActiveMember(progressive::Membership::Ban));
}

// ==== URL parsing ====
static void test_is_matrix_to_permalink() {
    ASSERT_TRUE(progressive::isMatrixToPermalink("https://matrix.to/#/!room:matrix.org"));
    ASSERT_FALSE(progressive::isMatrixToPermalink("https://google.com"));
}

static void test_extract_room_id_from_permalink() {
    auto roomId = progressive::extractRoomIdFromPermalink("https://matrix.to/#/!abc:matrix.org");
    ASSERT_TRUE(roomId.find("!abc") != std::string::npos);
}

// ==== SSO callback detection ====
static void test_is_sso_callback_url() {
    ASSERT_TRUE(progressive::isSsoCallbackUrl("https://app.element.io/?loginToken=abc123"));
    ASSERT_FALSE(progressive::isSsoCallbackUrl("https://app.element.io/welcome"));
}

// ==== Connection monitor ====
static void test_format_downtime() {
    auto result = progressive::ConnectionMonitor::formatDowntime(1000);
    ASSERT_TRUE(!result.empty());
    auto result2 = progressive::ConnectionMonitor::formatDowntime(3600000);
    ASSERT_TRUE(!result2.empty());
}

static void test_get_banner_color() {
    auto green = progressive::ConnectionMonitor::getBannerColor(0);
    ASSERT_TRUE(!green.empty());
    auto red = progressive::ConnectionMonitor::getBannerColor(600000);
    ASSERT_TRUE(!red.empty());
}

// ==== Date/time formatting ====
static void test_format_duration_seconds() {
    auto result = progressive::formatDuration(5000);
    ASSERT_TRUE(result.find("5") != std::string::npos);
}

static void test_format_duration_minutes() {
    auto result = progressive::formatDuration(120000);
    ASSERT_TRUE(result.find("2") != std::string::npos);
}

// ==== Matrix errors ====
static void test_is_password_error() {
    ASSERT_TRUE(progressive::isPasswordError("M_WEAK_PASSWORD"));
    ASSERT_FALSE(progressive::isPasswordError("M_FORBIDDEN"));
}

static void test_get_all_error_codes() {
    auto codes = progressive::getAllErrorCodes();
    ASSERT_GT(codes.size(), 5u);
    ASSERT_TRUE(std::find(codes.begin(), codes.end(), "M_UNKNOWN") != codes.end());
}

// ==== Content guard ====
static void test_count_emojis() {
    ASSERT_GT(progressive::countEmojis("hello 😀 world"), 0);
    ASSERT_EQ(progressive::countEmojis("no emoji"), 0);
}

static void test_format_media_collapse_label() {
    auto result = progressive::formatMediaCollapseLabel(5);
    ASSERT_TRUE(result.find("5") != std::string::npos);
}

// ==== Invite utilities ====
static void test_is_invite_expired() {
    ASSERT_TRUE(progressive::isInviteExpired(0, 30)); // invited at epoch, definitely expired
}

static void test_build_knock_body() {
    auto result = progressive::buildKnockBody("let me in");
    ASSERT_TRUE(result.find("let me in") != std::string::npos);
}

// ==== File validator ====
static void test_format_file_size() {
    auto result = progressive::formatFileSize(1024);
    ASSERT_TRUE(result.find("KB") != std::string::npos);
}

static void test_format_file_size_bytes() {
    auto result = progressive::formatFileSize(500);
    ASSERT_TRUE(result.find("B") != std::string::npos);
}

// ==== Member notice formatter ====
static void test_member_notice_join() {
    auto result = progressive::formatMemberNotice("join", "", "@alice:matrix.org", "Alice", "@alice:matrix.org", "Alice", "", false, false);
    ASSERT_TRUE(result.find("Alice") != std::string::npos);
    ASSERT_TRUE(result.find("joined") != std::string::npos);
}

static void test_member_notice_invite() {
    auto result = progressive::formatMemberNotice("invite", "", "@bob:matrix.org", "Bob", "@carol:matrix.org", "Carol", "welcome!", false, false);
    ASSERT_TRUE(result.find("Bob") != std::string::npos);
    ASSERT_TRUE(result.find("invited") != std::string::npos);
    ASSERT_TRUE(result.find("Carol") != std::string::npos);
}

static void test_call_notice_invite() {
    auto result = progressive::formatCallNotice("m.call.invite", true, "Alice", false);
    ASSERT_TRUE(result.find("Alice") != std::string::npos);
    ASSERT_TRUE(result.find("video call") != std::string::npos);
}

static void test_call_notice_reject() {
    auto result = progressive::formatCallNotice("m.call.reject", false, "Alice", false);
    ASSERT_TRUE(result.find("declined") != std::string::npos);
}

static void test_annotate_edited() {
    ASSERT_TRUE(progressive::annotateEdited("hello", true).find("(edited)") != std::string::npos);
    ASSERT_STREQ(progressive::annotateEdited("hello", false), "hello");
}

// ==== Megolm session manager ====
static void test_megolm_manager_empty() {
    progressive::MegolmSessionManager mgr;
    ASSERT_EQ(mgr.sessionCount(), 0);
    ASSERT_TRUE(mgr.findSession("!room", "sender", "sid") == nullptr);
}

static void test_megolm_clear_room() {
    progressive::MegolmSessionManager mgr;
    mgr.clearRoom("!test");
    ASSERT_EQ(mgr.sessionCount(), 0);
}

// ==== Canonical JSON ====
static void test_canonical_json_simple() {
    auto result = progressive::canonicalizeJson(R"({"b":2,"a":1})");
    ASSERT_TRUE(result.find("\"a\"") != std::string::npos);
    ASSERT_TRUE(result.find("\"b\"") != std::string::npos);
    ASSERT_TRUE(result.find("\"a\"") < result.find("\"b\"")); // a before b
}

static void test_canonical_json_no_spaces() {
    auto result = progressive::canonicalizeJson(R"( { "x" : 1 } )");
    ASSERT_TRUE(result.find(" ") == std::string::npos);
}

// ==== E2EE End-to-End: account → OTK → SAS → MAC ====
static void test_e2ee_full_pipeline() {
    // 1. Create Olm account (device identity)
    auto alice = progressive::createOlmAccount("@alice:matrix.org", "DEV_A");
    ASSERT_TRUE(alice.valid);

    // 2. Get identity keys
    auto keys = progressive::getAccountIdentityKeys(alice);
    ASSERT_TRUE(!keys.empty());

    // 3. Generate one-time keys
    auto otks = progressive::generateOneTimeKeys(alice, 10);
    ASSERT_TRUE(!otks.empty());

    // 4. Create SAS verification
    auto sas = progressive::sasCreate();
    ASSERT_TRUE(sas.valid);
    ASSERT_TRUE(!sas.ourPubkey.empty());

    // 5. Sign a message
    auto sig = progressive::accountSign(alice, "test message");
    ASSERT_TRUE(!sig.empty());

    // 6. Clean up
    progressive::sasDestroy(sas);
    progressive::destroyOlmAccount(alice);
}

// ==== Event encryption metadata ====
static void test_encryption_algorithm_parse() {
    auto alg = progressive::parseEncryptionAlgorithm("m.megolm.v1.aes-sha2");
    ASSERT_TRUE(alg.isMegolm);
    ASSERT_TRUE(alg.isDefault);
}

// ==== Push notification evaluation ====
static void test_push_eval_own_event() {
    progressive::PushRuleSet rules;
    auto result = progressive::evaluatePushNotification(
        R"({"type":"m.room.message","sender":"@me:matrix.org","room_id":"!r:matrix.org","content":{"body":"hello"}})",
        rules, "Me", "@me:matrix.org");
    ASSERT_FALSE(result.shouldNotify); // own event → no notify
}

// ==== Room upgrade handler ====
static void test_room_upgrade_not_upgrade() {
    auto info = progressive::processRoomUpgrade(R"({"body":"Room closed"})");
    ASSERT_FALSE(info.isUpgrade);
}

// ==== Redaction notice ====
static void test_redaction_self() {
    auto result = progressive::formatRedactionNotice("", true, false);
    ASSERT_TRUE(result.find("You") != std::string::npos);
}

static void test_redaction_with_reason() {
    auto result = progressive::formatRedactionNotice("spam", false, false);
    ASSERT_TRUE(result.find("spam") != std::string::npos);
}

// ==== Key backup validation ====
static void test_validate_bad_recovery_key() {
    auto result = progressive::validateAndFormatRecoveryKey("short");
    ASSERT_TRUE(result.find("\"valid\":false") != std::string::npos);
}

// ==== Chunked uploader ====
static void test_uploader_compute_chunks() {
    progressive::ChunkedUploader uploader;
    uploader.setChunkSizeMb(10);
    int chunks = uploader.computeChunks(15 * 1024 * 1024);
    ASSERT_EQ(chunks, 2);
}

static void test_uploader_suggest_chunk_size() {
    int mb = progressive::ChunkedUploader::suggestChunkSizeMb(50_000_000);
    ASSERT_EQ(mb, 10);
    int mb2 = progressive::ChunkedUploader::suggestChunkSizeMb(500_000_000);
    ASSERT_EQ(mb2, 20);
}

// ==== Audio support ====
static void test_is_supported_audio_type() {
    ASSERT_TRUE(progressive::isSupportedAudioType("audio/ogg"));
    ASSERT_FALSE(progressive::isSupportedAudioType("image/png"));
}

// ==== 3PID parsing ====
static void test_parse_three_pid_email() {
    auto pid = progressive::parseThreePid("alice@matrix.org");
    ASSERT_STREQ(pid.medium, "email");
    ASSERT_TRUE(pid.valid);
}

// ==== Widget Manager ====

#include "progressive/widget_manager.hpp"

static void test_widget_manager_init() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    ASSERT_EQ(mgr.widgetCount(), 0);
}

static void test_widget_url_template() {
    progressive::WidgetUrlTemplate tpl;
    tpl.userId = "@alice:example.org";
    tpl.roomId = "!room:example.org";
    tpl.widgetId = "widget_1";
    tpl.displayName = "Alice";
    tpl.clientId = "chat.progressive.app";
    tpl.clientTheme = "dark";

    std::string url = "https://widget.example.com/?userId=$matrix_user_id&roomId=$matrix_room_id&widgetId=$matrix_widget_id&theme=$matrix_client_theme";
    auto result = progressive::applyWidgetUrlTemplate(url, tpl);
    ASSERT_TRUE(result.find("@alice:example.org") != std::string::npos);
    ASSERT_TRUE(result.find("!room:example.org") != std::string::npos);
    ASSERT_TRUE(result.find("widget_1") != std::string::npos);
    ASSERT_TRUE(result.find("theme=dark") != std::string::npos);
    ASSERT_TRUE(result.find("$matrix_user_id") == std::string::npos);
}

static void test_widget_security_valid() {
    auto policy = progressive::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_TRUE(progressive::validateWidgetSecurity("https://safe.example.com/widget.html", policy, reason));
    ASSERT_STREQ(reason.c_str(), "");
}

static void test_widget_security_blocked_domain() {
    auto policy = progressive::defaultWidgetSecurityPolicy();
    policy.blockedDomains.insert("evil.example.com");
    std::string reason;
    ASSERT_FALSE(progressive::validateWidgetSecurity("https://evil.example.com/widget.html", policy, reason));
    ASSERT_TRUE(!reason.empty());
}

static void test_widget_security_blocked_scheme() {
    auto policy = progressive::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_FALSE(progressive::validateWidgetSecurity("http://insecure.example.com/widget.html", policy, reason));
}

static void test_widget_security_data_url() {
    auto policy = progressive::defaultWidgetSecurityPolicy();
    std::string reason;
    ASSERT_FALSE(progressive::validateWidgetSecurity("data:text/html,<h1>hello</h1>", policy, reason));
}

static void test_widget_security_max_length() {
    auto policy = progressive::defaultWidgetSecurityPolicy();
    policy.maxUrlLength = 20;
    std::string reason;
    ASSERT_FALSE(progressive::validateWidgetSecurity("https://very-long-url.example.com/too/long", policy, reason));
}

static void test_widget_classification() {
    ASSERT_EQ(static_cast<int>(progressive::classifyWidgetType("m.jitsi")),
              static_cast<int>(progressive::WidgetType::JITSI));
    ASSERT_EQ(static_cast<int>(progressive::classifyWidgetType("m.etherpad")),
              static_cast<int>(progressive::WidgetType::ETHERPAD));
    ASSERT_EQ(static_cast<int>(progressive::classifyWidgetType("m.custom")),
              static_cast<int>(progressive::WidgetType::CUSTOM));
    ASSERT_EQ(static_cast<int>(progressive::classifyWidgetType("m.stickerpicker")),
              static_cast<int>(progressive::WidgetType::STICKERPICKER));
    ASSERT_EQ(static_cast<int>(progressive::classifyWidgetType("unknown_type")),
              static_cast<int>(progressive::WidgetType::UNKNOWN));
}

static void test_widget_manager_create() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    auto result = mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Test Widget", false, error);
    ASSERT_TRUE(!result.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.widgetCount(), 1);

    progressive::WidgetInfo w;
    ASSERT_TRUE(mgr.getWidget("widget_1", w));
    ASSERT_STREQ(w.widgetId.c_str(), "widget_1");
    ASSERT_STREQ(w.name.c_str(), "Test Widget");
}

static void test_widget_manager_duplicate() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "First", false, error);
    auto result = mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Second", false, error);
    ASSERT_TRUE(result.empty());
    ASSERT_TRUE(error.find("already exists") != std::string::npos);
    ASSERT_EQ(mgr.widgetCount(), 1);
}

static void test_widget_manager_remove() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.custom", "https://safe.example.org/w", "Test", false, error);
    ASSERT_EQ(mgr.widgetCount(), 1);
    mgr.removeWidget("widget_1");
    ASSERT_EQ(mgr.widgetCount(), 0);
}

static void test_widget_manager_resize() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.jitsi", "https://safe.example.org/w", "Jitsi", false, error);
    auto result = mgr.resizeWidget("widget_1", 800, 600, error);
    ASSERT_TRUE(result.find("800") != std::string::npos);
    ASSERT_STREQ(error.c_str(), "");
}

static void test_widget_manager_minimized() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("widget_1", "m.jitsi", "https://safe.example.org/w", "Jitsi", false, error);

    progressive::WidgetInfo w;
    mgr.getWidget("widget_1", w);
    ASSERT_FALSE(w.isMinimized);

    mgr.setWidgetMinimized("widget_1", true);
    mgr.getWidget("widget_1", w);
    ASSERT_TRUE(w.isMinimized);
    ASSERT_FALSE(w.isMaximized); // minimize should un-maximize
}

static void test_widget_capability_auto_approve() {
    ASSERT_TRUE(progressive::isAutoApprovedCapability(
        progressive::WidgetCapability::CAMERA, "m.jitsi"));
    ASSERT_TRUE(progressive::isAutoApprovedCapability(
        progressive::WidgetCapability::STICKER, "m.stickerpicker"));
    ASSERT_FALSE(progressive::isAutoApprovedCapability(
        progressive::WidgetCapability::SCREENSHARE, "m.custom"));
}

static void test_widget_postmessage_build() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    auto msg = mgr.buildWidgetPostMessage("widget_1", "content_loaded", R"({"ready":true})");
    ASSERT_TRUE(msg.find("fromWidget") != std::string::npos);
    ASSERT_TRUE(msg.find("widget_1") != std::string::npos);
    ASSERT_TRUE(msg.find("content_loaded") != std::string::npos);
    ASSERT_TRUE(msg.find("\"ready\":true") != std::string::npos);
}

static void test_widget_postmessage_parse() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string msg = R"({"api":"toWidget","action":"set_always_on_screen","widgetId":"w1","data":{"value":true}})";
    std::string action, widgetId, data;
    auto api = mgr.parseWidgetPostMessage(msg, action, widgetId, data);
    ASSERT_STREQ(api.c_str(), "toWidget");
    ASSERT_STREQ(action.c_str(), "set_always_on_screen");
    ASSERT_STREQ(widgetId.c_str(), "w1");
    ASSERT_TRUE(data.find("true") != std::string::npos);
}

static void test_widget_pip_support() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("jitsi_1", "m.jitsi", "https://safe.example.org/j", "Jitsi", false, error);
    ASSERT_TRUE(mgr.supportsPiP("jitsi_1"));

    mgr.createWidget("custom_1", "m.custom", "https://safe.example.org/c", "Custom", false, error);
    ASSERT_FALSE(mgr.supportsPiP("custom_1"));
}

static void test_widget_by_type() {
    progressive::WidgetManager mgr("!room:example.org", "@alice:example.org", "Alice", "");
    std::string error;
    mgr.createWidget("j1", "m.jitsi", "https://safe.example.org/j1", "Jitsi1", false, error);
    mgr.createWidget("j2", "m.jitsi", "https://safe.example.org/j2", "Jitsi2", false, error);
    mgr.createWidget("c1", "m.custom", "https://safe.example.org/c1", "Custom1", false, error);

    auto jitsi = mgr.getWidgetsByType("m.jitsi");
    ASSERT_EQ(static_cast<int>(jitsi.size()), 2);
    ASSERT_STREQ(jitsi[0].name.c_str(), "Jitsi1");
    ASSERT_STREQ(jitsi[1].name.c_str(), "Jitsi2");

    ASSERT_EQ(mgr.widgetCount(), 3);
}

// ==== Key Backup Manager ====

#include "progressive/key_backup_manager.hpp"
#include "progressive/key_backup.hpp"

static void test_backup_parse_version() {
    progressive::KeyBackupManager mgr;
    auto ver = mgr.parseBackupVersion(
        R"({"version":"1","algorithm":"m.megolm_backup.v1.curve25519-aes-sha2","auth_data":{"public_key":"abc"},"count":5})");
    ASSERT_TRUE(ver.valid);
    ASSERT_STREQ(ver.version.c_str(), "1");
    ASSERT_EQ(ver.count, 5);
}

static void test_backup_unsupported_algorithm() {
    progressive::KeyBackupManager mgr;
    auto ver = mgr.parseBackupVersion(R"({"version":"1","algorithm":"bad_algo"})");
    ASSERT_FALSE(ver.valid);
    ASSERT_TRUE(!ver.error.empty());
}

static void test_backup_build_create_request() {
    progressive::KeyBackupManager mgr;
    progressive::KeyBackupConfig c;
    c.algorithm = "m.megolm_backup.v1.curve25519-aes-sha2";
    c.version = 1;
    c.authData = R"({"public_key":"test_key"})";
    auto req = mgr.buildCreateBackupVersionRequest(c);
    ASSERT_TRUE(req.find("m.megolm_backup.v1.curve25519-aes-sha2") != std::string::npos);
    ASSERT_TRUE(req.find("test_key") != std::string::npos);
    ASSERT_TRUE(req.find(R"("version":"1")") != std::string::npos);
}

static void test_backup_export_session() {
    progressive::KeyBackupManager mgr;
    auto exp = mgr.exportSessionForBackup(
        "!room:example.org", "sender_key_abc", "sess123", "AQIDBAUG", 100, false, 0);
    ASSERT_STREQ(exp.roomId.c_str(), "!room:example.org");
    ASSERT_STREQ(exp.senderKey.c_str(), "sender_key_abc");
    ASSERT_EQ(exp.firstMessageIndex, 100);
    ASSERT_FALSE(exp.isForwardedKey);
}

static void test_backup_encrypt_session() {
    progressive::KeyBackupManager mgr;
    progressive::MegolmSessionExport s;
    s.roomId = "!room:example.org";
    s.senderKey = "key1";
    s.sessionId = "sess1";
    auto enc = mgr.encryptSessionDataForBackup(s, "{}");
    ASSERT_TRUE(!enc.empty());
    ASSERT_TRUE(enc.find("room_id") != std::string::npos || enc.find("sender_key") != std::string::npos);
}

static void test_backup_verify_integrity() {
    progressive::KeyBackupManager mgr;
    ASSERT_TRUE(mgr.verifyBackupIntegrity(R"({"public_key":"abc","signatures":{"@alice:org":{"ed25519:dev":"sig"}}})"));
    ASSERT_FALSE(mgr.verifyBackupIntegrity(R"({"public_key":""})"));
    ASSERT_FALSE(mgr.verifyBackupIntegrity(R"({})"));
}

static void test_backup_progress() {
    progressive::KeyBackupManager mgr;
    mgr.setTotalKeys(100);
    mgr.advanceUploaded(10);
    mgr.advanceDownloaded(10);
    mgr.advanceDecrypted(8);
    mgr.advanceImported(8);

    auto json = mgr.progressToJson();
    ASSERT_TRUE(json.find("100") != std::string::npos);
    ASSERT_TRUE(json.find(R"("uploaded":10)") != std::string::npos);
    ASSERT_TRUE(json.find("is_running\":true") != std::string::npos);

    mgr.markComplete();
    auto json2 = mgr.progressToJson();
    ASSERT_TRUE(json2.find("is_complete\":true") != std::string::npos);
}

static void test_backup_parse_keys_response() {
    progressive::KeyBackupManager mgr;
    auto rooms = mgr.parseBackupKeysResponse(
        R"({"rooms":{"!room1:org":{"sessions":{"sess1":"data1","sess2":"data2"}},"!room2:org":{"sessions":{"sess3":"data3"}}}})");
    ASSERT_EQ(static_cast<int>(rooms.size()), 2);
    ASSERT_STREQ(rooms[0].roomId.c_str(), "!room1:org");
}

static void test_backup_trust() {
    progressive::KeyBackupManager mgr;
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(progressive::BackupTrust::UNKNOWN));
    mgr.markBackupAsTrusted();
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(progressive::BackupTrust::TRUSTED));
    mgr.markBackupAsUntrusted();
    ASSERT_EQ(static_cast<int>(mgr.getTrustLevel()), static_cast<int>(progressive::BackupTrust::UNTRUSTED));
}

// ==== Live Location ====

#include "progressive/live_location.hpp"

static void test_geo_uri_parse() {
    auto geo = progressive::parseGeoUri("geo:48.858093,2.294694;u=10");
    ASSERT_TRUE(geo.valid);
    ASSERT_TRUE(geo.latitude > 48.0 && geo.latitude < 49.0);
    ASSERT_TRUE(geo.longitude > 2.0 && geo.longitude < 3.0);
    ASSERT_TRUE(geo.uncertainty > 0);
}

static void test_geo_uri_invalid() {
    auto geo = progressive::parseGeoUri("not_a_geo:uri");
    ASSERT_FALSE(geo.valid);
}

static void test_geo_uri_format() {
    progressive::GeoCoordinate c; c.latitude = 48.858093; c.longitude = 2.294694; c.accuracy = 10;
    auto uri = progressive::formatGeoUri(c);
    ASSERT_TRUE(uri.find("geo:") == 0);
    ASSERT_TRUE(uri.find("48.858093") != std::string::npos);
    ASSERT_TRUE(uri.find("2.294694") != std::string::npos);
}

static void test_location_format_message() {
    progressive::GeoCoordinate c; c.latitude = 48.858093; c.longitude = 2.294694;
    auto msg = progressive::formatLocationMessage(c, "Eiffel Tower");
    ASSERT_TRUE(msg.find("Eiffel Tower") != std::string::npos);
    ASSERT_TRUE(msg.find("48.858093") != std::string::npos);
}

static void test_live_session_start_stop() {
    progressive::LiveLocationManager mgr;
    std::string error;
    auto result = mgr.startLiveSession("!room:org", "@alice:org", "Alice's location", 300, 30, true, 60, error);
    ASSERT_TRUE(!result.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.activeSessionCount(), 1);
}

static void test_beacon_info_build() {
    progressive::BeaconInfoContent info;
    info.description = "Test beacon";
    info.timeoutSec = 300;
    info.live = true;
    auto json = progressive::buildBeaconInfoContent(info);
    ASSERT_TRUE(json.find("Test beacon") != std::string::npos);
    ASSERT_TRUE(json.find("\"timeout\":300") != std::string::npos);
    ASSERT_TRUE(json.find("\"live\":true") != std::string::npos);
}

// ==== Notif Formatter ====

#include "progressive/notif_formatter.hpp"

static void test_format_image_notification() {
    auto r = progressive::formatImageNotification("Alice");
    ASSERT_TRUE(r.find("Alice") != std::string::npos);
    ASSERT_TRUE(r.find("image") != std::string::npos || r.find("Image") != std::string::npos || r.find("sent") != std::string::npos);
}

static void test_format_file_notification() {
    auto r = progressive::formatFileNotification("Bob", "report.pdf");
    ASSERT_TRUE(r.find("Bob") != std::string::npos);
    ASSERT_TRUE(r.find("file") != std::string::npos || r.find("File") != std::string::npos);
}

static void test_format_invite_notification() {
    auto r = progressive::formatInviteNotification("Charlie", "Engineering");
    ASSERT_TRUE(r.find("Charlie") != std::string::npos);
    ASSERT_TRUE(r.find("Engineering") != std::string::npos);
}

static void test_format_room_notification() {
    auto r = progressive::formatRoomNotification(5, "General");
    ASSERT_TRUE(r.find("5") != std::string::npos);
}

// ==== Offline Cache ====

#include "progressive/offline_cache.hpp"

static void test_can_fit_in_storage() {
    ASSERT_TRUE(progressive::canFitInStorage(100, 1000, 100));
    ASSERT_FALSE(progressive::canFitInStorage(1000, 1000, 100));
}

static void test_estimate_message_cache_size() {
    auto size = progressive::estimateMessageCacheSize(100, 500);
    ASSERT_TRUE(size > 0);
}

// ==== Lightweight Settings ====

#include "progressive/lightweight_settings.hpp"

static void test_settings_get_bool() {
    ASSERT_TRUE(progressive::getSettingBool(R"({"enabled":true})", "enabled", false));
    ASSERT_FALSE(progressive::getSettingBool(R"({"enabled":true})", "disabled", false));
}

static void test_settings_set_bool() {
    auto r = progressive::setSettingBool(R"({"a":true})", "b", true);
    ASSERT_TRUE(r.find("\"b\":true") != std::string::npos);
}

static void test_settings_get_string() {
    ASSERT_STREQ(progressive::getSettingString(R"({"key":"value"})", "key", "default").c_str(), "value");
    ASSERT_STREQ(progressive::getSettingString(R"({})", "key", "default").c_str(), "default");
}

// ==== Encrypted File ====

#include "progressive/encrypted_file.hpp"

static void test_encrypted_file_key_to_json() {
    progressive::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "test_key_base64"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    auto json = progressive::encryptedFileKeyToJson(k);
    ASSERT_TRUE(json.find("A256CTR") != std::string::npos);
}

static void test_is_valid_jwk_key() {
    progressive::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "key"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    ASSERT_TRUE(progressive::isValidJwkKey(k));
}

static void test_extract_file_key() {
    progressive::EncryptedFileKey k;
    k.alg = "A256CTR"; k.kty = "oct"; k.k = "secret_key"; k.ext = true;
    k.keyOps = {"encrypt", "decrypt"};
    auto key = progressive::extractFileKey(k);
    ASSERT_STREQ(key.c_str(), "secret_key");
}

// ==== URL Preview ====

#include "progressive/url_preview.hpp"

static void test_extract_html_title() {
    auto title = progressive::extractHtmlTitle("<html><head><title>My Page</title></head></html>");
    ASSERT_STREQ(title.c_str(), "My Page");
}

static void test_resolve_url() {
    auto resolved = progressive::resolveUrl("https://example.com/path/", "page.html");
    ASSERT_STREQ(resolved.c_str(), "https://example.com/path/page.html");
}

// ==== Web Search ====

#include "progressive/web_search.hpp"

static void test_build_searxng_url() {
    auto url = progressive::buildSearxngUrl("https://search.example.com", "test query", 10);
    ASSERT_TRUE(url.find("search.example.com") != std::string::npos);
    ASSERT_TRUE(url.find("test+query") != std::string::npos || url.find("test%20query") != std::string::npos);
}

static void test_build_duckduckgo_url() {
    auto url = progressive::buildDuckDuckGoUrl("test");
    ASSERT_TRUE(url.find("api.duckduckgo.com") != std::string::npos);
}

// ==== Signout Service ====

#include "progressive/signout_service.hpp"

static void test_should_ignore_signout_error() {
    ASSERT_TRUE(progressive::shouldIgnoreSignOutError("M_UNKNOWN_TOKEN", 401));
    ASSERT_FALSE(progressive::shouldIgnoreSignOutError("M_FORBIDDEN", 403));
}

// ==== Message Extras ====

#include "progressive/message_extras.hpp"
#include "progressive/room_uploads.hpp"
#include "progressive/auth_models.hpp"
#include "progressive/call_models.hpp"
#include "progressive/json_parser.hpp"

static void test_poll_type_to_string() {
    auto s = progressive::pollTypeToString(progressive::PollType::DISCLOSED);
    ASSERT_STREQ(s, "m.poll.disclosed");
}

static void test_is_sticker_event() {
    ASSERT_TRUE(progressive::isStickerEvent("m.sticker"));
    ASSERT_FALSE(progressive::isStickerEvent("m.room.message"));
}

static void test_presence_enum_to_string() {
    ASSERT_STREQ(progressive::presenceEnumToString(progressive::PresenceEnum::ONLINE), "online");
}

static void test_sdp_type_to_string() {
    ASSERT_STREQ(progressive::sdpTypeToString(progressive::SdpType::OFFER), "offer");
}

static void test_parse_json_string_value() {
    ASSERT_STREQ(progressive::parseJsonStringValue(R"({"key":"hello"})", "key").c_str(), "hello");
    ASSERT_STREQ(progressive::parseJsonStringValue(R"({"a":"b"})", "c").c_str(), "");
}

static void test_end_call_reason_to_string() {
    ASSERT_STREQ(progressive::endCallReasonToString(progressive::EndCallReason::USER_HUNG_UP), "user_hung_up");
}

// ==== Call Manager ====

#include "progressive/call_manager.hpp"

static void test_call_state_to_string() {
    ASSERT_STREQ(progressive::callStateToString(progressive::CallState::CONNECTED), "connected");
    ASSERT_STREQ(progressive::callStateToString(progressive::CallState::RINGING), "ringing");
}

static void test_call_start_outgoing() {
    progressive::CallManager mgr;
    std::string error;
    auto json = mgr.startOutgoingCall("!room:org", "@bob:org", "Bob", progressive::CallType::VOICE, "v=0\r\n", error);
    ASSERT_TRUE(!json.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_EQ(mgr.totalCalls(), 1);
    ASSERT_TRUE(mgr.isRoomInCall("!room:org"));
}

static void test_call_incoming() {
    progressive::CallManager mgr;
    mgr.handleIncomingCall("call_123", "!room:org", "@alice:org", "Alice", progressive::CallType::VIDEO, "v=0\r\n", 120);
    ASSERT_EQ(mgr.totalCalls(), 1);
    progressive::CallInfo ci;
    ASSERT_TRUE(mgr.getIncomingCall(ci));
    ASSERT_STREQ(ci.callId.c_str(), "call_123");
    ASSERT_TRUE(ci.type == progressive::CallType::VIDEO);
}

static void test_call_answer_reject() {
    progressive::CallManager mgr;
    mgr.handleIncomingCall("call_1", "!room:org", "@alice:org", "Alice", progressive::CallType::VOICE, "v=0\r\n", 120);
    std::string error;
    auto answerJson = mgr.answerCall("call_1", "v=0\r\na=sendrecv\r\n", error);
    ASSERT_TRUE(!answerJson.empty());
    ASSERT_STREQ(error.c_str(), "");
}

static void test_call_hangup() {
    progressive::CallManager mgr;
    std::string error;
    mgr.startOutgoingCall("!room:org", "@bob:org", "Bob", progressive::CallType::VOICE, "v=0\r\n", error);
    auto json = mgr.hangupCall("call_1", progressive::EndCallReason::USER_HUNG_UP);
    ASSERT_TRUE(!json.empty());
}

static void test_call_room_in_call() {
    progressive::CallManager mgr;
    ASSERT_FALSE(mgr.isRoomInCall("!room:org"));
    std::string error;
    mgr.startOutgoingCall("!room:org", "@bob:org", "Bob", progressive::CallType::VOICE, "v=0\r\n", error);
    ASSERT_TRUE(mgr.isRoomInCall("!room:org"));
}

static void test_call_format_duration() {
    progressive::CallManager mgr;
    ASSERT_STREQ(mgr.formatCallDuration(65).c_str(), "01:05");
    ASSERT_STREQ(mgr.formatCallDuration(3661).c_str(), "1:01:01");
}

static void test_call_sdp_parse() {
    std::string sdp = "v=0\r\no=- 123 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\nm=audio 9 UDP/TLS/RTP/SAVPF 111\r\nc=IN IP4 0.0.0.0\r\na=ice-ufrag:test\r\na=ice-pwd:secret\r\na=fingerprint:sha-256 AB:CD\r\na=setup:actpass\r\n";
    auto parsed = progressive::parseSdp(sdp, "offer");
    ASSERT_TRUE(parsed.valid);
    ASSERT_TRUE(parsed.hasAudio);
    ASSERT_STREQ(parsed.hash.c_str(), "sha-256");
}

// ==== Thread Manager ====

#include "progressive/thread_manager.hpp"

static void test_thread_is_root() {
    progressive::ThreadManager mgr;
    auto content = R"({"m.relates_to":{"rel_type":"m.thread","event_id":"$evt1"}})";
    ASSERT_TRUE(mgr.isThreadRoot(content, "$evt1"));
    ASSERT_FALSE(mgr.isThreadRoot(content, "$evt_other"));
}

static void test_thread_extract_root() {
    progressive::ThreadManager mgr;
    auto content = R"({"m.relates_to":{"rel_type":"m.thread","event_id":"$root123"}})";
    ASSERT_STREQ(mgr.extractThreadRoot(content).c_str(), "$root123");
}

static void test_thread_upsert_and_list() {
    progressive::ThreadManager mgr;
    progressive::ThreadInfo t;
    t.threadId = "$thread1"; t.roomId = "!room:org";
    t.rootSenderId = "@alice:org"; t.rootSenderName = "Alice";
    t.rootBody = "Hello thread!"; t.rootTimestampMs = 1000;
    t.valid = true;
    mgr.upsertThread(t);
    t.threadId = "$thread2"; t.rootSenderName = "Bob"; t.rootBody = "Another thread"; t.rootTimestampMs = 2000;
    mgr.upsertThread(t);

    ASSERT_EQ(mgr.totalThreads(), 2);
    auto list = mgr.getThreadList(10, 0);
    ASSERT_EQ(list.totalCount, 2);
    ASSERT_EQ(static_cast<int>(list.threads.size()), 2);
}

static void test_thread_unread() {
    progressive::ThreadManager mgr;
    progressive::ThreadInfo t;
    t.threadId = "$t1"; t.roomId = "!room:org"; t.rootSenderName = "A"; t.rootBody = "b"; t.valid = true;
    mgr.upsertThread(t);

    mgr.setThreadUnread("$t1", 5, true);
    ASSERT_EQ(mgr.getTotalUnreadCount(), 5);

    mgr.setThreadUnread("$t1", 0, false);
    ASSERT_EQ(mgr.getTotalUnreadCount(), 0);
}

static void test_thread_format_count() {
    progressive::ThreadManager mgr;
    ASSERT_STREQ(mgr.formatThreadNotificationCount(5).c_str(), "5");
    ASSERT_STREQ(mgr.formatThreadNotificationCount(99).c_str(), "99");
    ASSERT_STREQ(mgr.formatThreadNotificationCount(100).c_str(), "99+");
    ASSERT_STREQ(mgr.formatThreadNotificationCount(0).c_str(), "");
}

// ==== Poll Manager ====

#include "progressive/poll_manager.hpp"

static void test_poll_build_start() {
    progressive::PollManager mgr;
    std::string error;
    auto json = mgr.buildPollStartContent("Favorite color?", {"Red", "Blue", "Green"},
        progressive::PollKind::DISCLOSED, 1, true, error);
    ASSERT_TRUE(!json.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_TRUE(json.find("Favorite color?") != std::string::npos);
    ASSERT_TRUE(json.find("Red") != std::string::npos);
}

static void test_poll_parse_start() {
    progressive::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Yes or No?","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"Yes"},{"id":"B","org.matrix.msc3381.poll.org_text":"No"}]})";
    auto poll = mgr.parsePollStartContent(content, true);
    ASSERT_TRUE(poll.valid);
    ASSERT_STREQ(poll.question.c_str(), "Yes or No?");
    ASSERT_EQ(static_cast<int>(poll.options.size()), 2);
}

static void test_poll_validation() {
    progressive::PollManager mgr;
    ASSERT_TRUE(mgr.isValidPollQuestion("Good question?"));
    ASSERT_FALSE(mgr.isValidPollQuestion(""));
    ASSERT_TRUE(mgr.isValidPollOption("Option A"));
    ASSERT_FALSE(mgr.isValidPollOption(""));
    ASSERT_TRUE(mgr.isValidMaxSelections(1, 3));
    ASSERT_FALSE(mgr.isValidMaxSelections(5, 3));
}

static void test_poll_build_too_few_options() {
    progressive::PollManager mgr;
    std::string error;
    auto json = mgr.buildPollStartContent("Q?", {"Only one"}, progressive::PollKind::DISCLOSED, 1, true, error);
    ASSERT_TRUE(json.empty());
    ASSERT_TRUE(!error.empty());
}

static void test_poll_tally() {
    progressive::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Yes or No?","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"Yes"},{"id":"B","org.matrix.msc3381.poll.org_text":"No"}]})";
    auto poll = mgr.parsePollStartContent(content, true);

    progressive::PollVote v1, v2, v3;
    v1.selectedOptionIds = {"A"}; v1.voterId = "@alice:org";
    v2.selectedOptionIds = {"B"}; v2.voterId = "@bob:org";
    v3.selectedOptionIds = {"A"}; v3.voterId = "@charlie:org";

    auto result = mgr.tallyVotes(poll, {v1, v2, v3});
    ASSERT_EQ(result.totalVotes, 3);
    ASSERT_EQ(result.results[0].voteCount, 2); // Yes = 2
    ASSERT_EQ(result.results[1].voteCount, 1); // No = 1
}

static void test_poll_winners() {
    progressive::PollManager mgr;
    std::string content = R"({"org.matrix.msc3381.poll.start":{},"org.matrix.msc3381.poll.kind":"disclosed","org.matrix.msc3381.poll.max_selections":1,"org.matrix.msc3381.poll.question":{"body":"Pick one","msgtype":"m.text"},"org.matrix.msc3381.poll.answers":[{"id":"A","org.matrix.msc3381.poll.org_text":"A"},{"id":"B","org.matrix.msc3381.poll.org_text":"B"}]})";
    auto poll = mgr.parsePollStartContent(content, true);
    progressive::PollVote v; v.selectedOptionIds = {"A"}; v.voterId = "@u:org";
    auto result = mgr.tallyVotes(poll, {v});
    ASSERT_STREQ(mgr.getWinnerText(result).c_str(), "A — 1 vote");
}

// ==== Space Graph ====

#include "progressive/space_graph.hpp"

static void test_space_set_root() {
    progressive::SpaceGraph sg;
    sg.setRoot("!space:org", "My Space", "A test space", "");
    ASSERT_EQ(sg.nodeCount(), 1);
}

static void test_space_add_child() {
    progressive::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    progressive::SpaceChildEntry c;
    c.roomId = "!room1:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);

    auto children = sg.getChildren("!space:org");
    ASSERT_EQ(static_cast<int>(children.size()), 1);
    ASSERT_STREQ(children[0].roomId.c_str(), "!room1:org");
}

static void test_space_parents() {
    progressive::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    progressive::SpaceChildEntry c;
    c.roomId = "!room1:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);

    auto parents = sg.getParents("!room1:org");
    ASSERT_EQ(static_cast<int>(parents.size()), 1);
    ASSERT_STREQ(parents[0].c_str(), "!space:org");
}

static void test_space_depth() {
    progressive::SpaceGraph sg;
    sg.setRoot("!space:org", "Root");
    progressive::SpaceChildEntry c;
    c.roomId = "!sub:org"; c.suggested = true; c.valid = true;
    sg.addChild("!space:org", c);
    c.roomId = "!room1:org";
    sg.addChild("!sub:org", c);

    ASSERT_EQ(sg.getDepth("!space:org"), 0);
    ASSERT_EQ(sg.getDepth("!sub:org"), -1); // Not computed until traverse
}

static void test_space_search() {
    progressive::SpaceGraph sg;
    sg.setRoot("!s:org", "Root");
    sg.setNodeMetadata("!r1:org", "Engineering", "Tech room", "", "public", false);
    sg.setNodeMetadata("!r2:org", "Marketing", "Sales room", "", "public", false);
    progressive::SpaceChildEntry c;
    c.roomId = "!r1:org"; c.valid = true;
    sg.addChild("!s:org", c);
    c.roomId = "!r2:org";
    sg.addChild("!s:org", c);

    auto results = sg.searchSpaceRooms("!s:org", "engine");
    ASSERT_EQ(static_cast<int>(results.size()), 1);
    ASSERT_STREQ(results[0].name.c_str(), "Engineering");
}

// ==== Pin Manager ====

#include "progressive/pin_manager.hpp"

static void test_pin_event() {
    progressive::PinManager mgr;
    std::string error;
    auto json = mgr.pinEvent("!room:org", "$evt1", "@alice:org", 50, error);
    ASSERT_TRUE(!json.empty());
    ASSERT_STREQ(error.c_str(), "");
    ASSERT_TRUE(mgr.isEventPinned("!room:org", "$evt1"));
    ASSERT_EQ(mgr.getPinnedCount("!room:org"), 1);
}

static void test_pin_duplicate() {
    progressive::PinManager mgr;
    std::string error;
    mgr.pinEvent("!room:org", "$evt1", "@alice:org", 50, error);
    auto json = mgr.pinEvent("!room:org", "$evt1", "@alice:org", 50, error);
    ASSERT_TRUE(json.empty());
    ASSERT_TRUE(error.find("already pinned") != std::string::npos);
}

static void test_unpin_event() {
    progressive::PinManager mgr;
    std::string error;
    mgr.pinEvent("!room:org", "$evt1", "@alice:org", 50, error);
    ASSERT_EQ(mgr.getPinnedCount("!room:org"), 1);
    mgr.unpinEvent("!room:org", "$evt1", "@alice:org", 50, error);
    ASSERT_FALSE(mgr.isEventPinned("!room:org", "$evt1"));
    ASSERT_EQ(mgr.getPinnedCount("!room:org"), 0);
}

static void test_pin_power_level() {
    progressive::PinManager mgr;
    ASSERT_TRUE(mgr.canManagePins(50));
    ASSERT_TRUE(mgr.canManagePins(100));
    ASSERT_FALSE(mgr.canManagePins(10));
}

static void test_pin_parse_ids() {
    auto ids = progressive::PinManager::parsePinnedEventIds(R"({"pinned":["$evt1","$evt2","$evt3"]})");
    ASSERT_EQ(static_cast<int>(ids.size()), 3);
    ASSERT_STREQ(ids[0].c_str(), "$evt1");
    ASSERT_STREQ(ids[2].c_str(), "$evt3");
}

static void test_pin_build_content() {
    auto json = progressive::PinManager::buildPinnedEventsContent({"$a", "$b"});
    ASSERT_TRUE(json.find("$a") != std::string::npos);
    ASSERT_TRUE(json.find("$b") != std::string::npos);
    ASSERT_TRUE(json.find("pinned") != std::string::npos);
}

// ==== Media Viewer ====

#include "progressive/media_viewer.hpp"

static void test_media_parse() {
    std::string content = R"({"url":"mxc://example.org/abc123","mimetype":"image/jpeg","body":"photo.jpg","info":{"w":1920,"h":1080,"size":204800,"thumbnail_url":"mxc://example.org/thumb"}})";
    auto info = progressive::parseMediaInfo(content);
    ASSERT_STREQ(info.mxcUrl.c_str(), "mxc://example.org/abc123");
    ASSERT_EQ(info.width, 1920);
    ASSERT_EQ(info.height, 1080);
    ASSERT_TRUE(info.hasThumbnail);
}

static void test_media_format_size() {
    ASSERT_STREQ(progressive::formatMediaSize(1024).c_str(), "1.0 KB");
    ASSERT_STREQ(progressive::formatMediaSize(1048576).c_str(), "1.0 MB");
    ASSERT_STREQ(progressive::formatMediaSize(500).c_str(), "500 B");
}

static void test_media_format_duration() {
    ASSERT_STREQ(progressive::formatMediaDuration(65000).c_str(), "1:05");
    ASSERT_STREQ(progressive::formatMediaDuration(3661000).c_str(), "1:01:01");
}

static void test_media_exif_rotation() {
    ASSERT_EQ(progressive::exifRotationDegrees(progressive::exifFromRaw(6)), 90);
    ASSERT_EQ(progressive::exifRotationDegrees(progressive::exifFromRaw(3)), 180);
    ASSERT_EQ(progressive::exifRotationDegrees(progressive::exifFromRaw(8)), 270);
    ASSERT_EQ(progressive::exifRotationDegrees(progressive::exifFromRaw(1)), 0);
}

static void test_media_mxc_resolve() {
    auto url = progressive::resolveMxcDownloadUrl("mxc://matrix.org/abc123", "https://matrix.org");
    ASSERT_TRUE(url.find("_matrix/media/r0/download/matrix.org/abc123") != std::string::npos);
}

static void test_media_type_detect() {
    ASSERT_TRUE(progressive::detectMediaType("image/png") == progressive::MediaType::IMAGE);
    ASSERT_TRUE(progressive::detectMediaType("video/mp4") == progressive::MediaType::VIDEO);
    ASSERT_TRUE(progressive::detectMediaType("audio/ogg") == progressive::MediaType::AUDIO);
    ASSERT_TRUE(progressive::detectMediaType("application/pdf") == progressive::MediaType::FILE);
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
    
    printf("\n-- Membership & URL --\n");
    ADD_TEST(runner, test_format_membership_join);
    ADD_TEST(runner, test_is_active_member);
    ADD_TEST(runner, test_is_matrix_to_permalink);
    ADD_TEST(runner, test_extract_room_id_from_permalink);
    
    printf("\n-- SSO & Connection --\n");
    ADD_TEST(runner, test_is_sso_callback_url);
    ADD_TEST(runner, test_format_downtime);
    ADD_TEST(runner, test_get_banner_color);
    
    printf("\n-- Date/Time --\n");
    ADD_TEST(runner, test_format_duration_seconds);
    ADD_TEST(runner, test_format_duration_minutes);
    
    printf("\n-- Errors & Content --\n");
    ADD_TEST(runner, test_is_password_error);
    ADD_TEST(runner, test_get_all_error_codes);
    ADD_TEST(runner, test_count_emojis);
    ADD_TEST(runner, test_format_media_collapse_label);
    
    printf("\n-- Invite & Files --\n");
    ADD_TEST(runner, test_is_invite_expired);
    ADD_TEST(runner, test_build_knock_body);
    ADD_TEST(runner, test_format_file_size);
    ADD_TEST(runner, test_format_file_size_bytes);
    
    printf("\n-- Member & Call Notices --\n");
    ADD_TEST(runner, test_member_notice_join);
    ADD_TEST(runner, test_member_notice_invite);
    ADD_TEST(runner, test_call_notice_invite);
    ADD_TEST(runner, test_call_notice_reject);
    ADD_TEST(runner, test_annotate_edited);
    
    printf("\n-- Megolm --\n");
    ADD_TEST(runner, test_megolm_manager_empty);
    ADD_TEST(runner, test_megolm_clear_room);
    
    printf("\n-- Canonical JSON --\n");
    ADD_TEST(runner, test_canonical_json_simple);
    ADD_TEST(runner, test_canonical_json_no_spaces);
    
    printf("\n-- E2EE End-to-End --\n");
    ADD_TEST(runner, test_e2ee_full_pipeline);
    ADD_TEST(runner, test_encryption_algorithm_parse);
    
    printf("\n-- Serious Components --\n");
    ADD_TEST(runner, test_push_eval_own_event);
    ADD_TEST(runner, test_room_upgrade_not_upgrade);
    ADD_TEST(runner, test_redaction_self);
    ADD_TEST(runner, test_redaction_with_reason);
    ADD_TEST(runner, test_validate_bad_recovery_key);
    
    printf("\n-- Uploader --\n");
    ADD_TEST(runner, test_uploader_compute_chunks);
    ADD_TEST(runner, test_uploader_suggest_chunk_size);
    
    printf("\n-- Audio & 3PID --\n");
    ADD_TEST(runner, test_is_supported_audio_type);
    ADD_TEST(runner, test_parse_three_pid_email);
    
    printf("\n-- Widget Manager --\n");
    ADD_TEST(runner, test_widget_manager_init);
    ADD_TEST(runner, test_widget_url_template);
    ADD_TEST(runner, test_widget_security_valid);
    ADD_TEST(runner, test_widget_security_blocked_domain);
    ADD_TEST(runner, test_widget_security_blocked_scheme);
    ADD_TEST(runner, test_widget_security_data_url);
    ADD_TEST(runner, test_widget_security_max_length);
    ADD_TEST(runner, test_widget_classification);
    ADD_TEST(runner, test_widget_manager_create);
    ADD_TEST(runner, test_widget_manager_duplicate);
    ADD_TEST(runner, test_widget_manager_remove);
    ADD_TEST(runner, test_widget_manager_resize);
    ADD_TEST(runner, test_widget_manager_minimized);
    ADD_TEST(runner, test_widget_capability_auto_approve);
    ADD_TEST(runner, test_widget_postmessage_build);
    ADD_TEST(runner, test_widget_postmessage_parse);
    ADD_TEST(runner, test_widget_pip_support);
    ADD_TEST(runner, test_widget_by_type);
    
    printf("\n-- Key Backup Manager --\n");
    ADD_TEST(runner, test_backup_parse_version);
    ADD_TEST(runner, test_backup_unsupported_algorithm);
    ADD_TEST(runner, test_backup_build_create_request);
    ADD_TEST(runner, test_backup_export_session);
    ADD_TEST(runner, test_backup_encrypt_session);
    ADD_TEST(runner, test_backup_verify_integrity);
    ADD_TEST(runner, test_backup_progress);
    ADD_TEST(runner, test_backup_parse_keys_response);
    ADD_TEST(runner, test_backup_trust);
    
    printf("\n-- Live Location --\n");
    ADD_TEST(runner, test_geo_uri_parse);
    ADD_TEST(runner, test_geo_uri_invalid);
    ADD_TEST(runner, test_geo_uri_format);
    ADD_TEST(runner, test_location_format_message);
    ADD_TEST(runner, test_live_session_start_stop);
    ADD_TEST(runner, test_beacon_info_build);
    
    printf("\n-- Notif Formatter --\n");
    ADD_TEST(runner, test_format_image_notification);
    ADD_TEST(runner, test_format_file_notification);
    ADD_TEST(runner, test_format_invite_notification);
    ADD_TEST(runner, test_format_room_notification);
    
    printf("\n-- Offline Cache --\n");
    ADD_TEST(runner, test_can_fit_in_storage);
    ADD_TEST(runner, test_estimate_message_cache_size);
    
    printf("\n-- Lightweight Settings --\n");
    ADD_TEST(runner, test_settings_get_bool);
    ADD_TEST(runner, test_settings_set_bool);
    ADD_TEST(runner, test_settings_get_string);
    
    printf("\n-- Encrypted File --\n");
    ADD_TEST(runner, test_encrypted_file_key_to_json);
    ADD_TEST(runner, test_is_valid_jwk_key);
    ADD_TEST(runner, test_extract_file_key);
    
    printf("\n-- URL Preview + Web Search --\n");
    ADD_TEST(runner, test_extract_html_title);
    ADD_TEST(runner, test_resolve_url);
    ADD_TEST(runner, test_build_searxng_url);
    ADD_TEST(runner, test_build_duckduckgo_url);
    
    printf("\n-- Signout + Extras + Misc --\n");
    ADD_TEST(runner, test_should_ignore_signout_error);
    ADD_TEST(runner, test_poll_type_to_string);
    ADD_TEST(runner, test_is_sticker_event);
    ADD_TEST(runner, test_presence_enum_to_string);
    ADD_TEST(runner, test_sdp_type_to_string);
    ADD_TEST(runner, test_parse_json_string_value);
    ADD_TEST(runner, test_end_call_reason_to_string);
    
    printf("\n-- Call Manager --\n");
    ADD_TEST(runner, test_call_state_to_string);
    ADD_TEST(runner, test_call_start_outgoing);
    ADD_TEST(runner, test_call_incoming);
    ADD_TEST(runner, test_call_answer_reject);
    ADD_TEST(runner, test_call_hangup);
    ADD_TEST(runner, test_call_room_in_call);
    ADD_TEST(runner, test_call_format_duration);
    ADD_TEST(runner, test_call_sdp_parse);
    
    printf("\n-- Thread Manager --\n");
    ADD_TEST(runner, test_thread_is_root);
    ADD_TEST(runner, test_thread_extract_root);
    ADD_TEST(runner, test_thread_upsert_and_list);
    ADD_TEST(runner, test_thread_unread);
    ADD_TEST(runner, test_thread_format_count);
    
    printf("\n-- Poll Manager --\n");
    ADD_TEST(runner, test_poll_build_start);
    ADD_TEST(runner, test_poll_parse_start);
    ADD_TEST(runner, test_poll_validation);
    ADD_TEST(runner, test_poll_build_too_few_options);
    ADD_TEST(runner, test_poll_tally);
    ADD_TEST(runner, test_poll_winners);
    
    printf("\n-- Space Graph --\n");
    ADD_TEST(runner, test_space_set_root);
    ADD_TEST(runner, test_space_add_child);
    ADD_TEST(runner, test_space_parents);
    ADD_TEST(runner, test_space_depth);
    ADD_TEST(runner, test_space_search);
    
    printf("\n-- Pin Manager --\n");
    ADD_TEST(runner, test_pin_event);
    ADD_TEST(runner, test_pin_duplicate);
    ADD_TEST(runner, test_unpin_event);
    ADD_TEST(runner, test_pin_power_level);
    ADD_TEST(runner, test_pin_parse_ids);
    ADD_TEST(runner, test_pin_build_content);
    
    printf("\n-- Media Viewer --\n");
    ADD_TEST(runner, test_media_parse);
    ADD_TEST(runner, test_media_format_size);
    ADD_TEST(runner, test_media_format_duration);
    ADD_TEST(runner, test_media_exif_rotation);
    ADD_TEST(runner, test_media_mxc_resolve);
    ADD_TEST(runner, test_media_type_detect);
    
    return runner.summary();
}
