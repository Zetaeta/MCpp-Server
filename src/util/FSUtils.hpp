
#ifndef FSUTILS_HPP
#define FSUTILS_HPP

#include <string>

namespace MCServer {

bool isDirectory(const std::string &file);
bool exists(const std::string &file);

std::string findFile(const std::string &fileName);

}

#endif
