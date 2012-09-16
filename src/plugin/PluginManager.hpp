
#ifndef PLUGINMANAGER_HPP
#define PLUGINMANAGER_HPP

#include <string>

namespace MCServer {

class MinecraftServer;

namespace Plugins {

struct PluginManagerData;

class PluginManager {
public:
    PluginManager(MinecraftServer *server);
    ~PluginManager();

    int loadPlugins(const std::string &directory);
    bool loadPlugin(const std::string &file);
private:
    PluginManagerData *m;
};


}
}

#endif
