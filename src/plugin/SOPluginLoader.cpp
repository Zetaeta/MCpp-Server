#define DEBUG

#include <dlfcn.h>
#include <string>

#ifdef DEBUG
#include <vector>
#endif

#include "SOPluginLoader.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"
#include "Plugin.hpp"

using std::string;
#ifdef DEBUG
using std::vector;
#endif

namespace MCServer {
namespace Plugin {
    
Plugin * SOPluginLoader::loadPlugin(const std::string &file) {
    void *handle = dlopen(file.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        char *err = dlerror();
        MinecraftServer::server().getLogger() << "Failed to load " << file << ": " << (err ? err : "Unknown error.") << '\n';
        return 0;
    }
    Plugin * (*getPlugin)() = reinterpret_cast<Plugin * (*)()>(dlsym(handle, "getPlugin"));
    if (!getPlugin) {
        return loadCPlugin(handle);
    }
    return (*getPlugin)();
}

Plugin * SOPluginLoader::loadCPlugin(void *handle) {
    return 0;
}

SOPluginLoader::~SOPluginLoader() {

}

}
}
