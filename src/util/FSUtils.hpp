
#ifndef FSUTILS_HPP
#define FSUTILS_HPP

#include <string>
#include <vector>

namespace MCServer {

bool isDirectory(const std::string &file);
bool exists(const std::string &file);
size_t fileSize(const std::string &file);

std::string findFile(const std::string &fileName);

void getEntries(const std::string &directory, std::vector<std::string> &);
std::vector<std::string> getEntries(const std::string &directory);

}

#endif
