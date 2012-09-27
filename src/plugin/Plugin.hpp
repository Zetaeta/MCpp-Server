
#ifndef PLUGIN_HPP
#define PLUGIN_HPP

namespace MCServer {
namespace Plugin {

class Plugin {
public:
    virtual void load() = 0;
    virtual ~Plugin() = 0;
};

}
}

#endif
