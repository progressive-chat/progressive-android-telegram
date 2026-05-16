// Progressive Chat C++ unit tests — critical path verification
#include "test_framework.hpp"
#include "progressive/crypto_algorithms.hpp"
#include "progressive/timeline_chunk.hpp"
#include "progressive/markdown.hpp"
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
    
    return runner.summary();
}
