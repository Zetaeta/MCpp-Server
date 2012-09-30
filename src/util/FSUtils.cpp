
#include <errno.h>
#include <vector>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "FSUtils.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"

using std::string;
using std::vector;

namespace MCServer {

bool isDirectory(const string &file) {
    struct stat st;
    stat(file.c_str(), &st);
    return st.st_mode & S_IFDIR;
}

bool exists(const string &file) {
    struct stat st;
    if (stat(file.c_str(), &st) == 0) {
        return true;
    }
    else if (errno == ENOENT || errno == ENOTDIR) {
        return false;
    }
    else {
        MinecraftServer::server().getLogger() << "Failed stat(): " << strerror(errno) << '\n';
        return false;
    }
}

size_t fileSize(const string &file) {
    struct stat st;
    stat(file.c_str(), &st);
    return st.st_size;
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

void getEntries(const string &_directory, vector<string> &entries) {
    string directory = _directory;
    if (directory[directory.size() - 1] != '/') {
        directory += '/';
    }
    DIR *dir = opendir(directory.c_str());
    if (!dir) {
        return;
    }
    dirent *entry;
    while ( (entry = readdir(dir)) ) {
        if (strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0) {
            continue;
        }
        entries.push_back(directory + entry->d_name);
    }
    closedir(dir);
}

vector<string> getEntries(const string &directory) {
    vector<string> v;
    getEntries(directory, v);
    return v;
}

}

