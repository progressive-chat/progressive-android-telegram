#include "progressive/message_retry.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>

namespace progressive {

int computeRetryDelay(const RetryPolicy& policy, int attemptCount) {
    if (attemptCount <= 0) return 0;

    double delay = policy.baseDelayMs * std::pow(policy.backoffMultiplier, attemptCount - 1);
    if (delay > policy.maxDelayMs) delay = policy.maxDelayMs;

    if (policy.useJitter) {
        // Add ±25% jitter
        std::random_device rd;
        std::mt19937 gen(rd());
        double jitter = 1.0 + (std::uniform_real_distribution<double>(-0.25, 0.25)(gen));
        delay *= jitter;
    }

    return static_cast<int>(delay);
}

bool shouldRetry(const RetryState& state, const RetryPolicy& policy) {
    if (!state.shouldRetry) return false;
    if (state.isPermanentFailure) return false;
    if (state.attemptCount >= policy.maxRetries) return false;

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return now >= state.nextAttemptMs;
}

RetryState updateRetryState(const RetryState& state, const RetryPolicy& policy,
    const std::string& error, bool isPermanent) {
    RetryState updated = state;
    updated.attemptCount++;
    updated.lastError = error;
    updated.isPermanentFailure = isPermanent;

    if (isPermanent) {
        updated.shouldRetry = false;
        return updated;
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    updated.currentDelayMs = computeRetryDelay(policy, updated.attemptCount);
    updated.lastAttemptMs = now;
    updated.nextAttemptMs = now + updated.currentDelayMs;

    if (updated.attemptCount >= policy.maxRetries) {
        updated.shouldRetry = false;
    }

    return updated;
}

bool isPermanentError(const std::string& errorMessage) {
    // Matrix error codes that should NOT be retried
    if (errorMessage.find("M_FORBIDDEN") != std::string::npos) return true;
    if (errorMessage.find("M_UNKNOWN_TOKEN") != std::string::npos) return true;
    if (errorMessage.find("M_MISSING_TOKEN") != std::string::npos) return true;
    if (errorMessage.find("M_BAD_JSON") != std::string::npos) return true;
    if (errorMessage.find("M_NOT_JSON") != std::string::npos) return true;
    if (errorMessage.find("M_UNKNOWN") != std::string::npos) return true;
    if (errorMessage.find("M_INVALID_USERNAME") != std::string::npos) return true;

    // Network errors are retryable
    if (errorMessage.find("timeout") != std::string::npos) return false;
    if (errorMessage.find("connection") != std::string::npos) return false;
    if (errorMessage.find("network") != std::string::npos) return false;

    return false;
}

std::string formatRetryState(const RetryState& state) {
    std::ostringstream out;
    out << "Attempt " << state.attemptCount;
    if (!state.shouldRetry) {
        out << " (stopped)";
    } else if (state.currentDelayMs > 0) {
        out << " - next retry in " << (state.currentDelayMs / 1000) << "s";
    }
    if (!state.lastError.empty()) {
        out << "\nError: " << state.lastError;
    }
    return out.str();
}

RetryPolicy getRetryPolicyForNetwork(const std::string& networkType) {
    if (networkType == "wifi") {
        return {3, 1000, 10000, 1.5, true};
    } else if (networkType == "cellular") {
        return {5, 2000, 30000, 2.0, true};
    }
    return {5, 1000, 60000, 2.0, true}; // default
}

// ---- Send Queue ----

void enqueueMessage(std::vector<SendQueueItem>& queue, const SendQueueItem& item) {
    SendQueueItem copy = item;
    if (copy.enqueuedAtMs == 0) {
        copy.enqueuedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    queue.push_back(copy);
}

const SendQueueItem* getNextToSend(const std::vector<SendQueueItem>& queue) {
    // Find the highest priority item that hasn't failed and isn't sending
    const SendQueueItem* best = nullptr;
    for (const auto& item : queue) {
        if (item.isSending || item.failed) continue;
        if (!best || item.priority < best->priority) best = &item;
    }
    return best;
}

void markSent(std::vector<SendQueueItem>& queue, const std::string& eventId) {
    queue.erase(std::remove_if(queue.begin(), queue.end(),
        [&](const SendQueueItem& item) { return item.eventId == eventId; }
    ), queue.end());
}

void markFailed(std::vector<SendQueueItem>& queue, const std::string& eventId, const std::string& error) {
    for (auto& item : queue) {
        if (item.eventId == eventId) {
            item.failed = true;
            item.isSending = false;
            item.errorMessage = error;
            item.retryCount++;
            return;
        }
    }
}

SendQueueStats computeSendQueueStats(const std::vector<SendQueueItem>& queue) {
    SendQueueStats stats;
    stats.totalItems = static_cast<int>(queue.size());

    for (const auto& item : queue) {
        if (item.isSending) stats.sendingItems++;
        else if (item.failed) stats.failedItems++;
        else stats.pendingItems++;

        if (stats.oldestItemMs == 0 || item.enqueuedAtMs < stats.oldestItemMs) {
            stats.oldestItemMs = item.enqueuedAtMs;
        }
    }

    return stats;
}

void sortSendQueue(std::vector<SendQueueItem>& queue) {
    std::sort(queue.begin(), queue.end(), [](const auto& a, const auto& b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.enqueuedAtMs < b.enqueuedAtMs;
    });
}

std::string sendQueueStatsToJson(const SendQueueStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("total": )" << stats.totalItems << ",";
    json << R"("pending": )" << stats.pendingItems << ",";
    json << R"("sending": )" << stats.sendingItems << ",";
    json << R"("failed": )" << stats.failedItems << ",";
    json << R"("oldestItemMs": )" << stats.oldestItemMs;
    json << "}";
    return json.str();
}

} // namespace progressive
