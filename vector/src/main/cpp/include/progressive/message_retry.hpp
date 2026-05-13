#ifndef PROGRESSIVE_MESSAGE_RETRY_HPP
#define PROGRESSIVE_MESSAGE_RETRY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Message Retry Logic ----

struct RetryPolicy {
    int maxRetries = 5;
    int baseDelayMs = 1000;        // initial delay
    int maxDelayMs = 60000;        // maximum delay (1 minute)
    double backoffMultiplier = 2.0; // exponential backoff factor
    bool useJitter = true;         // add random jitter to delay
};

struct RetryState {
    std::string eventId;
    int attemptCount = 0;
    int64_t lastAttemptMs = 0;
    int64_t nextAttemptMs = 0;
    int currentDelayMs = 0;
    bool shouldRetry = true;
    std::string lastError;
    bool isPermanentFailure = false;
};

// Compute retry delay for the next attempt.
int computeRetryDelay(const RetryPolicy& policy, int attemptCount);

// Check if a message should be retried.
bool shouldRetry(const RetryState& state, const RetryPolicy& policy);

// Update retry state after a failed attempt.
RetryState updateRetryState(const RetryState& state, const RetryPolicy& policy,
    const std::string& error, bool isPermanent);

// Check if an error is a permanent failure (should not retry).
bool isPermanentError(const std::string& errorMessage);

// Format retry state for UI display.
std::string formatRetryState(const RetryState& state);

// Get recommended retry policy for a given network condition.
RetryPolicy getRetryPolicyForNetwork(const std::string& networkType); // "wifi", "cellular", "unknown"

// ---- Message Send Queue ----

struct SendQueueItem {
    std::string eventId;
    std::string roomId;
    std::string body;
    std::string formattedBody;
    int priority = 0;            // lower = sent first
    int64_t enqueuedAtMs = 0;
    int retryCount = 0;
    bool isSending = false;
    bool failed = false;
    std::string errorMessage;
};

struct SendQueueStats {
    int totalItems = 0;
    int pendingItems = 0;
    int sendingItems = 0;
    int failedItems = 0;
    int64_t oldestItemMs = 0;
};

// Add an item to the send queue.
void enqueueMessage(std::vector<SendQueueItem>& queue, const SendQueueItem& item);

// Get the next item to send (highest priority, not already sending).
const SendQueueItem* getNextToSend(const std::vector<SendQueueItem>& queue);

// Mark an item as sent (remove from queue).
void markSent(std::vector<SendQueueItem>& queue, const std::string& eventId);

// Mark an item as failed (will retry).
void markFailed(std::vector<SendQueueItem>& queue, const std::string& eventId, const std::string& error);

// Compute send queue statistics.
SendQueueStats computeSendQueueStats(const std::vector<SendQueueItem>& queue);

// Sort queue by priority (lowest first).
void sortSendQueue(std::vector<SendQueueItem>& queue);

// Format send queue stats as JSON.
std::string sendQueueStatsToJson(const SendQueueStats& stats);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_RETRY_HPP
