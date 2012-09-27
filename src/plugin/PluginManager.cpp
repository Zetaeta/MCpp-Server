
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <map>
#include <vector>

#include "PluginManager.hpp"
#include "MinecraftServer.hpp"
#include "util/FSUtils.hpp"
#include "logging/Logger.hpp"
#include "util/StringUtils.hpp"
#include "SOPluginLoader.hpp"
#include "Plugin.hpp"

namespace MCServer {
namespace Plugin {

using std::string;
using std::map;
using std::vector;

struct PluginManagerData {
    MinecraftServer *server;
    map<PluginLoaderRule, PluginLoader *> rules;
};

constexpr char _so[] = ".so";

PluginManager::PluginManager(MinecraftServer *server)
:m(new PluginManagerData) {
    m->server = server;

    registerRule(endsWithTemplate<_so>, new SOPluginLoader);
}

PluginManager::~PluginManager() {
    delete m;
}

void PluginManager::registerRule(PluginLoaderRule rule, PluginLoader *loader) {
    m->rules[rule] = loader;
}

int PluginManager::loadPlugins(const std::string &directory) {
    MinecraftServer::getServer().getLogger() << "Loading plugins in " << directory << '\n';
    int loaded = 0;
/*    DIR *pluginDir = opendir(directory.c_str());
    dirent *currentEntry;
    while (currentEntry = readdir(pluginDir)) {
        if (!isDirectory(currentEntry->d_name) && endsWith(currentEntry->d_name, ".so")) {
            if (loadPlugin(directory + currentEntry->d_name)) {
                ++loaded;
            }
        }
    } */
    vector<string> pluginFiles;
    getEntries(directory, pluginFiles);
    for (auto it = pluginFiles.begin(); it != pluginFiles.end(); ++it) {
        MinecraftServer::getServer().getLogger() << "Attempting to load " << *it << '\n';
        if (loadPlugin(*it)) {
            ++loaded;
        }
    }
    return loaded;
}

bool PluginManager::loadPlugin(const std::string &file) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
/*    log << "Loading plugin " << file << '\n';
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
    } */

    Plugin *plugin;
    #ifdef DEBUG
    vector<string> rulesTried;
    #endif
    for (auto it = m->rules.begin(); it != m->rules.end(); ++it) {
        if ((*it->first)(file.c_str())) {
            #ifdef DEBUG
            rulesTried.push_back(it->second->getName());
            #endif
            if (plugin = it->second->loadPlugin(file)) {
                log << "Succeeded loading " << file << '\n';
                break;
            }
        }
    }
    if (!plugin) {
        Logging::Logger &log = MinecraftServer::server().getLogger();
        log << "Failed to load plugin in file " << file << ": could not find a suitable loading rule.";
        #ifdef DEBUG
        log << "Rules tried: ";
        for (int i=0; i<rulesTried.size()-1; ++i) {
            log << rulesTried[i] << ", ";
        }
        log << *(rulesTried.end() - 1)
        #endif
        log << '\n';
        return 0;
    }
    plugin->load();
}

}
}


