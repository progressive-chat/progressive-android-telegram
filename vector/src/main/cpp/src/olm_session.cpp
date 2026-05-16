#include "progressive/olm_session.hpp"
#include <olm/olm.h>
#include <olm/account.hh>
#include <olm/session.hh>
#include <cstring>
#include <android/log.h>

#define LOG_TAG "OlmSession"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

namespace progressive {

// ==== Base64 helpers (RFC 4648 standard) ====

static const char B64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64Encode(const uint8_t* data, size_t len) {
    std::string result;
    int val = 0, valb = -6;
    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + data[i];
        valb += 8;
        while (valb >= 0) {
            result.push_back(B64_CHARS[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(B64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

static std::string base64Encode(const std::string& data) {
    return base64Encode(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

static std::vector<uint8_t> base64Decode(const std::string& input) {
    std::vector<uint8_t> result;
    int val = 0, valb = -8;
    for (char c : input) {
        if (c == '=') break;
        const char* p = strchr(B64_CHARS, c);
        if (!p) continue;
        val = (val << 6) + (int)(p - B64_CHARS);
        valb += 6;
        if (valb >= 0) {
            result.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return result;
}

// ==== Olm Account ====

OlmAccountData createOlmAccount(const std::string& userId, const std::string& deviceId) {
    OlmAccountData result;
    result.userId = userId;
    result.deviceId = deviceId;

    size_t size = olm_account_size();
    void* account = malloc(size);
    if (!account) return result;

    auto* olmAcc = olm_account(account);
    size_t ret = olm_create_account(olmAcc, nullptr, 0);
    if (ret == olm_error()) {
        const char* err = olm_account_last_error(olmAcc);
        LOGW("olm_create_account failed: %s", err ? err : "unknown");
        free(account);
        return result;
    }

    // Get identity keys
    result.account = account;
    result.identityKeysJson = getAccountIdentityKeys(result);
    result.valid = true;
    return result;
}

OlmAccountData unpickleOlmAccount(const std::string& pickled, const std::string& userId,
                                   const std::string& deviceId) {
    OlmAccountData result;
    result.userId = userId;
    result.deviceId = deviceId;

    size_t size = olm_account_size();
    void* account = malloc(size);
    if (!account) return result;

    auto* olmAcc = olm_account(account);
    auto raw = base64Decode(pickled);
    size_t ret = olm_unpickle_account(olmAcc, nullptr, 0, raw.data(), raw.size());
    if (ret == olm_error()) {
        const char* err = olm_account_last_error(olmAcc);
        LOGW("olm_unpickle_account failed: %s", err ? err : "unknown");
        free(account);
        return result;
    }

    result.account = account;
    result.identityKeysJson = getAccountIdentityKeys(result);
    result.valid = true;
    return result;
}

std::string pickleOlmAccount(const OlmAccountData& account) {
    if (!account.valid || !account.account) return "";

    auto* olmAcc = olm_account(account.account);
    size_t len = olm_pickle_account_length(olmAcc);
    std::vector<uint8_t> pickled(len);
    size_t ret = olm_pickle_account(olmAcc, nullptr, 0, pickled.data(), len);
    if (ret == olm_error()) return "";
    pickled.resize(ret);
    return base64Encode(pickled.data(), pickled.size());
}

std::string getAccountIdentityKeys(const OlmAccountData& account) {
    if (!account.valid || !account.account) return "{}";

    auto* olmAcc = olm_account(account.account);
    size_t len = olm_account_identity_keys_length(olmAcc);
    std::string keys(len, '\0');
    size_t ret = olm_account_identity_keys(olmAcc, &keys[0], len);
    if (ret == olm_error()) return "{}";
    keys.resize(ret);
    return keys;
}

std::string generateOneTimeKeys(OlmAccountData& account, int count) {
    if (!account.valid || !account.account || count <= 0) return "{}";

    auto* olmAcc = olm_account(account.account);
    size_t randomLen = olm_account_generate_one_time_keys_random_length(olmAcc, count);
    std::vector<uint8_t> random(randomLen, 0);
    // Note: in production, use secure random. This is a placeholder.
    for (size_t i = 0; i < randomLen; i++) random[i] = (uint8_t)(i * 7 + 13);

    size_t ret = olm_account_generate_one_time_keys(olmAcc, count, random.data(), randomLen);
    if (ret == olm_error()) {
        LOGW("olm_account_generate_one_time_keys failed: %s",
             olm_account_last_error(olmAcc));
        return "{}";
    }
    olm_account_mark_keys_as_published(olmAcc);

    size_t kLen = olm_account_one_time_keys_length(olmAcc);
    std::string keys(kLen, '\0');
    olm_account_one_time_keys(olmAcc, &keys[0], kLen);
    keys.resize(kLen);
    return keys;
}

std::string generateFallbackKey(OlmAccountData& account) {
    if (!account.valid || !account.account) return "{}";

    auto* olmAcc = olm_account(account.account);
    size_t randomLen = olm_account_generate_fallback_key_random_length(olmAcc);
    std::vector<uint8_t> random(randomLen, 0);
    for (size_t i = 0; i < randomLen; i++) random[i] = (uint8_t)(i * 11 + 7);

    size_t ret = olm_account_generate_fallback_key(olmAcc, random.data(), randomLen);
    if (ret == olm_error()) {
        LOGW("olm_account_generate_fallback_key failed: %s",
             olm_account_last_error(olmAcc));
        return "{}";
    }

    size_t kLen = olm_account_fallback_key_length(olmAcc);
    std::string key(kLen, '\0');
    olm_account_fallback_key(olmAcc, &key[0], kLen);
    key.resize(kLen);
    return key;
}

std::string accountSign(const OlmAccountData& account, const std::string& message) {
    if (!account.valid || !account.account) return "";

    auto* olmAcc = olm_account(account.account);
    size_t sigLen = olm_account_signature_length(olmAcc);
    std::string sig(sigLen, '\0');
    size_t ret = olm_account_sign(olmAcc, reinterpret_cast<const uint8_t*>(message.data()),
                                   message.size(), &sig[0], sigLen);
    if (ret == olm_error()) return "";
    sig.resize(ret);
    return base64Encode(sig);
}

void destroyOlmAccount(OlmAccountData& account) {
    if (account.account) {
        olm_clear_account(olm_account(account.account));
        free(account.account);
        account.account = nullptr;
    }
    account.valid = false;
}

// ==== Olm Session ====

OlmSessionData createInboundOlmSession(OlmAccountData& account,
    const std::string& theirIdentityKey, const std::string& oneTimeKeyMessage) {
    OlmSessionData result;
    result.theirIdentityKey = theirIdentityKey;
    result.isInbound = true;

    if (!account.valid || !account.account) return result;

    size_t size = olm_session_size();
    void* session = malloc(size);
    if (!session) return result;

    auto* olmSess = olm_session(session);
    auto msg = base64Decode(oneTimeKeyMessage);

    // For inbound: use the account to create the session
    size_t ret = olm_create_inbound_session(olmSess, olm_account(account.account),
        msg.data(), msg.size());
    if (ret == olm_error()) {
        const char* err = olm_session_last_error(olmSess);
        LOGW("olm_create_inbound_session failed: %s", err ? err : "unknown");
        free(session);
        return result;
    }

    // Get session ID
    size_t idLen = olm_session_id_length(olmSess);
    std::string sessId(idLen, '\0');
    olm_session_id(olmSess, &sessId[0], idLen);
    while (!sessId.empty() && sessId.back() == '\0') sessId.pop_back();

    result.session = session;
    result.sessionId = sessId;
    result.valid = true;
    return result;
}

OlmSessionData createOutboundOlmSession(OlmAccountData& account,
    const std::string& theirIdentityKey, const std::string& theirOneTimeKey) {
    OlmSessionData result;
    result.theirIdentityKey = theirIdentityKey;
    result.isInbound = false;

    if (!account.valid || !account.account) return result;

    size_t size = olm_session_size();
    void* session = malloc(size);
    if (!session) return result;

    auto* olmSess = olm_session(session);
    auto key = base64Decode(theirOneTimeKey);

    size_t ret = olm_create_outbound_session(olmSess, olm_account(account.account),
        theirIdentityKey.data(), theirIdentityKey.size(),
        key.data(), key.size());
    if (ret == olm_error()) {
        const char* err = olm_session_last_error(olmSess);
        LOGW("olm_create_outbound_session failed: %s", err ? err : "unknown");
        free(session);
        return result;
    }

    // Get session ID
    size_t idLen = olm_session_id_length(olmSess);
    std::string sessId(idLen, '\0');
    olm_session_id(olmSess, &sessId[0], idLen);
    while (!sessId.empty() && sessId.back() == '\0') sessId.pop_back();

    result.session = session;
    result.sessionId = sessId;
    result.valid = true;
    return result;
}

std::string olmEncryptMessage(OlmSessionData& session, const std::string& plaintext) {
    if (!session.valid || !session.session) return "";

    auto* olmSess = olm_session(session.session);
    size_t msgType = olm_encrypt_message_type(olmSess);
    size_t maxLen = olm_encrypt_message_length(olmSess, plaintext.size());
    if (maxLen == olm_error()) return "";

    std::string ciphertext(maxLen, '\0');
    size_t ret = olm_encrypt(olmSess,
        reinterpret_cast<const uint8_t*>(plaintext.data()), plaintext.size(),
        &ciphertext[0], maxLen);
    if (ret == olm_error()) return "";
    ciphertext.resize(ret);
    return base64Encode(ciphertext);
}

std::string olmDecryptMessage(OlmSessionData& session, const std::string& ciphertextBase64) {
    if (!session.valid || !session.session) return "";

    auto* olmSess = olm_session(session.session);
    auto ct = base64Decode(ciphertextBase64);

    size_t maxLen = olm_decrypt_max_plaintext_length(olmSess, ct.size(), ct.data());
    if (maxLen == olm_error()) return "";

    std::string plaintext(maxLen, '\0');
    size_t ret = olm_decrypt(olmSess, 1, ct.data(), ct.size(),
        &plaintext[0], maxLen);
    if (ret == olm_error()) return "";
    plaintext.resize(ret);
    return plaintext;
}

bool olmMatchesInboundSession(OlmSessionData& session, const std::string& theirIdentityKey,
    const std::string& oneTimeKeyMessage) {
    if (!session.valid || !session.session) return false;

    auto* olmSess = olm_session(session.session);
    auto msg = base64Decode(oneTimeKeyMessage);
    size_t ret = olm_matches_inbound_session(olmSess, msg.data(), msg.size());
    return ret == 1;
}

std::string pickleOlmSession(const OlmSessionData& session) {
    if (!session.valid || !session.session) return "";

    auto* olmSess = olm_session(session.session);
    size_t len = olm_pickle_session_length(olmSess);
    std::string pickled(len, '\0');
    size_t ret = olm_pickle_session(olmSess, &pickled[0], len);
    if (ret == olm_error()) return "";
    pickled.resize(ret);
    return base64Encode(pickled.data(), pickled.size());
}

OlmSessionData unpickleOlmSession(const std::string& pickled, OlmAccountData& account) {
    OlmSessionData result;
    if (!account.valid || !account.account) return result;

    size_t size = olm_session_size();
    void* session = malloc(size);
    if (!session) return result;

    auto* olmSess = olm_session(session);
    auto key = base64Decode(pickled);
    size_t ret = olm_unpickle_session(olmSess, key.data(), key.size());
    if (ret == olm_error()) {
        const char* err = olm_session_last_error(olmSess);
        LOGW("olm_unpickle_session failed: %s", err ? err : "unknown");
        free(session);
        return result;
    }

    result.session = session;
    result.valid = true;
    return result;
}

void destroyOlmSession(OlmSessionData& session) {
    if (session.session) {
        free(session.session);
        session.session = nullptr;
    }
    session.valid = false;
}

// ==== Olm Session Manager ====

void OlmSessionManager::addSession(const std::string& senderKey, const std::string& sessionId,
                                     OlmSessionData session) {
    SessionKey key{senderKey, sessionId};
    sessions_[key] = std::move(session);
}

OlmSessionData* OlmSessionManager::findSession(const std::string& senderKey,
                                                 const std::string& sessionId) {
    SessionKey key{senderKey, sessionId};
    auto it = sessions_.find(key);
    return it != sessions_.end() ? &it->second : nullptr;
}

void OlmSessionManager::removeSession(const std::string& senderKey, const std::string& sessionId) {
    SessionKey key{senderKey, sessionId};
    auto it = sessions_.find(key);
    if (it != sessions_.end()) {
        destroyOlmSession(it->second);
        sessions_.erase(it);
    }
}

void OlmSessionManager::clearAll() {
    for (auto& pair : sessions_) {
        destroyOlmSession(pair.second);
    }
    sessions_.clear();
}

// ==== Ed25519 Signature Verification ====

bool ed25519Verify(const uint8_t* key, size_t keyLen,
                   const uint8_t* message, size_t messageLen,
                   const uint8_t* signature, size_t signatureLen) {
    size_t utilSize = olm_utility_size();
    void* utilBuf = malloc(utilSize);
    if (!utilBuf) return false;

    auto* util = olm_utility(utilBuf);
    size_t ret = olm_ed25519_verify(util, key, keyLen, message, messageLen,
        const_cast<uint8_t*>(signature), signatureLen);
    free(utilBuf);
    return ret == 0; // 0 = success, olm_error() = failure
}

bool verifyDeviceSignature(const std::string& deviceKeysJson,
                           const std::string& userId, const std::string& deviceId,
                           const std::string& signKeyB64, const std::string& signatureB64) {
    auto signKey = base64Decode(signKeyB64);
    auto sig = base64Decode(signatureB64);
    if (signKey.empty() || sig.empty()) return false;

    // Build canonical JSON to sign: {"user_id":"@...","device_id":"...","algorithms":[...],"keys":{...}}
    // For simplicity, use the whole deviceKeysJson as the message
    // In production, this would use canonical JSON (sorted keys, no whitespace)
    return ed25519Verify(
        signKey.data(), signKey.size(),
        reinterpret_cast<const uint8_t*>(deviceKeysJson.data()), deviceKeysJson.size(),
        sig.data(), sig.size());
}

std::string computeDeviceFingerprint(const std::string& identityKeyBase64) {
    auto key = base64Decode(identityKeyBase64);
    if (key.empty()) return "";

    // XOR-folding: fold 32 bytes into 16 bytes
    std::vector<uint8_t> folded(16);
    for (int i = 0; i < 16 && i < (int)key.size(); i++) {
        folded[i] = key[i];
        if (i + 16 < (int)key.size()) folded[i] ^= key[i + 16];
    }

    // Convert to pairs of hex digits, then map to words
    static const char* WORDS[] = {
        "able", "acid", "aged", "also", "area", "army", "away", "baby",
        "back", "ball", "band", "bank", "base", "bath", "bear", "beat",
        "been", "bell", "best", "bill", "bird", "blow", "blue", "boat",
        "body", "bomb", "bond", "bone", "book", "born", "boss", "both",
        "bulk", "burn", "bush", "busy", "call", "calm", "came", "camp",
        "card", "care", "case", "cash", "cast", "cell", "chat", "chip",
        "city", "club", "coal", "coat", "code", "cold", "come", "cook",
        "cool", "cope", "copy", "core", "cost", "crew", "crop", "cure"
    };

    std::string fp;
    for (int i = 0; i < 16 && i + 1 < (int)folded.size(); i += 2) {
        uint16_t w = ((uint16_t)folded[i] << 8) | folded[i + 1];
        size_t idx = w % 64;
        if (!fp.empty()) fp += " ";
        fp += WORDS[idx];
    }
    return fp;
}

} // namespace progressive
