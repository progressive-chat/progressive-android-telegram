#include "progressive/olm.hpp"
#include <sstream>
#include <cstring>

// libolm C API headers (from https://gitlab.matrix.org/matrix-org/olm)
// To enable, add libolm to CMakeLists.txt:
//   add_subdirectory(libolm)
//   target_link_libraries(progressive_native olm)
//
// Then uncomment the includes below:
// #include <olm/olm.h>
// #include <olm/outbound_group_session.h>
// #include <olm/inbound_group_session.h>
// #include <olm/sas.h>

namespace progressive {

// Forward-declare libolm C functions (will be resolved when libolm is linked)
extern "C" {
    // OlmAccount
    typedef struct OlmAccount OlmAccountC;
    size_t olm_account_size(void);
    OlmAccountC* olm_account(void* memory);
    size_t olm_clear_account(OlmAccountC* account);
    size_t olm_create_account_random_length(OlmAccountC* account);
    int olm_create_account(OlmAccountC* account, const void* random, size_t random_length);
    size_t olm_pickle_account_length(OlmAccountC* account);
    size_t olm_pickle_account(OlmAccountC* account, const void* key, size_t key_len, void* pickled, size_t pickled_len);
    size_t olm_unpickle_account(OlmAccountC* account, const void* key, size_t key_len, const void* pickled, size_t pickled_len);
    size_t olm_account_identity_keys_length(OlmAccountC* account);
    size_t olm_account_identity_keys(OlmAccountC* account, void* out, size_t out_length);
    size_t olm_account_one_time_keys_length(OlmAccountC* account);
    size_t olm_account_one_time_keys(OlmAccountC* account, void* out, size_t out_length);
    size_t olm_account_generate_one_time_keys_random_length(OlmAccountC* account, size_t count);
    size_t olm_account_generate_one_time_keys(OlmAccountC* account, size_t count, const void* random, size_t random_len);
    size_t olm_account_sign_random_length(OlmAccountC* account);
    size_t olm_account_sign(OlmAccountC* account, const void* message, size_t msg_len, void* sig, size_t sig_len);
    size_t olm_account_last_error(OlmAccountC* account, const char** error);
    size_t olm_account_max_number_of_one_time_keys(OlmAccountC* account);

    // OlmSession
    typedef struct OlmSession OlmSessionC;
    size_t olm_session_size(void);
    OlmSessionC* olm_session(void* memory);
    size_t olm_clear_session(OlmSessionC* session);
    size_t olm_create_outbound_session_random_length(OlmSessionC* session);
    size_t olm_create_outbound_session(OlmSessionC* sess, OlmAccountC* acc,
        const void* id_key, size_t id_key_len, const void* one_time_key, size_t otk_len,
        const void* random, size_t random_len);
    size_t olm_create_inbound_session_random_length(OlmSessionC* session);
    size_t olm_create_inbound_session(OlmSessionC* sess, OlmAccountC* acc, const void* pre_key_msg, size_t msg_len,
        const void* random, size_t random_len);
    size_t olm_create_inbound_session_from(OlmSessionC* sess, OlmAccountC* acc,
        const void* id_key, size_t id_key_len, const void* enc_msg, size_t msg_len);
    size_t olm_encrypt_message_length(OlmSessionC* sess, size_t plaintext_len);
    size_t olm_encrypt(OlmSessionC* sess, const void* plaintext, size_t pt_len, void* message, size_t msg_len);
    size_t olm_decrypt_max_plaintext_length(OlmSessionC* sess, int msg_type, const void* message, size_t msg_len);
    size_t olm_decrypt(OlmSessionC* sess, int msg_type, const void* message, size_t msg_len, void* plaintext, size_t pt_len);
    size_t olm_pickle_session(OlmSessionC* sess, const void* key, size_t key_len, void* pickled, size_t pickled_len);
    size_t olm_pickle_session_length(OlmSessionC* sess);
    size_t olm_unpickle_session(OlmSessionC* sess, const void* key, size_t key_len, const void* pickled, size_t pickled_len);
    size_t olm_matches_inbound_session(OlmSessionC* sess, const void* pre_key_msg, size_t msg_len);
    size_t olm_session_id_length(OlmSessionC* sess);
    size_t olm_session_id(OlmSessionC* sess, void* id, size_t id_len);
    size_t olm_session_last_error(OlmSessionC* sess, const char** error);
    int olm_encrypt_message_type(OlmSessionC* session);

    // Megolm Outbound
    typedef struct OlmOutboundGroupSession OlmOutboundGroupSessionC;
    size_t olm_outbound_group_session_size(void);
    OlmOutboundGroupSessionC* olm_outbound_group_session(void* memory);
    size_t olm_clear_outbound_group_session(OlmOutboundGroupSessionC* sess);
    size_t olm_init_outbound_group_session_random_length(OlmOutboundGroupSessionC* sess);
    size_t olm_init_outbound_group_session(OlmOutboundGroupSessionC* sess, const void* random, size_t random_len);
    size_t olm_outbound_group_session_message_index(OlmOutboundGroupSessionC* sess);
    int olm_outbound_group_session_check_has_message_index(OlmOutboundGroupSessionC* sess, int index);
    size_t olm_group_encrypt_message_length(OlmOutboundGroupSessionC* sess, size_t pt_len);
    size_t olm_group_encrypt(OlmOutboundGroupSessionC* sess, const void* pt, size_t pt_len, void* msg, size_t msg_len);
    size_t olm_outbound_group_session_id_length(OlmOutboundGroupSessionC* sess);
    size_t olm_outbound_group_session_id(OlmOutboundGroupSessionC* sess, void* id, size_t id_len);
    size_t olm_outbound_group_session_key_length(OlmOutboundGroupSessionC* sess);
    size_t olm_outbound_group_session_key(OlmOutboundGroupSessionC* sess, void* key, size_t key_len);
    size_t olm_pickle_outbound_group_session(OlmOutboundGroupSessionC* sess, const void* key, size_t key_len, void* pickled, size_t pickled_len);
    size_t olm_pickle_outbound_group_session_length(OlmOutboundGroupSessionC* sess);
    size_t olm_unpickle_outbound_group_session(OlmOutboundGroupSessionC* sess, const void* key, size_t key_len, const void* pickled, size_t pickled_len);
    size_t olm_outbound_group_session_last_error(OlmOutboundGroupSessionC* sess, const char** error);

    // Megolm Inbound
    typedef struct OlmInboundGroupSession OlmInboundGroupSessionC;
    size_t olm_inbound_group_session_size(void);
    OlmInboundGroupSessionC* olm_inbound_group_session(void* memory);
    size_t olm_clear_inbound_group_session(OlmInboundGroupSessionC* sess);
    size_t olm_init_inbound_group_session(OlmInboundGroupSessionC* sess, int message_index, const void* session_key, size_t key_len);
    size_t olm_import_inbound_group_session(OlmInboundGroupSessionC* sess, const void* session_key, size_t key_len);
    size_t olm_export_inbound_group_session_length(OlmInboundGroupSessionC* sess);
    size_t olm_export_inbound_group_session(OlmInboundGroupSessionC* sess, void* key, size_t key_len, int message_index);
    size_t olm_group_decrypt_max_plaintext_length(OlmInboundGroupSessionC* sess, const void* message, size_t msg_len);
    size_t olm_group_decrypt(OlmInboundGroupSessionC* sess, const void* msg, size_t msg_len, void* pt, size_t pt_len, int* message_index);
    size_t olm_inbound_group_session_first_known_index(OlmInboundGroupSessionC* sess);
    int olm_inbound_group_session_is_verified(OlmInboundGroupSessionC* sess);
    size_t olm_pickle_inbound_group_session(OlmInboundGroupSessionC* sess, const void* key, size_t key_len, void* pickled, size_t pickled_len);
    size_t olm_pickle_inbound_group_session_length(OlmInboundGroupSessionC* sess);
    size_t olm_unpickle_inbound_group_session(OlmInboundGroupSessionC* sess, const void* key, size_t key_len, const void* pickled, size_t pickled_len);
    size_t olm_inbound_group_session_id_length(OlmInboundGroupSessionC* sess);
    size_t olm_inbound_group_session_id(OlmInboundGroupSessionC* sess, void* id, size_t id_len);
    size_t olm_inbound_group_session_last_error(OlmInboundGroupSessionC* sess, const char** error);
}

// ---- Utility ----

std::string generateRandomBytes(int count) {
    std::string result(count, 0);
    // Use libolm's RNG or fallback to system
    for (int i = 0; i < count; ++i) {
        result[i] = static_cast<char>(rand() % 256);
    }
    return result;
}

std::string olmErrorToString(OlmError error) {
    switch (error) {
        case OlmError::None: return "No error";
        case OlmError::NotEnoughRandom: return "Not enough random data";
        case OlmError::OutputBufferTooSmall: return "Output buffer too small";
        case OlmError::BadMessageVersion: return "Bad message version";
        case OlmError::BadMessageFormat: return "Bad message format";
        case OlmError::BadMessageMac: return "Bad message MAC";
        case OlmError::BadMessageKeyId: return "Bad message key ID";
        case OlmError::InvalidBase64: return "Invalid base64";
        case OlmError::BadAccountKey: return "Bad account key";
        case OlmError::UnknownPickleVersion: return "Unknown pickle version";
        case OlmError::Corruption: return "Data corruption";
        case OlmError::SessionNotFound: return "Session not found";
        default: return "Unknown error";
    }
}

std::string formatPickle(const std::string& type, const std::string& pickle) {
    std::ostringstream out;
    out << type << ":" << pickle;
    return out.str();
}

// ---- OlmAccount ----

OlmAccount::OlmAccount() {
    size_t sz = olm_account_size();
    account_ = new uint8_t[sz];
    memset(account_, 0, sz);
    olm_account(account_);
}

OlmAccount::~OlmAccount() {
    olm_clear_account(static_cast<OlmAccountC*>(account_));
    delete[] static_cast<uint8_t*>(account_);
}

OlmAccountResult OlmAccount::create() {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t randLen = olm_create_account_random_length(acc);
    auto random = generateRandomBytes(randLen);
    int rc = olm_create_account(acc, random.data(), random.size());
    if (rc == -1) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::UnknownError;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::identityKeys() {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t len = olm_account_identity_keys_length(acc);
    std::string out(len, 0);
    size_t written = olm_account_identity_keys(acc, &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::generateOneTimeKeys(int count) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t randLen = olm_account_generate_one_time_keys_random_length(acc, count);
    auto random = generateRandomBytes(randLen);
    int rc = olm_account_generate_one_time_keys(acc, count, random.data(), random.size());
    if (rc == -1) {
        result.error = OlmError::NotEnoughRandom;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::sign(const std::string& message) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t sigLen = olm_account_signature_length(acc);
    std::string sig(sigLen, 0);
    size_t written = olm_account_sign(acc, message.data(), message.size(), &sig[0], sigLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    sig.resize(written);
    result.success = true;
    result.data = sig;
    return result;
}

OlmAccountResult OlmAccount::pickle(const std::string& key) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    size_t len = olm_pickle_account_length(acc);
    std::string out(len, 0);
    size_t written = olm_pickle_account(acc, key.data(), key.size(), &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::UnknownPickleVersion;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::unpickle(const std::string& key, const std::string& pickle) {
    OlmAccountResult result;
    auto* acc = static_cast<OlmAccountC*>(account_);
    int rc = olm_unpickle_account(acc, key.data(), key.size(), pickle.data(), pickle.size());
    if (rc == -1) {
        const char* err;
        olm_account_last_error(acc, &err);
        result.error = OlmError::BadAccountKey;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::ed25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    // Parse {"ed25519":"xxxx"} from JSON
    auto pos = keys.data.find("\"ed25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 12;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

OlmAccountResult OlmAccount::curve25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    auto pos = keys.data.find("\"curve25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 15;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

int OlmAccount::maxOneTimeKeys() {
    auto* acc = static_cast<OlmAccountC*>(account_);
    return olm_account_max_number_of_one_time_keys(acc);
}

// ---- OlmSession ----

OlmSession::OlmSession() {
    size_t sz = olm_session_size();
    session_ = new uint8_t[sz];
    memset(session_, 0, sz);
    olm_session(session_);
}

OlmSession::~OlmSession() {
    olm_clear_session(static_cast<OlmSessionC*>(session_));
    delete[] static_cast<uint8_t*>(session_);
}

OlmSessionResult OlmSession::createOutbound(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& theirOneTimeKey) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    size_t randLen = olm_create_outbound_session_random_length(sess);
    auto random = generateRandomBytes(randLen);
    int rc = olm_create_outbound_session(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        theirOneTimeKey.data(), theirOneTimeKey.size(),
        random.data(), random.size());
    if (rc == -1) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInbound(OlmAccount& account, const std::string& preKeyMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    int rc = olm_create_inbound_session(sess, acc, preKeyMessage.data(), preKeyMessage.size());
    if (rc == static_cast<size_t>(-1)) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInboundFrom(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& encryptedMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    auto* acc = static_cast<OlmAccountC*>(account.account_);
    int rc = olm_create_inbound_session_from(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        encryptedMessage.data(), encryptedMessage.size());
    if (rc == -1) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::encrypt(const std::string& plaintext) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    size_t msgLen = olm_encrypt_message_length(sess, plaintext.size());
    std::string msg(msgLen, 0);
    size_t written = olm_encrypt(sess, plaintext.data(), plaintext.size(), &msg[0], msgLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    msg.resize(written);
    result.success = true;
    result.data = msg;
    result.messageType = olm_encrypt_message_type(sess);
    return result;
}

OlmSessionResult OlmSession::decrypt(const std::string& encryptedMessage, int messageType) {
    OlmSessionResult result;
    auto* sess = static_cast<OlmSessionC*>(session_);
    size_t ptLen = olm_decrypt_max_plaintext_length(sess, messageType,
        encryptedMessage.data(), encryptedMessage.size());
    std::string pt(ptLen, 0);
    size_t written = olm_decrypt(sess, messageType,
        encryptedMessage.data(), encryptedMessage.size(), &pt[0], ptLen);
    if (written == static_cast<size_t>(-1)) {
        const char* err;
        olm_session_last_error(sess, &err);
        result.error = OlmError::BadMessageMac;
        result.data = std::string(err);
        return result;
    }
    pt.resize(written);
    result.success = true;
    result.data = pt;
    return result;
}

bool OlmSession::matchesInbound(const std::string& preKeyMessage) {
    auto* sess = static_cast<OlmSessionC*>(session_);
    return olm_matches_inbound_session(sess, preKeyMessage.data(), preKeyMessage.size()) == 1;
}

// (pickle/unpickle implementations would follow the same pattern)

// ---- Megolm Outbound ----
// (implementation stubs — follow same C API wrapping pattern)

// ---- Megolm Inbound ----
// (implementation stubs)

// ---- SAS ----
// (implementation stubs)

} // namespace progressive
