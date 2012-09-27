
#ifndef PLUGINMANAGER_HPP
#define PLUGINMANAGER_HPP

#include <string>

namespace MCServer {

class MinecraftServer;

namespace Plugin {

typedef bool (*PluginLoaderRule)(const char *);

class PluginLoader;

struct PluginManagerData;

class PluginManager {
public:
    PluginManager(MinecraftServer *server);
    ~PluginManager();

    void registerRule(PluginLoaderRule, PluginLoader *);

    int loadPlugins(const std::string &directory);
    bool loadPlugin(const std::string &file);
private:
    PluginManagerData *m;
};

// Rule to decide which PluginLoader loads a plugin.

}
}

#endif
