#include "progressive/matrix_patterns.hpp"
#include <regex>
#include <algorithm>

namespace progressive {

static const char* DOMAIN_REGEX = ":[A-Z0-9.-]+(:[0-9]{2,5})?";
static const char* BASE64 = "[0-9A-Za-z/\\\\+=]+";
static const char* BASE64_SAFE = "[0-9A-Za-z/\\\\_\\-]+";

bool isUserId(const std::string& input) {
    std::regex re(std::string("@[A-Z0-9\\x21-\\x39\\x3B-\\x7F]+") + DOMAIN_REGEX, std::regex::icase);
    return std::regex_match(input, re);
}

bool isRoomId(const std::string& input) {
    std::regex re(std::string("^!.+") + DOMAIN_REGEX + "$", std::regex::icase);
    return std::regex_match(input, re);
}

bool isRoomAlias(const std::string& input) {
    std::regex re(std::string("#[A-Z0-9._%#@=+-]+") + DOMAIN_REGEX, std::regex::icase);
    return std::regex_match(input, re);
}

bool isEventId(const std::string& input) {
    std::regex reV4(std::string("\\$") + BASE64_SAFE, std::regex::icase);
    std::regex reV3(std::string("\\$") + BASE64, std::regex::icase);
    return std::regex_match(input, reV4) || std::regex_match(input, reV3);
}

bool isGroupId(const std::string& input) {
    std::regex re(std::string("\\+[A-Z0-9=_\\-./]+") + DOMAIN_REGEX, std::regex::icase);
    return std::regex_match(input, re);
}

bool isMatrixToPermalink(const std::string& url) {
    return url.rfind("https://matrix.to/#/", 0) == 0;
}

bool isAppPermalink(const std::string& url) {
    std::regex re(R"(https://[A-Z0-9.-]+\.[A-Z]{2,}/#/(room|user)/)", std::regex::icase);
    return std::regex_search(url, re);
}

PermalinkInfo parseMatrixToPermalink(const std::string& url) {
    PermalinkInfo info;

    if (!isMatrixToPermalink(url)) return info;

    auto rest = url.substr(std::string("https://matrix.to/#/").size());

    if (rest.rfind('@', 0) == 0) {
        info.type = "user";
        info.userId = rest;
        info.valid = true;
    } else if (rest.rfind('#', 0) == 0) {
        info.type = "room";
        info.roomId = rest;
        info.valid = true;
    } else if (rest.rfind('!', 0) == 0) {
        auto dollar = rest.find('/');
        if (dollar != std::string::npos && dollar + 1 < rest.size() && rest[dollar + 1] == '$') {
            info.type = "event";
            info.roomId = rest.substr(0, dollar);
            info.eventId = rest.substr(dollar + 1);
            info.valid = true;
        } else {
            info.type = "room";
            info.roomId = rest;
            info.valid = true;
        }
    }

    return info;
}

ExtractedIds extractMatrixIds(const std::string& text) {
    ExtractedIds ids;

    // User IDs: @xxx:domain
    std::regex userRe(std::string("@[A-Z0-9\\x21-\\x39\\x3B-\\x7F]+") + DOMAIN_REGEX, std::regex::icase);
    for (auto it = std::sregex_iterator(text.begin(), text.end(), userRe); it != std::sregex_iterator(); ++it)
        ids.userIds.push_back(it->str());

    // Room IDs: !xxx:domain
    std::regex roomRe(std::string("![A-Z0-9._%#@=+-]+") + DOMAIN_REGEX, std::regex::icase);
    for (auto it = std::sregex_iterator(text.begin(), text.end(), roomRe); it != std::sregex_iterator(); ++it)
        ids.roomIds.push_back(it->str());

    // Room aliases: #xxx:domain
    std::regex aliasRe(std::string("#[A-Z0-9._%#@=+-]+") + DOMAIN_REGEX, std::regex::icase);
    for (auto it = std::sregex_iterator(text.begin(), text.end(), aliasRe); it != std::sregex_iterator(); ++it)
        ids.roomAliases.push_back(it->str());

    // Event IDs: $xxx
    std::regex eventRe(std::string("\\$") + BASE64_SAFE, std::regex::icase);
    for (auto it = std::sregex_iterator(text.begin(), text.end(), eventRe); it != std::sregex_iterator(); ++it)
        ids.eventIds.push_back(it->str());

    return ids;
}

bool isMxcUrl(const std::string& url) {
    return url.rfind("mxc://", 0) == 0 && url.size() > 6;
}

bool isPhoneNumber(const std::string& input) {
    // E.164-ish: +1234567890 (7-15 digits)
    std::regex re(R"(^\+\d{7,15}$)");
    return std::regex_match(input, re);
}

bool isValidEmail(const std::string& input) {
    std::regex re(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(input, re);
}

} // namespace progressive
