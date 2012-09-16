
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "FSUtils.hpp"

using std::string;

namespace MCServer {

bool isDirectory(const string &file) {
    struct stat st;
    stat(file.c_str(), &st);
    return st.st_mode & S_IFDIR;
}

bool exists(const string &file) {
    struct stat st;
    return !(stat(file.c_str(), &st) == -1 && errno == ENOENT);
}

string findFile(const string &fileName) {
    if (exists(fileName)) {
        return fileName;
    }
    string path;
    if (exists(path = "/usr/share/mcpp-server/" + fileName)) {
        return path;
    }
    else {
        return "";
    }
}

}

