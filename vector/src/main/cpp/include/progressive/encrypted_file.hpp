#ifndef PROGRESSIVE_ENCRYPTED_FILE_HPP
#define PROGRESSIVE_ENCRYPTED_FILE_HPP

#include <string>
#include <vector>
#include <map>

namespace progressive {

// ---- Encrypted File Parser (Matrix E2EE Attachments) ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.crypto.model.EncryptedFileKey.kt (80 lines)
//   org.matrix.android.sdk.api.session.crypto.model.EncryptedFileInfo.kt (84 lines)
//
// Matrix spec for encrypted attachments:
//   https://matrix.org/docs/spec/client_server/latest#sending-encrypted-attachments
//
// The encrypted file JSON format:
//   {"v":"v2","key":{"alg":"A256CTR","ext":true,"k":"base64key",...},"iv":"base64iv","hashes":{"sha256":"..."},"url":"mxc://..."}

struct EncryptedFileKey {
    std::string alg;            // Must be "A256CTR"
    bool ext = false;           // Must be true (W3C extension)
    std::vector<std::string> keyOps; // Must contain "encrypt" and "decrypt"
    std::string kty;            // Must be "oct"
    std::string k;              // The key, base64-encoded
    bool valid = false;

    // Check if this JWK is valid per Matrix spec.
    // Original Kotlin (EncryptedFileKey.kt:isValid):
    //   alg == "A256CTR" && ext == true && keyOps contains encrypt+decrypt
    //   && kty == "oct" && k is not blank
    bool isValid();
};

struct EncryptedFileInfo {
    std::string url;                     // MXC URL to the encrypted file
    EncryptedFileKey key;                // JWK key object
    std::string iv;                      // Initialisation Vector (base64)
    std::map<std::string, std::string> hashes; // Hash map, must contain "sha256"
    std::string version;                 // Must be "v2"
    bool valid = false;

    // Check if this encrypted file descriptor is valid.
    // Original Kotlin (EncryptedFileInfo.kt:isValid):
    //   url not blank, key.isValid(), iv not blank,
    //   hashes contains "sha256", v == "v2"
    bool isValid();
};

// Parse an EncryptedFileKey from a JSON object.
EncryptedFileKey parseEncryptedFileKey(const std::string& json);

// Parse an EncryptedFileInfo from JSON.
EncryptedFileInfo parseEncryptedFileInfo(const std::string& json);

// Validate a JWK key per Matrix spec requirements.
bool isValidJwkKey(const EncryptedFileKey& key);

// Validate an encrypted file descriptor.
bool isValidEncryptedFile(const EncryptedFileInfo& info);

// Extract the base64-encoded key from a JWK.
std::string extractFileKey(const EncryptedFileKey& key);

// Extract the base64-encoded IV from encrypted file info.
std::string extractFileIv(const EncryptedFileInfo& info);

// Format as JSON for Kotlin UI.
std::string encryptedFileKeyToJson(const EncryptedFileKey& key);
std::string encryptedFileInfoToJson(const EncryptedFileInfo& info);

} // namespace progressive

#endif // PROGRESSIVE_ENCRYPTED_FILE_HPP
