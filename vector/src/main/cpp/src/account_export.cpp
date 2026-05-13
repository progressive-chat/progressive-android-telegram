#include "progressive/account_export.hpp"
#include <sstream>
#include <cstring>
#include <vector>

namespace progressive {

static const char BASE64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const std::string& input) {
    std::string out;
    out.reserve((input.size() + 2) / 3 * 4);
    int val = 0, bits = -6;
    for (unsigned char c : (const unsigned char*)input.data()) {
        val = (val << 8) + c;
        bits += 8;
        while (bits >= 0) {
            out += BASE64_CHARS[(val >> bits) & 0x3F];
            bits -= 6;
        }
    }
    if (bits > -6) out += BASE64_CHARS[((val << 8) >> (bits + 8)) & 0x3F];
    while (out.size() % 4) out += '=';
    return out;
}

std::string base64Decode(const std::string& input) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[BASE64_CHARS[i]] = i;
    std::string out;
    out.reserve(input.size() * 3 / 4);
    int val = 0, bits = -8;
    for (unsigned char c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        bits += 6;
        if (bits >= 0) {
            out += char((val >> bits) & 0xFF);
            bits -= 8;
        }
    }
    return out;
}

std::string accountToJson(const AccountData& data) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("userId": ")" << esc(data.userId) << R"(",)";
    json << R"("accessToken": ")" << esc(data.accessToken) << R"(",)";
    json << R"("refreshToken": ")" << esc(data.refreshToken) << R"(",)";
    json << R"("homeServerUrl": ")" << esc(data.homeServerUrl) << R"(",)";
    json << R"("deviceId": ")" << esc(data.deviceId) << R"(",)";
    json << R"("deviceName": ")" << esc(data.deviceName) << R"(",)";
    json << R"("displayName": ")" << esc(data.displayName) << R"(",)";
    json << R"("avatarUrl": ")" << esc(data.avatarUrl) << R"(",)";
    json << R"("includeCache": )" << (data.includeCache ? "true" : "false");
    json << "}";
    return json.str();
}

AccountData jsonToAccount(const std::string& json) {
    AccountData data;
    auto getStr = [&](const std::string& key) -> std::string {
        auto search = '"' + key + '"';
        auto pos = json.find(search);
        if (pos == std::string::npos) return {};
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ':' || json[pos] == '\t')) ++pos;
        if (pos >= json.size()) return {};
        // Expect string value
        if (json[pos] == '"') {
            ++pos;
            auto end = json.find('"', pos);
            if (end == std::string::npos) return {};
            return json.substr(pos, end - pos);
        }
        // Boolean or number
        auto end = pos;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ') ++end;
        return json.substr(pos, end - pos);
    };

    data.userId       = getStr("userId");
    data.accessToken  = getStr("accessToken");
    data.refreshToken = getStr("refreshToken");
    data.homeServerUrl = getStr("homeServerUrl");
    data.deviceId     = getStr("deviceId");
    data.deviceName   = getStr("deviceName");
    data.displayName  = getStr("displayName");
    data.avatarUrl    = getStr("avatarUrl");
    data.includeCache = getStr("includeCache") == "true";
    return data;
}

std::string encryptAccountData(const AccountData& data, const std::string& passphrase) {
    auto json = accountToJson(data);
    // XOR with passphrase-derived key
    for (size_t i = 0; i < json.size(); ++i) {
        json[i] ^= passphrase[i % passphrase.size()];
    }
    return base64Encode(json);
}

AccountData decryptAccountData(const std::string& encrypted, const std::string& passphrase) {
    auto decoded = base64Decode(encrypted);
    if (decoded.empty()) return {};

    for (size_t i = 0; i < decoded.size(); ++i) {
        decoded[i] ^= passphrase[i % passphrase.size()];
    }

    auto data = jsonToAccount(decoded);
    // Basic validation: must have userId and accessToken
    if (data.userId.empty() || data.accessToken.empty() || data.homeServerUrl.empty()) {
        return {};
    }
    return data;
}

} // namespace progressive
