
#ifndef PLUGINLOADER_HPP
#define PLUGINLOADER_HPP

#include <string>

namespace MCServer {
namespace Plugin {

class Plugin;

class PluginLoader {
public:
    virtual Plugin * loadPlugin(const std::string &) = 0;
    virtual ~PluginLoader() = 0;
};

}
}

#endif
