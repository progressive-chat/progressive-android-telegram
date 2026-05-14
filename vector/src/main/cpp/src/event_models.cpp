#include "progressive/event_models.hpp"

namespace progressive {

// ==== WithHeldCode ====

const char* withHeldCodeToString(WithHeldCode code) {
    switch (code) {
        case WithHeldCode::BLACKLISTED: return "m.blacklisted";
        case WithHeldCode::UNVERIFIED: return "m.unverified";
        case WithHeldCode::UNAUTHORISED: return "m.unauthorised";
        case WithHeldCode::UNAVAILABLE: return "m.unavailable";
        case WithHeldCode::NO_OLM: return "m.no_olm";
    }
    return "m.unavailable";
}

WithHeldCode withHeldCodeFromString(const std::string& s) {
    if (s == "m.blacklisted") return WithHeldCode::BLACKLISTED;
    if (s == "m.unverified") return WithHeldCode::UNVERIFIED;
    if (s == "m.unauthorised") return WithHeldCode::UNAUTHORISED;
    if (s == "m.unavailable") return WithHeldCode::UNAVAILABLE;
    if (s == "m.no_olm") return WithHeldCode::NO_OLM;
    return WithHeldCode::UNAVAILABLE;
}

// ==== JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// ==== Parse UnsignedData ====
//
// Original Kotlin (UnsignedData.kt:25-47)

UnsignedData parseUnsignedData(const std::string& json) {
    UnsignedData u;
    u.age = extractJsonInt64(json, "age");
    u.transactionId = extractJsonString(json, "transaction_id");
    u.replacesState = extractJsonString(json, "replaces_state");

    // redacted_because — parse only event_id and sender
    auto redactedJson = extractJsonObject(json, "redacted_because");
    if (!redactedJson.empty()) {
        u.redactedEventId = extractJsonString(redactedJson, "event_id");
        u.redactedSenderId = extractJsonString(redactedJson, "sender");
    }

    u.prevContentJson = extractJsonObject(json, "prev_content");

    // Relations
    auto relJson = extractJsonObject(json, "m.relations");
    if (!relJson.empty()) {
        // Parse chunked annotations/reactions
        size_t pos = 1;
        while (pos < relJson.size()) {
            while (pos < relJson.size() && (relJson[pos] == ' ' || relJson[pos] == ',')) pos++;
            if (pos >= relJson.size() || relJson[pos] == '}') break;
            if (relJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < relJson.size() && relJson[keyEnd] != '"') keyEnd++;
                std::string relationType = relJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < relJson.size() && relJson[pos] != ':') pos++;
                pos++;
                while (pos < relJson.size() && (relJson[pos] == ' ' || relJson[pos] == '\t')) pos++;
                // The value is an array of chunks
                if (pos < relJson.size() && relJson[pos] == '{') {
                    // Single chunk
                    int d = 1;
                    size_t cs = pos;
                    pos++;
                    while (pos < relJson.size() && d > 0) {
                        if (relJson[pos] == '{') d++;
                        else if (relJson[pos] == '}') d--;
                        pos++;
                    }
                    std::string chunkJson = relJson.substr(cs, pos - cs);
                    RelationChunkInfo chunk;
                    chunk.type = relationType;
                    chunk.key = extractJsonString(chunkJson, "key");
                    chunk.count = static_cast<int>(extractJsonInt64(chunkJson, "count"));
                    u.relations.chunks.push_back(chunk);
                }
            }
        }
    }

    return u;
}

// ==== Parse Event ====
//
// Original Kotlin (Event.kt:83-105)

Event parseEvent(const std::string& json) {
    Event ev;
    ev.type = extractJsonString(json, "type");
    ev.eventId = extractJsonString(json, "event_id");
    ev.senderId = extractJsonString(json, "sender");
    ev.stateKey = extractJsonString(json, "state_key");
    ev.roomId = extractJsonString(json, "room_id");
    ev.redacts = extractJsonString(json, "redacts");
    ev.originServerTs = extractJsonInt64(json, "origin_server_ts");

    // content — extract raw JSON object
    ev.contentJson = extractJsonObject(json, "content");

    // prev_content
    ev.prevContentJson = extractJsonObject(json, "prev_content");

    // unsigned
    auto unsignedJson = extractJsonObject(json, "unsigned");
    if (!unsignedJson.empty()) {
        ev.unsignedData = parseUnsignedData(unsignedJson);
    }

    return ev;
}

// ==== Parse EncryptedEventContent ====
//
// Original Kotlin (EncryptedEventContent.kt:25-52)

EncryptedEventContent parseEncryptedEventContent(const std::string& json) {
    EncryptedEventContent c;
    c.algorithm = extractJsonString(json, "algorithm");
    c.ciphertext = extractJsonString(json, "ciphertext");
    c.deviceId = extractJsonString(json, "device_id");
    c.senderKey = extractJsonString(json, "sender_key");
    c.sessionId = extractJsonString(json, "session_id");

    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.relatesTo.relationType = extractJsonString(relJson, "rel_type");
    }

    return c;
}

// ==== Parse EncryptionEventContent ====
//
// Original Kotlin (EncryptionEventContent.kt:24-42)

EncryptionEventContent parseEncryptionEventContent(const std::string& json) {
    EncryptionEventContent c;
    c.algorithm = extractJsonString(json, "algorithm");
    c.rotationPeriodMs = extractJsonInt64(json, "rotation_period_ms");
    if (c.rotationPeriodMs == 0) c.rotationPeriodMs = 604800000;
    c.rotationPeriodMsgs = extractJsonInt64(json, "rotation_period_msgs");
    if (c.rotationPeriodMsgs == 0) c.rotationPeriodMsgs = 100;
    return c;
}

// ==== Parse RoomKeyContent ====
//
// Original Kotlin (RoomKeyContent.kt:24-49)

RoomKeyContent parseRoomKeyContent(const std::string& json) {
    RoomKeyContent k;
    k.algorithm = extractJsonString(json, "algorithm");
    k.roomId = extractJsonString(json, "room_id");
    k.sessionId = extractJsonString(json, "session_id");
    k.sessionKey = extractJsonString(json, "session_key");
    k.chainIndex = extractJsonInt64(json, "chain_index");
    k.sharedHistory = extractJsonBool(json, "org.matrix.msc3061.shared_history");
    return k;
}

// ==== Parse RoomKeyWithHeldContent ====
//
// Original Kotlin (RoomKeyWithHeldContent.kt:24-64)

RoomKeyWithHeldContent parseRoomKeyWithHeldContent(const std::string& json) {
    RoomKeyWithHeldContent k;
    k.roomId = extractJsonString(json, "room_id");
    k.algorithm = extractJsonString(json, "algorithm");
    k.sessionId = extractJsonString(json, "session_id");
    k.senderKey = extractJsonString(json, "sender_key");
    k.reason = extractJsonString(json, "reason");
    k.fromDevice = extractJsonString(json, "from_device");
    k.code = withHeldCodeFromString(extractJsonString(json, "code"));
    return k;
}

// ==== Parse SecretSendEventContent ====
//
// Original Kotlin (SecretSendEventContent.kt:24-27)

SecretSendEventContent parseSecretSendEventContent(const std::string& json) {
    SecretSendEventContent s;
    s.requestId = extractJsonString(json, "request_id");
    s.secretValue = extractJsonString(json, "secret");
    return s;
}

// ==== Serialize Event ====

std::string eventToJson(const Event& ev) {
    std::string json = "{";
    json += "\"type\":\"" + ev.type + "\"";
    if (!ev.eventId.empty()) json += ",\"event_id\":\"" + ev.eventId + "\"";
    if (!ev.senderId.empty()) json += ",\"sender\":\"" + ev.senderId + "\"";
    if (!ev.roomId.empty()) json += ",\"room_id\":\"" + ev.roomId + "\"";
    if (!ev.stateKey.empty()) json += ",\"state_key\":\"" + ev.stateKey + "\"";
    if (ev.originServerTs > 0) json += ",\"origin_server_ts\":" + std::to_string(ev.originServerTs);
    if (!ev.contentJson.empty()) json += ",\"content\":" + ev.contentJson;
    if (!ev.prevContentJson.empty()) json += ",\"prev_content\":" + ev.prevContentJson;
    if (!ev.redacts.empty()) json += ",\"redacts\":\"" + ev.redacts + "\"";
    json += "}";
    return json;
}

// ==== Valid Decrypted Event ====
//
// Original Kotlin (EventExt.kt:21-41): fun Event.toValidDecryptedEvent()
//   Copies relation context from encrypted content into decrypted content.

ValidDecryptedEvent ValidDecryptedEvent::fromEncryptedEvent(
    const Event& ev, const std::string& decryptedContentJson)
{
    ValidDecryptedEvent vd;
    vd.type = ev.getClearType();
    vd.eventId = ev.eventId;
    vd.roomId = ev.roomId;
    vd.originServerTs = ev.originServerTs;
    vd.cryptoSenderKey = ev.getSenderKey();
    vd.unsignedData = ev.unsignedData;
    vd.redacts = ev.redacts;
    vd.prevContentJson = ev.prevContentJson;

    // Original Kotlin: algorithm from content["algorithm"]
    vd.algorithm = extractJsonString(ev.contentJson, "algorithm");

    // Original Kotlin: copy m.relates_to from encrypted content to decrypted
    // val updatedContent = content.get("m.relates_to")?.let {
    //     decryptedContent.toMutableMap().apply { put("m.relates_to", it) }
    // } ?: decryptedContent
    auto relJson = extractJsonObject(ev.contentJson, "m.relates_to");
    if (!relJson.empty()) {
        // Insert m.relates_to into decrypted content
        if (!decryptedContentJson.empty() && decryptedContentJson.size() > 1) {
            std::string updated = decryptedContentJson;
            updated.pop_back(); // remove closing }
            if (updated.back() != '{') updated += ",";
            updated += "\"m.relates_to\":" + relJson + "}";
            vd.clearContentJson = updated;
        } else {
            vd.clearContentJson = "{\"m.relates_to\":" + relJson + "}";
        }
    } else {
        vd.clearContentJson = decryptedContentJson;
    }

    return vd;
}

} // namespace progressive
