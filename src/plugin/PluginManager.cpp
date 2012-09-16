
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#include "PluginManager.hpp"
#include "MinecraftServer.hpp"
#include "util/FSUtils.hpp"
#include "logging/Logger.hpp"
#include "util/StringUtils.hpp"

namespace MCServer {
namespace Plugins {

using std::string;

struct PluginManagerData {
    MinecraftServer *server;
};

PluginManager::PluginManager(MinecraftServer *server)
:m(new PluginManagerData) {
    m->server = server;
}

PluginManager::~PluginManager() {
    delete m;
}

int PluginManager::loadPlugins(const std::string &directory) {
    MinecraftServer::getServer().getLogger() << "Loading plugins in " << directory << '\n';
    int loaded = 0;
    DIR *pluginDir = opendir(directory.c_str());
    dirent *currentEntry;
    while (currentEntry = readdir(pluginDir)) {
        if (!isDirectory(currentEntry->d_name) && endsWith(currentEntry->d_name, ".so")) {
            if (loadPlugin(directory + currentEntry->d_name)) {
                ++loaded;
            }
        }
    }
    return loaded;
}

bool PluginManager::loadPlugin(const std::string &file) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    log << "Loading plugin " << file << '\n';
    void *handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        char *err = dlerror();
        log << "Failed to load " << file << ", dlerror = " << (err ? err : "") << '\n';
    }
    void (*loadPlugin)() = reinterpret_cast<void (*)()>(dlsym(handle, "loadPlugin"));
    if (loadPlugin) {
        log << "loadPlugin found! Loading plugin!\n";
        (*loadPlugin)();
    }
    else {
        log << "Could not find loadPlugin!\n";
        const char *err = dlerror();
        if (err) {
            log << "dlerror: " << err << '\n';
        }
    }
}

}
}


