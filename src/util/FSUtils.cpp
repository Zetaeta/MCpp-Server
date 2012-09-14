
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "FSUtils.hpp"

namespace MCServer {

bool isDirectory(const std::string &file) {
    struct stat st;
    stat(file.c_str(), &st);
    return st.st_mode & S_IFDIR;
}

bool exists(const std::string &file) {
    struct stat st;
    return !(stat(file.c_str(), &st) == -1 && errno == ENOENT);
}

}

