
#include <algorithm>

#include "StringUtils.hpp"

using std::string;

namespace MCServer {

bool endsWith(const string &str, const string &ending) {
    if (str.size() > ending.size()) {
        return str.compare(str.size() - ending.size(), ending.size(), ending) == 0;
    }
    else {
        return false;
    }
}

string toLower(const std::string &str) {
    string lower(str);
    std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
    return lower;
}


}

