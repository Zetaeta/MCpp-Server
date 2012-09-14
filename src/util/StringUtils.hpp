
#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>

namespace MCServer {

bool endsWith(const std::string &string, const std::string &ending);
std::string toLower(const std::string &string);

}

#endif
