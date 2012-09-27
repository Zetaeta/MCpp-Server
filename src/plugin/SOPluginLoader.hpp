
#ifndef SOPLUGINLOADER_HPP
#define SOPLUGINLOADER_HPP

#include <string>

#include "PluginLoader.hpp"

namespace MCServer {
namespace Plugin {

class Plugin;

class SOPluginLoader : public PluginLoader {
public:
    Plugin * loadPlugin(const std::string &);
    Plugin * loadCPlugin(void *handle);
    ~SOPluginLoader();
};

}
}

#endif
