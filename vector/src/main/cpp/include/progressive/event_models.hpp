#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "progressive/message_content.hpp"

namespace progressive {

// ==== Event Type Constants ====
//
// Original Kotlin (EventType.kt:21-142):
//   object EventType { const val MESSAGE = "m.room.message", ... }

namespace EventType {
    constexpr const char* MISSING_TYPE = "org.matrix.android.sdk.missing_type";
    constexpr const char* PRESENCE = "m.presence";
    constexpr const char* MESSAGE = "m.room.message";
    constexpr const char* STICKER = "m.sticker";
    constexpr const char* ENCRYPTED = "m.room.encrypted";
    constexpr const char* TYPING = "m.typing";
    constexpr const char* REDACTION = "m.room.redaction";
    constexpr const char* RECEIPT = "m.receipt";
    constexpr const char* ROOM_KEY = "m.room_key";

    // State events
    constexpr const char* STATE_ROOM_NAME = "m.room.name";
    constexpr const char* STATE_ROOM_TOPIC = "m.room.topic";
    constexpr const char* STATE_ROOM_AVATAR = "m.room.avatar";
    constexpr const char* STATE_ROOM_MEMBER = "m.room.member";
    constexpr const char* STATE_ROOM_CREATE = "m.room.create";
    constexpr const char* STATE_ROOM_JOIN_RULES = "m.room.join_rules";
    constexpr const char* STATE_ROOM_GUEST_ACCESS = "m.room.guest_access";
    constexpr const char* STATE_ROOM_POWER_LEVELS = "m.room.power_levels";
    constexpr const char* STATE_ROOM_TOMBSTONE = "m.room.tombstone";
    constexpr const char* STATE_ROOM_CANONICAL_ALIAS = "m.room.canonical_alias";
    constexpr const char* STATE_ROOM_HISTORY_VISIBILITY = "m.room.history_visibility";
    constexpr const char* STATE_ROOM_ENCRYPTION = "m.room.encryption";
    constexpr const char* STATE_ROOM_SERVER_ACL = "m.room.server_acl";
    constexpr const char* STATE_ROOM_PINNED_EVENT = "m.room.pinned_events";
    constexpr const char* STATE_ROOM_ALIASES = "m.room.aliases";
    constexpr const char* STATE_SPACE_CHILD = "m.space.child";
    constexpr const char* STATE_SPACE_PARENT = "m.space.parent";

    // Call events
    constexpr const char* CALL_INVITE = "m.call.invite";
    constexpr const char* CALL_CANDIDATES = "m.call.candidates";
    constexpr const char* CALL_ANSWER = "m.call.answer";
    constexpr const char* CALL_HANGUP = "m.call.hangup";
    constexpr const char* CALL_REJECT = "m.call.reject";

    // Key verification
    constexpr const char* KEY_VERIFICATION_REQUEST = "m.key.verification.request";
    constexpr const char* KEY_VERIFICATION_START = "m.key.verification.start";
    constexpr const char* KEY_VERIFICATION_ACCEPT = "m.key.verification.accept";
    constexpr const char* KEY_VERIFICATION_KEY = "m.key.verification.key";
    constexpr const char* KEY_VERIFICATION_MAC = "m.key.verification.mac";
    constexpr const char* KEY_VERIFICATION_CANCEL = "m.key.verification.cancel";
    constexpr const char* KEY_VERIFICATION_DONE = "m.key.verification.done";
    constexpr const char* KEY_VERIFICATION_READY = "m.key.verification.ready";

    // Reactions
    constexpr const char* REACTION = "m.reaction";

    // Poll (stable)
    constexpr const char* POLL_START = "m.poll.start";
    constexpr const char* POLL_RESPONSE = "m.poll.response";
    constexpr const char* POLL_END = "m.poll.end";

    // Beacon
    constexpr const char* STATE_ROOM_BEACON_INFO = "m.beacon_info";
    constexpr const char* BEACON_LOCATION_DATA = "m.beacon";

    // Element Call
    constexpr const char* ELEMENT_CALL_NOTIFY = "m.call.notify";

    // Key share
    constexpr const char* ROOM_KEY_REQUEST = "m.room_key_request";
    constexpr const char* FORWARDED_ROOM_KEY = "m.forwarded_room_key";

    inline bool isCallEvent(const std::string& type) {
        return type == CALL_INVITE || type == CALL_CANDIDATES || type == CALL_ANSWER
            || type == CALL_HANGUP || type == CALL_REJECT;
    }
}

// ==== Relation Type Constants ====
//
// Original Kotlin (RelationType.kt:22-37):
//   object RelationType { const val ANNOTATION = "m.annotation", ... }

namespace RelationType {
    constexpr const char* ANNOTATION = "m.annotation";
    constexpr const char* REPLACE = "m.replace";
    constexpr const char* REFERENCE = "m.reference";
    constexpr const char* THREAD = "m.thread";
    constexpr const char* RESPONSE = "org.matrix.response";
}

// ==== Event Model ====
//
// Original Kotlin (Event.kt:83-105):
//   data class Event(
//       @Json(name="type") type, @Json(name="event_id") eventId,
//       @Json(name="content") content, @Json(name="prev_content") prevContent,
//       @Json(name="origin_server_ts") originServerTs, @Json(name="sender") senderId,
//       @Json(name="state_key") stateKey, @Json(name="room_id") roomId,
//       @Json(name="unsigned") unsignedData, @Json(name="redacts") redacts
//   )

// Send state for local echo messages
enum class SendState {
    UNKNOWN = 0,
    SENDING = 1,
    SENT = 2,
    FAILED_UNKNOWN_DEVICES = 3,
    UNDELIVERABLE = 4
};

struct OlmDecryptionResult {
    std::string senderKey;
    std::unordered_map<std::string, std::string> keysClaimed;
    std::unordered_map<std::string, std::string> payload; // parsed JSON
};

struct RelationChunkInfo {
    std::string type;        // "m.reaction"
    std::string key;         // "👍"
    int count = 0;
};

struct AggregatedRelations {
    std::vector<RelationChunkInfo> chunks;
};

struct UnsignedData {
    int64_t age = 0;                     // "age" key — milliseconds since event was sent
    std::string redactedEventId;         // "redacted_because" event_id
    std::string redactedSenderId;        // sender of the redaction event
    std::string transactionId;           // "transaction_id" key
    std::string prevContentJson;         // "prev_content" key — raw JSON
    AggregatedRelations relations;       // "m.relations" key
    std::string replacesState;           // "replaces_state" key

    bool isRedacted() const { return !redactedEventId.empty(); }
};

struct Event {
    std::string type;                    // "type" key — e.g. "m.room.message"
    std::string eventId;                 // "event_id" key
    std::string contentJson;             // "content" key — raw JSON
    std::string prevContentJson;         // "prev_content" key — raw JSON
    int64_t originServerTs = 0;          // "origin_server_ts" key
    std::string senderId;                // "sender" key
    std::string stateKey;                // "state_key" key — null for non-state events
    std::string roomId;                  // "room_id" key
    UnsignedData unsignedData;           // "unsigned" key
    std::string redacts;                 // "redacts" key — event ID being redacted

    // Transient fields (not in JSON wire format)
    // Original Kotlin: @Transient annotations
    OlmDecryptionResult decryptionResult; // mxDecryptionResult
    std::string cryptoError;             // mCryptoError
    std::string cryptoErrorReason;       // mCryptoErrorReason
    SendState sendState = SendState::UNKNOWN;
    int64_t ageLocalTs = 0;              // local timestamp when event received

    // Original Kotlin: isStateEvent() = stateKey != null
    bool isStateEvent() const { return !stateKey.empty(); }

    // Original Kotlin: isEncrypted() = type == EventType.ENCRYPTED
    bool isEncrypted() const { return type == "m.room.encrypted"; }

    // Original Kotlin: getClearType() = getDecryptedType() ?: type ?: MISSING_TYPE
    std::string getClearType() const {
        if (decryptionResult.payload.count("type")) {
            return decryptionResult.payload.at("type");
        }
        if (!type.empty()) return type;
        return "org.matrix.android.sdk.missing_type";
    }

    // Original Kotlin: isRedacted() = unsignedData?.redactedEvent != null
    bool isRedacted() const { return unsignedData.isRedacted(); }

    // Original Kotlin: getSenderKey()
    std::string getSenderKey() const { return decryptionResult.senderKey; }
};

// ==== Encrypted Event Content ====
//
// Original Kotlin (EncryptedEventContent.kt:25-52):
//   data class EncryptedEventContent(
//       algorithm, ciphertext, deviceId, senderKey, sessionId, relatesTo
//   )

struct EncryptedEventContent {
    std::string algorithm;       // "algorithm" key — "m.megolm.v1.aes-sha2"
    std::string ciphertext;      // "ciphertext" key
    std::string deviceId;        // "device_id" key
    std::string senderKey;       // "sender_key" key
    std::string sessionId;       // "session_id" key
    RelationDefaultContent relatesTo; // "m.relates_to" key
};

// Original Kotlin (EncryptionEventContent.kt:24-42):
//   data class EncryptionEventContent(algorithm, rotationPeriodMs, rotationPeriodMsgs)

struct EncryptionEventContent {
    std::string algorithm;              // "algorithm" key — must be "m.megolm.v1.aes-sha2"
    int64_t rotationPeriodMs = 604800000;    // default: 1 week
    int64_t rotationPeriodMsgs = 100;        // default: 100 messages
};

// Original Kotlin (OlmEventContent.kt:23-34):
//   data class OlmEventContent(ciphertext: Map<String, Any>?, senderKey)

struct OlmEventContent {
    std::string ciphertextJson;  // "ciphertext" key — raw JSON object
    std::string senderKey;       // "sender_key" key
};

// Original Kotlin (OlmPayloadContent.kt:24-50):
//   data class OlmPayloadContent(roomId, sender, recipient, recipientKeys, keys)

struct OlmPayloadContent {
    std::string roomId;
    std::string sender;
    std::string recipient;
    std::unordered_map<std::string, std::string> recipientKeys;
    std::unordered_map<std::string, std::string> keys;
};

// ==== Room Key Content ====
//
// Original Kotlin (RoomKeyContent.kt:24-49):
//   data class RoomKeyContent(algorithm, roomId, sessionId, sessionKey, chainIndex, sharedHistory)

struct RoomKeyContent {
    std::string algorithm;       // "algorithm" key
    std::string roomId;          // "room_id" key
    std::string sessionId;       // "session_id" key
    std::string sessionKey;      // "session_key" key
    int64_t chainIndex = 0;      // "chain_index" key
    bool sharedHistory = false;  // "org.matrix.msc3061.shared_history" key
};

// ==== Room Key Withheld ====
//
// Original Kotlin (RoomKeyWithHeldContent.kt:24-64):
//   data class RoomKeyWithHeldContent(roomId, algorithm, sessionId, senderKey, codeString, reason, fromDevice)

enum class WithHeldCode {
    BLACKLISTED = 0,   // "m.blacklisted"
    UNVERIFIED = 1,    // "m.unverified"
    UNAUTHORISED = 2,  // "m.unauthorised"
    UNAVAILABLE = 3,   // "m.unavailable"
    NO_OLM = 4         // "m.no_olm"
};

const char* withHeldCodeToString(WithHeldCode code);
WithHeldCode withHeldCodeFromString(const std::string& s);

struct RoomKeyWithHeldContent {
    std::string roomId;
    std::string algorithm;
    std::string sessionId;
    std::string senderKey;
    WithHeldCode code = WithHeldCode::UNAVAILABLE;
    std::string reason;
    std::string fromDevice;
};

// ==== Secret Send ====
//
// Original Kotlin (SecretSendEventContent.kt:24-27):
//   data class SecretSendEventContent(requestId, secretValue)

struct SecretSendEventContent {
    std::string requestId;       // "request_id" key
    std::string secretValue;     // "secret" key
};

// ==== Event Utility Functions ====
//
// Original Kotlin (Event.kt extension functions)

inline bool isTextMessage(const std::string& msgtype) {
    return msgtype == "m.text" || msgtype == "m.emote" || msgtype == "m.notice";
}

inline bool isImageMessage(const std::string& msgtype) { return msgtype == "m.image"; }
inline bool isVideoMessage(const std::string& msgtype) { return msgtype == "m.video"; }
inline bool isAudioMessage(const std::string& msgtype) { return msgtype == "m.audio"; }
inline bool isFileMessage(const std::string& msgtype) { return msgtype == "m.file"; }
inline bool isLocationMessage(const std::string& msgtype) { return msgtype == "m.location"; }

inline bool isAttachmentMessage(const std::string& msgtype) {
    return msgtype == "m.image" || msgtype == "m.audio"
        || msgtype == "m.video" || msgtype == "m.file";
}

inline bool isSticker(const std::string& type) { return type == "m.sticker"; }
inline bool isPollStart(const std::string& type) { return type == "m.poll.start"; }
inline bool isPollEnd(const std::string& type) { return type == "m.poll.end"; }
inline bool isPoll(const std::string& type) { return isPollStart(type) || isPollEnd(type); }

// ==== Server-side Aggregation ====
//
// Original Kotlin (AggregatedRelations.kt:28-44):
//   data class AggregatedRelations(annotations, references, replaces, latestThread)

struct UnsignedRelationInfo {
    bool limited = false;                // "limited" key
    int count = 0;                       // "count" key
};

struct AggregatedAnnotation : UnsignedRelationInfo {
    std::vector<RelationChunkInfo> chunk; // "chunk" key — reaction counts
};

struct DefaultUnsignedRelationInfo : UnsignedRelationInfo {
    std::vector<Event> chunk;            // "chunk" key — related events
};

// Original Kotlin (AggregatedReplace.kt:28-32):
//   data class AggregatedReplace(eventId, originServerTs, senderId)
struct AggregatedReplace {
    std::string eventId;                 // "event_id" key — latest edit
    int64_t originServerTs = 0;          // "origin_server_ts" key
    std::string senderId;                // "sender" key
};

// Extended AggregatedRelations (full struct, not just chunks)
struct AggregatedRelationsFull {
    AggregatedAnnotation annotations;    // "m.annotation" key
    DefaultUnsignedRelationInfo references; // "m.reference" key
    AggregatedReplace replaces;          // "m.replace" key
    LatestThreadUnsignedRelation latestThread; // "m.thread" key
};

// ==== Local Echo ====
//
// Original Kotlin (LocalEcho.kt:22-26):
//   object LocalEcho {
//       fun isLocalEchoId(eventId: String) = eventId.startsWith("\$local.")
//       fun createLocalEchoId() = "\$local.${UUID.randomUUID()}"
//   }

constexpr const char* LOCAL_ECHO_PREFIX = "$local.";

inline bool isLocalEchoId(const std::string& eventId) {
    return eventId.find(LOCAL_ECHO_PREFIX) == 0;
}

// ==== Stable/Unstable ID ====
//
// Original Kotlin (StableUnstableId.kt:21-25):
//   data class StableUnstableId(stable: String, unstable: String)
//   val values = listOf(stable, unstable)

struct StableUnstableId {
    std::string stable;
    std::string unstable;

    std::vector<std::string> values() const {
        return {stable, unstable};
    }

    bool matches(const std::string& type) const {
        return type == stable || type == unstable;
    }
};

// ==== Latest Thread Unsigned Relation ====
//
// Original Kotlin (LatestThreadUnsignedRelation.kt:25-34):
//   data class LatestThreadUnsignedRelation : UnsignedRelationInfo(
//       limited, count,
//       @Json(name="latest_event") event: Event?,
//       @Json(name="current_user_participated") isUserParticipating: Boolean?
//   )

struct LatestThreadUnsignedRelation : UnsignedRelationInfo {
    Event latestEvent;                       // "latest_event" key
    bool isUserParticipating = false;        // "current_user_participated" key
};

// ==== Valid Decrypted Event ====
//
// Original Kotlin (ValidDecryptedEvent.kt:28-38):
//   data class ValidDecryptedEvent(type, eventId, clearContent, prevContent,
//       originServerTs, cryptoSenderKey, roomId, unsignedData, redacts, algorithm)

struct ValidDecryptedEvent {
    std::string type;
    std::string eventId;
    std::string clearContentJson;            // decrypted content JSON
    std::string prevContentJson;
    int64_t originServerTs = 0;
    std::string cryptoSenderKey;             // curve25519 sender key
    std::string roomId;
    UnsignedData unsignedData;
    std::string redacts;
    std::string algorithm;                   // e.g. "m.megolm.v1.aes-sha2"

    // Original Kotlin (EventExt.kt:21-41): fun Event.toValidDecryptedEvent()
    static ValidDecryptedEvent fromEncryptedEvent(const Event& ev, const std::string& decryptedContentJson);
};

// ==== Reaction Aggregated Summary ====
//
// Original Kotlin (ReactionAggregatedSummary.kt:21-29):
//   data class ReactionAggregatedSummary(key, count, addedByMe,
//       firstTimestamp, sourceEvents, localEchoEvents)

struct ReactionAggregatedSummary {
    std::string key;                         // "👍"
    int count = 0;
    bool addedByMe = false;
    int64_t firstTimestamp = 0;
    std::vector<std::string> sourceEvents;   // event IDs
    std::vector<std::string> localEchoEvents;
};

// ==== References Aggregated Content ====
//
// Original Kotlin (ReferencesAggregatedContent.kt:28-32):
//   data class ReferencesAggregatedContent(verificationState: VerificationState)

struct ReferencesAggregatedContent {
    int verificationState = 0;               // VerificationState enum value
};

// ==== Room Local Echo ====
//
// Original Kotlin (RoomLocalEcho.kt:23-31):
//   object RoomLocalEcho { const val PREFIX = "!local." }
//   fun isLocalEchoId(roomId) = roomId.startsWith(PREFIX)
//   fun createLocalEchoId() = "${PREFIX}${UUID.randomUUID()}"

constexpr const char* ROOM_LOCAL_ECHO_PREFIX = "!local.";

inline bool isRoomLocalEchoId(const std::string& roomId) {
    return roomId.find(ROOM_LOCAL_ECHO_PREFIX) == 0;
}

// ==== JSON Parsing ====

Event parseEvent(const std::string& eventJson);
UnsignedData parseUnsignedData(const std::string& json);
EncryptedEventContent parseEncryptedEventContent(const std::string& json);
EncryptionEventContent parseEncryptionEventContent(const std::string& json);
RoomKeyContent parseRoomKeyContent(const std::string& json);
RoomKeyWithHeldContent parseRoomKeyWithHeldContent(const std::string& json);
SecretSendEventContent parseSecretSendEventContent(const std::string& json);

std::string eventToJson(const Event& ev);

} // namespace progressive
